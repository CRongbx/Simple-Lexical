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
	else {
		forwardPtr++;
	}
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
	fmid.seekg(ios::beg);
	fmid.seekp(ios::beg);
}
