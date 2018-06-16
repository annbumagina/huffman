#ifndef CODER_H
#define CODER_H

#include "my_structs.h"

struct coder {
    coder();
    void update(std::vector<unsigned char> const &a);
    info build();
    std::vector<unsigned char> code(std::vector<unsigned char> const &a);

private:
    static const size_t SIZE = 256;

    std::vector<size_t> cnt;
    std::vector<unsigned long long> codes;
    std::vector<unsigned> codes_sizes;
    unsigned char tail;
    size_t tail_size;
    size_t file_size;

    void tree_traversal(std::shared_ptr<node> root, unsigned code, unsigned s, info &ret, int &pos);
};

#endif // CODER_H
