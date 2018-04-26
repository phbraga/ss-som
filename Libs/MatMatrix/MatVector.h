#ifndef MATVECTOR_H_
#define MATVECTOR_H_

//#define MatVectorDebugOn0 // Para ativar o modo debug mensagens
//#define MatVectorDebugOn1 //
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <list>
#include <cmath>
#include "MatUtils.h"
#include "MatAlgorithms.h"
#include "MatMatrix.h"
#include <typeinfo>

//#define MatVectorDebugOn0
//#define MatVectorDebugOn1
//#define MatVectorDebugOn2

/*
 *A notações
 *
 *Proposta de novo divisão das rotinas da MatMatix
 *Classes
 *As classes Base só terão rotinas de manipulação de dados
 *sem nenuma rotina matemática
 * StaticShortVectorBase - Sem alocação dinamica de memória utilizada para
 *                       vetores pequenos que necessitem de artimética rápida
 * VectorBase - rotinas de manipulação de dados, mas sem rotinas de alocação de memória
 * MatrixBase:VectorBase - extende VectorBase para manipulações de dados em matrizes
 * SliceVectorBase:VectorBase - Representação de partes com memória compartilhada
 * SliceMatrixBase:MatrixBase - Representação de partes com memória compartilhada
 * MatVectorBase - Só rotinas matemáticas
 * MatMatrixBase - Só rotinas matemáticas
 * MatVector:VectorBase,MatVectorBase+alocador
 * MatMatrix:MatVector+alocador
 */


enum operationType {
    column, row, diag, overall
};
template<class Number> class MetaMatVector;

template<class Number = DefaultNumberType> class MatVector {
protected:
    /**
     * TODO O gcc aloca mem�ria para os objetos de 4 em 4 bytes.
     * Assim, em uma implementa��o futura, � melhor separa a capacidade
     * de mem�ria compartilhada em uma nova classe que herde as caracteristicas
     * de MatVector. Isso porque a simples adi��o de uma variavel bool a classe
     * MatVector faz o tamanho do objeto saltar de 8 bytes para 12 byte.
     */

    mutable unsigned int nrows;
    mutable Number *vect;


public:

    typedef Number TNumber;
    inline void clear();
    inline void size(const unsigned int &size);
    inline unsigned int size() const;
    inline Number* getVect(void);
#ifdef MatVectorDebugOn1
    static int allocs;
    static int deallocs;
#endif


    MatVector();
    explicit MatVector(const unsigned int size);
    MatVector(const Number &v, const unsigned int size);
    MatVector(const Number *v, const unsigned int size);
    explicit MatVector(const MetaMatVector<Number> &v);
    explicit MatVector(MetaMatVector<Number> &v);
    MatVector(const MatVector<Number> &v);
    MatVector(MatVector<Number> &v);
    //explicit MatVector(MatVector<Number> v);

    ~MatVector();

    inline Number & operator[](const int i);
    inline const Number & operator[](const int i) const;
    inline Number& at(const int i);
    inline const Number& at(const int i) const;

    inline Number& x(void) {
        Number x = 0;
        return nrows > 0 ? vect[0] : x;
    };

    inline Number& y(void) {
        Number x = 0;
        return nrows > 1 ? vect[1] : x;
    };

    inline Number& z(void) {
        Number x = 0;
        return nrows > 2 ? vect[2] : x;
    };

    inline Number& t(void) {
        Number x = 0;
        return nrows > 3 ? vect[3] : x;
    };

    inline const Number& x(void) const {
        return nrows > 0 ? vect[0] : 0;
    };

    inline const Number& y(void) const {
        return nrows > 1 ? vect[1] : 0;
    };

    inline const Number& z(void) const {
        return nrows > 2 ? vect[2] : 0;
    };

    inline const Number& t(void) const {
        return nrows > 3 ? vect[3] : 0;
    };

    MatVector& operator=(const MetaMatVector<Number> &vmeta);
    MatVector& operator=(const MatVector<Number> &v);
    MatVector& operator=(const Number &k);
    MatVector& resize(const unsigned int s);
    MatVector& resize(const unsigned int s, const Number &fillN);
    bool existNan(void);
    MatVector& replaceInf(double x);
    MatVector& range(const Number &start, const Number &stop, Number step);
    MatVector& range(const Number &start, const Number &stop);
    MatVector& fill(const Number x);
    MatVector& fill(const Number x, const unsigned int vsize);
    inline MatVector& zeros(void);
    inline MatVector& zeros(const unsigned int newSize);
    inline MatVector& ones(void);
    inline MatVector& ones(const unsigned int newSize);
    MatVector& srandom(const long s);
    MatVector& random(const int s);
    MatVector& random(void);
    MatVector& randomn(const Number &m, const Number &var);
    MatVector& randomn(const MatVector<Number> &vm, const Number &var);
    MatVector& randomn(const MatVector<Number> &vm, const MatVector<Number> &vvar);
    MatVector& shuffler(void);
    MatVector& concat(const MatVector<Number> &v);
    MatVector& concat(const MatVector<Number> &v1, const MatVector<Number> &v2);
    MatVector& append(Number k);
    MatVector& copy(const MatVector<Number> &v, const int firsts);
    MatVector& copy(const MatVector<Number> &v, int beginIndex, int endIndex);
    MatVector& flip(void);
    MatVector& shift(int d, Number vfill = 0);
    MatVector* clone(void);
    MatVector& swap(MatVector<Number> &v);
    Number mean();
    Number var();
    Number cov(const MatVector<Number> &v);
    Number corr(const MatVector<Number> &v);
    inline Number stdDev();
    Number max() const;
    Number max(uint &index) const;
    template<class T> MatVector& max(const MatVector<T> &v);
    Number min() const;
    Number min(uint &index) const;
    template<class T> MatVector& min(const MatVector<T> &v);
    MatVector& minMax(Number &min, Number &max);
    Number dot(const MatVector<Number> &v);
    MetaMatVector<Number> cross(const MatVector<Number> &v); //Só é válido para vetores 3D
    Number dist2(const MatVector<Number> &v) const;
    Number dist(const MatVector<Number> &v) const;
    Number norm2() const;
    inline Number norm() const;
    MatVector& rescale01();
    MatVector& vpow(const Number e);
    MatVector& vsqrt(void);
    MatVector& vqrt(void);
    MatVector& vlog(void);
    MatVector& vabs(void);
    Number polival(const Number x);
    //MatVector& sort(const unsigned int left, const unsigned int right);
    MatVector& sort(void);
    Number sortAndCalcMedian();
    MetaMatVector<Number> sortAndCalcOrderStatistics();
    MetaMatVector<Number> orderStatistics();
    MetaMatVector<Number> statistics();
    Number median();
    Number sum() const;
    Number prod();
    template<class T> MatVector& mult(const MatVector<T> &v);
    template<class T> MatVector& mult(const T k);
    template<class T> MatVector& div(const MatVector<T> &v);
    template<class T> MatVector& div(const T k);
    template<class T> MatVector& add(const T k);
    MatVector& inv(void);
    template<class T> MatVector& add(const MatVector<T> &v);
    template<class T> MatVector& sub(const MatVector<T> &v);
    template<class T> MatVector& sub(const T k);
    MatVector& neg(void);
    MatVector& normalize();
    MatVector& diff(const Number &k);
    inline MatVector& normalize2One();
    MatVector& apply(float f(float));
    MatVector& apply(float f(const float&));
    MatVector& apply(double f(double));
    MatVector& apply(double f(const double&));



    inline bool isEmpty(void);
    bool isEqual(const MatVector<Number> &v);
    bool operator==(const MatVector<Number> &v);
    bool operator!=(const MatVector<Number> &v);
    bool operator>(MatVector<Number> &v);
    bool operator>=(MatVector<Number> &v);
    bool operator<(MatVector<Number> &v);
    bool operator<=(MatVector<Number> &v);
    template<class T> inline MatVector& operator+=(const MatVector<T> &v);
    template<class T> inline MatVector& operator+=(const T &k);
    inline MatVector& operator++();
    template<class T> inline MatVector& operator-=(const MatVector<T> &v);
    template<class T> inline MatVector& operator-=(const T &k);
    inline MatVector& operator--();
    template<class T> inline MatVector& operator*=(const MatVector<T> &v);
    template<class T> inline MatVector& operator*=(const T &k);
    std::string toString() const;
    std::string toCSV() const;
    bool save2Txt(std::ofstream outfile);
    bool save2Txt(std::string fileName);
    bool save2Sce(const std::string &varName, std::ofstream &outfile);
    bool save2Sce(const std::string &varName, const std::string &fileName);

};

