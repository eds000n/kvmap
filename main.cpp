#include "kvmap.h"
//#include <pistache/http.h>
//#include <pistache/endpoint.h>
//https://github.com/Microsoft/cpprestsdk
//https://github.com/Corvusoft/restbed
//https://github.com/oktal/pistache

//#define MAX_SIZE 10000000
#define MAX_SIZE 4
#define PORT 3000


KVMap kvmap(MAX_SIZE);
void putValueHandler(const shared_ptr<Session> session){
  kvmap.putValueHandler(session);
  /*const auto request = session->get_request();
  cout << request->get_method() << endl;
  cout << request->get_query_parameter("key") << endl;
  cout << request->get_query_parameter("value") << endl;
  int content_length = request->get_header( "Content-Length",0);
  *///session->fetch(content_length, [](const shared_ptr<Session> session, const Bytes & body ){

    //const auto request = session->get_request();
    //session->close( OK, "put    value!", { { "Content-Length", "13" } } );
 // });
}

void deleteValueHandler(const shared_ptr<Session> session){
  kvmap.deleteValueHandler(session);
}

void getValueHandler(const shared_ptr<Session> session){
  kvmap.getValueHandler(session);
}

int main () {

  //const KVMap& map = KVMap();

  auto resource = make_shared< Resource >( );
  resource->set_path( "/value" );
  //resource->set_method_handler( "POST", post_method_handler );
  resource->set_method_handler( "POST", putValueHandler );
  resource->set_method_handler( "DELETE", deleteValueHandler );
  resource->set_method_handler( "GET", getValueHandler );

  auto settings = make_shared< Settings >( );
  settings->set_port( PORT );
  settings->set_default_header( "Connection", "close" );

  Service service;
  service.publish( resource );
  service.start( settings );

  return EXIT_SUCCESS;

}


