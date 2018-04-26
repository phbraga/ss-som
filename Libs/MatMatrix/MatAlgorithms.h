#ifndef MATALGORITHMS_H_
#define MATALGORITHMS_H_

#include <iostream>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <MatUtils.h>


//Definições
//O objetivo principal dos métodos definidos aqui é servir de base para a contrução de outros métodos
//mais complexos e de mais alto nivel. Por isso as definições aqui devem utilizar, preferencialmente,
//templates, serem declaradas inline e não realizarem alocação/desalocação de memória.
//Os métodos neste arquivo devem conter somente as rotinas essencias ao funcionamento do algoritmo,
//sem conter procedimentos de checagem de tamanha dos vetores e matrizes. Estes funcionalidades
//devem estar presentes apenas nas rotinas de mais alto nível que chamarem os métodos aqui contidos.

//Notação
//Todos os métodos que trabalham somente com escalares iniciam com mat*
//Todos os métodos que trabalham com vetores iniciam com matV*
//Todos os métodos que trabalham com matrizes iniciam com matM*
//Todos os métodos que trabalham com vetores e matrizes iniciam com matMV*
//O nome padrão de template para escalares � Number
//O nome padrão de template para vetores � TVector
//O nome padrão de template para matrizes � TMatrix

//#pragma GCC diagnostic error "-Wall"


template<class TVector> void printVector(TVector &v,const unsigned int size,std::string label="")
{
	using namespace std;
	register unsigned int i;

	cout << label;
	for(i=0;i<size;i++)
		cout << v[i] << " ";

	cout << endl;
}

template<class TMatrix> void printMatrix(TMatrix &m, cuint32 mm, cuint32 nn,std::string label="",std::string separator="\n")
{
	using namespace std;
	uint i,j;

	cout << label;
	for(i=0;i<mm;i++)
	{
		for(j=0;j<nn;j++)
			cout << m[i][j] << " ";
		cout << separator;
	}
}

template<class TVector> inline void matVCopy(TVector &v2,const TVector &v1,const unsigned int size)
{
	register unsigned int i;
	for(i=0;i<size;i++)
	{
		v2[i]=v1[i];
	}
}

template<class TVector> inline bool matVIsEqual(const TVector &v2, const TVector &v1, unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		if(v2[i]!=v1[i])
			return false;

	return true;
}

template<class Number,class TVector> inline void matVFill(TVector &v,unsigned int size,const Number &x)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=x;
}

template<class Number,class TVector> inline void matVFillUnoise(TVector &v,const unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=MatUtils::unoise<Number>();
}

template<class Number, class TVector> inline void matVFillNNoise(TVector &v, const unsigned int size, const Number &m, const Number &dev)
{
    register unsigned int i;

    for(i=0;i<size;i++)
        v[i]=MatUtils::nnoise(m,dev);
};

template<class Number, class TVector> inline void matVFillNNoise(TVector &v, const unsigned int size, const TVector &vm, const Number &dev)
{
    register unsigned int i;

    for(i=0;i<size;i++)
        v[i]=MatUtils::nnoise(vm[i],dev);
};

template<class TVector> inline void matVFillNNoise(TVector &v, const unsigned int size, const TVector &vm, const TVector &vdev)
{
    register unsigned int i;

    for(i=0;i<size;i++)
        v[i]=MatUtils::nnoise(vm[i],vdev[i]);
};


template<class Number> inline int compareInc (const void * a, const void * b)
{
	if(*(Number*)a>*(Number*)b)
		return 1;
	else
	if(*(Number*)a==*(Number*)b)
		return 0;
	else
		return -1;
}

template<class Number> inline int compareDec (const void * a, const void * b)
{
	if(*(Number*)a<*(Number*)b)
		return 1;
	else
	if(*(Number*)a==*(Number*)b)
		return 0;
	else
		return -1;
}

template<class Number> inline void matVQsort(Number v[],const unsigned int size)
{
	  qsort (v, size, sizeof(Number), compareInc<Number>);
}

template<class Number> inline void matVQsortDec(Number v[],const unsigned int size)
{
	  qsort (v, size, sizeof(Number), compareDec<Number>);
}

template<class Number,class TVector> inline void matVReplaceLower(TVector &v,const unsigned int size,const Number &limiar,const Number &replacement)
{
	register unsigned int i;
	for(i=0;i<size;i++)
	{
		if(v[i]<limiar)
			v[i]=replacement;
	}
}

template<class Number,class TVector> inline void matVReplaceUpper(TVector &v,const unsigned int size,const Number &limiar,const Number &replacement)
{
	register unsigned int i;
	for(i=0;i<size;i++)
	{
		if(v[i]>limiar)
			v[i]=replacement;
	}
}

template<class Number,class TVector> inline void matVReplace(TVector &v,const unsigned int size,const Number &target,const Number &replacement)
{
	register unsigned int i;
	for(i=0;i<size;i++)
	{
		if(v[i]==target)
			v[i]=replacement;
	}
}

template<class Number,class TVector> inline Number matVMax(const TVector &v, const unsigned int size)
{
	register unsigned int i;
	Number vmax=v[0];

	for(i=1;i<size;i++)
	{
		if(vmax<v[i])
			vmax=v[i];
	}

	return vmax;
}

