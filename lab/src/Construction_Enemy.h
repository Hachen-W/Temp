#ifndef TOWER
#define TOWER

#include "Matrix.h"
#include "Enemy.h"
#include <memory>

using std::shared_ptr;

/**
        @brief Class Tower

        One of the most important classes in the game. Towers can attack enemies, and can also be upgraded for gold.
*/

class Tower: public Construction{
        private:
        ///level of tower
        int level;
        ///A set of characteristics
        Table_Characteristics characteristics;
        public:
        Tower(){};
        /**
                A constructor for a Tower with several characteristics
                @param current_position - position of Tower
                @param landscape - the field where the Tower appears
                @param level - level of Tower
                @param radius_action - radius of action of Tower
                @param damage - damage with which the tower attacks enemies
                @param rate_fire - the rate of fire of the tower
                @param cost - how much gold is the tower worth
        */
        Tower(int current_position[2], shared_ptr<Landscape> landscape, int level, int radius_action, int damage, int rate_fire, int cost);
        ///Redefining the to_do_step method for the tower
        void to_do_step() override;
        ///The method of attacking the enemy if the range of the tower reaches the enemy
        void attack_enemy(shared_ptr<Enemy>);
        ///The method of increasing the levels of the tower for gold
        void up_level();
        ///The method of adding tower characteristics
        void add_characteristic(Characteristic& characteristic);

        int get_level() const;
        Table_Characteristics get_characteristics() const;
        ///The method of improving all the characteristics of the tower (when increasing its level)
        void upgrade_characteristics();
        ~Tower();
};

/**
        @brief Class Lair

        The class responsible for spawning enemies. He is the main antagonist of the castle
*/

class Lair: public Construction{
        public:
        ///level of Lair
        int level;
        ///Step counter for choosing which enemy to spawn (regular or hero)
        int count_step;
        Lair(){};
        /**
                A constructor for a Lair with several characteristics
                @param landscape_in - the field where the Lair appears
                @param matrix_enemy - enemy matrix (after initialization)
                @param enemies - a set of current enemies that the lair has created
        */
        Lair(std::shared_ptr<Landscape> landscape_in);
        Matrix<shared_ptr<Enemy>> matrix_enemy;
        vector<shared_ptr<Enemy>> enemies;
        ///Redefining the to_do_step method for the Lair
        void to_do_step() override;
        ///Spawn an ordinary enemy (every second step)
        void spawn_enemy();
        ///Spawn the hero's enemy (every tenth step)
        void spawn_enemy_time();
        ~Lair();
        private:
        ///Table of initialized enemies for easy spawn
        vector<shared_ptr<Enemy>> init_enemies(int level);
        ///The method of reinitializing enemies
        void reinit_enemies();
};

#endif

