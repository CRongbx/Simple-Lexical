#ifndef MACHINE
#define MACHINE
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#define BUFFER_SIZE 1000		//输入缓冲区半区大小N
#define KEY_SIZE 37				//关键字个数
using namespace std;
/*----------------------------关键字表-----------------------*/
	//按表格1的顺序填写，从左到右从上到下，填写。
const string key[KEY_SIZE] = { "char","double","enum","float","int","long","short",
"signed","struct","union","unsigned","void","break","case","continue","default",
"do","else","for","goto","if","return","switch","while","auto","extern","register",
"static","const","sizeof","typedef","volatile","_Bool","_Complex",
"_Imaginary","inline","restrict" };

/*---------------------------全局变量声明--------------------------*/
extern ofstream fout;		//输出文件
extern ifstream fin;		//输入文件
extern fstream fmid;		//过滤后的文件
extern ofstream table;		//输出标识符符号表
extern int isKey ;		//标志单词是否为关键字，若是则为列表key中的下标；否则为-1	
extern char ch ;		//字符变量，存放当前从文件读入的字符
extern unsigned int row;	//记录当前ch所在行号
extern string token;	//字符串，存放当前正在识别的单词字符串；
extern string buffer;	//输入缓冲区
extern unsigned int forwardPtr;		//字符串下标，配对缓冲区时的向前指针
extern unsigned int beginPtr ;			//字符串下标，配对缓冲区时的开始指针
extern vector<string> userDefinedIdTable;		//用户自定义的标识符符号表
struct SourceFileInfo {					
	unsigned int numRow = 1;			//行数（初始行数应为0）
	unsigned int numChar = 0;			//字符个数
	unsigned int numToken = 0;			//单词个数
	//unsigned int numId = 0;				//标识符个数
	//unsigned int numNum = 0;			//常数个数
	//unsigned int numOper = 0;			//运算符个数
	//unsigned int numPunc = 0;			//标点符号个数
	//unsigned int numString = 0;			//字符串数目
	//unsigned int numChar = 0;			//字符常量数目
	//unsigned int numNote = 0;			//注释数目
} ;
extern struct SourceFileInfo sourceFileInfo;	//源文件的统计信息

/*------------------------函数声明-----------------------------*/
//向buffer中读入数据，一次读入半个缓冲区，用left的布尔值表示填充左/右半区
void PutBuffer(bool left);
//向前指针forwardPre从buffer中读入一个字符放入ch,移动forward使之指向下一个字符
void GetChar();			
//跳过空白符，直到ch中读入一个非空白字符
void GetNBC();
//词法分析前的预处理，过滤掉注释和空白符，输出中间文件test_pure.txt
void GetPure();
//将识别出的单词返还给调用程序，令向前指针和开始指针指向下一个单词的位置
void Return(string token, string value);
//把ch中的字符连接到字符串token后
void Cat();
//向前指针forwardPtr向后退一个字符
void Retract();
//根据token查找关键字表，若是关键字则返回该关键字在词表中的下标，否则返回-1
int FindKey(const string token);
//将标识符在符号表中的迭代器类型，转换为string类型并返回，原本迭代器类型不变
string IntToS(const int index);
//将识别出来的用户自定义的标识符插入符号表，返回该元素的位置指针（数字下标 (从0开始)）
int InsertTable(const string token);
//错误处理
void Error(const int row,const int state);
//输出标识符符号表到指定文件
void PutTable();
//输出源程序统计信息结果
void PutSourceFileInfo();

#endif