template<class TVector> inline void matVMax(TVector &vmax, const TVector &v, const unsigned int size)
{
    register unsigned int i;
    for(i=0;i<size;i++)
    {
        vmax[i]<v[i] ? vmax[i]=v[i]:0;
    }
}

template<class Number, class TVector> inline Number matVMaxIndex(const TVector &v,uint &imax,const unsigned int size)
{
	register unsigned int i;

	imax=0;
	for(i=1;i<size;i++)
	{
		if(v[imax]<v[i])
			imax=i;
	}

	return v[imax];
}

template<class Number,class TVector> inline Number matVMin(const TVector &v, const unsigned int size)
{
	register unsigned int i;
	Number vmin=v[0];

	for(i=1;i<size;i++)
	{
		if(vmin>v[i])
			vmin=v[i];
	}

	return vmin;
}

template<class TVector> inline void matVMin(TVector &vmin, const TVector &v, const unsigned int size)
{
    register unsigned int i;
    for(i=0;i<size;i++)
    {
        vmin[i]>v[i] ? vmin[i]=v[i]:0;
    }
}

template<class Number, class TVector> inline Number matVMinIndex(const TVector &v,uint &imin,const unsigned int size)
{
	register unsigned int i;

	imin=0;
	for(i=1;i<size;i++)
	{
		if(v[imin]>v[i])
			imin=i;
	}

	return v[imin];
}


template<class Number,class TVector> inline void matVMinMax(TVector &v, const unsigned int size, Number &vmin, Number &vmax)
{
	register unsigned int i;

	vmin=v[0];
	vmax=v[0];

	for(i=1;i<size;i++)
	{
		if(vmin>v[i])
			vmin=v[i];

		if(vmax<v[i])
			vmax=v[i];
	}
}

template<class Number,class TVector> inline Number matVMean(const TVector &v, const unsigned int size)
{
	register unsigned int i;
	Number vmean=0;

	for(i=0;i<size;i++)
		vmean+=v[i];

	return vmean/size;
}

template<class Number,class TVector> inline Number matVVar(const TVector &v, const unsigned int size)
{
	//size preciza ser >=2
	register unsigned int i;
	Number vvar=0;
	Number m=matVMean<Number>(v,size);

	for(i=0;i<size;i++)
	{
		vvar+=(v[i]-m)*(v[i]-m);
	}

	return vvar/(size-1);
}

template<class Number,class TVector> inline Number matVSum(const TVector &v,const unsigned int size)
{
	register unsigned int i;
	Number vsum=0;

	for(i=0;i<size;i++)
	{
		vsum+=v[i];
	}

	return vsum;
}



template<class Number,class TVector> inline Number matVProd(const TVector &v, unsigned int size)
{
	register unsigned int i;
	Number vsum=1;

	for(i=0;i<size;i++)
	{
		vsum*=v[i];
	}

	return vsum;
}

template<class Number,class TVector> inline Number matVSortAndGetMedian(TVector &v, unsigned int size)
{
	// matVSortAndMedian modifica o vetor v
	matVQsort(v,size);

	return (size%2)==0 ? (v[size/2]+v[size/2-1])/2 : v[(size-1)/2];
}


template<class Number,class TVector> inline void matVStatistics(Number &vmax,Number &vmin,Number &vsum, Number &vmean, Number &vvar,const TVector v, const unsigned int size)
{
	register unsigned int i;
	vsum=v[0];
	vmax=v[0];
	vmin=v[0];

	for(i=1;i<size;i++)
	{
		vsum+=v[i];
		if(vmax<v[i])
			vmax=v[i];

		if(vmin>v[i])
			vmin=v[i];
	}

	vmean=vsum/size;

	vvar=0;
	for(i=0;i<size;i++)
	{
		vvar+=(v[i]-vmean)*(v[i]-vmean);
	}

	vvar=vvar/(size-1);
}

template<class Number,class TVector> inline void matVSortAndGetOrderStatistics(Number &vmin,Number &quartile1, Number &median, Number &quartile3,Number &vmax, TVector &v, unsigned int size)
{
	unsigned int sizeTemp;
	matVQsort<Number>(v,size);
	vmin=v[0];
	vmax=v[size-1];

	if((size%2)==0)
	{
		sizeTemp=size/2;
		median=(v[sizeTemp]+v[sizeTemp-1])/2;
		quartile1=((sizeTemp%2)==0 ? (v[sizeTemp/2]+v[sizeTemp/2-1])/2 : v[(sizeTemp-1)/2]);
		quartile3=((sizeTemp%2)==0 ? (v[sizeTemp+sizeTemp/2]+v[sizeTemp+sizeTemp/2-1])/2 : v[sizeTemp+(sizeTemp-1)/2]);
	}
	else
	{
		sizeTemp=(size-1)/2;
		median=v[sizeTemp+1];
		quartile1=((sizeTemp%2)==0 ? (v[sizeTemp/2]+v[sizeTemp/2-1])/2 : v[(sizeTemp-1)/2]);
		quartile3=((sizeTemp%2)==0 ? (v[sizeTemp+1+sizeTemp/2]+v[sizeTemp+sizeTemp/2])/2 : v[sizeTemp+1+(sizeTemp-1)/2]);
	}
}

