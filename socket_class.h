#ifndef _SOCKET_CLASS_H_
#define _SOCKET_CLASS_H_
#define WIN_OS
#include<string>
#ifdef WIN_OS
#include<winsock2.h>
#else

#endif
#pragma comment(lib,"libws2_32")  
class Socket{
	public:
		Socket(){
			using namespace std;
			if(m_count==0){
				WORD sockVersion=MAKEWORD(2,2);
				WSADATA wsaData;
				if(WSAStartup(sockVersion,&wsaData))
					throw string("Socket::Socket:cannot start up the socket");
			}
			m_count++;
    		if((m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==INVALID_SOCKET)
				throw string("Socket::Socket:create socket failed");
		}
		Socket(const Socket&)=delete;
		Socket(Socket &&other):m_socket(other.m_socket){
			other.m_socket=INVALID_SOCKET;
			m_count++;
		}
		Socket& operator=(const Socket&)=delete;
		Socket& operator=(Socket&&)=delete;
		~Socket(){
			closesocket(m_socket);
			if(--m_count==0)
				WSACleanup();
		}
		Socket* accept(sockaddr_in &address){
			int len=sizeof(sockaddr_in);
			SOCKET s=::accept(m_socket,(sockaddr*)&address,&len);  
			if(s==INVALID_SOCKET)  
				throw std::string("Socket::accept:failed");
			return new Socket(s);
		}
		void bind(unsigned short host){
			sockaddr_in sin;
			sin.sin_family=AF_INET;
			sin.sin_port=htons(host);
			sin.sin_addr.S_un.S_addr=INADDR_ANY;
			if(::bind(m_socket,(sockaddr*)&sin,sizeof(sin))==SOCKET_ERROR)
				throw std::string("Socket::bind:failed");
		}
		void connect(unsigned short host,const char *ip){
			sockaddr_in sin;
			sin.sin_family=AF_INET;
			sin.sin_port=htons(host);
			sin.sin_addr.S_un.S_addr=inet_addr(ip);
			if(::connect(m_socket,(sockaddr*)&sin,sizeof(sin))==SOCKET_ERROR)
				throw std::string("Socket::connect:failed");
		}
		void connect(unsigned short host,const std::string &ip){
			connect(host,ip.c_str());
		}
		void listen(int len){
			if(::listen(m_socket,len)==SOCKET_ERROR)
				throw std::string("Socket::listen:failed");
		}
		std::string recv(int maxlen){
			using namespace std;
			char *buffer=new char[maxlen+1];
			int len=::recv(m_socket,buffer,maxlen,0);       
			string str;
			if(len>0){
				buffer[len]='\0';
				str=buffer;
			}
			delete[] buffer;
			return str;
		}
		void send(const char *str){
			::send(m_socket,str,strlen(str),0);  
		}
		void send(const std::string &str){
			send(str.c_str());
		}
	private:
		static int m_count;
		SOCKET m_socket;
		Socket(SOCKET s):m_socket(s){
			m_count++;
		}
};
#endif