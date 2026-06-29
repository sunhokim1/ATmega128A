/*
 * cal.c
 *
 * Created: 2026-06-29 오후 4:08:31
 *  Author: kccistc
 */ 


#include "cal.h"

int calculator(char* buff) {
	int i = 0;
	int num = 0;
	int result = 0;
	int midnum = 0;
	char symbol = '+';

	while (buff[i] != '=' && buff[i] != '\0') {
		if (buff[i] >= '0' && buff[i] <= '9')
			num = num * 10 + (buff[i] - '0');
		else if (buff[i] == '+' || buff[i] == '-' || buff[i] == '*' || buff[i] == '/') {
			if (symbol == '+') {
				result += midnum;
				midnum = num;
			}else if (symbol == '-') {
				result += midnum;
				midnum = -num;
			}else if (symbol == '*')
				midnum *= num;
			else if (symbol == '/')
				midnum /= num;
			symbol = buff[i];
			num = 0;
		}else return -999;

		i++;
	}
	if (symbol == '+') {
		result += midnum;
		midnum = num;
	}else if (symbol == '-') {
		result += midnum;
		midnum = -num;
	}else if (symbol == '*')
		midnum *= num;
	else if (symbol == '/')
		midnum /= num;

	result += midnum;

	return result;
}