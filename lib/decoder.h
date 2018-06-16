#ifndef DECODER_H
#define DECODER_H

#include "my_structs.h"

struct decoder {
    decoder();
    void build(info const &inf);
    std::vector<unsigned char> decode(std::vector<unsigned char> const &a);
    bool is_all_data_decoded();

private:
    static const size_t SIZE = 256;

    std::shared_ptr<node> head;
    unsigned short tail;
    size_t tail_size;
    long long file_size;

    void next_bit(int data, int bit, node *&root, std::vector<unsigned char> &ret,
                  unsigned short &new_tail, size_t &new_tail_size);
    void tree_traversal(std::shared_ptr<node> root, info const &inf, int &symb, int &pos, size_t &ind);
};

#endif // DECODER_H
