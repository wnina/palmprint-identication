#include "Imagen.h"


using namespace std;
using namespace cv;

Imagen::Imagen(const char *nombreArchivo, string  _nombImagen)
{	
	// Leemos la imagen y se inicializan algunso datos acerca de ello
	imagen = cvLoadImage(nombreArchivo);
	nombImagen = _nombImagen;
	// Antes de procesar se inicialza algunas atributos de la imagen
	EstablecerTamanioImagen();

	// Otra incializaciones
	angulo =-1;

}

Imagen::~Imagen(void)
{
	// Liberamos memoria
	cvReleaseImage( &imagen );
	cvReleaseImage( &imagenSuavizada);	
	cvReleaseImage( &imagenBinarizada);
	cvReleaseImage( &imagenDilatacionErosion);
	cvReleaseImage( &imagenContorno);
	cvReleaseImage( &imagenContornoCompl);
	cvReleaseImage( &imagenDeteccionDedos);
	cvReleaseImage( &imagenPuntosCentroide);
	cvReleaseImage( &imagenGirada);
	//cvReleaseImage( &imagenAjustada);

	cvDestroyWindow( "example_img" );
	cvDestroyWindow( "example_smoothimg" );
	cvDestroyWindow( "example_binimg" );
	cvDestroyWindow( "example_imagenDilatacionErosion" );
	cvDestroyWindow( "example_imagenContorno" );
	cvDestroyWindow( "example_imagenContornoCompl" );
	cvDestroyWindow( "example_imagenDedos" );
	cvDestroyWindow( "example_imagenPuntosCentroide" );
	cvDestroyWindow( "example_imagenAjustada" );
}

void Imagen::ProcesarImagen()
{

	//Se llama al ancho de la ventana 3x3 y se aplica el filtro gaussiano que viene hacer el valor 2 (CV_GAUSSIAN)
	imagenSuavizada = AplicarPreProcesado(imagen);
	
	//Entrana a la funcion el umbral y el valor maximo para el pixel q se usara para binarizar la imagen
	imagenBinarizada = ConvertiraBinario(imagenSuavizada,17,255);
	imagenDilatacionErosion = AplicarDilatacionErosion(imagenBinarizada);
	imagenContorno = SacarContorno(imagenDilatacionErosion);
	imagenDeteccionDedos = DetectarPuntasDedos(imagenContorno);	

	// Nos establece la cantidad de vecinos (usaremos 10)por lado que se va tomar y saca el centroide
	imagenPuntosCentroide = SacarCentroidePM(imagenDilatacionErosion, 15);
	imagenGirada = GirarImagen(imagenDilatacionErosion, angulo);

	// Ya que la imagen fue girada entonces se tiene q sacar el contorno y detectarPuntasDedos otra vez
	imagenContorno = SacarContorno(imagenGirada);
	imagenDeteccionDedos = DetectarPuntasDedos(imagenContorno);	
	imagenPuntosCentroideNew = SacarCentroidePM(imagenGirada, 15);
	AjustarImagen(imagenPuntosCentroideNew); // Se llama este metodo para calcular cantPixelAjustados, que se va borra en la imagen original
	imagenPuntoMedioROI = HallarPuntoMedioROI(imagenPuntosCentroideNew); // Hallamo el punto medio de la region de interes

	// Ahora Giramos la imagen original y dibujamos la ROI
	imagenGiradaNuevo = GirarImagen(imagen, angulo);
	//El parametro que entra es el lado del cuadrado y la imagen girada
	imagenTrazaCuadradoROI =TrazaROIImagen(imagenGiradaNuevo, 128);
	imagenROI = ExtraerROIImagen(imagenTrazaCuadradoROI);
	imagenSobelROI = AplicarROISobel(imagenROI);
	// El tamanio de la grilla es 8
	SacarPixelROISobel(imagenSobelROI,8);

	//cvShowImage("wilder",);
	//MostrarTodasImagenes();


}

IplImage* Imagen::AplicarPreProcesado(IplImage * imagenEntrada)
{
	// Convertir a Imagen a Escala de grises con un solo canal de 8 bits 
	IplImage * imagenSalida;
	imagenSalida = cvCreateImage(tamanio, IPL_DEPTH_8U, 1);
	cvCvtColor(imagenEntrada,imagenSalida,CV_BGR2GRAY);

	// Aplicar un suavizado(quitar el ruido) mediante el filtro gausiano
	//cvSmooth( GrayImage, SmoothImage, CV_MEDIAN, 1,1);// Perform a Gaussian blur ( Convolving with 11 X 11 Gaussian).
	
	
	cvSmooth( imagenSalida, imagenSalida, 2, 3,3);// Perform a Gaussian blur ( Convolving with 11 X 11 Gaussian).
	return imagenSalida;
}

