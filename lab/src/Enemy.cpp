#include "Enemy.h"
#include "Construction_Enemy.h"
#include <queue>
#include <algorithm>

using std::dynamic_pointer_cast;

void Enemy::set_max_hp(int max_hp_in){
    max_hp = max_hp_in;
}

void Enemy::try_to_reg_hp(){
    if (hp + regeneration_rate > max_hp){
        hp = max_hp;
    }else{
         hp += regeneration_rate;
    }
}

void Enemy::set_hp(int hp_in){
    hp = hp_in;
}

void Enemy::set_regeneration_rate(int regeneration_rate_in){
    regeneration_rate = regeneration_rate_in;
}

void Enemy::set_movement_speed(int movement_speed_in){
    movement_speed = movement_speed_in;
}

void Enemy::set_damage_castle(int damage_castle_in){
    damage_castle = damage_castle_in;
}

void Enemy::add_aura(shared_ptr<Aura> aura_in){
    auras.push_back(aura_in);
}

int Enemy::get_hp() const{
    return hp;
}

int Enemy::get_max_hp() const{
    return max_hp;
}

int Enemy::get_regeneration_rate() const{
    return regeneration_rate;
}

int Enemy::get_movement_speed() const{
    return movement_speed;
}

int Enemy::get_damage_castle() const{
    return damage_castle;
}

vector <shared_ptr<Aura>> Enemy::get_auras() const{
    return auras;
}


bool Enemy::attack_castle(int radius_of_action){
     //   std::cout << "My hp is: " << get_hp() << std::endl;
    bool result = 0;
    int size_x = (get_landscape() -> get_size_play_field())[0];
    int size_y = (get_landscape() -> get_size_play_field())[1];
    Matrix<Cell> cells = landscape -> cells;
    for (int x = -radius_of_action; x <= radius_of_action; ++x){
        for (int y = -radius_of_action; y <= radius_of_action; ++y){  
            if ( ((current_position[0] + x) >= size_x) || ((current_position[1] + y) >= size_y)  || ((current_position[0] + x) < 0) || ((current_position[1] + y) < 0) ) continue;      //Проверка, что не вышли за границы
            Cell cell = cells[current_position[0] + x][current_position[1] + y];

            if (cell.ptr_entities.size() == 0){
                 continue;
            }else for(shared_ptr<Entity> entity : cell.ptr_entities){
                if (dynamic_pointer_cast<Construction>(entity) == nullptr){
                    continue;
                }else{
                    if (dynamic_pointer_cast<Castle>(entity) != nullptr){
                        result = 1;
                        int castle_hp = dynamic_pointer_cast<Castle>(entity) -> get_current_hp();
                        int damage_castle = get_damage_castle();
                        if ((castle_hp - damage_castle) < 0){
                            dynamic_pointer_cast<Castle>(entity) -> set_current_hp(0);
                        }else {dynamic_pointer_cast<Castle>(entity) -> set_current_hp(castle_hp - damage_castle);}
                        //std::cout << "Attack castle, castle hp is " << dynamic_pointer_cast<Castle>(entity) -> get_current_hp() << std::endl;

                    }
                    if (dynamic_pointer_cast<Wall>(entity) != nullptr){
                        attack_wall(dynamic_pointer_cast<Wall>(entity));
                    }
                }
            }
        }
    }
    return result;
}

size_t Enemy::shortest_way_size(){
        return shortest_way.size();
}

bool Enemy::check_shortest_way(){
        int len = shortest_way.size();
        if (len == 0) return 1;
        for (int i = 0; i < len; ++i){              //Проверка, не поменялся ли кратчайший путь
                int x = shortest_way[i].x;
                int y = shortest_way[i].y;
                Cell cell = (landscape -> cells).matrix[x][y];
                if (cell.ptr_entities.size() == 0){
                        continue;
                }else for(shared_ptr<Entity> entity : cell.ptr_entities){
                        if (dynamic_pointer_cast<Construction>(entity) == nullptr){
                                continue;
                        }else{
                                if (dynamic_pointer_cast<Wall>(entity)){        //Проверка, что пользователь не добавил стену
                                        return 1;
                                }
                                if (dynamic_pointer_cast<Tower>(entity)){        //Проверка, что пользователь не добавил стену
                                        return 1;
                                }
                        }
                }
        }
        return 0;
}