template<class Number,class TVector> inline Number matVCov(const TVector &v2, const TVector &v1, const unsigned int size)
{
	Number xm=0,ym=0,ac=0;
	register unsigned int i;

	for(i=0;i<size;i++)
	{
		xm+=v1[i];
		ym+=v2[i];
	}
	xm=xm/size;
	ym=ym/size;

	for(i=0;i<size;i++)
		ac+=(v1[i]-xm)*(v2[i]-ym);

	return ac/(size-1);
}


template<class Number,class TVector> inline Number matVCorr(const TVector &v1, const TVector &v2,const unsigned int size)
{
	Number xm=0,ym=0,acXY=0,acX=0,acY=0,tempX=0,tempY=0;
	register unsigned int i;

	for(i=0;i<size;i++)
	{
		xm+=v1[i];
		ym+=v2[i];
	}
	xm=xm/size;
	ym=ym/size;

	for(i=0;i<size;i++)
	{
		tempX=(v1[i]-xm);
		tempY=(v2[i]-ym);

		acXY+=tempX*tempY;
		acX+=tempX*tempX;
		acY+=tempY*tempY;
	}

	return acXY/(sqrt(acX)*sqrt(acY));
}

template<class Number,class TVector> inline Number matVDot(const TVector &v1,const TVector &v2, unsigned int size)
{
	register unsigned int i;
	Number vdot=0;

	for(i=0;i<size;i++)
	{
		vdot+=v1[i]*v2[i];
	}

	return vdot;
}

template<class Number,class TVector> inline void matVCross(const TVector &v2,const TVector &v1,TVector &vout)
{
	vout[0]=v2[1]*v1[2]-v2[2]*v1[1];
	vout[1]=v2[2]*v1[0]-v2[0]*v1[2];
	vout[2]=v2[0]*v1[1]-v2[1]*v1[0];
}

template<class Number,class TVector> inline Number matVDist2(const TVector &v2,const TVector &v1,unsigned int size)
{
	register unsigned int i;
	Number d2=0;
	Number temp;

	for(i=0;i<size;i++)
	{
		temp=v2[i]-v1[i];
		d2+=temp*temp;
	}

	return d2;
}

template<class Number,class TVector> inline void matVpow(TVector &v, unsigned int size, const Number &e)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=pow(v[i],e);
}

template<class TVector> inline void matVsqrt(TVector &v, unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=sqrt(v[i]);
}

template<class TVector> inline void matVsin(TVector &v, const unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=sin(v[i]);
}

template<class TVector> inline void matVcos(TVector &v, const unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=cos(v[i]);
}

template<class TVector> inline void matVtan(TVector &v, const unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=tan(v[i]);
}

template<class TVector> inline void matVasin(TVector &v, const unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=asin(v[i]);
}

template<class TVector> inline void matVacos(TVector &v, const unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=acos(v[i]);
}

template<class TVector> inline void matVatan(TVector &v, const unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=atan(v[i]);
}

template<class TVector> inline void matVlog(TVector &v, const unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=log(v[i]);
}

template<class TVector> inline void matVexp(TVector &v, const unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=exp(v[i]);
}

template<class TVector> inline void matVabs(TVector &v, const unsigned int size)
{
	register unsigned int i;
	for(i=0;i<size;i++)
        {
            (v[i]<0 ? v[i]*=(-1):v[i]);
            
        }
}


template<class Number,class TVector> inline Number matVPolival(const TVector &v, unsigned int size, const Number &x)
{
	register unsigned int i;
	Number poli=v[0];
	Number xx=x;

	for(i=1;i<size;i++)
	{
		poli+=v[i]*xx;
		xx*=x;
	}

	return poli;
}

template<class TVector> inline void matVMult(TVector &v2, const TVector &v1, unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v2[i]=v2[i]*v1[i];
}

template<class Number,class TVector> inline void matVMult(TVector &v, unsigned int size, const Number &k)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=v[i]*k;
}

template<class TVector> inline void matVDiv(TVector &v2, const TVector &v1, unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v2[i]=v2[i]/v1[i];
}

template<class Number,class TVector> inline void matVDiv(TVector &v, unsigned int size, const Number &k)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=v[i]/k;
}

template<class TVector> inline void matVInv(TVector &v, unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=1/v[i];
}

template<class TVector, class Number> inline void matVAdd(TVector &v, unsigned int size, const Number &k)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=v[i]+k;
}

template<class TVector> inline void matVAdd(TVector &v2, const TVector &v1, unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v2[i]=v2[i]+v1[i];
}


template<class TVector> inline void matVSub(TVector &v2,const TVector &v1, unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v2[i]=v2[i]-v1[i];
}


template<class TVector, class Number> inline void matVSub(TVector &v, unsigned int size, const Number &k)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=v[i]-k;
}


template<class TVector> inline void matVNeg(TVector &v, unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
		v[i]=-v[i];
}

template<class Number,class TVector> inline void matVNormalize(TVector &v, const unsigned int size)
{
	register unsigned int i;
	Number m=sqrt(matVDot<Number>(v,v,size));

	for(i=0;i<size;i++)
		v[i]=v[i]/m;
}


