#include "Landscape.h"

void Landscape::set_size_play_field(int x, int y){
    size_play_field[0] = x;
    size_play_field[1] = y;
}

array<int, 2> Landscape::get_size_play_field() const{
    array<int, 2> size = {size_play_field[0], size_play_field[1]};
    return size;
}
