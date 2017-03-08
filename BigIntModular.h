#ifndef BIGINTEGER_MODULAR_H
#define BIGINTEGER_MODULAR_H
#include "BigInt.h"
#include <vector>
using namespace std;
void doFirst304PrimeNumber(void);

BigInt pow(const BigInt&Num, const BigInt&power);
BigInt nod(const BigInt&Num1, const BigInt&Num2);
BigInt abs(const BigInt&Num);
BigInt fact(const BigInt&Num);
BigInt log2(const BigInt&Num);
vector<int> from10to2(const BigInt&Num);
BigInt from2to10(const vector<int>& arrayOfBytes, int length);
BigInt randNumLength(int bytes,bool parity=1);


bool isPrimeNumber(const BigInt& X);
BigInt getPrimeNumber(const int bytes=5);
BigInt getPrimeNumber2(const int bytes);
BigInt getPrimeNumber3(const int bytes);


BigInt* qEuclidAlgorithm(const BigInt&Num1, const BigInt&Num2,int&length);
BigInt Continuant(BigInt*p,int length);
BigInt backMultMod(const BigInt&Num, const BigInt&Mod);
BigInt sqrMultMod(const BigInt&Num, const BigInt&pow_Num, const BigInt&Mod);

BigInt getInterSimpleNumber(const BigInt& Num);
#endif