template<class Number,class TVector> inline void matVRescale01(TVector &v, const unsigned int size)
{
	Number vmin,vmax;

	matVMinMax(v,size,vmin,vmax);
	matVSub(v,size,vmin);
	matVDiv(v,size,vmax-vmin);
}

template<class Number,class TVector> inline void matVRescale(const Number &minIn,const Number &maxIn,TVector &v,const unsigned int size)
{
	register unsigned int i;
	Number vmin;
	Number vmax;

	matMinMax(v,size,vmin,vmax);
	Number temp=(maxIn-minIn)/(vmax-vmin);

	for(i=0;i<size;i++)
	{
		v[i]=((v[i]-vmin)*temp)+minIn;
	}
}

template<class Number, class TVector> inline void matVRect2Polar(TVector &v2,const TVector &v1)
{
	// (m�dulo,angulo)
    v2[0]=sqrt(matVDot<Number>(v1,v1,2));
    v2[1]=atan(v1[1]/v1[0]);
}

template<class Number, class TVector> inline void matVRect2Polar(TVector &v0)
{
	// (m�dulo,angulo)
	Number temp0=sqrt(matVDot<Number>(v0,v0,2));

	v0[1]=atan(v0[1]/v0[0]);
	v0[0]=temp0;
}


template<class Number, class TVector> inline void matVPolar2Rect(TVector &v2, const TVector &v1)
{
	// (x,y)
	v2[0]=v1[0]*cos(v1[1]);
	v2[1]=v1[0]*sin(v1[1]);
}

template<class Number, class TVector> inline void matVPolar2Rect(TVector &v0)
{
	// (x,y)
	Number temp0=v0[0]*cos(v0[1]);
	v0[1]=v0[0]*sin(v0[1]);
	v0[0]=temp0;
}


template<class Number, class TVector> inline void matVRect2Cyli(TVector &v2, const TVector &v1)
{
	//(r,THETA,z)
	matVRect2Polar<Number>(v2,v1);
	v2[2]=v1[2];
}


template<class Number, class TVector> inline void matVRect2Cyli(TVector &v0)
{
	//(r,THETA,z)
	matVRect2Polar<Number>(v0);
}



template<class Number, class TVector> inline void matVCyli2Rect(TVector &v2,const TVector &v1)
{
	//(x,y,z)
    matVPolar2Rect<Number>(v2,v1);
    v2[2]=v1[2];
}

template<class Number, class TVector> inline void matVCyli2Rect(TVector &v0)
{
	//(x,y,z)
    matVPolar2Rect<Number>(v0);
}


template<class Number, class TVector> inline void matVRect2Spher(TVector &v2,const TVector &v1)
{
	//(R,THETA,PHI)
    v2[0]=sqrt(matVDot(v1,v1,3));
    v2[1]=acos(v1[2]/v2[0]);
    v2[2]=atan(v1[1]/v1[0]);
}

template<class Number, class TVector> inline void matVRect2Spher(TVector &v0)
{
	//(R,THETA,PHI)
    Number tempR=sqrt(matVDot(v0,v0,3));
    Number tempPHI=atan(v0[1]/v0[0]);

    v0[1]=acos(v0[2]/tempR);
    v0[0]=tempR;
    v0[2]=tempPHI;
}


template<class Number, class TVector> inline void matVSpher2Rect(TVector &v2, const TVector &v1)
{
	//(x,y,z)
   v2[0]=v1[0]*cos(v1[1])*sin(v1[2]);
   v2[1]=v1[0]*sin(v1[1])*sin(v1[2]);
   v2[2]=v1[0]*cos(v1[1]);
}


template<class Number, class TVector> inline void matVSpher2Rect(TVector &v0)
{
	//(x,y,z)
   Number temp0=v0[0]*cos(v0[1])*sin(v0[2]);
   Number temp1=v0[0]*sin(v0[1])*sin(v0[2]);

   v0[2]=v0[0]*cos(v0[1]);
   v0[1]=temp1;
   v0[0]=temp0;
}

template<class Number,class TVector> inline void matVApply(TVector &v,const unsigned int size,Number f(Number))
{
	register unsigned int i;
	for(i=0;i<size;i++)
		v[i]=f(v[i]);
}

template<class Number,class TVector> inline void matVDiff(TVector &v,const Number &P0, const unsigned int size)
{
	register int i;
	unsigned int n=size-1;

	for(i=n;i>=1;i--)
	{
		v[i]=v[i]-v[i-1];
	}
	v[0]=v[0]-P0;
}

template<class Number,class TVector> inline void matVFlip(TVector &v,const unsigned int size)
{
	int leftIndex=0;
	int rightIndex=size-1;
	Number temp;

	while((rightIndex-leftIndex)>=1)
	{
		temp=v[leftIndex];
		v[leftIndex]=v[rightIndex];
		v[rightIndex]=temp;
		leftIndex++;
		rightIndex--;
	}
}

template<class TVector> inline void matVShuffler(TVector &v, const unsigned int size)
{
	using namespace MatUtils;
	register unsigned int i;
	for(i=0;i<size;i++)
	{
		swap(v[i],v[random(size)]);
	}
}

