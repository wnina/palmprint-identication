#include "RedNeuronal.h"
#include <iostream>
#include <fstream>

using namespace std;

RedNeuronal::RedNeuronal(vector<int> info)
{
    num_capas=info.size()
		;
    numNeuroPorCapa=info;
    for(unsigned int i=0;i<info.size();i++){
		//Se crea un constructor  de la clase de capa (nrocapas,nroneunaxcapa,**NuloIzq,**NuloDer)
        Capa capa(i,info[i],NULL,NULL);
        capas.push_back(capa);
    }

    //Agregamos matrices de pesos
	//Aqui se inicializa el doble puntero
	for(unsigned int i=0;i<info.size()-1;i++){
        float **p=new float*[info[i]]; //Se asigna memoria para un **
        for(int j=0;j<info[i];j++)
            p[j]=new float[info[i+1]];
		//Se guarda en **p dependiento de la capa
        capas[i].der=p;
        capas[i+1].izq=p;
    }
}

RedNeuronal::~RedNeuronal()
{
}

vector<float> RedNeuronal::evaluar(vector<float> entradaPatron)
{
    float suma; // Va almacenar  la suma full conect
    //Establecemos los valores de las neuronas capa entrada
    for(int i=0;i<capas[0].num_neuronas;i++){
        capas[0].neuronas[i].setNeto(entradaPatron[i]); 
    }


    //Calculamos la salida PROPAGACION HACIA ADELANTE 
	for(int c=1;c<num_capas;c++){
        //Hallamos el valor neto de cada neurona de la capa
        for(int n1=0;n1<capas[c].num_neuronas;n1++){
            suma=0;
            for(int n0=0;n0<capas[c-1].num_neuronas;n0++){ 
				//Aqui se llama funcion activacion ya que hemos establecido setneto y por tanto
				//Multiplica el valor regresado del valor de activacion por su peso
                suma+=capas[c-1].neuronas[n0].salida()*capas[c-1].der[n0][n1]; //Se llama a la funciontransferia
            }
            capas[c].neuronas[n1].setNeto(suma);
        }
    }

	//Finalmente retornamos el en un vector<float> los valore de la capa de salida
    vector<float> salida;
    for(int i=0;i<capas[num_capas-1].num_neuronas;i++){
        salida.push_back(capas[num_capas-1].neuronas[i].salida());
	}	

    return salida;
}

void RedNeuronal::pesosAleatorios()
{
    int a,b;
    for(int i=0;i<num_capas-1;i++){
        a=numNeuroPorCapa[i];
        b=numNeuroPorCapa[i+1];
        for(int _a=0;_a<a;_a++){
            for(int _b=0;_b<b;_b++){
        		capas[i].der[_a][_b]=(((float)rand()/RAND_MAX)>0.5)?((float)rand()/RAND_MAX):(-1*((float)rand()/RAND_MAX));
		    }
        }
    }
}

vector<float> RedNeuronal::entrenar(vector<pair<vector<float>,vector<int> > > Patron)
{
    vector<vector<float> > error;
    vector<float> salida;
    vector<float> newerror;
    vector<float> er;
    vector<float> error_total;
    float aux,p;

    for(unsigned int i=0;i<Patron.size();i++){
        error.clear();
        //Calculamos la salida para una entrada
        salida=evaluar(Patron[i].first);
        //Halla el Error capa de salida
        er.clear();
        for(int s=0;s<capas[num_capas-1].num_neuronas;s++){
			// Se calcula el error de la neurona y se utiliza la derivada de la funcion sigmoide
			aux=(Patron[i].second[s]-salida[s])*salida[s]*(1-salida[s]); //Calcula el error delta
            er.push_back(aux);
        }
        //calculamos error total
        p=0;
        for(unsigned int e=0;e<er.size();e++){
            p=p+er[e]*er[e];//Se hace una sumatoria de todos los errores de la capa de salida para un patron
        }
        error_total.push_back(p*0.5); 

        error.insert(error.begin(),er);
  
		//Error para capas ocultas
        for(int s=num_capas-2;s>0;s--){
            for(int n=0;n<capas[s].num_neuronas;n++){
                aux=0;
                for(int w=0;w<capas[s+1].num_neuronas;w++){
                    aux=aux+capas[s].der[n][w]*er[w];
                }
                newerror.push_back(aux*capas[s].neuronas[n].salida()*(1-capas[s].neuronas[n].salida())); //Se calcula el error de la neurona
            }
            error.insert(error.begin(),newerror);
            er=newerror;
            newerror.clear();
        }
        error.insert(error.begin(),newerror);
	

        //Actualizamos los nuevos pesos
        for(int c=0;c<num_capas-1;c++){
            for(int n=0;n<capas[c].num_neuronas;n++){
                for(int w=0;w<capas[c+1].num_neuronas;w++){
                    aux=(error[c+1])[w]*capas[c].neuronas[n].salida(); //Tasa de Crecimiento
                    capas[c].der[n][w]=capas[c].der[n][w]+aux;
                }
            }
        }

    }
    return error_total;
}

void RedNeuronal::setPesos(vector<float**> pesosEntrada){
	//Los pesos de la RN se almacena en clase capa
    for(unsigned int c=0;c<pesosEntrada.size();c++){
        capas[c].der=pesosEntrada[c];
        capas[c+1].izq=pesosEntrada[c];
    }
}

vector<float> RedNeuronal::fitness(vector<pair<vector<float>,vector<int> > > Patron){
   
	vector<float> salida;
    vector<float> error_total;
    vector<float> er; // El vector de errores 
    float aux,p;

    for(unsigned int i=0;i<Patron.size();i++){
        //Calculamos la salida para una entrada
        salida=evaluar(Patron[i].first); 
        //Calcula el Error capa de salida del valor esperado - obtenido
        er.clear();
        for(int s=0;s<capas[num_capas-1].num_neuronas;s++){
			aux=(Patron[i].second[s]-salida[s]);
            aux=aux*aux;
			//Aux almacena el error de cada neurona de un patron
            er.push_back(aux);
        }
        //calculamos error total de todos los patrones para el individuo
        p=0;
        for(unsigned int e=0;e<er.size();e++){
            p=p+er[e]*er[e];
        }

		
        error_total.push_back(p*0.5);
    }
	//Al final retorno un vector errores de cada patron
    return error_total;
}

void RedNeuronal::retornarPesos()
{
	ofstream pesosRed, pesosRed2;

	pesosRed.open("archivosPixelROI/pesosRed.txt");
	pesosRed2.open("archivosPixelROI/pesosRed2.txt");

	//Actualizamos los nuevos pesos

        for(int n=0;n<capas[0].num_neuronas;n++){
            for(int w=0;w<capas[0+1].num_neuronas;w++){
				//Guardamos los pesos en un archivo			
				pesosRed<<capas[0].der[n][w]<<"  ";
			}
			pesosRed<<endl;
        }


        for(int n=0;n<capas[0].num_neuronas;n++){
            for(int w=0;w<capas[1+1].num_neuronas;w++){
				//Guardamos los pesos en un archivo			
				pesosRed2<<capas[1].der[n][w]<<"  ";
			}
			pesosRed2<<endl;
        }

}