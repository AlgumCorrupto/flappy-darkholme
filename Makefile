all:
	g++ -Iinclude -Llib -o build/flappyclone.exe main.cc -lmingw32 -lSDL2main -lSDL2 -lSDL2_image