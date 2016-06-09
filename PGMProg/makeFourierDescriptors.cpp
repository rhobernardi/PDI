#include <opencv2/opencv.hpp>
#include "opencv2/xfeatures2d.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <cstdio>
#include <ctime>


using namespace cv;
using namespace std;


vector<Mat> READ_IMGS(int quantity);
vector<Mat> GEN_FOURIER_VEC(vector<Mat> imgs);
vector<Mat> CONVERT_0_255(vector<Mat> piu);
vector<vector<Point3i> > GEN_FOURIER_DESCRIPTOR(vector<Mat> fourier_vec);
void WRITE_FOURIER(vector<vector<Point3i> > fourier_descriptors);


int main( int argc, char** argv ){

	vector<Mat> imgs, fourier_vec, converted_fourier;
	vector<vector<Point3i> > fourier_descriptors; 
	clock_t begin, end;
	double elapsed_secs;

  	//READING IMAGES FROM PICTURES FOLDER
	begin = clock();
	imgs = READ_IMGS(10000);
	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "\nTime to read the images: " << elapsed_secs/60 << " minutes.\n" << endl;
 	//READING IMAGES FROM PICTURES FOLDER

 	//GENERATING FOURIER VECTOR
 	begin = clock();
	fourier_vec = GEN_FOURIER_VEC(imgs);
	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "\nTime to generate Fourier Vector: " << elapsed_secs/60 << " minutes.\n" << endl;
 	//GENERATING FOURIER VECTOR

 	//CONVERT TO 0 - 255 RANGE
	begin = clock();
	converted_fourier = CONVERT_0_255(fourier_vec);
	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "\nTime to convert Fourier Vector: " << elapsed_secs/60 << " minutes.\n" << endl;
	//CONVERT TO 0 - 255 RANGE

	//GENERATING DESCRIPTORS
	begin = clock();
	fourier_descriptors = GEN_FOURIER_DESCRIPTOR(converted_fourier);
	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "\nTime to generate Fourier Descriptors: " << elapsed_secs/60 << " minutes.\n" << endl;
	//GENERATING DESCRIPTORS

	//GENERATING FOURIER.TXT
	begin = clock();
	WRITE_FOURIER(fourier_descriptors);
	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "\nTime to generate Fourier Descriptors: " << elapsed_secs/60 << " minutes.\n" << endl;
	//GENERATING FOURIER.TXT

	// cout << "\n\nX: " << fourier_descriptors[0][8].x << "\n";
	// cout << "\n\nY: " << fourier_descriptors[0][8].y << "\n";
	// cout << "\n\nValue: " << fourier_descriptors[0][8].z << "\n";

	imshow("Fourier_1", converted_fourier[0]);

	waitKey(0);

	return 0;
}

//FUNCAO QUE LE AS IMAGENS E RETORNA UM VETOR DELAS
vector<Mat> READ_IMGS(int quantity){

  int i, i2; //iteradores
  string path = "Pictures_GHIM20/";//caminho para imagens (pode ser passado por parametro =p)
  string auxString = "";//str auxiliar para geracao de end
  vector<Mat> images, null;//vector para retorno
  Mat aux;//openCV Mat para auxiliar na criacao do vector de retorno

  if(quantity > 10000){
  	cout << "No way..." << endl; 
  	return null;
  }

  //recebe-se a quantidade de imagens a ser lida
  //gera-se o endereco de cada uma
  //le a imagem e salva em aux
  for(i = 1; i <= quantity; i++){
  	auxString = path + to_string(i) + ".jpg";
  	cout << "\ropening " << auxString; 
  	aux = imread(auxString, 0);

    //verifica se aux possui informacao
    //caso alguma imagem tenha problemas ou nao exista
    //eh criada no lugar uma com a anterior
    //com excessao da primeira que recebera a proxima
  	if(!aux.data){

  		if(i == 1)
  			i2 = i+1;

  		i2 = i-1;
      //cout << "\nNo image data on " << auxString << endl;
  		auxString = path + to_string(i2) + ".jpg";
      //cout << "Changing it to " << auxString << endl;
  		aux = imread(auxString, 0);
  	}

    //por fim coloca a imagem no vetor
  	images.push_back(aux);
  }

  //retorna o vetor
  return images;
}

