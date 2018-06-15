#include "coder.h"
#include <set>
#include <numeric>
#include <cstdint>

coder::coder() :
    cnt(SIZE, 0),
    codes(SIZE),
    codes_sizes(SIZE, 0),
    tail_size(0),
    file_size(0)
{}

void coder::update(vector<unsigned char> a) {
    file_size += a.size();
    // for-each
    for (size_t i = 0; i < a.size(); i++) {
        cnt[a[i]]++;
    }
}

inf coder::build() {
    // убери std::pair и сделай структуру с полями и нормальными названиями
    std::set< std::pair<size_t, node *> > q;
    cnt[0]++; cnt[1]++;
    for (size_t i = 0; i < SIZE; i++) {
        if (cnt[i] != 0) {
            node *t = new node(i);
            q.insert({cnt[i], t});
        }
    }

    // но ведь так намного понятнее)))
    while (q.size() != 1) {
        auto a = *q.begin();
        q.erase(q.begin());

        auto b = *q.begin();
        q.erase(q.begin());

        node *t = new node(SIZE, a.second, b.second);
        q.insert({a.first + b.first, t});
    }

    inf ret;
    ret.tree.push_back(0);
    int pos = 7;
    tree_traversal(q.begin()->second, 0, 0, ret.symbols, pos, ret.tree);
    ret.file_size = std::inner_product(codes_sizes.begin(), codes_sizes.end(), cnt.begin(), 0);
    ret.file_size -= codes_sizes[0] + codes_sizes[1];
    return ret;
}

vector<unsigned char> coder::code(vector<unsigned char> a) {
    size_t block_size = std::accumulate(a.begin(), a.end(), tail_size,
                                        [&](size_t sum, uint8_t c) { return sum + codes_sizes[c];});


    size_t block = block_size / 8;
    size_t ost = block_size % 8;
    if (file_size == a.size()) {
        if (ost) {
            ost = 0;
            block++;
        }
    }

    vector<unsigned char> ret(block + 1, 0);
    unsigned pos = 8;
    int ind = 0;
    if (tail_size) {
        pos = pos - tail_size;
        ret[0] = tail << pos;
    }

    // for-each based loop
    for (size_t i = 0; i < a.size(); i++) {
        if (codes_sizes[a[i]] <= pos) {
            pos = pos - codes_sizes[a[i]];
            ret[ind] += codes[a[i]] << pos;
        } else {
            ret[ind] += codes[a[i]] >> (codes_sizes[a[i]] - pos);
            ind++;
            pos = codes_sizes[a[i]] - pos;
            while (pos >= 8) {
                ret[ind] += (codes[a[i]] % (1 << pos)) >> (pos - 8);
                ind++;
                pos = pos - 8;
            }
            ret[ind] += (codes[a[i]] % (1 << pos)) << (8 - pos);
            pos = 8 - pos;
        }
        if (pos == 0) {
            pos = 8;
            ind++;
        }
    }
    if (ost) {
        tail = ret[ind] >> pos;
    }
    tail_size = ost;
    ret.pop_back();
    file_size -= a.size();
    return ret;
}

// подумай, чтобы уменьшить число принимаемых аргументов
void coder::tree_traversal(node *root, unsigned code, unsigned s,
                    vector<unsigned char> &symbols, int &pos,
                    vector<unsigned char> &tree)
{
    if (root->val != SIZE) {
        symbols.push_back(root->val);
        codes[root->val] = code;
        codes_sizes[root->val] = s;
        tree[tree.size() - 1] += 1 << pos;
        pos--;
        if (pos == -1) {
            pos = 7;
            tree.push_back(0);
        }
        delete root;
        return;
    }
    pos--;
    if (pos == -1) {
        pos = 7;
        tree.push_back(0);
    }
    tree_traversal(root->left, code * 2, s + 1, symbols, pos, tree);
    tree_traversal(root->right, code * 2 + 1, s + 1, symbols, pos, tree);
    delete root;
}
