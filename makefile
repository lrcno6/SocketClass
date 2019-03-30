libsocket_class.a:socket_class.o
	ar cr libsocket_class.a socket_class.o
socket_class.o:socket_class.cpp socket_class.h
	g++ -c socket_class.cpp -Wall -g
clean:
	rm libsocket_class.a socket_class.o
gcc:
	g++ -c socket_class.cpp -O3
	ar cr libsocket_class.a socket_class.o
msvc:
	cl /c socket_class.cpp
	lib socket_class.obj /out:socket_class.lib