template<class Number, class TVector, class TIntVector> inline void matVReOrder(TVector &v,TIntVector &order, const unsigned int size)
{
	register unsigned int i;
	for(i=0;i<size;i++)
	{
		MatUtils::swap(v[i],v[order[i]]);
	}
}

template<class Number,class TVector> inline void matVCrossOver(TVector &v1,TVector &v2,const unsigned int crossPoint,const unsigned int size)
{
	register unsigned int i;
	for(i=0;i<crossPoint;i++)
		MatUtils::swap(v1[i],v2[i]);

	for(i=crossPoint;i<size;i++)
		MatUtils::swap(v1[i],v2[i]);

}

template<class Number,class TMatrix> inline void matMFlipRows(TMatrix &m,const unsigned int nrows,const unsigned int ncols)
{
	register unsigned int j;
	unsigned int leftIndex;
	unsigned int rightIndex;
	Number temp;

	for(j=0;j<ncols;j++)
	{
		leftIndex=0;
		rightIndex=nrows-1;

		while((rightIndex-leftIndex)>=1)
		{
			temp=m[leftIndex][j];
			m[leftIndex][j]=m[rightIndex][j];
			m[rightIndex][j]=temp;
			leftIndex++;
			rightIndex--;
		}

	}
}

template<class Number,class TMatrix> inline void matMFlipCols(TMatrix &m,const unsigned int nrows,const unsigned int ncols)
{
	register unsigned int i;
	unsigned int leftIndex;
	unsigned int rightIndex;
	Number temp;

	for(i=0;i<ncols;i++)
	{
		leftIndex=0;
		rightIndex=ncols-1;

		while((rightIndex-leftIndex)>=1)
		{
			temp=m[i][leftIndex];
			m[i][leftIndex]=m[i][rightIndex];
			m[i][rightIndex]=temp;
			leftIndex++;
			rightIndex--;
		}

	}
}

template<class Number,class TMatrix> inline void matMSwapRows(TMatrix &m,const unsigned int nrows,const unsigned int ncols,
		                                                      const unsigned int r1, const unsigned int r0)
{
	register unsigned int j;
	Number temp;

	for(j=0;j<ncols;j++)
	{
		temp=m[r1][j];
		m[r1][j]=m[r0][j];
		m[r0][j]=temp;
	}
}

template<class Number,class TMatrix> inline void matMSwapCols(TMatrix &m,const unsigned int nrows,const unsigned int ncols,
		                                                       const unsigned int c1, const unsigned int c0)
{
	register unsigned int i;
	Number temp;

	for(i=0;i<nrows;i++)
	{
		temp=m[i][c1];
		m[i][c1]=m[i][c0];
		m[i][c0]=temp;
	}
}

template<class Number,class TMatrix>
inline void matMAddRows(TMatrix &m0, cuint32 mm,cuint32 nn, cuint32 r0, cuint32 r1, Number &k=1)
{
	//Faz m0[r0][j]+=k*m0[r1][j];
	register uint32 j;

	for(j=0;j<nn;j++)
		m0[r0][j]+=k*m0[r1][j];
}

template<class Number,class TMatrix>
inline void matMAddCols(TMatrix &m0, cuint32 mm,cuint32 nn, cuint32 c0, cuint32 c1, Number &k=1)
{
	//Faz m0[i][c0]+=k*m0[i][c1];
	register uint32 i;

	for(i=0;i<mm;i++)
		m0[i][c0]+=k*m0[i][c1];
}

template<class Number,class TMatrix>
inline void matMMultRow(TMatrix &m0, cuint32 nn, cuint32 r0, Number &k)
{
	//Faz m0[r0][j]=k*m0[r0][j];
	register uint32 j;

	for(j=0;j<nn;j++)
		m0[r0][j]=k*m0[r0][j];
}

template<class Number,class TMatrix>
inline void matMMultCol(TMatrix &m0, cuint32 mm, cuint32 c0, Number &k)
{
	//Faz m0[i][c0]=k*m0[i][c0];
	register uint32 i;

	for(i=0;i<mm;i++)
		m0[i][c0]=k*m0[i][c0];
}

template<class Number, class TVector> inline Number matVInte(const TVector &v, const unsigned int size, int k0, int k1)
{
	register unsigned int i;

	Number summation=0;
	Number temp=1;

	if(k1<k0)
	{
		k1=temp;
		k1=k0;
		k0=temp;
		temp=-1;
	}

	if(k1>size)
		k1=size;

	if(k0<0)
		k0=0;

	for(i=k0;i<k1;i++)
	{
		summation+=v[i];
	}

	return summation*temp;
}

template<class Number, class TVector> inline void matVPdf(TVector &v, const TVector &vIn,const unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
	{
		v[i]=matInte(vIn,size,0,i);
	}
}

template<class TVector> inline unsigned int matVZeroCrossPoint(const unsigned int startPoint,const TVector &v,const unsigned int size)
{
	register unsigned int i;
	unsigned int n=size-1;

	for(i=startPoint;i<n;i++)
		if((v[i]<=0)&&(v[i+1]>0))
			return i;
}

