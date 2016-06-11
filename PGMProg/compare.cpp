#include <opencv2/opencv.hpp>
#include "opencv2/xfeatures2d.hpp"
// #include <opencv2/opencv.hpp>
// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/features2d/features2d.hpp>
// #include <opencv2/nonfree/nonfree.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdio>
#include <ctime>

#define BD_SIZE 10000;

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

Mat READ_ONE_SIFT_XML(string pathToFile, int pos);
vector<Mat> READ_ALL_SIFT_XML();
vector<vector<Point3i> > READ_ALL_FOURIER();
Mat GEN_SIFT_DESCRIPTOR(Mat consultImg);
vector<Point3i> GEN_FOURIER_DESCRIPTOR(Mat consultImg);
vector<string> GET_K_IMAGES();


int main( int argc, char** argv ){

	clock_t begin, end;
	double elapsed_secs;

	if( argc < 4 || argc > 6 ) {
		cout << "\nAlgo foi digitado errado!\n";
		cout << "\nUsage: $ ./corel10k <imagem de consulta> <k-proximos> <metodo> [parametros adicionais do metodo]" << "\n\n";
		return -1;
	}

	string pathToImage = "Pictures_GHIM20/";
	string imagemConsulta = argv[1];
	pathToImage += imagemConsulta;
	int kaProximos = stoi(argv[2], NULL, 10);
	string metodo = argv[3];

	if( metodo.compare("sift") == 0 || metodo.compare("SIFT") == 0 ) {
		//GERAR VARIAVEIS PARA OS PARAM. DE SIFT
		cout << "You choosed SIFT!" << endl;
	}
	if( metodo.compare("Fourier") == 0 || metodo.compare("fourier") == 0 || metodo.compare("FOURIER") == 0 ) {
		//GERAR VARIAVEIS PARA OS PARAM. DE FOURIER
		cout << "You choosed FOURIER!" << endl;
	}

	waitKey(0);

	return 0;
}

Mat READ_ONE_SIFT_XML( string pathToFile, int pos ) {
	Mat descriptor;
	string picture = "Descriptor_";
	picture += to_string(pos);
	cout << picture << endl;
	FileStorage fs(pathToFile, FileStorage::READ);
	fs[picture] >> descriptor;
	return descriptor;
}

vector<Mat> READ_ALL_SIFT_XML() {

	int i;
	string position;
	vector<Mat> descriptors;
	Mat temp;
	FileStorage fs("SIFT.XML", FileStorage::READ);
	FileStorage fs2("SIFT2.XML", FileStorage::READ);

	for(i = 1; i <= 5000; i++) {
		position = "Descriptor_";
		position += to_string(i);
		//cout << "\rReading: " << position << endl;
		fs[position] >> temp;
		descriptors.push_back(temp);
		position.clear();
		temp.release();
	}

	for(i = 5001; i <= 10000; i++) {
		position = "Descriptor_";
		position += to_string(i);
		//cout << "\rReading: " << position << endl;
		fs2[position] >> temp;
		descriptors.push_back(temp);
		position.clear();
		temp.release();
	}

	return descriptors;
}

vector< vector<Point3i> > READ_ALL_FOURIER() {
	int i, j, x, y, value;
	fstream file;
	vector<Point3i> temp;
	vector<vector<Point3i> > fourier_descriptors;

	file.open( "FOURIER.txt", fstream::in | fstream::out );

	if(file.is_open()) {
		for(i = 0; i < BD_SIZE; i++) {
			for(j = 0; j < 9; j++) {
				file >> x >> y >> value;
				temp.push_back(Point3i(x, y, value));
			}

			fourier_descriptors.push_back(temp);
			temp.clear();
		}
	}else{
		cout << "\n\nCould not open the FOURIER.TXT file on `READ_ALL_FOURIER` function!" << endl;
		return fourier_descriptors;
	}

	return fourier_descriptors;
}

Mat GEN_SIFT_DESCRIPTOR(Mat consultImg) {
	Mat descriptor;
	vector<KeyPoint> keypoints;

	Ptr<Feature2D> f2d = xfeatures2d::SIFT::create(0, 3, 0.22, 10, 1.6);

	f2d->detect(consultImg, keypoints);
	f2d->compute(consultImg, keypoints, descriptor);

	return descriptor;
}

