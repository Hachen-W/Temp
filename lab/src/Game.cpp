#define default_size 10

#include <algorithm>
#include <ctime>
#include <random>
#include "Game.h"
#include "Entity.h"
#include <execution>
#include <atomic>
#include <future>

using std::for_each;
using std::dynamic_pointer_cast;
using std::mt19937;

bool Game::to_do_step(){
    count_steps += 1;
    bool game_end = 0;
    int size_x = (landscape -> cells).size();
    int size_y = (landscape -> cells)[0].size();

    std::promise<void> promise;
    std::future<void> future = promise.get_future();

    auto process_entities_wrapped = [&]() {
        for_each(std::execution::par_unseq, (landscape -> cells).matrix.begin(), (landscape -> cells).matrix.end(), [&](vector<Cell>& cells){
            for_each(std::execution::par_unseq, cells.begin(), cells.end(), [&](Cell cell){
                for (shared_ptr<Entity>(entity) : cell.ptr_entities){
                    if (dynamic_pointer_cast<Construction>(entity) != nullptr){
                        entity -> to_do_step();
                        if (dynamic_pointer_cast<Lair>(entity) != nullptr){
                            enemies = dynamic_pointer_cast<Lair>(entity) -> enemies;
                        }
                        if (dynamic_pointer_cast<Castle>(entity) != nullptr){
                            int castle_hp = dynamic_pointer_cast<Castle>(entity) -> get_current_hp();
                            int castle_level = dynamic_pointer_cast<Castle>(entity) -> get_level();
                            int money =  dynamic_pointer_cast<Castle>(entity) -> get_amount_gold();
                            std::cout << "Castle level is " << castle_level << std::endl;
                            std::cout << "Castle hp is: " << castle_hp << std::endl;
                            std::cout << "Gold : " << money << std::endl;

                            if (castle_hp == 0){
                                std::cout << "Game over!" << std::endl;
                                game_end = 1;
                            }
                        }
                    }
                }
            });
        });
        promise.set_value(); // Устанавливаем значение promise после завершения
    };

    std::thread th(process_entities_wrapped);
    th.join();

    future.get();

    if (game_end == 1) return 1;

    vector<int> numbers;
    for_each(std::execution::par_unseq, enemies.begin(), enemies.end(), [&](shared_ptr<Enemy> &entity){
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
        entity -> to_do_step();
    });
    std::reverse(enemies.begin(), enemies.end());
    for_each(std::execution::par_unseq, numbers.begin(), numbers.end(), [&](int num){
        enemies.erase(enemies.begin() + num);
    });
    return 0;
}


#define e_move 1    //Клеток

#define l_hp 50
#define l_reg_rate 10   //%
#define l_castle_damage 20
#define l_probability 5 //%


void Game::try_generate_game_field(){
    bool status = 1;
    while (status == 1){
    //    std::cout << "Regeneration" << std::endl;
        status = generate_game_field();
    }
    return;
}

bool Game::generate_game_field(){
    Landscape landscape_new;
    landscape_new.cells.matrix.clear();
    landscape_new.cells.matrix.resize(default_size);
    mt19937 eng(time(nullptr));
    landscape_new.set_size_play_field(default_size, default_size);
    for (int i = 0; i < default_size; i++){
        landscape_new.cells.matrix[i].resize(default_size);
        for (int j = 0; j < default_size; j++){
            landscape_new.cells.matrix[i][j].x = i;
            landscape_new.cells.matrix[i][j].y = j;
            int first_step_rand = eng() % 100 + 1;
            if (first_step_rand < 80){
                landscape_new.cells.matrix[i][j].type = static_cast<Land_Type>(1);
            }else if (first_step_rand < 90){
                landscape_new.cells.matrix[i][j].type = static_cast<Land_Type>(2);
            }else {
                landscape_new.cells.matrix[i][j].type = static_cast<Land_Type>(3);
            }
        }
    }
    std::shared_ptr<Landscape> land = std::make_shared<Landscape>(landscape_new);
    landscape = land;
    Castle castle(land);
    Lair lair(land);
    (land -> cells).matrix[0][default_size-1].ptr_entities.push_back(std::make_shared<Castle>(castle));
    (land -> cells).matrix[0][default_size-1].type = static_cast<Land_Type>(1);
    (land -> cells).matrix[default_size-1][0].ptr_entities.push_back(std::make_shared<Lair>(lair));
    (land -> cells).matrix[default_size-1][0].type = static_cast<Land_Type>(1);

    //Test block

    int castle_pos[2] = {0,9};
    int cur_pos[2] = {9,0};
    int level = 0;
    Light_Infantry l_enemy("l_in", castle_pos, cur_pos, landscape, l_hp + level*10, l_hp + level*10, l_reg_rate + level*2, e_move, l_castle_damage + level*5, l_probability);
    vector<Cell> shortest_way = l_enemy.fsw_castle();
    if (shortest_way.size() == 0){
        return 1;
    }
    return 0;
}