template<class Number,class TVector> inline void matVShift(TVector &v,const unsigned int size, int d,const Number &vfill)
{
// v - Vetor a ser deslocado
// size - Tamanho do vetor
// d - Deslocamento. Pode ser positivo(esquerda) ou negativo(direita)
// vfill - Valor para preenchimento dos espaçõs gerados pelo deslocamento

	register int i;
	unsigned int n;

	if(d>0)
	{
		//d%size - garante que d será sempre menor que size
		d=d%size;
		n=size-d;

		for(i=0;i<(int)n;i++)
			v[i]=v[i+d];

		for(i=n;i<(int)size;i++)
			v[i]=vfill;

	}
	else
	if(d<0)
	{
		unsigned int s=size-1;
		d=-d;
		d=d%size;

		for(i=s;i>=d;i--)
			v[i]=v[i-d];
		d--;
		for(i=d;i>=0;i--) //diferentemento do loop incremental, aqui i vai até zero apesar do sinal >
			v[i]=vfill;
	}

}

template<class Number, class TVector> inline void matVUpperBoundViolations(unsigned int violationsIndex[],unsigned int &countViolations,const TVector &v, const unsigned int size,const Number &upperBound)
{
	register unsigned int i;
	countViolations=0;
	for(i=0;i<size;i++)
	{
		if(v[i]>upperBound)
		{
			violationsIndex[countViolations++]=i;
		}
	}
}

template<class Number, class TVector> inline void matVLowerBoundViolations(unsigned int violationsIndex[],unsigned int &countViolations,const TVector &v, const unsigned int size,const Number &lowerBound)
{
	//violationsIndex[] precisa ter pelomenos o tamanho de v[] para garantir que n�o haver� estouro de mem�ria
	register unsigned int i;
	countViolations=0;
	for(i=0;i<size;i++)
	{
		if(v[i]>lowerBound)
		{
			violationsIndex[countViolations++]=i;
		}
	}
}

template<class TVector> inline void matVReIndexVector(TVector &v2,const TVector &v1,const unsigned int index[],const unsigned int indexSize)
{
	//V2,v1 e index precis�o ser todos do mesmo tamanho
	register unsigned int i;

	for(i=0;i<indexSize;i++)
	{
		v2[i]=v1[index[i]];
	}
}

template<class TVector> inline bool matVExistNan(const TVector &v, const unsigned int size)
{
	register unsigned int i;
	bool isNanFlag=false;

	for(i=0;i<size;i++)
	{
		if(isnan(v[i]))
		{
			isNanFlag=true;
			break;
		}
	}

	return isNanFlag;
}

template<class TVector> inline bool matVExistInf(const TVector &v, const unsigned int size)
{
	register unsigned int i;
	bool isInfFlag=false;

	for(i=0;i<size;i++)
	{
		if(isinf(v[i]))
		{
			isInfFlag=true;
			break;
		}
	}

	return isInfFlag;
}

template<class TVector> inline bool matVReplaceInf(const TVector &v, const unsigned int size, double x)
{
	register unsigned int i;
	bool isInfFlag=false;

	for(i=0;i<size;i++)
	{
		if(isinf(v[i]))
		{
			isInfFlag=true;
                        v[i]=(v[i]<0 ? -x:x);
		}
	}

	return isInfFlag;
}

//template<class TVector> inline bool matVReplaceNan(const TVector &v, const unsigned int size, double x)
//{
//	register unsigned int i;
//	bool isInfFlag=false;
//
//	for(i=0;i<size;i++)
//	{
//		if(isinf(v[i]))
//		{
//			isInfFlag=true;
//                        v[i]=(v[i]<0 ? -x:x);
//		}
//	}
//
//	return isInfFlag;
//}

template<class Number, class TVector> inline void matVVDotKAdd(const TVector &v2,const TVector &v1,const Number &k0,Number &kOut,unsigned int size)
{
	register unsigned int i;

	kOut=0;
	for(i=0;i<size;i++)
	{
		kOut+=v2[i]*v1[i];
	}
	kOut+=k0;
}

template<class Number, class TVector> inline void matVKMultKAdd(const TVector &v,const Number &k,const Number &k0,TVector vOut,unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
	{
		vOut[i]=v[i]*k+k0;
	}

}

template<class Number, class TVector> inline void matVKMultVAdd(const TVector &v,const Number &k,const TVector &v0,TVector &vOut,unsigned int size)
{
	register unsigned int i;

	for(i=0;i<size;i++)
	{
		vOut[i]=v[i]*k+v0[i];
	}

}

template<class TVector, class TMatrix> inline void matMVMult(const TMatrix &m,const TVector &v,TVector &vOut,unsigned int mm, unsigned int nn)
{
	// mm - Número de linhas de m e vOut
	// nn - Número de colunas de m e linhas de v

	register unsigned int i,k;

	for(i=0;i<mm;i++)
	{
		vOut[i]=0;
		for(k=0;k<nn;k++)
		{
			vOut[i]+=m[i][k]*v[k];
		}
	}

}

template<class Number, class TVector, class TMatrix> inline void matMVMultKAdd(const TMatrix &m,const TVector &v1,const Number &kk,TVector &vOut,unsigned int mm, unsigned int nn)
{
	// mm - Número de linhas de m e vOut
	// nn - Número de colunas de m e linhas de v1

	register int i,k;

	for(i=0;i<mm;i++)
	{
		vOut[i]=0;
		for(k=0;k<nn;k++)
		{
			vOut[i]+=m[i][k]*v1[k];
		}
		vOut[i]+=kk;
	}

}

