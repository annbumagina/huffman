#include "decoder.h"

#include <stdexcept>

decoder::decoder() :
    head(nullptr),
    tail(0),
    tail_size(0)
{}

decoder::~decoder() {
    delete_tree(head);
}

void decoder::build(inf inf) {
    int symb = 0;
    int pos = 7;
    size_t ind = 0;
    head = new node(SIZE);
    tree_traversal(head, symb, inf.symbols, pos, ind, inf.tree);
    file_size = inf.file_size;
}

vector<unsigned char> decoder::decode(vector<unsigned char> a) {
    vector<unsigned char> ret;
    node *root = head;
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

void decoder::next_bit(int data, int bit, node *&root, vector<unsigned char> &ret,
                       unsigned short &new_tail, size_t &new_tail_size)
{
    if (root == nullptr) {
        throw std::runtime_error("Damaged file");
    }
    new_tail_size++;
    new_tail *= 2;
    if (data & (1 << bit)) {
        new_tail++;
        root = root->right;
    } else {
        root = root->left;
    }
    if (root == nullptr) {
        throw std::runtime_error("Damaged file");
    }
    if (root->val != SIZE) {
        ret.push_back(root->val);
        root = head;
        new_tail = 0;
        new_tail_size = 0;
    }
}

void decoder::tree_traversal(node *root, int &symb, vector<unsigned char> const &symbols,
                             int &pos, size_t &ind, vector<unsigned char> const &tree)
{
    if (ind >= tree.size()) {
        throw std::runtime_error("Damaged control part of file");
    }
    if (tree[ind] & (1 << pos)) {
        root->val = symbols[symb];
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
    // не используй явно new
    // используй shared_ptr, unique_ptr, weak_ptr...
    root->left = new node(SIZE);
    root->right = new node(SIZE);
    tree_traversal(root->left, symb, symbols, pos, ind, tree);
    tree_traversal(root->right, symb, symbols, pos, ind, tree);
}

void decoder::delete_tree(node *root) {
    if (root == nullptr) {
        return;
    }
    if (root->val != SIZE) {
        delete root;
        return;
    }
    delete_tree(root->left);
    delete_tree(root->right);
    delete root;
}
