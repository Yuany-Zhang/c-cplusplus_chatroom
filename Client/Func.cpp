#include "Func.h"
#define  _CRT_SECURE_NO_WARNINGS

//��ȡ�ַ���β���ո�
void Rtrim(char *sstring)
{
	size_t len = 0;
	if (sstring == NULL)
		return;
	len = strlen(sstring);
	while (len > 0 && sstring[len - 1] == ' ')//λ�û�һ��
		sstring[--len] = 0;
}

//��ȡ�ַ����ײ��ո�
void Ltrim(char *sstring)
{
	size_t len = 0;
	if (sstring == NULL)
		return;
	char *p_tmp = sstring;  //��ʱp_tmpָ����sstring
	if ((*p_tmp) != ' ')
		return;  //�����Կո�ͷ

	//�ҵ���һ����Ϊ�ո��λ��
	while ((*p_tmp) != '\0')
	{
		if ((*p_tmp) == ' ')
			p_tmp++;
		else
			break;
	}
	if ((*p_tmp) == '\0') //ȫ�ǿո�
	{
		*sstring = '\0';
		return;
	}
	char *p_tmp2 = sstring;  //��ʱ p_tmp2 ָ���� sstring
	while ((*p_tmp2) != '\0')
	{
		(*p_tmp2) = (*p_tmp);
		p_tmp++;
		p_tmp2++;
	}
	(*p_tmp2) = '\0';  //�ո�����ϣ��ǵ���ĩβ���һ��'\0' ��ʾ�ַ�����β
	return;
}


//�Ƚ�str1��str2�����ַ��������Ƿ�һ�������Դ�Сд�ıȽ�
bool StrcaseCmp(const char* str1, const char* str2)
{
	if (str1 == NULL || str2 == NULL)
		return false;
	size_t strLen1 = strlen(str1);
	size_t strLen2 = strlen(str2);
	if (strLen1 != strLen2)
		return false;
	char *str = new char[strLen2]; 
	strcpy(str, str2); //��str2�����ݿ�����str��
	StringCase(str);  //��str�е���������ת�ɴ�д��ĸ
	while ((*str) != '\0')
	{
		if ((*str1) > 90)
		{
			if (((*str1) - 32) == (*str))
			{
				++str1;
				++str;
			}
			else {
				return false;
			}
		}
		else {
			if ((*str1) == (*str))
			{
				++str1;
				++str;
			}
			else {
				return false;
			}
		}
	}
	return true;
}

//��str�����е�Сд��ĸ��ɴ�д
void StringCase(char* str)
{
	if (str == NULL)
		return ;
	if (strlen(str) <= 0)
		return ;
	int strLen = strlen(str);
	char *ptmp = str;
	while ((*ptmp) != '\0')
	{
		if ((97 <= (*ptmp)) && ((*ptmp) <= 122))
		{
			(*ptmp) -= 32;
		}
		++ptmp;
	}
	return;
}