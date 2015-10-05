#include "BigInt.h"
#include "BigIntModular.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
using namespace std;
int BigInt::Dim = 10;

void BigInt::set_dim(int x)
{
	if (x >= Dim)
		Dim = x;
}
void BigInt::incLength(int D)
{
	if (D <= 0)
		return;
	Num.insert(Num.begin(), D, 0);
	Length = Num.size();
}
void BigInt::decLength()
{
	int nullNum=0;
	for (int i = 0; i < Length&&!Num[i]; i++)
		nullNum++;
	if (!nullNum)
		return;
	else
	if (nullNum == Length)
	{
		Num.assign(1, 0);
		Length = Num.size();
		return;
	}
	else
	{
		Num.erase(Num.begin(), Num.begin() + nullNum);
		Length = Num.size();
		return;
	}
}

BigInt::BigInt()
{
	Num.assign(1, 0);
	Length = 1;
}
BigInt::BigInt(int x1)
{
	if (!x1)
	{
		Num.assign(1, 0);
		Length = 1;
		return;
	}

	int x = (x1>0) ? x1 : (-1 * x1);
	Num.assign((int)log10(x+0.0)+1,0);
	Length = (int)log10(x+0.0) + 1;

		
	for (int i = Length-1; x >0; i--, x /= 10)
		Num[i] = x % 10;

	if (x1 < 0)
		Num[0] *= (-1);
	
}
BigInt::BigInt(const vector<int>& num)//!!!
{
	
	Num.assign(num.begin(),num.end());
	Length = Num.size();
	decLength();
	Length = Num.size();
}

BigInt::BigInt(const BigInt& X)
{
	Length = X.Length;
	Num.assign(X.Num.begin(), X.Num.end());
}
BigInt::~BigInt()
{
	Num.clear();
}

BigInt& BigInt::operator=(const BigInt& X)//!!
{
	Length = X.Length;
	Num.assign(X.Num.begin(),X.Num.end());
	return *this;
}
BigInt operator-( const BigInt X)
{
	BigInt Y(X);
	Y.Num[0] *= -1;
	return Y;
}
BigInt& BigInt::operator += (const BigInt&Z)
{
	BigInt X(Z);
	if (sign(*this) == 1 && sign(X) == (-1))
		return *this -= (X);
	if (sign(*this) == (-1) && sign(X) == 1)
	{
		BigInt tmp;
		tmp=(-*this);
		*this = X;
		return(*this -= tmp);
	}
		
	int k = 1;
	if (Num[0] < 0 && X.Num[0] < 0)
	{
		k = -1;
		Num[0] *= k;
		X.Num[0] *= k;
	}

	if (Length < X.Length)
	{
		BigInt tmp(*this);
		*this = X;
		X = tmp;
	}
	int k1 = 0;
	for (int i = Length - 1, j = X.Length - 1; j >= 0; i--, j--)
	{
		int t;
		t = (Num[i] + X.Num[j] + k1) / 10;
		Num[i] = (Num[i] + X.Num[j]+k1) % 10;
		k1 = t;
	}
	for (int i = Length - X.Length - 1; k1&&i>=0; i--)
	{
		int t;
		t = (Num[i] + k1) / 10;
		Num[i] = (Num[i] + k1) % 10;
		k1 = t;
	}
	
	if (k1)
	{
		incLength(1);
		Num[0] = 1;
	}
	Num[0] *= k;
	return(*this);

}

