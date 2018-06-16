#include "coder.h"
#include <set>
#include <numeric>

coder::coder() :
    cnt(SIZE, 0),
    codes(SIZE),
    codes_sizes(SIZE, 0),
    tail_size(0),
    file_size(0)
{}

void coder::update(std::vector<unsigned char> const &a) {
    file_size += a.size();
    for (auto ch : a) {
        cnt[ch]++;
    }
}

info coder::build() {
    std::multiset<pair> q;
    cnt[0]++; cnt[1]++;
    for (size_t i = 0; i < SIZE; i++) {
        if (cnt[i] != 0) {
            std::shared_ptr<node> t(new node(i));
            q.insert(pair(cnt[i], t));
        }
    }

    while (q.size() != 1) {        
        pair a = *q.begin();
        q.erase(q.begin());

        pair b = *q.begin();
        q.erase(q.begin());

        std::shared_ptr<node> t(new node(SIZE, a.ptr, b.ptr));
        q.insert({a.freq + b.freq, t});
    }

    info ret;
    ret.tree.push_back(0);
    int pos = 7;
    tree_traversal(q.begin()->ptr, 0, 0, ret, pos);
    ret.file_size = std::inner_product(codes_sizes.begin(), codes_sizes.end(), cnt.begin(), -(codes_sizes[0] + codes_sizes[1]));
    return ret;
}

std::vector<unsigned char> coder::code(std::vector<unsigned char> const &a) {
    size_t block_size = std::accumulate(a.begin(), a.end(), tail_size, [&](size_t sum, unsigned char c)
                                                                        { return sum + codes_sizes[c]; });

    size_t block = block_size / 8;
    size_t ost = block_size % 8;
    if (file_size == a.size()) {
        if (ost) {
            ost = 0;
            block++;
        }
    }

    std::vector<unsigned char> ret(block + 1, 0);
    unsigned pos = 8;
    int ind = 0;
    if (tail_size) {
        pos = pos - tail_size;
        ret[0] = tail << pos;
    }

    for (auto ch : a) {
        if (codes_sizes[ch] <= pos) {
            pos = pos - codes_sizes[ch];
            ret[ind] += codes[ch] << pos;
        } else {
            ret[ind] += codes[ch] >> (codes_sizes[ch] - pos);
            ind++;
            pos = codes_sizes[ch] - pos;
            while (pos >= 8) {
                ret[ind] += (codes[ch] % (1 << pos)) >> (pos - 8);
                ind++;
                pos = pos - 8;
            }
            ret[ind] += (codes[ch] % (1 << pos)) << (8 - pos);
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

void coder::tree_traversal(std::shared_ptr<node> root, unsigned code, unsigned s, info &ret, int &pos)
{
    if (root->val != SIZE) {
        ret.symbols.push_back(root->val);
        codes[root->val] = code;
        codes_sizes[root->val] = s;
        ret.tree[ret.tree.size() - 1] += 1 << pos;
        pos--;
        if (pos == -1) {
            pos = 7;
            ret.tree.push_back(0);
        }
        return;
    }
    pos--;
    if (pos == -1) {
        pos = 7;
        ret.tree.push_back(0);
    }
    tree_traversal(root->left, code * 2, s + 1, ret, pos);
    tree_traversal(root->right, code * 2 + 1, s + 1, ret, pos);
}
