
all:
	g++ main.cpp bitmap.cpp -o bitmap

debug:
	g++  -g main.cpp bitmap.cpp -o bitmap

mac:
	clang++ -std=c++17  main.cpp bitmap.cpp -o bitmap

mac-debug:
	clang++ -std=c++17 -g main.cpp bitmap.cpp -o bitmap
