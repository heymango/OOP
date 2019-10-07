#pragma once
#ifndef _INF_INT_H_
#define _INF_INT_H_

#include <iostream>
#include <ostream>
using namespace std;
class inf_int
{
public:
	char* digits;
	unsigned int length;
	bool thesign;   
	

	inf_int();               // assign 0 as a default value
	inf_int(int);
	inf_int(const char*);
	inf_int(const inf_int&); // copy constructor
	~inf_int(); // destructor

	inf_int& operator=(const inf_int&); // assignment operator

	void Add(const char num, const unsigned int index);

	friend bool operator==(const inf_int&, const inf_int&);
	friend bool operator!=(const inf_int&, const inf_int&);
	friend bool operator>(const inf_int&, const inf_int&);
	friend bool operator<(const inf_int&, const inf_int&);

	friend inf_int operator+(const inf_int&, const inf_int&);
	friend inf_int operator-(const inf_int&, const inf_int&);
	//friend inf_int operator*(const inf_int&, const inf_int&);
	// friend inf_int operator/(const inf_int& , const inf_int&); // not required
	friend ostream& operator<<(ostream&, const inf_int&);
	 //friend istream& operator>>(istream& , inf_int&);    // not required
};

#endif