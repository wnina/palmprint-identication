#include "AgAlgoritmoGenetico.h"
#include "RedNeuronal.h"

using namespace std;

AgAlgoritmoGenetico::AgAlgoritmoGenetico(int nroCromososomas,vector<int> info, RedNeuronal *_red)
{
	//Se crea una nueva poblacion 
	poblacion=new AgPoblacion(nroCromososomas,info,_red);
}

AgAlgoritmoGenetico::~AgAlgoritmoGenetico()
{
}

AgCromosoma *AgAlgoritmoGenetico::nextGeneracion(){

	poblacion->calcularFitness(patron);
	poblacion->generarHijos();
	poblacion->ordenarPorFitness();
	AgCromosoma  *temp = poblacion->getMejor();
    poblacion->siguientePoblacion();
	
	return temp;
}

void AgAlgoritmoGenetico::agregarPatron(pair<vector<float>,vector<int> > _patron){
    patron.push_back(_patron);
}
