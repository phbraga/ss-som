#ifndef MATFUNCTIONS_H_
#define MATFUNCTIONS_H_

#include <MatVector.h>

namespace MatSignals {

/**
 * Retorna um vetor de covariancia do vetor com v
 * @param *v vetor de comparação
 * return vetor de covariancia
 * @author Daniel F.Gomes
 * @since  22-02-2008
 */
template<class Number> void shortCorrel(const MatVector<Number> &v1, const MatVector<Number> &v2, MatVector<Number> &vCorr, Number s1=0,Number s2=0)
{
	std::cerr << "Esta rotina é apenas uma implementação inicial(lenta), não usa FFT, e só é válida para vetores Reais" << std::endl;

	if(s1==0)
		s1=v1.size();

	if(s2==0)
		s2=v2.size();

	if(v1.size()==0)
		vCorr=v2;
	else
	if(v2.size()==0)
		vCorr=v1;
	else
	{

		Number v1m,v2m,ac;
		int M;
		int N;
		int vMax;

		if(v1.size()>v2.size())
		{
			M=v1.size();
			N=v2.size();
			vMax=1;
		}
		else
		{
			M=v2.size();
			N=v1.size();
			vMax=2;
		}

		unsigned int iStart;
		unsigned int iStop;
		register int x;
		register unsigned int i;

		v1m=0;
		for(i=0;i<v1.size();i++)
			v1m+=v1[i];

		v2m=0;
		for(i=0;i<v2.size();i++)
			v2m+=v2[i];

		v1m=v1m/s1;
		v2m=v2m/s2;

		vCorr.resize(M+N-1);
		for(x=0;x<(M+N-1);x++)
		{

			if(x<(N-1))
			{
				iStart=0;
				iStop=x;
				ac=0;

				if(vMax==1)
				{
					for(i=iStart;i<=iStop;i++)
						ac+=(v1[i]-v1m)*(v2[i+(N-1-x)]-v2m);
				}
				else
				{
					for(i=iStart;i<=iStop;i++)
						ac+=(v2[i]-v2m)*(v1[i+(N-1-x)]-v1m);
				}


				vCorr[x]=ac;

			}
			else
			if(x<(M-1))
			{
				iStart=x-(N-1);
				iStop=x;
				ac=0;

				if(vMax==1)
				{
					for(i=iStart;i<=iStop;i++)
						ac+=(v1[i]-v1m)*(v2[i-(x-N+1)]-v2m);
				}
				else
				{
					for(i=iStart;i<=iStop;i++)
						ac+=(v2[i]-v2m)*(v1[i-(x-N+1)]-v1m);
				}


				vCorr[x]=ac;

			}
			else
			{
				iStart=x-(N-1);
				iStop=M-1;
				ac=0;
				if(vMax==1)
				{
					for(i=iStart;i<=iStop;i++)
						ac+=(v1[i]-v1m)*(v2[i-(x-N+1)]-v2m);
				}
				else
				{
					for(i=iStart;i<=iStop;i++)
						ac+=(v2[i]-v2m)*(v1[i-(x-N+1)]-v1m);
				}


				vCorr[x]=ac;

			}

		}


	}
}

}

/**
 * Rotina que calcula os coeficientes de interpolação spline cubica retornados em y2
 */
template<class Number> void spline(const MatVector<Number> &x, const MatVector<Number> &y,MatVector<Number> &y2,const Number yp1,const Number ypn)
{
	int i,k;

	Number p,qn,sig,un;

	int n=y2.size();

	MatVector<Number> u(n-1);

	if(yp1>0.99e30)
		y2[0]=u[0]=0.0;
	else
	{
		y2[0]=-0.5;
		u[0] = (3.0/(x[1]-x[0]))*((y[1]-y[0])/(x[1]-x[0])-yp1);
	}

	for(i=1;i<n-1;i++)
	{
		sig=(x[i]-x[i-1])/(x[i+1]-x[i-1]);
		p=sig*y2[i-1]+2.0;
		y2[i]=(sig-1.0)/p;
		u[i]=(y[i+1]-y[i])/(x[i+1]-x[i])-(y[i]-y[i-1])/(x[i]-x[i-1]);
		u[i]=(6.0*u[i]/(x[i+1]-x[i-1])-sig*u[i-1])/p;
	}

	if(ypn > 0.99e30)
		qn=un=0.0;
	else
	{
		qn=0.5;
		un=(3.0/(x[n-1]-x[n-2]))*(ypn-(y[n-1]-y[n-2])/(x[n-1]-x[n-2]));
	}
	y2[n-1]=(un-qn*u[n-2])/(qn*y2[n-2]+1.0);

	for(k=n-2;k>=0;k--)
		y2[k]=y2[k]*y2[k+1]+u[k];

}

