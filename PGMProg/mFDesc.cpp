#include <opencv2/opencv.hpp>
#include "opencv2/xfeatures2d.hpp"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <string>
#include <ctime>

using namespace cv;
using namespace std;

vector<Mat> READ_IMGS(int quantity);
vector<Mat> GEN_FOURIER_TRANSFORM_VEC(vector<Mat> imgs);
vector<Mat> CONVERT_0_255(vector<Mat> piu);
vector<int> GEN_FOURIER_DESCRIPTOR(Mat consultImg);
vector<vector<int> > ASSEMBLE_FOURIER_DESCRIPTOR(vector<Mat> imgs);
vector<vector<int> > GEN_F_FOURIER_DESCRIPTOR(vector<Mat> fourier_vec);
void WRITE_FOURIER(vector<vector<int> > fourier_descriptors);

int main( int argc, char** argv ){

	vector<Mat> imgs, fourier_vec, converted_fourier;
	vector<vector<int> > fourier_descriptors; 
	clock_t begin, end;
	double elapsed_secs;

  	//READING IMAGES FROM PICTURES FOLDER
	begin = clock();
	imgs = READ_IMGS(10000);
	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "\nTime to read the images: " << elapsed_secs/60 << " minutes.\n" << endl;
 	//READING IMAGES FROM PICTURES FOLDER

 	//CREATING FOURIER DESCRIPTORS
 	begin = clock();
 	fourier_descriptors = ASSEMBLE_FOURIER_DESCRIPTOR(imgs);
 	end = clock();
 	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
 	cout << "\nTime to generate Fourier Descriptors: " << elapsed_secs/60 << " minutes.\n" << endl;
 	//CREATING FOURIER DESCRIPTORS

 	//GENERATING FOURIER VECTOR
 // 	begin = clock();
	// fourier_vec = GEN_FOURIER_TRANSFORM_VEC(imgs);
	// end = clock();
	// elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	// cout << "\nTime to generate Fourier Vector: " << elapsed_secs/60 << " minutes.\n" << endl;
 	//GENERATING FOURIER VECTOR

 	//CONVERT TO 0 - 255 RANGE
	// begin = clock();
	// converted_fourier = CONVERT_0_255(fourier_vec);
	// end = clock();
	// elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	// cout << "\nTime to convert Fourier Vector: " << elapsed_secs/60 << " minutes.\n" << endl;
	//CONVERT TO 0 - 255 RANGE

	//GENERATING DESCRIPTORS
	// begin = clock();
	// fourier_descriptors = GEN_F_FOURIER_DESCRIPTOR(fourier_vec);
	// end = clock();
	// elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	// cout << "\nTime to generate Fourier Descriptors: " << elapsed_secs/60 << " minutes.\n" << endl;
	//GENERATING DESCRIPTORS

	//GENERATING FOURIER.TXT
	begin = clock();
	WRITE_FOURIER(fourier_descriptors);
	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "\nTime to generate Fourier Descriptors File: " << elapsed_secs/60 << " minutes.\n" << endl;
	//GENERATING FOURIER.TXT

	// cout << "\n\nX: " << fourier_descriptors[0][8].x << "\n";
	// cout << "\n\nY: " << fourier_descriptors[0][8].y << "\n";
	// cout << "\n\nValue: " << fourier_descriptors[0][8].z << "\n";

	// imshow("Fourier_1", converted_fourier[0]);
	// imshow("Fourier_2", converted_fourier[500]);
	// imshow("Fourier_3", converted_fourier[1000]);
	// imshow("Fourier_4", converted_fourier[1500]);
	// imshow("Fourier_5", converted_fourier[2000]);
	// imshow("Fourier_6", converted_fourier[2500]);
	// imshow("Fourier_7", converted_fourier[3000]);
	// imshow("Fourier_8", converted_fourier[3500]);
	// imshow("Fourier_9", converted_fourier[4000]);
	// imshow("Fourier_10", converted_fourier[4500]);
	// imshow("Fourier_11", converted_fourier[5000]);
	// imshow("Fourier_12", converted_fourier[5500]);
	// imshow("Fourier_13", converted_fourier[6000]);
	// imshow("Fourier_14", converted_fourier[6500]);
	// imshow("Fourier_15", converted_fourier[7000]);
	// imshow("Fourier_16", converted_fourier[7500]);
	// imshow("Fourier_17", converted_fourier[8000]);
	// imshow("Fourier_18", converted_fourier[8500]);
	// imshow("Fourier_19", converted_fourier[9000]);
	// imshow("Fourier_20", converted_fourier[9500]);

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


vector<Mat> GEN_FOURIER_TRANSFORM_VEC(vector<Mat> imgs){
	
	int i, optimalHeight, optimalWidth, cx, cy;
	Mat padded, complexI, tmp, temp;
	vector<Mat> fourier;

	for(i = 0; i < imgs.size(); i++){
		Mat magI;
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
		magI.convertTo(temp, CV_8U, 255, 0);
		fourier.push_back(temp);
		cout << "\rGenerated: " << i+1 << " Fourier image(s)!"; 
		magI.release();
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


	//cout << int(fourier[0].at<uchar>(50,50));

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

vector<vector<int> > GEN_F_FOURIER_DESCRIPTOR(vector<Mat> fourier_vec){
	int i, j, k,  x = 0, y = 0, cx, cy;
	vector<vector<int> > fourier_descriptors;

	for(i = 0; i < fourier_vec.size(); i++){
		vector<int> temp;
		cy = fourier_vec[i].rows/2;
		cx = fourier_vec[i].cols/2;

		// if(i == 199){
		// 	cout << "Rows: " << fourier_vec[i].rows << " Cols: " << fourier_vec[i].cols << endl;
		// }

		for(j = cy - 10; j < cy + 10	; j++){
			for(k = cx - 10; k < cx + 10; k++){
				temp.push_back( int(fourier_vec[i].at<uchar>(j,k)) );
			}
		}
		//cout << "\rTemp Size: " << temp.size() << endl;
		fourier_descriptors.push_back(temp);
		cout << "\rGenerated: " << i+1 << " Fourier Descriptor(s)!"; 
	}

	return fourier_descriptors;
}

vector<int> GEN_FOURIER_DESCRIPTOR(Mat consultImg) {
	int optimalHeight, optimalWidth, cx, cy, i, j;
	Mat padded, complexI, magI, tmp;
	Mat fourier;
	vector<int> img_fourier_descriptor;

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

	cy = fourier.rows/2;
	cx = fourier.cols/2;

	//cout << "Rows: " << fourier.rows << " Cols: " << fourier.cols << endl;

	for(i = cy - 10; i < cy + 10; i++){
		for(j = cx - 10; j < cx + 10; j++){
			img_fourier_descriptor.push_back(int(fourier.at<uchar>(i,j)));
		}
	}

	// img_fourier_descriptor.push_back(Point3i(cx, cy, int(fourier.at<uchar>(cx, cy))));

	// x = cx - 30;

	// img_fourier_descriptor.push_back(Point3i(x, cy, int(fourier.at<uchar>(x, cy))));

	// y = cy - 30;

	// img_fourier_descriptor.push_back(Point3i(cx, y, int(fourier.at<uchar>(cx, y))));

	// x = cx - 60;

	// img_fourier_descriptor.push_back(Point3i(x, cy, int(fourier.at<uchar>(x, cy))));

	// y = cy - 60;

	// img_fourier_descriptor.push_back(Point3i(cx, y, int(fourier.at<uchar>(cx, y))));

	// x = cx + 30;

	// img_fourier_descriptor.push_back(Point3i(x, cy, int(fourier.at<uchar>(x, cy))));		

	// y = cy + 30;

	// img_fourier_descriptor.push_back(Point3i(cx, y, int(fourier.at<uchar>(cx, y))));

	// x = cx + 60;

	// img_fourier_descriptor.push_back(Point3i(x, cy, int(fourier.at<uchar>(x, cy))));

	// y = cy + 60;

	// img_fourier_descriptor.push_back(Point3i(cx, y, int(fourier.at<uchar>(cx, y))));

	return img_fourier_descriptor;
}

vector<vector<int> > ASSEMBLE_FOURIER_DESCRIPTOR(vector<Mat> imgs){

	int i;
	vector<int> temp;
	vector<vector<int> > fourier_descriptors;

	for(i = 0; i < imgs.size(); i++){
		temp = GEN_FOURIER_DESCRIPTOR(imgs[i]);
		fourier_descriptors.push_back(temp);
		cout << "\rGenerated: " << i+1 << " Fourier Descriptors!";
	}

	return fourier_descriptors;
}

void WRITE_FOURIER(vector<vector<int> > fourier_descriptors){
	int i, j;
	fstream file;

	file.open("FOURIER.txt", fstream::in | fstream::out | fstream::app);

	if(file.is_open()){
		for(i = 0 ; i < fourier_descriptors.size(); i++){
			for(j = 0; j < fourier_descriptors[i].size(); j++){
				// if(i == 199){
				// 	cout << "Descriptor[199][" << j << "]: " << fourier_descriptors[i][j] << endl;
				// }
				file << fourier_descriptors[i][j] << " ";
			}
		file << "\n";
		cout << "\rWrited: " << i+1 << " Fourier Descriptors to File!";
		}
	}else{
		cout << "\n\nCould not open the FOURIER.TXT file on `WRITE_FOURIER` function!" << endl;
	}

	return;
}