IplImage* Imagen::ConvertiraBinario(IplImage * imagenEntrada, int umbral, int colorMaximo)
{
	// Convertir a binario mediante la operacion de umbralizacion en donde se tomara el umbral de otsu que nos provee Opencv (umbral glo bal optimo) 
	IplImage * imagenSalida;
	imagenSalida = cvCreateImage(tamanio, IPL_DEPTH_8U, 1);
	cvThreshold(imagenEntrada,imagenSalida, umbral,colorMaximo,CV_THRESH_BINARY);

	//cvThreshold(SmoothImage, BinImage, 0, 123, CV_THRESH_BINARY| CV_THRESH_OTSU);

	// Se usara la funcion cvFindContours para encontrar los contornos de la imagen binarizada
	// para hacer una umbralizacion por area y asi solo quedarnos con la palma de la mano y
	// eliminar las manchas.

	IplImage* img_out       = cvCloneImage( imagenSalida ); 

	CvMemStorage* almacenamiento   = cvCreateMemStorage( 0 );    // container of retrieved contours
	CvSeq* contornos         = 0;   
	CvScalar black          = CV_RGB( 0, 0, 0 ); // black color
	CvScalar white          = CV_RGB( 255, 255, 255 );   // white color

	double area;

	// La funcion cvFindContours tiene un bug ya no toma lo pixeles de los bordes  de la imagen
	// es por eso que se crea un imagen q tenga 2 filas y 2 columnas mas que la imagen binarizada para eliminar
	// asi poder capturar los contornos completos y posteriormente eliminar las manchas 

	IplImage * image_find = cvCreateImage(cvSize(imagenSalida->width+2,imagenSalida->height+2), 8, 1);
	cvCopyMakeBorder(imagenSalida, image_find, cvPoint(1,1), IPL_BORDER_CONSTANT);

	//IplImage* img_out       = cvCloneImage (image_find);

	//IplImage * image_find2 = cvCloneImage( image_find ); 
	cvFindContours(image_find,almacenamiento,&contornos, sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE, cvPoint(-1,-1));


	while (contornos)   // itera todos los contornos encontrados
	{
		area = cvContourArea( contornos, CV_WHOLE_SEQ );

		if (fabs(area) <= thresholdArea)  // Si el area de cortono es menor que el umbral de area entonces se remueve
		{
			// dibuja los contornos dentro de una nueva imagen que vendria hacer imagenSinMancjha
			cvDrawContours (img_out, contornos, black, black, -1, CV_FILLED, 8, cvPoint(0,0)); //  remueve los puntos blancos    
		}
		/* else
		{
		cvDrawContours( img_out, contornos, white, white, -1, CV_FILLED, 8 ); // rellena los huecos
		}*/
		contornos = contornos->h_next;    // salta al siguiente contorno
	}

	cvReleaseMemStorage( &almacenamiento );
	// Al final se guarda la imagen en imagenSinManchas
	return img_out;


}

IplImage* Imagen::AplicarDilatacionErosion(IplImage* imagenEntrada)
{
	// En este metodo se aplicacion la tecnica de la dilatacion (para eliminar las
	// grietas que exite entre los dedos despues de hacer la binarizacion) y despues 
	// se aplica la erosion para eliminar los reflejos

	IplImage * imagenSalida;
	imagenSalida = cvCreateImage(tamanio, IPL_DEPTH_8U, 1);	
	cvDilate(imagenBinarizada, imagenSalida, NULL, 3);

	//Se aplica erosion
	cvErode(imagenSalida, imagenSalida, NULL, 1);

	return imagenSalida;	
}

