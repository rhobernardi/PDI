all: compile

compile:
	g++ -ggdb `pkg-config opencv --cflags --libs` compare_atualizado.cpp -o corel10k
	# g++ -ggdb `pkg-config opencv --cflags --libs` makeDescriptors.cpp -o mkDesc
	# g++ -ggdb `pkg-config opencv --cflags --libs` mFDesc.cpp -o fourier
run:
	./corel10k 200.jpg 5 SIFT
	# ./corel10k 200.jpg 5 FOURIER
	# ./mkDesc
	# ./fourier
