#pragma once


#include "Neurona.h"

#include <vector>

//ref class Capa
using namespace std;

class Capa
{
	private:
		int num_capa;
		int num_neuronas;
		float **izq; // Es la matriz de pesos 
		float **der;
		vector<Neurona> neuronas;

	public:
		Capa(int nroCapas,int nroNeuronas,float **matrizIzq, float **matrizDer);
		virtual ~Capa();

	friend class RedNeuronal;

};
