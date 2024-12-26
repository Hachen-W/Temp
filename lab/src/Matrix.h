#ifndef MATRIX
#define MATRIX

#include <iostream>
#include <iterator>
#include <concepts>
#include <cassert>
#include <vector>
#include <algorithm>

using std::vector;
template <typename Type>
class MyVector;

///The template class is a Matrix
template<typename T>
class Matrix{
        public:
        vector<vector<T>> matrix;

        size_t size() const{
            return matrix.size();
        }
        vector<T>& operator [](int num){
            if ((num > matrix.capacity()) || (num < 0)) throw std::invalid_argument("illegal index");
            return matrix[num];
        }
        Matrix& operator = (const Matrix &copy){
            if (this != &copy){
                int size_ = copy.size();
                matrix = copy.matrix;
            }
        return *this;
        }
};

///The iterator for MyVector
template<typename T, bool is_const>
class VectorIterator;

///The template class is a MyVector
template <typename Type>
class MyVector{
    private:
    Type* values;
    size_t size;
    size_t capacity;
    public:
    
    friend VectorIterator<Type, true>;
    friend VectorIterator<Type, false>;
    typedef Type value_type;
    typedef Type& reference;
    typedef const Type& const_reference;
    typedef Type* pointer;
    typedef VectorIterator<Type, false> iterator;
    typedef VectorIterator<Type, true> const_iterator;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    MyVector(const MyVector& copy){
        size = copy.get_size();
        capacity = copy.get_capacity();
        values = new Type[capacity];
        Type* values_copy = copy.get_values();
        for (int i = 0; i < size; ++i){
            values[i] = values_copy[i];
        }
    }
    MyVector(MyVector&& move) noexcept : values(move.get_values()), size(move.get_size()), capacity(move.get_capacity()) {move.set_values(nullptr);}
    MyVector(){
        values = nullptr;
        size = 1;
        capacity = 1;
    }
    MyVector(Type value_in){
        values = nullptr;
        size = 0;
        capacity = 1;
        push_back(value_in);
    }
    int get_size() const{return size;}
    int get_capacity() const{return capacity;}
    Type* get_values() const {return values;}
    
    void set_values(Type* values_in){
        values = values_in;
    }

    iterator begin() noexcept{
        return iterator(values);
    }

    iterator end() noexcept{
        if (size != 0) return iterator(values + size);
        return iterator(values);
    }

    const_iterator begin() const noexcept{
        return const_iterator(values);
    }

    const_iterator end() const noexcept{
        if (size != 0) return const_iterator(values + size);
        return const_iterator(values);
    }

    const_iterator cbegin() const noexcept{
        return const_iterator(values);
    }

    const_iterator cend() const noexcept{
        if (size != 0) return const_iterator(values + size);
        return const_iterator(values);
    }


    void print() const{
        std::for_each(this->begin(), this->end(), [](value_type value){
            std::cout << value << ' ';
        });
        std::cout << *(this->end() - 3) << std::endl;
    }
    void push_back(const Type& value_in){
        if (!values){
            values = new Type[capacity];
        }
        else if (size == capacity){
            Type* temp_values = new Type[capacity];
            for (int i = 0; i < size; ++i){
                temp_values[i] = values[i];
            }
            delete[] values;
            capacity *= 2;
            values = new Type[capacity];
            for (int i = 0; i < size; ++i){
                values[i] = temp_values[i];
            }
            delete[] temp_values;
        }
        values[size] = value_in;
        size += 1;
    }
    
    void push_back(Type&& value_in){
        if (!values){
            values = new Type[capacity];
        }
        else if (size == capacity){
            Type* temp_values = new Type[capacity];
            for (int i = 0; i < size; ++i){
                temp_values[i] = values[i];
            }
            delete[] values;
            capacity *= 2;
            values = new Type[capacity];
            for (int i = 0; i < size; ++i){
                values[i] = temp_values[i];
            }
            delete[] temp_values;
        }
        values[size] = std::move(value_in);
        size += 1;
    }
    

    Type& operator [](int index) const{
        return values[index];
    }
    MyVector& operator = (MyVector &&move) noexcept{
        delete[] values;
        values = move.values;
        move.values = nullptr;
        return *this;
    }
    MyVector& operator = (const MyVector &copy){
        if (this != &copy){
            if (values) delete[] values;
            size = copy.get_size();
            capacity = copy.get_capacity();
            values = new Type[capacity];
            for (int i = 0; i < capacity; ++i){
                values[i] = (copy.get_values())[i];
            }
        }
        return *this;
    }
    ~MyVector(){
        delete[] values;
    }
};

template<typename T, bool is_const>
class VectorIterator{
    private:
    typedef std::conditional_t<is_const, const T, T> *vector_ptr;   //Тип данных зависит от is_const
    vector_ptr node;

    VectorIterator(vector_ptr node): node(node) {}

    friend MyVector<T>;
    friend VectorIterator<T, !is_const>;    //Для противоположного типа
    
    public:
    
    typedef ptrdiff_t difference_type;  //Расстояние между итераторами
    typedef T value_type;   //Какой тип значения
    
    
    typedef std::conditional_t<is_const, const T, T>* pointer;   //Тип указателя
    typedef std::conditional_t<is_const, const T, T>& reference;   //Тип ссылки
    typedef std::random_access_iterator_tag iterator_category;  //Категория итератора
    
    //for movable

    template<bool other_const>
    VectorIterator(VectorIterator<T, other_const>& o) noexcept requires(is_const >= other_const): node(o.node) {}  //Нельзя терять константу
    template<bool other_const>
    VectorIterator& operator = (const VectorIterator<T, other_const>& o) noexcept requires(is_const >= other_const){ node = o.node; return *this;}
    
    //for weakly_incrementable

    VectorIterator& operator ++ () noexcept{
        node++;
        return *this;
    }
    VectorIterator operator ++(int) noexcept{
        VectorIterator res(node);
        node++;
        return res;
    }

    //for input_or_output_iterator

    reference operator * () const noexcept{
        return *node;
    } 
    pointer operator -> () const noexcept{
        return node;
    }

    //for forward_iterator

    VectorIterator() noexcept : node(nullptr) {}

    template<bool other_const>
    bool operator == (const VectorIterator<T, other_const>& o) const noexcept{
        return node == o.node;
    }

    VectorIterator& operator -- () noexcept{
        node--;
        return *this;
    }

    VectorIterator operator -- (int) noexcept{
        VectorIterator res(node);
        node--;
        return res;
    }
    
    //for random_access

    reference operator [] (unsigned int index) const{
        return node[index];
    }

    VectorIterator& operator += (unsigned int index){
        node += index;
        return *this;
    }

    VectorIterator& operator -= (unsigned int index){
        node -= index;
        return *this;
    }
   
    VectorIterator operator - (difference_type index) const{
        VectorIterator newIter(node - index);
        return newIter;
    }
    
    friend VectorIterator operator + (difference_type index, const VectorIterator<T, is_const>& other){ 
            VectorIterator newIter(other + index);
            return newIter; 
    }

    friend VectorIterator operator + (const VectorIterator<T, is_const>& other, difference_type index){ 
            VectorIterator newIter(other + index);
            return newIter; 
    }

    difference_type operator - (const VectorIterator& other) const{
        return node - other.node;
    }

    bool operator >= (const VectorIterator& other) const{
        return node >= other.node;
    }
    
    bool operator > (const VectorIterator& other) const{
        return node > other.node;
    }
    
    bool operator <= (const VectorIterator& other) const{
        return node <= other.node;
    }

    bool operator < (const VectorIterator& other) const{
        return node < other.node;
    }
};

#endif