BigInt operator+(const BigInt&X, const BigInt&Y)
{
	BigInt Z;
	Z=(X);
	Z += Y;
	return Z ;
}
BigInt& BigInt::operator++()
{
	*this += 1;
	return *this;
}
BigInt BigInt::operator++(int notused)
{
	BigInt ans(*this);
	*this += 1;
	return ans;
}
BigInt& BigInt::operator-= (const BigInt& X)
{
	if (sign(*this) == 1 && sign(X) == (-1))
		return *this += (-X);
	if (sign(*this) == (-1) && sign(X) == 1)
		return(*this += -X);
		
		int k=1;
		BigInt tmp(X);

		if (sign(*this) == (-1) && sign(tmp) == (-1))
		{
			if (*this*(-1) < tmp*(-1))
			{
				tmp = -*this;
				*this = -X;
				k = -1;
			}
			else
			{
				Num[0] *= -1;
				tmp.Num[0] *= -1;
				k = -1;
			}
		}
		

			
		if (*this < tmp)
		{
			BigInt t(tmp);
			tmp = *this;
			*this = t;
			k = -1;

		}

		int i = Length-1, j = tmp.Length-1;
		for (; j >= 0; j--, i--)
		{
			if (Num[i] >= tmp.Num[j])
				Num[i] -= tmp.Num[j];
			else
			{
				Num[i] = Num[i] + 10 - tmp.Num[j];
				int k;
				for (k = i-1; !Num[k]&&k>0; k--)
					Num[k] = 9;
				Num[k] -= 1;
			}
		}

	decLength();
	Num[0] *= k;
	return *this;
}
BigInt operator-(const BigInt&X, const BigInt&Y)
{
	BigInt Z;
	Z = X;
	return(Z -= Y);
}
BigInt& BigInt::operator--()
{
	*this -= 1;
	return *this;
}
BigInt BigInt::operator--(int notused)
{
	BigInt ans(*this);
	*this -= 1;
	return ans;
}
BigInt& BigInt::operator*= (const BigInt& X)
{
	if (X == 0||*this==0)
		return (*this = 0);

	BigInt Z;
	Z = X;
	int k = 1;
	if (*this < 0 && Z<0)
	{
		Num[0]*= -1;
		Z.Num[0]*= -1;
	}
	if (*this>0 && Z<0)
	{
		Z.Num[0] *= -1;
		k = -1;
	}
	if (*this<0 && Z>0)
	{
		Num[0] *= -1;
		k = -1;
	}

	if (Z.Length > Length)
	{
		BigInt tmp(*this);
		*this = Z;
		Z = tmp;
	}

	int**tab = new int*[Z.Length];
	for (int i = 0; i < Z.Length; i++)
		tab[i] =new int[Length];

	for (int i = 0; i < Z.Length; i++)
	for (int j = 0; j < Length; j++)
		tab[i][j] = Num[j] * Z.Num[i];

	
	int buf = 0;
	int i=Z.Length-1, j=Length-1;

	for (; j >= 0; j--)
	{
		for (int j1 = j, i1 = Z.Length - 1; (j1 < Length && i1 >= 0); j1++, i1--)
		{
			buf += tab[i1][j1] % 10;
			if (j1 < Length - 1 && i1 >= 0)
				buf += tab[i1][j1+1] / 10;
		}

		Num[j] = buf % 10;
		buf /= 10;
		}
///!!
	int pos = Dim-1;
	incLength();
	for (; i >= 0; i--)
	{
		for (int i1 = i, j1 = 0; i1 >= 0 && j1 < Length; i1--, j1++)
		{
			buf += tab[i1][j1] / 10;
			if (i1>0)
				buf += tab[i1 - 1][j1] % 10;;
		}
		if (pos == 0)
		{
			incLength();
			pos = Dim;
		}
		Num[pos] = buf % 10;
		buf /= 10;
		pos--;
	}

	for (int i = 0; i < Z.Length; i++)
		delete[]tab[i];
	delete[]tab;
	if (buf)
	{
		incLength();///Проверь, нужно ли добавлять
		for (int l = Dim - 1; l >= 0&&buf; l--)
		{
			Num[l] = buf % 10;
			buf /= 10;
			if (!l&&buf)
			{
				incLength();
				l = Dim;//смотри incLength()
			}
		}
	}
	decLength();
	Num[0] *= k;
	return *this;
}
BigInt operator*(const BigInt& X, const BigInt& Y)
{
	BigInt Z;
	Z = X;
	Z *= Y;
	return Z;
}


