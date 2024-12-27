#include <random>
#include <ctime>
#include <stdexcept>
#include <algorithm>
#include "Construction_Enemy.h"

using std::mt19937;
using std::dynamic_pointer_cast;
using std::make_shared;

void Tower::to_do_step(){
    int size_x = (landscape -> get_size_play_field())[0];
    int size_y = (landscape -> get_size_play_field())[1];
    const int radius_action = characteristics.get_characteristic("radius_action").get_value();
    array <int, 2> current_pos = get_current_position();
    Matrix<Cell> cells = landscape -> cells;
    for (int x = -radius_action; x <= radius_action; ++x){
        for (int y = -radius_action; y <= radius_action; ++y){
            if (abs(x) + abs(y) == 2 * radius_action) continue;
            if ( ((current_position[0] + x) >= size_x) || ((current_position[1] + y) >= size_y) || ((current_position[0] + x) < 0) || ((current_position[1] + y) < 0)) continue;
            Cell cell = cells[current_pos[0] + x][current_pos[1] + y];
            if (cell.ptr_entities.size() == 0){
                 continue;       
            }else for(shared_ptr<Entity> entity : cell.ptr_entities){
                if (dynamic_pointer_cast<Enemy>(entity) != nullptr){
                    attack_enemy(dynamic_pointer_cast<Enemy>(entity));
                }
            }
        }
    }
}

void Tower::attack_enemy(shared_ptr<Enemy> enemy){
    if (dynamic_pointer_cast<Light_Infantry>(enemy) != nullptr){
        int probability_of_evasion = dynamic_pointer_cast<Light_Infantry>(enemy) -> get_probability_of_evasion();
        mt19937 eng(time(nullptr));
        if (eng() % 100 < probability_of_evasion){
            std::cout << "Tower miss!" <<std::endl;
            return;
        }
    }
    int damage = characteristics.get_characteristic("damage").get_value();
    int enemy_hp = enemy -> get_hp();
    if ( (enemy_hp - damage) <= 0){
        enemy -> set_hp(0);
    }else enemy -> set_hp(enemy_hp - damage);

    std::cout << "Tower attack enemy" /* enemy -> name*/<<" with damage: " << damage << " Enemy hp is: " << enemy -> get_hp() << std::endl;
}

void Tower::up_level(){
    level += 1;
    upgrade_characteristics();
}

void Tower::upgrade_characteristics(){
    for (auto& characteristic: characteristics.characteristics){
        characteristic.set_value(characteristic.get_value() * 1.2);
        characteristic.up_level();
    }
}

Tower::Tower(int current_position_in[2], shared_ptr<Landscape> landscape_in, int level_in, int radius_action_in, int damage_in, int rate_fire_in, int cost_in){ 
    landscape = landscape_in;
    current_position[0] = current_position_in[0];
    current_position[1] = current_position_in[1];
    Characteristic radius_action("radius_action", radius_action_in, 1);
    Characteristic damage("damage", damage_in, 1); //30
    Characteristic rate_fire("rate_of_fire", rate_fire_in, 1);
    Characteristic cost("cost", cost_in, 1); // 500
    this->add_characteristic(radius_action);
    this->add_characteristic(damage);
    this->add_characteristic(rate_fire);
    this->add_characteristic(cost);
    level = level_in;
  //  std::cout << "Tower created" <<  (this -> get_characteristics()).characteristics.size() << std::endl;
}


void Tower::add_characteristic(Characteristic& characteristic){
    characteristics.add_characteristic(characteristic);
}

int Tower::get_level() const{
    return level;
}

Table_Characteristics Tower::get_characteristics() const{
    return characteristics;
}

#define e_move 1    //Клеток

#define l_hp 50
#define l_reg_rate 10   //%
#define l_castle_damage 20
#define l_probability 5 //%

#define h_hp 100
#define h_reg_rate 7
#define h_castle_damage 30
#define h_walls_damage 25
#define h_radius_action 2

#define a_hp 75
#define a_reg_rate 10 
#define a_castle_damage 40
#define a_walls_damage  40
#define a_radius_action 2
//to do mega

string unique_name(int dis){
    std::random_device rd;
    mt19937 eng(rd());
    string name;
    for (int i = 0; i < 10; ++i){
        name += char((eng() % 100) + dis );
    }
    return name;
}


vector<shared_ptr<Enemy>> Lair::init_enemies(int level){
        string name_l = unique_name(20);
        string name_h = unique_name(30);
        string name_a = unique_name(40);
        string name_m = unique_name(50);
        //to do mega
        int castle_pos[2] = {0,9};
        int cur_pos[2] = {9,0};
        Light_Infantry l_enemy(name_l, castle_pos, cur_pos, landscape, l_hp + level*10, l_hp + level*10, l_reg_rate + level*2, e_move, l_castle_damage + level*5, l_probability);
        Heavy_Infantry h_enemy(name_h, castle_pos, cur_pos, landscape, h_hp + level*20, h_hp + level*20, h_reg_rate + level, e_move, h_castle_damage + level*7, h_walls_damage + level*2, h_radius_action + level / 5);
        Aviation a_enemy(name_a, castle_pos, cur_pos, landscape, a_hp + level*15, a_hp + level*15, a_reg_rate + level, e_move, a_castle_damage + level*5, a_walls_damage + level*4, a_radius_action + level / 5);
        Mega_Infantry m_enemy(name_h, castle_pos, cur_pos, landscape, h_hp + level*20, h_hp + level*20, h_reg_rate + level, e_move, h_castle_damage + level*7, h_walls_damage + level*2, h_radius_action + level / 5);
        vector<shared_ptr<Enemy>> enemies = {make_shared<Light_Infantry>(l_enemy), make_shared<Heavy_Infantry>(h_enemy), make_shared<Aviation>(a_enemy), make_shared<Mega_Infantry>(m_enemy)};

        return enemies;
}


