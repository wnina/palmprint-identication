#pragma once
#include <math.h>

//ref class Neurona
class Neurona
{

	private:
        float neto;
        int icapa;
        int ineur;
    
	public:
        Neurona(int,int);
        virtual ~Neurona();
        float salida();
        void setNeto(float);
        float getNeto();

};
