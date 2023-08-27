#ifndef UTIL_H
#define UTIL_H

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define UUID_CHAR_KIND 62
#define UUID_LIMITED 839299365868340224ULL

// maxからminまでの乱数を生成する(int)
int randBetween(int max, int min);

// maxからminまでの乱数を生成する(ull)
unsigned long long randULL(unsigned long long max, unsigned long long min);

// 0-61を大小英字および数字に変換する
char convertChar(char number);

// ランダムに大小英字および数字を返す
char randChar();

// randULL()を使用して、uuidを作成する
void generateUUID(char *uuid);

void copyUuid(char **copy, char **copied);

void scanEnv(char host[15], int *port, char user[10], char pin[20]);

void printb(char *out, int size, unsigned int v);

int powInt(int a, int b);

#endif