#ifdef MatVectorDebugOn1
template<class Number> int MatVector<Number>::allocs = 0;
template<class Number> int MatVector<Number>::deallocs = 0;
#endif

/*
 *Quando um objeto é declarado como MetaMatVector esta inplicito que o conteúdo deste objeto
 *pode ser alterado de qualquer maneira conveiente na rotina que chama o objeto.
 */
template<class Number = DefaultNumberType> class MetaMatVector : public MatVector<Number> {
public:

    MetaMatVector() : MatVector<Number>() {
    };

    explicit MetaMatVector(const unsigned int size) : MatVector<Number>(size) {
    };

    ~MetaMatVector() {
#ifdef MatVectorDebugOn0
        using namespace std;
        if (MatVector<Number>::vect != NULL)
            errMsg("MetaMatVector->vect nao ee nulo");
        else
            errMsg("MetaMatVector->vect ee nulo");
#endif
    };
};

typedef MatVector<double> MatVectorDP;
typedef MatVector<float> MatVectorSP;

/**
 * Desaloca a mem�ria de "vect"
 * @author Daniel F.Gomes
 * @since  05-02-2008
 */
template<class Number> inline void MatVector<Number>::clear() {
#ifdef MatVectorDebugOn1
    deallocs++;
#endif 
    delete[] (vect);
    vect = NULL;
    nrows = 0;
}

/**
 * Aloca a mem�ria para "vect"
 * @author Daniel F.Gomes
 * @since 05-02-2008
 */
template<class Number> inline void MatVector<Number>::size(const unsigned int &vsize) {
    clear();
    if (vsize != 0) {
        nrows = vsize;
        vect = new Number[nrows];
#ifdef MatVectorDebugOn1
        allocs++;
#endif
    } else
        errMsg(INVALID_DIMENSION_SIZE);
}

/**
 * return Tamanho do vetor
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> inline unsigned int MatVector<Number>::size() const {
    return nrows;
}

template<class Number> inline Number* MatVector<Number>::getVect(void) {
    return vect;
}

/**
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> inline MatVector<Number>::MatVector() {
    vect = NULL;
    nrows = 0;
}

/**
 * Cria um vetor de tamanho s
 * @param s comprimento do vetor
 *
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> MatVector<Number>::MatVector(const unsigned int vsize) {
    nrows = vsize;
    vect = new Number[nrows];
#ifdef MatVectorDebugOn1
    allocs++;
#endif
}

/**
 * Cria um vetor a partir de uma cópia de v com tamanho igual a size
 * @param &v array de Number com tamanho size
 * @param size comprimento do vetor
 *
 * @author Daniel F.Gomes
 * @since  05-02-2008
 */
template<class Number> MatVector<Number>::MatVector(const Number &x, const unsigned int xsize) {
    register int i;
    nrows = xsize;
    vect = new Number[nrows];

    for (i = 0; i < nrows; i++)
        vect[i] = x;
#ifdef MatVectorDebugOn1
    allocs++;
#endif

}

/**
 * Cria um vetor a partir de uma cópia de v com tamanho igual a size
 * @param *v array de Number com tamanho size
 * @param size comprimento do vetor
 *
 * @author Daniel F.Gomes
 * @since  05-02-2008
 */
template<class Number> MatVector<Number>::MatVector(const Number *v, const unsigned int vsize) {
    register unsigned int i;
    nrows = vsize;
    vect = new Number[nrows];

    for (i = 0; i < nrows; i++)
        vect[i] = v[i];
#ifdef MatVectorDebugOn1
    allocs++;
#endif

}