template<class TVector, class TMatrix> inline void matMVMultVAdd(const TMatrix &m,const TVector &v1,const TVector &v0,TVector &vOut,unsigned int mm, unsigned int nn)
{
	// mm - Número de linhas de m e vOut
	// nn - Número de colunas de m e linhas de v1 e v0

	register unsigned int i,k;

	for(i=0;i<mm;i++)
	{
		vOut[i]=0;
		for(k=0;k<nn;k++)
		{
			vOut[i]+=m[i][k]*v1[k];
		}
		vOut[i]+=v0[k];
	}

}

template<class TMatrix> inline void matMMMult(const TMatrix &m2, const TMatrix &m1, TMatrix &mOut,unsigned int mm2,unsigned int nnk,unsigned int nn1)
{
	// mm2 - Número de linhas de m2
	// nnk - Número de colunas de m2 e de linhas de m1
	// nn1 - Número de colunas de m1
	register unsigned int i,j,k;

	for(i=0;i<mm2;i++)
		for(j=0;j<nn1;j++)
		{
			mOut[i][j]=0;
			for(k=0;k<nnk;k++)
			{
				mOut[i][j]+=m2[i][k]*m1[k][j];
			}
		}
}


template<class Number, class TMatrix> inline void matMMMultKAdd(const TMatrix &m2,const TMatrix &m1,const Number &kk,TMatrix &mOut,unsigned int mm2,unsigned int nnk,unsigned int nn1)
{
	// mm2 - Número de linhas de m2
	// nnk - Número de colunas de m2 e de linhas de m1
	// nn1 - Número de colunas de m1
	register unsigned int i,j,k;

	for(i=0;i<mm2;i++)
		for(j=0;j<nn1;j++)
		{
			mOut[i][j]=0;
			for(k=0;k<nnk;k++)
			{
				mOut[i][j]+=m2[i][k]*m1[k][j];
			}
			mOut[i][j]+=kk;
		}
}

template<class TMatrix> inline void matMMMultMAdd(const TMatrix &m2,const TMatrix &m1,const TMatrix &m0, TMatrix &mOut,unsigned int mm2,unsigned int nnk,unsigned int nn1)
{
	// mm2 - Número de linhas de m2
	// nnk - Número de colunas de m2 e de linhas de m1
	// nn1 - Número de colunas de m1
	register unsigned int i,j,k;

	for(i=0;i<mm2;i++)
		for(j=0;j<nn1;j++)
		{
			mOut[i][j]=0;
			for(k=0;k<nnk;k++)
			{
				mOut[i][j]+=m2[i][k]*m1[k][j];
			}
			mOut[i][j]+=m0[i][j];
		}
}


template<class Number, class TVector, class TMatrix>
bool matMVGEM(TMatrix &m0 ,TVector &b, uint32 mrow[], cuint32 nn)
 {

	//M�todo de elimina��o de Gauss com pivo parcial.
	// A matriz m0 precisa ser quadrada.

	int32 k;    //Loops mais externos
	register uint32 i,j; //Loops mais internos

	//Inicializa��o do mapeamento de linhas
	for(i=0;i<nn;i++)
		mrow[i]=i;

	uint32 n=nn-1;

	//Inicio da triangulariza��o
	for(k=0;k<(int)nn;k++)
	{
		//Seleciona p, talque m0[mrow[p]][k] seja m�ximo em m�dulo
		uint32 p=k;
		for(i=(k+1);i<nn;i++)
		{
			if(fabs(m0[mrow[p]][k])<fabs(m0[mrow[i]][k]))
				p=i;
		}

		//Existe uma coluna nula, retorna a opera��o como nula
		if(m0[mrow[p]][k]==0)
			return false;

		//Se o valor m�ximo da coluna k n�o for o da diagonal troca-se as linhas
		if((int)p!=k)
		 	MatUtils::swap(mrow[p],mrow[k]);

		Number mp=0; //Pivo
		for(i=(k+1);i<nn;i++)
		{
			mp=m0[mrow[i]][k]/m0[mrow[k]][k];
			m0[mrow[i]][k]=0; // Zera o elemento abaixo da diagonal

			for(j=k+1;j<nn;j++)
				m0[mrow[i]][j]-=mp*m0[mrow[k]][j];

			b[mrow[i]]-=mp*b[mrow[k]];
		}
	}
	// Fim da triangulariza��o

	//n=nn-1
	//cout << "n=" << n << endl;
	if(m0[mrow[n]][n]==0)
		return false; //A matriz tem uma deficiencia de rank.

	return true;

 }

