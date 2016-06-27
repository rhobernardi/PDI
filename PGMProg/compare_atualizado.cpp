#include <opencv2/opencv.hpp>
#include "opencv2/xfeatures2d.hpp"
// #include <opencv2/opencv.hpp>
// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/features2d/features2d.hpp>
// #include <opencv2/nonfree/nonfree.hpp>
#include <iostream>
#include <fstream>
#include <climits>
#include <vector>
#include <string>
#include <cstdio>
#include <cmath>
#include <ctime>


#define BD_SIZE 10000

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

Mat READ_ONE_SIFT_XML(string pathToFile, int pos);
vector<Mat> READ_ALL_SIFT_XML();
vector<vector<int> > READ_ALL_FOURIER();
Mat GEN_SIFT_DESCRIPTOR(Mat consultImg);
vector<int> GEN_FOURIER_DESCRIPTOR(Mat consultImg);
vector<string> GET_K_SIFT_IMAGES(Mat img, string imgPath, int k);
vector<string> GET_K_FOURIER_IMAGES(Mat img, string imgPath, int k);
void testeFourier(Mat img, Mat img2);

int main( int argc, char** argv ){

	clock_t begin, end;
	double elapsed_secs;

	if( argc < 4 || argc > 6 ) {
		cout << "\nAlgo foi digitado errado!\n";
		cout << "\nUsage: $ ./corel10k <imagem de consulta> <k-proximos> <metodo> [parametros adicionais do metodo]" << "\n\n";
		return -1;
	}

	// Leitura da linha de comando
	string pathToImage = "Pictures_GHIM20/";
	string imagemConsulta = argv[1];
	pathToImage += imagemConsulta;
	int kaProximos = stoi(argv[2], NULL, 10);
	string metodo = argv[3];
	// Leitura da linha de comando

	// Escolha do metodo
	if( metodo.compare("sift") == 0 || metodo.compare("SIFT") == 0 ) {
		//GERAR VARIAVEIS PARA OS PARAM. DE SIFT
		cout << "You choosed SIFT!" << endl;
		Mat img = imread(pathToImage, 0);
		Mat test = GEN_SIFT_DESCRIPTOR(img);
		imshow("test", test);
		// vector<Mat> teste = READ_ALL_SIFT_XML();
		// imshow("test", teste[529]);
		vector<string> k_next;
		k_next = GET_K_SIFT_IMAGES(img, pathToImage, kaProximos);
		for(int i = 0; i < k_next.size(); i++){
			cout << k_next[i] << endl;
		}
	}
	if( metodo.compare("Fourier") == 0 || metodo.compare("fourier") == 0 || metodo.compare("FOURIER") == 0 ) {
		//GERAR VARIAVEIS PARA OS PARAM. DE FOURIER
		cout << "You choosed FOURIER!" << endl;
		Mat img = imread(pathToImage, 0);
		//Mat img2 = imread("Pictures_GHIM20/199.jpg", 0);
		vector<string> k_next;
		k_next = GET_K_FOURIER_IMAGES(img, pathToImage, kaProximos);
		for(int i = 0; i < k_next.size(); i++)
			cout << k_next[i] << endl;
		//testeFourier(img, img2);
	}
	// Escolha do metodo

	// //TESTE PARA O CASO DA IMAGEM DE CONSUTLA SER A 200.JPG E K=5
	// Mat i1 = imread("Pictures_GHIM20/591.jpg", 1);
	// Mat i2 = imread("Pictures_GHIM20/590.jpg", 1);
	// // Mat i3 = imread("Pictures_GHIM20/194.jpg", 1);
	// // Mat i4 = imread("Pictures_GHIM20/199.jpg", 1);
	// // Mat i5 = imread("Pictures_GHIM20/204.jpg", 1);
	// Mat iconsulta = imread(pathToImage, 1);

	// imshow("img1", i1);
	// imshow("img2", i2);
	// // imshow("img3", i3);
	// // imshow("img4", i4);
	// // imshow("img5", i5);
	// imshow("imgConsulta", iconsulta);

	waitKey(0);

	return 0;
}

