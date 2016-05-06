
/*****************************************************************
*
*    SCC0251 - Processamento de Imagem
*    Tarefa 6 - Linhas
*
*    -Rodrigo das Neves Bernardi - 8066395
*
******************************************************************/

//#include "pdi.hpp"

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/**
 * Funcao Main
 * 
 * @param  argc     Numero de argumentos
 * @param  argv     Nome do arquivo de imagem de entrada e comando
 * @return          0
 */
int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        cout << "usage: DisplayImage.out <Image_Path>\n" << endl;
        return -1;
    }
    Mat image;
    image = imread( argv[1], 1 );
    if ( !image.data )
    {
        cout << "No image data \n" << endl;
        return -1;
    }
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);
    waitKey(0);
    return 0;
}