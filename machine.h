#ifndef MACHINE
#define MACHINE
#include <string>
#include <fstream>
#define BUFFER_SIZE 1000		//输入缓冲区半区大小N
using namespace std;
/*----------------------------关键字表-----------------------*/
	//按表格1的顺序填写，从左到右从上到下，填写。
const string key[37] = { "char","double","enum","float","int","long","short","signed","struct",
"union","unsigned","void","break","case","continue","default","do","else",
"for","goto","if","return","switch","while","auto","extern","register",
"static","const","sizeof","typedef","volatile","_Bool","_Complex",
"_Imaginary","inline","restrict" };

/*---------------------------全局变量--------------------------*/
ofstream fout("test_out.txt");		//输出文件
ifstream fin("test_in.txt");		//输入文件
fstream fmid("test_pure.txt");	//过滤后的文件
long long int fmidPtr = 0;		//中间文件的文件指针
int isKey = -1;		//标志单词是否为关键字，若是则为列表key中的下标；否则为-1	
char ch = '\0';		//字符变量，存放当前从文件读入的字符
string token = "";	//字符串，存放当前正在识别的单词字符串；
string buffer(BUFFER_SIZE * 2, '\0');	//输入缓冲区
unsigned int forwardPtr = 0;		//字符串下标，配对缓冲区时的向前指针
unsigned int beginPtr = 0;			//字符串下标，配对缓冲区时的开始指针

/*------------------------函数声明-----------------------------*/
//向buffer中读入数据，一次读入半个缓冲区，用left的布尔值表示填充左/右半区
void PutBuffer(bool left);
//向前指针forwardPre从buffer中读入一个字符放入ch,移动forward使之指向下一个字符
void GetChar();			
//词法分析前的预处理，过滤掉注释和空白符，输出中间文件test_pure.txt
void GetPure();
//将识别出的单词返还：<记号，属性>给调用程序
void Return(string token, string value);
//把ch中的字符连接到字符串token后
void Cat();
//向前指针forwardPtr向后退一个字符
void Retract();
//根据token查找关键字表，若是关键字则返回该关键字在词表中的下标，否则返回-1
int FindKey(const string token);
//将标识符在符号表中的迭代器类型，转换为string类型并返回，原本迭代器类型不变
string IntToS(const int index);
//将识别出来的用户自定义的标识符插入符号表，返回该元素的位置指针（迭代器）
int InsertTable(const string token);
//错误处理
void Error();
#endif