#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

static std::string jsonEscape(const std::string& s){
  std::string o;
  for(char c:s){ if(c=='"'||c=='\\') o+='\\'; if(c=='\n') o+="\\n"; else o+=c; }
  return o;
}
static std::string bodyOf(const std::string& req){
  auto p=req.find("\r\n\r\n"); return p==std::string::npos?"":req.substr(p+4);
}
static std::string jsonValue(const std::string& body,const std::string& key){
  std::string k="\""+key+"\":";
  auto p=body.find(k); if(p==std::string::npos) return "";
  p+=k.size(); while(p<body.size()&&(body[p]==' '||body[p]=='\t')) p++;
  if(p<body.size()&&body[p]=='"'){ p++; auto e=body.find('"',p); return e==std::string::npos?"":body.substr(p,e-p); }
  auto e=body.find_first_of(",}",p); return body.substr(p,e==std::string::npos?body.size()-p:e-p);
}
static std::string tcpRequest(const std::string& ip,int port,const std::string& line){
  int fd=socket(AF_INET,SOCK_STREAM,0); if(fd<0) throw std::runtime_error("socket failed");
  timeval tv{3,0}; setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv)); setsockopt(fd,SOL_SOCKET,SO_SNDTIMEO,&tv,sizeof(tv));
  sockaddr_in a{}; a.sin_family=AF_INET; a.sin_port=htons(port);
  if(inet_pton(AF_INET,ip.c_str(),&a.sin_addr)!=1){close(fd);throw std::runtime_error("invalid IP");}
  if(connect(fd,(sockaddr*)&a,sizeof(a))<0){std::string e=strerror(errno);close(fd);throw std::runtime_error("ESP32 connect failed: "+e);}
  std::string out=line+"\n"; if(send(fd,out.data(),out.size(),0)<0){close(fd);throw std::runtime_error("send failed");}
  std::string r; char b[1024];
  while(r.find('\n')==std::string::npos){ssize_t n=recv(fd,b,sizeof(b)-1,0);if(n<=0)break;b[n]=0;r+=b;if(r.size()>65536)break;}
  close(fd); if(r.empty()) throw std::runtime_error("ESP32 returned no response");
  auto nl=r.find('\n'); if(nl!=std::string::npos) r.resize(nl);
  return r;
}
static void sendHttp(int fd,int code,const std::string& body){
  std::string status=code==200?"200 OK":code==400?"400 Bad Request":code==404?"404 Not Found":"500 Internal Server Error";
  std::string h="HTTP/1.1 "+status+"\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Headers: Content-Type\r\nAccess-Control-Allow-Methods: GET,POST,OPTIONS\r\nContent-Length: "+std::to_string(body.size())+"\r\nConnection: close\r\n\r\n";
  send(fd,h.data(),h.size(),0); send(fd,body.data(),body.size(),0);
}
static void handle(int fd){
  char buf[16384]; int n=recv(fd,buf,sizeof(buf)-1,0); if(n<=0){close(fd);return;} buf[n]=0;
  std::string req(buf,n);
  if(req.rfind("OPTIONS ",0)==0){sendHttp(fd,200,"{}");close(fd);return;}
  auto lineEnd=req.find("\r\n"); if(lineEnd==std::string::npos){sendHttp(fd,400,"{\"ok\":false,\"error\":\"bad HTTP\"}");close(fd);return;}
  std::string first=req.substr(0,lineEnd), body=bodyOf(req);
  std::istringstream ss(first); std::string method,path; ss>>method>>path;
  try{
    if(method=="GET"&&path=="/api/health"){sendHttp(fd,200,"{\"ok\":true,\"service\":\"robot-control-core\",\"transport\":\"HTTP->TCP\"}");close(fd);return;}
    std::string ip=jsonValue(body,"ip"); int port=std::stoi(jsonValue(body,"port").empty()?"5000":jsonValue(body,"port"));
    if(method!="POST"){sendHttp(fd,400,"{\"ok\":false,\"error\":\"POST required\"}");close(fd);return;}
    if(path=="/api/connect"){auto r=tcpRequest(ip,port,"PING");sendHttp(fd,r=="PONG"||r=="OK"?200:502,"{\"ok\":"+std::string((r=="PONG"||r=="OK")?"true":"false")+",\"reply\":\""+jsonEscape(r)+"\"}");}
    else if(path=="/api/hardware"){auto r=tcpRequest(ip,port,"GET_HARDWARE");sendHttp(fd,200,"{\"ok\":true,\"hardware\":"+r+"}");}
    else if(path=="/api/telemetry"){auto r=tcpRequest(ip,port,"GET_TELEMETRY");sendHttp(fd,200,"{\"ok\":true,\"telemetry\":"+r+"}");}
    else if(path=="/api/command"){auto cmd=jsonValue(body,"command");auto r=tcpRequest(ip,port,"COMMAND "+cmd);sendHttp(fd,200,"{\"ok\":true,\"reply\":\""+jsonEscape(r)+"\"}");}
    else sendHttp(fd,404,"{\"ok\":false,\"error\":\"unknown endpoint\"}");
  }catch(const std::exception& e){sendHttp(fd,500,"{\"ok\":false,\"error\":\""+jsonEscape(e.what())+"\"}");}
  close(fd);
}
int main(){
  int s=socket(AF_INET,SOCK_STREAM,0); int one=1; setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));
  sockaddr_in a{}; a.sin_family=AF_INET; a.sin_addr.s_addr=htonl(INADDR_ANY); a.sin_port=htons(8080);
  if(bind(s,(sockaddr*)&a,sizeof(a))<0||listen(s,32)<0){std::cerr<<"cannot bind :8080\n";return 1;}
  std::cout<<"Robot Control Core bridge listening on http://localhost:8080\n";
  while(true){int c=accept(s,nullptr,nullptr);if(c>=0)std::thread(handle,c).detach();}
}