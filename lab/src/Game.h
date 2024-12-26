#ifndef GAME
#define GAME

#include "Matrix.h"
#include "Construction.h"
#include "Construction_Enemy.h"
#include <vector>

class Game{
        public:
        shared_ptr<Landscape> landscape;
        int count_steps;
        bool to_do_step();
        bool generate_game_field();
        void try_generate_game_field();
        std::vector<shared_ptr<Enemy>>(enemies);
        //void update_enemies_hp(std::vector<shared_ptr<Enemy>>(enemies), std::shared_ptr<Landscape> landscape);

};


class Player{
        public:
        shared_ptr<Landscape> landscape;
        int castle_pos[2];
        void fix_wall(int x_pos, int y_pos);
        void upgrade_tower(int x_pos, int y_pos);
        void upgrade_castle();
        void build_tower(int x_pos, int y_pos);
        void build_wall(int x_pos, int y_pos);

};


#endif
