#ifndef CONSTRUCTION
#define CONSTRUCTION

#include <string>
#include "Characteristics.h"
#include "Entity.h"

using std::string;

/**
        @brief Class Construction

        Construction class. Has children: Tower, Castle, Wall, Lair.
*/

class Construction: public Entity{
        protected:
        ///Name of Construction
        string name;
        public:
        void set_name(string name_in);
        string get_name() const;
        ///Redefining the to do step method.
        virtual void to_do_step(){};
        virtual ~Construction();
};


/**
        @brief Class Castle

        The main class for the user. Contains a table of characteristics (profitability, maximum strength, repair speed, cost)
*/

class Castle: public Construction{
        private:
        ///The current level of the Castle
        int level;
        ///The current health of the Castle
        int current_hp;
        ///The amount of gold in the castle
        int amount_gold;
        Table_Characteristics characteristics;
        public:
        Castle(){};
        Castle(std::shared_ptr<Landscape> landscape_in);
        ///Redefining the to do step (bring gold) method.
        void to_do_step() override;
        ///The method for up level of castle (upgrade characteristics)
        void up_level();
        void set_current_hp(int current_hp_in);
        void set_amount_gold(int amount_gold_in);
        void add_characteristic(Characteristic characteristic);

        int get_level() const;
        int get_current_hp() const;
        int get_amount_gold() const;
        Table_Characteristics get_characteristics() const;
        ///The method that upgrade all characteristics in the castle.
        void upgrade_characteristics();
        ~Castle();
};


/**
        @brief Class Wall

        Wall. It is a temporary barrier for enemies. It can be destroyed by aircraft and heavy infantry
*/

class Wall: public Construction{
        private:
        ///The current health of the Wall
        int current_hp;
        ///The max health of the Wall
        int max_hp;
        public:

        Wall(){};
        Wall(int cur_pos[2], int max_hp_in, shared_ptr<Landscape> landscape);
        
        ///Redefining the to do step
        void to_do_step() override;

        void set_current_hp(int current_hp_in);
        void set_max_hp(int max_hp_in);

        int get_current_hp() const;
        int get_max_hp() const;
        ~Wall();
};


#endif