/**
 * Rotina que realiza interpolação spline cubica.
 * @param xa - Vetor x não interpolado
 * @param ya - Vetor y não interpolado
 * @param y2a - Vetor de coeficientes da interpolação spline
 * @param x - Valor não tabelado de x
 * @return y - Valor interpolado de y
 */
template<class Number> void splint(const MatVector<Number> &xa,const MatVector<Number> &ya,const MatVector<Number> &y2a,const Number x,Number &y)
{
	int k;
	Number h,b,a;

	int n = xa.size();
	int klo=0;
	int khi=n-1;

	while(khi-klo>1)
	{
		k=(khi+klo)>>1;

		if(xa[k]>x)
			khi=k;
		else
			klo=k;
	}

	h=xa[khi]-xa[klo];

	if(h==0.0)
		std::cerr << "Bad xa input to routine splint" << std::endl << std::flush;

	a=(xa[khi]-x)/h;
	b=(x-xa[klo])/h;

	y=a*ya[klo]+b*ya[khi]+((a*a*a-a)*y2a[klo]+(b*b*b-b)*y2a[khi])*(h*h)/6.0;
}

/**
 * Rotina de fit linear
 * @param x - Vetor com as coordenadas do eixo x dos pontos
 * @param y - Vetor com as coordenadas do eixo y dos pontos
 * @return a - Coeficiente angular do fit
 * @return b - Coeficiente linear do fit
 *
 *@author Daniel de F.Gomes
 */
template<class Number> void linearFit(const MatVector<Number> &x,const MatVector<Number> &y,Number &a, Number &b)
{

	unsigned int n=x.size();
	Number xya,xa,x2a,ya,xx,yy;

	register unsigned int i;

	xya=0;
	xa=0;
	x2a=0;
	ya=0;

	for(i=0;i<n;i++)
	{
		xa+=x[i];
		ya+=y[i];
	}
	xa=xa/n;
	ya=ya/n;

	for(i=0;i<n;i++)
	{
		xx=(x[i]-xa);
		yy=(y[i]-ya);
		xya+=xx*yy;
		x2a+=xx*xx;
	}

	a=xya/x2a;
	b=(ya-a*xa);

}

/**
 * Rotina de fit linear
 * @param y - Vetor com as coordenadas do eixo y considerando o espacamento de x uniforme e igual a 1.
 * @return a - Coeficiente angular do fit
 * @return b - Coeficiente linear do fit
 *
 *@author Daniel de F.Gomes
 */
template<class Number> void linearFit(const MatVector<Number> &y,Number &a, Number &b)
{
	MatVector<> x(y.size());
	x.range(0,y.size()-1);
	linearFit<Number>(x,y,a,b);
}

/**
 * Rotação de um conjunto de pontos representados por um par de vetores entorno do eixo Z
 */
template<class Number> void rotZAxis(const double theta, MatVector<Number> &x, MatVector<Number> &y)
{
	register int i;
	int n=x.size();
	Number xx,yy;

	for(i=0;i<n;i++)
	{
		xx=x[i]*cos(theta)+y[i]*sin(theta);
		yy=-x[i]*sin(theta)+y[i]*cos(theta);
		x[i]=xx;
		y[i]=yy;
	}
}

/**
 * Rotação de um conjunto de pontos representados por um par de vetores entorno do eixo X
 */
template<class Number> void rotXAxis(const double alpha, MatVector<Number> &y, MatVector<Number> &z)
{
	register int i;
	int n=y.size();
	Number yy,zz;

	for(i=0;i<n;i++)
	{
		yy=y[i]*cos(alpha)+z[i]*sin(alpha);
		zz=-y[i]*sin(alpha)+z[i]*cos(alpha);
		y[i]=yy;
		z[i]=zz;
	}
}

