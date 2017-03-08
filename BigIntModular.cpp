#include "BigInt.h"
#include "BigIntModular.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

const int firstPrimeTo2000 = 304;
int*firstPrimeNum= new int[firstPrimeTo2000];
static bool inDoFirst304PrimeNumber = false;

void doFirst304PrimeNumber()
{
	if (inDoFirst304PrimeNumber)
		return;
	firstPrimeNum[0] = 1;
	firstPrimeNum[1] = 2;
	for (int i = 3, k = 2; i < 2000; i += 2)
	{
		bool ok;
		ok = true;
		for (int j = 2; j <= i / 2; j++)
		if (i%j == 0)
		{
			ok = false;
			break;
		}
		if (ok)
		{
			firstPrimeNum[k] = i;
			k++;
		}
	}
	inDoFirst304PrimeNumber = true;
}



BigInt pow(const BigInt& X, const BigInt&Y)
{
	if (Y < 0)
		return 0;
	if (Y == 0)
		return 1;
	BigInt ans(1),x(X),y(Y);
	while (y!=0)
	{
		if (y % 2 == 0)
		{
			y /= 2;
			x *= x;
		}
		else
		{
			y--;
			ans *= x;
		}
	}
	return ans;
}
BigInt nod(const BigInt& X,const BigInt& Y)
{
	BigInt x(X), y(Y);
	if (!x || !y)
		return (!x ? y : x);
	if (x < 0)
		x *= -1;
	if (y < 0)
		y *= -1;
	if (y>x)
		swap(x, y);

	while (y!=0)
	{	
		x %= y;
		swap(x, y);
	}
	return x;
}
BigInt* qEuclidAlgorithm(const BigInt&X, const BigInt&Y,  int&Length)
{
	
	BigInt x(X), y(Y);	
	if (x < 0)
		x *= -1;
	if (y < 0)
		y *= -1;
	if (y>x)
		swap(x, y);

	int D = 5 * ((x > y) ? y.gLength() : x.gLength());
	BigInt*ans = new BigInt[D];
	Length = 0;
	if (!x)
	{
		ans[0] = y;
		Length++;
		return ans;
	}
	if (!y)
	{
		ans[0] = x;
		Length++;
		return ans;
	}
	
	for (int i = 0; y!=0;i++)
	{
		ans[i] = x/y;
		x %= y;
		
		Length++;
		swap(x, y);
	}
	if (Length>1)
	Length--;
	return ans;
}
BigInt abs(const BigInt&X)
{
	if (X < 0)
		return(X*(-1));
	else
		return X;
}
BigInt fact(const BigInt&X)
{
	if (X == 1)
		return X;

	return fact(X - 1)*X;
}

BigInt log2(const BigInt& X)
{
	BigInt x(X),ans;
	while (x>3)
	{

		ans += x.gLength() * 2;
		x /= pow((BigInt)2, (BigInt)x.gLength() * 2);
	}
	if (x > 1)
	{
		x /= 2;
		ans += 1;
	}
	return ans;
}

vector<int> from10to2(const BigInt&Num)
{
	BigInt x(Num);
	vector<int> ans;
	while (x > 0)
	{
		ans.push_back(toInt(x.mod10()%2));
		x /= 2;	
	}
	
	for (int i = 0; i < ans.size() / 2; i++)
	{
		int tmp;
		tmp = ans[i];
		ans[i] = ans[ans.size() - i - 1];
		ans[ans.size() - i - 1] = tmp;
	}

	return ans;	
}
BigInt from2to10(const vector<int>& arrayOfBytes, int length)
{
	BigInt ans;
	for (int i = length - 1; i >= 0; i--)
	{
		
		if (arrayOfBytes[i])
		{
			int k , pos;
			pos = i;
			k = 1;
			for (i--; i >= 0  ; i--, k++)
			if (!arrayOfBytes[i])
				break;
			if (k > 1)
				ans += -(pow((BigInt)2, length - 1 - pos)) * (1 - pow((BigInt)2, k));
			else
				ans += pow((BigInt)2, length - 1 - (i + 1));
		}	
	}
	return ans;
}