template<class Number> MatVector<Number>::MatVector(const MetaMatVector<Number> &v) {
    nrows = v.nrows;
    vect = v.vect;
    v.vect = 0;
    v.nrows = 0;

#ifdef MatVectorDebugOn1
    //allocs++; //Não faz nenhuma alocação neste caso.
#endif
}

template<class Number> MatVector<Number>::MatVector(MetaMatVector<Number>& v) {
    nrows = v.nrows;
    vect = v.vect;
    v.vect = 0;
    v.nrows = 0;

#ifdef MatVectorDebugOn1
    //allocs++; //Não faz nenhuma alocação neste caso.
#endif
}

/**
 * Cria um vetor a partir de uma cópia de v
 * @param &v vetor a ser utilizado como base para o vetor atual
 *
 * @author Daniel F.Gomes
 * @since  05-02-2008
 */
template<class Number> MatVector<Number>::MatVector(const MatVector &v) {
    register unsigned int i;
    nrows = v.size();
    vect = new Number[nrows];

    for (i = 0; i < nrows; i++)
        vect[i] = v[i];
#ifdef MatVectorDebugOn1
    allocs++;
#endif

    //std::cout << typeid(v.).name() << "\n";
}

/**
 * Cria um vetor a partir de uma cópia de v
 * @param &v vetor a ser utilizado como base para o vetor atual
 *
 * @author Daniel F.Gomes
 * @since  03-03-2009
 */
template<class Number> MatVector<Number>::MatVector(MatVector &v) {
    register unsigned int i;
    nrows = v.size();
    vect = new Number[nrows];

    for (i = 0; i < nrows; i++)
        vect[i] = v[i];
#ifdef MatVectorDebugOn1
    allocs++;
#endif
}

/**
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> MatVector<Number>::~MatVector() {
    clear();
}

template<class Number> inline Number & MatVector<Number>::operator[](const int i) {
    return vect[i];
}

template<class Number> inline const Number & MatVector<Number>::operator[](const int i) const {
    return vect[i];
}

template<class Number> inline Number & MatVector<Number>::at(const int i) {
    Number x = 0;
    return i >= 0 ? (i < nrows ? vect[i] : x) : x;
}

template<class Number> inline const Number & MatVector<Number>::at(const int i) const {
    return i >= 0 ? (i < nrows ? vect[i] : 0) : 0;
}

template<class Number> MatVector<Number>& MatVector<Number>::operator=(const MetaMatVector<Number> &vmeta) {
#ifdef MatVectorDebugOn0
    using namespace std;
    errMsg("Transferindo memória por cópia de ponteiros->operator=(const MetaMatVector<Number> &vmeta)");
#endif
    nrows = vmeta.nrows;
    vect = vmeta.vect;
    vmeta.vect = NULL;

    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::operator=(const MatVector<Number> &v) {
    if ((void*) this != (void*) &v) {
        if (nrows != v.size()) {
            size(v.size());
        }
        register unsigned int i;
        for (i = 0; i < nrows; i++)
            vect[i] = v[i];
    }
    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::operator=(const Number &k) {
    if (size() == 0) {
        size(1);
    }

    register unsigned int i;
    for (i = 0; i < nrows; i++)
        vect[i] = k;

    return *this;
}

/**
 * Redimensiona um vetor para o tamanha setado por s
 * @param s novo tamanho do vetor atual
 *
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> MatVector<Number>& MatVector<Number>::resize(const unsigned int s) {
    register int i;
    int minSize;
    Number *vectTemp = new Number[s];

    minSize = MatUtils::min(s, nrows);

    for (i = 0; i < minSize; i++)
        vectTemp[i] = vect[i];

    clear();

    nrows = s;
    vect = vectTemp;

    return *this;
}

/**
 * Redimensiona um vetor para o tamanha setado por s
 * @param s novo tamanho do vetor atual
 *
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> MatVector<Number>& MatVector<Number>::resize(const unsigned int s, const Number &fillN) {
    register int i;
    int minSize;
    Number *vectTemp = new Number[s];

    minSize = MatUtils::min(s, nrows);

    for (i = 0; i < minSize; i++)
        vectTemp[i] = vect[i];

    for (i = minSize; i < s; i++) {
        vectTemp[i] = fillN;
    }

    clear();

    nrows = s;
    vect = vectTemp;

    return *this;
}

template<class Number> bool MatVector<Number>::existNan(void) {
    return matVExistNan(vect, nrows);
}

template<class Number> MatVector<Number>& MatVector<Number>::replaceInf(double x) {
    matVReplaceInf(vect, nrows, x);
    return *this;
};

/**
 * Concatena o vetor atual com o vetor v
 * @param *v vetor a ser concatenado com o vetor atual
 *
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> MatVector<Number>& MatVector<Number>::concat(const MatVector<Number> &v) {
    register unsigned int i;
    unsigned int oldSize;
    unsigned int vSize = v.size();

    if (vect != NULL) {
        oldSize = nrows;
        resize(nrows + vSize);

        for (i = 0; i < vSize; i++)
            vect[i + oldSize] = v[i];
    } else {
        nrows = vSize;
        vect = new Number[nrows];

        for (i = 0; i < nrows; i++)
            vect[i] = v[i];
    }

    return *this;
}

/**
 * Concatena o vetor atual com o vetor v
 * @param *v vetor a ser concatenado com o vetor atual
 *
 * @author Daniel F.Gomes
 * @since  24-03-2009
 */
template<class Number> MatVector<Number>& MatVector<Number>::concat(const MatVector<Number> &v1, const MatVector<Number> &v2) {
    register unsigned int i;
    unsigned int oldSize;
    uint vSize;

    if (vect != NULL) {
        oldSize = nrows;
        resize(nrows + v1.size() + v2.size());

        vSize = v1.size();
        for (i = 0; i < vSize; i++)
            vect[i + oldSize] = v1[i];

        oldSize += vSize;
        vSize = v2.size();
        for (i = 0; i < vSize; i++)
            vect[i + oldSize] = v2[i];
    } else {
        nrows = v1.size() + v2.size();
        vect = new Number[nrows];

        vSize = v1.size();
        for (i = 0; i < vSize; i++)
            vect[i] = v1[i];

        vSize = v2.size();
        oldSize = v1.size();
        for (i = 0; i < vSize; i++)
            vect[i + oldSize] = v2[i];
    }

    return *this;
}

