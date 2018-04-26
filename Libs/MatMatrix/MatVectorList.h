#ifndef MATVECTORLIST_H_
#define MATVECTORLIST_H_

#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include "MatVector.h"

//using namespace std;

template<class Number=DefaultNumberType> class MatVectorList
{
	private:
		typename std::list<MatVector<Number>* >::iterator iterator;
	public:
		std::list<MatVector<Number>*> lstMatVector;

		MatVectorList();
		~MatVectorList();
		MatVectorList& removeFirst(const MatVector<Number> *v);
		MatVectorList& removeAll(const MatVector<Number> *v);
		MatVectorList& insert(const MatVector<Number> *v);
		MatVectorList& push_back(const MatVector<Number> *v);
		MatVectorList& push_front(const MatVector<Number> *v);
		unsigned int size(void);
		MatVectorList& clearAllRef(void);
		MatVectorList& begin(void);
		MatVectorList& prev(void);
		MatVectorList& next(void);
		MatVectorList& end(void);
		bool at(const unsigned int index);
		bool empty(void);
		bool isBegin(void);
		bool isEnd(void);
		MatVector<Number>* item(void);
		MatVectorList& zeroMatVectorList(unsigned int s, unsigned int sv);
		MatVectorList& randomMatVectorList(unsigned int s, unsigned int sv);
		MatVector<Number>* mean(void);
		MatVector<Number>* var(void);
		MatVector<Number>* stddev(void);
		std::vector<MatVector<Number>*> vectorList();
		MatVectorList<Number>* clone();
		bool isEqual(const MatVectorList<Number> *lst);
		MatVectorList& save2Txt(std::string name);
		MatVectorList& loadTxt(std::string name);
		std::string toString();
};


template<class Number> MatVectorList<Number>::MatVectorList()
{
	iterator = lstMatVector.begin();
}

template<class Number> MatVectorList<Number>& MatVectorList<Number>::removeFirst(const MatVector<Number> *v)
{
	using namespace std;
	typename list<MatVector<Number>*>::iterator it = this->lstMatVector.begin();
	typename list<MatVector<Number>*>::iterator itEnd = this->lstMatVector.end();
	while(it != itEnd)
	{
		if(*it == v)
		{
			this->lstMatVector.erase(it);
			break;
		}
		it++;
	}

	return *this;
}


template<class Number> MatVectorList<Number>& MatVectorList<Number>::removeAll(const MatVector<Number> *v)
{
	lstMatVector.remove(v);
	return *this;
}

template<class Number> MatVectorList<Number>& MatVectorList<Number>::insert(const MatVector<Number> *v)
{
	using namespace std;
	  list<int> mylist;
	  list<int>::iterator it;

	  // set some initial values:
	  for (int i=1; i<=5; i++) mylist.push_back(i); // 1 2 3 4 5

	  it = mylist.begin();
	  ++it;       // it points now to number 2           ^

	  mylist.insert (it,10);                        // 1 10 2 3 4 5

	  // "it" still points to number 2                      ^
	  mylist.insert (it,2,20);                      // 1 10 20 20 2 3 4 5

	  --it;       // it points now to the second 20            ^

	  vector<int> myvector (2,30);
	  mylist.insert (it,myvector.begin(),myvector.end());
	                                                // 1 10 20 30 30 20 2 3 4 5
	                                                //               ^
	  cout << "mylist contains:";
	  for (it=mylist.begin(); it!=mylist.end(); it++)
	    cout << " " << *it;
	  cout << endl;

	//lstMatVector.push_back(v);

	lstMatVector.insert(iterator,v);
	return *this;
}


template<class Number> MatVectorList<Number>& MatVectorList<Number>::push_back(const MatVector<Number> *v)
{
	lstMatVector.push_back(v);
	return *this;
}


template<class Number> unsigned int MatVectorList<Number>::size(void)
{
	return lstMatVector.size();
}

template<class Number> MatVectorList<Number>& MatVectorList<Number>::clearAllRef(void)
{
	using namespace std;
	typename list<MatVector<Number>*>::iterator it = this->lstMatVector.begin();
	typename list<MatVector<Number>*>::iterator itEnd = this->lstMatVector.end();
	while(it != itEnd)
	{
		delete (*it);
		++it;
	}

	lstMatVector.clear();
}

template<class Number> MatVectorList<Number>& MatVectorList<Number>::begin(void)
{
	iterator = lstMatVector.begin();
}

template<class Number> MatVectorList<Number>& MatVectorList<Number>::prev(void)
{
	--iterator;
}

template<class Number> MatVectorList<Number>& MatVectorList<Number>::next(void)
{
	++iterator;
}

template<class Number> MatVectorList<Number>& MatVectorList<Number>::end(void)
{
	iterator=lstMatVector.end();
}

template<class Number> bool MatVectorList<Number>::at(const unsigned int index)
{
	register unsigned int i;
	//for(i=0;)

	return true;
}

template<class Number> bool MatVectorList<Number>::empty()
{
	return lstMatVector.empty();
}


template<class Number> bool MatVectorList<Number>::isBegin(void)
{
	return iterator==lstMatVector.begin();
}

template<class Number>bool MatVectorList<Number>::isEnd(void)
{
	return iterator==lstMatVector.end();
}


template<class Number> MatVector<Number>* MatVectorList<Number>::item(void)
{
	return (*iterator);
}

template<class Number> MatVectorList<Number>& MatVectorList<Number>::zeroMatVectorList(unsigned int s, unsigned int sv)
{
	register unsigned int i;
	MatVector<Number> *vtemp;

	for(i=0;i<s;i++)
	{
		vtemp = new MatVector<Number>(sv);
		vtemp->zeros();

		lstMatVector.push_back(vtemp);
	}
}

