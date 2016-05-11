#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <array>

using namespace cv;
using namespace std;


int main( int argc, char** argv )
{
  Mat image, grayImg, blurredImg, cannyImg, linesImg;
  image = imread( argv[1], 1);
  //vector< pair<double, double> > lines;
  vector<Vec2f> lines;

  double point[2];


  if( argc != 2 || !image.data )
    {
      printf( "No image data \n" );
      return -1;
    }

  cvtColor(image, grayImg, CV_BGR2GRAY); 

  //GaussianBlur(grayImg, blurredImg, Size(3,3), 0, 0, BORDER_DEFAULT); 
  blur(grayImg, blurredImg, Size(5,5));

  Canny(blurredImg, cannyImg, 50, 150, 3);

  HoughLines(cannyImg, lines, 1, CV_PI/180, 100);

  size_t i;
  for( i = 0; i < lines.size(); i++){
        float rho = lines[i][0];
        float theta = lines[i][1];
        double a = cos(theta);
        double b = sin(theta);
        double x0 = a * rho;
        double y0 = b * rho;
        double x1 = cvRound(x0 + 1000 * (-b));
        double y1 = cvRound(y0 + 1000 * (a));
        double x2 = cvRound(x0 - 1000 * (-b));
        double y2 = cvRound(y0 - 1000 * (a));
        Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
        Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
        double Angle = atan2(y2 - y1, x2 - x1) * 180.0 / CV_PI;
        if(Angle > -1.1 && Angle <= 0){
          line(image, pt1, pt2, Scalar(0, 255, 0), 3, 8);
        }else{
          line(image, pt1, pt2, Scalar(255, 0, 0), 3, 8);
        }
        
    //  if(i == 5 || (i >= 11 && i < lines.size())){
    //     float rho = lines[i][0];
    //     float theta = lines[i][1];
    //     double a = cos(theta);
    //     double b = sin(theta);
    //     double x0 = a * rho;
    //     double y0 = b * rho;
    //     Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
    //     Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
    //     line(image, pt1, pt2, Scalar(255, 0, 0), 3, 8);
    // }

    // if(i != 5 && i < 11){
    //   //todas horizontais neste intervalo
    //   float rho = lines[i][0];
    //   float theta = lines[i][1];
    //   double a = cos(theta);
    //   double b = sin(theta);
    //   double x0 = a * rho;
    //   double y0 = b * rho;
    //   Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
    //   Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
    //   line(image, pt1, pt2, Scalar(0, 255, 0), 3, 8);
    // }

  }



  namedWindow( "Display Image", WINDOW_AUTOSIZE );
  imshow( "original", image);
  //imshow( "gray", grayImg);
  imshow( "Canny", cannyImg);
  //imshow( "blurredImg", blurredImg);

  waitKey(0);

  return 0;
}