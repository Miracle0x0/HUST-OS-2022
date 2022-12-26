/*
 * header file to be used by applications.
 */

int printu(const char *s, ...);
int exit(int code);

// $ added @lab1_challenge1

// 打印用户进程栈函数原型
int print_backtrace(int depth);