template<class Number> MatVectorList<Number>& MatVectorList<Number>::randomMatVectorList(unsigned int s, unsigned int sv)
{
	register unsigned int i;
	MatVector<Number> *vtemp;

	for(i=0;i<s;i++)
	{
		vtemp = new MatVector<Number>(sv);
		vtemp->random();

		lstMatVector.push_back(vtemp);
	}
}

template<class Number> MatVector<Number>* MatVectorList<Number>::mean(void)
{
	using namespace std;
	typename list<MatVector<Number>*>::iterator it = lstMatVector.begin();
	typename list<MatVector<Number>*>::iterator itEnd = lstMatVector.end();

	MatVector<Number> *vtemp =new MatVector<Number>((*it)->size());
	vtemp->zeros();

	while(it!=itEnd)
	{
		vtemp->add((*it));
		++it;
	}

	vtemp->mult(1.0/lstMatVector.size());

	return vtemp;
}

template<class Number> MatVector<Number>* MatVectorList<Number>::var(void)
{
	using namespace std;
	typename list<MatVector<Number>* >::iterator it = lstMatVector.begin();
	typename list<MatVector<Number>* >::iterator itEnd = lstMatVector.end();

	MatVector<Number> *vm = mean();
	MatVector<Number> *ac =new MatVector<Number>((*it)->size());
	MatVector<Number> *vtemp;
	ac->zeros();

	while(it!=itEnd)
	{
		vtemp=(*it)->clone();
		vtemp->sub(vm);
		vtemp->vpow(2);
		ac->add(vtemp);
		delete vtemp;
		++it;
	}

	delete vm;
	ac->mult(1.0/(lstMatVector.size()-1));

	return ac;
}

template<class Number> MatVector<Number>* MatVectorList<Number>::stddev(void)
{
	MatVector<Number> *vtemp=var();
	vtemp->vsqrt();
	return vtemp;
}

template<class Number> std::vector<MatVector<Number>*> MatVectorList<Number>::vectorList()
{
	using namespace std;
	int i=0;
	typename list<MatVector<Number>* >::iterator it = lstMatVector.begin();
	typename list<MatVector<Number>* >::iterator itEnd = lstMatVector.end();
	vector<MatVector<Number>* > vlist(lstMatVector.size());

	while(it!=itEnd)
	{
		vlist[i++]=(*it);
		++it;
	}

	return vlist;
}

template<class Number> MatVectorList<Number>* MatVectorList<Number>::clone()
{
	using namespace std;
	typename list<MatVector<Number>*>::iterator it = lstMatVector.begin();
	typename list<MatVector<Number>*>::iterator itEnd = lstMatVector.end();

	MatVectorList<Number> *lstTemp =new MatVectorList<Number>();

	while(it!=itEnd)
	{
		lstTemp->insert((*it)->clone());
		++it;
	}

	return lstTemp;
}

template<class Number> bool MatVectorList<Number>::isEqual(const MatVectorList<Number> *lst)
{
	using namespace std;
	if(size()==lst->size())
	{
		bool ret=true;
		typename list<MatVector<Number>*>::iterator it = lstMatVector.begin();
		typename list<MatVector<Number>*>::iterator it_lst = lst->lstMatVector.begin();
		typename list<MatVector<Number>*>::iterator itEnd = lstMatVector.end();

		while(it!=itEnd)
		{
			ret=(ret)&((*it)->isEqual((*it_lst)));
			++it;
			++it_lst;
			if(ret==false)
				break;
		}

		return ret;
	}
	return false;
}


template<class Number> MatVectorList<Number>& MatVectorList<Number>::save2Txt(std::string name)
{
	using namespace std;
	int i;
	int vect_size;

	ofstream outfile;
	outfile.open(name.c_str());

	if (outfile.is_open())
	{
		list<MatVector<>*>::iterator it = lstMatVector.begin();
		list<MatVector<>*>::iterator itEnd = lstMatVector.end();

		outfile << "# " << lstMatVector.size() << " " << (*it)->size() << endl;

		while(it!=itEnd)
		{
			if((*it)!=NULL)
			{
				vect_size=(*it)->size();

				for(i=0;i<vect_size;i++)
					outfile << (*(*it))[i] << " ";

				outfile << endl;
			}

			++it;
		}

  		outfile.close();
	}

	return *this;
}

template<class Number> MatVectorList<Number>& MatVectorList<Number>::loadTxt(std::string name)
{
	using namespace std;
	ifstream infile;
	infile.open(name.c_str());

	if (infile.is_open())
	{
		string line;

		infile >> line;
		if(line[0]=='#')
		{
			int l,c;
			infile >> l;
			infile >> c;

			while(!((l<=0)||(infile.eof())))
			{
				MatVector<Number> *vtemp = new MatVector<Number>(c);

				for(int i=0;i<vtemp->size();i++)
					infile >> vtemp->vect[i];

				lstMatVector.push_back(vtemp);
				l--;
			}
		}

  		infile.close();
	}
	else
		return false;

	return true;
}






template<class Number> std::string MatVectorList<Number>::toString()
{
	using namespace std;
	int i;
	string outputS;

	if(!lstMatVector.empty())
	{
		typename list<MatVector<Number>*>::iterator it = lstMatVector.begin();
		typename list<MatVector<Number>*>::iterator itEnd = lstMatVector.end();

		while(it!=itEnd)
		{
			if((*it)!=NULL)
				outputS += (*it)->toString() +"\n";
			++it;
		}
	}
	else
		outputS+="[NULL list]";

	return outputS;
}




#endif /*MATVECTORLIST_H_*/
