#include <algorithm>
#include "parser.h"

int Parser::_hash(string key) {
  int sum = 0;
  for (char c : key) {
    sum += c;
  }
  return sum % 100;
}

server_response Parser::_set(string key, string value) {
  int index = _hash(key);
  struct server_response res;

  if (index > 100) {
    res.response = "The index was too hight\033[0m\n";
    res.status = "\033[31mERR";
  }
  else if(value.empty()) {
    res.response = "Introduce a valid key\033[0m\n";
    res.status = "\033[31mERR";
  }
  else if(!this->_map->values[index].key.empty()) {
    res.response = "This item is alredy setted\033[0m\n";
    res.status = "\033[31mERR";
  }
  else {
    struct node new_item = { key, value };
    this->_map->values[index] = new_item;
    res.response = "\033[032mSuccesfully setted the new item\033[0m\n";
    res.status = "\033[032mOK";
  }

  return res;
}

server_response Parser::_get(string key) {
  int index = _hash(key);
  struct server_response res;

  if (index > 100) {
    res.response = "The index was too hight\033[0m\n";
    res.status = "\033[31mERR";
  }
  else if(this->_map->values[index].key.empty()) {
    res.response = "This item is not defined yet\033[0m\n";
    res.status = "\033[31mERR";
  }
  else {
    res.response = this->_map->values[index].value.append("\033[0m\n");
    res.status = "\033[32mOK";
  }

  return res;
}

server_response Parser::_delete(string key) {
  int index = _hash(key);
  struct server_response res;

  if (index > 100) {
    res.response = "The index was too hight\033[0m\n";
    res.status = "\033[31mERR";
  }
  else if(this->_map->values[index].key.empty()) {
    res.response = "This item doesn't exist\033[0m\n";
    res.status = "\033[31mERR";
  }
  else {
    this->_map->values[index] = { "", "" };
    res.response = "Item deleted succesfully\033[0m\n";
    res.status = "\033[032mOK";
  }

  return res;
}

struct splitted_buffer Parser::_split(string buffer) {
  static const string delimiter = " ";
  struct splitted_buffer data_splitted;
  size_t counter = 0;
  size_t pos = buffer.find_last_of("\n");
  if (pos != std::string::npos) {
    buffer.erase(pos);
  }

  pos = 0;

  buffer.append(" ");

  while ((pos = buffer.find(delimiter)) != string::npos) {
    if (counter > 2) {
      data_splitted.error.response = "Unexpected argument " + buffer.substr(0, pos) + "\033[0m\n";
      data_splitted.error.status = "\033[31mERR";
      break;
    }
    data_splitted.strings[counter] = buffer.substr(0, pos);
    buffer.erase(0, pos + delimiter.length());
    counter++;
  }
  return data_splitted;
}

server_response Parser::parse(string buffer) {
  struct splitted_buffer splitted_data = _split(buffer);
  string method = splitted_data.strings[0];
  transform(method.begin(), method.end(), method.begin(), ::toupper);

  if (!splitted_data.error.response.empty()) {
    return splitted_data.error;
  }
  else if (method == "GET") {
    return _get(splitted_data.strings[1]);
  }
  else if (method == "SET") {
    return _set(splitted_data.strings[1], splitted_data.strings[2]);
  }
  else if (method == "DELETE") {
    return _delete(splitted_data.strings[1]);
  }
  else {
    struct server_response res;
    res.response = "Please use a valid method\033[0m\n";
    res.status = "\033[31mERR";
    return res;
  }
}

void Parser::free_map() {
  delete this->_map;
}