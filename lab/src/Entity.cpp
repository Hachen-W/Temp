#include "Entity.h"

Entity::Entity(shared_ptr<Landscape>(landscape_in), int current_position_in[2]){
    landscape = landscape_in;
    current_position[0] = current_position_in[0];
    current_position[1] = current_position_in[1];
}

void Entity::set_current_position(int x, int y){
    current_position[0] = x;
    current_position[1] = y;
}

void Entity::set_landscape(shared_ptr<Landscape> landscape_in){
    landscape = landscape_in;
}

array <int, 2> Entity::get_current_position() const{
    array <int, 2> position = {current_position[0], current_position[1]};
    return position;
}

shared_ptr<Landscape> Entity::get_landscape() const{
    return landscape;
}

Entity::~Entity(){}