BigInt& BigInt::operator/=(const BigInt& X)
{
	if (*this < X || *this == 0)
		return (*this = 0);
	BigInt Z(X);
	int k = 1;

	if (*this < 0 && Z<0)
	{
		Num[0] *= -1;
		Z.Num[0] *= -1;
	}
	if (*this>0 && Z<0)
	{
		Z.Num[0] *= -1;
		k = -1;
	}
	if (*this<0 && Z>0)
	{
		Num[0] *= -1;
		k = -1;
	}
	
	while (!Z.Num[Z.Length - 1])
	{
		Z.Num.pop_back();
		Num.pop_back();
		Length--;
		Z.Length--;
	}
	if (Z == 1)
		return(*this *= k);
	int qS, rS;

	if (Length == Z.Length)
	{
		BigInt  Q;
		while (true)
		if (*this >= Z)
		{
			*this -= Z;
			Q++;
		}
		else
			break;
		return(*this = k*Q);
	}
	if (Z.Length == 1)
	{
		*this *= 10;
		Z *= 10;
	}

	
	int b = 10;
	int d = b / (Z.Num[0] + 1);
	if (d == 1)
		incLength(1);
	else
	{
		*this *= d;
		Z *= d;
		
	}	
	int M = Length - Z.Length;
	int N = Z.Length;
	vector<int> Q(M+1);
	Q.back() = -1;
	for (int j = 0,pos=0; j < M; j++,pos++)
	{

		qS = (Num[j] * b + Num[j + 1]) / Z.Num[0];
		rS = (Num[j] * b + Num[j + 1]) % Z.Num[0];
		

		while (true)
		if (qS == b || qS*Z.Num[1]>b*rS + Num[j + 2])
		{
			qS--;
			rS += Z.Num[0];
			if (rS < b)
				continue;
			else
				break;
		}
		else
			break;

		BigInt tmp,tmp1;
		tmp = Z*qS;
		vector<int> buf(N+1,0);
		for (int i = j; i<=j+N; i++)
			buf[i - j] = Num[i];
		tmp1 = array2BigInt(buf);
		tmp1.decLength();
		if (tmp > tmp1)
		{
			qS--;
			tmp = Z*qS;
		}
		
		tmp1 -= tmp;
		if (tmp1 >= Z)
		{
			tmp1 -= Z;
			qS++;
		}
		tmp1.incLength(N+1- tmp1.Length);
		Q[pos]=qS;
		if (qS > 9)
		{
			Q[pos] /= 10;
			pos++;
			Q[pos]=qS%10;

		}

		for (int i = j; i <= j + N;i++)
			Num[i] = tmp1.Num[i-j];

		
	}
	if (Q.back() < 0)
		Q.pop_back();

	*this = array2BigInt(Q);
	return(*this *= k);
}
BigInt operator/(const BigInt& X, const BigInt& Y)
{
	BigInt Z;
	Z = X;
	return(Z /= Y);
}