vector<Point3i> GEN_FOURIER_DESCRIPTOR(Mat consultImg) {
	int optimalHeight, optimalWidth, cx, cy, x, y;
	Mat padded, complexI, magI, tmp;
	Mat fourier;
	vector<Point3i> img_fourier_descriptor;

	optimalHeight = getOptimalDFTSize(consultImg.rows);
	optimalWidth = getOptimalDFTSize(consultImg.cols);
	copyMakeBorder(consultImg, padded, 0, optimalHeight - consultImg.rows, 0, optimalWidth - consultImg.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	merge(planes, 2, complexI);
	dft(complexI, complexI);

	split(complexI, planes);
	magnitude(planes[0], planes[1], planes[0]);
	magI = planes[0];

	magI += Scalar::all(1);
	log(magI, magI);

	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
	cx = magI.cols/2;
	cy = magI.rows/2;

	Mat q0(magI, Rect(0, 0, cx, cy));
	Mat q1(magI, Rect(cx, 0, cx, cy));
	Mat q2(magI, Rect(0, cy, cx, cy));
	Mat q3(magI, Rect(cx, cy, cx, cy));

	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(magI, magI, 0, 1, NORM_MINMAX);
	magI.convertTo(fourier, CV_8U, 255, 0);

	cx = fourier.rows/2;
	cy = fourier.cols/2;

	img_fourier_descriptor.push_back(Point3i(cx, cy, int(fourier.at<uchar>(cx, cy))));

	x = cx - 30;

	img_fourier_descriptor.push_back(Point3i(x, cy, int(fourier.at<uchar>(x, cy))));

	y = cy - 30;

	img_fourier_descriptor.push_back(Point3i(cx, y, int(fourier.at<uchar>(cx, y))));

	x = cx - 60;

	img_fourier_descriptor.push_back(Point3i(x, cy, int(fourier.at<uchar>(x, cy))));

	y = cy - 60;

	img_fourier_descriptor.push_back(Point3i(cx, y, int(fourier.at<uchar>(cx, y))));

	x = cx + 30;

	img_fourier_descriptor.push_back(Point3i(x, cy, int(fourier.at<uchar>(x, cy))));		

	y = cy + 30;

	img_fourier_descriptor.push_back(Point3i(cx, y, int(fourier.at<uchar>(cx, y))));

	x = cx + 60;

	img_fourier_descriptor.push_back(Point3i(x, cy, int(fourier.at<uchar>(x, cy))));

	y = cy + 60;

	img_fourier_descriptor.push_back(Point3i(cx, y, int(fourier.at<uchar>(cx, y))));

	return img_fourier_descriptor;
}

vector<string> GET_K_IMAGES( Mat img, string imgPath, int k )
{
	Mat imgDescriptor = GEN_SIFT_DESCRIPTOR(img);
	vector<Mat> descriptors = READ_ALL_SIFT_XML();

	FlannBasedMatcher matcher;
	vector< vector<DMatch> > matches;
	
	for ( int i = 0; i < BD_SIZE; ++i )
	{
		vector<DMatches> temp;

		if( i ==  )
		{

		}

		else
		{
			matcher.match( imgDescriptor, descriptors[i], temp );
			matches.push_back(temp);
		}
	}

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for ( int i = 0; i < BD_SIZE; ++i )
	{	
		for( int j = 0; j < descriptors[i].rows; j++ )
		{
			double dist = matches[i][j].distance;
			
			if( dist < min_dist ) min_dist = dist;
			if( dist > max_dist ) max_dist = dist;
		}
	}

	vector< vector<DMatch> > good_matches;
	vector<int> good_matches_idx;

	// for( int i = 0; i < BD_SIZE; ++i )
	// {
	// 	for( int j = 0; j < descriptors[i].rows; j++ )
	// 	{
	// 		//vector<DMatches> temp;

	// 		if( matches[i][j].distance <= max(2*min_dist, 0.02) )
	// 		{
	// 			good_matches.push_back(matches[i][j]);
	// 			good_matches_idx.push_back(i);
	// 		}
	// 	}
	// }

	for (int i = 0; i < matches.size(); ++i)
	{
		for (int j = 0; j < imgDescriptor.rows; ++j)
		{
			if ( matches[i][j].distance <= max(2*min_dist, 0.02) )
			{
				vector<DMatches> temp;

				temp.push_back(matches[i][j]);
			}
		}

		good_matches.push_back(temp);
		good_matches_idx.push_back(i);
	}

	int idx, size = 0;

	for (int i = 0; i < ; ++i)
	{
		if (/* condition */)
		{
			/* code */
		}
	}


	vector<string> s;

	return ;
}