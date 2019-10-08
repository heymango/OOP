#define _CRT_SECURE_NO_WARNINGS
#include "inf_int.h"
#include <ostream>
using namespace std;

/*
Originally written by
��ǻ�Ͱ��к�
������
*/

inf_int::inf_int()
{
	this->digits = new char[2];	// �����Ҵ�

	this->digits[0] = '0';		// default �� 0 ����
	this->digits[1] = '\0';
	this->length = 1;
	this->thesign = true;
}

inf_int::inf_int(int n) {
	char buf[100];

	if (n < 0) {		// ���� ó��
		this->thesign = false;
		n = -n;
	}
	else {
		this->thesign = true;
	}

	int i = 0;
	while (n > 0) {			// ���ڸ� ���ڿ��� ��ȯ�ϴ� ����
		buf[i] = n % 10 + '0';

		n /= 10;
		i++;
	}

	if (i == 0) {	// ������ ������ 0�� ���...
		new (this) inf_int();	// ������ ��ȣ��...gcc���� �����Ͽ����� �ִٰ� ��. inf_int()�� ��� ������ �ν��Ͻ��� ������.
	}
	else {
		buf[i] = '\0';

		this->digits = new char[i + 1];
		this->length = i;
		strcpy(this->digits, buf);
	}
}

inf_int::inf_int(const char* str)
{
	char buf[100];
	int strsize;
	if (str[0]=='-') {		// ���� ó��
		this->thesign = false;
		strsize = strlen(str) - 1;
		for (int i = strsize; i >= 1; i--) {
			buf[strsize - i] = str[i];
		}
	}
	else {
		this->thesign = true;
		strsize = strlen(str);
		for (int i = strsize-1; i >= 0; i--) {
			buf[strsize-1 - i] = str[i];
		}
	}

	if (((this->thesign==false)&&(str[1]=='0'))||str[0]=='0') {	// ������ ������ 0�� ���
		new (this) inf_int();	// ������ ��ȣ��...gcc���� �����Ͽ����� �ִٰ� ��. inf_int()�� ��� ������ �ν��Ͻ��� ������. 
	}
	else {
		buf[strsize] = '\0';

		this->digits = new char[strsize + 1];
		this->length = strsize;
		strcpy(this->digits, buf);
	}
}

inf_int::inf_int(const inf_int& a) {
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;
}

inf_int::~inf_int() {
	delete digits;		// �޸� �Ҵ� ����
}

inf_int& inf_int::operator=(const inf_int& a)
{
	if (this->digits) {
		delete this->digits;		// �̹� ���ڿ��� ���� ��� ����.
	}
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;

	return *this;
}

bool operator==(const inf_int& a, const inf_int& b)
{
	// we assume 0 is always positive.
	if ((strcmp(a.digits, b.digits) == 0) && a.thesign == b.thesign)	// ��ȣ�� ����, ������ ��ġ�ؾ���.
		return true;
	return false;
}

bool operator!=(const inf_int& a, const inf_int& b)
{
	return !operator==(a, b);
}

bool operator>(const inf_int& a, const inf_int& b)
{
	bool isbig = false;
	if (a.length == b.length) {
		for (int i = a.length - 1; i >= 0; i--) {
			if (a.digits[i] > b.digits[i]) {
				isbig = true;
				break;
			}
		}
	}
	else if (a.length > b.length) isbig = true;
	if (a.thesign && b.thesign) return isbig;
	else if (!a.thesign && !b.thesign) return !isbig;
	else return a.thesign;
}

bool operator<(const inf_int& a, const inf_int& b)
{
	if (operator>(a, b) || operator==(a, b)) {
		return false;
	}
	else {
		return true;
	}
}