void Light_Infantry::to_do_step(){
        if (hp == 0){
                int index = 0;
                for (auto entity : (landscape->cells)[current_position[0]][current_position[1]].ptr_entities){
                        if (dynamic_pointer_cast<Light_Infantry>(entity) != nullptr){
                                if (dynamic_pointer_cast<Light_Infantry>(entity) -> name == this -> name) {
                                        (landscape->cells)[current_position[0]][current_position[1]].ptr_entities.erase((landscape->cells)[current_position[0]][current_position[1]].ptr_entities.begin()+index);
                                        break;
                                }
                        }
                        index += 1;
                }
                return;
        }
        try_to_reg_hp();
      //  std::cout << current_position[0] << current_position[1] << std::endl;
        if (attack_castle(1) == 0){     //Атакуем замок если можем
                if (check_shortest_way() == 1) shortest_way = fsw_castle();      //Меняем кратчайший путь, если пользователь добавил стену или его нет
                int index = 0;
                for (auto entity : (landscape->cells)[current_position[0]][current_position[1]].ptr_entities){
                        if (dynamic_pointer_cast<Light_Infantry>(entity) != nullptr){
                                if (dynamic_pointer_cast<Light_Infantry>(entity) -> name == this -> name) {
                                        (landscape->cells)[current_position[0]][current_position[1]].ptr_entities.erase((landscape->cells)[current_position[0]][current_position[1]].ptr_entities.begin()+index);
                                        break;
                                }
                        }
                        index += 1;
                }
                
                set_current_position(shortest_way.back().x, shortest_way.back().y);
                shortest_way.pop_back();
                
                (landscape->cells)[current_position[0]][current_position[1]].ptr_entities.push_back(std::make_shared<Light_Infantry>(*this));
        }    
        if (auras.size() != 0){
                for (auto aura : auras){
                        aura -> make_effect(landscape, std::make_unique<Enemy>(*this));
                        std::cout << "Aura do" << std::endl;
                }
        }
           
}

int Light_Infantry::get_probability_of_evasion(){
        return probability_of_evasion;
}

void Heavy_Infantry::to_do_step(){
        if (hp == 0){
                int index = 0;
                for (auto entity : (landscape->cells)[current_position[0]][current_position[1]].ptr_entities){
                        if (dynamic_pointer_cast<Heavy_Infantry>(entity) != nullptr){
                                if (dynamic_pointer_cast<Heavy_Infantry>(entity) -> name == this -> name) {
                                        (landscape->cells)[current_position[0]][current_position[1]].ptr_entities.erase((landscape->cells)[current_position[0]][current_position[1]].ptr_entities.begin()+index);
                                        break;
                                }
                        }
                        index += 1;
                }
                return;
        }
        try_to_reg_hp();

        if (attack_castle(radius_of_action) == 0){     //Атакуем замок если можем
                if (check_shortest_way() == 1) shortest_way = fsw_castle();      //Меняем кратчайший путь, если пользователь добавил стену или его нет
                int index = 0;
                for (auto entity : (landscape->cells)[current_position[0]][current_position[1]].ptr_entities){
                        if (dynamic_pointer_cast<Heavy_Infantry>(entity) != nullptr){
                                if (dynamic_pointer_cast<Heavy_Infantry>(entity) -> name == this -> name) {
                                        (landscape->cells)[current_position[0]][current_position[1]].ptr_entities.erase((landscape->cells)[current_position[0]][current_position[1]].ptr_entities.begin()+index);
                                        break;
                                }
                        }
                        index += 1;
                }
                
                set_current_position(shortest_way.back().x, shortest_way.back().y);
                shortest_way.pop_back();
                
                (landscape->cells)[current_position[0]][current_position[1]].ptr_entities.push_back(std::make_shared<Heavy_Infantry>(*this));
        }
        if (auras.size() != 0){
                for (auto aura : auras){
                        if (dynamic_pointer_cast<Max_Hp>(aura) != nullptr){
                        aura -> make_effect(landscape, std::make_unique<Enemy>(*this));
                                                std::cout << "Aura do" << std::endl;
                        }
                }
        }
}

