# SocketClass

to use *socket* by a class

--------

## version

inside version: change the way to detect the build environment

---

License:*LGPL v3.0*

GitHub address:https://github.com/lrcno6/SocketClass

--------

*SocketClass* is a class library to use *socket*, and it is used on both *Windows* and *linux*.

What is more, if you cannot link function *_imp__htons* and so on you do not know when you use *SocketClass*, maybe you also need to link *libws2_32.a* or *ws2_32.lib*

--------

## how to use it

you should make first:

```bash
make gcc # for G++
make msvc # for Visual C++
```

and then link to *libsocket_class.a* or *socket_class.lib*:

```bash
# for example
g++ user.cpp -o user -LSocketClass -lsocket_class # for linux (g++)
cl user.cpp console/console.lib ws2_32.lib # for Windows (MSVC), remember to link to ws2_32.lib
g++ user.cpp -o user -LSocketClass -lsocket_class -lws2_32 # for Windows (MinGW), remember to link to libws2_32
```

---

## Example

```cpp
// server.cpp
#include<iostream>
#include<thread>
#include<mutex>
#include"socket_class.h"
using namespace std;
mutex output;
int main(){
	Socket server;
	Socket *client;
	sockaddr_in address;
	server.bind(8888);
	server.listen(5);
	while(1){
		auto l=[](Socket *client){
			string str=client->recv(255);
			lock_guard<mutex> lock(output);
			cout<<str<<endl; 
			delete client;
		};
		try{
			client=server.accept(address);
		}
		catch(string &str){
			cerr<<str<<endl;
			delete client;
			continue;
		}
		{
			lock_guard<mutex> lock(output);
			printf("Accept:%s\n",inet_ntoa(address.sin_addr));
		}
		thread t(l,client);
		t.detach();
	}
	return 0;  
}
```

```cpp
// client.cpp
#include<iostream>
#include"socket_class.h"
using namespace std;
int main(){
	Socket root;
	while(1){
		try{
			Socket client;
			client.connect(8888,"127.0.0.1");
			string str;
			getline(cin,str);
			client.send(str);
		}
		catch(string &str){
			cerr<<str<<endl;
		}
	}
	return 0;
}
```

--------

Welcome to help to improve *SocketClass*!

by [lrcno6](https://github.com/lrcno6)