BigInt& BigInt::operator%=(const BigInt& X)
{
	if (abs(*this)<abs(X) )
		return (*this);
	BigInt Z(X);
	int k = 1, k1 = 1;

	if (*this < 0 && Z<0)
	{
		Num[0] *= -1;
		Z.Num[0] *= -1;
		k = k1 = -1;
	}
	if (*this>0 && Z<0)
	{
		Z.Num[0] *= -1;
		k = 1;
		k1 = -1;
	}
	if (*this<0 && Z>0)
	{
		Num[0] *= -1;
		k = -1;
		k1 = 1;
	}

	if (Z == 1)
		return(*this=0);
	

	if (Length == Z.Length)
	{
		while (*this>=Z)
			*this -= Z;
		return *this;
	}
	bool okMult10 = false;
	if (Z.Length == 1)
	{
		okMult10 = true;
		*this *= 10;
		Z *= 10;
	}

	int qS, rS;
	int b = 10;
	int d = b / (Z.Num[0] + 1);
	if (d == 1)
		incLength(1);
	else
	{
		*this *= d;
		Z *= d;
	}

	int M = Length - Z.Length;
	int N = Z.Length;
	for (int j = 0; j < M; j++)
	{
		qS = (Num[j] * b + Num[j + 1]) / Z.Num[0];
		rS = (Num[j] * b + Num[j + 1]) % Z.Num[0];
		while (true)
		if (qS == b || qS*Z.Num[1]>b*rS + Num[j + 2])
		{
			qS--;
			rS += Z.Num[0];
			if (rS < b)
				continue;
			else
				break;
		}
		else
			break;

		BigInt tmp, tmp1;
		tmp = Z*qS;
		vector<int> buf(N+1,0);
		for (int i = j; i <= j + N; i++)
			buf[i - j] = Num[i];
		tmp1 = array2BigInt( buf);

		if (tmp > tmp1)
		{
			qS--;
			tmp = Z*qS;
		}
		tmp1 -= tmp;
		if (tmp1 >= Z)
			tmp1 -= Z;
		tmp1.incLength(N + 1 - tmp1.Length);

		for (int i = j; i <= j + N; i++)
			Num[i] = tmp1.Num[i - j];
	}
	decLength();
	*this /= d;
	if (okMult10)
		*this /= 10;


	if (k>0 && k1>0 || k > 0 && k1 < 0)
	return *this;
	else 
		return (*this*=-1);
}
BigInt operator%(const BigInt& X, const BigInt&Y)
{
	BigInt Z;
	Z = X;
	return (Z %= Y);
}

bool operator< (const BigInt& Y, const BigInt&X)
{
	if (Y.Length == X.Length)
		return (Y.Num < X.Num);
	if (Y.Length<X.Length)
	{
		if (X.Num[0]>=0)
			return true;
		else
			return false;
	}
	else
	{
		if (Y.Num[0] < 0)
			return true;
		else
			return false;
	}
}
bool operator== (const BigInt& Y, const BigInt&X)
{
		return (Y.Num == X.Num);
}
bool operator!= (const BigInt& Y, const BigInt&X)
{
	return !(X == Y);
}
bool operator>(const BigInt& Y, const BigInt&X)
{
	return (!(X==Y||Y<X));
}
bool operator<= (const BigInt& Y, const BigInt&X)
{
	return (Y < X || Y == X);
}
bool operator>= (const BigInt& Y, const BigInt&X)
{
	return (Y > X || Y == X);
}
bool operator!(const BigInt& X)
{
	BigInt Y(0);
	return (Y==X);
}

BigInt randNum(const BigInt& fromNum, const BigInt&toNum)
{
	if (fromNum == toNum)
		return toNum;
	if (fromNum > toNum)
		return 0;

	int d = (toNum.gLength() - fromNum.gLength()) ? (toNum.gLength() - fromNum.gLength()):1;
	int length = fromNum.gLength() + rand() % d;
	vector<int>ans(length,0);
	if ( fromNum.gLength()<length && length < toNum.gLength())
	{
		ans[0] = 1 + rand() % 9;
		for (int i = 1; i < length; i++)
			ans[i] = rand() % 10;
	}
	else
	{
		vector<int> fN(BigInt2array(fromNum));
		vector<int> tN(BigInt2array(toNum));
		
		if (fromNum.gLength() == length && length < toNum.gLength())
		{
			bool ok = false;
			while (ans[0]<fN[0])
			ans[0] = 1 + rand() % 9;
			if (ans[0]>fN[0])
				ok = true;

			for (int i = 1; i < length; i++)
			{
				ans[i] = rand() % 10;
				if (!ok)
				{
					while (ans[i]<fN[i])
						ans[i] = fN[i] + rand() % (10 - fN[i]);
					if (ans[i]>fN[i])
						ok = true;
				}
			}
		}
		else
		if (fromNum.gLength() < length && length == toNum.gLength())
		{
			bool ok = false;
			while (ans[0]>tN[0])
				ans[0] = 1 + rand() % 9;
			if (ans[0]<tN[0])
				ok = true;

			for (int i = 1; i < length; i++)
			{
				ans[i] = rand() % 10;
				if (!ok)
				{
					while (ans[i]>tN[i])
						ans[i] = rand() % tN[i];
					if (ans[i] < tN[i])
						ok = true;
				}
			}
		}
		else
		if (fromNum.gLength()== toNum.gLength())
		{
			int d1 = (tN[0] - fN[0]>0) ? (tN[0] - fN[0]) : 1;
			bool ok1 = false, ok2 = false;
			ans[0] = 1 + rand() % 9;
			while (ans[0]>tN[0] || ans[0] < fN[0])
				ans[0] = fN[0] + rand() %d1 ;
			if (ans[0] > fN[0])
				ok1 = true;
			if (ans[0] < tN[0])
				ok2 = true;
			for (int i = 1; i < length; i++)
			{
				ans[i] = rand() % 10;
				if (!ok1&&!ok2)
				{
					d1 = (tN[i] - fN[i]>0) ? (tN[i] - fN[i]) : 1;
					while (ans[i]>tN[i] || ans[i] < fN[i])
						ans[i] = fN[i] + rand() % d1;
					if (ans[i] > fN[i])
						ok1 = true;
					if (ans[i] < tN[i])
						ok2 = true;
				}
				else
				if (!ok1)
				{
					while (ans[i]<fN[i])
						ans[i] = fN[i] + rand() % (10 - fN[i]);
					if (ans[i]>fN[i])
						ok1 = true;
				}
				else
				if (!ok2)
				{
					while (ans[i] > tN[i])
						ans[i] = rand() % tN[i];
					if (ans[i] < tN[i])
						ok2 = true;
				}
			}
		}
	}
	return array2BigInt(ans);
}