/**
 * Rotação de um conjunto de pontos representados por um par de vetores entorno do eixo Y
 */
template<class Number> void rotYAxis(const double beta, MatVector<Number> &x, MatVector<Number> &z)
{
	register int i;
	int n=x.size();
	Number xx,zz;

	for(i=0;i<n;i++)
	{
		xx=x[i]*cos(beta)-z[i]*sin(beta);
		zz=x[i]*sin(beta)+z[i]*cos(beta);
		x[i]=xx;
		z[i]=zz;
	}
}


template<class Number> void perspectiveZ(const double lambda,const MatVector<Number> &z, MatVector<Number> &Pz)
{
	register int i;
	int n=z.size();

	for(i=0;i<n;i++)
	{
		Pz[i]=Pz[i]*(1-z[i]/lambda);
	}
}

template<class Number> void upperCover(const MatVector<Number> &x, const MatVector<Number> &y, const MatVector<Number> &xc, MatVector<Number> &yc,const Number delta)
{
	register int i;
	register int ii;
	int n=x.size();
	int nc=xc.size();

	for(i=0;i<nc;i++)
	{
		for(ii=0;ii<n;ii++)
		{
			if(abs(xc[i]-x[ii])<delta)
			{
				if(yc[i]<y[ii])
					yc[i]=y[ii];
			}
		}
	}
}

/**
 * Filtro de mediana
 * @param v - Vetor de dados
 * @param alfa - Taxa de tolerancia do filtro, quanto menor mais sensível é o filtro
 * @param windoLSize - É a metade do tamanho da janela do filtro menos 1. (windowLSize=(windowSize-1)/2)
 */
template<class Number> void medianFilter(MatVector<Number> &v,const float &alfa,const int windowLSize=1)
{
	using namespace MatUtils;
	register unsigned int i;
	unsigned int n=v.size();
	unsigned int lsize=windowLSize*2+1;
	int leftLimit;
	int rightLimit;
	Number median;
	MatVector<Number> vtemp(v);
	MatVector<Number> temp2(lsize);

	for(i=0;i<n;i++)
	{
		leftLimit=i-windowLSize;
		if(leftLimit<0)
			leftLimit=0;

		rightLimit=i+windowLSize;
		if(rightLimit>=n)
			rightLimit=n-1;

		temp2.copy(vtemp,leftLimit,rightLimit);
		median=temp2.sortAndCalcMedian();
		if(fabs(v[i]-median)>(median*alfa))
			v[i]=median;
	}
}

/**
 * Filtro de máximo
 * @param v - Vetor de dados
 * @param windoLSize - É a metade do tamanho da janela do filtro menos 1. (windowLSize=(windowSize-1)/2)
 */
template<class Number> void maxFilter(MatVector<Number> &v,const int windowLSize=1)
{
	register int i;
	int n=v.size()-windowLSize;
	int lsize=windowLSize*2+1;
	MatVector<Number> vtemp(v);

	for(i=windowLSize;i<n;i++)
	{
		MatVector<Number> temp(lsize);
		temp.copy(vtemp,i-windowLSize,i+windowLSize);
		v[i]=temp.max();
	}
}

/**
 * Filtro de mínimo
 * @param v - Vetor de dados
 * @param windoLSize - É a metade do tamanho da janela do filtro menos 1. (windowLSize=(windowSize-1)/2)
 */
template<class Number> void minFilter(MatVector<Number> &v,const int windowLSize=1)
{
	register int i;
	int n=v.size()-windowLSize;
	int lsize=windowLSize*2+1;
	MatVector<Number> vtemp(v);

	for(i=windowLSize;i<n;i++)
	{
		MatVector<Number> temp(lsize);
		temp.copy(vtemp,i-windowLSize,i+windowLSize);
		v[i]=temp.min();
	}
}

