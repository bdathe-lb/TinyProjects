#include <stdio.h>
#include <getopt.h>
#include "hzip.h"

void help() {
  printf("hzip - Simple Huffman compression/decompression tool\n\n");
  printf("Usage:\n");
  printf("  hzip [options]\n\n");
  printf("Options:\n");
  printf("  -f, --file <filename>       Specify the file to compress or decompress\n");
  printf("  -o, --output <filename>     Specify the output file name\n");
  printf("  -k, --keep                  Keep the original file after compression/decompression\n");
  printf("  -c, --compress              Compress the file\n");
  printf("  -d, --decompress            Decompress the file\n");
  printf("  -h, --help                  Show this help message\n");
  printf("  -v, --version               Show program version information\n\n");
  printf("Examples:\n");
  printf("  Compress a file:   hzip -f test.txt -c\n");
  printf("  Decompress a file: hzip -f test.txt.hz -o test.txt -d\n");
  printf("Note:\n");
  printf("  -c/-d should be placed at the end of the options\n");
}

void version() {
  printf("hzip version 1.0.0\n");
  printf("Author: Bdathe\n");
  printf("A simple Huffman compression/decompression tool.\n");
}

int main(int argc, char *argv[]) {
  char *input_filename = NULL;  // 待压缩/解压的文件
  char *output_filename = NULL;  // 输出文件
  bool keep = false;    // 是否保留原文件
  bool do_compress = false;
  bool do_decompress = false;

  struct option long_options[] = {
    { "file",       required_argument, 0, 'f' },
    { "output",     required_argument, 0, 'o' },
    { "keep",       no_argument,       0, 'k' },
    { "compress",   no_argument,       0, 'c' },
    { "decompress", no_argument,       0, 'd' },
    { "help",       no_argument,       0, 'h' },
    { "version",    no_argument,       0, 'v' },
    { 0,            0,                 0,  0  }
  };
  const char *short_options = "f:o:kcdhv";

  int opt;
  int optidx = 0;
  while ((opt = getopt_long(argc, argv, short_options, long_options, &optidx)) != -1) {
    switch (opt) {
      case 'f': input_filename = optarg;                           break;
      case 'o': output_filename = optarg;                          break;
      case 'k': keep = true;                                       break;
      case 'c': do_compress = true;                                break;
      case 'd': do_decompress = true;                              break;
      case 'h': help();                                            break;
      case 'v': version();                                         break;
      default:  help();                                            return 1;
    }
  }

  if (do_compress) { compress(input_filename, output_filename, keep); }
  if (do_decompress) { decompress(input_filename, output_filename, keep); }

  return 0;
}