#include <opencv2/opencv.hpp>
#include "opencv2/xfeatures2d.hpp"
// #include <opencv2/opencv.hpp>
// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/features2d/features2d.hpp>
// #include <opencv2/nonfree/nonfree.hpp>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <string>
#include <ctime>

using namespace cv;
using namespace std;

Mat READ_ONE_SIFT_XML(string pathToFile, int pos);
vector<Mat> READ_ALL_SIFT_XML();
Mat GEN_SIFT_DESCRIPTOR(Mat consultImg);

int main( int argc, char** argv ){

	clock_t begin, end;
 	double elapsed_secs;

	if(argc < 4 || argc > 6){
		cout << "\nAlgo foi digitado errado!\n";
		cout << "\n$./corel10k <imagem de consulta> <k-proximos> <metodo> [parametros adicionais do metodo]" << "\n\n";
		return -1;
	}

	string pathToImage = "Pictures_GHIM20/";
	string imagemConsulta = argv[1];
	pathToImage += imagemConsulta;
	int kaProximos = stoi(argv[2], NULL, 10);
	string metodo = argv[3];

	// cout << "pathToImage: " << pathToImage << endl;
	// cout << "kaProximos: " << kaProximos << endl;
	// cout << "metodo: " << metodo << endl;

	if(metodo.compare("sift") == 0 || metodo.compare("SIFT") == 0){
		//GERAR VARIAVEIS PARA OS PARAM. DE SIFT
		cout << "You choosed SIFT!" << endl;
	}
	if(metodo.compare("Fourier") == 0 || metodo.compare("fourier") == 0 || metodo.compare("FOURIER") == 0){
		//GERAR VARIAVEIS PARA OS PARAM. DE FOURIER
		cout << "You choosed FOURIER!" << endl;
	}

	//le a imagem em tons de cinza
	Mat consult = imread(pathToImage, 0);
	imshow("Consulta", consult);

	// Mat consulDescriptor = GEN_SIFT_DESCRIPTOR(consult);
	// imshow("Consult_SIFT", consulDescriptor);

	// begin = clock();
	// vector<Mat> descriptors = READ_ALL_SIFT_XML();
	// end = clock();
	// elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
 //  	cout << "\nTime to read the descriptors: " << elapsed_secs/60 << " minutes." << endl;

 //  	imshow("Descriptor_1", descriptors[0]);

	waitKey(0);

	return 0;
}

Mat READ_ONE_SIFT_XML(string pathToFile, int pos){
  Mat descriptor;
  string picture = "Descriptor_";
  picture += to_string(pos);
  cout << picture << endl;
  FileStorage fs(pathToFile, FileStorage::READ);
  fs[picture] >> descriptor;
  return descriptor;
}

vector<Mat> READ_ALL_SIFT_XML(){

	int i;
	string position;
	vector<Mat> descriptors;
	Mat temp;
	FileStorage fs("SIFT.XML", FileStorage::READ);
	FileStorage fs2("SIFT2.XML", FileStorage::READ);

	for(i = 1; i <= 5000; i++){
		position = "Descriptor_";
		position += to_string(i);
		//cout << "\rReading: " << position << endl;
		fs[position] >> temp;
		descriptors.push_back(temp);
		position.clear();
		temp.release();
	}

	for(i = 5001; i <= 10000; i++){
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

Mat GEN_SIFT_DESCRIPTOR(Mat consultImg){
	Mat descriptor;
	vector<KeyPoint> keypoints;

	Ptr<Feature2D> f2d = xfeatures2d::SIFT::create(0, 3, 0.22, 10, 1.6);

	f2d->detect(consultImg, keypoints);
	f2d->compute(consultImg, keypoints, descriptor);

	return descriptor;
}