template<class Number, class TMatrix>
bool matMVGEM(TMatrix &m0 , uint32 mrow[], cuint32 nn)
 {

	//M�todo de elimina��o de Gauss com pivo parcial sem vetor b.
	// A matriz m0 precisa ser quadrada.

	int32 k;    //Loops mais externos
	register uint32 i,j; //Loops mais internos

	//Inicializa��o do mapeamento de linhas
	for(i=0;i<nn;i++)
		mrow[i]=i;

	uint32 n=nn-1;

	//Inicio da triangulariza��o
	for(k=0;k<(int)nn;k++)
	{
		//Seleciona p, talque m0[mrow[p]][k] seja m�ximo em m�dulo
		uint32 p=k;
		for(i=(k+1);i<nn;i++)
		{
			if(fabs(m0[mrow[p]][k])<fabs(m0[mrow[i]][k]))
				p=i;
		}

		//Existe uma coluna nula, retorna a opera��o como nula
		if(m0[mrow[p]][k]==0)
			return false;

		//Se o valor m�ximo da coluna k n�o for o da diagonal troca-se as linhas
		if((int)p!=k)
		 	MatUtils::swap(mrow[p],mrow[k]);

		Number mp=0; //Pivo
		for(i=(k+1);i<nn;i++)
		{
			mp=m0[mrow[i]][k]/m0[mrow[k]][k];
			m0[mrow[i]][k]=0; // Zera o elemento abaixo da diagonal

			for(j=k+1;j<nn;j++)
				m0[mrow[i]][j]-=mp*m0[mrow[k]][j];
		}
	}
	// Fim da triangulariza��o

	//n=nn-1
	//cout << "n=" << n << endl;
	if(m0[mrow[n]][n]==0)
		return false; //A matriz tem uma deficiencia de rank.

	return true;

 }



template<class Number, class TMatrix>
Number matMDet(const TMatrix &m0, cuint32 mrow[], cuint32 nn)
{
	//A matriz m0 precisa ser quadrada e triangular superior
	register uint32 k;
	Number temp=1;
	for(k=0;k<nn;k++)
		temp*=m0[mrow[k]][k];

	return temp;
}


template<class Number, class TVector, class TMatrix>
void matMVBackSub(const TMatrix &m0 ,TVector &x,const TVector &b, cuint32 mrow[], cuint32 nn)
{
	//A matriz m0 precisa ser uma matriz triangular superior sem deficiencia de rank
	cuint32 n=nn-1;
	//Inicio da substitui��o retroativa
	x[n]=b[mrow[n]]/m0[mrow[n]][n];

	register int32 k,j;
	Number temp;
    for(k=(n-1);k>=0;k--)
    {
       temp=0;

       for(j=k+1;j<=(int)n;j++)
          temp+=m0[mrow[k]][j]*x[j];

       x[k]=(b[mrow[k]]-temp)/m0[mrow[k]][k];
    }
}

template<class Number, class TVector, class TMatrix>
void matLinSolve(TMatrix &m0 ,TVector &x, TVector &b, uint32 mrow[], cuint32 nn)
{
	//Metodo implace de resolu��o de um sistema linear;
	matMVGEM(m0,b,mrow,nn);
	matMVBackSub(m0,x,b,mrow,nn);
}

template<class Number,class TVector>
inline void matVLinInterCoe(TVector &y0,TVector &y1,const Number &x0, const Number &x1, const uint32 size)
{
	/*
	 * Retorna o valor dos coeficientes lineare em y1 e os coeficientes angulares em y0
	 */
	register uint32 i;

	for(i=0;i<size;i++)
	{
		y0[i]=(y1[i]-y0[i])/(x1-x0);
		y1[i]=y1[i]-y0[i]*x1;
	}
}

template<class Number,class TVector>
inline void matVLinInterEval(TVector &y,const Number &x,const TVector &a,const TVector &b,const uint32 size)
{
	register uint32 i;

	for(i=0;i<size;i++)
	{
		y[i]=a[i]*x+b[i];
	}
}

template<class Number,class TVector>inline void matFFT(TVector &vr,TVector &vi,int32 rindex[],int32 pow2)
{
	int32 nsize=1<<pow2;
	int32 j;
	int32 k=nsize/2;
	int32 count=0;


	//Algoritmo de reordenacao
	for(j=1;j<=nsize;j+=k)
	{
		k=nsize/2;
		rindex[count++]=j-1;

		if(j==nsize)
			break;

//		for(j;j>k;j-=k,	k=k/2);
	}
	printVector(rindex,nsize,"reindex=");
	int32 m,n,l;
	Number Ur,Ui,Wr,Wi,tr,ti;

	for(m=1;m<=pow2;m++)
	{
		Ur=1;
		Ui=0;

		k=(1<<(m-1));
		Wr=cos(MatUtils::PI/k);
		Wi=-sin(MatUtils::PI/k);

		for(j=1;j<=k;j++)
		{
			for(l=j;l<=nsize;l+=(1<<m))
			{
				tr=vr[rindex[l+k-1]]*Ur-vi[rindex[l+k-1]]*Ui;
				ti=vr[rindex[l+k-1]]*Ui+vi[rindex[l+k-1]]*Ur;

				vr[rindex[l+k-1]]=vr[rindex[l-1]]-tr;
				vi[rindex[l+k-1]]=vi[rindex[l-1]]-ti;

				vr[rindex[l-1]]=vr[rindex[l-1]]+tr;
				vi[rindex[l-1]]=vi[rindex[l-1]]+ti;
			}

			Ur=Ur*Wr-Ui*Wi;
			Ui=Ui*Wi+Ur*Wr;
		}
	}


}


#endif /*MATALGORITHMS_H_*/
