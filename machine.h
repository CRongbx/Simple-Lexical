#ifndef MACHINE
#define MACHINE
#include <string>
#include <vector>

/*----------------------------关键字表-----------------------*/
	//按表格1的顺序填写，从左到右从上到下，填写。
const vector<string> key =
{ "char","double","enum","float","int","long","short","signed","struct",
"union","unsigned","void","break","case","continue","default","do","else",
"for","goto","if","return","switch","while","auto","extern","register",
"static","const","sizeof","typedef","volatile","_Bool","_Complex",
"_Imaginary","inline","restrict" };

/*------------------------函数声明-----------------------------*/
//每调用一次，向前指针forwardPre从buffer中读入一个字符放入ch,移动forward使之指向下一个字符
void GetChar();			
//每次调用检查字符ch是否为空白符，若是重复调用GetChar()直至ch读入一个非空字符
void GetNbc();
//将识别出的单词返还：<记号，属性>给调用程序
void Return(string token, string value);
//把ch中的字符连接到字符串token后
void Cat();
//向前指针forwardPtr向后退一个字符
void Retract();
//根据token查找关键字表，若是关键字则返回该关键字在词表中的下标，否则返回-1
int FindKey(const string token);
//将标识符在符号表中的迭代器类型，转换为string类型并返回，原本迭代器类型不变
string IterToS(const vector<string>::iterator iter);
//将识别出来的用户自定义的标识符插入符号表，返回该元素的位置指针（迭代器）
vector<string>::iterator InsertTable(const string token);
//错误处理
void Error();
#endif