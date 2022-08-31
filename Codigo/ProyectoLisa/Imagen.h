#pragma once

#include <opencv\cxcore.h>
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <fstream>
#include <sstream>

using namespace std;
using namespace cv;

//ref class Imagen
class Imagen
{

private:
	
	string nombImagen;
	static const int thresholdArea = 27599; // Es el umbral de area para quitar las manchas
	double angulo;
	int LadoROI; // Es es tamaño del lado del cuadrado de la region de interes (ROI)
	vector<vector<int>> operadorsolbel;
	
	ofstream imagenAchivoROI; // Se guaran los pixel del imagen ROI
	ofstream imagenArchivoROIVector; // Se guardan lo pixel pero en el formato del vector

	//int depth; // La profundidad de la imagen
	CvSize tamanio; // El tamamaño de la imagen	
	IplImage *imagen;
	IplImage *imagenSuavizada;
	IplImage *imagenBinarizada;
	IplImage *imagenDilatacionErosion;
	IplImage *imagenContornoCompl;
	IplImage *imagenContorno;
	IplImage *imagenDeteccionDedos;
	IplImage *imagenPuntosCentroide;
	IplImage *imagenPuntosCentroideNew;
	IplImage *imagenGirada;
	IplImage imagenAjustadaPre;
	IplImage *imagenAjustada;
	IplImage *imagenPuntoMedioROI;
	IplImage *imagenGiradaNuevo;
	IplImage *imagenTrazaCuadradoROI;
	IplImage imagenROIPre;
	IplImage *imagenROI;
	IplImage imagenSobelROIPre;
	IplImage *imagenSobelROI;
	Mat gradiente;

	CvSeq *contours; // Almacena el contorno de la mano
	CvSeq *contoursCompl; // Almacena el contorno completo de la mano
	CvMemStorage* 	almacenamiento; // Lo que se almacena para contorno y detectar las puntas de los dedos
	
	vector<vector<Point> > contours2;// Almacena los contornos y puntos del contorno de la mano
	vector<Point> puntoSeparacion; //Almacena los Point(X,Y) de los puntos con mas curva de la mano en este caso seria altos y bajos 
	vector<Point> puntoCentSeparacion;//Almacena los Point(X,Y) de los centroides de la separacion de los dedos
	vector<Point> puntoContorno;//Almacena los Point(X,Y) del contorno
	vector<Point> puntoROI;//Almacena el punto medio del ROI y los cuatro vertices del cuadrado
	vector<Point> puntoMedioCentSep; // Almacena el punto medio entre los 2 centroides de lo bajo de los dedos
	vector<int> cantPixelAjustados;

	vector<vector<int>> pixelROI; //Almacena todos los pixel del imagenROI 64x256
	vector<float> vectorCaract; //Almacena la media de las 64 celdas de mi ROI, 8 filas y 8 columnas (obteniendo 80)

public:

	Imagen(const char *nombreArchivo, string  _nombImagen);
	~Imagen(void);

	void ProcesarImagen();

	// Tecnicas para el Pre-Procesado
	IplImage* AplicarPreProcesado(IplImage * imagenEntrada);
	IplImage* ConvertiraBinario(IplImage * imagenEntrada,int umbral,int colorMaximo);
	IplImage* AplicarDilatacionErosion(IplImage* imagenEntrada);
	IplImage* SacarContorno(IplImage* imagenEntrada);
	IplImage* DetectarPuntasDedos(IplImage* imagenEntrada);
	IplImage* SacarCentroidePM(IplImage* imagenEntrada,int cantVecinoLado);
	IplImage* GirarImagen(IplImage *imagenEntrada, double anguloGrados);
	IplImage* AjustarImagen(IplImage *imagenEntrada); 
	IplImage* HallarPuntoMedioROI(IplImage *imagenEntrada); 
	IplImage* TrazaROIImagen(IplImage *imagenEntrada, int LadoROI); 
	IplImage* ExtraerROIImagen(IplImage *imagenEntrada); 
	IplImage* AplicarROISobel(IplImage *imagenEntrada);
	void SacarPixelROISobel(IplImage *imagenEntrada, int tamGrilla);
	
	//Visualizar
	void MostrarTodasImagenes();

	// Otras Funciones
	void EstablecerTamanioImagen();
	IplImage* RetornarimagenPuntosCentroideNew();
	IplImage* RetornarimagenDeteccionDedos();
	IplImage* RetornarimagenSobelROI();
	vector<Point> RetornarpuntoROI();
	vector<float> RetornarvectorCaract();

	//friend int _tmain(int argc, _TCHAR* argv[]);
};