inf_int operator+(const inf_int& a, const inf_int& b)
{
	inf_int c;
	unsigned int i;

	if (a.thesign == b.thesign) {	// ������ ��ȣ�� ���� ��� + �����ڷ� ����
		for (i = 0; i < a.length; i++) {
			c.Add(a.digits[i], i + 1);
		}
		for (i = 0; i < b.length; i++) {
			c.Add(b.digits[i], i + 1);
		}

		c.thesign = a.thesign;

		return c;
	}
	else {	// ������ ��ȣ�� �ٸ� ��� - �����ڷ� ����
		c = b;
		c.thesign = a.thesign;

		return a - c;
	}
}
bool absBig(const inf_int& a, const inf_int& b) {
	if (a.length == b.length) {
		for (int i = a.length - 1; i >= 0; i--) {
			if (a.digits[i] > b.digits[i]) {
				return true;
			}
		}
	}
	else if (a.length > b.length) return true;
	else return false;
}

inf_int operator-(const inf_int& a, const inf_int& b)
{
	inf_int c;
	unsigned int i;
	if (a.thesign ==b.thesign) {
		if (absBig(a,b)) {
			for (i = 0; i < a.length; i++) {
				c.Add(a.digits[i], i + 1);
			}
			for (i = 0; i < b.length; i++) {
				c.Sub(b.digits[i], i + 1);
			}
			int k = c.length - 1;
			while (c.digits[k] == '0') {
				if (c.digits[k] == '0') {
					c.digits[k] = NULL;
					c.length--;
				}
				k--;
			}
			c.thesign = a.thesign;
		
			return c;
		}
		else {
			for (i = 0; i < a.length; i++) {
				c.Add(a.digits[i], i + 1);
			}
			for (i = 0; i < b.length; i++) {
				c.Sub(b.digits[i], i + 1);
			}
			c.thesign = !a.thesign;

			return c;

		}
	}
	else {	// ������ ��ȣ�� �ٸ� ��� - �����ڷ� ����
		c = b;
		c.thesign = a.thesign;

		return a + c;
	}
}


inf_int operator*(const inf_int& a, const inf_int& b)
{
	inf_int t1;
	if (a.length == 1 && a.digits[0] == '0' || b.length == 1 && b.digits[0] == '0') return t1;
	unsigned int i;
	int carry_b = 0;
	int carry = 0;
	int carry_a = 0;
	t1 = a;
	for (int j = 1; j <= a.length; j++) {
		int carry_b = t1.Mul(b.digits[0], j);
		if (carry > 0) {
			if (t1.Add2(carry + 48, j) == true) carry_a = 1;
			else carry_a = 0;
		}
		carry = carry_b + carry_a;
	}
	if (carry > 0) t1.Add(carry + 48, a.length + 1);

	for (int i = 1; i < b.length; i++) {
		inf_int temp;
		temp = a;
		carry_b = 0;
		carry = 0;
		carry_a = 0;
		for (int j = 1; j <= a.length; j++) {
			int carry_b = temp.Mul(b.digits[i], j);
			if (carry > 0) {
				if (temp.Add2(carry + 48, j) == true) carry_a = 1;
				else carry_a = 0;
			}
			carry = carry_b + carry_a;
		}
		if (carry > 0) temp.Add(carry + 48, a.length + 1);

		for (int k = 0; k < temp.length; k++) {
			t1.Add(temp.digits[k], k + i + 1);
		}
	}
	if (a.thesign == b.thesign) t1.thesign = true;
	else t1.thesign = false;

	return t1;
}





ostream& operator<<(ostream& out, const inf_int& a)
{
	int i;

	if (a.thesign == false) {
		out << '-';
	}
	for (i = a.length - 1; i >= 0; i--) {
		out << a.digits[i];
	}
	return out;
}

