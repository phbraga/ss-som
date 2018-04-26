#ifndef MATMATRIX_H_
#define MATMATRIX_H_

#include <iostream>
#include <string.h>
#include <MatVector.h>
//#define MatMatrixDebugOn
//using namespace std; // << sem isto não compila sem declarar MatMatrixDebugOn
#include <MatAlgorithms.h>
//#define MatMatrixDebugOn

const static std::string COMPATIBILITY_MSG = "Esta funcao e somente para compatibilizar com a versao antiga da MatMatrix";

template<class Number> class MetaMatMatrix;

template<class Number=DefaultNumberType> class MatMatrix
{
	private:
		unsigned int nrows;
		unsigned int ncols;
		mutable Number **vect;
		virtual inline Number** alloc(unsigned int rows,unsigned int cols);
		virtual inline void free(Number** vectTemp);
	public:
		friend class MetaMatMatrix<Number>;
		friend class MetaMatVector<Number>;
		friend class MatVector<Number>;

		MatMatrix& swap(MatMatrix<Number> &m);
		inline MatMatrix<Number>& swap(MetaMatMatrix<Number> &meta);
		inline MatMatrix& inputMeta(MetaMatMatrix<Number> &meta);
		inline MatMatrix& outputMeta(MetaMatMatrix<Number> &meta);
		inline Number* getVect();
		inline unsigned int rows() const;
		inline unsigned int cols() const;
		inline unsigned int size() const;
		inline MatMatrix& clear();
		inline MatMatrix& size(unsigned int r,unsigned int c);
		inline MatMatrix& size(const MatMatrix<Number> &m);
		MatMatrix& operator=(const MetaMatMatrix<Number> &meta);
		MatMatrix& operator=(const MatMatrix<Number> &m);
		MatMatrix& operator=(const MatVector<Number> &v);
		MatMatrix& operator=(const Number &k);
		inline Number* operator[](const unsigned int i);
		inline const Number* operator[](const unsigned int i) const;
		inline Number& operator()(const unsigned int i, const unsigned int j);
		inline const Number& operator()(const unsigned int i, const unsigned int j) const;
		MatMatrix(unsigned int r,unsigned int c);
		MatMatrix(Number *v,unsigned int r,unsigned int c,bool memcopy=true);
		MatMatrix(const MatMatrix<Number> &m);
		MatMatrix(const MatVector<Number> &v, operationType op=column);
		MatMatrix(MetaMatMatrix<Number> &meta);
		MatMatrix();
		virtual ~MatMatrix();
		MatMatrix& resize(unsigned int l,unsigned int r);
		MatMatrix& fill(Number x,unsigned int l,unsigned int r);
		MatMatrix& fill(Number x);
		MatMatrix& zeros(unsigned int r,unsigned int c);
		MatMatrix& zeros();
		MatMatrix& ones(unsigned int r,unsigned int c);
		MatMatrix& ones();
		MatMatrix& srandom(const long s);
		MatMatrix& random(const int s);
		Number mean();
		Number var();
		Number stdDev();
                Number sum();
                Number prod();

		MatMatrix& random(void);
		MatMatrix& range(const Number &start,const Number &stop);
		MatMatrix& rangeRows(const Number &start,const Number &stop,const Number &step);
		MatMatrix& rangeCols(const Number &start,const Number &stop,const Number &step);
		MatMatrix& concatRows(const MatMatrix<Number> &m);
		MatMatrix& concatRows(const MatVector<Number> &v);
		MatMatrix& concatCols(const MatMatrix<Number> &m);
		MatMatrix& concatCols(const MatVector<Number> &v);
		MatMatrix* clone(void);
		MatMatrix& getRow(int row,MatMatrix<Number> &m);
		MatMatrix& getCol(int col,MatMatrix<Number> &m);
		MatMatrix& getRow(int row,MatVector<Number> &v);
		MatMatrix& getCol(int col,MatVector<Number> &v);
		inline MatMatrix& set(const Number &x,unsigned int row, unsigned int col);
                MatMatrix& setRow(unsigned int r, MatVector<Number> &row);
		inline const Number& get(unsigned int row, unsigned int col);
		MatMatrix& eye(unsigned int n);
		MatMatrix& eye();
		MatMatrix& diag(const MatVector<Number> &v);
		MatMatrix& diag(const MatMatrix<Number> &m);
		MatMatrix& diag();
		MatMatrix& getDiag(MatVector<Number> &v);
		MatMatrix& rotMatrix4X(Number theta);
		MatMatrix& rotMatrix4Y(Number theta);
		MatMatrix& rotMatrix4Z(Number theta);
		MatMatrix& rotMatrix4XYZ(Number thetaX,Number thetaY,Number thetaZ);
		MatMatrix& transMatrix4(const MatMatrix<Number> &m);
		MatMatrix& transMatrix4(const MatVector<Number> &v);
		MatMatrix& transMatrix4(Number X0, Number Y0, Number Z0);
		MatMatrix& transMatrix4(Number v[3]);
		MatMatrix& scaleMatrix4(Number scaleX,Number scaleY,Number scaleZ);
		MatMatrix& scaleMatrix4(Number scale);
		MatMatrix& perspectiveMatrix4(Number gama);
		MatMatrix& trn(void);
		Number det();
		Number tr(void);
		MatMatrix& add(Number k);
		MatMatrix& add(const MatVector<Number> &v,operationType op=column );
		MatMatrix& add(const MatMatrix<Number> &m);
		MatMatrix& mult(Number k);
		MatMatrix& mult(const MatMatrix<Number> &m);
		MatMatrix& preMult(const MatMatrix<Number> &m);
		MatMatrix& mult(const MatVector<Number> &v);
		MatMatrix& normalize();
		MatMatrix& rescale01();
                MatMatrix& msqrt();
                MatMatrix& mqrt();
                MatMatrix& abs();



		MatMatrix& matrix2vector(MatVector<Number> &v);
		std::string toString(void);
                std::string toCSV(void);
		MatMatrix& rowsAverages(MatVector<Number> &v);
		MatVector<Number>* rowsAverages(void);
		MatMatrix& colsAverages(MatVector<Number> &v);
		MatMatrix& rowsSum(MatVector<Number> &v);
		MatMatrix& colsSum(MatVector<Number> &v);
		MatMatrix& rowsMaximun(MatVector<Number> &v);
		MatMatrix& colsMaximun(MatVector<Number> &v);

		Number rowMin(unsigned int row);
		Number rowMax(unsigned int row);
		Number colMin(unsigned int col);
		Number colMax(unsigned int col);
		const Number max() const;
		Number max(uint &rindex, uint &cindex);
		Number min();
		Number min(uint &rindex, uint &cindex);
		void minMax(Number &min,Number &max);


		MetaMatMatrix<Number> operator+ (const MatMatrix<Number> &m);
		MetaMatMatrix<Number> operator+ (const MetaMatMatrix<Number> &meta);
		MetaMatMatrix<Number> operator+ (const MatVector<Number> &vmeta);
		MetaMatMatrix<Number> operator+ (const MetaMatVector<Number> &vmeta);
		MetaMatMatrix<Number> operator+ (const Number &k);
		MatMatrix& operator++();
		MatMatrix& operator--();
		MetaMatMatrix<Number> operator* (const MatMatrix<Number> &m);
		MetaMatMatrix<Number> operator* (const MetaMatMatrix<Number> &m);
		MetaMatMatrix<Number> operator* (const MatVector<Number> &m);
		MetaMatMatrix<Number> operator* (const MetaMatVector<Number> &m);
		MetaMatMatrix<Number> operator* (const Number &k);
		MetaMatMatrix<Number> operator/ (const Number k);
};