#define cost_wall_fix 100

void Player::fix_wall(int x_pos, int y_pos){

    shared_ptr<Castle> castle = std::dynamic_pointer_cast<Castle>((landscape->cells).matrix[castle_pos[0]][castle_pos[1]].ptr_entities[0]);
    int cur_amount_gold = castle -> get_amount_gold();
    if (cur_amount_gold < cost_wall_fix ){
        std::cout << "You dont have enough money for fix wall. Cost is " << cost_wall_fix << std::endl;
        return;
    }
    castle -> set_amount_gold (cur_amount_gold - cost_wall_fix);
    shared_ptr<Wall> wall;

    for (auto entity : (landscape->cells).matrix[x_pos][y_pos].ptr_entities){
        if (dynamic_pointer_cast<Wall>(entity) != nullptr){
                wall = dynamic_pointer_cast<Wall>(entity);
        }
    }
    int current_hp = wall -> get_current_hp();
    if ( current_hp > wall -> get_max_hp() - 70){
         wall->set_current_hp(wall->get_max_hp());
    }else wall->set_current_hp(current_hp + 70);
    std::cout << "Wall hp now: " << wall -> get_current_hp() << std::endl;

}

#define st_radius 2
#define st_damage 30
#define st_rate_fire 1
#define st_cost 500

void Player::build_tower(int x_pos, int y_pos){
    shared_ptr<Castle> castle = std::dynamic_pointer_cast<Castle>((landscape->cells).matrix[castle_pos[0]][castle_pos[1]].ptr_entities[0]);
    int castle_gold = castle -> get_amount_gold();
    if (castle_gold < st_cost){
        std::cout << "You dont have enough money for tower! Tower cost is" << st_cost << std::endl;
        return;
    } 
    castle -> set_amount_gold (castle_gold - st_cost);
    std::cout << "You bought the tower for " << st_cost << " gold" << std::endl;
    int level_tower = castle -> get_level();
    int cur_pos[2] = {x_pos, y_pos};
    int radius_action = (st_radius + level_tower / 5);
    int damage = (st_damage + level_tower * 5);
    int rate_fire = (st_rate_fire);
    int cost = (st_cost + level_tower*20); 
    for (auto entity : (landscape -> cells).matrix[x_pos][y_pos].ptr_entities){
        
        if (dynamic_pointer_cast<Tower>(entity) != nullptr){
            std::cout << "Tower already exist!" << std::endl;
            castle -> set_amount_gold (castle_gold);
            std::cout << "Gold was returned" << std::endl;
            return; //Tower already exist
        }
    }
    Tower tower (cur_pos, landscape, level_tower, radius_action, damage, rate_fire, cost);
    (landscape -> cells).matrix[x_pos][y_pos].ptr_entities.push_back(std::make_shared<Tower>(tower));
   // std::cout << (landscape -> cells).matrix[x_pos][y_pos].ptr_entities.size() << x_pos << y_pos << std::endl;

    //Test block 

    int castle_pos[2] = {0,9};
    int cur_pos_e[2] = {9,0};
    int level = 0;
    Light_Infantry l_enemy("l_in", castle_pos, cur_pos_e, landscape, l_hp + level*10, l_hp + level*10, l_reg_rate + level*2, e_move, l_castle_damage + level*5, l_probability);
    vector<Cell> shortest_way = l_enemy.fsw_castle();
    if (shortest_way.size() == 0){
         (landscape -> cells).matrix[x_pos][y_pos].ptr_entities.pop_back();
         std::cout << "Dont try close the way to the castle!" << std::endl;
         castle -> set_amount_gold (castle_gold);
         std::cout << "Gold was returned" << std::endl;
    }
}

