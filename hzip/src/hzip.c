#include "hzip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/// 全局变量初始化
int freq[256] = { 0 };  // 频率表
uint64_t original_byte_size = 0;  // 原始文件的字节数 

/**
 * @brief 压缩文件
 * @param input_filename 待压缩的文件名 
 * @param output_filename 输出文件
 * @param keep 是否保留原文件
 * @return false 压缩失败
 * @return true 压缩成功
 */
bool compress(char *input_filename, char *output_filename, bool keep) {
  /// 处理输入文件，生成频率表
  count_byte_freq(input_filename);
  /// 构建哈夫曼树
  Node *root = build_huffman_tree(freq);
  /// 构建哈夫曼编码表
  char *codes[256] = { NULL };
  char path[MAX_DEPTH];
  build_huffman_codes(root, path, 0, codes);
  
  bool out_malloced = false;  // 如果为 output_filename == NULL，要分配内存并手动释放，否则不用
  // 构造输出文件名
  if (output_filename == NULL) {
    output_filename = malloc(strlen(input_filename) + 5);
    if (!output_filename) {
      free_huffman_tree(root);
      free_huffman_codes(codes);
      
      perror("malloc");
      return false;
    }

    out_malloced = true;

    if (sprintf(output_filename, "%s.hz", input_filename) < 0) {
      free(output_filename);

      fprintf(stderr, "hzip.c: sprintf() is failed!\n");
      return false;
    } 
  }

  // 打开输入和输出文件
  FILE *in = fopen(input_filename, "rb");
  FILE *out = fopen(output_filename, "wb");
  if (!in) {
    free_huffman_tree(root);
    free_huffman_codes(codes);
    if (out_malloced) { free(output_filename); }

    perror("fopen");
    return false;
  }

  if (!out) {
    free_huffman_tree(root);
    free_huffman_codes(codes);
    if (out_malloced) { free(output_filename); }
    fclose(in);
    
    perror("fopen");
    return false;
  }
  
  /// 写文件头
  if (fwrite(&original_byte_size, sizeof(original_byte_size), 1, out) != 1 ||
      fwrite(freq, sizeof(freq[0]), 256, out) != 256) {
    free_huffman_tree(root);
    free_huffman_codes(codes);
    if (out_malloced) { free(output_filename); }
    fclose(in);
    fclose(out);

    perror("fwrite");
    return false;
  }

  /// 压缩数据
  unsigned char in_buffer[IN_BUFFER_SIZE];  // 输入缓冲区
  unsigned char out_buffer[OUT_BUFFER_SIZE];  // 输出缓冲区
  unsigned char out_byte = 0;
  int out_buffer_count = 0;
  int bit_count = 0;
  uint64_t done_bytes = 0;  // 用于进度条，已完成的字节数量
  uint64_t total_bytes = original_byte_size;  // 用于进度条，总的字节数量
  
  uint64_t n;
  while ((n = fread(in_buffer, 1, sizeof(in_buffer), in)) > 0) {    
    for (int i = 0; i < n; ++ i) {
      char *code = codes[in_buffer[i]];
      // 更新进度条
      done_bytes += 1;
      show_progress(done_bytes, total_bytes);

      while (*code != '\0') {
        int bit = (*code == '1') ? 1 : 0;
        out_byte |= (bit << (7 - bit_count));
        bit_count ++;
        code ++;

        // 满一个字节，写入缓冲区
        if (bit_count == 8) {
          out_buffer[out_buffer_count ++] = out_byte;
          out_byte = 0;
          bit_count = 0;
        }

        // 缓冲区满，写入文件
        if (out_buffer_count == OUT_BUFFER_SIZE) {
          if (fwrite(out_buffer, 1, out_buffer_count, out) != out_buffer_count) {
            free_huffman_tree(root);
            free_huffman_codes(codes);
            if (out_malloced) { free(output_filename); }
            fclose(in);
            fclose(out);

            perror("fwrite");
            return false;
          }

          out_buffer_count = 0;
        }
      }
    }
  }

  // 先处理残余 bit（不满一个字节的情况）
  if (bit_count > 0) {
    out_buffer[out_buffer_count ++] = out_byte;
    out_byte = 0;
    bit_count = 0;
  }

  // 把缓冲区里剩下的都写出去
  if (out_buffer_count > 0) {
    if (fwrite(out_buffer, 1, out_buffer_count, out) != out_buffer_count) {
      free_huffman_tree(root);
      free_huffman_codes(codes);
      if (out_malloced) { free(output_filename); }
      fclose(in);
      fclose(out);
      
      perror("fwrite");
      return false;
    }
  }

  /// 释放空间
  fclose(in);
  fclose(out);
  free_huffman_tree(root);
  free_huffman_codes(codes);
  
  /// 计算压缩比
  compute_compression_ratio(input_filename, output_filename);

  if (out_malloced) { free(output_filename); }

  /// 判断是否保留原文件
  if (!keep) {
    if (remove(input_filename) == -1) {
      perror("remove");
      return false;
    }
  }

  return true;
}

