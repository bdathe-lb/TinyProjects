/**
 * @file tiny-shell.c
 * @brief 一个简单的 TinyShell 实现
 *
 * 功能:
 *   - 提供交互式命令行
 *   - 支持内置命令 (cd, help, exit)
 *   - 调用 execvp 执行外部程序
 *
 * 限制:
 *   - 暂不支持管道、重定向
 *   - 没有信号处理
 *
 * 依赖:
 *   - GNU readline 库
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define RESET "\033[0m"
#define GREEN "\033[1;32m"
#define RED   "\033[1;31m"
#define BLUE  "\033[1;34m"

#define TOKEN_BUFSIZE 64
#define TOKEN_SPLIT " \n\t\r"
#define PATH_BUFSIZE 256

#define SHELL_CONTINUE 0
#define SHELL_ERROR    1
#define SHELL_EXIT    -1

#define ARRLEN(array) (sizeof(array) / sizeof(array[0])) 

/// 内置命令定义
int builtin_cd(char **args);
int builtin_help(char **args);
int builtin_exit(char **args);

/// 命令名数组
char *builtin_cmds[] = {
  "cd",
  "help",
  "exit"
};

/// 命令函数映射数组
int (*builtin_funcs[])(char **) = {
  &builtin_cd,
  &builtin_help,
  &builtin_exit
};


/**
 * @brief 切换当前工作目录
 *
 * @param args 参数数组 (args[1] 应为路径)
 * @return SHELL_CONTINUE 成功或参数错误
 * @return SHELL_ERROR    chdir 失败
 */
int builtin_cd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, RED"usage: cd [path]\n"RESET);
    return SHELL_CONTINUE;
  }

  char *path = args[1];
  if (chdir(path) == -1) {
    fprintf(stderr, RED"sh: chdir failed!\n"RESET);
    return SHELL_ERROR;
  }

  return SHELL_CONTINUE;
}

/**
 * @brief 显示内置命令列表
 *
 * @param args 参数数组 (未使用)
 * @return SHELL_CONTINUE
 */
int builtin_help(char **args) {
  printf(BLUE"TinyShell built-in commands:\n"RESET);

  int bulitin_cmd_num = ARRLEN(builtin_cmds);
  for (int i = 0; i < bulitin_cmd_num; ++ i) {
    printf(BLUE"\t%s\n"RESET, builtin_cmds[i]);
  }

  return SHELL_CONTINUE;
}


/**
 * @brief 退出 TinyShell
 *
 * @param args 参数数组 (未使用)
 * @return SHELL_EXIT
 */
int builtin_exit(char **args) {
  return SHELL_EXIT;
}

/**
 * @brief 解析用户输入的命令行
 *
 * 功能:
 *   - 按空格/Tab/换行符切分输入
 *   - 动态分配 tokens 数组并自动扩展
 *   - 返回一个以 NULL 结尾的参数列表
 *
 * 内存管理:
 *   - 返回的数组由调用者负责 free()
 *
 * @param line 原始输入行 (由 readline 返回)
 * @return 参数数组，失败时返回 NULL
 */
char **parse_command(char *line) {
  if (!line || !*line) return NULL;
  
  int token_bufsize = TOKEN_BUFSIZE;
  char **tokens = malloc(token_bufsize * sizeof(char *));
  char *token;
  int token_cnt = 0;

  // 分割出命令参数
  token = strtok(line, TOKEN_SPLIT);
  while (token != NULL) {
    tokens[token_cnt ++] = token;
    token = strtok(NULL, TOKEN_SPLIT); 

    if (token_cnt >= token_bufsize) {
      token_bufsize = token_bufsize * 2;
      char **tmp = realloc(tokens, token_bufsize * sizeof(char *));
      if (!tmp) {
        fprintf(stderr, RED"sh: realloc failed!\n"RESET);
        free(tokens);
        exit(SHELL_ERROR);
      }
      tokens = tmp;
    }
  }
  tokens[token_cnt] = NULL;

  return tokens; 
}

/**
 * @brief 执行用户输入的命令
 *
 * 流程:
 *   1. 检查是否为内置命令 (cd/help/exit)
 *   2. 如果不是，fork 子进程
 *   3. 子进程: 调用 execvp 执行外部命令
 *   4. 父进程: 使用 waitpid 等待子进程结束
 *
 * @param args 参数数组 (args[0] 为命令名)
 * @return SHELL_CONTINUE 继续循环
 * @return SHELL_EXIT     退出主循环
 */
int execute_command(char **args) {
  if (!args || !*args) return SHELL_CONTINUE;
  
  int bulitin_cmd_num = ARRLEN(builtin_cmds);
  for (int i = 0; i < bulitin_cmd_num; ++ i) {
    if (strcmp(args[0], builtin_cmds[i]) == 0) {
      return builtin_funcs[i](args);
    }
  }

  int pid = fork();

  if (pid == 0) {
    execvp(args[0], args);
    fprintf(stderr, RED"sh: execvp failed!\n"RESET);  // execvp 执行失败，会执行这一句
    exit(SHELL_ERROR);
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
  } else { 
    fprintf(stderr, RED"sh: fork failed!\n"RESET);
    return SHELL_CONTINUE;
  }

  return SHELL_CONTINUE;
}

/**
 * @brief Shell 主循环
 *
 * 功能:
 *   - 获取当前工作目录作为提示符
 *   - 读取用户输入 (readline)
 *   - 解析并执行命令
 *   - 根据返回值决定是否退出
 */
void loop() {
  char *line;
  char **args;
  int status;

  do {
    char path[PATH_BUFSIZE];
    // 读取当前路径作为提示符 (如果失败则显示 "?")
    if (!getcwd(path, sizeof(path))) {
      fprintf(stderr, RED"sh: getcwd failed!\n"RESET);
      strcpy(path, "?");
    }

    char prompt[PATH_BUFSIZE + 10];
    snprintf(prompt, sizeof(prompt), "\001"GREEN"\002""%.200s ➜ ""\001"RESET"\002", path);

    line = readline(prompt);
    if (line && *line) {
      add_history(line);
    }

    args = parse_command(line);
    status = execute_command(args);

    if (status == SHELL_EXIT) {
      free(line);
      free(args);
      break;
    }

    free(line);
    free(args);
  } while (1);

  clear_history();
}

/**
 * @brief 程序入口
 */
int main(int argc, char *argv[]) {
  printf(GREEN"Welcome TinyShell!\n"RESET);
  
  loop();
  
  return 0;
}