#define st_cost_wall 300
#define st_wall_hp 100


void Player::build_wall(int x_pos, int y_pos){
    shared_ptr<Castle> castle = std::dynamic_pointer_cast<Castle>((landscape->cells).matrix[castle_pos[0]][castle_pos[1]].ptr_entities[0]);
    int castle_gold = castle -> get_amount_gold();
    if (castle_gold < st_cost_wall){
        std::cout << "You dont have enough money for wall! Wall cost is " << st_cost_wall << std::endl;
        return;
    } 
    castle -> set_amount_gold (castle_gold - st_cost_wall);
    std::cout << "You bought the wall for " << st_cost_wall << " gold" << std::endl;
    int level_tower = castle -> get_level();
    int cur_pos[2] = {x_pos, y_pos};
    int cost = (st_cost + level_tower*20); 
    for (auto entity : (landscape -> cells).matrix[x_pos][y_pos].ptr_entities){
        if (dynamic_pointer_cast<Wall>(entity) != nullptr) return; //Wall already exist
    }
    Wall wall(cur_pos, st_wall_hp, landscape);
    (landscape -> cells).matrix[x_pos][y_pos].ptr_entities.push_back(std::make_shared<Wall>(wall));
   // std::cout << (landscape -> cells).matrix[x_pos][y_pos].ptr_entities.size() << x_pos << y_pos << std::endl;

    //Test block 

    int castle_pos[2] = {0,9};
    int cur_pos_e[2] = {9,0};
    int level = 0;
    Light_Infantry l_enemy("l_in", castle_pos, cur_pos_e, landscape, l_hp + level*10, l_hp + level*10, l_reg_rate + level*2, e_move, l_castle_damage + level*5, l_probability);
    vector<Cell> shortest_way = l_enemy.fsw_castle();
    if (shortest_way.size() == 0){
         (landscape -> cells).matrix[x_pos][y_pos].ptr_entities.pop_back();
         std::cout << "Dont try close the way to the castle!" << std::endl;
         castle -> set_amount_gold (castle_gold);
         std::cout << "Gold was returned" << std::endl;
    }
}

#define cost_tower_upgrade 300

void Player::upgrade_tower(int x_pos, int y_pos){
    shared_ptr<Castle> castle = std::dynamic_pointer_cast<Castle>((landscape->cells).matrix[castle_pos[0]][castle_pos[1]].ptr_entities[0]);
    int cur_amount_gold = castle -> get_amount_gold();
    if (cur_amount_gold < cost_tower_upgrade * castle-> get_level()){
        std::cout << "You dont have enough money for upgrade tower. Cost is " << cost_tower_upgrade * castle -> get_level() << std::endl;
        return;
    }
    shared_ptr<Tower> tower;
    for (auto entity : (landscape->cells).matrix[x_pos][y_pos].ptr_entities){
        if (dynamic_pointer_cast<Tower>(entity) != nullptr){
                tower = dynamic_pointer_cast<Tower>(entity);
        }
    }
    castle -> set_amount_gold (cur_amount_gold - cost_tower_upgrade * castle -> get_level());
    std::cout << "Tower was upgrade" << std::endl;

    tower -> up_level();
}

#define cost_castle_upgrade 750



void Player::upgrade_castle(){
    shared_ptr<Castle> castle = std::dynamic_pointer_cast<Castle>((landscape->cells).matrix[castle_pos[0]][castle_pos[1]].ptr_entities[0]);
    int cur_amount_gold = castle -> get_amount_gold();
    if (cur_amount_gold < cost_castle_upgrade * castle-> get_level()){
        std::cout << "You dont have enough money for upgrade castle. Cost is " << cost_castle_upgrade * castle -> get_level() << std::endl;
        return;
    }
    std::cout << "Castle was upgrade" << std::endl;
    castle -> set_amount_gold (cur_amount_gold - cost_castle_upgrade * castle -> get_level());
    castle -> up_level();
}
