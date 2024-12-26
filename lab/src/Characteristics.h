#ifndef CHARACTERISTICS
#define CHARACTERISTICS

#include <vector>
#include <string>

using std::vector;
using std::string;

/**
        @brief Class Characteristic

        This class is used to store characteristics (for example, the profitability of the castle or the amount of damage inflicted by the tower)  
*/

class Characteristic{
        private:
        ///The name of the characteristic
        string name;
        ///The value of the characteristic (how strong is the effect)
        int value;
        ///The level of the characteristic
        int level;
        public:

        Characteristic(){};
        /**
                A constructor for a Characteristic with third arguments
                @param name_in - name of Characteristic
                @param value_in - value of Characteristic
                @param level_in - level of Characteristic
        */
        Characteristic(string name_in, int value_in, int level_in): name(name_in), value(value_in), level(level_in){};
        
        void set_name(string name_in);
        void set_value(int value_in);
        void up_level();

        string get_name() const;
        int get_value() const;
        int get_level() const;
};

/**
        @brief Class Table_Characteristics

        A class that stores a vector of characteristic classes for convenient storage of a set of characteristics for each entity
*/

class Table_Characteristics{
        public:
        vector<Characteristic> characteristics;

        vector<Characteristic> get_characteristics() const;
        void add_characteristic(Characteristic);
        Characteristic get_characteristic(string name) const;
};

#endif
