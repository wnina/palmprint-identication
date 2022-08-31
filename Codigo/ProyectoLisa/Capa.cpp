#include "Capa.h"

using namespace std;


Capa::Capa(int nroCapas,int nroNeuronas,float **matrizIzq, float **matrizDer)
{
    num_capa=nroCapas;
    num_neuronas=nroNeuronas;
    izq=matrizIzq;
    der=matrizDer;

    for(int i=0;i<nroNeuronas;i++){
		// Se crean las neuronas por cada capa	y lo almacenamos en vector de tipo neurnas;
        Neurona neu(nroCapas,i);
        neuronas.push_back(neu);
    }
}

Capa::~Capa()
{

}
