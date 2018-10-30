#ifndef MACHINE
#define MACHINE
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#define BUFFER_SIZE 1000		//���뻺����������СN
#define KEY_SIZE 37				//�ؼ��ָ���
using namespace std;
/*----------------------------�ؼ��ֱ�-----------------------*/
	//�����1��˳����д�������Ҵ��ϵ��£���д��
const string key[KEY_SIZE] = { "char","double","enum","float","int","long","short",
"signed","struct","union","unsigned","void","break","case","continue","default",
"do","else","for","goto","if","return","switch","while","auto","extern","register",
"static","const","sizeof","typedef","volatile","_Bool","_Complex",
"_Imaginary","inline","restrict" };

/*---------------------------ȫ�ֱ�������--------------------------*/
extern ofstream fout;		//����ļ�
extern ifstream fin;		//�����ļ�
extern fstream fmid;		//���˺���ļ�
extern ofstream table;		//�����ʶ�����ű�
extern int isKey ;		//��־�����Ƿ�Ϊ�ؼ��֣�������Ϊ�б�key�е��±ꣻ����Ϊ-1	
extern char ch ;		//�ַ���������ŵ�ǰ���ļ�������ַ�
extern unsigned int row;	//��¼��ǰch�����к�
extern string token;	//�ַ�������ŵ�ǰ����ʶ��ĵ����ַ�����
extern string buffer;	//���뻺����
extern unsigned int forwardPtr;		//�ַ����±꣬��Ի�����ʱ����ǰָ��
extern unsigned int beginPtr ;			//�ַ����±꣬��Ի�����ʱ�Ŀ�ʼָ��
extern vector<string> userDefinedIdTable;		//�û��Զ���ı�ʶ�����ű�
struct SourceFileInfo {					
	unsigned int numRow = 1;			//��������ʼ����ӦΪ0��
	unsigned int numChar = 0;			//�ַ�����
	unsigned int numToken = 0;			//���ʸ���
	//unsigned int numId = 0;				//��ʶ������
	//unsigned int numNum = 0;			//��������
	//unsigned int numOper = 0;			//���������
	//unsigned int numPunc = 0;			//�����Ÿ���
	//unsigned int numString = 0;			//�ַ�����Ŀ
	//unsigned int numChar = 0;			//�ַ�������Ŀ
	//unsigned int numNote = 0;			//ע����Ŀ
} ;
extern struct SourceFileInfo sourceFileInfo;	//Դ�ļ���ͳ����Ϣ

/*------------------------��������-----------------------------*/
//��buffer�ж������ݣ�һ�ζ���������������left�Ĳ���ֵ��ʾ�����/�Ұ���
void PutBuffer(bool left);
//��ǰָ��forwardPre��buffer�ж���һ���ַ�����ch,�ƶ�forwardʹָ֮����һ���ַ�
void GetChar();			
//�����հ׷���ֱ��ch�ж���һ���ǿհ��ַ�
void GetNBC();
//�ʷ�����ǰ��Ԥ�������˵�ע�ͺͿհ׷�������м��ļ�test_pure.txt
void GetPure();
//��ʶ����ĵ��ʷ��������ó�������ǰָ��Ϳ�ʼָ��ָ����һ�����ʵ�λ��
void Return(string token, string value);
//��ch�е��ַ����ӵ��ַ���token��
void Cat();
//��ǰָ��forwardPtr�����һ���ַ�
void Retract();
//����token���ҹؼ��ֱ����ǹؼ����򷵻ظùؼ����ڴʱ��е��±꣬���򷵻�-1
int FindKey(const string token);
//����ʶ���ڷ��ű��еĵ��������ͣ�ת��Ϊstring���Ͳ����أ�ԭ�����������Ͳ���
string IntToS(const int index);
//��ʶ��������û��Զ���ı�ʶ��������ű����ظ�Ԫ�ص�λ��ָ�루�����±� (��0��ʼ)��
int InsertTable(const string token);
//������
void Error(const int row,const int state);
//�����ʶ�����ű�ָ���ļ�
void PutTable();
//���Դ����ͳ����Ϣ���
void PutSourceFileInfo();

#endif