ostream& operator<<(ostream& out, const BigInt& X)
{
	for (int i = 0; i < X.Length; i++)
		out << X.Num[i];
	return out;
}
istream& operator>>(istream& in, BigInt& X)
{
	string str;
	in >> str;
	int k = (str[0] == '-') ? (-1) : 1;
	if (k < 0)
		str.erase(str.begin());

	vector<int> p(str.size());
	for (int i = 0; i < str.size(); i++)
		p[i] = str[i] - '0';
	X = array2BigInt(p);
	X *= k;
	return in;
}


int BigInt::gLength() const
{
	return Length;
}
//not work in VS2010
/*BigInt::operator int() const
{
	if (*this > INT_MAX)
		return INT_MAX;
	if (*this < -INT_MAX)
		return (-INT_MAX);
	int ans = 0;
	if (*this>0)
	for (int i = 0; i < Length; i++)
		ans = ans * 10 + Num[i];
	else
	{
		ans = Num[0];
		for (int i = 1; i < Length; i++)
			ans = ans * 10 - Num[i];
	}
	return ans;
}*/
int toInt(BigInt X)
{
	if (X > INT_MAX)
		return INT_MAX;
	if (X < -INT_MAX)
		return (-INT_MAX);
	int ans = 0;
	if (X>0)
		for (int i = 0; i < X.Length; i++)
			ans = ans * 10 + X.Num[i];
	else
	{
		ans = X.Num[0];
		for (int i = 1; i < X.Length; i++)
			ans = ans * 10 - X.Num[i];
	}
	return ans;

}

BigInt BigInt::mod10() const
{
	return Num.back();
}

BigInt array2BigInt(vector<int>& Array)
{
	BigInt X(Array);
	return X;
}
vector<int> BigInt2array(const BigInt& X)
{

	vector<int> ans(X.Num.begin(),X.Num.end());
	return ans;
}
int sign(const BigInt& Y)
{
	if (Y == 0)
		return 0;
	if (Y < 0)
		return (-1);
	else
		return 1;
}
BigInt str2BigInt(char*s)
{
	vector<int> ans(strlen(s));
	for(int i=0;i<strlen(s);i++)
		ans[i]=s[i]-'0';
	BigInt x(ans);
	return x;
}

char* BigInt2str(const BigInt& X)
{
	vector<int> ans;
	ans=BigInt2array(X);
	char* a=new char[ans.size()+1];
	for(int i=0;i<ans.size();i++)
		a[i]=ans[i]+'0';
	a[ans.size()]=0;
	return a;
}
