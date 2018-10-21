#include "machine.h"

void PutBuffer(bool left)
{
	//获取中间文件get指针位置
	//fmidPtr = fmid.tellg();
	int l, r;		//Buffer的左右端指针
	if (left) {
		l = 0;
		r = BUFFER_SIZE - 1;
	}
	else {
		l = BUFFER_SIZE;
		r = 2 * BUFFER_SIZE - 1;
	}

	for (int i = l;i <= r;++i) 
		buffer[i] = fmid.get();
}

void GetChar()
{
	if (forwardPtr == BUFFER_SIZE - 1) {
		//左半区终点，填充右半区buffer
		PutBuffer(false);
		forwardPtr++;
	}
	else if (forwardPtr == BUFFER_SIZE * 2 - 1) {
		//右半区终点，填充左半区buffer
		PutBuffer(true);
		forwardPtr = 0;
	}
	else {
		forwardPtr++;
	}
}

void GetPure()
{
	char preC;			//c之前读入的字符
	int state = 0;		//过滤程序自动机状态
	char c;

	c = fin.get();
	preC = '\0';
	while (!fin.eof()) {

		switch (state) {
		case 0:
			switch (c)
			{
			case ' ': case '\t':case'\f': case '\v':case '\r':
				/* 跳过转义字符 */
				state = 0;
				break;
			case '/':
				state = 1;
				break;
			case '\"':
				state = 2;
				fmid << c;
				break;
			default:
				state = 0;
				fmid << c;
				break;
			}
			break;
		case 1:
			switch (c) {
			case '*':
				state = 3;
				break;
			case '/':
				state = 5;
				break;
			default:
				state = 0;
				fmid << preC;
				fmid << c;
				break;
			}
			break;
		case 2:
			/* 忽略""中的注释符号 */
			if (c == '\"')
				state = 0;
			else
				state = 2;
			fmid << c;
			break;
		case 3:
			if ('*' == c)
				state = 4;
			else
				state = 3;
			break;
		case 4:
			switch (c)
			{
			case '*':
				state = 4;
				break;
			case '/':
				state = 0;
				break;
			default:
				state = 3;
				break;
			}
			break;
		case 5:
			if ('\n' == c)
				state = 0;
			else
				state = 5;
			break;
		default:
			break;
		}
		preC = c;
		c = fin.get();	//“>>”会跳过换行符
	}
	fin.close();
	/* 将中间文件的文件指针定位到开头，方便后续词法分析处理 */
	fmid.seekg(ios::beg);
	fmid.seekp(ios::beg);
}