/**
 * Concatena o vetor atual com escalar k
 * @param *k escalar a ser concatenado com o vetor atual
 *
 * @author Hansenclever Bassani
 * @since  14-04-2008
 */
template<class Number> MatVector<Number>& MatVector<Number>::append(Number k) {
    if (vect == NULL) {
        size(1);
        vect[0] = k;
    } else {
        unsigned int oldSize = nrows;
        resize(size() + 1);

        vect[oldSize] = k;
    }

    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::copy(const MatVector<Number> &v, int firsts) {
    if (v.size() != 0) {
        register int i;

        clear();
        if (firsts > 0) {
            nrows = MatUtils::min(firsts, v.size);
            vect = new Number[nrows];

            for (i = 0; i < nrows; i++)
                vect[i] = v[i];
        } else {
            int beginIndex = nrows - 1;
            int endIndex;

            firsts = -firsts;
            if (firsts > v.size()) {
                nrows = v.size();
                endIndex = 0;
            } else {
                endIndex = (nrows - firsts);
            }

            vect = new Number[nrows];

            for (i = beginIndex; i >= endIndex; i--)
                vect[i] = v[i];
        }

    } else
        errMsg(MEM_NO_ALLOC);

    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::copy(const MatVector<Number> &v, int beginIndex, int endIndex) {
    if (v.size() > 0) {
        if (beginIndex != endIndex) {
            register unsigned int i;
            register unsigned int ii;

            if (beginIndex < endIndex) {
                if (endIndex >= v.size())
                    endIndex = v.size() - 1;

                clear();
                nrows = endIndex - beginIndex + 1;
                vect = new Number[nrows];

                ii = 0;
                for (i = beginIndex; i <= endIndex; i++)
                    vect[ii++] = v[i];
            } else
                if (endIndex < v.size()) {
                if (beginIndex >= v.size())
                    beginIndex = v.size() - 1;

                ii = 0;
                for (i = beginIndex; i >= endIndex; i--)
                    vect[ii++] = v[i];
            }
        }
    }
    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::flip() {
    if (nrows != 0) {
        int leftIndex = 0;
        int rightIndex = nrows - 1;
        Number temp;

        while ((rightIndex - leftIndex) >= 1) {
            temp = vect[leftIndex];
            vect[leftIndex] = vect[rightIndex];
            vect[rightIndex] = temp;
            leftIndex++;
            rightIndex--;
        }
    }
    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::shift(int d, Number vfill) {
    matShift(vect, nrows, d, vfill);
    return *this;
}

template<class Number> MatVector<Number>* MatVector<Number>::clone() {
    MatVector<Number> *v = new MatVector<Number>((*this));

    return v;
}

template<class Number> MatVector<Number>& MatVector<Number>::swap(MatVector<Number> &v) {
    Number *tempVect = v.vect;
    unsigned int tempVectSize = v.size();

    v.vect = vect;
    v.nrows = nrows;

    vect = tempVect;
    nrows = tempVectSize;

    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::range(const Number &start, const Number &stop, Number step) {
    register unsigned int i;
    clear();

    if (start > stop) {
        if (step > 0)
            step = -step;

        nrows = (int) ((start - stop) / MatUtils::abs(step) + 1);
    } else {
        nrows = (int) ((stop - start) / step + 1);
    }

    vect = new Number[nrows];

    for (i = 0; i < nrows; i++) {
        vect[i] = start + i*step;
    }

    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::range(const Number &start, const Number &stop) {
    // TODO: alocar memória se a mesma não tiver sido alocada ainda
    if (nrows != 0) {
        register unsigned int i;
        Number step = ((stop - start) / (nrows - 1));

        for (i = 0; i < nrows; i++) {
            vect[i] = start + i*step;
        }
    } else {
        register unsigned int i;
        int step = 1;
        clear();

        if (start > stop) {
            if (step > 0)
                step = -step;

            nrows = (int) ((start - stop) / abs(step) + 1);
        } else {
            nrows = (int) ((stop - start) / step + 1);
        }

        vect = new Number[nrows];

        for (i = 0; i < nrows; i++) {
            vect[i] = start + i*step;
        }

    }

    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::fill(const Number x) {
    if (vect != NULL) {
        matVFill<Number>(vect, size(), x);
    } else
        errMsg(MEM_NO_ALLOC);

    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::fill(const Number x, const unsigned int vsize) {
    size(vsize);

    matVFill<Number>(vect, size(), x);

    return *this;
}

/**
 * Preenche com zeros o vetor
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> inline MatVector<Number>& MatVector<Number>::zeros(void) {
    fill(0);

    return *this;
}

template<class Number> inline MatVector<Number>& MatVector<Number>::zeros(const unsigned int newSize) {
    fill(0, newSize);

    return *this;
}

/**
 * Preenche com "uns" o vetor
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> inline MatVector<Number>& MatVector<Number>::ones(void) {
    fill(1);

    return *this;
}

template<class Number> inline MatVector<Number>& MatVector<Number>::ones(const unsigned int newSize) {
    fill(1, newSize);

    return *this;
}

/**
 * Semente de n�meros aleat�rios
 * @author Daniel F.Gomes
 * @since 05-02-2008
 */
template<class Number> MatVector<Number>& MatVector<Number>::srandom(const long s) {
    srand(s);

    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::random(const int s) {
    size(s);

    matVFillUnoise<Number>(vect, nrows);

    return *this;
}

/**
 * Preenche o vetor com números aleatórios com distribuição uniforme de 0 a 1
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> MatVector<Number>& MatVector<Number>::random(void) {
    if (size() != 0)
        matVFillUnoise<Number > (vect, nrows);

    return *this;
}

/**
 * Preenche o vetor com números aleatórios com distribuição normal de média m e
 * desvio dev
 * @author Daniel F.Gomes
 * @since  22-03-2009
 */
template<class Number> MatVector<Number>& MatVector<Number>::randomn(const Number& m, const Number& dev) {
    if (size() != 0)
        matVFillNNoise(vect, nrows, m, dev);
    return *this;
};

/**
 * Preenche o vetor com números aleatórios com distribuição normal com vetor de médias vm e
 * desvio dev
 * @author Daniel F.Gomes
 * @since  22-03-2009
 */
template<class Number> MatVector<Number>& MatVector<Number>::randomn(const MatVector<Number> &vm, const Number& dev) {
    if (size() != vm.size())
        size(vm.size());
    matVFillNNoise(vect, nrows, vm.vect, dev);
    return *this;
};

/**
 * Preenche o vetor com números aleatórios com distribuição normal com vetor de média vm e vetor de
 * desvio vdev
 * @author Daniel F.Gomes
 * @since  22-03-2009
 */
template<class Number> MatVector<Number>& MatVector<Number>::randomn(const MatVector<Number> &vm, const MatVector<Number>& vdev) {
    if (vm.size() == vdev.size()) {
        if (size() != vm.size())
            size(vm.size());

        matVFillNNoise(vect, nrows, vm.vect, vdev.vect);
    } else
        errMsg(INVALID_DIMENSION_SIZE);

    return *this;
};

template<class Number> MatVector<Number>& MatVector<Number>::shuffler(void) {
    matVShuffler(vect, nrows);

    return *this;
}

/**
 * Retorna a média dos valores contidos no vetor
 * return valor médio do vetor
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> Number MatVector<Number>::mean() {
    return matVMean<Number>(vect, nrows);
};

/**
 * Retorna a variancia dos valores contidos no vetor
 * return variancia do vetor
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> Number MatVector<Number>::var() {
    return matVVar<Number>(vect, nrows);
}

/**
 * Retorna a covariancia do vetor com v
 * OBS: AINDA NÃO IMPLEMENTADO
 * @param *v vetor de comparação
 * return covariancia
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> Number MatVector<Number>::cov(const MatVector &v) {
    if (v.size() == nrows) {
        Number xm = 0, ym = 0, ac = 0;
        register unsigned int i;

        for (i = 0; i < nrows; i++) {
            xm += vect[i];
            ym += v[i];
        }
        xm = xm / nrows;
        ym = ym / nrows;

        for (i = 0; i < nrows; i++)
            ac += (vect[i] - xm)*(v[i] - ym);

        return ac / (nrows - 1);
    }
    errMsg("NaN");
    return MatUtils::NaN;
}

/**
 * Retorna a correlação do vetor com v
 * @param *v vetor de comparação
 * return correlação
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> Number MatVector<Number>::corr(const MatVector &v) {
    if (v.size() == nrows) {
        Number xm = 0, ym = 0, acXY = 0, acX = 0, acY = 0, tempX = 0, tempY = 0;
        register unsigned int i;

        for (i = 0; i < nrows; i++) {
            xm += vect[i];
            ym += v[i];
        }
        xm = xm / nrows;
        ym = ym / nrows;

        for (i = 0; i < nrows; i++) {
            tempX = (vect[i] - xm);
            tempY = (v[i] - ym);

            acXY += tempX*tempY;
            acX += tempX*tempX;
            acY += tempY*tempY;
        }

        return acXY / (sqrt(acX) * sqrt(acY));
    }
    return -1;
}

/**
 * Retorna o desvio padrão do vetor
 * return desvio padrão
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> inline Number MatVector<Number>::stdDev() {
    return sqrt(var());
}

/**
 * Retorna valor máximo
 * return valor máximo do vetor
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> Number MatVector<Number>::max() const {
    return matVMax<Number>(vect, nrows);
}

/**
 * Retorna valor m�ximo
 * return valor m�ximo do vetor e a posis�o deste
 * @author Daniel F.Gomes
 * @since  17-08-2008
 */
template<class Number> Number MatVector<Number>::max(uint &index) const {
    return matVMaxIndex<Number>(vect, index, nrows);
}

/**
 * Retorna valor m�ximo
 * return valor m�ximo do vetor e a posis�o deste
 * @author Daniel F.Gomes
 * @since  01-03-2009
 */
template<class Number> template<class T> MatVector<Number>& MatVector<Number>::max(const MatVector<T> &v) {
    matVMax(vect, v.vect, MatUtils::max(nrows, v.size()));

    return *this;
}

/**
 * Retorna o valor m�nimo do vetor
 * return valor mínimo
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> Number MatVector<Number>::min() const {
    return matVMin<Number>(vect, nrows);
}

/**
 * Retorna o valor m�nimo do vetor
 * return valor m�nimo e a posi��o deste
 * @author Daniel F.Gomes
 * @since  17-08-2008
 */
template<class Number> Number MatVector<Number>::min(uint &index) const {
    return matVMinIndex<Number>(vect, index, nrows);
}

/**
 * Retorna o valor m�nimo do vetor
 * return valor m�nimo e a posi��o deste
 * @author Daniel F.Gomes
 * @since  01-03-2009
 */
template<class Number> template<class T>MatVector<Number>& MatVector<Number>::min(const MatVector<T> &v) {
    matVMin(vect, v.vect, MatUtils::min(nrows, v.size()));

    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::minMax(Number &min, Number &max) {
    matVMinMax(vect, nrows, min, max);
    return *this;
}

/**
 * Retorna o produto interno entre o vetor atual e v
 * @param *v vetor de comparação
 * return  produto interno entre o vetor atual e v
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> Number MatVector<Number>::dot(const MatVector &v) {
    if (v.size() == nrows) {
        return matVDot<Number>(vect, v.vect, nrows);
    } else
        errMsg(INVALID_DIMENSION_SIZE);

    return 0;
}

/**
 * Retorna o produto externo entre o vetor atual e v
 * Só é válido para vetores 3D
 * @param *v vetor de comparação
 * return produto externo entre o vetor atual e v
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> MetaMatVector<Number> MatVector<Number>::cross(const MatVector &v) {
    MetaMatVector<Number> vtemp(3);

    if ((size() == 3)&&(v.size())) {
        matVCross<Number>(vect, v.vect, vtemp.vect);
    } else
        errMsg(INVALID_DIMENSION_SIZE);

    return vtemp;
}

/**
 * Retorna a distância euclidiana ao quadrado entre o vetor atual
 * e v
 * @param *v vetor de comparação
 * return distância euclidiana ao quadrado
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> Number MatVector<Number>::dist2(const MatVector &v) const {
    if (v.size() == nrows) {
        return matVDist2<Number>(vect, v.vect, nrows);
    } else
        errMsg(INVALID_DIMENSION_SIZE);

    return 0;
}

/**
 * Retorna a distância euclidiana entre o vetor atual e v
 * @param *v vetor de comparação
 * return distância euclidiana
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> Number MatVector<Number>::dist(const MatVector &v) const {
    return sqrt(dist2(v));
}

/**
 * Retorna a norma do vetor atual
 * return norma do vetor
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> Number MatVector<Number>::norm2() const {
    if (nrows != 0) {
        return matVDot<Number>(vect, vect, nrows);
    }

    return 0;
}

template<class Number> inline Number MatVector<Number>::norm() const {
    return sqrt(norm2());
}

template<class Number> MatVector<Number>& MatVector<Number>::rescale01() {
    matVRescale01<Number>(vect, size());
    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::vpow(const Number e) {
    register unsigned int i;

    for (i = 0; i < nrows; i++)
        vect[i] = pow(vect[i], e);

    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::vsqrt(void) {
    register unsigned int i;

    for (i = 0; i < nrows; i++)
        vect[i] = sqrt(vect[i]);

    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::vqrt(void) {
    register unsigned int i;

    for (i = 0; i < nrows; i++)
        vect[i] = (vect[i] * vect[i]);

    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::vlog(void) {
    matVlog(vect, nrows);
    return *this;
};

template<class Number> MatVector<Number>& MatVector<Number>::vabs(void) {
    matVabs(vect, nrows);
    return *this;
};

template<class Number> Number MatVector<Number>::polival(const Number x) {
    register unsigned int i;
    Number poli = vect[0];

    for (i = 1; i < nrows; i++)
        poli += vect[i] * pow(x, i);

    return poli;
}

template<class Number> MatVector<Number>& MatVector<Number>::sort(void) {
    matVQsort<Number>(vect, nrows);
    return *this;
}

template<class Number> Number MatVector<Number>::sortAndCalcMedian() {
    matVQsort<Number>(vect, nrows);
    return vect[size() / 2];
}

template<class Number> Number MatVector<Number>::median() {
    MatVector<Number> temp(*this);
    return temp.sortAndCalcMedian();
}

template<class Number> Number MatVector<Number>::sum() const {
    register unsigned int i;
    Number vsum = 0;

    for (i = 0; i < nrows; i++) {
        vsum += vect[i];
    }

    return vsum;
}

template<class Number> Number MatVector<Number>::prod() {
    register unsigned int i;
    Number vsum = 1;

    for (i = 0; i < nrows; i++) {
        vsum *= vect[i];
    }

    return vsum;
}

template<class Number> MetaMatVector<Number> MatVector<Number>::sortAndCalcOrderStatistics() {
    // vmeta[0] - Mínimo
    // vmeta[1] - 1 quartil
    // vmeta[2] - 2 quartil ou mediana
    // vmeta[3] - 3 quartil
    // vmeta[4] - Máximo
    MetaMatVector<Number> vmeta(5);
    matVSortAndGetOrderStatistics<Number>(vmeta.vect[0], vmeta.vect[1], vmeta.vect[2], vmeta.vect[3], vmeta.vect[4], vect, nrows);
    return vmeta;
}

template<class Number> MetaMatVector<Number> MatVector<Number>::orderStatistics() {
    // vmeta[0] - Mínimo
    // vmeta[1] - 1 quartil
    // vmeta[2] - 2 quartil ou mediana
    // vmeta[3] - 3 quartil
    // vmeta[4] - Máximo

    MetaMatVector<Number> vmeta(5);

    Number *vectTemp = new Number[nrows];

    matVCopy<Number*>(vectTemp, vect, nrows);
    matVSortAndGetOrderStatistics<Number*, Number>(vmeta[0], vmeta[1], vmeta[2], vmeta[3], vmeta[4], vect, nrows);

    delete[] vectTemp;
    return vmeta;
}

template<class Number> MetaMatVector<Number> MatVector<Number>::statistics() {
    // vmeta[0] - Maximo
    // vmeta[1] - Mínimo
    // vmeta[2] - Somatório
    // vmeta[3] - Média
    // vmeta[4] - variancia

    MetaMatVector<Number> vmeta(5);

    matVStatistics<Number>(vmeta[0], vmeta[1], vmeta[2], vmeta[3], vmeta[4], vect, nrows);

    return vmeta;
}

template<class Number> template<class T> MatVector<Number>& MatVector<Number>::mult(const MatVector<T> &v) {
    register unsigned int i;

    if (v.size() == nrows) {
        for (i = 0; i < nrows; i++)
            vect[i] = vect[i] * v[i];
    } else
        if (size() == 0)
        zeros(v.size());
    else
        errMsg(INVALID_DIMENSION_SIZE);

    return *this;
}

template<class Number> template<class T> MatVector<Number>& MatVector<Number>::mult(const T k) {
    register unsigned int i;

    for (i = 0; i < nrows; i++)
        vect[i] = vect[i] * k;

    return *this;
}

template<class Number> template<class T> MatVector<Number>& MatVector<Number>::div(const MatVector<T> &v) {
    register unsigned int i;
    if (v.size() == nrows) {
        for (i = 0; i < nrows; i++)
            vect[i] = vect[i] / v[i];
    } else
        if (size() == 0)
        zeros(v.size());
    else
        errMsg(INVALID_DIMENSION_SIZE);

    return *this;
}

template<class Number> template<class T> MatVector<Number>& MatVector<Number>::div(const T k) {
    if (k != 0) {
        register unsigned int i;
        for (i = 0; i < nrows; i++)
            vect[i] = vect[i] / k;
    } else
        errMsg(DIVIDE_BY_ZERO);

    return *this;
}

template<class Number> template<class T> MatVector<Number>& MatVector<Number>::add(const T k) {
    register unsigned int i;

    for (i = 0; i < nrows; i++)
        vect[i] = vect[i] + k;

    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::inv(void) {
    register unsigned int i;

    for (i = 0; i < nrows; i++)
        vect[i] = 1 / vect[i];

    return *this;
}

template<class Number> template<class T> MatVector<Number>& MatVector<Number>::add(const MatVector<T> &v) {
    register unsigned int i;
    unsigned int minSize;

    if (size() == 0) {
        resize(v.size());
        minSize = v.size();
    } else
        minSize = MatUtils::min(size(), v.size());

    for (i = 0; i < minSize; i++)
        vect[i] = vect[i] + v[i];

    return *this;
}

template<class Number> template<class T> MatVector<Number>& MatVector<Number>::sub(const MatVector<T> &v) {
    register unsigned int i;
    unsigned int minSize;

    if (size() == 0) {
        resize(v.size());
        minSize = v.size();
    } else
        minSize = MatUtils::min(size(), v.size());

    for (i = 0; i < minSize; i++)
        vect[i] = vect[i] - v[i];

    return *this;
}

template<class Number> template<class T> MatVector<Number>& MatVector<Number>::sub(const T k) {
    register unsigned int i;

    for (i = 0; i < nrows; i++)
        vect[i] = vect[i] - k;

    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::neg(void) {
    register unsigned int i;

    for (i = 0; i < nrows; i++)
        vect[i] = -vect[i];

    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::normalize() {
    //matRescale01
    matVNormalize<Number>(vect, size());
    return *this;
}

template<class Number> inline MatVector<Number>& MatVector<Number>::normalize2One() {
    div(max());

    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::diff(const Number &k) {
    matDiff(vect, k, nrows);

    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::apply(float f(float)) {
    if (nrows != 0) {
        register unsigned int i;
        for (i = 0; i < nrows; i++)
            vect[i] = f(vect[i]);
    }
    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::apply(float f(const float&)) {
    if (nrows != 0) {
        register unsigned int i;
        for (i = 0; i < nrows; i++)
            vect[i] = f(vect[i]);
    }
    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::apply(double f(double)) {
    if (nrows != 0) {
        register unsigned int i;
        for (i = 0; i < nrows; i++)
            vect[i] = f(vect[i]);
    }
    return *this;
}

template<class Number> MatVector<Number>& MatVector<Number>::apply(double f(const double&)) {
    if (nrows != 0) {
        register unsigned int i;
        for (i = 0; i < nrows; i++)
            vect[i] = f(vect[i]);
    }
    return *this;
}

template<class Number> inline bool MatVector<Number>::isEmpty(void) {
    if (vect != NULL)
        return false;
    else
        return true;
}

template<class Number> bool MatVector<Number>::isEqual(const MatVector &v) {
    if (v.size() == nrows) {
        for (register unsigned int i = 0; i < nrows; i++)
            if (vect[i] != v[i])
                return false;
    } else
        return false;

    return true;
}

template<class Number> inline bool MatVector<Number>::operator==(const MatVector<Number> &v) {
    return isEqual(v);
}

template<class Number> inline bool MatVector<Number>::operator!=(const MatVector<Number> &v) {
    return !isEqual(v);
}

template<class Number> bool MatVector<Number>::operator>(MatVector<Number> &v) {
    if (v.size() != nrows)
        errMsg(INVALID_DIMENSION_SIZE);

    if (norm() > v.norm())
        return true;

    return false;

}

template<class Number> bool MatVector<Number>::operator>=(MatVector<Number> &v) {
    if (v.size() != nrows)
        errMsg(INVALID_DIMENSION_SIZE);

    if (norm() >= v.norm())
        return true;

    return false;

}

template<class Number> bool MatVector<Number>::operator<(MatVector<Number> &v) {
    if (v.size() != nrows)
        errMsg(INVALID_DIMENSION_SIZE);

    if (norm() < v.norm())
        return true;

    return false;
}

template<class Number> bool MatVector<Number>::operator<=(MatVector<Number> &v) {
    if (v.size() != nrows)
        errMsg(INVALID_DIMENSION_SIZE);

    if (norm() <= v.norm())
        return true;

    return false;
}

template<class Number> template<class T> inline MatVector<Number>& MatVector<Number>::operator+=(const MatVector<T> &v) {
    add(v);

    return *this;
}

template<class Number> template<class T> inline MatVector<Number>& MatVector<Number>::operator+=(const T &k) {
    add(k);

    return *this;
}

template<class Number> inline MatVector<Number>& MatVector<Number>::operator++() {
    add(1);

    return *this;
}

template<class Number> template<class T> inline MatVector<Number>& MatVector<Number>::operator-=(const MatVector<T> &v) {
    sub(v);

    return *this;
}

template<class Number> template<class T> inline MatVector<Number>& MatVector<Number>::operator-=(const T &k) {
    sub(k);

    return *this;
}

template<class Number> inline MatVector<Number>& MatVector<Number>::operator--() {
    sub(1);

    return *this;
}

template<class Number> template<class T> inline MatVector<Number>& MatVector<Number>::operator*=(const MatVector<T> &v) {
    mult(v);

    return *this;
}

template<class Number> template<class T> inline MatVector<Number>& MatVector<Number>::operator*=(const T &k) {
    mult(k);

    return *this;
}

/**
 * Transformo o vetor atual em uma string
 * @author Daniel F.Gomes
 * @since  24-10-2007
 */
template<class Number> std::string MatVector<Number>::toString() const {
    using namespace std;
    register unsigned int i;
    string outputS = "";

    if (vect != NULL) {
        char strbuff[100];
        outputS += "[ ";
        for (i = 0; i < nrows; i++) {
            sprintf(strbuff, "%f", (double) vect[i]);
            outputS += strbuff;
            outputS += " ";
        }


        outputS += "]";
    } else
        outputS += "[NULL]";

    return outputS;
}

/**
 * Transformo o vetor atual em uma linha de arquivo CSV
 * @author Hansenclever Bassani
 * @since  24-10-2007
 */
template<class Number> std::string MatVector<Number>::toCSV() const {
    using namespace std;
    register unsigned int i;
    string outputS = "";

    if (vect != NULL) {
        char strbuff[100];
        for (i = 0; i < nrows; i++) {
            sprintf(strbuff, "%f", (double) vect[i]);
            outputS += strbuff;
            outputS += "\t";
        }
    } else
        outputS += "[NULL]";

    return outputS;
}

template<class Number> bool MatVector<Number>::save2Txt(std::ofstream outfile) {
    using namespace std;
    if (outfile.is_open()) {
        outfile << "# 1 " << nrows << endl;

        for (register unsigned int i = 0; i < nrows; i++)
            outfile << vect[i] << " ";

        outfile << endl;
    }

    return outfile.is_open();
}

template<class Number> bool MatVector<Number>::save2Txt(std::string fileName) {
    using namespace std;
    ofstream outfile;
    outfile.open(fileName.c_str());

    if (save2Txt(outfile)) {
        outfile.close();
        return true;
    }

    return false;
}

template<class Number> bool MatVector<Number>::save2Sce(const std::string &varName, std::ofstream &outfile) {
    using namespace std;
    if (outfile.is_open()) {
        outfile << varName << "=[";

        for (register unsigned int i = 0; i < nrows; i++)
            outfile << vect[i] << " ";

        outfile << "];" << endl;
    }

    return outfile.is_open();
}

template<class Number> bool MatVector<Number>::save2Sce(const std::string &varName, const std::string &fileName) {
    using namespace std;

    std::ofstream outfile;
    outfile.open(fileName.c_str(), ios_base::app);
    bool flag = save2Sce(varName, outfile);
    if (flag) {
        outfile.close();
        return true;
    }

    return false;
}

template<class Number> inline MatVector<Number> operator+(const MatVector<Number> &v1, const MatVector<Number> &v2) {
#ifndef NO_INVALID_DIMENSION_SIZE
    if (v1.size() != v2.size())
        errMsg(INVALID_DIMENSION_SIZE);
#endif
    unsigned int v1Size = MatUtils::min(v1.size(), v2.size());


    MatVector<Number> vret(v1Size);
    register unsigned int i;

    for (i = 0; i < v1Size; i++)
        vret[i] = v1[i] + v2[i];

    return vret;

}

template<class Number, class T> inline MatVector<Number> operator+(const MatVector<Number> &v1, const T &k) {
    int v1Size = v1.size();

    MatVector<Number> vret(v1Size);
    register unsigned int i;

    for (i = 0; i < v1Size; i++)
        vret[i] = v1[i] + k;

    return vret;
}

template<class Number, class T> inline MatVector<Number> operator+(const T &k, const MatVector<Number> &v1) {
    int v1Size = v1.size();

    MatVector<Number> vret(v1Size);
    register unsigned int i;

    for (i = 0; i < v1Size; i++)
        vret[i] = v1[i] + k;

    return vret;
}

template<class Number> inline MatVector<Number> operator-(const MatVector<Number> &v1, const MatVector<Number> &v2) {

#ifndef NO_INVALID_DIMENSION_SIZE
    if (v1.size() != v2.size())
        errMsg(INVALID_DIMENSION_SIZE);
#endif
    unsigned int v1Size = MatUtils::min(v1.size(), v2.size());

    MatVector<Number> vret(v1Size);
    register unsigned int i;

    for (i = 0; i < v1Size; i++)
        vret[i] = v1[i] - v2[i];

    return vret;
}

template<class Number, class T> inline MatVector<Number> operator-(const MatVector<Number> &v1, const T &k) {
    int v1Size = v1.size();

    MatVector<Number> vret(v1Size);
    register unsigned int i;

    for (i = 0; i < v1Size; i++)
        vret[i] = v1[i] - k;

    return vret;
}

template<class Number> inline Number operator*(const MatVector<Number> &v1, const MatVector<Number> &v2) {
#ifndef NO_INVALID_DIMENSION_SIZE
    if (v1.size() != v2.size())
        errMsg(INVALID_DIMENSION_SIZE);
#endif
    unsigned int v1Size = MatUtils::min(v1.size(), v2.size());


    Number ac = 0;
    register unsigned int i;

    for (i = 0; i < v1Size; i++)
        ac += v1[i] * v2[i];

    return ac;

}

template<class Number, class T> inline MatVector<Number> operator*(const MatVector<Number> &v1, const T &k) {
    int v1Size = v1.size();

    MatVector<Number> vret(v1Size);
    register unsigned int i;

    for (i = 0; i < v1Size; i++)
        vret[i] = v1[i] * k;

    return vret;
}

template<class Number, class T> inline MatVector<Number> operator*(const T &k, const MatVector<Number> &v1) {
    uint v1Size = v1.size();

    MatVector<Number> vret(v1Size);
    register unsigned int i;

    for (i = 0; i < v1Size; i++)
        vret[i] = v1[i] * k;

    return vret;
}

template<class Number> inline MatVector<Number> operator/(const MatVector<Number> &v1, const MatVector<Number> &v2) {
#ifndef NO_INVALID_DIMENSION_SIZE
    if (v1.size() != v2.size())
        errMsg(INVALID_DIMENSION_SIZE);
#endif
    unsigned int v1Size = MatUtils::min(v1.size(), v2.size());

    MatVector<Number> vret(v1Size);
    register unsigned int i;

    for (i = 0; i < v1Size; i++)
        vret[i] = v1[i] / v2[i];

    return vret;

}

template<class Number, class T> inline MatVector<Number> operator/(const MatVector<Number> &v1, const T &k) {
    int v1Size = v1.size();

    MatVector<Number> vret(v1Size);
    register unsigned int i;

    for (i = 0; i < v1Size; i++)
        vret[i] = v1[i] / k;

    return vret;
}

template<class Number> inline MatVector<Number> sqrt(const MatVector<Number> &v) {
    MatVector<Number> vret(v);
    matVsqrt(vret, vret.size());
    return vret;
};

template<class Number, class T> inline MatVector<Number> pow(const MatVector<Number> &v, const T &e) {
    MatVector<Number> vret(v);
    matVpow(vret, vret.size(), e);
    return vret;
};

template<class Number> inline MatVector<Number> log(const MatVector<Number> &v) {
    MatVector<Number> vret(v);
    matVlog(vret, vret.size());
    return vret;
};

std::ostream& operator<<(std::ostream& out, const MatVector<int> &v);
std::ostream& operator<<(std::ostream& out, const MatVector<float> &v);
std::ostream& operator<<(std::ostream& out, const MatVector<double> &v);



#endif /*MATVECTOR_H_*/