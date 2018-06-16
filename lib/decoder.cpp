#include "decoder.h"

#include <stdexcept>

decoder::decoder() :
    head(new node(SIZE)),
    tail(0),
    tail_size(0)
{}

void decoder::build(info const &inf) {
    int symb = 0;
    int pos = 7;
    size_t ind = 0;
    tree_traversal(head, inf, symb, pos, ind);
    file_size = inf.file_size;
}

std::vector<unsigned char> decoder::decode(std::vector<unsigned char> const &a) {
    std::vector<unsigned char> ret;
    node *root = head.get();
    unsigned short new_tail = 0;
    size_t new_tail_size = 0;
    for (int j = tail_size - 1; j >= 0; j--) {
        next_bit(tail, j, root, ret, new_tail, new_tail_size);
    }
    long long k = 0;
    for (size_t i = 0; i < a.size() && k < file_size; i++) {
        for (int j = 7; j >= 0 && k < file_size; j--, k++) {
            next_bit(a[i], j, root, ret, new_tail, new_tail_size);
        }
    }
    tail = new_tail;
    tail_size = new_tail_size;
    file_size -= a.size() * 8;
    if (file_size <= -8) {
        throw std::runtime_error("Extra data found");
    }
    return ret;
}

bool decoder::is_all_data_decoded() {
    return file_size <= 0;
}

void decoder::next_bit(int data, int bit, node *&root, std::vector<unsigned char> &ret,
                       unsigned short &new_tail, size_t &new_tail_size)
{
    if (root == nullptr) {
        throw std::runtime_error("Damaged file");
    }
    new_tail_size++;
    new_tail *= 2;
    if (data & (1 << bit)) {
        new_tail++;
        root = root->right.get();
    } else {
        root = root->left.get();
    }
    if (root == nullptr) {
        throw std::runtime_error("Damaged file");
    }
    if (root->val != SIZE) {
        ret.push_back(root->val);
        root = head.get();
        new_tail = 0;
        new_tail_size = 0;
    }
}

void decoder::tree_traversal(std::shared_ptr<node> root, info const &inf, int &symb, int &pos, size_t &ind)
{
    if (ind >= inf.tree.size()) {
        throw std::runtime_error("Damaged control part of file");
    }
    if (inf.tree[ind] & (1 << pos)) {
        root->val = inf.symbols[symb];
        symb++;
        pos--;
        if (pos == -1) {
            pos = 7;
            ind++;
        }
        return;
    }
    pos--;
    if (pos == -1) {
        pos = 7;
        ind++;
    }
    new(&root->left) std::shared_ptr<node>(new node(SIZE));
    new(&root->right) std::shared_ptr<node>(new node(SIZE));
    tree_traversal(root->left, inf, symb, pos, ind);
    tree_traversal(root->right, inf, symb, pos, ind);
}
