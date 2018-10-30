#include "machine.h"

void PutBuffer(bool left)
{
	int l, r;		//Buffer�����Ҷ�ָ��
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

void GetNBC()
{
	while (' ' == ch) {
		GetChar();
	}
}

void GetPure()
{
	char preC;			//c֮ǰ������ַ�
	int state = 0;		//���˳����Զ���״̬
	char c;

	c = fin.get();
	//���ԭ�ļ�����
	fstream fmid("test_pure.txt", ios::out);
	sourceFileInfo.numChar++;
	preC = '\0';
	while (!fin.eof()) {
		switch (state) {
		case 0:
			switch (c)
			{
			case' ':case '\t':case'\f': case '\v':case '\r':
				/* ����ת���ַ�,�ÿո������棨���ִ�� */
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
			/* ����""�е�ע�ͷ��� */
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
		c = fin.get();	//��>>�����������з�
		sourceFileInfo.numChar++;
	}
	fin.close();
	/* ���м��ļ����ļ�ָ�붨λ����ͷ����������ʷ��������� ��ȡ���ļ�ĩβʱ����״̬��eof��
	��Ҫ��������Ϊgoodbit���ܳɹ����ļ�ָ���ƶ����ļ���ͷ�������ں��������ã�*/
	fmid.clear();
	fmid.seekg(0, ios::beg);

}

void Return(string token, string value)
{
	//��<token,value>����ʽд��out�ļ�
	fout << '<' << token << ',' << value << '>' << endl;

	//��������һ
	sourceFileInfo.numToken++;
	//��ʼָ��ָ����һ��������ʼ
	beginPtr = forwardPtr;
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
	//�ȼ��ñ�ʶ���Ƿ��Ѿ��ڱ��У��Ƿ��Ѿ����ֹ���
	for (vector<string>::iterator it = userDefinedIdTable.begin();
		it != userDefinedIdTable.end(); it++) {
		if (*it == token)
			return (it - userDefinedIdTable.begin() + 1);
	}
	userDefinedIdTable.push_back(token);
	//��Ԫ�ص�λ��ָ�룬����ʼ�մ�������vector��λ����ŵ���������С-1
	return (userDefinedIdTable.size() - 1);
}

void Error(const int row, const int state)
{
	cout << "ERROR! " << "row = " << row << ": ";
	switch (state) {
	case 0:
		cout << "����Ƿ��ַ���" << endl;
		break;
	case 2:
		cout << "��ʶ����������" << endl;
		break;
	case 3: case 5: case 6:
		cout << "����/��ʶ����ʽ����" << endl;
		break;
	case 16:
		cout << "ӦΪ��||�������" << endl;
		break;
	case 17:
		cout << "ӦΪ��?:�������" << endl;
		break;
	case 20:
		cout << "�ַ������������" << endl;
		break;
	default:
		break;
	}

	/*�������ǰָ����ǰ�ƶ�һ������������*/
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
	cout << "�ַ��ܸ���:" << sourceFileInfo.numChar << endl;
	cout << "������:" << sourceFileInfo.numRow << endl;
	cout << "�����ܸ���:" << sourceFileInfo.numToken << endl;
}
