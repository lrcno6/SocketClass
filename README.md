## SocketClass Alpha 1.0

To use *Socket* by a class

--------

License:*LGPL v3.0*

--------

GitHub address:https://github.com/lrcno6/SocketClass

--------

*SocketClass* is a static library,and it is used on both *Windows* and *linux*.Please compile *Socket_class.cpp* and make a static library file *libSocketClass.a*(or *SocketClass.lib* or others)

And,if you cannot link function *_imp__htons* and so on you do not know,maybe you also need to link *libws2_32.a* or *ws2_32.lib*

--------

### Example

```cpp
//server.cpp
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
//client.cpp
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

By *EnderPearl*