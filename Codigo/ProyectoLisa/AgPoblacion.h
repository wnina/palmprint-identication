#pragma once

#include "AgCromosoma.h"
#include "RedNeuronal.h"
#include <algorithm>

 
using namespace std;

class AgPoblacion
{
    private:
        vector<AgCromosoma*> individuos;
        vector<AgCromosoma*> hijos;
        vector<float> apt_indiv;
        vector<int> num_neu_capa;
        RedNeuronal *red;

    public:
        AgPoblacion(int nroCromososomas,vector<int> info,RedNeuronal *_red);
        virtual ~AgPoblacion();
        vector<AgCromosoma*> cruzar(AgCromosoma*,AgCromosoma*);
        void mutar(AgCromosoma*);
        void calcularFitness(vector<pair<vector<float>,vector<int> > >);
        AgCromosoma* seleccionarIndividuo();
        void llenarRuleta();
        void ordenarPorFitness();
        void generarHijos();
        void siguientePoblacion();
        AgCromosoma* getMejor();
};
