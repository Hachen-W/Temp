#include <stdexcept>
#include <iostream>
#include "Characteristics.h"

void Characteristic::set_name(string name_in){
    name = name_in;
}

void Characteristic::set_value(int value_in){
    value = value_in;
}

void Characteristic::up_level(){
    level += 1;
}

string Characteristic::get_name() const{
    return name;
}

int Characteristic::get_value() const{ 
    return value;
}

int Characteristic::get_level() const{
    return level;
}

void Table_Characteristics::add_characteristic(Characteristic characteristic){
    characteristics.push_back(characteristic);
}

vector<Characteristic> Table_Characteristics::get_characteristics() const{
    return characteristics;
}

Characteristic Table_Characteristics::get_characteristic(string name) const{
    int len = characteristics.size();
    for (int i = 0; i < len; ++i){
        if (characteristics[i].get_name() != name) continue;
        return characteristics[i];
    }
    throw std::invalid_argument("Invalid name");
}

