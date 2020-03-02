#ifndef _Func_h_
#define _Func_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void Rtrim(char* sstring);
void Ltrim(char* sstring);

//比较str1和str2两个字符串内容是否一样，忽略大小写的比较
bool StrcaseCmp(const char* str1, const char* str2);

//把str全都变成大写
void StringCase(char* str);

#endif // !_Func_h_