/**
 * @brief 解压文件
 * @param intput_filename 待解压的文件名
 * @param output_filename 输出文件
 * @param keep 是否保留原文件
 * @return false 解压失败
 * @return true 解压成功
 */
bool decompress(char *intput_filename, char *output_filename, bool keep) {
  uint64_t original_byte_size = 0;
  int freq[256] = { 0 };

  /// 构建输出文件名
  bool out_malloced = false;  // 如果为 output_filename == NULL，要分配内存并手动释放，否则不用
  if (output_filename == NULL) {
    int len = strlen(intput_filename);
    output_filename = malloc(len + 1);
    if (!output_filename) {
      perror("malloc");
      return false;
    }

    out_malloced = true;

    strcpy(output_filename, intput_filename);
    if (strlen(output_filename) > 3 && strcmp(output_filename + len - 3, ".hz") == 0) {
      output_filename[len - 3] = '\0';
    }
  }
  
  /// 打开输入和输出文件
  FILE *in = fopen(intput_filename, "rb");
  FILE *out = fopen(output_filename, "wb");
  if (!in) {
    if (out_malloced) { free(output_filename); }

    perror("fopen");
    return false;
  }

  if (!out) {
    if (out_malloced) { free(output_filename); }
    fclose(in);

    perror("fopen");
    return false;
  }

  /// 读取文件头
  if (fread(&original_byte_size, sizeof(original_byte_size), 1, in) != 1 ||
      fread(freq, sizeof(freq[0]), 256, in) != 256) {
    if (out_malloced) { free(output_filename); }
    fclose(in);
    fclose(out);

    perror("fread");
    return false;
  }
  
  /// 构建哈夫曼树
  Node *root = build_huffman_tree(freq);
  
  /// 解码数据
  unsigned char in_buffer[IN_BUFFER_SIZE];  // 输入缓冲区
  unsigned char out_buffer[OUT_BUFFER_SIZE];  // 输出缓冲区
  int out_buffer_count = 0;
  unsigned char in_byte;
  bool done = false;
  Node *cur = root;  // 用于遍历哈夫曼树
  uint64_t remaining_bytes_size = original_byte_size;  // 剩余字节数量
  uint64_t done_bytes = 0;  // 用于进度条，已处理的字节数
  uint64_t total_bytes = original_byte_size;  // 用于进度条，总的字节数

  uint64_t n;
  while ((n = fread(in_buffer, sizeof(in_buffer[0]), IN_BUFFER_SIZE, in)) > 0) {
    for (int i = 0; i < n; ++ i) {
      in_byte = in_buffer[i];
      for (int j = 7; j >= 0; -- j) {
        int bit = (in_byte >> j) & 1;  // 取出 bit 
        if (bit == 0) cur = cur->left;
        if (bit == 1) cur = cur->right;

        // 到达叶子节点，记录字节内容
        if (cur->left == NULL && cur->right == NULL) {
          remaining_bytes_size -= 1;
          // 更新进度条
          done_bytes += 1;
          show_progress(done_bytes, total_bytes);
          out_buffer[out_buffer_count ++] = (unsigned char)cur->byte;
          cur = root;

          // 特殊处理最后一个字节（可能存在 bit 填充）
          if (remaining_bytes_size == 0) { done = true; break; }
        }

        // 输入缓冲区满，写入文件
        if (out_buffer_count == OUT_BUFFER_SIZE) {
          if (fwrite(out_buffer, sizeof(out_buffer[0]), out_buffer_count, out) != out_buffer_count) {
            if (out_malloced) { free(output_filename); }
            fclose(in);
            fclose(out);
            free_huffman_tree(root);
            
            perror("fwrite");
            return false;
          }

          out_buffer_count = 0;
        }
      }

      // 结束标志（remaining_byte_size == 0）
      if (done) break;
    }
  }

  /// 输出缓冲区剩余内容写入文件
  if (out_buffer_count > 0) {
    if (fwrite(out_buffer, 1, out_buffer_count, out) != out_buffer_count) {
      if (out_malloced) { free(output_filename); }
      fclose(in);
      fclose(out);
      free_huffman_tree(root);

      perror("fwrite");
      return false;
    }
  }

  /// 释放资源
  if (out_malloced) { free(output_filename); }
  fclose(in);
  fclose(out);
  free_huffman_tree(root);

  /// 判断是否删除原文件
  if (!keep) {
    if (remove(intput_filename) == -1) {
      perror("remove");
      return false;
    }
  }

  return true;
}

