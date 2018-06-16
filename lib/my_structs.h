#ifndef MY_STRUCTS_H
#define MY_STRUCTS_H

#include <vector>
#include <cstddef>
#include <memory>

struct node {
    unsigned val;
    std::shared_ptr<node> left, right;
    node(unsigned val, std::shared_ptr<node> left, std::shared_ptr<node>right) : 
    	val(val), 
    	left(left), 
    	right(right) 
    {}
    node(unsigned val) : node(val, nullptr, nullptr) {}
};

struct pair {
    size_t freq;
    std::shared_ptr<node> ptr;
    
    pair(size_t freq, std::shared_ptr<node> ptr) :
    	freq(freq),
    	ptr(ptr)
    {}
    friend bool operator<(pair a, pair b) {
    	return a.freq < b.freq;
    }
};

struct info {
    union {
        unsigned long long file_size;
        char fz[8];
    };
    std::vector<unsigned char> symbols;
    std::vector<unsigned char> tree;
};

#endif // MY_STRUCTS_H
