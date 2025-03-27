#include "Road.h"


std::string Road::get_name() const{return name;}
int Road::get_length() const{return length;}

void Road::set_name(std::string new_name) {name = std::move(new_name);}
void Road::set_length(int new_length) {length = new_length;}