void Heavy_Infantry::attack_wall(shared_ptr<Wall> wall){
        int wall_hp = wall -> get_current_hp();
        int max_wall_hp = wall -> get_max_hp();
        if ((wall_hp - damage_walls) < 0) {
                wall -> set_current_hp(0);
        } else wall -> set_current_hp(wall_hp - damage_walls);
        std::cout << "Mega_Infantry attack wall with damage: " << damage_walls << " New wall hp is:" <<   wall -> get_current_hp() << std::endl;
}

void Mega_Infantry::to_do_step(){
        if (hp == 0) {
                int index = 0;
                for (auto entity : (landscape->cells)[current_position[0]][current_position[1]].ptr_entities){
                        if (dynamic_pointer_cast<Mega_Infantry>(entity) != nullptr) {
                                if (dynamic_pointer_cast<Mega_Infantry>(entity) -> name == this -> name) {
                                        for (int i = 0; i < 5; ++i) {
                                                Light_Infantry l_enemy(unique_name(20), castle_position, current_position, landscape, l_hp, l_hp, l_reg_rate, e_move, l_castle_damage, l_probability);
                                                shared_ptr<Lair> lair = dynamic_pointer_cast<Lair>(l_enemy.get_landscape()->cells[0][9].ptr_entities[0]);
                                                lair->enemies.push_back(std::make_shared<Light_Infantry>(l_enemy));
                                                (landscape -> cells).matrix[current_position[0]][current_position[1]].ptr_entities.push_back(std::make_shared<Light_Infantry>(l_enemy));
                                        }
                                        (landscape->cells)[current_position[0]][current_position[1]].ptr_entities.erase((landscape->cells)[current_position[0]][current_position[1]].ptr_entities.begin()+index);
                                        break;
                                }
                        }
                        index += 1;
                }
                return;
        }
        try_to_reg_hp();

        if (attack_castle(radius_of_action) == 0) {                             // Атакуем замок если можем
                if (check_shortest_way() == 1) shortest_way = fsw_castle();     // Меняем кратчайший путь, если пользователь добавил стену или его нет
                int index = 0;
                for (auto entity : (landscape->cells)[current_position[0]][current_position[1]].ptr_entities){
                        if (dynamic_pointer_cast<Mega_Infantry>(entity) != nullptr){
                                if (dynamic_pointer_cast<Mega_Infantry>(entity) -> name == this -> name) {

                                        (landscape->cells)[current_position[0]][current_position[1]].ptr_entities.erase((landscape->cells)[current_position[0]][current_position[1]].ptr_entities.begin()+index);
                                        break;
                                }
                        }
                        index += 1;
                }
                
                set_current_position(shortest_way.back().x, shortest_way.back().y);
                shortest_way.pop_back();
                
                (landscape->cells)[current_position[0]][current_position[1]].ptr_entities.push_back(std::make_shared<Mega_Infantry>(*this));
        }
        if (auras.size() != 0){
                for (auto aura : auras){
                        if (dynamic_pointer_cast<Max_Hp>(aura) != nullptr){
                                aura -> make_effect(landscape, std::make_unique<Enemy>(*this));
                                std::cout << "Aura do" << std::endl;
                        }
                }
        }
}

void Mega_Infantry::attack_wall(shared_ptr<Wall> wall){
        int wall_hp = wall -> get_current_hp();
        int max_wall_hp = wall -> get_max_hp();
        if ((wall_hp - damage_walls) < 0){
                wall -> set_current_hp(0);
        }else wall -> set_current_hp(wall_hp - damage_walls);
        std::cout << "Heavy_Infantry attack wall with damage: " << damage_walls << " New wall hp is:" <<   wall -> get_current_hp() << std::endl;
}

