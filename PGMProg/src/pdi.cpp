
/*****************************************************************
*
*    SCC0251 - Processamento de Imagem
*    Arquivo pdi.cpp - Funcoes implementadas
*
*    -Rodrigo das Neves Bernardi - 8066395
*
******************************************************************/

#include "pdi.hpp"


/**
 * This function allows the user to paint lines on a specific image 
 * using hough transform
 * 
 * @param image [description]
 * @param lines [description]
 */
void generateLines( Mat image, vector<Vec2f> lines )
{
    float radius, t_angle; 
    double x0, y0, x1, y1, x2, y2, a, b, angle;
    vector<Point> H_points, V_points;
    vector<Point> B, G, R;

    for( unsigned int i = 0; i < lines.size(); i++ )
    {
        radius = lines[i][0];
        t_angle = lines[i][1];

        a = cos(t_angle);
        b = sin(t_angle);

        x0 = radius * a;
        y0 = radius * b;

        x1 = cvRound( x0 + 1000 * (-b) );
        y1 = cvRound( y0 + 1000 * (a) );

        x2 = cvRound( x0 - 1000 * (-b) );
        y2 = cvRound( y0 - 1000 * (a) );

        Point point_1( cvRound( x0 + 1000 * (-b) ), cvRound( y0 + 1000 * (a) ) );
        Point point_2( cvRound( x0 - 1000 * (-b) ), cvRound( y0 - 1000 * (a) ) );

        angle = atan2(y2 - y1, x2 - x1) * 180.0 / CV_PI;
        if( angle == 0 )
        {
            H_points.push_back( point_1 );
            H_points.push_back( point_2 );
        }

        if( (angle > 70 || angle < -60) ) 
        {
            V_points.push_back( point_1 );
            V_points.push_back( point_2 );
        }
    }


    B.push_back( V_points[2] );
    B.push_back( V_points[3] );
    B.push_back( V_points[8] );
    B.push_back( V_points[9] );
    B.push_back( V_points[10] );
    B.push_back( V_points[11] );
    B.push_back( V_points[14] );
    B.push_back( V_points[15] );
    B.push_back( V_points[18] );
    B.push_back( V_points[19] );
    B.push_back( V_points[20] );
    B.push_back( V_points[21] );
    B.push_back( V_points[22] );
    B.push_back( V_points[23] );
    
    B[0].x = B[0].x + 140;
    B[0].y = B[0].y + 1150;
    B[1].x = B[1].x - 59;
    B[1].y = B[1].y - 495;
    line(image, B[0], B[1], Scalar(255, 0, 0), 3, LINE_AA);
    
    B[2].x = B[2].x + 80;
    B[2].y = B[2].y + 1125;
    B[3].x = B[3].x - 39;
    B[3].y = B[3].y - 531;
    line(image, B[2], B[3], Scalar(255, 0, 0), 3, LINE_AA);
    
    B[4].x = B[4].x + 205;
    B[4].y = B[4].y + 1175;
    B[5].x = B[5].x - 80;
    B[5].y = B[5].y - 455;
    line(image, B[4], B[5], Scalar(255, 0, 0), 3, LINE_AA);
    
    B[6].x = B[6].x + 20;
    B[6].y = B[6].y + 1100;
    B[7].x = B[7].x - 10;
    B[7].y = B[7].y - 555;
    line(image, B[6], B[7], Scalar(255, 0, 0), 3, LINE_AA);
    
    B[8].x = B[8].x + 92;
    B[8].y = B[8].y - 590;
    B[9].x = B[9].x - 165;
    B[9].y = B[9].y + 1045;
    line(image, B[8], B[9], Scalar(255, 0, 0), 3, LINE_AA);
    
    B[10].x = B[10].x + 62;
    B[10].y = B[10].y - 590;
    B[11].x = B[11].x - 110;
    B[11].y = B[11].y + 1060;
    line(image, B[10], B[11], Scalar(255, 0, 0), 3, LINE_AA);
    
    B[12].x = B[12].x + 21;
    B[12].y = B[12].y - 574;
    B[13].x = B[13].x - 38;
    B[13].y = B[13].y + 1080;
    line(image, B[12], B[13], Scalar(255, 0, 0), 3, LINE_AA);
 


    G.push_back( H_points[2] );
    G.push_back( H_points[3] );
    G.push_back( H_points[6] );
    G.push_back( H_points[7] );
    G.push_back( H_points[8] );
    G.push_back( H_points[9] );
    G.push_back( H_points[10] );
    G.push_back( H_points[11] );
    G.push_back( H_points[12] );
    G.push_back( H_points[13] );
    G.push_back( H_points[14] );
    G.push_back( H_points[15] );
    G.push_back( H_points[16] );
    G.push_back( H_points[17] );

    G[0].x = G[0].x + 1150;
    G[1].x = G[1].x - 360;
    line(image, G[0], G[1], Scalar(0, 255, 0), 3, LINE_AA);
    
    G[2].x = G[2].x + 1135;
    G[3].x = G[3].x - 340;
    line(image, G[2], G[3], Scalar(0, 255, 0), 3, LINE_AA);

    G[4].x = G[4].x + 1155;
    G[5].x = G[5].x - 370;
    line(image, G[4], G[5], Scalar(0, 255, 0), 3, LINE_AA);
    
    G[6].x = G[6].x + 1125;
    G[7].x = G[7].x - 330;
    line(image, G[6], G[7], Scalar(0, 255, 0), 3, LINE_AA);
    
    G[8].x = G[8].x + 1140;
    G[9].x = G[9].x - 350;
    line(image, G[8], G[9], Scalar(0, 255, 0), 3, LINE_AA);
    
    G[10].x = G[10].x + 1115;
    G[11].x = G[11].x - 325;
    line(image, G[10], G[11], Scalar(0, 255, 0), 3, LINE_AA);

    G[12].x = G[12].x + 1105;
    G[13].x = G[13].x - 310;
    line(image, G[12], G[13], Scalar(0, 255, 0), 3, LINE_AA);
    


    R.push_back( H_points[0] );
    R.push_back( H_points[1] );
    R.push_back( H_points[4] );
    R.push_back( H_points[5] );
    R.push_back( H_points[18] );
    R.push_back( H_points[19] );
    R.push_back( H_points[22] );
    R.push_back( H_points[23] );
    R.push_back( V_points[0] );
    R.push_back( V_points[1] );
    R.push_back( V_points[4] );
    R.push_back( V_points[5] );
    R.push_back( V_points[12] );
    R.push_back( V_points[13] );
    R.push_back( V_points[44] );
    R.push_back( V_points[45] );

    R[0].x = R[0].x + 1020;
    R[1].x = R[1].x - 218;
    line(image, R[0], R[1], Scalar(0, 0, 255), 3, LINE_AA);
    
    R[2].x = R[2].x + 1090;
    R[3].x = R[3].x - 295;
    line(image, R[2], R[3], Scalar(0, 0, 255), 3, LINE_AA);
    
    R[4].x = R[4].x + 1125;
    R[5].x = R[5].x - 330;
    line(image, R[4], R[5], Scalar(0, 0, 255), 3, LINE_AA);

    R[6].x = R[6].x + 1165;
    R[7].x = R[7].x - 375;
    line(image, R[6], R[7], Scalar(0, 0, 255), 3, LINE_AA);
    
    R[8].x = R[8].x + 122;
    R[8].y = R[8].y - 580;
    R[9].x = R[9].x - 220;
    R[9].y = R[9].y + 1035;
    line(image, R[8], R[9], Scalar(0, 0, 255), 3, LINE_AA);

    R[10].x = R[10].x + 275;
    R[10].y = R[10].y + 1200;
    R[11].x = R[11].x - 95;
    R[11].y = R[11].y - 405;
    line(image, R[10], R[11], Scalar(0, 0, 255), 3, LINE_AA);
    
    R[12].x = R[12].x + 130;
    R[12].y = R[12].y - 522;
    R[13].x = R[13].x - 250;
    R[13].y = R[13].y + 1005;
    line(image, R[12], R[13], Scalar(0, 0, 255), 3, LINE_AA);

    R[14].x = R[14].x + 313;
    R[14].y = R[14].y + 1200;
    R[15].x = R[15].x - 92;
    R[15].y = R[15].y - 320;
    line(image, R[14], R[15], Scalar(0, 0, 255), 3, LINE_AA);
}