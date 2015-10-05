#ifndef BigInt_H
#define BigInt_H
#include <iostream>
#include <vector>
#include <string>
using namespace std;
class BigInt
{
private:
	
	vector<int> Num;	
	int Length;
	void incLength(int=Dim);
	void decLength(void);
	static int Dim;

public:	
	
	static void set_dim(int);
	int gLength(void)const;
	BigInt(void);
	BigInt(int);
	BigInt(const vector<int>&);
	BigInt(const BigInt&);
	~BigInt(void);

	BigInt& operator=(const BigInt& );
	
	BigInt& operator+= (const BigInt& );
	BigInt& operator++();
	BigInt operator++(int notused);


	BigInt& operator-= (const BigInt& );
	
	BigInt& operator--();
	BigInt operator--(int notused);

	BigInt& operator*= (const BigInt&);
	

	BigInt& operator/=(const BigInt&);
	BigInt& operator%=(const BigInt&);
	
	

	friend BigInt  operator-(const BigInt);

	BigInt mod10(void) const;


	friend bool operator< (const BigInt&, const BigInt&);
	friend bool operator == (const BigInt&, const BigInt&);
	friend bool operator> (const BigInt&, const BigInt&);
	friend bool operator<= (const BigInt&, const BigInt&);
	friend bool operator>= (const BigInt&, const BigInt&);
	friend bool operator!= (const BigInt&, const BigInt&);

	friend istream& operator>>(istream&, BigInt&);
	friend ostream& operator<<(ostream& , const BigInt&);
	
	//explicit operator int(void)const; not working
	friend int toInt(BigInt);

	friend vector<int> BigInt2array(const BigInt& Num);
	

};	



bool operator! (const BigInt&);
BigInt operator+(const BigInt&, const BigInt&);
BigInt operator-(const BigInt&, const BigInt&);
BigInt  operator*(const BigInt&, const BigInt&);
BigInt operator/(const BigInt&, const BigInt&);
BigInt operator%(const BigInt&, const BigInt&);

int sign(const BigInt&);
BigInt randNum(const BigInt& fromNum, const BigInt&toNum);
BigInt array2BigInt( vector<int>& Array);

BigInt str2BigInt(char*);
char* BigInt2str(const BigInt& X);


#endif