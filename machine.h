#ifndef MACHINE
#define MACHINE
#include <string>
#include <vector>

/*----------------------------�ؼ��ֱ�-----------------------*/
	//�����1��˳����д�������Ҵ��ϵ��£���д��
const vector<string> key =
{ "char","double","enum","float","int","long","short","signed","struct",
"union","unsigned","void","break","case","continue","default","do","else",
"for","goto","if","return","switch","while","auto","extern","register",
"static","const","sizeof","typedef","volatile","_Bool","_Complex",
"_Imaginary","inline","restrict" };

/*------------------------��������-----------------------------*/
//ÿ����һ�Σ���ǰָ��forwardPre��buffer�ж���һ���ַ�����ch,�ƶ�forwardʹָ֮����һ���ַ�
void GetChar();			
//ÿ�ε��ü���ַ�ch�Ƿ�Ϊ�հ׷��������ظ�����GetChar()ֱ��ch����һ���ǿ��ַ�
void GetNbc();
//��ʶ����ĵ��ʷ�����<�Ǻţ�����>�����ó���
void Return(string token, string value);
//��ch�е��ַ����ӵ��ַ���token��
void Cat();
//��ǰָ��forwardPtr�����һ���ַ�
void Retract();
//����token���ҹؼ��ֱ����ǹؼ����򷵻ظùؼ����ڴʱ��е��±꣬���򷵻�-1
int FindKey(const string token);
//����ʶ���ڷ��ű��еĵ��������ͣ�ת��Ϊstring���Ͳ����أ�ԭ�����������Ͳ���
string IterToS(const vector<string>::iterator iter);
//��ʶ��������û��Զ���ı�ʶ��������ű����ظ�Ԫ�ص�λ��ָ�루��������
vector<string>::iterator InsertTable(const string token);
//������
void Error();
#endif