// FUNCAO QUE LE APENAS UM DESCRITOR SIFT DO ARQUIVO
Mat READ_ONE_SIFT_XML( string pathToFile, int pos ) {
	Mat descriptor;
	string picture = "Descriptor_";
	picture += to_string(pos);
	cout << picture << endl;
	FileStorage fs(pathToFile, FileStorage::READ);
	fs[picture] >> descriptor;
	return descriptor;
}

// FUNCAO QUE LE TODOS DESCRITORES SIFT DO ARQUIVO
vector<Mat> READ_ALL_SIFT_XML() {

	int i;
	string position;
	vector<Mat> descriptors;
	Mat temp;
	FileStorage fs("SIFT.XML", FileStorage::READ);
	FileStorage fs2("SIFT2.XML", FileStorage::READ);

	// Loop de 1 ate 5000
	// lendo todas a posicoes do arquivo xml e salvando em
	// objeto do tipo MAT
	for(i = 1; i <= 5000; i++) {
		position = "Descriptor_";
		position += to_string(i);
		//cout << "\rReading: " << position << endl;
		fs[position] >> temp;
		descriptors.push_back(temp);
		position.clear();
		temp.release();
	}

	// Loop de 5001 ate 10000
	// lendo todas a posicoes do arquivo xml e salvando em
	// objeto do tipo MAT
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

// FUNCAO QUE LE TODAS DESCRITORES SIFT DO ARQUIVO
vector< vector<int> > READ_ALL_FOURIER() {
	int i, j, value;
	fstream file;
	vector<int> temp;
	vector<vector<int> > fourier_descriptors;

	file.open( "FOURIER.txt", fstream::in | fstream::out );

	if(file.is_open()) {
		for(i = 0; i < BD_SIZE; i++) {
			for(j = 0; j < 400; j++) {
				file >> value;
				temp.push_back(value);
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

// FUNCAO QUE GERA DESCRITOR SIFT DE UMA IMAGEM
Mat GEN_SIFT_DESCRIPTOR(Mat consultImg) {
	Mat descriptor;
	vector<KeyPoint> keypoints;

	Ptr<Feature2D> f2d = xfeatures2d::SIFT::create(0, 3, 0.22, 10, 1.6);
	// Ptr<Feature2D> f2d = xfeatures2d::SIFT::create(0, 3, 0.18, 10, 0.8);

	f2d->detect(consultImg, keypoints);
	f2d->compute(consultImg, keypoints, descriptor);

	return descriptor;
}

// FUNCAO QUE GERA DESCRITOR FOURIER DE UMA IMAGEM
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

	cout << "Rows: " << fourier.rows << " Cols: " << fourier.cols << endl;

	for(i = cy - 10; i < cy + 10; i++){
		for(j = cx - 10; j < cx + 10; j++){
			img_fourier_descriptor.push_back(int(fourier.at<uchar>(i,j)));
		}
	}

	return img_fourier_descriptor;
}

// FUNCAO QUE ANALISA OS DESCRITORES SIFT E RETORNA AS K MAIS PROXIMAS
vector<string> GET_K_SIFT_IMAGES( Mat img, string imgPath, int k )
{
	Mat imgDescriptor = GEN_SIFT_DESCRIPTOR(img);
	vector<Mat> descriptors = READ_ALL_SIFT_XML();

	//BFMatcher matcher(NORM_L1);
	FlannBasedMatcher matcher;
	vector< vector<DMatch> > matches;
	vector<int> matches_idx;

	string s = imgPath;
	string delimiter = ".";
	string token = s.erase(0, 16);// token will be the number part of the imgPath
	token = token.substr(0, token.find(delimiter));
	int index = stoi(token, NULL, 10);
	//cout << index << endl;

	//Percorre os descritores e cria os objetos DMatches a serem adicionados no vector matches.
	//caso seja igual ao descritor em questao pulamos ele.
	for ( int i = 0; i < BD_SIZE; ++i )
	{
		vector<DMatch> temp;
		if(imgDescriptor.type() == descriptors[i].type() && imgDescriptor.cols == descriptors[i].cols){
			if( i+1 ==  index)
			{
				//cout << "INDEX CHECK: " << i+1 << endl;
				continue;
				// matcher.match(imgDescriptor, descriptors[i], temp);
				// temp.back().distance = 9999999;
				// matches.push_back(temp);
				// matches_idx.push_back(i+1);
			}

			else
			{
				matcher.match( imgDescriptor, descriptors[i], temp );
				matches.push_back(temp);
				//cout << "I: " << i << " TEMP.SIZE(): " << temp.size() << endl;
				matches_idx.push_back(i+1);
				//cout << "ELSE CHECK: " << i << endl;
			}
		}

		// if(descriptors[i].type() != CV_32F){
		// 	descriptors[i].convertTo(descriptors[i], CV_32F);
		// }
		// if(imgDescriptor.type() != CV_32F){
		// 	imgDescriptor.convertTo(imgDescriptor, CV_32F);
		// }

	}

	// for (int i = 0; i < matches_idx.size(); ++i)
	// {
	// 	cout << matches_idx[i] << endl;
	// }

	//cout << matches.size() << endl;

	// for(int i = 0; i < matches.size(); i++){
	// 	cout << matches[i].size() << endl;
	// }

	double max_dist = 0; double min_dist = 100;

	//-- Calculo de menor a maior distancias entre os matches.
	for ( int i = 0; i < matches.size(); ++i )
	{
		// for( int j = 0; j < imgDescriptor.rows; j++ )
		for(int j = 0; j < matches[i].size(); j++)
		{
			double dist = matches[i][j].distance;

			if( dist < min_dist ) min_dist = dist;
			if( dist > max_dist ) max_dist = dist;
		}
	}


	vector< vector<DMatch> > good_matches;
	vector<int> good_matches_idx;
	vector<DMatch> temp;

	//-- Filtragem dos melhores matches!
	for (int i = 0; i < matches.size(); ++i)
	{
		//cout << "GOOD_MATCHES[" << i << "].size: " << size << endl;
		// for (int j = 0; j < imgDescriptor.rows; ++j)
		for (int j = 0; j < matches[i].size(); ++j)
		{
			if ( matches[i][j].distance <= max(3*min_dist, 0.1) )
			{
				temp.push_back(matches[i][j]);
			}
		}

		good_matches.push_back(temp);
		//cout << "I: " << i << " TEMP.SIZE(): " << temp.size() << endl;
		temp.clear();
		good_matches_idx.push_back(matches_idx[i]);
	}

	int idx = -1, size = 0, step = -1;
	string imgPosition;
	vector<int> int_next_k;
	vector<string> next_k;

	for(int i = 0; i < k; i++){
		for(int j = 0; j < good_matches.size(); j++){
			// cout << "J: " << j << endl;
			// cout << "GOOD_MATCHES_IDX[J]: " << good_matches_idx[j] << endl;
			// cout << "GOOD_MATCHES[j].size(): " << good_matches[j].size() << endl;
			if(good_matches_idx[j] == index){
				;
			}else{
				if(size < good_matches[j].size()){
					// cout << "K: " << i << " J: " << j << endl;
					// cout << "SIZE: " << size << endl;
					// cout << "GOOD_MATCHES[" << j << "].size: " << good_matches[j].size() << endl;
					// cout << "INDEX: " << good_matches_idx[j] << endl;	 			
					idx = good_matches_idx[j];
					size = good_matches[j].size();
					step = j;
				}
			}

		}
		int_next_k.push_back(idx);
		good_matches.erase(good_matches.begin()+step);
		good_matches_idx.erase(good_matches_idx.begin()+step);
		// cout << "GOOD_MATCHES.SIZE(): " << good_matches.size() << endl;
		size = 0;
	}
	sort(int_next_k.begin(), int_next_k.end());
	for(int i = 0; i < int_next_k.size(); i++){
		imgPosition = to_string(int_next_k[i]);
		imgPosition += ".jpg";
		next_k.push_back(imgPosition);
	}

	return next_k;
}

// FUNCAO QUE ANALISA OS DESCRITORES FOURIER E RETORNA AS K MAIS PROXIMAS
vector<string> GET_K_FOURIER_IMAGES( Mat img, string imgPath, int k ){
	vector<int> consultDescriptor = GEN_FOURIER_DESCRIPTOR(img);
	vector<vector<int> > fourier_descriptors = READ_ALL_FOURIER();
	vector<float> vec_sum;
	vector<int> sum_idx;
	vector<string> k_next;
	int i, j;
	float sum = 0;

	string s = imgPath;
	string delimiter = ".";
	string token = s.erase(0, 16);// token will be the number part of the imgPath
	token = token.substr(0, token.find(delimiter));
	int index = stoi(token, NULL, 10);

	// manhattan distance
	// citada no slide de aula
	for(i = 0; i < fourier_descriptors.size(); i++){

		for(j = 0; j < fourier_descriptors[i].size(); j++){

			// cout << "Consulta: " << consultDescriptor.size() << endl;
			// cout << "Fourier_Descriptor[" << i << "]: " << fourier_descriptors[i].size() << endl;

			if(consultDescriptor[j] > fourier_descriptors[i][j])
				sum += sqrt((consultDescriptor[j] - fourier_descriptors[i][j]));
			else
				sum += sqrt((fourier_descriptors[i][j] - consultDescriptor[j]));	 	

		}

		vec_sum.push_back(sum);
		sum_idx.push_back(i+1);
		sum = 0;
	}

	// cout << "201: " << vec_sum[201] << endl;
	// cout << "854: " << vec_sum[854] << endl;
	// cout << "853: " << vec_sum[853] << endl;
	// cout << "852: " << vec_sum[852] << endl;
	// cout << "851: " << vec_sum[851] << endl;
	// cout << "850: " << vec_sum[850] << endl;

	int min = INT_MAX, idx, step;
	string pos;

	//finding the smallest distance that represents the closest match
	for(i = 0; i < k+1; i++){
		for(j = 0; j < vec_sum.size(); j++){
			// Sempre que houver uma distancia menor
			// Tanto o indice, quanto o valor da soma (var min) eh atualizado
			// Ate o loop terminar e termos a menor distancia junto
			// Do indice referente a ela
			if(vec_sum[j] < min){
				min = vec_sum[j];
				idx = sum_idx[j];
				step = j-1;
			}
		}
		pos = to_string(idx);
		pos += ".jpg";
		k_next.push_back(pos);
		min = INT_MAX; // Reset da variavel min
		vec_sum.erase(vec_sum.begin()+step); // remocao do vector para evitar comparar duas vezes
	}
	// cout << "Number of Descriptors: " << fourier_descriptors.size() << endl;
	// cout << "Size of Descriptors: " << fourier_descriptors[0].size() << endl;
	// cout << "Size of Descriptors: " << fourier_descriptors[9999].size() << endl;

	// cout << "Value Check: " << fourier_descriptors[0][10] << endl;

	k_next.erase(k_next.begin()); // o codigo compara a imagem de consulta com ela mesmo, portanto
								  // deleto o primeiro indice que eh a propria imagem
							      // ela estara em primeiro sempre pois eh a mais proxima dela mesma!
	return k_next;
}

// >>>>> UTILIZADA APENAS PARA TESTES <<<<<
void testeFourier(Mat img1, Mat img2){
	vector<int> consultDescriptor = GEN_FOURIER_DESCRIPTOR(img1);
	vector<int> consultDescriptor2 = GEN_FOURIER_DESCRIPTOR(img2);
	//vector<vector<int> > fourier_descriptors = READ_ALL_FOURIER();
	vector<int> differIdxs;

	for(int i = 0; i < consultDescriptor.size(); i++){
		// for(int j = 0; j < four)
		if(consultDescriptor[i] != consultDescriptor2[i] /*fourier_descriptors[198][i]*/){
		cout << "Consult[" << i << "]: " << consultDescriptor[i] << " Descriptor[" << i << "]: " << consultDescriptor2[i] << endl;
		differIdxs.push_back(i);
		}
	}

	if(differIdxs.size() == 0)
		cout << "iguais!" << endl;

	// for(int i = 0; i < differIdxs.size(); i++)
	// 	cout << 

	return;
}