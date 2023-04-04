#pragma once
#include <iostream>

#define MAX_ITEMS 100

using namespace std;

struct node {
  string key;
  string value;  
};

struct map {
  struct node values[MAX_ITEMS];  
};

struct server_response {
  string status;
  string response;
};

struct splitted_buffer {
  string strings[3];
  struct server_response error;
};

class Parser {
  public:
    server_response parse(string buffer); 
    void free_map();
  private:
    struct map *_map = new struct map;
    struct splitted_buffer _split(string buffer);

    //Hash function
    int _hash(string key);

    //Methods
    server_response _set(string key, string value);
    server_response _get(string key);
    server_response _delete(string key);
};