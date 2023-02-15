#include <boost/lexical_cast.hpp>
#include "serverdb.h"
#include "logger.h"

using namespace logs;

int main( int argc, char *argv[])
{  
  {
    LOG log(logTRACE);
    log << "main() START";
    for (int i = 0; i<argc; ++i)
      log << " " << argv[i];
  }

  #ifdef WIN32
  system("chcp 1251 > nul");
  #endif

  try
  {
    if ( argc != 8)
    {
      std::cerr << "\nUsage: serverdb <port> <threads> <db_host_ip> <db_host_port> <db_name> <db_user> <db_upassw>\n";
      std::cerr << "  Try:\n";
      std::cerr << "    serverdb 3300 5 127.0.0.1 3306 gambling root [****]\n";
      return 1;
    }

    std::size_t srv_port = boost::lexical_cast<std::size_t>( argv[1]);
    std::size_t num_threads = boost::lexical_cast<std::size_t>( argv[2]);
    char *db_host_ip = argv[3];
    std::size_t db_host_port = boost::lexical_cast<std::size_t>( argv[4]);
    char *db_name = argv[5];
    string db_user( argv[6]);
    string db_upassw( argv[7]);

    serverdb server( srv_port, num_threads);
    server.init_db( db_host_ip, db_host_port, db_name, db_user, db_upassw);
    server.run();
  }
  catch ( const std::exception &e)
  {
    LOG(logFATAL) << "main() exception: " << e.what();
    return 1;
  }

  return 0;
}
