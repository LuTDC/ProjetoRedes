all: 
	g++ Cliente.cpp -o client
	g++ Servidor.cpp -pthread -o server

run:
	./server
