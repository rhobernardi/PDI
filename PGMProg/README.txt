Os codigos foram gerados em um MacBook PRO late 2013 Mac OS El Capitan, utilizando OpenCV 3.1.0.


Caso seja utilizado outro sistema operacional diferente do Mac OS, as devidas alteracoes deveram ser feitas.


>>>> NAO UTILIZE IMAGENS QUE NAO ESTEJAM DENTRO DA PASTA DE IMAGENS CHAMADA 'Pictures_GHIM20' <<<<<<
						>>>> APENAS IMAGENS DA PASTA SAO ACEITAS <<<<<	


Instrucoes de compilacao no Mac OS El Capitan:
	

COMPARACAO:	
	1)	Certifique-se de que a pasta contendo as imagens tenha o seguinte nome: "Pictures_GHIM20".
	2)	Os arquios de codigo deveram estar fora da pasta de imagens, porem dentro da mesma pasta em que a pasta contendo as imagens estao.
	3)  Para a comparacao execute: "g++ -ggdb `pkg-config opencv --cflags --libs` compare_atualizado.cpp -o corel10k".
	4)  A seguir: "./corel10k 200.jpg 5 SIFT" ou "./corel10k 200.jpg 5 FOURIER".

GERANDO DESCRITORES:
	1)	Certifique-se de que a pasta contendo as imagens tenha o seguinte nome: "Pictures_GHIM20".
	2)	Os arquios de codigo deveram estar fora da pasta de imagens, porem dentro da mesma pasta em que a pasta contendo as imagens estao.
	3)	Para a geracao dos descritores execute: "g++ -ggdb `pkg-config opencv --cflags --libs` makeDescriptors.cpp -o mkDesc" para SIFT
											ou: "g++ -ggdb `pkg-config opencv --cflags --libs` mFDesc.cpp -o fourier" para FOURIER
	4)	A seguir: "./mkDesc" para SIFT, ou: "./fourier" para FOURIER													
	