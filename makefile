libsocket_class.a:socket_class.o
	ar cr libsocket_class.a socket_class.o
socket_class.o:socket_class.cpp socket_class.h
	g++ -c socket_class.cpp -O3
clean:
	rm libsocket_class.a socket_class.o