BigInt randNumLength(int bytes, bool parity)
{
	vector<int> p(bytes, 0);
	p[0] = 1 + rand() % 9;
	for (int i = 1; i < bytes; i++)
		p[i] = rand() % 10;
	BigInt ans(p);
	if (parity != p.back()%2)
	{
		ans -= 1;
		if (ans.gLength() < bytes)
			ans += 2;
	}
	return ans;
}
BigInt Continuant(BigInt*p, int length)
{
	BigInt ans, k,tmp;
	k = 1;
	ans = p[0];
	for (int i = 1; i < length; i++)
	{
		tmp = ans;
		ans = ans*p[i] + k;
		k = tmp;
	}
	return ans;
}
BigInt backMultMod(const BigInt&Num, const BigInt&Mod)
{
	int length = 0;
	BigInt*p = qEuclidAlgorithm(Num, Mod, length);
	BigInt ans;
	ans = (int)pow(-1.0,(int) length)*Continuant(p, length);
	if (ans < 0)
		ans += (abs(ans) / Mod + 1)*Mod;
	return ans;
}

BigInt sqrMultMod(const BigInt&Num, const BigInt&pow_Num, const BigInt&Mod)
{
	int length = 0;
	BigInt ans;
	vector<int> p = from10to2(pow_Num/*, length*/);///!!!!!!!
	ans = Num%Mod;
	for (int i = 1; i <p.size(); i++)
	{
		ans = (ans*ans*pow(Num, p[i])) % Mod;
	}
	return ans;
}
bool isPrimeNumber(const BigInt& X)
//it is test of Miller-Rabin
{
	if (!inDoFirst304PrimeNumber)
	{
		doFirst304PrimeNumber();
	}

	for (int i = 1; i < firstPrimeTo2000&&X>firstPrimeNum[i] /*&& firstPrimeNum[i]<256*/; i++)
	if (!(X%firstPrimeNum[i]))
		{
			return false;
	}
	if (X < 2000)
		return true;
	int r =/*(int)log2(X)*/5;
	BigInt parity;//power of 2
	BigInt x(X - 1);
	while (!(x % 2))
	{
		parity++;
		x /= 2;
	}
	BigInt t(x);//t=2n+1
	bool ok = false;
	for (int i = 0; i < r; i++)//main ñycle
	{
		ok = false;
		BigInt a;
		a= randNum(2,/*X - 3*/50);
		x = sqrMultMod(a, t, X);

		if (x == 1 || x == X - 1)
			continue;
		for (int j = 0; j < parity - 1; j++)
		{
			x = (x*x) % X;
			if (x == 1)
				return false;
			if (x == X - 1)
			{
				ok = true;
				break;
			}
		}
		if (!ok)
			return false;
	}
	return true;
}
BigInt getPrimeNumber(const int bytes)
{
	if (!inDoFirst304PrimeNumber)
	doFirst304PrimeNumber();
	BigInt ans;
	ans = randNumLength(bytes);
	//while (!ans)
	//ans = randNum(pow((BigInt)10, bytes));
	/*if (!((int)(ans % 10) % 2))
	{
		ans -= 1;
	}*/
	//cout << ans << endl;
	//cin.get();
	ofstream out("hello.txt");
	
	while (!isPrimeNumber(ans))
	{
		out<<bytes<<' '<<ans<<endl;
		ans += 2;
		//cout << ans << endl;
	}
	return ans;
}
BigInt getPrimeNumber2(const int bytes)
{
	BigInt S,R,N;
	if(bytes<=10)
		return getPrimeNumber(bytes);
	S = getPrimeNumber(10);
	//cout << "Begin: " << S << endl;
	while (S.gLength()<bytes)
	{
		R = randNum(S, 4 * S + 2);

		if (toInt(R.mod10() % 2))
			R += 1;
		N = S*R + 1;
		if (!isPrimeNumber(N))
			continue;
		else
		{
			S = N;
		}
	}
	return S;
}
BigInt getPrimeNumber3(const int bytes)
{
	BigInt S, N;
	S = getPrimeNumber(24);
	//cout << "Begin: " << S << endl;
	while (S.gLength() < bytes)
	{
		N = 6 * S + 1;
		if (!isPrimeNumber(N))
			S ++;
		else
		{
			cout << "PRIME NUMBER!!!" << endl << N << endl;
			cout << N.gLength() << endl;
			S = N * N + 1;
		}
	}
	return S;

}


BigInt getInterSimpleNumber(const BigInt& Num)
{
	BigInt ans;
	ans = randNum(3, Num-1);
	if (!ans.mod10() % 2)
		ans -= 1;
	while (nod(Num, ans) != 1)
	{
		ans = randNum(3, Num);
		if (!ans.mod10() % 2)
			ans -= 1;
	}
	return ans;
}