template<class Number=DefaultNumberType> class MetaMatMatrix: public MatMatrix<Number>
{
public:


	~MetaMatMatrix()
	{
#ifdef MatMatrixDebugOn
		using namespace std;
		if(MatMatrix<Number>::vect!=NULL)
			errMsg("MetaMatMatrix->vect nao ee nulo");
		else
			errMsg("MetaMatMatrix->vect ee nulo");
#endif


	};
};


typedef MatMatrix<double> MatMatrixDP;
typedef MatMatrix<float>  MatMatrixSP;


template<class Number> inline Number** MatMatrix<Number>::alloc(unsigned int rows,unsigned int cols)
{
#ifdef MatMatrixDebugOn
	using namespace std;
	errMsg("Alocando memoria->alloc(rows,cols)");
#endif
	Number **vectTemp;
	register unsigned int i;

	vectTemp = new Number*[rows];
	vectTemp[0] = new Number[cols*rows];

	for(i=1;i<rows;i++)
		vectTemp[i]=vectTemp[i-1]+cols;

	return vectTemp;
}


template<class Number> inline void MatMatrix<Number>::free(Number** vectTemp)
{
#ifdef MatMatrixDebugOn
	using namespace std;
	errMsg("MatMatrix<number>::free(vectTemp)");
#endif
	if(vectTemp!=NULL)
	{
#ifdef MatMatrixDebugOn
	using namespace std;
	errMsg("liberando memória->free(Number** vectTemp)");
#endif

		delete[] (vectTemp[0]);
		delete[] (vectTemp);
		vectTemp=NULL;
	}
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::swap(MatMatrix<Number> &m)
{
	register unsigned int temp;
	Number **vectTemp;

	temp = m.nrows;
	m.nrows = nrows;
	nrows = temp;

	temp = m.ncols;
	m.ncols = ncols;
	ncols = temp;

	vectTemp = m.vect;
	m.vect = vect;
	vect = vectTemp;
}

template<class Number> inline MatMatrix<Number>& MatMatrix<Number>::swap(MetaMatMatrix<Number> &meta)
{
	int tempNrows, tempNcols;
	Number **tempVect;

	nrows=tempNrows;
	ncols=tempNcols;
	vect=tempVect;

	nrows=meta.nrows;
	ncols=meta.ncols;
	vect=meta.vect;

	meta.nrows=tempNrows;
	meta.ncols=tempNcols;
	meta.vect=tempVect;
}

template<class Number> inline MatMatrix<Number>& MatMatrix<Number>::inputMeta(MetaMatMatrix<Number> &meta)
{
#ifdef MatMatrixDebugOn
	using namespace std;
	errMsg("Transferindo memória por cópia de ponteiros->inputMate(MetaMatMatrix<Number> &meta)");
#endif
	if(vect!=NULL)
		free(vect);

	nrows=meta.nrows;
	ncols=meta.ncols;
	vect=meta.vect;

	meta.nrows=0;
	meta.ncols=0;
	meta.vect=NULL;

	return *this;
}

template<class Number> inline MatMatrix<Number>& MatMatrix<Number>::outputMeta(MetaMatMatrix<Number> &meta)
{

	meta.nrows=nrows;
	meta.ncols=ncols;
	meta.vect=vect;

	nrows=0;
	ncols=0;
	vect=NULL;

	return *this;
}

template<class Number> inline Number* MatMatrix<Number>::getVect()
{
#ifdef MatMatrixDebugOn
	errMsg("MatMatrix::getVect() :"+COMPATIBILITY_MSG);
#endif

	return vect[0];
}

template<class Number> inline unsigned int MatMatrix<Number>::rows() const
{
	return nrows;
}

template<class Number> inline unsigned int MatMatrix<Number>::cols() const
{
	return ncols;
}

template<class Number> inline unsigned int MatMatrix<Number>::size() const
{
	return ncols*nrows;
}

template<class Number> inline MatMatrix<Number>& MatMatrix<Number>::clear()
{
	/**
	 * Libera a mem�ria alocada para a matriz e faz vect=NULL,ncols=0,nrows=0
	 */
	free(vect);
	ncols=0;
	nrows=0;

	return *this;
}

template<class Number> inline MatMatrix<Number>& MatMatrix<Number>::size(unsigned int r,unsigned int c)
{
	/**
	 * Redimenciona uma matriz sem conservar os valores antigos
	 */
	free(vect);
	ncols=c;
	nrows=r;

	vect = alloc(nrows,ncols);

	return *this;
}

template<class Number> inline MatMatrix<Number>& MatMatrix<Number>::size(const MatMatrix<Number> &m)
{
	size(m.rows(),m.cols());

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::operator =(const MetaMatMatrix<Number> &meta)
{
#ifdef MatMatrixDebugOn
	using namespace std;
	errMsg("Transferindo memória por cópia de ponteiros->operator=(const MetaMatMatrix<Number> &meta)");
#endif
	nrows=meta.nrows;
	ncols=meta.ncols;
	vect=meta.vect;
	meta.vect=NULL;

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::operator =(const MatMatrix<Number> &m)
{
#ifdef MatMatrixDebugOn
	using namespace std;
	errMsg("copiando memória->operator=(const MatMatrix<Number> &m)");
#endif
	using namespace std;
	if(this!=&m)
	{
		register unsigned int i,j;

		size(m.rows(),m.cols());

		for(i=0;i<nrows;i++)
			for(j=0;j<ncols;j++)
				vect[i][j]=m[i][j];
	}

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::operator =(const MatVector<Number> &v)
{
#ifdef MatMatrixDebugOn
	using namespace std;
	errMsg("copiando memória->operator=(const MatVector<Number> &v)");
#endif

	register unsigned int i;

	size(v.size(),1);

	for(i=0;i<nrows;i++)
		vect[i][1]=v[i];

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::operator =(const Number &k)
{
#ifdef MatMatrixDebugOn
	using namespace std;
	errMsg("copiando memória->operator=(const Number &k)");
#endif

	register unsigned int i,j;

	if(vect!=NULL)
	{
		for(i=0;i<nrows;i++)
			for(j=0;j<ncols;j++)
				vect[i][j]=k;

	}
	else
	{
		size(1,1);
		vect[0][0]=k;
	}

	return *this;

}

template<class Number> inline Number* MatMatrix<Number>::operator [](const unsigned int i)
{
	return vect[i];
}

template<class Number> inline const Number* MatMatrix<Number>::operator [](const unsigned int i) const
{
	return vect[i];
}

template<class Number> inline Number& MatMatrix<Number>::operator ()(const unsigned int i, const unsigned j)
{
	return vect[i][j];
}

template<class Number> inline const Number& MatMatrix<Number>::operator ()(const unsigned int i, const unsigned j) const
{
	return vect[i][j];
}


template<class Number> MatMatrix<Number>::MatMatrix(Number *v, unsigned int r, unsigned int c,bool memcopy)
{
	if(v!=NULL)
	{
		nrows=r;
		ncols=c;
		register unsigned int i;

		if(memcopy)
		{
			vect=alloc(nrows,ncols);

			unsigned int size_v=r*c;

			for(i=0;i<size_v;i++)
				vect[0][i]=v[i];
		}
		else
		{
			vect = new Number*[nrows];
			vect[0] = v;

			for(i=1;i<nrows;i++)
				vect[i]=vect[i-1]+ncols;

		}
	}
}

template<class Number> MatMatrix<Number>::MatMatrix(unsigned int r,unsigned int c)
{
#ifdef MatMatrixDebugOn
	using namespace std;
	errMsg("construindo objeto->MatMatrix(r,c)");
#endif
	if((r!=0)&&(c!=0))
	{
		nrows=r;
		ncols=c;
		vect=alloc(nrows,ncols);
	}
};

template<class Number> MatMatrix<Number>::MatMatrix()
{
#ifdef MatMatrixDebugOn
	using namespace std;
	errMsg("construindo objeto->MatMatrix()");
#endif
	ncols=0;
	nrows=0;
	vect=NULL;
};

template<class Number> MatMatrix<Number>::MatMatrix(const MatMatrix<Number> &m)
{
#ifdef MatMatrixDebugOn
	using namespace std;
	errMsg("construindo objeto->MatMatrix(const MatMatrix<Number> &m)");
#endif
	if(m.size()!=0)
	{
		register unsigned int i,j;

		nrows=m.rows();
		ncols=m.cols();;
		vect=alloc(nrows,ncols);

		for(i=0;i<nrows;i++)
			for(j=0;j<ncols;j++)
			{
				vect[i][j]=m[i][j];
			}
	}
};

template<class Number> MatMatrix<Number>::MatMatrix(const MatVector<Number> &v, operationType op)
{
#ifdef MatMatrixDebugOn
	using namespace std;
	errMsg("construindo objeto->MatMatrix(const MatMatrix<Number> &v,operationType op=column)");
#endif
	if(v.size()!=0)
	{
		if(op==row)
		{
			register unsigned int j;

			nrows=1;
			ncols=v.size();
			vect=alloc(nrows,ncols);

			for(j=0;j<ncols;j++)
			{
				vect[0][j]=v[j];
			}
		}
		else
		{
			register unsigned int i;

			nrows=v.size();
			ncols=1;
			vect=alloc(nrows,ncols);

			for(i=0;i<nrows;i++)
			{
				vect[i][0]=v[i];
			}
		}

	}
	else
	{
		ncols=0;
		nrows=0;
		vect=NULL;

		errMsg(INVALID_DIMENSION_SIZE);
	}
};

template<class Number> MatMatrix<Number>::MatMatrix(MetaMatMatrix<Number> &meta)
{
#ifdef MatMatrixDebugOn
	using namespace std;
	errMsg("construindo objeto->MatMatrix(MetaMatMatrix<Number> &meta)");
#endif
	nrows=meta.nrows;
	ncols=meta.ncols;
	vect=meta.vect;
	meta.vect=NULL;
}


template<class Number> MatMatrix<Number>::~MatMatrix()
{
#ifdef MatMatrixDebugOn
	using namespace std;
	errMsg("destruindo objeto");
#endif
	free(vect);
}


template<class Number> MatMatrix<Number>& MatMatrix<Number>::resize(unsigned int r,unsigned int c)
{
	/**
	 * Redimenciona uma matriz conservando os valores antigos
	 */


	if(vect!=NULL)
	{
		register unsigned int i,j;

		//Modificado de int para unsigned em 05/08/2008 ->>> Vai dar merda....
		unsigned int rMin=MatUtils::min(r,nrows);
		unsigned int cMin=MatUtils::min(c,ncols);

		Number **vectTemp;

		vectTemp = alloc(r,c);

		for(i=0;i<rMin;i++)
			for(j=0;j<cMin;j++)
				vectTemp[i][j]=vect[i][j];

		free(vect);

		vect=vectTemp;
		nrows=r;
		ncols=c;
	}
	else
		size(r,c);

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::fill(Number x)
{
	if(vect!=NULL)
	{
		matVFill<Number>(vect[0],size(),x);
	}
	else
		errMsg(MEM_NO_ALLOC);

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::fill(Number x,unsigned int r,unsigned int c)
{
	if((r!=0)&&(c!=0))
	{
		size(r,c);
		fill(x);
	}
	else
		errMsg(INVALID_DIMENSION_SIZE);

	return *this;
 }



template<class Number> MatMatrix<Number>& MatMatrix<Number>::zeros(unsigned int r,unsigned int c)
{
	fill(0,r,c);

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::zeros()
{
	fill(0);

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::ones(unsigned int r,unsigned int c)
{
	fill(1,r,c);

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::ones()
{
	fill(1);

	return *this;
}

/**
 * Semente de numeros aleatorios
 * @author Daniel F.Gomes
 * @since 08-05-2008
 */
template<class Number> MatMatrix<Number>& MatMatrix<Number>::srandom(const long s)
{
	srand(s);

	return *this;
}


template<class Number> MatMatrix<Number>& MatMatrix<Number>::random(const int s)
{
	size(s);
	matVFillUnoise(vect[0],size());

	return *this;
}

/**
 * Preenche o vetor com números aleatórios com distribuição uniforme de 0 a 1
 * @author Daniel F.Gomes
 * @since  08-05-2008
 */
template<class Number> MatMatrix<Number>& MatMatrix<Number>::random(void)
{
	matVFillUnoise<Number>(vect[0],size());

	return *this;
}

/**
 * Retorna a média dos valores contidos na matriz
 * return valor médio do vetor
 * @author Daniel F.Gomes
 * @since  08-05-2008
 */
template<class Number> Number MatMatrix<Number>::mean()
{
	return matVMean<Number>(vect[0],size());
};

/**
 * Retorna a variancia dos valores contidos na matriz
 * return variancia do vetor
 * @author Daniel F.Gomes
 * @since  08-05-2008
 */
template<class Number> Number MatMatrix<Number>::var()
{
	return matVVar<Number>(vect[0],size());
}

/**
 * Retorna o desvio padrão do vetor
 * return desvio padrão
 * @author Daniel F.Gomes
 * @since  08-05-2008
 */
template<class Number> inline Number MatMatrix<Number>::stdDev()
{
	return sqrt(var());
}

template<class Number> inline Number MatMatrix<Number>::sum()
{
    return matVSum<Number>(vect[0],size());
};

template<class Number> inline Number MatMatrix<Number>::prod()
{
    return matVProd<Number>(vect[0],size());
};

template<class Number> MatMatrix<Number>& MatMatrix<Number>::range(const Number &start,const Number &stop)
{
	if(vect!=NULL)
	{
		register unsigned int i,j;

		Number counter=0;

		Number step=(stop-start)/(size()-1);

		counter=start;
		for(i=0;i<nrows;i++)
		{
			for(j=0;j<ncols;j++)
			{
				vect[i][j]=counter;
				counter+=step;
			}

		}
	}
	else
		errMsg(INVALID_DIMENSION_SIZE);

	return *this;
}


template<class Number> MatMatrix<Number>& MatMatrix<Number>::rangeRows(const Number &start,const Number &stop,const Number &step)
{
	if(vect!=NULL)
	{
		register unsigned int i,j;
		Number counter;

		size((int)((stop-start)/step),ncols);

		counter=start;
		for(i=0;i<nrows;i++)
		{
			for(j=0;j<ncols;j++)
			{
				vect[i][j]=counter;
			}
			counter+=step;
		}
	}

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::rangeCols(const Number &start,const Number &stop,const Number &step)
{
	if(vect!=NULL)
	{
		register unsigned int i,j;
		Number counter;

		size(nrows,(int)((stop-start)/step));

		counter=start;
		for(j=0;j<ncols;j++)
		{
			for(i=0;i<nrows;i++)
			{
				vect[i][j]=counter;
			}
			counter+=step;
		}
	}
	return *this;
}



template<class Number> MatMatrix<Number>& MatMatrix<Number>::concatRows(const MatMatrix<Number> &m)
{
	if(m.cols()==ncols)
	{
		register unsigned int i,j,counter;
		int newnRows=nrows+m.rows();

		Number **vectTemp;

		vectTemp = alloc(newnRows,ncols);

		for(i=0;i<nrows;i++)
			for(j=0;j<ncols;j++)
				vectTemp[i][j]=vect[i][j];

		counter=0;
		for(i=nrows;i<newnRows;i++)
		{
			for(j=0;j<ncols;j++)
				vectTemp[i][j]=m[counter][j];

			counter++;
		}

		free(vect);
		nrows=newnRows;
		vect=vectTemp;
	}
	else
		if(vect==NULL)
		{
			(*this)=m;
		}

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::concatRows(const MatVector<Number> &v)
{
	unsigned int vsize=v.size();

	if(vsize!=0)
	{
		if(vect==NULL)
		{
			size(1,vsize);

			register unsigned int j;
			for(j=0;j<vsize;j++)
			{
				vect[0][j]=v[j];
			}
		}
		else
		if(vsize==ncols)
		{
			register unsigned int i,j;
			Number **vectTemp;

			vectTemp = alloc(nrows+1,ncols);

			for(i=0;i<nrows;i++)
				for(j=0;j<ncols;j++)
					vectTemp[i][j]=vect[i][j];

			for(j=0;j<ncols;j++)
				vectTemp[nrows][j]=v[j];



			free(vect);
			nrows++;
			vect=vectTemp;

		}
		else
			errMsg(INVALID_DIMENSION_SIZE);
	}
	else
		errMsg(INVALID_DIMENSION_SIZE);

	return *this;
}


/**
 * Concatena um matriz a outra matriz
 * @param m - Uma matriz com o mesmo n�mero de linhas da matriz
 *
 * @author Daniel F.Gomes
 * @since  10-01-2008
 */
template<class Number> MatMatrix<Number>& MatMatrix<Number>::concatCols(const MatMatrix<Number> &m)
{
	if(m.rows()==nrows)
	{
		register unsigned int i,j,counter;
		int newnCols=ncols+m.cols();

		Number **vectTemp;

		vectTemp = alloc(nrows,newnCols);

		for(i=0;i<nrows;i++)
			for(j=0;j<ncols;j++)
				vectTemp[i][j]=vect[i][j];

		counter=0;
		for(j=ncols;j<newnCols;j++)
		{
			for(i=0;i<nrows;i++)
				vectTemp[i][j]=m[i][counter];

			counter++;
		}

		free();
		vect=vectTemp;
		ncols=newnCols;
	}
	else
		if(vect==NULL)
		{
			(*this)=m;
		}

	return *this;
}

/**
 * Concatena um vetor a uma matriz
 * @param v - Vetor com o mesmo n�mero de linhas da matriz
 *
 * @author Daniel F.Gomes
 * @since  10-01-2008
 */
template<class Number> MatMatrix<Number>& MatMatrix<Number>::concatCols(const MatVector<Number> &v)
{
	unsigned int vsize=v.size();

	if(vsize!=0)
	{
		if(vect==NULL)
		{
			size(vsize,1);

			register unsigned int i;
			for(i=0;i<vsize;i++)
			{
				vect[i][0]=v[i];
			}
		}
		else
		if(vsize==nrows)
		{
			register unsigned int i,j;
			Number **vectTemp;

			vectTemp=alloc(nrows,ncols+1);

			for(i=0;i<nrows;i++)
				for(j=0;j<ncols;j++)
					vectTemp[i][j]=vect[i][j];

			for(i=0;i<nrows;i++)
				vectTemp[i][ncols]=v[i];

			free(vect);
			vect=vectTemp;
			ncols++;
		}
		else
			errMsg(INVALID_DIMENSION_SIZE);
	}
	else
		errMsg(INVALID_DIMENSION_SIZE);

	return *this;
}


template<class Number> MatMatrix<Number>* MatMatrix<Number>::clone(void)
{
	MatMatrix<Number> *m = new MatMatrix<Number>((*this));

	return m;
}


template<class Number> MatMatrix<Number>& MatMatrix<Number>::getRow(int row,MatMatrix<Number> &m)
{
  if (row>=nrows)
  {
	  m.clear();
  }
  else
  {
	  m.size(1,ncols);
	  for (register unsigned int j=0; j<ncols; j++)
	  {
	      m[0][j] = vect[row][j];
	  }
  }

  return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::getCol(int col,MatMatrix<Number> &m)
{

  if (col>=ncols)
  {
	  m.clear();
  }
  else
  {
	  m.size(nrows,1);
	  for (register unsigned int i=0; i<nrows; i++)
	  {
	      m[i][0] = vect[row][i];
	  }
  }

  return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::getRow(int row,MatVector<Number> &v)
{

  if (row>=(int)nrows)
  {
	  v.clear();
  }
  else
  {
	  v.size(ncols);
	  for (register unsigned int j=0; j<ncols; j++)
	  {
	      v[j] = vect[row][j];
	  }
  }

  return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::getCol(int col,MatVector<Number> &v)
{

  if (col>=(int)ncols)
  {
	  v.clear();
  }
  else
  {
	  v.size(nrows);
	  for (register unsigned int i=0; i<nrows; i++)
	  {
	      v[i] = vect[i][col];
	  }
  }

  return *this;
}

template<class Number> inline MatMatrix<Number>& MatMatrix<Number>::set(const Number &x, unsigned int row, unsigned int col)
{
	(*this)[row][col] = x;
	return *this;
}

template<class Number> inline MatMatrix<Number>& MatMatrix<Number>::setRow(unsigned int r, MatVector<Number> &row)
{    
    for (register unsigned int c=0; c<row.size(); c++)
	(*this)[r][c] = row[c];
    
	return *this;
}

template<class Number> inline const Number& MatMatrix<Number>::get(unsigned int row, unsigned int col)
{
	return (*this)[row][col];
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::eye(unsigned int n)
{
	register unsigned int i,j;

	size(n,n);

	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
		{
			if(i==j)
				vect[i][j]=1;
			else
				vect[i][j]=0;
		}

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::eye()
{
	if(vect!=NULL)
	{
		if(nrows==ncols)
		{
			register unsigned int i,j;

			for(i=0;i<nrows;i++)
				for(j=0;j<nrows;j++)
				{
					if(i==j)
						vect[i][j]=1;
					else
						vect[i][j]=0;
				}
		}
		else
			errMsg(INVALID_DIMENSION_SIZE);
	}
	else
		errMsg(MEM_NO_ALLOC);

	return *this;
}


template<class Number> MatMatrix<Number>& MatMatrix<Number>::diag(const MatVector<Number> &v)
{
	int n=v.size();
	register unsigned int i,j;

	size(n,n);

	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
		{
			if(i==j)
				vect[i][j]=v[j];
			else
				vect[i][j]=0;
		}

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::diag(const MatMatrix<Number> &m)
{

	if(m.size()!=0)
	{
		register unsigned int i,j;
		int n=MatUtils::min(m.rows(),m.cols());

		size(n,n);

		for(i=0;i<n;i++)
		{
			for(j=0;j<n;j++)
			{
				if(i==j)
					vect[i][j]=m[i][j];
				else
					vect[i][j]=0;
			}
		}

	}
	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::diag()
{
	if(vect!=NULL)
	{
		register unsigned int i,j;

		for(i=0;i<nrows;i++)
		{
			for(j=0;j<ncols;j++)
				if(i!=j)
					vect[i][j]=0;
		}

	}
	else
		errMsg(MEM_NO_ALLOC);

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::getDiag(MatVector<Number> &v)
{
	if(vect!=NULL)
	{
		unsigned int n=MatUtils::min(ncols,nrows);
		register unsigned int i;

		v.size(n);

		for(i=0;i<n;i++)
		{
			v[i]=vect[i][i];
		}

	}
	else
		errMsg(MEM_NO_ALLOC);

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::rotMatrix4X(Number theta)
{
	eye(4);

	double cosTheta = cos(theta);
	double sinTheta = sin(theta);

	vect[1][1]= cosTheta;
	vect[1][2]= sinTheta;
	vect[2][1]= -sinTheta;
	vect[2][2]= cosTheta;

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::rotMatrix4Y(Number theta)
{
	eye(4);

	double cosTheta = cos(theta);
	double sinTheta = sin(theta);

	vect[0][0]= cosTheta;
	vect[0][2]= sinTheta;
	vect[2][0]= -sinTheta;
	vect[2][2]= cosTheta;

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::rotMatrix4Z(Number theta)
{
	eye(4);

	double cosTheta = cos(theta);
	double sinTheta = sin(theta);

	vect[0][0]= cosTheta;
	vect[0][1]= sinTheta;
	vect[1][0]= -sinTheta;
	vect[1][1]= cosTheta;

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::rotMatrix4XYZ(Number thetaX,Number thetaY,Number thetaZ)
{
	MatMatrix<Number> mZTemp;
	MatMatrix<Number> mYTemp;
	mZTemp.rotMatrix4Z(thetaZ);
	mYTemp.rotMatrix4Y(thetaY);
	rotMatrix4X(thetaX);

	mYTemp.mult(mZTemp);
	mult(mYTemp);

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::transMatrix4(const MatMatrix<Number> &m)
{
	eye(4);

	vect[0][3]=m[0][0];
	vect[1][3]=m[1][0];
	vect[2][3]=m[2][0];

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::transMatrix4(const MatVector<Number> &v)
{
	eye(4);

	vect[0][3]=v[0];
	vect[1][3]=v[1];
	vect[2][3]=v[2];

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::transMatrix4(Number x0, Number y0, Number z0)
{
	eye(4);

	vect[0][3]=x0;
	vect[1][3]=y0;
	vect[2][3]=z0;

	return *this;

}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::transMatrix4(Number v[3])
{
	transMatrix4(v[0],v[1],v[2]);

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::scaleMatrix4(Number scaleX,Number scaleY,Number scaleZ)
{
	eye(4);

	vect[0][0]=scaleX;
	vect[1][1]=scaleY;
	vect[2][2]=scaleZ;

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::scaleMatrix4(Number scale)
{
	scaleMatrix4(scale,scale,scale);

	return *this;
}


template<class Number> MatMatrix<Number>& MatMatrix<Number>::perspectiveMatrix4(Number gama)
{
	eye(4);

	vect[3][2]= -1/gama;

	return *this;
}


template<class Number> MatMatrix<Number>& MatMatrix<Number>::trn(void)
{
	register unsigned int i,j;

	if(nrows==ncols)
	{
		Number temp;

		for(i=0;i<ncols;i++)
		{
			for(j=i;j<nrows;j++)
			{
				temp=vect[i][j];
				vect[i][j]=vect[j][i];
				vect[j][i]=temp;
			}
		}
	}
	else
	{
		Number **vectTemp;

		vectTemp = alloc(ncols,nrows);

		for(j=1;j<ncols;j++)
			vectTemp[j]=vectTemp[j-1]+nrows;

		for(i=0;i<nrows;i++)
		{
			for(j=0;j<ncols;j++)
			{
				vectTemp[j][i]=vect[i][j];
			}
		}

		free(vect);
		vect=vectTemp;
		unsigned int temp=nrows;
		nrows=ncols;
		ncols=temp;
	}

	return *this;
}

template<class Number> Number MatMatrix<Number>::det()
{
	if(ncols==nrows)
	{
		Number **vectTemp = alloc(ncols,ncols);
		matVCopy(vectTemp[0],vect[0],ncols*ncols);

		uint32 *mrow = new uint32[ncols];

		matMVGEM<Number>(vectTemp,mrow,ncols);

		Number temp=matMDet<Number>(vectTemp,mrow,ncols);

		delete[] mrow;
		free(vectTemp);

		return temp;
	}
	else
	{
		errMsg(INVALID_DIMENSION_SIZE);
	}

	return MatUtils::NaN;
}

template<class Number> Number MatMatrix<Number>::tr()
{
	register unsigned int i;
	int n=MatUtils::min(nrows,ncols);

	Number dsum=0;

	for(i=0;i<n;i++)
		dsum+=vect[i][i];

	return dsum;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::add(Number k)
{

	matVAdd(vect[0],size(),k);

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::add(const MatVector<Number> &v,operationType op )
{
	if(op==column)
	{
		if(v.size()==nrows)
		{
			register unsigned int i,j;

			for(i=0;i<nrows;i++)
			{
				for(j=0;j<ncols;j++)
				{
					vect[i][j]=vect[i][j]+v[i];
				}
			}
		}
		else
			errMsg(INVALID_DIMENSION_SIZE);
	}
	else
	if(op==row)
	{
		if(v.size()==ncols)
		{
			register unsigned int i,j;

			for(i=0;i<nrows;i++)
			{
				for(j=0;j<ncols;j++)
				{
					vect[i][j]=vect[i][j]+v[j];
				}
			}
		}
		else
			errMsg(INVALID_DIMENSION_SIZE);

	}
	else
		errMsg(INVALID_TYPE);

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::add(const MatMatrix<Number> &m)
{
	if(vect==NULL)
	{
		(*this)=m;
	}
	else
	if((m.rows()==nrows)&&(m.cols()==ncols))
	{
		matVAdd(vect[0],m.vect[0],size());
	}
	else
		errMsg(INVALID_DIMENSION_SIZE);

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::mult(Number k)
{
	if(vect!=NULL)
	{
		matVMult(vect[0],size(),k);
	}

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::mult(const MatVector<Number> &v)
{
	if(ncols==v.size())
	{
		register unsigned int i,j;

		Number **vectTemp;

		vectTemp = alloc(nrows,1);

		matMVMult(vect,v.vect,vectTemp[0],nrows,ncols);

		free(vect);
		ncols=1;
		vect=vectTemp;

	}
	else
		errMsg(INVALID_DIMENSION_SIZE);

	return *this;
}


template<class Number> MatMatrix<Number>& MatMatrix<Number>::mult(const MatMatrix<Number> &m)
{
	if(ncols==m.rows())
	{
		Number **vectTemp = alloc(nrows,m.cols());

		matMMMult(vect,m.vect,vectTemp,nrows,ncols,m.cols());

		free(vect);
		ncols=m.cols();
		vect=vectTemp;

	}
	else
		errMsg(INVALID_DIMENSION_SIZE);

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::preMult(const MatMatrix<Number> &m)
{
	if(nrows==m.cols())
	{
		Number **vectTemp=alloc(m.rows(),ncols);
		matMMMult(m.vect,vect,vectTemp,m.rows(),m.cols(),ncols);

		free(vect);
		nrows=m.rows();
		vect=vectTemp;
	}
	else
		errMsg(INVALID_DIMENSION_SIZE);

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::normalize()
{
	matVNormalize<Number>(vect[0],size());
	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::rescale01()
{
	matVRescale01<Number>(vect[0],size());
	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::mqrt()
{
        register unsigned int i,j;
        for(i=0;i<nrows;i++)
        {
            for(j=0;j<ncols;j++)
            {
                vect[i][j] = vect[i][j]*vect[i][j];
            }
        }
	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::msqrt()
{
        register unsigned int i,j;
        for(i=0;i<nrows;i++)
        {
            for(j=0;j<ncols;j++)
            {
                vect[i][j] = sqrt(vect[i][j]);
            }
        }
	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::abs()
{
        register unsigned int i,j;
        for(i=0;i<nrows;i++)
        {
            for(j=0;j<ncols;j++)
            {
                vect[i][j] = fabs(vect[i][j]);
            }
        }
	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::matrix2vector(MatVector<Number> &v)
{
	//TODO Rotina para transferir os dados de uma matriz para um vetor;
	unsigned int n=size();
	v.size(n);
	matVCopy(v.vect,vect[0],n);
	return *this;
}

template<class Number> std::string MatMatrix<Number>::toString(void)
{
	using namespace std;
	string outputS;

	if(vect!=NULL)
	{
		register unsigned int i,j;
		char strbuff[16];
		outputS += "[ ";

		for(i=0;i<nrows;i++)
		{
			for(j=0;j<ncols;j++)
			{
				sprintf(strbuff,"%.2f",(double)vect[i][j]);
				outputS += strbuff;
				outputS += " ";
			}

			if(i<(nrows-1))
				outputS += ";\n";
		}

		outputS += "]";
	}
	else
		outputS += "[NULL]";

	return outputS;
}

template<class Number> std::string MatMatrix<Number>::toCSV(void)
{
	using namespace std;
	string outputS;

	if(vect!=NULL)
	{
		register unsigned int i,j;
		char strbuff[16];

		for(i=0;i<nrows;i++)
		{
			for(j=0;j<ncols;j++)
			{
				sprintf(strbuff,"%.2f",(double)vect[i][j]);
				outputS += strbuff;
				outputS += "\t";
			}

			if(i<(nrows-1))
				outputS += "\n";
		}

		outputS += "\n";
	}
	else
		outputS += "[NULL]";

	return outputS;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::rowsAverages(MatVector<Number> &v)
{
	register unsigned int i,j;
	Number sum;
	v.size(nrows);

	for(i=0;i<nrows;i++)
	{
		sum=0;
		for (j=0;j<ncols;j++)
			  sum += vect[i][j];

		v[i]=sum/ncols;
	}

	return *this;
}

template<class Number> MatVector<Number>* MatMatrix<Number>::rowsAverages(void)
{
	errMsg(COMPATIBILITY_MSG);
	MatVector<Number> *v = new MatVector<Number>();
	rowsAverages(*v);

	return v;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::colsAverages(MatVector<Number> &v)
{
	register unsigned int i,j;
	Number sum;
	v.size(ncols);


	for (j=0;j<ncols;j++)
	{
		sum=0;
		for(i=0;i<nrows;i++)
			  sum += vect[i][j];

		v[j]=sum/nrows;
	}

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::colsSum(MatVector<Number> &v)
{
	if(vect!=NULL)
	{
		//Calcula a média de cada colun
		register unsigned int i,j;
		Number sum;
		v.size(ncols);

		for (j=0; j<ncols; j++)
		{
			sum = 0;
			for (i=0; i<nrows; i++)
				sum +=vect[i][j];

			v[j]=sum;
		}

	}

	return *this;
}

template<class Number> MatMatrix<Number>& MatMatrix<Number>::colsMaximun(MatVector<Number> &v)
{
	if(vect!=NULL)
	{
		//Calcula a média de cada colun
		register unsigned int i,j;
		Number maxvar;
		v.size(ncols);

		for (j=0; j<ncols; j++)
		{
			maxvar = vect[0][j];
			for (i=1; i<nrows; i++)
				if(maxvar<vect[i][j])
					maxvar=vect[i][j];

			v[j]=maxvar;
		}

	}

	return *this;
}

template<class Number> Number MatMatrix<Number>::rowMin(unsigned int row)
{
	Number temp;
	Number min = vect[row][0];

	for (register unsigned int j=1; j<ncols; j++)
	{
		temp = vect[row][j];
		if (temp<min)
			min = temp;
	}

	return min;
}

template<class Number> Number MatMatrix<Number>::rowMax(unsigned int row)
{
	Number temp;
	Number max = vect[row][0];

	for (register unsigned int j=1; j<ncols; j++)
	{
		temp = vect[row][j];
		if (temp>max)
			max = temp;
	}

	return max;
}

template<class Number> Number MatMatrix<Number>::colMin(unsigned int col)
{
	Number temp;
	Number min = vect[0][col];

	for (register unsigned int i=1; i<nrows; i++)
	{
		temp = vect[i][col];
		if (temp<min)
			min = temp;
	}

	return min;
}

template<class Number> Number MatMatrix<Number>::colMax(unsigned int col)
{
	Number temp;
	Number max = vect[0][col];

	for (register unsigned int i=1; i<nrows; i++)
	{
		temp = vect[i][col];
		if (temp>max)
			max = temp;
	}

	return max;
}

/**
 * Retorna valor m�ximo
 * return valor m�ximo
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> const Number MatMatrix<Number>::max() const
{
	return matVMax<Number>(vect[0],size());
}

/**
 * Retorna valor m�ximo
 * return valor m�ximo do vetor e a posis�o deste
 * @author Daniel F.Gomes
 * @since  17-08-2008
 */
template<class Number> Number MatMatrix<Number>::max(uint &rindex,uint &cindex)
{

	Number ret=matVMaxIndex<Number>(vect[0],rindex,size());

	div_t divresult;
	divresult = div(rindex,cols());

	rindex=divresult.quot;
	cindex=divresult.rem;

	return ret;
}


/**
 * Retorna valor m�nimo
 * return valor m�nimo
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> Number MatMatrix<Number>::min()
{
	return matVMin<Number>(vect[0],size());
}

/**
 * Retorna valor m�ximo
 * return valor m�ximo do vetor e a posis�o deste
 * @author Daniel F.Gomes
 * @since  17-08-2008
 */
template<class Number> Number MatMatrix<Number>::min(uint &rindex,uint &cindex)
{
	Number ret=matVMinIndex<Number>(vect[0],rindex,size());

	div_t divresult;
	divresult = div(rindex,cols());

	rindex=divresult.quot;
	cindex=divresult.rem;

	return ret;
}

template<class Number> void MatMatrix<Number>::minMax(Number &vmin,Number &vmax)
{
	matVMinMax<Number>(vect[0],size(),vmin,vmax);
}


std::ostream& operator << (std::ostream& out,MatMatrix<int> &m);
std::ostream& operator << (std::ostream& out,MatMatrix<float> &m);
std::ostream& operator << (std::ostream& out,MatMatrix<double> &m);




#endif /*MATMATRIX_H_*/
