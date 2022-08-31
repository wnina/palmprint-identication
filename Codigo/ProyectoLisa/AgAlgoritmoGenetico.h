#pragma once
//ref class AgAlgoritmoGenetico
#include "AgPoblacion.h"
#include "AgCromosoma.h"
#include <vector>

using namespace std;

class AgAlgoritmoGenetico
{
    private:
        int num_generaciones;
        AgPoblacion *poblacion;
        vector<pair<vector<float>,vector<int> > > patron;
    public:
        AgAlgoritmoGenetico(int nroCromososomas, vector<int> info, RedNeuronal *_red);
        AgCromosoma *nextGeneracion();
        virtual ~AgAlgoritmoGenetico();
        void agregarPatron(pair<vector<float>,vector<int> > _patron);

};
