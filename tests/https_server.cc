#include <signal.h>
#include "libiqxmlrpc/libiqxmlrpc.h"
#include "server_general.h"

using namespace iqnet;
using namespace iqxmlrpc;

Server *server = 0;


void sigint_handler( int )
{
  std::cout << "Doing graceful exit..." << std::endl;
  
  if( server )
    server->set_exit_flag();
}


int main()
{
  signal( SIGINT, sigint_handler );

  try {
    ssl::ctx = ssl::Ctx::server_only( "data/cert.pem", "data/pk.pem" );

    server = new Server( 3344, new Executor_fabric<Serial_executor> );
    server->register_method<Get_weather>( "get_weather" );
    server->log_errors( &std::cerr );
    server->work<Https_server_connection>();
  }
  catch( const std::exception& e )
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  
  return 0;
}
