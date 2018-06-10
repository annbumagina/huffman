#ifndef CODER_H
#define CODER_H

#include "my_structs.h"

struct coder {
    coder();
    void update(vector<unsigned char> a);
    inf build();
    vector<unsigned char> code(vector<unsigned char> a);

private:
    vector<size_t> cnt;
    vector<unsigned long long> codes;
    vector<unsigned> codes_sizes;
    unsigned char tail;
    size_t tail_size;
    size_t file_size;

    void tree_traversal(node *root, unsigned code, unsigned s,
                        vector<unsigned char> &symbols, int &pos,
                        vector<unsigned char> &tree);
};

#endif // CODER_H
