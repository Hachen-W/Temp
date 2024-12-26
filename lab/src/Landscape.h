#ifndef LANDSCAPE
#define LANDSCAPE

#include <array>
#include <memory>
#include "Entity.h"
#include "Matrix.h"

using std::vector;
using std::array;
using std::shared_ptr;

class Entity;
///Enum class, which stores the types of possible cells
enum class Land_Type {Plain = 1, Mountain, River};

/**
        @brief class Cell
        
        The cell class. It stores a set of entities that are located on it and the type of landscape that is. It is the basis of the field.
*/

class Cell{
        public:
        ///x position
        int x;
        ///y position
        int y;
        ///Landscape type
        Land_Type type;
        ///Set of entities
        vector < shared_ptr<Entity> > ptr_entities;
        Cell(){};
        Cell(const Cell& other) : x(other.x), y(other.y), type(other.type), ptr_entities(other.ptr_entities){};
};

/**
        @brief class Landscape

        Landscape class. It contains the size of the playing field and a matrix of cells on which entities are located and actions take place.
*/

#define default_size 10

class Landscape{
        private:
        ///size of playing field;
        int size_play_field[2];
        public:
        ///Matrix of cells
        Matrix<Cell> cells;

        void set_size_play_field(int x, int y);
        array <int, 2> get_size_play_field() const;
        Landscape(){
            size_play_field[0] = default_size;
            size_play_field[1] = default_size;
        }
};

#endif