template<class Number> void lowerBoundFilter(MatVector<Number> &x,MatVector<Number> &y,const Number &lowerBound)
{
	using namespace std;

	if(x.size()==y.size())
	{
		unsigned int counter=0;
		unsigned int n=x.size();
		register unsigned int i;

		MatVector<Number> xTemp(n);
		MatVector<Number> yTemp(n);
		MatVector<Number> y2;

		for(i=0;i<n;i++)
		{
			if(y[i]>lowerBound)
			{
				xTemp[counter]=x[i];
				yTemp[counter++]=y[i];
			}
		}

		x.size(counter);
		y.size(counter);
		y2.size(counter);

		for(i=0;i<counter;i++)
		{
			x[i]=xTemp[i];
			y[i]=yTemp[i];
		}

		spline<Number>(x,y,y2,0,n-1);

		xTemp.size(n);
		xTemp.range(0,n-1);
		yTemp.size(n);

		for(i=0;i<n;i++)
		{
			splint<Number>(x,y,y2,xTemp[i],yTemp[i]);
		}

		x.swap(xTemp);
		y.swap(yTemp);

	}
	else
		cerr << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "::" << INVALID_DIMENSION_SIZE << endl << flush;
}


template<class Number> Number findRootsBiSec(Number f(Number),const Number tol, const Number a,const Number b,const uint maxInt)
{
	//Metodo da bisseção para encontrar os zeros de uma função
	register uint i;
	Number lLimit=a,rLimit=b;
	Number fLimit,fPoint,p;
	Number step;

	for(i=0;i<maxInt;i++)
	{

		fLimit=f(lLimit);
		step=(rLimit-lLimit)/2;
		p=lLimit+step;
		fPoint=f(p);

		if((fPoint<0)&&(step<tol))
			return p;

		if(((fLimit>0)&&(fPoint>0))||((fLimit<0)&&(fPoint<0)))
			lLimit=p;
		else
			rLimit=p;
	}

	return p;
}

template<class Number> Number findRootsNewton(Number f(Number),Number fL(Number),const Number tol, Number p0,const uint maxInt)
{
	//Metodo de Newton para encontrar os zeros de uma fun��o
	register uint i;
	Number fp;
	Number p;

	for(i=0;i<maxInt;i++)
	{
		fp=f(p0);
		p=p0-fp/fL(p0);
		if((fp<tol)&&(fabs(p-p0)<tol))
			return p;

		p0=p;
	}

	return p;
}

template<class Number> Number findRootsSec(Number f(Number),const Number tol, Number p0,Number p1,const uint maxInt)
{
	//Metodo da secante para encontrar os zeros de uma função
	register uint i;
	Number fp0;
	Number fp1;
	Number fp;
	Number p;

	for(i=0;i<maxInt;i++)
	{
		fp0=f(p0);
		fp1=f(p1);
		p=p1-fp1*(p1-p0)/(fp1-fp0);
		fp=f(p);

		if((fp<tol)&&(fabs(p-p0)<tol))
			return p;

		p0=p1;
		fp0=fp1;
		p1=p;
		fp1=fp;
	}

	return p;
}


/**
 * Rotina de conversão do sistema de coordenadas do sonar Tritech DFP para um sistema de coordenadas cartesiano.
 */
template<class Number> void convertSSoC(const MatVector<Number> &sonarData,const Number &leftScanAngle,const Number &rightScanAngle, \
		                                MatVector<Number> &x, MatVector<Number> &y)
{
	/** convertSSoC -> convert Sonar System of Coordinates
	 */

	int i;
	int n=sonarData.size();
	Number step=(rightScanAngle-leftScanAngle)/(n-1);
	Number angle=0;
	x.zeros(n);
	y.zeros(n);

	for(i=0;i<n;i++)
	{
		angle=(leftScanAngle+i*step)/180;
		x[i]=sonarData[i]*cos(angle*MatUtils::PI);
		y[i]=sonarData[i]*sin(angle*MatUtils::PI);
	}

}


template<class Number> bool linearInterpolation(MatVector<Number> &v,const Number &x, MatVector<Number> &v0, MatVector<Number> &v1,const Number &x0=0,const Number &x1=1)
{
	/*
	 * Os vetores v0 e v1 são sobrescritos
	 */

	uint32 n=v0.size();
	if(n==v1.size())
	{
		v.size(n);
		matVLinInterCoe(v0,v1,x0,x1,n);
		matVLinInterEval(v,x,v0,v1,n);

		return true;
	}

	return false;
}


#endif /*MATFUNCTIONS_H_*/
