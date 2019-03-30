#ifndef _SOCKET_CLASS_H_
#define _SOCKET_CLASS_H_
#include<string>
#include<cstring>
#if defined(_WIN32) || defined(_WIN64)
#define WIN_OS
#endif
#ifdef WIN_OS
#include<winsock2.h>
// #pragma comment(lib,"libws2_32")
#define cs closesocket
#else
#include<unistd.h>
// #include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define cs close
#endif
class Socket{
	public:
		Socket(){
			using namespace std;
			#ifdef WIN_OS
			if(m_count==0){
				WORD sockVersion=MAKEWORD(2,2);
				WSADATA wsaData;
				if(WSAStartup(sockVersion,&wsaData))
					throw string("Socket::Socket:cannot start up the socket");
			}
			m_count++;
			#endif
    		if((m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1)
				throw string("Socket::Socket:create socket failed");
		}
		Socket(const Socket&)=delete;
		Socket(Socket &&other):m_socket(other.m_socket){
			other.m_socket=-1;
			#ifdef WIN_OS
			m_count++;
			#endif
		}
		Socket& operator=(const Socket&)=delete;
		Socket& operator=(Socket&&)=delete;
		~Socket(){
			cs(m_socket);
			#ifdef WIN_OS
			if(--m_count==0)
				WSACleanup();
			#endif
		}
		Socket* accept(sockaddr_in &address){
			ltype len=sizeof(sockaddr_in);
			stype s=::accept(m_socket,(sockaddr*)&address,&len);
			if(s==-1)
				throw std::string("Socket::accept:failed");
			return new Socket(s);
		}
		void bind(unsigned short host){
			sockaddr_in sin;
			sin.sin_family=AF_INET;
			sin.sin_port=htons(host);
			#ifdef WIN_OS
			sin.sin_addr.S_un.S_addr=INADDR_ANY;
			#else
			sin.sin_addr.s_addr=INADDR_ANY;
			#endif
			if(::bind(m_socket,(sockaddr*)&sin,sizeof(sin))==-1)
				throw std::string("Socket::bind:failed");
		}
		void connect(unsigned short host,const char *ip){
			sockaddr_in sin;
			sin.sin_family=AF_INET;
			sin.sin_port=htons(host);
			#ifdef WIN_OS
			sin.sin_addr.S_un.S_addr=inet_addr(ip);
			#else
			sin.sin_addr.s_addr=inet_addr(ip);
			#endif
			if(::connect(m_socket,(sockaddr*)&sin,sizeof(sin))==-1)
				throw std::string("Socket::connect:failed");
		}
		void connect(unsigned short host,const std::string &ip){
			connect(host,ip.c_str());
		}
		void listen(int len){
			if(::listen(m_socket,len)==-1)
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
		#ifdef WIN_OS
		typedef SOCKET stype;
		typedef int ltype;
		static int m_count;
		#else
		typedef int stype;
		typedef socklen_t ltype;
		#endif
		stype m_socket;
		Socket(stype s):m_socket(s){
			#ifdef WIN_OS
			m_count++;
			#endif
		}
};
#undef cs
#endif