void inf_int::Add(const char num, const unsigned int index){
	if (this->length < index) {
		this->digits = (char*)realloc(this->digits, index + 1);
		
		if (this->digits == NULL) {		// �Ҵ� ���� ����ó��
			cout << "Memory reallocation failed, the program will terminate." << endl;

			exit(0);
		}

		this->length = index;					// ���� ����
		this->digits[this->length] = '\0';	// �ι��� ����
	}

	if (this->digits[index - 1] < '0') {	// ���� ���� '0'���� ���� �ƽ�Ű���� ��� 0���� ä��. �������� �ʾҴ� ���ο� �ڸ����� ��� �߻�
		this->digits[index - 1] = '0';
	}

	this->digits[index - 1] += num - '0';	// �� ����


	if (this->digits[index - 1] > '9') {	// �ڸ��ø��� �߻��� ���
		this->digits[index - 1] -= 10;	// ���� �ڸ������� (�ƽ�Ű��) 10�� ����
		Add('1', index + 1);			// ���ڸ��� 1�� ���Ѵ�
	}
}

void inf_int::Sub(const char num, const unsigned int index) {
	if (this->length < index) {
		this->digits = (char*)realloc(this->digits, index + 1);

		if (this->digits == NULL) {		// �Ҵ� ���� ����ó��
			cout << "Memory reallocation failed, the program will terminate." << endl;
			exit(0);
		}
		this->length = index;					// ���� ����
		this->digits[this->length] = '\0';	// �ι��� ����
	}

	this->digits[index - 1] -= num - '0';	// �� ����


	if (this->digits[index - 1] < '0') {	// �ڸ��ø��� �߻��� ���
		this->digits[index - 1] += 10;	// ���� �ڸ������� (�ƽ�Ű��) 10�� ����
		Sub('1', index + 1);			// ���ڸ��� 1�� ���Ѵ�
	}
}

bool inf_int::Add2(const char num, const unsigned int index)	// a�� index �ڸ����� n�� ���Ѵ�. 0<=n<=9, ex) a�� 391�϶�, Add(a, 2, 2)�� ����� 411
{
	if (this->length < index) {
		this->digits = (char*)realloc(this->digits, index + 1);

		if (this->digits == NULL) {		// �Ҵ� ���� ����ó��
			cout << "Memory reallocation failed, the program will terminate." << endl;

			exit(0);
		}

		this->length = index;					// ���� ����
		this->digits[this->length] = '\0';	// �ι��� ����
	}

	if (this->digits[index - 1] < '0') {	// ���� ���� '0'���� ���� �ƽ�Ű���� ��� 0���� ä��. �������� �ʾҴ� ���ο� �ڸ����� ��� �߻�
		this->digits[index - 1] = '0';
	}

	this->digits[index - 1] += num - '0';	// �� ����


	if (this->digits[index - 1] > '9') {	// �ڸ��ø��� �߻��� ���
		this->digits[index - 1] -= 10;	// ���� �ڸ������� (�ƽ�Ű��) 10�� ����
		return true;			// ���ڸ��� 1�� ���Ѵ�
	}
	else false;

}

int inf_int::Mul(const char num, const unsigned int index)	// a�� index �ڸ����� n�� ���Ѵ�. 0<=n<=9, ex) a�� 391�϶�, Add(a, 2, 2)�� ����� 411
{
	int n = num - 48;
	if (this->length < index) {
		this->digits = (char*)realloc(this->digits, index + 1);

		if (this->digits == NULL) {		// �Ҵ� ���� ����ó��
			cout << "Memory reallocation failed, the program will terminate." << endl;

			exit(0);
		}

		this->length = index;					// ���� ����
		this->digits[this->length] = '\0';	// �ι��� ����
	}

	if (this->digits[index - 1] < '0') {	// ���� ���� '0'���� ���� �ƽ�Ű���� ��� 0���� ä��. �������� �ʾҴ� ���ο� �ڸ����� ��� �߻�
		this->digits[index - 1] = '0';
	}
	int d = digits[index - 1] - 48;
	//this->digits[index - 1] =d*n+48;	// �� ����
	if (d * n > 9) {
		int add = d * n / 10;// �ڸ��ø��� �߻��� ���
		this->digits[index - 1] = 48 + (d * n) % 10;	// ���� �ڸ������� (�ƽ�Ű��) 10�� ����
		return add;
	}
	else {
		this->digits[index - 1] = d * n + 48;
		return 0;
	}

}