/**
 * @brief 构建频率表
 * @param filename 输入文件名称
 */
void count_byte_freq(char *filename) {
  FILE *fp = fopen(filename, "rb");  
  if (!fp) {
    perror("fopen");
    return;
  }

  unsigned char in_buffer[IN_BUFFER_SIZE];

  uint64_t n;
  while ((n = fread(in_buffer, 1, sizeof(in_buffer), fp)) > 0) {
    original_byte_size += n;
    for (int i = 0; i < n; ++ i) {
      freq[in_buffer[i]] ++;
    }
  }
 
  return;
}

/**
 * @brief 计算压缩比
 * @param orignal_filename 原始文件 
 * @param compressed_filename 压缩后的文件
 */
void compute_compression_ratio(char *orignal_filename, char *compressed_filename) {
  size_t orignal_size;
  size_t compressed_size;
  
  struct stat st;
  if (stat(orignal_filename, &st) == -1) {
    perror("stat");
    return;
  }
  orignal_size = st.st_size;
  
  if (stat(compressed_filename, &st) == -1) {
    perror("stat");
    return;
  }

  compressed_size = st.st_size - (sizeof(int) * 256) - sizeof(uint64_t);

  double ratio = (double)compressed_size / (double)orignal_size;

  printf("\n");
  printf(GREEN"Original size: "RESET BLUE"%lu bytes\n"RESET, orignal_size);
  printf(GREEN"Compressed size: "RESET BLUE"%lu bytes\n"RESET, compressed_size);
  printf(GREEN"Compression ratio: "RESET BLUE"%.2f\n"RESET, ratio);
}

/**
 * @brief 显示压缩进度
 * @param done 已完成的字节数
 * @param total 总共的字节数
 */
void show_progress(uint64_t done, uint64_t total) {
  int bar_width = 50;  // 进度条长度
  int pos = done * bar_width / total;  // 在 50 个字符宽的进度条中，显示已完成的字符数
  int percent = done * 100 / total;  // 显示的百分比

  printf("\r[");  // '\r' 表示回车，光标回到行首，每次覆盖之前输出
  for (int i = 0; i < bar_width; ++ i) {
    if (i < pos) printf("=");
    else if (i == pos) printf(">");
    else printf(" ");
  }

  printf("] %d%%", percent);
  fflush(stdout);  // 强制把缓冲区的内容立即输出到终端
}