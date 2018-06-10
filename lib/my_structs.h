#ifndef MY_STRUCTS_H
#define MY_STRUCTS_H

#include <vector>
#include <cstddef>

using std::vector;

const size_t SIZE = 256;

struct node {
    unsigned val;
    node *left, *right;
    node(unsigned val) : val(val), left(nullptr), right(nullptr) {}
};

struct inf {
    union {
        unsigned long long file_size;
        struct {
            char fz[8];
        };
    };
    vector<unsigned char> symbols;
    vector<unsigned char> tree;
};

#endif // MY_STRUCTS_H
