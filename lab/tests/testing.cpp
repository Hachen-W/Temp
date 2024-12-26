#define CATCH_CONFIG_MAIN 
#include <catch2/catch.hpp>
#include "../src/Game.h"
#include "../src/Game.cpp"

using std::cout;

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

TEST_CASE("Testing classes") {
    Game game;
    game.try_generate_game_field();
    SECTION("Test class Cell"){
        Cell cell = (*game.landscape).cells[9][0];
        REQUIRE(cell.x == 9);
        REQUIRE(cell.ptr_entities.size() == 1);
    }

    SECTION("Test class Landscape"){
        Landscape landscape = *(game.landscape);
        REQUIRE(landscape.cells.size() == 10);
        REQUIRE(landscape.get_size_play_field()[0] == 10);
        landscape.set_size_play_field(15,15);
        REQUIRE(landscape.get_size_play_field()[1] == 15);
   } 
    
    int current_position_tower[2] = {8,3};
    Tower tower(current_position_tower, game.landscape, 1, 2, 35, 1, 500);
    REQUIRE(tower.get_current_position()[1] == 3);
    REQUIRE(tower.get_characteristics().get_characteristic("cost").get_value() == 500);
    REQUIRE(tower.get_characteristics().get_characteristic("radius_action").get_value() == 2);
    REQUIRE(tower.get_characteristics().get_characteristic("damage").get_value() == 35);
    REQUIRE(tower.get_characteristics().get_characteristic("rate_of_fire").get_value() == 1);
    tower.up_level();
    REQUIRE(tower.get_level() == 2);


    SECTION("Test class Enemy"){

            int castle_pos[2] = {0,9};
            int cur_enemy_pos[2] = {9,0};

            Light_Infantry l_enemy("name_l", castle_pos, cur_enemy_pos, game.landscape, l_hp, l_hp, l_reg_rate, e_move, l_castle_damage, l_probability);
            l_enemy.to_do_step();
            REQUIRE(l_enemy.get_probability_of_evasion() == l_probability);
            REQUIRE(l_enemy.get_hp() == l_hp);
            REQUIRE(l_enemy.get_max_hp() == l_hp);
            REQUIRE(l_enemy.get_regeneration_rate() == l_reg_rate);
            REQUIRE(l_enemy.get_movement_speed() == e_move);
            REQUIRE(l_enemy.check_shortest_way() == 0);
            REQUIRE(l_enemy.shortest_way_size() != 0);
            l_enemy.set_hp(0);
            l_enemy.to_do_step();
            Heavy_Infantry h_enemy("name_h", castle_pos, cur_enemy_pos, game.landscape, h_hp, h_hp, h_reg_rate, e_move, h_castle_damage, h_walls_damage, h_radius_action);
            Aviation a_enemy("name_a", castle_pos, cur_enemy_pos, game.landscape, a_hp, a_hp, a_reg_rate, e_move, a_castle_damage, a_walls_damage, a_radius_action);
            h_enemy.to_do_step();
            h_enemy.set_hp(0);
            h_enemy.to_do_step();

            a_enemy.to_do_step();
            a_enemy.to_do_step();
            a_enemy.to_do_step();
            

                Enemy enemy;
                enemy.set_current_position(5,5);
                enemy.set_hp(10);
                enemy.set_max_hp(20);
                enemy.set_regeneration_rate(50);
                enemy.set_damage_castle(100);
                enemy.set_movement_speed(30);
                Enemy enemy_2;
                enemy.set_current_position(6,5);
                enemy.set_hp(10);
                enemy.set_max_hp(20);
                enemy.set_regeneration_rate(5);
                enemy.set_damage_castle(100);
                REQUIRE(enemy.get_max_hp() == 20);
                REQUIRE(enemy.get_hp() == 10);
                REQUIRE(enemy.get_regeneration_rate() == 5);
                REQUIRE(enemy.get_damage_castle() == 100);
                REQUIRE(enemy.get_movement_speed() == 30);
                enemy.try_to_reg_hp();
                enemy.try_to_reg_hp();
                REQUIRE(enemy.get_hp() == 20);
                SECTION("Test class Aura"){
                    Speed aura_speed(2,5);
                    enemy.add_aura(std::make_shared<Aura>(aura_speed));
                    aura_speed.make_effect(game.landscape, std::make_unique<Enemy>(enemy));
                    Max_Hp aura_max_hp(2,4);
                    enemy.add_aura(std::make_shared<Aura>(aura_max_hp));
                    aura_max_hp.make_effect(game.landscape, std::make_unique<Enemy>(enemy));
                    Regeneration aura_regeneration(2,3);
                    enemy.add_aura(std::make_shared<Aura>(aura_regeneration));
                    aura_regeneration.make_effect(game.landscape, std::make_unique<Enemy>(enemy));
                    REQUIRE(enemy.get_auras().size() == 3);
                    
                }
    }
    Castle castle(game.landscape);
    castle.set_name("Castle_2");
    castle.up_level();
    castle.set_current_hp(6000);
    castle.set_amount_gold(500);
    REQUIRE(castle.get_level() == 2);
    REQUIRE(castle.get_current_hp() == 6000);
    REQUIRE(castle.get_amount_gold() == 500);
    REQUIRE(castle.get_name() == "Castle_2"); 

    int cur_pos[2] = {5,5};
    Wall wall(cur_pos, 100, game.landscape);
    wall.set_max_hp(200);
    wall.set_current_hp(200);
    
    REQUIRE(wall.get_max_hp() == 200);
    REQUIRE(wall.get_current_hp() == 200);
    SECTION("Test class Characteristic") {
        Characteristic attack("ATAKA",15, 1);
        REQUIRE(attack.get_name() == "ATAKA");
        REQUIRE(attack.get_level() == 1);
        attack.up_level();
        REQUIRE(attack.get_level() == 2);
        attack.set_name("attack");
        attack.set_value(12);
        REQUIRE(attack.get_name() == "attack");
        REQUIRE(attack.get_value() == 12);
        SECTION("Test class Table_Characteristics"){
            Table_Characteristics characteristics;
            characteristics.add_characteristic(attack);
            REQUIRE(characteristics.get_characteristic("attack").get_level() == 2);
            REQUIRE(characteristics.get_characteristics().size() == 1);
            REQUIRE_THROWS(characteristics.get_characteristic("undefined"));
        }
    }

    
}