IplImage* Imagen::SacarContorno(IplImage* imagenEntrada){

	// La funcion sacara el contorno (recorrido del borde de la imagen), en se utilizara
	// cvFindCountours para sacar los contornos y cvDrawContours para dibujar los que
	// fueron encontrador (se dibujea de colrr blanco)

	// Eliminamos el contenido anterior 
	puntoContorno.erase(puntoContorno.begin(), puntoContorno.end());

	IplImage *imagenContornoSalida = cvCloneImage(imagenEntrada);
	imagenContornoCompl = cvCloneImage(imagenDilatacionErosion);
	almacenamiento = cvCreateMemStorage(0);
	contours = 0;
	CvScalar white  = CV_RGB( 255, 255, 255 );   // Color Blanco
	vector<Vec4i> hierarchy;

	// Aqui almacenamos todos los puntos del contorno y lo almacenamos en un vector de puntos
	// Convertiendo de  IplImage a cv::Mat  

	Mat image=cvarrToMat(imagenContornoSalida);  
	findContours(image, contours2, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	for(size_t k = 0; k<contours2.size(); k++)
	{
		//procesamos el contours2[i]
		for (size_t j=0; j<contours2[k].size(); j++)
		{
			puntoContorno.push_back(Point(contours2[k][j].x, contours2[k][j].y));

		}
	}

	cvFindContours( imagenContornoCompl, almacenamiento, &contoursCompl );
	// Aqui estamos .. modificando esto contours2 por contoursCompl
	//cvDrawContours(imagenContornoCompl,contoursCompl , CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 1, 1, 8, cvPoint(0, 0));
	cvDrawContours(imagenContornoCompl,contoursCompl , CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 1, 1, 8, cvPoint(0, 0));


	// Aqui sacamos especificamenete sacamos el contorno de la mano

	//cvZero( imagenContorno );
	cvFindContours( imagenContornoSalida, almacenamiento, &contours );
	//cvFindContours(imagenContorno, almacenamiento, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
	contours = cvApproxPoly(contours, sizeof(CvContour), almacenamiento, CV_POLY_APPROX_DP, 25, 1);

	cvDrawContours(imagenContornoSalida, contours, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 1, 1, 8, cvPoint(0, 0));

	for(int i=0; i<contoursCompl->total; i++){

		//Try to access Point data -- Crash here when threshold level 114 ?
		//Works when uncommented
		CvPoint* p = CV_GET_SEQ_ELEM(CvPoint, contoursCompl, i);
		// cvCircle(prueba, Point(p->x, p->y), 1, CV_RGB(255, 255, 255), 1, 8, 0);

	}

	return imagenContornoSalida;
}

IplImage* Imagen::DetectarPuntasDedos(IplImage* imagenEntrada)
{

	IplImage* imagenDeteccionDedosS = cvCloneImage(imagenEntrada);
	puntoSeparacion.erase(puntoSeparacion.begin(),puntoSeparacion.end()); // Limpiamos el contenido anterior

	CvSeq *hull = cvConvexHull2(contours, 0, CV_CLOCKWISE, 0);
	CvSeq *defect = cvConvexityDefects(contours, hull, NULL);


	for(; defect; defect = defect->h_next)
	{
		int nomdef = defect->total;

		if(!nomdef)
			continue;
		CvConvexityDefect* defectArray = (CvConvexityDefect*)malloc(sizeof(CvConvexityDefect)*nomdef);
		cvCvtSeqToArray(defect, defectArray, CV_WHOLE_SEQ);

		for(int i = 0; i < nomdef; i++)
		{
			cvCircle(imagenDeteccionDedosS, *(defectArray[i].end), 3, CV_RGB(255, 255, 255), -1, 8, 0);
			cvCircle(imagenDeteccionDedosS, *(defectArray[i].start), 3, CV_RGB(255, 255, 255), -1, 8, 0);
			cvCircle(imagenDeteccionDedosS, *(defectArray[i].depth_point), 3, CV_RGB(255, 255, 255), -1, 8, 0);

			cout<< "Punto: "<< defectArray[i].depth_point->x<<","<<defectArray[i].depth_point->y<<endl;
			// Se almacena los puntos de Separacion 
			puntoSeparacion.push_back(Point( defectArray[i].depth_point->x, defectArray[i].depth_point->y));
			//puntoSeparacion[i].x = defectArray[i].depth_point->x;
			//puntoSeparacion[i].y = defectArray[i].depth_point->y;
		}

		free(defectArray);
	}

	// Se libera memoria
	cvReleaseMemStorage( &almacenamiento );

	return imagenDeteccionDedosS;

}

IplImage* Imagen::SacarCentroidePM(IplImage* imagenEntrada, int cantVecinoLado)
{
	// Sacar el centroide a partir de los puntos detectados de ConvexityDefects (Punto de separacion) y los sus
	// respectivos vecinos de ahi se obtendra C1 y C2 de ahi se calcula el punto medio entre los 2 centroides la
	// razon por la que se hace es porque los centroidos son mucho invariantes 

	puntoCentSeparacion.erase(puntoCentSeparacion.begin(),puntoCentSeparacion.end()); // Limpiamos el contenido anterior
	puntoMedioCentSep.erase(puntoMedioCentSep.begin(),puntoMedioCentSep.end());

	// Eliminamos el punto el que se encuentra entre los 2 puntos de separacion
	vector<int> p;
	vector<Point> puntoSeparacionNuevo;

	for(int z = 0;z<puntoSeparacion.size();++z)
	{
		p.push_back(puntoSeparacion[z].y);
	}

	sort(p.begin(), p.end());

	// Buscamos el indice
	for(int i = 0;i<puntoSeparacion.size();++i)
	{
		if(p[1]!=puntoSeparacion[i].y)
		{
			puntoSeparacionNuevo.push_back(Point(puntoSeparacion[i].x,puntoSeparacion[i].y));
		}
	}

	// Buscamos los indices de los puntos de seperacion  
	// y recorremos los vecinos para hallar el centroide

	for(size_t ps=0; ps<2; ps++){

		// Se recorren los vecinos por la derecha por las 2 coordenadas
		int sumDerechaX=0, sumDerechaY=0;				
		int sumizquierdaX=0, sumizquierdaY=0;

		int sumX=0, sumY=0;
		int mediaX=0, mediaY=0;

		for (size_t j=0; j<puntoContorno.size(); j++)
		{

			if(puntoContorno[j].x == puntoSeparacionNuevo[ps].x && puntoContorno[j].y == puntoSeparacionNuevo[ps].y)
			{
				cout<<"que esta dentrando"<< puntoSeparacionNuevo[ps].x<<";"<< puntoSeparacionNuevo[ps].y<<endl;
				for (size_t k=j+1; k<=j+cantVecinoLado; k++)
				{
					sumDerechaX = sumDerechaX + puntoContorno[k].x;
					sumDerechaY = sumDerechaY + puntoContorno[k].y;	
				}
				// Se recorren los vecinos por la izquierda
				for (size_t k=j-1; k>=j-cantVecinoLado; k--)
				{
					sumizquierdaX = sumizquierdaX + puntoContorno[k].x;	
					sumizquierdaY = sumizquierdaY + puntoContorno[k].y;	
				}
				// Calculamos la suma global y la media
				sumX = sumDerechaX + sumizquierdaX +puntoSeparacionNuevo[ps].x;
				mediaX = sumX/(cantVecinoLado*2+1);

				sumY = sumDerechaY + sumizquierdaY +puntoSeparacionNuevo[ps].y;
				mediaY = sumY/(cantVecinoLado*2+1);

			}

		}
		puntoCentSeparacion.push_back(Point(mediaX,mediaY));
		cout<< "Centroide Punto : "<<mediaX<<","<<mediaY<<endl;
	}

	// Dibuja en la mano
	IplImage* imagenPuntosCentroideS;

	imagenPuntosCentroideS = cvCloneImage(imagenEntrada);
	cvCircle(imagenPuntosCentroideS, Point(puntoCentSeparacion[0].x,puntoCentSeparacion[0].y), 5, CV_RGB(255, 255, 255), -1, 8, 0);
	cvCircle(imagenPuntosCentroideS, Point(puntoCentSeparacion[1].x,puntoCentSeparacion[1].y), 5, CV_RGB(255, 255, 255), -1, 8, 0);

	// Se calcula el angulo para ello primero hallamos  la pendiente (tag del angulo)	
	double pendiente=(double)(puntoCentSeparacion[0].x - puntoCentSeparacion[1].x)/(puntoCentSeparacion[0].y - puntoCentSeparacion[1].y);

	// Sacamos el arctg de la pendiente y nos da el angulo
	if(angulo ==-1){
		angulo = atan (pendiente) * 180 / CV_PI;
	}
	cout<<"El angulo es: "<<angulo<<"La pendiente es: "<<pendiente;
	// Ahora sacaremos el Punto medio entre los centroides

	int PuntoMedioX=0,PuntoMedioY=0;

	PuntoMedioX = (puntoCentSeparacion[0].x + puntoCentSeparacion[1].x)*0.5;
	PuntoMedioY = (puntoCentSeparacion[0].y + puntoCentSeparacion[1].y)*0.5;

	puntoMedioCentSep.push_back(Point(PuntoMedioX,PuntoMedioY));

	cout<<"El punto medio es: "<< PuntoMedioX<<","<<PuntoMedioY;

	// Dibujamos el punto medio
	cvCircle(imagenPuntosCentroideS, Point(PuntoMedioX,PuntoMedioY), 5, CV_RGB(255, 255, 255), -1, 8, 0);

	// Dibujamos la linea horizontal
	cvLine(imagenPuntosCentroideS, Point(puntoCentSeparacion[0].x,puntoCentSeparacion[0].y),Point(puntoCentSeparacion[1].x,puntoCentSeparacion[1].y), CV_RGB(255, 255, 255), 1, 88, 0);

	// Dibujamos la linea horizontal
	// Aqui se crea una recta con angulo de 90 sobre la linea vertical

	int XDest=60;
	int XOri=280;

	// La formula de mi recta vertical
	int YDest = pendiente*-1*(XDest-PuntoMedioX)+PuntoMedioY;
	int YOri = pendiente*-1*(XOri-PuntoMedioX)+PuntoMedioY;

	cvLine(imagenPuntosCentroideS, Point(XOri,YOri),Point(XDest,YDest), CV_RGB(255, 255, 255), 1, 88, 0);

	return imagenPuntosCentroideS;
}

IplImage* Imagen::GirarImagen(IplImage *imagenEntrada, double anguloGrados)
{
	// Se crea una matrix, en donde la matriz izquierda es 2x2
	// es la transformada y la derecha es 2x1 es la dimension

	float m[6];

	CvMat M = cvMat(2, 3, CV_32F, m);
	int w = imagenEntrada->width;
	int h = imagenEntrada->height;
	//float angleRadians = anguloGrados * ((float)CV_PI / 180.0f);

	float angleRadians = anguloGrados * ((float)CV_PI / 180.0f);

	m[0] = (float)( cos(angleRadians) );
	m[1] = (float)( sin(angleRadians) );
	m[3] = -m[1];
	m[4] = m[0];
	m[2] = w*0.5f;  
	m[5] = h*0.5f;  

	// Hacer una imagen de recambio para el resultado
	CvSize sizeRotated;
	sizeRotated.width = cvRound(w);
	sizeRotated.height = cvRound(h);

	// Rotatar
	IplImage *imageRotated = cvCreateImage( sizeRotated,
		imagenEntrada->depth, imagenEntrada->nChannels );

	// Transformar la imagen
	cvGetQuadrangleSubPix( imagenEntrada, imageRotated, &M);

	cout<<"El angulo de giro es:"<<angulo<<endl;
	return imageRotated;
}

IplImage* Imagen::AjustarImagen(IplImage *imagenEntrada)
{

	//IplImage *imagenContornoSalida = cvCloneImage(imagenEntrada);
	Mat image=cvarrToMat(imagenEntrada);  

	int contador = 0;
	int indIzq=0, indDerecha=0, indArriba=0, indAbajo=0;

	// Por arriba
	for(int i = 0; i<imagenEntrada->height; ++i)
	{
		for (int j=0; j<imagenEntrada->width; ++j)
		{
			if(CV_IMAGE_ELEM(imagenEntrada, unsigned char, i, j)==255){
				indArriba=i;
				goto parar;
			}
		}
	}

parar:
	cout<<"Valor de arriba "<<indArriba<<endl;


	// Por la derecha
	for(int i = 0; i<imagenEntrada->width; ++i)
	{
		for (int j=0; j<imagenEntrada->height; ++j)
		{
			if(CV_IMAGE_ELEM(imagenEntrada, unsigned char, j, i)==255){
				indDerecha=i;
				goto parar2;
			}
		}
	}

parar2:
	cout<<"Valor de derecha "<<indDerecha<<endl;


	// Por abajo
	for(int i = (imagenEntrada->height)-1; i>=0; i--)
	{
		for (int j=0; j<imagenEntrada->width; ++j)
		{
			if(CV_IMAGE_ELEM(imagenEntrada, unsigned char, i, j)==255){
				indAbajo=i;
				goto parar3;
			}
		}
	}

parar3:
	cout<<"Valor de abajo "<<indAbajo<<endl;



	// Por izquierda
	// Hay un problema al momento de iterar ... :(
	for(int i = imagenEntrada->width-1; i>0; --i)
	{
		for (int j=0; j<imagenEntrada->height; ++j)
		{
			if(CV_IMAGE_ELEM(imagenEntrada, unsigned char, j, i)==255){
				indIzq=i;
				goto parar4;
			}
		}
	}

parar4:
	cout<<"Valor de izquierda "<<indIzq<<endl;

	cout<< "El width es"<<(imagenEntrada->width)<< "El height"<<(imagenEntrada->height)<<endl;


	Mat imagenSinArriba = image.rowRange(indArriba, image.rows);
	Mat imagenSinAbajo = imagenSinArriba.rowRange(0, indAbajo-indArriba);
	Mat imagenSinDer = imagenSinAbajo.colRange(indDerecha,imagenSinAbajo.cols);
	Mat imagenSinIzq = imagenSinDer.colRange(0, indIzq-indDerecha);

	cantPixelAjustados.push_back( indIzq);
	cantPixelAjustados.push_back( indDerecha);
	cantPixelAjustados.push_back( indArriba);
	cantPixelAjustados.push_back( indAbajo);

	//imshow("wilderrrrrrrrrrrrrr",imagenSinIzq);

	imagenAjustadaPre;
	imagenAjustadaPre = imagenSinIzq;	

	//IplImage *imagenSalida2 =&

	return &imagenAjustadaPre;
}	

IplImage* Imagen::HallarPuntoMedioROI(IplImage *imagenEntrada)
{
	// Primero se calcula la altura de la palma se vendra ser la distancia entre el punto medio 
	// de los centroides hallados y la base de muñeca

	IplImage *imagenSalida;
	imagenSalida = cvCloneImage(imagenEntrada);


	// Se toman los puntos de inicio el punto medio y fin el tamaño de maximo de x
	int XDest=cantPixelAjustados[0];
	int XOri=puntoMedioCentSep[0].x;

	// La formula de mi recta vertical
	int YDest =puntoMedioCentSep[0].y;
	int YOri = puntoMedioCentSep[0].y;

	// Hallamos el punto medio del ROI
	int puntoMedioRoiX = (XOri+XDest)/2;
	int puntoMedioRoiY = (YOri+YDest)/2;

	// Se hallan los 4 vertices

	// Se almacena en el vector el punto medio del ROI
	puntoROI.push_back(Point(puntoMedioRoiX,puntoMedioRoiY));

	cvLine(imagenSalida, Point(puntoMedioCentSep[0].x,YOri),Point(XDest,YDest), CV_RGB(0, 0, 0), 1, 88, 0);
	cvCircle(imagenSalida, Point(puntoROI[0].x,puntoROI[0].y), 5, CV_RGB(0, 0, 0), -1, 8, 0);
	//cout<<"PuntomedioROI"<<puntoROI[0].x<<","<<puntoROI[0].y;

	return imagenSalida;
}

IplImage* Imagen::TrazaROIImagen(IplImage *imagenEntrada, int lado)
{
	IplImage *imagenSalida;
	imagenSalida = cvCloneImage(imagenEntrada);

	LadoROI=lado;

	puntoROI[0].x;
	//cvCircle(imagenSalida, Point(puntoROI[0].x,puntoROI[0].y), 5, CV_RGB(0, 0, 0), -1, 8, 0);

	// Se dibuja el cuadrado y se guarda las posicione s
	puntoROI.push_back(Point(puntoROI[0].x-lado*0.5,puntoROI[0].y-lado*0.5));
	puntoROI.push_back(Point(puntoROI[0].x+lado*0.5,puntoROI[0].y+lado*0.5));
	puntoROI.push_back(Point(puntoROI[0].x-lado*0.5,puntoROI[0].y+lado*0.5));
	puntoROI.push_back(Point(puntoROI[0].x+lado*0.5,puntoROI[0].y-lado*0.5));


	cvRectangle(imagenSalida, Point(puntoROI[0].x-lado*0.5 ,puntoROI[0].y-lado*0.5), Point(puntoROI[0].x+lado*0.5 ,puntoROI[0].y+lado*0.5), CV_RGB(255, 255, 255), 2, 8, 0);
	return imagenSalida;
}

IplImage* Imagen::ExtraerROIImagen(IplImage *imagenEntrada)
{
	IplImage *imagenSalida;
	imagenSalida = cvCloneImage(imagenEntrada);
	Mat image=cvarrToMat(imagenEntrada);  

	Mat imagenSinArriba = image.colRange(puntoROI[1].x, puntoROI[4].x);
	Mat imagenSinAbajo = imagenSinArriba.rowRange(puntoROI[1].y, puntoROI[3].y);
	//Mat imagenSinDer = imagenSinAbajo.colRange(indDerecha,imagenSinAbajo.cols);
	//Mat imagenSinIzq = imagenSinDer.colRange(0, indIzq-indDerecha);
	imagenROIPre = imagenSinAbajo;


	return &imagenROIPre;
}

IplImage* Imagen::AplicarROISobel(IplImage *imagenEntrada)
{
	// Se sacara la gradiendo en X y Y usando el operador de sobel con mascara
	// Donde Gx: -1 0 1   y Gy: -1 -2 -1
	//			 -2 0 2		     0  0  0
	//			 -1 0 1		     1  2  1

	Mat src, src_gray;
	src = cvarrToMat(imagenEntrada);

	int scale = 1;
	int delta = 0;
	//int ddepth = CV_16S;
	int ddepth = CV_8U;

	
	// Se aplica el filtro gaussianoblur
	GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );


	/// Se convierte a Escala de Grises
	cvtColor( src, src_gray, COLOR_RGB2GRAY );

	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_x, abs_grad_x );

	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_y, abs_grad_y );

	/// Total Gradient (approximate)
	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, gradiente );

	imagenSobelROIPre = gradiente;

	return &imagenSobelROIPre;
}

