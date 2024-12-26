#ifndef ENEMY
#define ENEMY

#include <vector>
#include <string>
#include "Aura.h"
#include "Construction.h"

using std::vector;
using std::string;
using std::shared_ptr;
class Aura;

///Enum class, which contains types of enemies
enum class Enemy_Type {Light_Infantry = 1, Heavy_Infantry, Aviation};
///Enum class, which contains the types of auras
enum class Aura_Type {Speed = 1, Regeneration, Max_hp};

/**
        @brief class Enemy

        The main opponent of the player. Enemy. Can attack the castle, spawn with the help of a lair
*/


class Enemy : public Entity{
        public:
        ///name of enemy
        string name;
        protected:
        int max_hp;
        int hp;
        int regeneration_rate;
        int movement_speed;
        ///Lock position for finding the shortest way
        int castle_position[2];
        int damage_castle;
        ///The enemy has a set of auras, if he is a hero (if there are none, then the enemy is ordinary)
        vector <shared_ptr<Aura> > auras;
        ///The shortest way to the castle
        vector <Cell> shortest_way;
        public:
        Enemy(){};
        ///Enemy constuctor with several characteristics
        Enemy(string name_in, int castle_position_in[2], int current_position_in[2], shared_ptr<Landscape>(landscape_in), int max_hp_in, int hp_in, int reg_rate, int move_speed_in, int damage_c_in) : Entity(landscape_in, current_position_in), name(name_in), max_hp(max_hp_in), hp(hp_in), regeneration_rate(reg_rate), movement_speed(move_speed_in), damage_castle(damage_c_in){
                castle_position[0] = castle_position_in[0];
                castle_position[1] = castle_position_in[1];
                shortest_way = std::vector<Cell>();
        }; 
        void set_max_hp(int hp_max_in);
        void set_hp(int hp_in);
        void set_regeneration_rate(int regeneration_rate_in);
        void set_movement_speed(int movement_speed_in);
        void set_damage_castle(int damage_castle_in);
        void add_aura(shared_ptr<Aura> aura_in);

        int get_max_hp() const;
        int get_hp() const;
        int get_regeneration_rate() const;
        int get_movement_speed() const;
        int get_damage_castle() const;
        vector < shared_ptr<Aura> > get_auras() const;
        ///The method of finding the shortest path to the castle
        virtual vector <Cell> fsw_castle();
        ///Wall attack method (only for heavy infantry and aviation)
        virtual void attack_wall(shared_ptr<Wall>){};
        ///Redefining the to_do_step method
        virtual void to_do_step(){}; 
        ///Castle attack method
        bool attack_castle(int);
        ///The method of checking the shortest path (whether there is a wall or a tower on the way)
        bool check_shortest_way();
        void try_to_reg_hp();
        size_t shortest_way_size();

        virtual ~Enemy();
};


/**
        @brief class Light_Infantry

        Cannot attack walls, bypasses mountains and rivers. Has a small chance to evade the attack of the tower
*/

class Light_Infantry: public Enemy{
        private:
        ///The probability of evading a tower attack
        int probability_of_evasion;
        public:
        Light_Infantry(){};
        ///Light_Infantry constructor with several characteristics
        Light_Infantry(string name_in, int castle_position_in[2], int current_position_in[2], shared_ptr<Landscape>(landscape_in), int max_hp_in, int hp_in, int reg_rate, int move_speed_in, int damage_c_in, int probability_of_evasion_in) : Enemy(name_in, castle_position_in, current_position_in, landscape_in, max_hp_in, hp_in, reg_rate, move_speed_in, damage_c_in){probability_of_evasion = probability_of_evasion_in;};
        ///Redefining the to_do_step method
        void to_do_step() override;
        int get_probability_of_evasion();
        ~Light_Infantry();
};

/**
        @brief class Heavy_Infantry

        Can't dodge tower attacks. It can destroy walls. Bypasses mountains and rivers
*/

class Heavy_Infantry: public Enemy{
        private:
        ///Damage to walls
        int damage_walls;
        int radius_of_action;
        public:
        Heavy_Infantry(){};
        ///Heavy_Infantry constructor with several characteristics
        Heavy_Infantry(string name_in, int castle_position_in[2], int current_position_in[2], shared_ptr<Landscape>(landscape_in), int max_hp_in, int hp_in, int reg_rate, int move_speed_in, int damage_c_in, int damage_w_in, int radius_in) : Enemy(name_in, castle_position_in, current_position_in, landscape_in, max_hp_in, hp_in, reg_rate, move_speed_in, damage_c_in){damage_walls = damage_w_in; radius_of_action = radius_in;}; 
        ///Redefining the to_do_step method
        void to_do_step() override;
        ///Wall attack method
        void attack_wall(shared_ptr<Wall>) override;
        ~Heavy_Infantry();
};

/**
        @brief class Aviation

        Can't dodge the tower. Drops bombs on the castle and walls. It flies over rivers and mountains
*/

class Aviation: public Enemy{
        private:
        ///Damage to walls
        int damage_walls;
        ///Number of ammunition
        int amout_of_ammunition;
        public:
        Aviation(){};
        ///Aviation constructor with several characteristics
        Aviation(string name_in, int castle_position_in[2], int current_position_in[2], shared_ptr<Landscape>(landscape_in), int max_hp_in, int hp_in, int reg_rate, int move_speed_in, int damage_c_in, int damage_w_in, int amout_in) : Enemy(name_in, castle_position_in, current_position_in, landscape_in, max_hp_in, hp_in, reg_rate, move_speed_in, damage_c_in){damage_walls = damage_w_in; amout_of_ammunition = amout_in;}; 
        ///Redefining the to_do_step method
        void to_do_step() override;
        ///Wall attack method
        void attack_wall(shared_ptr<Wall>) override;
        ///Redefining the shortest path search method (different from heavy and light infantry)
        vector <Cell> fsw_castle() override;
        ~Aviation();
};

#endif
