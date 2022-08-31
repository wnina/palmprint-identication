#include "agpoblacion.h"


AgPoblacion::AgPoblacion(int nroCromososomas,vector<int> info,RedNeuronal *_red)
{
    num_neu_capa=info;
	//Se crean n cromosomas en la poblacion y lo almacenamos en vector de individuos
    for(int i=0;i<nroCromososomas;i++){
        AgCromosoma *ag=new AgCromosoma(info); // Cada Cromosoma creada se crea un conjunto de pesos aleatorios rand
        individuos.push_back(ag);
        apt_indiv.push_back(0.0); // cada individuo empieza con aptitud de 0
    }
    red=_red;
}

AgPoblacion::~AgPoblacion()
{
    //dtor
}

float **matriz(int x,int y){
    float **p=new float*[x];
    for(int j=0;j<x;j++)
            p[j]=new float[y];
    return p;
}

vector<AgCromosoma*> AgPoblacion::cruzar(AgCromosoma *hijo1,AgCromosoma *hijo2)
{
    vector<AgCromosoma*> h; //Almacena el vector de hijos cruzados
    int x,y;
    float **h1,**h2; // es la matriz de contendra los pesos a cruzar
    vector<float**> H1;
    vector<float**> H2;

    //Probabilidad de Cruce
    if(float(rand()/RAND_MAX)<0.8){

		//Iteramos  entre capas
        for(unsigned int i=0;i<(num_neu_capa.size()-1);i++){

           //Elegimos punto de cruce en donde x fila y y la col 
            x=rand()%num_neu_capa[i]; //Hacemos un rand que estara entre rangp de numero de neuronas por capa
            y=rand()%num_neu_capa[i+1];
            //Asignamos memoria a la matriz entre capas y establecemos el peso de la cromosomas hijos
			h1=matriz(num_neu_capa[i],num_neu_capa[i+1]);
            h2=matriz(num_neu_capa[i],num_neu_capa[i+1]);

			
            //intercambiamos informacion segun fila y columna
			for(int m=0;m<x;m++)
                for(int n=0;n<y;n++){
                    h1[m][n]=(hijo1->data[i])[m][n];
                    h2[m][n]=(hijo2->data[i])[m][n];
                }
            for(int m=x;m<num_neu_capa[i];m++)
                for(int n=y;n<num_neu_capa[i+1];n++){
                    h1[m][n]=(hijo2->data[i])[m][n];
                    h2[m][n]=(hijo1->data[i])[m][n];
                }

            H1.push_back(h1);
            H2.push_back(h2);
        }
        //Creamos Hijos
        AgCromosoma *hijo1=new AgCromosoma();
        hijo1->setData(H1);
        AgCromosoma *hijo2=new AgCromosoma();
        hijo2->setData(H2);
		//---LLamamos a la operacion mutar-----//
        mutar(hijo1);
        mutar(hijo2);
        h.push_back(hijo1);
        h.push_back(hijo2);
    }
	//Devuel 2 nuevos hijos
    return h;
}      

void AgPoblacion::mutar(AgCromosoma *a)
{
	
    int x,y;
    //Probabilidad de Mutacion, iteramos las capas
    for(unsigned int i=0;i<(num_neu_capa.size()-1);i++){
            //cantidad a mutar %
       for(int j=0;j<int(num_neu_capa[i]+num_neu_capa[i+1]);j++){
                //elegimos genes a mutar
                x=rand()%num_neu_capa[i];
                y=rand()%num_neu_capa[i+1];
				(a->data[i])[x][y]=float (rand()/RAND_MAX)*((float(rand()/RAND_MAX)>0.5)?(1.0):(-1.0));
       }
       
    }
}

void AgPoblacion::calcularFitness(vector<pair<vector<float>,vector<int> > > patrones)
{
    float sum;
    vector<float> error;


    for(unsigned int i=0;i<individuos.size();i++){
        red->setPesos((individuos[i])->data); // se envia la matriz de pesos
        error=red->fitness(patrones); // Esto de me devuelve un vector de los errores de los patrones
        sum=0;

		//Sumo el error de cada patron	
		for(unsigned int a=0;a<error.size();a++){
            sum+=error[a];
        }

        apt_indiv[i]=1-sum/error.size();//dividimos entre la cantidad de patrones
        
		(individuos[i])->aptitud=apt_indiv[i];
    }
}

bool fun_sort(AgCromosoma *a,AgCromosoma *b)
{
    return (a->aptitud)<(b->aptitud);
}

void AgPoblacion::ordenarPorFitness()
{
	// Hacemos el ordenamiento por aptitud de cada indivudo de la poblacion 
    sort(individuos.begin(),individuos.end(),fun_sort);
}

AgCromosoma* AgPoblacion::seleccionarIndividuo(){
    
	// La operacion de seleccion lo sacamos con rand de 0-1 si uno
	// de los aptitudes es menor que ese rand se selecciona ese 

	int i=0;
	float ale=(float)rand()/RAND_MAX;
    while(apt_indiv[i]<ale) i++;
    return individuos[i];
}

void AgPoblacion::generarHijos()
{
	//Generamos lo hijos de cada individuo
    AgCromosoma *h1;
    AgCromosoma *h2;
    hijos.clear();
    vector<AgCromosoma*> aux;
    unsigned int c=0;

    while(c<(individuos.size()-2)){
        //-----Etapa de seleccion-----//
		h1=seleccionarIndividuo();
        h2=seleccionarIndividuo();

		//--- Etapa de cruzamiento ----//
        aux=cruzar(h1,h2); //Me retorna vector q contiene los 2 hijos  generados
        
		if(!aux.empty()){
            hijos.push_back(aux[0]); // Despues guardamos los hijos en vector<hijos> del cromosoma
            hijos.push_back(aux[1]);
            c=c+2;//recorre de 2 
        };
    }

}

void AgPoblacion::siguientePoblacion(){
    //nuevaGeneracion
    for(unsigned int i=0;i<individuos.size()-2;i++){
        individuos[i]=hijos[i];
    }
}

AgCromosoma* AgPoblacion::getMejor(){
	//Me retorna el mejor individuo con mejor fitness
    return individuos[individuos.size()-1];
}








