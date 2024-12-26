#include "Aura.h"

using std::dynamic_pointer_cast;

void Speed::make_effect(shared_ptr<Landscape> landscape, unique_ptr<Enemy> enemy){
    int size_x = ( landscape-> get_size_play_field())[0];
    int size_y = ( landscape -> get_size_play_field())[1];

    array <int, 2> current_pos = enemy -> get_current_position();
    Matrix<Cell> cells;
    cells = landscape -> cells;
    for (int x = -radius; x < radius; ++x){
        for (int y = -radius; y < radius; ++y){
            if ( ((current_pos[0] + x) >= size_x) || ((current_pos[1] + y) >= size_y) || ((current_pos[0] + x) < 0) || ((current_pos[1] + y) < 0) ) continue;
            Cell cell = cells[current_pos[0] + x][current_pos[1] + y];
            if (cell.ptr_entities.size() == 0){
                 continue;
            }else for(shared_ptr<Entity> entity : cell.ptr_entities){
                if (dynamic_pointer_cast<Enemy>(entity) != nullptr){    ///If find enemy -> make_effect
                    int cur_speed = dynamic_pointer_cast<Enemy>(entity) -> get_movement_speed();
                    dynamic_pointer_cast<Enemy>(entity) -> set_movement_speed(cur_speed + value);
                }
            }
        }
    }
}

void Regeneration::make_effect(shared_ptr<Landscape> landscape, unique_ptr<Enemy> enemy){
    int size_x = ( landscape-> get_size_play_field())[0];
    int size_y = ( landscape -> get_size_play_field())[1];

    array <int, 2> current_pos = enemy -> get_current_position();
    Matrix<Cell> cells = landscape -> cells;
    for (int x = -radius; x < radius; ++x){
        for (int y = -radius; y < radius; ++y){
            if ( ((current_pos[0] + x) >= size_x) || ((current_pos[1] + y) >= size_y) || ((current_pos[0] + x) < 0) || ((current_pos[1] + y) < 0) ) continue;
            Cell cell = cells[current_pos[0] + x][current_pos[1] + y];
            if (cell.ptr_entities.size() == 0){
                 continue;
            }else for(shared_ptr<Entity> entity : cell.ptr_entities){
                if (dynamic_pointer_cast<Enemy>(entity) != nullptr){
                    int cur_re = dynamic_pointer_cast<Enemy>(entity) -> get_regeneration_rate();
                    dynamic_pointer_cast<Enemy>(entity) -> set_regeneration_rate(cur_re + value);
                }
            }
        }
    }
}

void Max_Hp::make_effect(shared_ptr<Landscape> landscape, unique_ptr<Enemy> enemy){
    int size_x = ( landscape-> get_size_play_field())[0];
    int size_y = ( landscape -> get_size_play_field())[1];

    array <int, 2> current_pos = enemy -> get_current_position();
    Matrix<Cell> cells = landscape -> cells;
    for (int x = -radius; x <= radius; ++x){
        for (int y = -radius; y <= radius; ++y){
            if ( ((current_pos[0] + x) >= size_x) || ((current_pos[1] + y) >= size_y) || ((current_pos[0] + x) < 0) || ((current_pos[1] + y) < 0) ) continue;
            Cell cell = cells.matrix[current_pos[0] + x][current_pos[1] + y];
            if (cell.ptr_entities.size() == 0){
                 continue;
            }else for(shared_ptr<Entity> entity : cell.ptr_entities){
                if (dynamic_pointer_cast<Enemy>(entity) != nullptr){
                    int cur_max_hp = dynamic_pointer_cast<Enemy>(entity) -> get_max_hp();
                    dynamic_pointer_cast<Enemy>(entity) -> set_max_hp(cur_max_hp + value);
                //    std::cout << "New hp is: " << cur_max_hp + value << std::endl;
                }
            }
        }
    }
}

