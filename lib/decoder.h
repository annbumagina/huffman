#ifndef DECODER_H
#define DECODER_H

#include "my_structs.h"

struct decoder {
    decoder();
    ~decoder();
    void build(inf inf);
    vector<unsigned char> decode(vector<unsigned char> a);
    bool is_all_data_decoded();

private:
    node *head;
    unsigned short tail;
    size_t tail_size;
    long long file_size;

    void next_bit(int data, int bit, node *&root, vector<unsigned char> &ret,
                  unsigned short &new_tail, size_t &new_tail_size);
    void tree_traversal(node* root, int &symb, vector<unsigned char> const &symbols,
                        int &pos, size_t &ind, vector<unsigned char> const &tree);
    void delete_tree(node * root);
};

#endif // DECODER_H
