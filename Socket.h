// Definition of the Socket class

#ifndef Socket_class
#define Socket_class


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <queue>
#include <vector>
#include <signal.h>
//#define MYPORT "1600"				/// the port users will be connected to
#define MYPORT "22000"
const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

class Socket
{
 public:
  Socket();
  virtual ~Socket();

  // Server initialization
  int create_server();
  bool create();

  bool accept(Socket&) const;
  bool listen() const;
  
  bool connect ( const std::string host, const int port );
  
// Data Transimission
  
  int send_data (char *p, size_t size);
  
  int recv_data( char buffer[200]); 
//  int recv ( std::string& ) const;
  char buf[10000];
  void set_non_blocking ( const bool );

  char * ptr_buff;
  bool is_valid() const { return sockfd != -1; }
  int m_sock;
  int num_of_bytes_available;
  int *ptr;
 private:
  int sockfd;
  sockaddr_in m_addr;
  struct sockaddr *si_other;
  struct addrinfo hints;
  struct addrinfo *servinfo, *p;
  struct sockaddr_storage their_addr;
  socklen_t their_addr_len;
  int rv;
  int buf_size;
  
};


#endif
