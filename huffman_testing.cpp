#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <utility>
#include <gtest/gtest.h>

#include <string>
#include "lib/coder.h"
#include "lib/decoder.h"

TEST(correctness, test1)
{
    coder z;
    std::string s = "abracadabra";
    vector<unsigned char> a(s.begin(), s.end()), b;
    z.update(a);
    inf inf = z.build();
    b = z.code(a);
    decoder dz;
    dz.build(inf);
    b = dz.decode(b);
    EXPECT_EQ(a, b);
}

TEST(correctness, test2)
{
    coder z;
    std::string s = "abkakdjlfh;awsoient[uvaowpn;qrupaw;oevlkajmbswvnqo/I;28U95O7[]";
    vector<unsigned char> a(s.begin(), s.end()), b1, b2;
    z.update(a);
    z.update(a);
    inf inf = z.build();
    b1 = z.code(a);
    b2 = z.code(a);
    decoder dz;
    dz.build(inf);
    b1 = dz.decode(b1);
    b2 = dz.decode(b2);
    a.insert(a.end(), s.begin(), s.end());
    b1.insert(b1.end(), b2.begin(), b2.end());
    EXPECT_EQ(a, b1);
}

TEST(correctness, test3)
{
    coder z;
    std::string s = "";
    for (char i = 'b'; i < 'm'; i++) {
        for (int j = 0; j <= (1 << (i - 'a')); j++) {
            s += i;
        }
    }
    s.insert(s.begin() + 17, 'a');
    vector<unsigned char> a(s.begin(), s.end()), b;
    z.update(a);
    inf inf = z.build();
    b = z.code(a);
    decoder dz;
    dz.build(inf);
    b = dz.decode(b);
    EXPECT_EQ(a, b);
}

TEST(throw_exceptions, test4)
{
    coder z;
    std::string s = "abracadabra";
    vector<unsigned char> a(s.begin(), s.end()), b;
    z.update(a);
    inf inf = z.build();
    b = z.code(a);
    decoder dz;
    //dz.build(inf);
    EXPECT_ANY_THROW(dz.decode(b));
}

TEST(throw_exceptions, test5)
{
    decoder dz;
    inf inf;
    inf.tree = vector<unsigned char> (1, 0);
    inf.symbols = vector<unsigned char> (100, 0);
    EXPECT_ANY_THROW(dz.build(inf););
}

TEST(throw_exceptions, test6)
{
    coder z;
    std::string s = "abracadabra";
    vector<unsigned char> a(s.begin(), s.end()), b;
    z.update(a);
    inf inf = z.build();
    b = z.code(a);
    inf.tree[0] = (1 << 7);
    decoder dz;
    dz.build(inf);
    EXPECT_ANY_THROW(dz.decode(b));
}