void Aviation::to_do_step(){
        if (hp == 0){
                int index = 0;
                for (auto entity : (landscape->cells)[current_position[0]][current_position[1]].ptr_entities){
                        if (dynamic_pointer_cast<Aviation>(entity) != nullptr){
                                if (dynamic_pointer_cast<Aviation>(entity) -> name == this -> name) {
                                        (landscape->cells)[current_position[0]][current_position[1]].ptr_entities.erase((landscape->cells)[current_position[0]][current_position[1]].ptr_entities.begin()+index);
                                        break;
                                }
                        }
                        index += 1;
                }
                return;
        }
        try_to_reg_hp();

        if (attack_castle(1) == 0){     //Атакуем замок если можем
            //    if (check_shortest_way() == 1) 
                if (shortest_way_size() == 0) shortest_way = fsw_castle();      //Меняем кратчайший путь, если пользователь добавил стену или его нет
                int index = 0;
                for (auto entity : (landscape->cells)[current_position[0]][current_position[1]].ptr_entities){
                        if (dynamic_pointer_cast<Aviation>(entity) != nullptr){
                                if (dynamic_pointer_cast<Aviation>(entity) -> name == this -> name) {
                                        (landscape->cells)[current_position[0]][current_position[1]].ptr_entities.erase((landscape->cells)[current_position[0]][current_position[1]].ptr_entities.begin()+index);
                                        break;
                                }
                        }
                        index += 1;
                }
                
                set_current_position(shortest_way.back().x, shortest_way.back().y);
                shortest_way.pop_back();
                
                (landscape->cells)[current_position[0]][current_position[1]].ptr_entities.push_back(std::make_shared<Aviation>(*this));
        }
        if (auras.size() != 0){
                for (auto aura : auras){
                        if (dynamic_pointer_cast<Max_Hp>(aura) != nullptr){
                        aura -> make_effect(landscape, std::make_unique<Enemy>(*this));
                                                std::cout << "Aura do" << std::endl;
                        }
                }
        }
}

void Aviation::attack_wall(shared_ptr<Wall> wall){
        if (amout_of_ammunition == 0) return;
        int wall_hp = wall -> get_current_hp();
        int max_wall_hp = wall -> get_max_hp();
        if ((wall_hp - damage_walls) < 0){
                wall -> set_current_hp(0);
        }else wall -> set_current_hp(wall_hp - damage_walls);
        amout_of_ammunition -= 1;
        std::cout << "Aviation attack wall with damage: " << damage_walls << " New wall hp is:" <<   wall -> get_current_hp() << std::endl;

}


struct Cell_s{
        Cell cell;
        bool visited;
        shared_ptr<Cell_s> prev; 
};

vector<Cell> bfs_castle(Matrix<Cell_s>& cell_s, Cell_s& cur_pos, Cell_s& castle_pos){
    std::queue<Cell_s> queue;
//    std::cout << "Start" << std::endl;
    queue.push(cur_pos);
  //  std::cout << cur_pos.cell.x << cur_pos.cell.y << std::endl;
    cur_pos.visited = true;
    vector<Cell> result;
    while (queue.size() > 0){
                int flag = 0;           //Проверка, что данная клетка не стена и не башня
                Cell_s cell = queue.front();      //Верхний элемент очереди
                queue.pop();
            //    if (cell.cell.ptr_entities.size() != 0){
                for (shared_ptr<Entity> entity : cell.cell.ptr_entities){
                        if (dynamic_pointer_cast<Wall>(entity) != nullptr){
                            flag = 1;
                            continue;
                        }else if (dynamic_pointer_cast<Tower>(entity) != nullptr){
                            flag = 1;
                            continue;
                        }
                }
            //    }
                    

                if (static_cast<int>(cell.cell.type) != 1) flag = 1;

             //   std::cout << "Flag check: " << flag << std::endl;

                if (flag == 1) continue;
               
                for (int x = -1; x <= 1; x++){
                        for (int y = -1; y <= 1; y++){
                                if ((abs(x) + abs(y)) == 2) continue; //Не ходим по диагонали
                          //      std::cout << "Here is check" << std::endl;
                                if (  ( ( (cell.cell.x + x) < 0) || ( (cell.cell.x + x) >= cell_s.size()) )   || ( ( (cell.cell.y + y) < 0) || ( (cell.cell.y + y) >= cell_s[0].size()) )) continue;      //Проверка, что не вышли за рамки матрицы

                         //      std::cout << "Checking: " << cell.cell.x + x << cell.cell.y + y << std::endl;
                                if (cell_s[cell.cell.x + x][cell.cell.y + y].visited == true) continue;
                          //      std::cout << queue.size() << " " << cell.cell.x + x << cell.cell.y + y << cell_s[cell.cell.x][cell.cell.y].visited << std::endl; 

                                queue.push(cell_s[cell.cell.x + x][cell.cell.y + y]); 
                               // std::cout << "Was added: " << cell.cell.x + x << cell.cell.y + y << std::endl; 
                                cell_s[cell.cell.x + x][cell.cell.y + y].visited = true;
                                cell_s[cell.cell.x + x][cell.cell.y + y].prev = std::make_shared<Cell_s>(cell);  
                             //   std::cout << "1: " << cell.cell.x << cell.cell.y << std::endl;
                             //   std::cout << "2: " << castle_pos.cell.x << castle_pos.cell.y << std::endl;

                                if ( ( (cell.cell.x + x) == castle_pos.cell.x) && ( (cell.cell.y + y) == castle_pos.cell.y)){
                                     //   std::cout << (cell_s[cell.cell.x + x][cell.cell.y + y].prev == nullptr) << std::endl;
                                 //       std::cout << "End1" << std::endl;
                                        Cell_s temp = cell;
                                        while ( (temp.cell.x != cur_pos.cell.x) || (temp.cell.y != cur_pos.cell.y) ){  
                                                result.push_back(temp.cell);
                                                
                                                //std::cout << (temp.prev == nullptr) << std::endl;

                                                temp = *((cell_s.matrix[temp.cell.x][temp.cell.y]).prev);

  //                                              std::cout << "prev" << temp.cell.x << temp.cell.y << std::endl;
    //                                            std::cout << cur_pos.cell.x << cur_pos.cell.y << std::endl;


                                        }
                                        
                                }
                        }
                }
    }
 //   std::cout << "End to do" << std::endl;
    return result;
}

