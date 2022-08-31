#pragma once
//ref class AgCromosoma

#include <vector>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>

using namespace std;

class AgCromosoma{
    private:
        vector<float**>data; // Es la matriz de pesos
        float aptitud;
    public:
		AgCromosoma();
        AgCromosoma(vector<int>);
        void setData(vector<float**>);
        virtual ~AgCromosoma();
		vector<float**> retonarData();

   friend class AgPoblacion;
   friend bool fun_sort(AgCromosoma*,AgCromosoma*);
};

