#include "Construction.h"

void Construction::set_name(string name_in){
    name = name_in;
}

string Construction::get_name() const{
    return name;
}

void Castle::to_do_step(){
    if (current_hp == 0) return;
    int regeneration_value = characteristics.get_characteristic("speed_regeneration").get_value();
    int max_hp = characteristics.get_characteristic("max_hp").get_value();
    if (current_hp + regeneration_value > max_hp){
        current_hp = max_hp;
    }else{
         current_hp += regeneration_value;
    }
    amount_gold += level * 50;
}

Castle::Castle(std::shared_ptr<Landscape> landscape_in){
    landscape = landscape_in;
  //  std::cout << "Castle is start" << std::endl;
    name = "Castle";
    level = 1;
    current_hp = 5000;
    amount_gold = 500;
    Characteristic max_hp ("max_hp", 5000, 1);
    Characteristic speed_regeneration ("speed_regeneration", 15, 1);
    Characteristic profitability ("profitability", 200, 1);
    characteristics.add_characteristic(max_hp);
    characteristics.add_characteristic(speed_regeneration);
    characteristics.add_characteristic(profitability);
 //   std::cout << "Castle is end" << std::endl;
}


void Castle::up_level(){
    level += 1;
    upgrade_characteristics();
}

void Castle::upgrade_characteristics(){
    for (auto& characteristic: characteristics.characteristics){
        characteristic.set_value(characteristic.get_value() * 1.2);
        characteristic.up_level();
    }
}

void Castle::set_current_hp(int current_hp_in){
    current_hp = current_hp_in;
}

void Castle::set_amount_gold(int amount_gold_in){
    amount_gold = amount_gold_in;
}

void Castle::add_characteristic(Characteristic characteristic){
    characteristics.add_characteristic(characteristic);
}

int Castle::get_level() const{
    return level;
}

int Castle::get_current_hp() const{
    return current_hp;
}

int Castle::get_amount_gold() const{
    return amount_gold;
}

Table_Characteristics Castle::get_characteristics() const{
    return characteristics;
}

Wall::Wall(int cur_pos[2], int max_hp_in, shared_ptr<Landscape> landscape_in){
    current_position[0] = cur_pos[0];
    current_position[1] = cur_pos[1];
    landscape = landscape_in;
    current_hp = max_hp_in;
    max_hp = max_hp_in;

}

void Wall::to_do_step(){
        if (current_hp == 0){
                int index = 0;
                vector<shared_ptr<Entity>> entities = (landscape -> cells).matrix[current_position[0]][current_position[1]].ptr_entities;
                for (auto entity : entities){
                        if (std::dynamic_pointer_cast<Wall>(entity) != nullptr){
                                break;
                        }
                        index += 1;
                }
                (landscape -> cells).matrix[current_position[0]][current_position[1]].ptr_entities.erase( (landscape -> cells).matrix[current_position[0]][current_position[1]].ptr_entities.begin() + index);
                return;
        }
};


void Wall::set_current_hp(int current_hp_in){
    current_hp = current_hp_in;
}

void Wall::set_max_hp(int max_hp_in){
    max_hp = max_hp_in;
}

int Wall::get_current_hp() const{
    return current_hp;
}

int Wall::get_max_hp() const{
    return max_hp;
}


Construction::~Construction(){};
Castle::~Castle(){};
Wall::~Wall(){};
