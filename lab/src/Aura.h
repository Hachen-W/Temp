#ifndef AURA
#define AURA

#include <memory>
#include "Enemy.h"
#include "Landscape.h"

using std::unique_ptr;
using std::shared_ptr;
class Enemy;

/**
        @brief class Aura 

        Used for enemies who are heroes. Heroes can influence nearby enemies. (Increase maximum health, movement speed, or regeneration speed)
 */

class Aura{
        protected:
        ///Radius - the maximum distance at which the hero can improve the characteristics of enemies
        int radius;
        ///Value - how much the hero improves the characteristics of enemies
        int value;
        public:
        Aura(){};
        /**
                A constructor for an aura with two arguments
                @param radius_in - radius of Aura
                @param value_in - value of Aura
        */
        Aura(int radius_in, int value_in): radius(radius_in), value(value_in){};
        /**
                The main function for the aura. Redefined in each of the children's classes
                @param landscape - The field on which the aura will act
                @param enemy - A hero who has an impact
        */
        virtual void make_effect(shared_ptr<Landscape> landscape, unique_ptr<Enemy> enemy){};
};

/**
        @brief Speed aura class. 

        An aura class that accelerates enemies 
 */

class Speed: public Aura{
        public:
        Speed(){};
        Speed(int radius, int value): Aura(radius, value){};
        ///Redefining the effect for a class
        void make_effect(shared_ptr<Landscape> landscape, unique_ptr<Enemy> enemy) override;
};

/**
        @brief Regeneration aura class. 

        An aura class that increases the regeneration of enemies
 */

class Regeneration: public Aura{
        public:
        Regeneration(){};
        Regeneration(int radius, int value): Aura(radius, value){};
        ///Redefining the effect for a class
        void make_effect(shared_ptr<Landscape> landscape, unique_ptr<Enemy> enemy) override;
};

/**
        @brief Max_Hp aura class. 

        Aura is a class that increases the maximum health of enemies
 */

class Max_Hp: public Aura{
        public:
        Max_Hp(){};
        Max_Hp(int radius, int value): Aura(radius, value){};
        ///Redefining the effect for a class
        void make_effect(shared_ptr<Landscape> landscape, unique_ptr<Enemy> enemy) override;
};

#endif
