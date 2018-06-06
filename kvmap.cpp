#include "KVMap.h"

/**
 * Constructor
 * @param n size of the hash
 */
KVMap::KVMap(int n) {
  this->size = n;
  swapFileName = "swap.txt";
  ofstream swap(swapFileName);
  swap.close();
}

int KVMap::evict(int n) {
  ofstream swap(swapFileName,  ofstream::out | ofstream::app);
  for (int k=0; k<n; k++){
    // Fill persistent memory
    std::list<string>::reverse_iterator rit=fifo.rbegin(); 
    swap << *rit << " " << hash[*rit].first << endl;
    // Clean memory
    list<string>::iterator it = next(rit).base();
    fifo.erase(it);
    hash.erase(hash.find(*it));
  }
  swap.close();

  //debug(); 
  return 0;
}

/**
 * getSwap searches on the swap, if the key is found, restores the value into memory and deletes from the swap
 * @param key to be searched
 */
int KVMap::getSwap(string key) {
  fstream swap(swapFileName );
  ofstream temp("temp.txt");
  string line;
  bool found = false;
  string fvalue;
  while (getline(swap,line)) {
    std::istringstream iss(line);
    string k,v;
    iss >> k >> v;
    if ( k == key ){
      //line.replace(0,line.length(),"");
      fvalue = v;
      found = true;
      //break;
    } else {
      temp << line << endl;
    }

  }

  // FIXME: I guess this part may lead to some racing conditions
  swap.close();
  temp.close();
  remove(swapFileName.c_str());
  rename("temp.txt", swapFileName.c_str());

  if ( found ){
    putValue(key, fvalue);
    return 0;
  }
  return 1;
}

/**
 * putValue adds a value to the hashtable
 * @param key 
 * @param value
 * @return 0 if inserted, 1 otherwise
 */
int KVMap::putValue(string key, string value) {
  // cache 
  if ( hash.size() >= size ) {
    // evict the least recently used
    evict(hash.size()/2);
  }
  

  if ( hash.find(key) == hash.end() )
    fifo.push_front(key);
  list<string>::iterator it = fifo.begin();
  hash[key]=make_pair(value, it);

  cout << "inserted (" << key << "," << value << ")" << endl;
  debug();



  //LRU
  /*if ( hash.size() == 0 ) {
    Node n(value); 
    front=n;
    back=n;
    n.left = NULL;
    n.right= NULL;
    hash[key]=n;
    cout << "initializing hash (" << key << "," << value << ")" << endl;
  } else {
    if ( hash.find(key) != hash.end() ){
      auto n=hash[key];
      auto left = &(n.left);
      auto right = &(n.right);
      left.right = right;
      right.left = left;
      n.right=front;
      n.left=NULL;
      front = n;
      cout << "updated (" << key << "," << value << ")" << endl;
    } else {
      Node n(value); 
      n.right = front;
      n.left = NULL;
      front = n;
      hash[key]=n;
      cout << "inserted (" << key << "," << value << ")" << endl;
    }
  }*/

  return 0;
}

/**
 * deleteValue deletes by key
 * @param key
 * @return 0 if deleted, 1 if the key didn't exist
 */
int KVMap::deleteValue(string key){ 
  if ( hash.find(key) != hash.end() ){
    ////////////////////////////////// FIXME: too slow
    /*for (std::list<string>::iterator it=fifo.begin(); it != fifo.end(); ++it)
      if ( *it == key ) {
        fifo.erase(it);
        break;
      }*/
    fifo.erase(hash[key].second);
    ////////////////////////////////// 
    hash.erase(key);
    cout << "deleted (" << key << ")" << endl;
    debug();
    return 0;
  } else if ( getSwap(key) == 0 ) {
    ////////////////////////////////// FIXME: too slow
    /*for (std::list<string>::iterator it=fifo.begin(); it != fifo.end(); ++it)
      if ( *it == key ) {
        fifo.erase(it);
        break;
      }*/
    fifo.erase(hash[key].second);

    ////////////////////////////////// 

    hash.erase(key);
    debug();
    return 0;
  } else {
    debug();
    return 1;
  }
}

/**
 * getValue returns a pair if found
 * @param key
 * @param value reference to the value
 * @return 0 if found, 1 if no key exists
 */
int KVMap::getValue(string key, string& value) {
  if ( hash.find(key)  != hash.end() ){
    value=hash[key].first;
    cout << "got (" << key << "," << value << ")" << endl;
    //For LRU update the list so the least used remain at the end
    fifo.erase(hash[key].second);
    fifo.push_front(key);
    list<string>::iterator it = fifo.begin();
    hash[key]=make_pair(value,it);
    debug();
    return 0;
  } else if ( getSwap(key) == 0 ) {
    value=hash[key].first;
    cout << "got (" << key << "," << value << ")" << endl;
    debug();
    return 0;
  } else {
    debug();
    return 1;
  }
}

void KVMap::putValueHandler(const shared_ptr<Session> session) {
  const auto request = session->get_request( );

  string key   = request->get_query_parameter("key");
  string value = request->get_query_parameter("value");
  //cout << "k,v" << key << " " << value << endl;

  if ( key != "" && value != "" ){
    int r = putValue(key, value);
    if ( r == 0 ){
      string msg = "pair inserted";
      session->close( OK, msg, { { "Content-Length", to_string(msg.length()) } } );
    } else {
      string msg = "pair not inserted";
      session->close( OK, msg, { { "Content-Length", to_string(msg.length()) } } );
    }
  } else {
    string msg = "key or value cannot be empty";
    session->close( OK, msg, { { "Content-Length", to_string(msg.length()) } } );
  }

}

void KVMap::deleteValueHandler(const shared_ptr<Session> session) {
  const auto request = session->get_request( );
  string key = request->get_query_parameter("key");
  if ( deleteValue(key) == 0 ){
    string msg = "deleted key";
    session->close( OK, msg, { { "Content-Length", to_string(msg.length()) } } );
  } else {
    string msg = "invalid key";
    session->close( OK, msg, { { "Content-Length", to_string(msg.length()) } } );
  }
}

void KVMap::getValueHandler(const shared_ptr<Session> session) {
  const auto request = session->get_request( );
  string value;
  string key = request->get_query_parameter("key");
  if ( getValue(key, value) == 0 ) {
    session->close( OK, value, { { "Content-Length", to_string(value.length() ) } } );
  } else {
    string msg = "key not found";
    session->close( OK, msg, { { "Content-Length", to_string(msg.length()) } } );
  }

}

void KVMap::debug() {
  cout << "DEBUG" << endl;
  cout << "=====" << endl;
  cout << "list content" << endl;
  for (std::list<string>::iterator it=fifo.begin(); it != fifo.end(); ++it)
    std::cout << ' ' << *it;
  cout << endl << "=====" << endl;
}
