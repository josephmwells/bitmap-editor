
all:
	g++ main.cpp bitmap.cpp effects.cpp -o bitmap

debug:
	g++  -g main.cpp bitmap.cpp effects.cpp -o bitmap

mac:
	clang++ -std=c++17 main.cpp bitmap.cpp effects.cpp -o bitmap

mac-debug:
	clang++ -std=c++17 -g main.cpp bitmap.cpp effects.cpp -o bitmap

mac-verbose:
	clang++ -std=c++17 -g -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic main.cpp bitmap.cpp effects.cpp -o bitmap
