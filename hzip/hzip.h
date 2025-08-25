#ifndef __HZIP_H__
#define __HZIP_H__

#include <stddef.h>
#include <stdint.h>
#include "huffman_tree.h"

#define IN_BUFFER_SIZE  4096
#define OUT_BUFFER_SIZE 1024

#define RESET   "\033[0m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"

/// 全局变量声明
extern int freq[256];  // 频率表
extern uint64_t original_byte_size;  // 原始文件的字节数 

/// 主要函数
bool compress(char *input_filename, char *output_filename, bool keep);
bool decompress(char *input_filename, char *output_filename, bool keep);

/// 辅助函数
void count_byte_freq(char *filename);
void compute_compression_ratio(char *orignal_filename, char *compressed_filename);
void show_progress(uint64_t done, uint64_t total);

#endif  // __HZIP_H__
