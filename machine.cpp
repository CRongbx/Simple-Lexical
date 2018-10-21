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
	//向前指针forwardPre从buffer中读入一个字符放入ch
	ch = buffer[forwardPtr];

	//移动forward使之指向下一个字符
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
	else 
		forwardPtr++;
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
	//读取到文件末尾时，流状态是eof，需要将其设置为goodbit才能成功将文件指针移动到文件开头。
	fmid.clear(ios::goodbit);
	fmid.seekg(ios::beg);
	fmid.seekp(ios::beg);
}

void Return(string token, string value)
{
	//以<token,value>的形式写入out文件
	fout << '<' << token << ',' << value << '>' << endl;

	//移动开始指针和向前指针到下一个单词
	if (forwardPtr == BUFFER_SIZE - 1) {
		//左半区终点，填充右半区buffer
		PutBuffer(false);
		beginPtr = ++forwardPtr;
	}
	else if (forwardPtr == BUFFER_SIZE * 2 - 1) {
		//右半区终点，填充左半区buffer
		PutBuffer(true);
		beginPtr = 0;
		forwardPtr = 0;
	}
	else
		beginPtr = ++forwardPtr;
}

void Cat()
{
	//在字符串token尾端加一个字符ch
	token.push_back(ch);
}

void Retract()
{
	if (forwardPtr == BUFFER_SIZE) {
		forwardPtr = BUFFER_SIZE - 1;
		//让文件指针从当前位置向文件开始方向移动半个buffer字节
		fmid.seekg(-BUFFER_SIZE, ios::cur);
	}
	else if (forwardPtr == 0) {
		forwardPtr = BUFFER_SIZE * 2 - 1;
		fmid.seekg(-BUFFER_SIZE, ios::cur);
	}
	forwardPtr--;
}

int FindKey(const string token)
{
	for (int i = 0; i < KEY_SIZE;++i) 
		if (key[i] == token)
			return i;
	return -1;
}

string IntToS(const int index)
{
	return to_string(index);
}

int InsertTable(const string token)
{
	userDefinedIdTable.push_back(token);
	//该元素的位置指针，由于始终从最后加入vector故位置序号等于容器大小-1
	return (userDefinedIdTable.size()-1);
}

void Error()
{
	cout << "ERROR!" << endl;
}