vector<Mat> GEN_FOURIER_VEC(vector<Mat> imgs){
	
	int i, optimalHeight, optimalWidth, cx, cy;
	Mat padded, complexI, magI, tmp;
	vector<Mat> fourier;

	for(i = 0; i < imgs.size(); i++){
		optimalHeight = getOptimalDFTSize(imgs[i].rows);
		optimalWidth = getOptimalDFTSize(imgs[i].cols);
		copyMakeBorder(imgs[i], padded, 0, optimalHeight - imgs[i].rows, 0, optimalWidth - imgs[i].cols, BORDER_CONSTANT, Scalar::all(0));

		Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
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
		fourier.push_back(magI);
		cout << "\rGenerated: " << i+1 << " Fourier image(s)!"; 
	}

	return fourier;
}

vector<Mat> CONVERT_0_255(vector<Mat> fourier){
	int i, j, k;
	double min, max;
	Mat aux;
	vector<Mat> converted;

	// for(i = 0; i < fourier.size(); i++){
	// 	minMaxLoc(fourier[i], &min, &max);
	// 	cout << min << " | " << max << "\n";
	// 	fourier[i].convertTo(aux, CV_8U, 255.0/(float)(max-min), -255.0/(float)min);
	// 	converted.push_back(aux);
	// }

	for(i = 0; i < fourier.size(); i++){
		fourier[i].convertTo(aux, CV_8U, 255, 0);
		converted.push_back(aux);
		cout << "\rConverted to the interval [0-255]: " << i+1 << " Fourier image(s)!"; 
	}


	cout << int(fourier[0].at<uchar>(50,50));

	// for(i = 0; i < fourier.size(); i++){
	// 	for(j = 0; j < fourier[i].rows; j++){
	// 		for(k = 0; k < fourier[i].cols; k++){
	// 			fourier[i].at<float>(j,k) = fourier[i].at<float>(j,k) * 255;
	// 		}
	// 	}
	// 	converted.push_back(fourier[i]);
	// }

	return converted;
}

vector<vector<Point3i> > GEN_FOURIER_DESCRIPTOR(vector<Mat> fourier_image){
	int i, x = 0, y = 0, r = 0, cx, cy;
	vector<vector<Point3i> > fourier_descriptors;

			//get the value from the center of the image;

	for(i = 0; i < fourier_image.size(); i++){
		vector<Point3i> temp;

		cx = fourier_image[i].rows/2;
		cy = fourier_image[i].cols/2;
		temp.push_back(Point3i(cx, cy, int(fourier_image[i].at<uchar>(cx, cy))));

		x = cx - 30;

		temp.push_back(Point3i(x, cy, int(fourier_image[i].at<uchar>(x, cy))));

		y = cy - 30;

		temp.push_back(Point3i(cx, y, int(fourier_image[i].at<uchar>(cx, y))));

		x = cx - 60;

		temp.push_back(Point3i(x, cy, int(fourier_image[i].at<uchar>(x, cy))));

		y = cy - 60;

		temp.push_back(Point3i(cx, y, int(fourier_image[i].at<uchar>(cx, y))));

		x = cx + 30;

		temp.push_back(Point3i(x, cy, int(fourier_image[i].at<uchar>(x, cy))));		

		y = cy + 30;

		temp.push_back(Point3i(cx, y, int(fourier_image[i].at<uchar>(cx, y))));

		x = cx + 60;

		temp.push_back(Point3i(x, cy, int(fourier_image[i].at<uchar>(x, cy))));

		y = cy + 60;

		temp.push_back(Point3i(cx, y, int(fourier_image[i].at<uchar>(cx, y))));

		fourier_descriptors.push_back(temp);
		cout << "\rGenerated: " << i+1 << " Fourier Descriptor(s)!"; 
	}

	return fourier_descriptors;
}

void WRITE_FOURIER(vector<vector<Point3i> > fourier_descriptors){
	int i, j;
	fstream file;

	file.open("FOURIER.txt", fstream::in | fstream::out | fstream::app);

	if(file.is_open()){
		for(i = 0 ; i < fourier_descriptors.size(); i++){
			for(j = 0; j < fourier_descriptors[i].size(); j++){
				file << fourier_descriptors[i][j].x << " " << fourier_descriptors[i][j].y << " " << fourier_descriptors[i][j].z << " ";
			}
		file << "\n";
		}
	}

	return;
}
