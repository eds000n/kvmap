#pragma once

#include <string>
#include <map>
#include <list>
#include <memory>
#include <cstdlib>
#include <restbed>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
using namespace std;
using namespace restbed;

/*
class Node {
  string value;
  Node* left;
  Node* right;
public:
  Node(string v){
    this->value=v;
  }
};
*/
class KVMap {
  
public:
  int putValue(string key, string value);
  int deleteValue(string key);
  int getValue(string key, string& value);

  void putValueHandler(const shared_ptr<Session> session);
  void deleteValueHandler(const shared_ptr<Session> session);
  void getValueHandler(const shared_ptr<Session> session);

  KVMap(int n);
private:
  //std::map<string, Node> hash;
  //Node* front;
  //Node* back;
  std::map<string, pair<string,list<string>::iterator> > hash;
  //std::map<string, string> hash;
  std::list<string> fifo;
  string swapFileName;
  int size ;
  int evict(int n);
  int getSwap(string key);
  void debug();
};
