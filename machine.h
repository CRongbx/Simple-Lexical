#ifndef MACHINE
#define MACHINE
#include <string>
#include <fstream>
#define BUFFER_SIZE 1000		//���뻺����������СN
using namespace std;
/*----------------------------�ؼ��ֱ�-----------------------*/
	//�����1��˳����д�������Ҵ��ϵ��£���д��
const string key[37] = { "char","double","enum","float","int","long","short","signed","struct",
"union","unsigned","void","break","case","continue","default","do","else",
"for","goto","if","return","switch","while","auto","extern","register",
"static","const","sizeof","typedef","volatile","_Bool","_Complex",
"_Imaginary","inline","restrict" };

/*---------------------------ȫ�ֱ���--------------------------*/
ofstream fout("test_out.txt");		//����ļ�
ifstream fin("test_in.txt");		//�����ļ�
fstream fmid("test_pure.txt");	//���˺���ļ�
long long int fmidPtr = 0;		//�м��ļ����ļ�ָ��
int isKey = -1;		//��־�����Ƿ�Ϊ�ؼ��֣�������Ϊ�б�key�е��±ꣻ����Ϊ-1	
char ch = '\0';		//�ַ���������ŵ�ǰ���ļ�������ַ�
string token = "";	//�ַ�������ŵ�ǰ����ʶ��ĵ����ַ�����
string buffer(BUFFER_SIZE * 2, '\0');	//���뻺����
unsigned int forwardPtr = 0;		//�ַ����±꣬��Ի�����ʱ����ǰָ��
unsigned int beginPtr = 0;			//�ַ����±꣬��Ի�����ʱ�Ŀ�ʼָ��

/*------------------------��������-----------------------------*/
//��buffer�ж������ݣ�һ�ζ���������������left�Ĳ���ֵ��ʾ�����/�Ұ���
void PutBuffer(bool left);
//��ǰָ��forwardPre��buffer�ж���һ���ַ�����ch,�ƶ�forwardʹָ֮����һ���ַ�
void GetChar();			
//�ʷ�����ǰ��Ԥ�������˵�ע�ͺͿհ׷�������м��ļ�test_pure.txt
void GetPure();
//��ʶ����ĵ��ʷ�����<�Ǻţ�����>�����ó���
void Return(string token, string value);
//��ch�е��ַ����ӵ��ַ���token��
void Cat();
//��ǰָ��forwardPtr�����һ���ַ�
void Retract();
//����token���ҹؼ��ֱ����ǹؼ����򷵻ظùؼ����ڴʱ��е��±꣬���򷵻�-1
int FindKey(const string token);
//����ʶ���ڷ��ű��еĵ��������ͣ�ת��Ϊstring���Ͳ����أ�ԭ�����������Ͳ���
string IntToS(const int index);
//��ʶ��������û��Զ���ı�ʶ��������ű����ظ�Ԫ�ص�λ��ָ�루��������
int InsertTable(const string token);
//������
void Error();
#endif