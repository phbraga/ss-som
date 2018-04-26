#include "MatUtils.h"
#include <string>

double MatUtils::getCurrentRealTimer(void)
{
    struct timeval t;
    gettimeofday(&t, 0);
    return t.tv_sec + t.tv_usec * 1.0e-6;
}

double MatUtils::getCurrentCPUTimer(void)
{
	return static_cast<double>(clock()) / CLOCKS_PER_SEC;
}


/**
 * Converte informa��o de tempo no formato hh:mm:ss.mmm para milisegundos.
 * A string n�o precisa ter todos os campos para a convers�o.
 * @author Daniel F.Gomes
 * @since 06-08-2008
 */
int64 MatUtils::hmsTime2Milliseconds(std::string timeStr,char fieldSeparator0,char fieldSeparator1,char decimalSeparator)
{
	if(decimalSeparator!='.')
	{
		for(unsigned int i=0;i<timeStr.length();i++)
			if(timeStr[i]==decimalSeparator)
				timeStr[i]='.';
	}
	
	using namespace std;

	/*
	 * Converte informa��o de tempo no formato hh:mm:ss.mmm para milisegundos.
	 * A string n�o precisa ter todos os campos para a convers�o.
	 */
	
	int stringLength=timeStr.length();
	if(stringLength!=0)
	{
	
		//Procura pelo primeiro separador
		int fieldSeparator0Position=timeStr.find(fieldSeparator0,0);
		
		//Caso tenha nenhum separator principal
		if(fieldSeparator0Position!=-1)
		{
			double hour=0;
			double minutes=0;
			double seconds=0;
			
			if(fieldSeparator0Position>0)
				hour=atof(timeStr.substr(0,fieldSeparator0Position).c_str());
			
			//Procura pelo pr�ximo separador
			int fieldSeparator1Position;
			
			if((fieldSeparator0Position+1)<stringLength)
			{
				//Se n�o houver separador principal fieldSeparator0position=-1 e somado a 1 d� zero,
				//Ou seja o algoritmo procura automaticamente pelo pr�ximo separador
				fieldSeparator1Position=timeStr.find(fieldSeparator1,fieldSeparator0Position+1);
				
				if(fieldSeparator1Position==-1)
					minutes=atof(timeStr.substr(fieldSeparator0Position+1,stringLength).c_str());
				else
				if((fieldSeparator1Position-fieldSeparator0Position)>1)
					minutes=atof(timeStr.substr(fieldSeparator0Position+1,fieldSeparator1Position).c_str());

			}
			else
				fieldSeparator1Position=-1;
			
			if((fieldSeparator1Position!=-1)&&((fieldSeparator1Position+1)<stringLength))
				seconds = atof(timeStr.substr(fieldSeparator1Position+1,stringLength).c_str());
			
			hour*=3600000.0;
			minutes*=60000.0;
			seconds*=1000.0;
			
			return ((int64)hour)+((int64)minutes)+((int64)seconds);
			
		}
		
		//Se n�o houver separador principal considera o primeiro campo horas
		double temp=(3600000.0*atof(timeStr.c_str()));
		return (int64)temp;
	}
	
	return 0;
}
