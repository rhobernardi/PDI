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

vector<Mat> READ_IMGS(int quantity);
vector<Mat> GEN_POINTS(vector<Mat> images);
vector<Mat> GEN_SIFT_VEC(vector<Mat> images);
void WRITE_SIFT_XML(vector<Mat> descriptors);
Mat READ_SIFT_XML(string pathToFile, int pos);

int main( int argc, char** argv )
{
  //initModule_nonfree();
  vector<Mat> imgs, point_imgs, descriptors;
  clock_t begin, end;
  double elapsed_secs;

  //READING IMAGES FROM PICTURES FOLDER
  begin = clock();
  imgs = READ_IMGS(10000);
  end = clock();
  elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  cout << "\nTime to read the images: " << elapsed_secs/60 << " minutes." << endl;
  //READING IMAGES FROM PICTURES FOLDER

  //point_imgs = GEN_POINTS(imgs);
  //imshow("KeyPoints 1", point_imgs[0]);

  //GENERATING THE DESCRIPTORS USING SIFT 
  // begin = clock();
  // descriptors = GEN_SIFT_VEC(imgs);
  // end = clock();
  // elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  // cout << "\nTime to generate the descriptors: " << elapsed_secs/60 << " minutes." << endl;
  //GENERATING THE DESCRIPTORS USING SIFT

  //WRITE_SIFT_XML(descriptors);

  //READING DESCRIPTORS FROM FILES!
  // Mat teste255, teste1;
  // begin = clock();
  // teste1 = READ_SIFT_XML("SIFT.xml", 1);
  // teste255 = READ_SIFT_XML("SIFT.xml", 2);
  // end = clock();
  // elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  // cout << "\nTime to read the descripto from file: " << elapsed_secs/60 << " minutes." << endl;
  // imshow("Image 1", imgs[0]);
  // imshow("Descriptor 2", descriptors[1]);
  // imshow("Descriptor 2 From File", teste255);
  //READING DESCRIPTORS FROM FILES!

  //descriptors.clear();

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

//FUNCAO QUE GERA APENAS PONTOS DE INTERESSE E RETORNA VETOR COM IMAGENS CHEIA DE PONTOS
vector<Mat> GEN_POINTS(vector<Mat> images){

  int i;//iterador
  Mat aux;//Mat auxiliar para geracao do vector de retorno
  vector<Mat> point_images;//vector de retorno

  //funcao de descricao da imagem
  //EH AQUI QUE SE MANIPULA PARA REDUZIR O NRO. DE PONTOS
  //OLHAR: http://docs.opencv.org/3.0-beta/modules/xfeatures2d/doc/nonfree_features.html
  //PARA MAIOR DESCRICAO DE PARAMETROS 
  Ptr<Feature2D> f2d = xfeatures2d::SIFT::create(0, 3, 0.22, 10, 1.6);
  //vector de pontos de relevancia
  vector<KeyPoint> keypoints;

  //loop para detectar os pontos
  //impressao de quantidade para reducao ou aumento na
  //quantidade de pontos
  //desenho dos keypoints da imagem original para a imagem aux
  //adicao da Mat aux no vetor de retorno
  for(i = 0; i < images.size(); i++){
    //cout << i << endl;
    f2d->detect(images[i], keypoints);
    cout << i << " Keypoints: " << keypoints.size() << endl;
    //detector.detect(images, keypoints);
    drawKeypoints(images[i], keypoints, aux);
    point_images.push_back(aux);
  }
  
  //retorno do vetor com as imagens e seus pontos
  return point_images;
}

//FUNCAO QUE GERA DESCRITORES
vector<Mat> GEN_SIFT_VEC(vector<Mat> images){

  cout << "\n";

  int i;//iterador
  vector<Mat> descriptors;//vector com os descritores de tipo Mat
  vector<KeyPoint> keypoints;//pontos de interesse

  //funcao de descricao da imagem
  //EH AQUI QUE SE MANIPULA PARA REDUZIR O NRO. DE PONTOS
  //OLHAR: http://docs.opencv.org/3.0-beta/modules/xfeatures2d/doc/nonfree_features.html
  //PARA MAIOR DESCRICAO DE PARAMETROS 
  Ptr<Feature2D> f2d = xfeatures2d::SIFT::create(0, 3, 0.22, 10, 1.6);


  //loop para detectar pontos de interesse
  //geracao de descritores com a funcao compute
  //adicao da Mat aux no vetor de retorno
  //impressao para checagem de qual descritor foi criado
  for(i = 0; i < images.size(); i++){
    //cout << i << endl;
    Mat aux;
    f2d->detect(images[i], keypoints);
    f2d->compute(images[i], keypoints, aux);
    descriptors.push_back(aux);
    //cout << "Descriptor: " << i << " -> " << aux.type() << endl;
    cout << "\rDescriptor: " << i+1 << " created!";
    aux.release();
  }

  cout << "\n";

  //retorno do vetor com os descritores
  return descriptors;
}

//FUNCAO QUE CRIA ARQUIVOS DE DESCRITORES
void WRITE_SIFT_XML(vector<Mat> descriptors){
  
  int i;//iterador
  string names = "Descriptor_", aux;//strings para geracao dos id`s !!IMPORTANTE!!
  FileStorage fs("SIFT.xml", FileStorage::WRITE);//metodo openCV para R/W em arquivos 
  FileStorage fs2("SIFT2.xml", FileStorage::WRITE);//metodo openCV para R/W em arquivos 

  //criacao de dois arquivos dividos na metade
  //pois a criacao de apenas um com 10000
  //estava dando problemas xD
  for(i = 1; i <= 5000; i++){
    aux = names + to_string(i);
    //a escrita necessita de um nome
    //fs << aux
    //e entao da Mat em questao
    //fs << aux << descriptors[]...
    fs << aux << descriptors[i-1];
  }
  for(i = 5001; i <= descriptors.size(); i++){
    aux = names + to_string(i);
    fs2 << aux << descriptors[i-1]; 
  }

  fs.release();
  fs2.release();

  return;
}

//FUNCAO QUE LE ARQUIVOS DE DESCRITORES
Mat READ_SIFT_XML(string pathToFile, int pos){
  //estou lendo apenas um por hora
  Mat descriptor;//Mat de retorno
  string picture = "Descriptor_";//id do descriptor
  picture += to_string(pos);//id do descriptor completo
  //cout << picture << endl;
  FileStorage fs(pathToFile, FileStorage::READ);//metodo openCV para R/W em arquivos 
  
  //a leitura recebe o id do descriptor fs[id_do_descriptor]
  fs[picture] >> descriptor;//eh passado para o Mat
  return descriptor;
}

/*MakeFile
#g++ -ggdb `pkg-config opencv --cflags --libs` makeDescriptors.cpp -o generate
#./generateDescriptors
*/
