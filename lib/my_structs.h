#ifndef MY_STRUCTS_H
#define MY_STRUCTS_H

#include <vector>
#include <cstddef>

// 2: это библиотека, а ты засоряешь пространство имен пользователей
using std::vector;

// ^2
const size_t SIZE = 256;

struct node {
    unsigned val;
    node *left, *right;
    node(unsigned val, node *left, node *right) : val(val), left(left), right(right) {}
    
    // explicit
    node(unsigned val) : node(val, nullptr, nullptr) {}
};

// интересное название
struct inf {
    union {
        unsigned long long file_size;
        char fz[8];
    };
    vector<unsigned char> symbols;
    vector<unsigned char> tree;
};

#endif // MY_STRUCTS_H
