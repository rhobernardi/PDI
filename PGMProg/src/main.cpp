
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
#include <vector>
#include <array>

#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;


int main( int argc, char** argv )
{
    Mat imgIn, imgGray, ImgBlurred, ImgCanny, ImgLines;
    imgIn = imread( argv[1], 1);
    
    //vector< pair<double, double> > lines;
    vector<Vec2f> lines;

    double point[2];

    if( argc != 2 || !imgIn.data )
    {
        printf( "No imgIn data \n" );
        return -1;
    }

    cvtColor( imgIn, imgGray, CV_BGR2GRAY );

    GaussianBlur(imgGray, imgBlurred, Size(3,3), 0, 0, BORDER_REFLECT); 
    //blur( imgGray, imgBlurred, Size(5,5) );

    Canny( imgBlurred, imgCanny, 50, 150, 3 );

    HoughLines( imgCanny, lines, 1, CV_PI/180, 100 );

    size_t i;
    for( i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0];
        float theta = lines[i][1];
        double a = cos(theta);
        double b = sin(theta);
        double x0 = a * rho;
        double y0 = b * rho;
        double x1 = cvRound( x0 + 1000 * (-b) );
        double y1 = cvRound( y0 + 1000 * (a) );
        double x2 = cvRound( x0 - 1000 * (-b));
        double y2 = cvRound( y0 - 1000 * (a));

        Point pt1( cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)) );
        Point pt2( cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)) );

        double Angle = atan2( y2 - y1, x2 - x1 ) * 180.0 / CV_PI;
    
        if( Angle == 0 )
        {
            line( imgIn, pt1, pt2, Scalar(0, 255, 0), 3, 8 );
        }
        
        else //if(  ) 
            line( imgIn, pt1, pt2, Scalar(255, 0, 0), 3, 8 );

        

    }

    red.push_back( Hpoints[0] );
    red.push_back( Hpoints[1] );
    red[].x += +100;
    red[].y += -100;
    line( imgIn, pt1, pt2, Scalar(255, 0, 0), 3, 8 );

    //namedWindow( "Display imgIn", WINDOW_AUTOSIZE );
    imshow( "original", imgIn );
    //imshow( "gray", grayImg );
    //imshow( "Canny", imgCanny );
    //imshow( "imgBlurred", imgBlurred );

    waitKey( 0 );

    return 0;

}