#ifndef _Func_h_
#define _Func_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void Rtrim(char* sstring);
void Ltrim(char* sstring);

//�Ƚ�str1��str2�����ַ��������Ƿ�һ�������Դ�Сд�ıȽ�
bool StrcaseCmp(const char* str1, const char* str2);

//��strȫ����ɴ�д
void StringCase(char* str);

#endif // !_Func_h_
