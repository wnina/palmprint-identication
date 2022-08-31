#include "Neurona.h"


Neurona::Neurona(int nroCapa,int nroNeurona)
{
    icapa=nroCapa;
    ineur=nroNeurona;
}

Neurona::~Neurona()
{
    //dtor
}

float Neurona::salida(){
	//Funcion sigmoide
    if(icapa==0)return neto;
    else return 1.0/(1.0+exp(-1.0*neto));
}

float Neurona::getNeto()
{
    return neto;
}

void Neurona::setNeto(float _neto)
{
    neto=_neto;
}
