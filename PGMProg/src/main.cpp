
/*****************************************************************
*
*    SCC0251 - Processamento de Imagem
*    Tarefa 6 - Linhas
*
*    -Rodrigo das Neves Bernardi - 8066395
*
******************************************************************/

#include "pdi.hpp"


int main( int argc, char** argv )
{
    Mat imgIn, imgOut, imgCanny, imgGray, imgBlurred;
    vector<Vec2f> lines;

    if( argc != 3 )
        cout << " Usage: ./DisplayLines <image_in_path> <image_out_path> " << endl;

    imgIn = imread( argv[1], 1 );
    if( !imgIn.data )
    {
        cout << " No image data. " << endl;
        return -1;
    }

    cvtColor( imgIn, imgGray, CV_BGR2GRAY );
    GaussianBlur( imgGray, imgBlurred, Size(3,3), 0, 0, BORDER_REFLECT );
    Canny( imgBlurred, imgCanny, 50, 150, 3 );
    HoughLines( imgCanny, lines, 1, CV_PI/180, 100 );

    generateLines( imgIn, lines );

    imshow( "ChessBoard Processed", imgIn );
    waitKey(0);

    imgOut = imwrite( argv[2], imgIn );
    cout << " Image saved. " << endl;

    return 0;
}