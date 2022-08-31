#include "agcromosoma.h"

AgCromosoma::AgCromosoma()
{
}
AgCromosoma::AgCromosoma(vector<int> info)
{
	//Recorre la info para almecenar memoria para las matrices de pesos
    for(unsigned int m=0;m<(info.size()-1);m++){
        float **a=new float*[info[m]];
        for(int i=0;i<info[m];i++){
            a[i]=new float[info[m+1]];
        }
        data.push_back(a); //Se almacenan los pesos
    }

    int n;
	//Se establecen de manera aleatorio entre 0-1 para los pesos inciales de algoritmos geneticos
    for(unsigned int m=0;m<data.size();m++){
        for(int i=0;i<info[m];i++){
            for(int j=0;j<info[m+1];j++){
                (data[m])[i][j]=(float)(rand()/RAND_MAX)*((float(rand()/RAND_MAX)>0.5)?1.0:-1.0);
            }
        }
	}
}

AgCromosoma::~AgCromosoma()
{
    //dtor
}

 void AgCromosoma::setData(vector<float**> _data)
 {
     data=_data;
 }
 
 vector<float**> AgCromosoma::retonarData()
 {
	 return  data;
 }