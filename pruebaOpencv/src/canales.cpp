/*
 * Autores
 * Guillermo Vicente González
 * Jorge Prieto de la Cruz
 *
 * Breve explicacion:
 * tiene varios pasos. Como el cuadrado es rojo lo primero que hacemos es aislar el canal rojo.
 * Despues para limpiar la imagen hacemos umbralizacion (con valor cercano a 255 ya que el cuadrado es rojo)
 * la imagen umbralizada deberia estar limpia, ahi detectamos formas y obtenemos las esquinas del cuadrado
 * Con la posicion de las esquinas comparamos la posicion de los lados con la mitad de la pantalla
 * Segun su posicion respecto a la mitad de la pantalla indicamos en un string como habria que moverse.
 *
 */


#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

using namespace std;
using namespace cv;

int main() {
	String instruccion = " ";
	Mat imagen;
	Mat imagenCanales;
	Mat imagenThreshold;
	Mat imagenContornos;
	int xMax, xMin, yMax, yMin;

	namedWindow("Original",WINDOW_NORMAL);
	namedWindow("Canal R",WINDOW_NORMAL);
	namedWindow("ImagenThreshold",WINDOW_NORMAL);

	Scalar colorRojo(255,0,0);

	imagen = imread("/home/guillermo/eclipse-workspace/pruebaOpencv/src/cuadro.png");
	imshow("Original",imagen);

	/***********************************************************
	 * me quedo con el canal rojo y lo filtro con un threshold
	 ******************************************************************/
	vector<Mat> canales;
	split(imagen,canales);
	imshow("Canal R", canales[0]);
	threshold(canales[0],imagenThreshold,200,255,THRESH_BINARY);
	imshow("ImagenThreshold",imagenThreshold);

	/**********************************************************************
	 * ahora trato de reconocer la forma del cuadro
	 *********************************************************************/
	vector<vector<Point>> contornos;
	findContours(imagenThreshold,contornos, RETR_CCOMP  ,CHAIN_APPROX_SIMPLE);
	if((int) contornos[0].size() != 4){
		cout << "no es un cuadrado, deteccion erronea" << endl;
		return 0;
	}

	/*************************************************************************
	 * Conociendo la silueta y las dimensiones de la imagen
	 * puedo determinar la posicion relativa del cuadro y dar
	 * instrucciones al robot
	 *************************************************************************/

	//debug con muchas comillas
	cout <<"estructura de la imagen:" << imagen.size << endl;
	cout <<"numero de puntos del contorno:" << contornos.size() << endl;


	yMin = 10000000;
	xMin = yMin;
	yMax = 0;
	xMax = yMax;

	cout << contornos[0] << endl;


	for(int i=0 ; i < (int)contornos[0].size(); i++){
		if(contornos[0][i].x < xMin){
			xMin = contornos[0][i].x;
		}
		if(contornos[0][i].x > xMax){
			xMax = contornos[0][i].x;
		}

		if(contornos[0][i].y < yMin){
			yMin = contornos[0][i].y;
		}
		if(contornos[0][i].y > yMax){
			yMax = contornos[0][i].y;
		}
	}

	/**********************************************************
	 * Aqui se responde al robot
	 **********************************************************/

	if(xMin > imagen.cols/2){
		//si el lado izquierdo del cuadro esta mas alla de la mitad de la imagen
		instruccion += "mueve a la izquierda. ";
	}else if(xMax < imagen.cols/2){
		instruccion += "mueve a la derecha. ";
	}else{
		instruccion += "no hace falta mover a la derecha.";
	}

	//recuerda que la y va al reves en opencv
	if(yMax < imagen.rows/2){
		instruccion += " mueve hacia abajo.";
	}else if(yMin > imagen.rows/2){
		instruccion += " mueve hacia arriba.";
	}else{
		instruccion += "no hace falta mover hacia arriba.";
	}

	/*********************************************************************
	 * Una vez el cuadrado esta centrado vemos si es necesario que avance.
	 *********************************************************************/


	cout << instruccion << endl;

	waitKey(0);
	return 0;
}
