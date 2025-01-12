#define _CRT_SECURE_NO_WARNINGS
#include "inf_int.h"
#include <ostream>
using namespace std;

/*
Originally written by
컴퓨터공학부
정진경
*/

inf_int::inf_int()
{
	this->digits = new char[2];	// 동적할당

	this->digits[0] = '0';		// default 값 0 설정
	this->digits[1] = '\0';
	this->length = 1;
	this->thesign = true;
}

inf_int::inf_int(int n) {
	char buf[100];

	if (n < 0) {		// 음수 처리
		this->thesign = false;
		n = -n;
	}
	else {
		this->thesign = true;
	}

	int i = 0;
	while (n > 0) {			// 숫자를 문자열로 변환하는 과정
		buf[i] = n % 10 + '0';

		n /= 10;
		i++;
	}

	if (i == 0) {	// 숫자의 절댓값이 0일 경우...
		new (this) inf_int();	// 생성자 재호출...gcc에서 컴파일에러가 있다고 함. inf_int()의 경우 별개의 인스턴스가 생성됨.
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
	if (str[0]=='-') {		// 음수 처리
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

	if (((this->thesign==false)&&(str[1]=='0'))||str[0]=='0') {	// 숫자의 절댓값이 0일 경우
		new (this) inf_int();	// 생성자 재호출...gcc에서 컴파일에러가 있다고 함. inf_int()의 경우 별개의 인스턴스가 생성됨. 
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
	delete digits;		// 메모리 할당 해제
}

inf_int& inf_int::operator=(const inf_int& a)
{
	if (this->digits) {
		delete this->digits;		// 이미 문자열이 있을 경우 제거.
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
	if ((strcmp(a.digits, b.digits) == 0) && a.thesign == b.thesign)	// 부호가 같고, 절댓값이 일치해야함.
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

	if (a.thesign == b.thesign) {	// 이항의 부호가 같을 경우 + 연산자로 연산
		for (i = 0; i < a.length; i++) {
			c.Add(a.digits[i], i + 1);
		}
		for (i = 0; i < b.length; i++) {
			c.Add(b.digits[i], i + 1);
		}

		c.thesign = a.thesign;

		return c;
	}
	else {	// 이항의 부호가 다를 경우 - 연산자로 연산
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
	else {	// 이항의 부호가 다를 경우 - 연산자로 연산
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
		
		if (this->digits == NULL) {		// 할당 실패 예외처리
			cout << "Memory reallocation failed, the program will terminate." << endl;

			exit(0);
		}

		this->length = index;					// 길이 지정
		this->digits[this->length] = '\0';	// 널문자 삽입
	}

	if (this->digits[index - 1] < '0') {	// 연산 전에 '0'보다 작은 아스키값인 경우 0으로 채움. 쓰여지지 않았던 새로운 자리수일 경우 발생
		this->digits[index - 1] = '0';
	}

	this->digits[index - 1] += num - '0';	// 값 연산


	if (this->digits[index - 1] > '9') {	// 자리올림이 발생할 경우
		this->digits[index - 1] -= 10;	// 현재 자릿수에서 (아스키값) 10을 빼고
		Add('1', index + 1);			// 윗자리에 1을 더한다
	}
}

void inf_int::Sub(const char num, const unsigned int index) {
	if (this->length < index) {
		this->digits = (char*)realloc(this->digits, index + 1);

		if (this->digits == NULL) {		// 할당 실패 예외처리
			cout << "Memory reallocation failed, the program will terminate." << endl;
			exit(0);
		}
		this->length = index;					// 길이 지정
		this->digits[this->length] = '\0';	// 널문자 삽입
	}

	this->digits[index - 1] -= num - '0';	// 값 연산


	if (this->digits[index - 1] < '0') {	// 자리올림이 발생할 경우
		this->digits[index - 1] += 10;	// 현재 자릿수에서 (아스키값) 10을 빼고
		Sub('1', index + 1);			// 윗자리에 1을 더한다
	}
}

bool inf_int::Add2(const char num, const unsigned int index)	// a의 index 자리수에 n을 더한다. 0<=n<=9, ex) a가 391일때, Add(a, 2, 2)의 결과는 411
{
	if (this->length < index) {
		this->digits = (char*)realloc(this->digits, index + 1);

		if (this->digits == NULL) {		// 할당 실패 예외처리
			cout << "Memory reallocation failed, the program will terminate." << endl;

			exit(0);
		}

		this->length = index;					// 길이 지정
		this->digits[this->length] = '\0';	// 널문자 삽입
	}

	if (this->digits[index - 1] < '0') {	// 연산 전에 '0'보다 작은 아스키값인 경우 0으로 채움. 쓰여지지 않았던 새로운 자리수일 경우 발생
		this->digits[index - 1] = '0';
	}

	this->digits[index - 1] += num - '0';	// 값 연산


	if (this->digits[index - 1] > '9') {	// 자리올림이 발생할 경우
		this->digits[index - 1] -= 10;	// 현재 자릿수에서 (아스키값) 10을 빼고
		return true;			// 윗자리에 1을 더한다
	}
	else false;

}

int inf_int::Mul(const char num, const unsigned int index)	// a의 index 자리수에 n을 더한다. 0<=n<=9, ex) a가 391일때, Add(a, 2, 2)의 결과는 411
{
	int n = num - 48;
	if (this->length < index) {
		this->digits = (char*)realloc(this->digits, index + 1);

		if (this->digits == NULL) {		// 할당 실패 예외처리
			cout << "Memory reallocation failed, the program will terminate." << endl;

			exit(0);
		}

		this->length = index;					// 길이 지정
		this->digits[this->length] = '\0';	// 널문자 삽입
	}

	if (this->digits[index - 1] < '0') {	// 연산 전에 '0'보다 작은 아스키값인 경우 0으로 채움. 쓰여지지 않았던 새로운 자리수일 경우 발생
		this->digits[index - 1] = '0';
	}
	int d = digits[index - 1] - 48;
	//this->digits[index - 1] =d*n+48;	// 값 연산
	if (d * n > 9) {
		int add = d * n / 10;// 자리올림이 발생할 경우
		this->digits[index - 1] = 48 + (d * n) % 10;	// 현재 자릿수에서 (아스키값) 10을 빼고
		return add;
	}
	else {
		this->digits[index - 1] = d * n + 48;
		return 0;
	}

}




