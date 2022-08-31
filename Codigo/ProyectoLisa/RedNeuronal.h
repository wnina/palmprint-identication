#pragma once

#include "Capa.h"

#include <vector>
#include <stdlib.h>
#include <utility>
#include <math.h>

using namespace std;

class RedNeuronal
{
	private:
		int num_capas;
		vector<int> numNeuroPorCapa;
		vector<Capa> capas;
		
	public:
		RedNeuronal(vector<int>);
		virtual ~RedNeuronal();
		vector<float> evaluar(vector<float>);
		void setPesos(vector<float**>);
		void pesosAleatorios();
		vector<float> entrenar(vector<pair<vector<float>,vector<int> > > );
		vector<float> fitness(vector<pair<vector<float>,vector<int> > > );
		void  retornarPesos();

};