vector<Cell> Enemy::fsw_castle(){
        Matrix<Cell_s> cell_s;
        shared_ptr<Landscape> land = landscape;
        int size_x = (land -> get_size_play_field())[0]; 
        int size_y = (land -> get_size_play_field())[1];
        Matrix<Cell> cells = land -> cells;
        cell_s.matrix.resize(10);
        for (int i = 0; i < size_x; ++i){
                cell_s.matrix[i].resize(10);
                for (int j = 0; j < size_y; ++j){
                        cell_s[i][j].cell = cells[i][j];
                        cell_s[i][j].visited = 0;
                }
        }
        Cell_s cur_pos;
        cur_pos.cell = cells.matrix[current_position[0]][current_position[1]];
        Cell_s castle_pos;
        castle_pos.cell = cells.matrix[castle_position[0]][castle_position[1]];
        return bfs_castle(cell_s, cur_pos, castle_pos);
}

vector<Cell> Aviation::fsw_castle(){
        vector<Cell> result;
        shared_ptr<Landscape> land = landscape;
        int size_x = (land -> get_size_play_field())[0];
        int size_y = (land -> get_size_play_field())[1];
        Matrix<Cell> cells = land -> cells;
        int cur_pos_x = cells[current_position[0]][current_position[1]].x;
        int cur_pos_y = cells[current_position[0]][current_position[1]].y;
        int castle_pos_x = cells[castle_position[0]][castle_position[1]].x;
        int castle_pos_y = cells[castle_position[0]][castle_position[1]].y;
        while (cur_pos_x != castle_pos_x){
                if (cur_pos_x > castle_pos_x){
                        cur_pos_x -= 1;
                        if (cur_pos_y > castle_pos_y){
                                cur_pos_y -= 1;
                                result.push_back(cells[cur_pos_x][cur_pos_y]);
                        }else{
                                cur_pos_y += 1;
                                result.push_back(cells[cur_pos_x][cur_pos_y]);
                        }
                }else{
                        cur_pos_x += 1;
                        if (cur_pos_y > castle_pos_y){
                                cur_pos_y -= 1;
                                result.push_back(cells[cur_pos_x][cur_pos_y]);
                        }else{
                                cur_pos_y += 1;
                                result.push_back(cells[cur_pos_x][cur_pos_y]);
                        }
                }
        }
        while (cur_pos_y != castle_pos_y){
                if (cur_pos_y > castle_pos_y){
                        cur_pos_y -= 1;
                        result.push_back(cells[cur_pos_x][cur_pos_y]);
                }else{
                        cur_pos_y += 1;
                        result.push_back(cells[cur_pos_x][cur_pos_y]);
                }
        }
        std::reverse(result.begin(), result.end());
        return result;
}

Enemy::~Enemy(){};
Heavy_Infantry::~Heavy_Infantry(){};
Aviation::~Aviation(){};
Light_Infantry::~Light_Infantry(){};
Mega_Infantry::~Mega_Infantry(){};