void Imagen::SacarPixelROISobel(IplImage *imagenEntrada, int tamGrilla)
{
	//Se guardan los pixeles del ROI en un vector<vector<int>> pixelROI
	// Donde el ROI es de 128x128, y la grilla sera de tamanio de 8x8 entonces
	// cada  celda se de tamanio 16x16. Finalmente se guarda en un .txt 

	int tamCelda = LadoROI/tamGrilla; // es 16
	vector<int> temp; // vector temporal 
	int indiceGrilla = 0;

	// Se inicializa pixelROI 64 celdas
	for(int i=0;i<tamGrilla*tamGrilla;++i) { pixelROI.push_back(temp);}


	for(int i = 0; i<imagenEntrada->height; i+=tamCelda)
	{
		for(int  w=i; w<tamCelda+i; ++w){
		
			indiceGrilla=i/2;

			for (int j=0; j<imagenEntrada->width; j+=tamCelda)
			{				
				for(int k=j; k<tamCelda+j; ++k)
				{				
					int valor=CV_IMAGE_ELEM(imagenEntrada, unsigned char, w, k);
					//temp.push_back(valor);
					pixelROI[indiceGrilla].push_back(valor);
				}
				//temp.erase(temp.begin(),temp.end());
				indiceGrilla++;
			}	
		}
	}

	//Una vez obtenidos el vector<<vector<int>>> pixelROI extraemos vector caracteristica 
	//vector<int> vectorCaract que se usara para la capa de entrada

	float sumTemp=0;
	float mediaTemp=0;

	//Los 64 componentes del vector caracteristica
	
	for(unsigned int i=0;i<pixelROI.size();++i){
		for(unsigned int j=0;j<pixelROI[i].size();++j){
			sumTemp = sumTemp + pixelROI[i][j];
		}
		//Sacamos la media de todos lo valores (128x128) de la  celda, los 64 componentes
		mediaTemp = (float)sumTemp/pixelROI[i].size();
		//vectorCaract[i] = mediaTemp;
		vectorCaract.push_back(mediaTemp*0.1);
		sumTemp = 0;
		mediaTemp = 0;
	}

	
	
	// Los 16 componenentes restantes del vector caracteristica
	for(unsigned int i=0;i<vectorCaract.size()-tamGrilla;i+=tamGrilla){
		for(unsigned int j=i;j<(tamGrilla+i);++j){
			//cout<<"j: "<<j<<"i: "<<i<<endl;
			sumTemp = sumTemp + vectorCaract[j];
			//cout<<"revision"<<endl;
			//cout<<sumTemp<<endl;		
		}
		//Las 8 filas, empieza el vectorCaract desde 64
		mediaTemp = (float)sumTemp/tamGrilla;
		//vectorCaract[pixelROI.size()+i] = mediaTemp;
		vectorCaract.push_back(mediaTemp*0.1);
		sumTemp = 0;
		mediaTemp = 0;
	}


	for(unsigned int i=0;i<tamGrilla;++i){
		for(unsigned int j=i;j<=vectorCaract.size()-2*tamGrilla+i;j+=8){
			sumTemp = sumTemp + vectorCaract[j];
		}
		//Las 8 columnas, empieza el vectorCaract desde 72
		mediaTemp = (float)sumTemp/tamGrilla;
		//vectorCaract[pixelROI.size()+tamGrilla+i] = mediaTemp;
		vectorCaract.push_back(mediaTemp*0.1);
		sumTemp = 0;
		mediaTemp = 0;
	}


	
	/*
	cout<<"Vector Caracteristica"<<endl;
	
	for(unsigned int i=0;i<vectorCaract.size();++i){
		cout<<vectorCaract[i]<<endl;

	}
	cin.get();
	*/
	// Escribimos en el disco

	stringstream concatenar;
	string salida;
	concatenar<<"imagenROIVector"<<"_"<<nombImagen;
	concatenar >>salida;

    imagenArchivoROIVector.open("archivosPixelROI/"+salida + ".txt");

	for(int i = 0; i<tamGrilla*tamGrilla; ++i)
	{
		imagenArchivoROIVector<<"Celda"<<i<<endl;

		for (int j=0; j<tamCelda*tamCelda; ++j)
		{
			if((j)%16==0){
				imagenArchivoROIVector <<endl;
			}

			if(pixelROI[i][j]<10) { imagenArchivoROIVector <<" "<<"00"<<pixelROI[i][j]<<" "; }
			else if(pixelROI[i][j]<100) { imagenArchivoROIVector <<" "<<"0"<<pixelROI[i][j]<<" "; }
			else  {  imagenArchivoROIVector <<" "<<pixelROI[i][j]<<" "; }
		}
		imagenArchivoROIVector<<endl<<endl;
	}
   
	stringstream concatenar2;
	string salida2;
	concatenar2<<"imagenROI"<<"_"<<nombImagen;
	concatenar2>>salida2;

    imagenAchivoROI.open("archivosPixelROI/"+salida2+".txt");
	
	for(int i = 0; i<imagenEntrada->height; ++i)
	{
		if((i+1)%17==0){
			imagenAchivoROI <<endl<<endl;
		}

		for (int j=0; j<imagenEntrada->width; ++j)
		{
			if((j+1)%17==0){
				imagenAchivoROI <<"      ";
			}
			int v = CV_IMAGE_ELEM(imagenEntrada, unsigned char, i, j);
			if(v<10) { imagenAchivoROI <<" "<<"00"<<v<<" "; }
			else if(v<100) { imagenAchivoROI <<" "<<"0"<<v<<" "; }
			else  {  imagenAchivoROI <<" "<<v<<" "; }
		}
		imagenAchivoROI<<endl;
	}


}

