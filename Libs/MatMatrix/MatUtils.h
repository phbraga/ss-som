#ifndef MATUTILS_H_
#define MATUTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ostream>
#include <sys/time.h>
#include <time.h>
#include <math.h>


#define DefaultNumberType float
#define INVALID_DIMENSION_SIZE "Dimensao invalida"
#define DIVIDE_BY_ZERO "Divisao por zero"
#define INVALID_TYPE "Tipo invalido"
#define INVALID_PARAMETER "Parametro inválido"
#define MEM_NO_ALLOC "Memoria nao alocada"
#define INVALID_FUNCTION_FOR_THIS_TYPE "Esta funcao nao pode ser utilizada com esse tipo"
#define errMsg(___msg) (std::cerr << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << "::" << (___msg) << std::endl)
#define testMsg(___msg) (std::cout << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << "::" << (___msg) << std::endl)

typedef unsigned int uint;
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef const int cint32;
typedef const short cint16;
typedef int int32;
typedef const unsigned int cuint32;
typedef unsigned long long uint64;
typedef const unsigned long long cuint64;
typedef long long int64;
typedef const long long cint64;


namespace MatUtils {

static double ticTocElapsedTime;
//As macros PI xmax e mmxmin estavam dando conflito com CImg
//por isto MM (MatMatix) foi acrescentado no começo de cada macro
//const int NULL=0;
//#pragma "-w"

const double PI=3.1415926535897932385;
const double HALF_PI=1.5707963267948966192;
const double TWO_PI=6.2831853071795864769;
const double SQR_PI=9.8696044010893586188;
const double SQRT_PI=1.7724538509055160273;
const double E=2.7182818284590452354;
const double NaN=0.0/0.0;
const double Inf=1.0/0.0;
//#pragma "-Wall"


double getCurrentRealTimer(void);

double getCurrentCPUTimer(void);

inline void tic(void) { ticTocElapsedTime=getCurrentRealTimer(); };

inline double toc(void) { return (getCurrentRealTimer()-ticTocElapsedTime); };

//Tempo em segundos
inline void delay(double time=1.0)
{
	double start=getCurrentRealTimer();
	double delta;
	do
	{
		delta=getCurrentRealTimer()-start;
	}while(delta<time);
}

template<class Number> inline const Number max(const Number &a,const Number &b)
{
	return a>b ? a:b;
}

#ifndef MAX
#define MAX(a,b) (a>b ? a:b)
#endif

inline float max(const float &a,const float &b)
{
	return a>b ? a:b;
}

inline double max(const double &a,const double &b)
{
	return a>b ? a:b;
}

#ifndef MIN
#define MIN(a,b) (a>b ? b:a)
#endif

template<class Number> inline const Number min(const Number &a,const Number &b)
{
	return a>b ? b:a;
}

inline float min(const float &a,const float &b)
{
	return a>b ? b:a;
}

inline double min(const double &a,const double &b)
{
	return a>b ? b:a;
}

inline float abs(const float &a)
{
	return a>=0 ? a:-a;
}

inline double abs(const double &a)
{
	return a>=0 ? a:-a;
}

template<class Number> inline void swap(Number &a,Number &b)
{
	Number temp=a;
	a=b;
	b=temp;
}

inline void swap(float &a,float &b)
{
	float temp=a;
	a=b;
	b=temp;
}

inline void swap(double &a,double &b)
{
	double temp=a;
	a=b;
	b=temp;
}

template<class Number> inline float signal(const Number &a)
{
	return a>0 ? 1.0f:(a<0 ? -1.0f:0.0f);
}

inline float signal(const float &a)
{
	return a>0 ? 1.0f:(a<0 ? -1.0f:0.0f);
}


//inline const double NaN(void)
//{
//	return sqrt(-1.0);
//}

//Gera um ruído uniforme entre 0 e 1
//#define unoise() ((DefaultNumberType)rand()/(RAND_MAX-1))
template<class Number> inline const Number unoise(void)
{
	return ((Number)rand()/(RAND_MAX-1));
}

/**
 * Gera um n�mero aleat�rio de uma distribui��o uniforme entre leftBound(limite inferior) e rightBound(limite superior)
 */
template<class Number> inline const Number unoise(const Number &lowerBound,const Number &upperBound)
{
	return ((double)rand()/(RAND_MAX-1))*(upperBound-lowerBound)+lowerBound;
}

//Gera um ruído com distribuição normal com média m e desvio d
//#define nnoise(m,d) (sqrt(-2*log(unoise()))*cos(PI2*unoise())*d+m)
template<class Number> inline const Number nnoise(const Number &m,const Number &d)
{
	return (sqrt(-2*log(unoise<Number>()))*cos(TWO_PI*unoise<Number>())*d+m);
}

inline unsigned int random(unsigned int size)
{
	return (size == 0 ? 0 : (unsigned int)((size)*((float)rand()/(RAND_MAX-1))));
}

inline int randomN(const int size)
{
	return (size == 0 ? 0 : (unsigned int)((size)*((float)rand()/(RAND_MAX-1))));
}

template<class Number> inline Number lfat(const unsigned int k)
{
	const unsigned int memSize=80;
	static Number fatMem[memSize];

	if(k<=1)
		return 0.0;
	else
	if(k<memSize)
	{
		return (fatMem[k] != 0.0 ? fatMem[k] : (fatMem[k]=lgamma(k+1.0)));
	}
	else
		return lgamma(k+1.0);

}

template<class Number> inline Number fat(const unsigned int k)
{

	const unsigned int memSize=25;
	static unsigned int memTop=11;
	static Number fatMem[memSize]={1,1,2,6,24,120,720,5040,40320,362880,3628800,39916800};

	if(k<=memTop)
		return fatMem[k];
	else
	{
		if(k<memSize)
		{

			Number fatRet=fatMem[memTop];
			unsigned int i;

			for(i=(memTop+1);i<=k;i++)
			{
				fatRet*=++memTop;
				fatMem[memTop]=fatRet;
			}

			return fatRet;
		}
		else
		{
			return exp(lgamma(k+1.0));
		}
	}
}

template<class Number> inline Number comb(const unsigned int n,const unsigned int k)
{
	return floor(0.5+exp(lfat<Number>(n)-lfat<Number>(k)-lfat<Number>(n-k)));
}

template<class Number> inline Number perm(const unsigned int n,const unsigned int k)
{
	return floor(0.5+exp(lfat<Number>(n)-lfat<Number>(n-k)));
}

int64 hmsTime2Milliseconds(std::string timeStr,char fieldSeparator0=':',char fieldSeparator1=':',char decimalSeparator='.');


}

#endif /*MATUTILS_H_*/
