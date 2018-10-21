#include <iostream>
#include <fstream>
#include <string>
#include <cctype>			//isdigit(),isalpha()
#include <algorithm>		//find():在vector中查找指定元素，返回指向该元素的迭代器指针
#include "machine.h"
#define BUFFER_SIZE 10		//输入缓冲区大小


using namespace std;

int  main(void) {
	ofstream fout("test_out.txt");
	ifstream fin("test_in.txt");

	int state = 0;	//自动机当前状态
	int isKey = -1;		//标志单词是否为关键字，若是则为列表key中的下标；否则为-1	
	char ch = '\0';		//字符变量，存放当前从文件读入的字符
	string token = "";	//字符串，存放当前正在识别的单词字符串；
	string buffer(BUFFER_SIZE, '\0');	//输入缓冲区
	unsigned int forwardPtr = 0;		//字符串下标，配对缓冲区时的向前指针
	unsigned int beginPtr = 0;			//字符串下标，配对缓冲区时的开始指针
	

	while (!fin.eof()) {
		fin >> ch;
		switch (state) {
		case 0:
			token = "";
			GetChar();
			GetNbc();
			switch (ch) {
			case ';':
				state = 0;
				Return(";", "-");
				break;
			case ',':
				state = 0;
				Return(",", "-");
				break;
			case '(':
				state = 0;
				Return("(", "-");
				break;
			case ')':
				state = 0;
				Return(")", "-");
				break;
			case '[':
				state = 0;
				Return("[", "-");
				break;
			case ']':
				state = 0;
				Return("]", "-");
				break;
			case '{':
				state = 0;
				Return("{", "-");
				break;
			case '}':
				state = 0;
				Return("}", "-");
				break;
			case ':':
				state = 0;
				Return(":", "-");
				break;
			case '.':
				state = 0;
				Return(".", "-");
			case '_':
			case 'a': case 'b': case 'c': case 'd': case 'e':case 'f': case 'g': case 'h': case 'i':
			case 'j': case 'k': case 'l':case 'm': case 'n':case 'o': case 'p': case 'q': case 'r':
			case 's':case 't': case 'u':case 'v': case 'w': case 'x': case 'y':case 'z':case 'A':
			case 'B':case 'C': case 'D': case 'E':case 'F': case 'G': case 'H': case 'I':case 'J':
			case 'K': case 'L':case 'M': case 'N': case 'O': case 'P':case 'Q': case 'R': case 'S':
			case 'T': case 'U': case 'V': case 'W':case 'X': case 'Y': case 'Z':
				state = 1;
				break;
			case '1':case '2': case '3': case'4':case'5':case'6':case'7':case'8':case '9':case '0':
				state = 2;
				break;
			case '<': 
				state = 8;
				break;
			case  '+':
				state = 9;
				break;
			case '-':
				state = 10;
				break;
			case '=':
				state = 11;
				break;
			case '\\':
				state = 27;
				break;
			case '/':
				state = 12;
				break;
			case '"':
				state = 22;
				break;
			case '\'':
				state = 26;
				break;
			case '*':
				state = 13;
				break;
			case '%':
				state = 14;
				break;
			case '|':
				state = 16;
				break;
			case '&':
				state = 15;
				break;
			case '?':
				state = 17;
				break;
			case '>':
				state = 24;
				break;
			case '!':
				state = 23;
				break;
			case '^':
				state = 25;
				break;
			default:
				Error();
				state = 0;
				break;
			}//switch(ch) case 0
			break;
		case 1:										//标识符状态
			Cat();
			GetChar();
			if ('_' == ch || isdigit(ch) || isalpha(ch)) 
				state = 1;
			else {
				Retract();
				state = 0;
				isKey = FindKey(token);
				if (-1 == isKey) {		//用户自定义的标识符
					vector<string>::iterator iter = InsertTable(token);
					Return("id", IterToS(iter));
				}
				else					//关键字
					Return(token, "-");
			}
			break;
		case 2:										//无符号数状态
			Cat();
			GetChar();
			switch (ch){
			case '1':case '2': case '3': case'4':case'5':case'6':case'7':case'8':case '9':case '0':
				state = 2;
				break;
			case 'E':
				state = 5;
				break;
			case '.':
				state = 3;
				break;
			default:
				Retract();
				state = 0;
				Return("num", token);
				break;
			}//switch(ch) case 2
			break;
		case 3:										//小数点状态
			Cat();
			GetChar();
			if (isdigit(ch))
				state = 4;
			else {
				Error();
				state = 0;
			}
			break;
		case 4:										//小数状态
			Cat();
			GetChar();
			if (isdigit(ch))
				state = 4;
			else if ('E' == ch) 
				state = 5;
			else {
				state = 0;
				Retract();
				Return("num", token);
			}
			break;
		case 5:										//指数状态
			Cat();
			GetChar();
			if (isdigit(ch))
				state = 7;
			else if ('+' == ch || '-' == ch)
				state = 6;
			else {
				state = 0;
				Error();
			}
			break;
		case 6:
			Cat();
			GetChar();
			if (isdigit(ch))
				state = 7;
			else {
				state = 0;
				Error();
			}
			break;
		case 7:
			Cat();
			GetChar();
			if (isdigit(ch))
				state = 7;
			else {
				state = 0;
				Retract();
				Return("num", token);
			}
			break;
		case 8:										//'<'状态
			Cat();
			GetChar();
			if ('=' == ch) {
				state = 0;
				Return("operater_r", "LE");
			}
			else {
				state = 0;
				Retract();
				Return("operater_r", "LT");
			}
			break;
		case 9:										//'+'状态
			Cat();
			GetChar();
			if ('+' == ch) {
				state = 0;
				Return("++", "-");
			}
			else if ('=' == ch) {
				state = 0;
				Return("operater_as", "PE");
			}
			else {
				state = 0;
				Retract();
				Return("+", "-");
			}
			break;
		case 10:									//'-'状态
			Cat();
			GetChar();
			if ('-' == ch) {
				state = 0;
				Return("--", "-");
			}
			else if ('=' == ch) {
				state = 0;
				Return("operater_as", "SE");
			}
			else {
				state = 0;
				Retract();
				Return("-", "-");
			}
			break;
		case 11:									//'='状态
			Cat();
			GetChar();
			if ('=' == ch) {
				state = 0;
				Return("operater_r", "EQ");
			}
			else {
				state = 0;
				Retract();
				Return("operater_as", "E");
			}
			break;
		case 12:									//'/'状态
			Cat();
			GetChar();
			switch (ch) {
			case '=':
				state = 0;
				Return("operater_as", "DE");
				break;
			case '*':
				state = 18;
				break;
			case '/':
				state = 21;
				break;
			default:
				state = 0;
				Retract();
				Return("/", "-");
				break;
			}
			break;
		case 13:									//'*'状态
			Cat();
			GetChar();
			if ('=' == ch) {
				state = 0;
				Return("operater_as", "MUE");
			}
			else {
				state = 0;
				Retract();
				Return("*", "-");
			}
			break;
		case 14:									//'%'状态
			Cat();
			GetChar();
			if ('=' == ch) {
				state = 0;
				Return("operater_as", "MOE");
			}
			else {
				state = 0;
				Retract();
				Return("%", "-");
			}
			break;
		case 15:									//'&'状态
			Cat();
			GetChar();
			if ('&' == ch) {
				state = 0;
				Return("operater_l", "AND");
			}
			else {
				state = 0;
				Retract();
				Return("&", "-");
			}
			break;
		case 16:									//'|'状态
			Cat();
			GetChar();
			if ('|' == ch) {
				state = 0;
				Return("operater_l", "OR");
			}
			else {
				state = 0;
				Error();
			}
			break;
		case 17:									//'?'状态
			Cat();
			GetChar();
			if (':' == ch) {
				state = 0;
				Return("?:", "-");
			}
			else {
				state = 0;
				Error();
			}
			break;
		case 18:										//"/*"状态
			Cat();
			GetChar();
			if ('*' == ch)
				state = 19;
			else
				state = 18;
			break;
		case 19:										// "/**"状态
			Cat();
			GetChar();
			switch (ch)
			{
			case '*':
				state = 19;
				break;
			case '/':		
				/* 跳过注释 */
				state = 0;
				break;
			default:
				state = 18;
				break;
			}
			break;
		case 21:										//"//"状态
			Cat();
			GetChar();
			if ('\n' == ch)
				/* 跳过注释 */
				state = 0;
			else
				state = 21;
			break;
		case 22:										//"""状态
			Cat();
			GetChar();
			if ('"' != ch)
				state = 22;
			else {
				state = 0;
				Return("STRING", token);
			}
			break;
		case 23:										//'!'状态
			Cat();
			GetChar();
			if ('=' == ch) {
				state = 0;
				Return("operater_r", "NE");
			}
			else {
				state = 0;
				Retract();
				Return("operater_l", "NOT");
			}
			break;
		case 24:										//'>'状态
			Cat();
			GetChar();
			if ('=' == ch) {
				state = 0;
				Return("operater_r", "GE");
			}
			else {
				state = 0;
				Retract();
				Return("operater_r", "GT");
			}
			break;
		case 25:											//'^'运算符
			Cat();
			GetChar();
			if (':' == ch) {
				state = 0;
				Return("operater_as", "XE");
			}
			else {
				state = 0;
				Error();
			}
			break;
		case 26:											//"'"状态
			Cat();
			GetChar();
			if ('\0' == ch || '\t'==ch || '\n'== ch || '\b'==ch||'f'==ch
				|| '\\'==ch||'\''== ch|| isdigit(ch)|| isalpha(ch)) 
				state = 28;
			else {
				Error();
				state = 0;
			}
			break;
		case 27:											//"'x"状态
			Cat();
			GetChar();
			if ('\'' == ch) {
				state = 0;
				Return("CHAR", token);
			}
			else {
				state = 0;
				Error();
			}
			break;
		default:
			break;
		}//switch(state)
	}//while


	fin.close();
	fout.close();
	return 0;
}