void Imagen::EstablecerTamanioImagen()
{
	tamanio=cvGetSize(imagen);
}

void Imagen::MostrarTodasImagenes()
{
	// Muestra la Original
	cvShowImage("example_img", imagen);

	// Muestra la Imagen Procesada
	cvShowImage("example_smoothimg", imagenSuavizada);
	cvShowImage("example_binimg", imagenBinarizada);
	cvShowImage("example_imagenDilatacionErosion", imagenDilatacionErosion);
	cvShowImage("example_Contours", imagenContorno );
	cvShowImage("example_imagenContornoCompl", imagenContornoCompl );
	cvShowImage("example_Dedos", imagenDeteccionDedos );
	cvShowImage("example_imagenPuntosCentroide", imagenPuntosCentroide );
	cvShowImage("example_imagenGirada", imagenGirada );
	cvShowImage("example_imagenPuntosCentroideNew", imagenPuntosCentroideNew );

	cvShowImage("example_imagenAjustada", imagenAjustada );
	cvShowImage("example_imagenPuntoMedioROI", imagenPuntoMedioROI );
	cvShowImage("example_imagenGiradaNuevo", imagenGiradaNuevo );
	cvShowImage("example_imagenTrazaCuadradoROI", imagenTrazaCuadradoROI );
	cvShowImage("example_imagenROI", imagenROI );
	cvShowImage("example_imagenSobelROI", imagenSobelROI);

	cvWaitKey(0); // Espera un presionada de tecla
}

IplImage* Imagen::RetornarimagenPuntosCentroideNew()
{
	return imagenPuntosCentroideNew;
}

IplImage* Imagen::RetornarimagenDeteccionDedos()
{
	return imagenDeteccionDedos;
}

IplImage* Imagen::RetornarimagenSobelROI()
{
	return 	imagenTrazaCuadradoROI;
}

vector<Point> Imagen::RetornarpuntoROI()
{
	return puntoROI;
}

vector<float> Imagen::RetornarvectorCaract()
{
	return vectorCaract;
}