Lair::Lair(std::shared_ptr<Landscape> landscape_in){
    landscape = landscape_in;
    level = 0;
    count_step = 0;
    matrix_enemy.matrix.resize(3);
    matrix_enemy.matrix[0].resize(30);
    matrix_enemy.matrix[1].resize(30);
    matrix_enemy.matrix[2].resize(30);
    for (int i = 0; i < 30; ++i){
        vector<shared_ptr<Enemy>> enemies = init_enemies(i);
        matrix_enemy.matrix[0][i] = enemies[0];
        matrix_enemy.matrix[1][i] = enemies[1];
        matrix_enemy.matrix[2][i] = enemies[2];
    }
    int pos_lair[2] = {9, 0};
    current_position[0] = pos_lair[0];
    current_position[1] = pos_lair[1];
}

void Lair::reinit_enemies(){
    matrix_enemy.matrix.clear();
    matrix_enemy.matrix.resize(4);
    matrix_enemy.matrix[0].resize(30);
    matrix_enemy.matrix[1].resize(30);
    matrix_enemy.matrix[2].resize(30);
    matrix_enemy.matrix[3].resize(30);
    for (int i = 0; i < 30; ++i){
        vector<shared_ptr<Enemy>> enemies = init_enemies(i);
        matrix_enemy.matrix[0][i] = enemies[0];
        matrix_enemy.matrix[1][i] = enemies[1];
        matrix_enemy.matrix[2][i] = enemies[2];
        matrix_enemy.matrix[3][i] = enemies[3];
    }
    int pos_lair[2] = {9, 0};
    current_position[0] = pos_lair[0];
    current_position[1] = pos_lair[1];
}

void Lair::to_do_step(){
    vector<int> numbers;
    for (auto entity : enemies){
      //  (entity -> set_landscape)(landscape);
        int index = 0;
        for (auto ent : (landscape -> cells).matrix[entity->get_current_position()[0]][entity->get_current_position()[1]].ptr_entities){
            if (dynamic_pointer_cast<Enemy>(ent) != nullptr){
                    if (dynamic_pointer_cast<Enemy>(ent) -> name == entity -> name) {
                            entity -> set_hp(dynamic_pointer_cast<Enemy>(ent) -> get_hp());
                            
                            if (entity -> get_hp() == 0) numbers.push_back(index);
                            break;
                    }
                    index += 1;
            }
            
        }
    } 

    count_step += 1;
    if (count_step % 10 == 0){ 
        spawn_enemy_time();
        level = (level + 1) % 30;
        if (level == 29){reinit_enemies();}
    }else if (count_step % 2 == 0){
        spawn_enemy();
        level = (level + 1) % 30;
        if (level == 29){reinit_enemies();}
    }
}

void Lair::spawn_enemy(){
    mt19937 eng(time(nullptr));
    int x_pos = current_position[0];
    int y_pos = current_position[1];
    Enemy_Type type = static_cast<Enemy_Type>(eng() % 3 + 1); 
    Cell cell = (landscape -> cells).matrix[x_pos][y_pos];
    (landscape -> cells).matrix[x_pos][y_pos].ptr_entities.push_back(matrix_enemy.matrix[static_cast<int>(type)-1][level]); 
    enemies.push_back(matrix_enemy.matrix[static_cast<int>(type)-1][level]);
//    std::cout << "type:" << static_cast<int>(type) << "Was spawned" << x_pos << y_pos << std::endl;
}

#define speed_r 3
#define speed_v 1

#define reg_r 2
#define reg_v 2

#define max_hp_r 3
#define max_hp_v 15


void Lair::spawn_enemy_time(){
    mt19937 eng(time(nullptr));
    int x_pos = current_position[0];
    int y_pos = current_position[1];
    Enemy_Type type = static_cast<Enemy_Type>(eng() % 3 + 1); 
    Cell cell = (landscape -> cells)[x_pos][y_pos];
    shared_ptr<Enemy> enemy = shared_ptr<Enemy>(matrix_enemy[static_cast<int>(type)-1][level]);
    Aura_Type type_a = static_cast<Aura_Type>(eng() % 3 + 1);
    switch (static_cast<int> (type_a))
    {
        case 1:
        { 
            Speed speed(speed_r, speed_v);
            shared_ptr<Speed> aura_s = make_shared<Speed>(speed);
            enemy -> add_aura(aura_s); 
            break;
        }
        case 2:
        {
            Regeneration regeneration(reg_r, reg_v);
            shared_ptr<Regeneration> aura_r = make_shared<Regeneration>(regeneration);
            enemy -> add_aura(aura_r); 
            break;
        }
        case 3:
        {
            Max_Hp max_hp(max_hp_r, max_hp_v);
            shared_ptr<Max_Hp> aura_m = make_shared<Max_Hp>(max_hp);
            enemy -> add_aura(aura_m); 
            break;
        }
        default:
            throw std::invalid_argument("Invalid input");
    }
    enemies.push_back(enemy);
}

Lair::~Lair(){};
Tower::~Tower(){};
