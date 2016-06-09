all: compile run

compile:
	#g++ -ggdb `pkg-config opencv --cflags --libs` compare.cpp -o corel10k
	g++ -ggdb `pkg-config opencv --cflags --libs` makeFourierDescriptors.cpp -o fourier
	
run:
	#./corel10k 1.jpg 2 SIFT
	./fourier