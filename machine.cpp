#include "machine.h"

void PutBuffer(bool left)
{
	int l, r;		//Buffer的左右端指针
	if (left) {
		l = 0;
		r = BUFFER_SIZE - 1;
	}
	else {
		l = BUFFER_SIZE;
		r = 2 * BUFFER_SIZE - 1;
	}

	int i = 1;
	for (i = l;!fmid.eof() && i <= r;++i)
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

void GetNBC()
{
	while (' ' == ch) {
		GetChar();
	}
}

void GetPure()
{
	char preC;			//c之前读入的字符
	int state = 0;		//过滤程序自动机状态
	char c;

	c = fin.get();
	//清空原文件内容
	fstream fmid("test_pure.txt", ios::out);
	sourceFileInfo.numChar++;
	preC = '\0';
	while (!fin.eof()) {
		switch (state) {
		case 0:
			switch (c)
			{
			case' ':case '\t':case'\f': case '\v':case '\r':
				/* 跳过转义字符,用空格来代替（划分词语） */
				state = 0;
				fmid << ' ';
				break;
			case '\n':
				state = 0;
				fmid << '\n';
				sourceFileInfo.numRow++;
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
			else if (c == '\n') {
				state = 2;
				fmid << '\n';
				sourceFileInfo.numRow++;
			}
			else
				state = 2;
			fmid << c;
			break;
		case 3:
			if ('*' == c)
				state = 4;
			else if ('\n' == c) {
				state = 3;
				sourceFileInfo.numRow++;
			}
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
				fmid << '\n';
				break;
			case '\n':
				state = 3;
				fmid << '\n';
				sourceFileInfo.numRow++;
				break;
			default:
				state = 3;
				break;
			}
			break;
		case 5:
			if ('\n' == c) {
				fmid << '\n';
				sourceFileInfo.numRow++;
				state = 0;
			}
			else
				state = 5;
			break;
		default:
			break;
		}
		preC = c;
		c = fin.get();	//“>>”会跳过换行符
		sourceFileInfo.numChar++;
	}
	fin.close();
	/* 将中间文件的文件指针定位到开头，方便后续词法分析处理 读取到文件末尾时，流状态是eof，
	需要将其设置为goodbit才能成功将文件指针移动到文件开头。不可在函数里设置，*/
	fmid.clear();
	fmid.seekg(0, ios::beg);

}

void Return(string token, string value)
{
	//以<token,value>的形式写入out文件
	fout << '<' << token << ',' << value << '>' << endl;

	//单词数加一
	sourceFileInfo.numToken++;
	//开始指针指向下一个单词起始
	beginPtr = forwardPtr;
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
	//先检查该标识符是否已经在表中（是否已经出现过）
	for (vector<string>::iterator it = userDefinedIdTable.begin();
		it != userDefinedIdTable.end(); it++) {
		if (*it == token)
			return (it - userDefinedIdTable.begin() + 1);
	}
	userDefinedIdTable.push_back(token);
	//该元素的位置指针，由于始终从最后加入vector故位置序号等于容器大小-1
	return (userDefinedIdTable.size() - 1);
}

void Error(const int row, const int state)
{
	cout << "ERROR! " << "row = " << row << ": ";
	switch (state) {
	case 0:
		cout << "输入非法字符！" << endl;
		break;
	case 2:
		cout << "标识符命名错误！" << endl;
		break;
	case 3: case 5: case 6:
		cout << "常数/标识符格式错误！" << endl;
		break;
	case 16:
		cout << "应为“||”运算符" << endl;
		break;
	case 17:
		cout << "应为“?:”运算符" << endl;
		break;
	case 20:
		cout << "字符常量定义错误！" << endl;
		break;
	default:
		break;
	}

	/*出错后，向前指针向前移动一个，跳过错误*/
	beginPtr = ++forwardPtr;
}

void PutTable()
{
	vector<string>::iterator it;
	for (it = userDefinedIdTable.begin();it != userDefinedIdTable.end(); ++it) {
		table << '[' << *it << ',' << distance(userDefinedIdTable.begin(), it) << ']' << endl;
	}
}

void PutSourceFileInfo()
{
	ofstream sourceInfo("source_info_out.txt");

	sourceInfo << "number of char:" << sourceFileInfo.numChar << endl;
	sourceInfo << "number of row:" << sourceFileInfo.numRow << endl;
	sourceInfo << "number of token:" << sourceFileInfo.numToken << endl;

	sourceInfo.close();

	cout << "----------------------------------------------------" << endl;
	cout << "字符总个数:" << sourceFileInfo.numChar << endl;
	cout << "总行数:" << sourceFileInfo.numRow << endl;
	cout << "单词总个数:" << sourceFileInfo.numToken << endl;
}
