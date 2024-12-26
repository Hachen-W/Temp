#ifndef ENTITY
#define ENTITY

#include "Landscape.h"
#include <array>
#include <memory>

using std::shared_ptr;
using std::array;

class Landscape;

/**
        @brief class Entity

        The base class of entities. It stores the current position on the field and the field itself where the actions take place.
*/

class Entity{
    protected:
    ///Current position of entity
    int current_position[2];
    ///The field 
    shared_ptr<Landscape> landscape;
    
    public:
    Entity(){};
    Entity(shared_ptr<Landscape>(landscape_in), int current_position_in[2]);
    void set_current_position(int x, int y);
    void set_landscape(shared_ptr<Landscape>);
    array <int, 2> get_current_position() const;
    shared_ptr<Landscape> get_landscape() const;
    ///Virtual method to make a move (redefined for constructions and enemies)
    virtual void to_do_step() = 0;
    virtual ~Entity();
};

#endif
