#include "machine.h"

void PutBuffer(bool left)
{
	//��ȡ�м��ļ�getָ��λ��
	//fmidPtr = fmid.tellg();
	int l, r;		//Buffer�����Ҷ�ָ��
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
	//��ǰָ��forwardPre��buffer�ж���һ���ַ�����ch
	ch = buffer[forwardPtr];

	//�ƶ�forwardʹָ֮����һ���ַ�
	if (forwardPtr == BUFFER_SIZE - 1) {
		//������յ㣬����Ұ���buffer
		PutBuffer(false);
		forwardPtr++;
	}
	else if (forwardPtr == BUFFER_SIZE * 2 - 1) {
		//�Ұ����յ㣬��������buffer
		PutBuffer(true);
		forwardPtr = 0;
	}
	else 
		forwardPtr++;
}

void GetPure()
{
	char preC;			//c֮ǰ������ַ�
	int state = 0;		//���˳����Զ���״̬
	char c;

	c = fin.get();
	preC = '\0';
	while (!fin.eof()) {

		switch (state) {
		case 0:
			switch (c)
			{
			case ' ': case '\t':case'\f': case '\v':case '\r':
				/* ����ת���ַ� */
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
			/* ����""�е�ע�ͷ��� */
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
		c = fin.get();	//��>>�����������з�
	}
	fin.close();
	/* ���м��ļ����ļ�ָ�붨λ����ͷ����������ʷ��������� */
	//��ȡ���ļ�ĩβʱ����״̬��eof����Ҫ��������Ϊgoodbit���ܳɹ����ļ�ָ���ƶ����ļ���ͷ��
	fmid.clear(ios::goodbit);
	fmid.seekg(ios::beg);
	fmid.seekp(ios::beg);
}

void Return(string token, string value)
{
	//��<token,value>����ʽд��out�ļ�
	fout << '<' << token << ',' << value << '>' << endl;

	//�ƶ���ʼָ�����ǰָ�뵽��һ������
	if (forwardPtr == BUFFER_SIZE - 1) {
		//������յ㣬����Ұ���buffer
		PutBuffer(false);
		beginPtr = ++forwardPtr;
	}
	else if (forwardPtr == BUFFER_SIZE * 2 - 1) {
		//�Ұ����յ㣬��������buffer
		PutBuffer(true);
		beginPtr = 0;
		forwardPtr = 0;
	}
	else
		beginPtr = ++forwardPtr;
}

void Cat()
{
	//���ַ���tokenβ�˼�һ���ַ�ch
	token.push_back(ch);
}

void Retract()
{
	if (forwardPtr == BUFFER_SIZE) {
		forwardPtr = BUFFER_SIZE - 1;
		//���ļ�ָ��ӵ�ǰλ�����ļ���ʼ�����ƶ����buffer�ֽ�
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
	//��Ԫ�ص�λ��ָ�룬����ʼ�մ�������vector��λ����ŵ���������С-1
	return (userDefinedIdTable.size()-1);
}

void Error()
{
	cout << "ERROR!" << endl;
}
