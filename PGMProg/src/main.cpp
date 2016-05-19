
/*****************************************************************
*
*    SCC0251 - Processamento de Imagem
*    Projeto Final - CBIR (Content Based Image Retrieval)
* 
*    -Gabriel Estrela             -  xxxxxxx
*    -Rodrigo das Neves Bernardi  -  8066395
*
******************************************************************/

#include "pdi.hpp"


int main( int argc, char** argv )
{
    Mat imgIn, imgOut, imgCanny, imgGray, imgBlurred;
    vector<Vec2f> lines;

    if( argc <= 4 )
    {
        cout << " Usage: ./corel10k <image path> <k-similar images> <method> [other args] " << endl;
        return -1;
    }

    imgIn = imread( argv[1], 1 );
    if( !imgIn.data )
    {
        cout << " No image data. " << endl;
        return -1;
    }

    cvtColor( imgIn, imgGray, CV_BGR2GRAY );
    GaussianBlur( imgGray, imgBlurred, Size(3,3), 0, 0, BORDER_REFLECT );
    Canny( imgBlurred, imgCanny, 50, 150, 3 );

    generateLines( imgIn, lines );

    imshow( "ChessBoard Processed ", imgIn );
    imshow( "Canny Filter", imgCanny );
    waitKey(0);

    imgOut = imwrite( argv[2], imgIn );
    cout << " Image saved. " << endl;

    return 0;
}