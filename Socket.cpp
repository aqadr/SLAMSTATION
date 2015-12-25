// Implementation of the Socket class.


#include "Socket.h"
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <cstdlib>



using namespace std;





Socket::Socket() : sockfd ( -1 ), buf_size(350) {
    memset ( &hints, 0, sizeof ( hints ) );
    if(!create())
      std::cout<<"couldnot create port"<<std::endl;
    ptr=&sockfd;
    ptr_buff = buf;
}


Socket::~Socket() {
    if ( is_valid() )::close ( sockfd );
}




bool Socket::create(){
  
    memset ( &hints,
	   0,
	   sizeof ( hints ) );	 

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_flags =AI_PASSIVE;		    

    if ((rv=getaddrinfo(NULL, MYPORT, &hints, &servinfo)) !=0) {
	fprintf (stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	return 1;
    }

    for (p =servinfo; p != NULL; p = p->ai_next) {
    
	if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {  
	    perror ("talker:socket");
	    continue;
	}
	
	if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
	    close (sockfd);
	    perror ("listener: bind");
	    continue;
	}
	break;
    }

     std::cout<<"does it come here????:"<<std::endl;
 
    if (p == NULL) {

	fprintf(stderr, "listener: failed to bind socket\n");
	return 2;
    }
    cout<<"sockfd:  "<<sockfd<<endl;
    
/*    int opts = fcntl ( m_sock,
		 F_GETFL );

    opts = ( opts | O_NONBLOCK );
  
    fcntl ( m_sock,
	  F_SETFL,opts );
*/

    freeaddrinfo(servinfo);

}






bool Socket::connect ( const std::string host, const int port ) {
    if ( ! is_valid() ) return false;

    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons ( port );

    int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

    if ( errno == EAFNOSUPPORT ) return false;

    status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

    if ( status == 0 )
      return true;
    else
      return false;
}


  
 
int Socket::send_data (char *p, size_t size) {
  
  /*cout<<"does it come here:"<<endl;
  int status = sendto(sockfd, p, size , 0);
  cout<<"status:"<<status<<endl;
  if(status ==-1) {//int flags);
    cout<<"status error:"<<status<<endl;
    perror("send");
    return 1;
  }
  
  return status; 
  */
  socklen_t addr_len = sizeof(their_addr);
  int status = ::sendto(sockfd, p, size, 0, (struct sockaddr *)&their_addr, addr_len);

  if ( status == -1 )
    {
      std::cout << "status == -1   errno == " << errno << "  in Socket::send\n";
      return 0;
    }
  else if ( status == 0 )
    {
      
      cout<<"try"<<endl;
      return 0;
    }
   else 
    {
      return status;
    }
  
  
  
  
  
}



int Socket::recv_data( char buffer[200]){
    
    

    //num_of_bytes_available =0;  
    //if(ioctl(sockfd, FIONREAD, & num_of_bytes_available) <0) {
//	    cout<<"no data"<<endl;
//	}   else {
//	    cout<<"no_of_bytes=="<<num_of_bytes_available<<endl;
//	}
//	
//    memset ( p, 0, num_of_bytes_available);
//    int status = recv( sockfd, p, num_of_bytes_available, 0 );
//    if ( status == -1 ) {
//	     std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
//	     return 1;
//	 } else if ( status == 0 ) {
//	    return 0;
//	 } 
	 
	 
	their_addr_len = sizeof(struct sockaddr_storage);
	ssize_t nread;
//	char buf[150];
	char* p=buffer;
        nread = recvfrom(sockfd, buffer, 300, 0,
                (struct sockaddr *) &their_addr, &their_addr_len);
        if (nread == -1){
            std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
            return 1;
         }else {
             memcpy ( ptr_buff, p, nread );
            return 0;
         }

//        char host[NI_MAXHOST], service[NI_MAXSERV];

//        int s = getnameinfo((struct sockaddr *) &their_addr,
//                        their_addr_len, host, NI_MAXHOST,
//                        service, NI_MAXSERV, NI_NUMERICSERV);
//       if (s == 0)
//            printf("Received %ld bytes from %s:%s\n",
 //                   (long) nread, host, service);
 //       else
//            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
	 
}









/*

bool Socket::connect ( const std::string host, const int port )
{
  if ( ! is_valid() ) return false;

  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons ( port );

  int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

  if ( errno == EAFNOSUPPORT ) return false;

  status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

  if ( status == 0 )
    return true;
  else
    return false;
}

*/

void Socket::set_non_blocking ( const bool b ) {

  int opts;

  opts = fcntl ( sockfd,F_GETFL );

  if ( opts < 0 ) {
      return;
  }

  if ( b )
    opts = ( opts | O_NONBLOCK );
  else
    opts = ( opts & ~O_NONBLOCK );

  fcntl ( sockfd, F_SETFL,opts );

}

