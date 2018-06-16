#include "lib/coder.h"
#include "lib/decoder.h"
#include <ios>
#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

const size_t LENGTH = 10000;

void read_buf(ifstream &in, vector<unsigned char> &buf) {
    in.read((char*) (buf.data()), LENGTH);
    buf.resize(in.gcount());
}

void code(ifstream &in, ofstream &out) {
    vector<unsigned char> buf(LENGTH), block;
    coder z;
    do {
        read_buf(in, buf);
        z.update(buf);
    } while (in);

    info inf = z.build();
    for (int i = 0; i < 8; i++) out << inf.fz[i];

    out << (unsigned char) inf.tree.size() << (unsigned char) (inf.symbols.size() - 1);
    out.write((char*) (inf.symbols.data()), inf.symbols.size());
    out.write((char*) (inf.tree.data()), inf.tree.size());

    buf.resize(LENGTH);
    in.clear();
    in.seekg(0, ios::beg);
    do {
        read_buf(in, buf);
        block = z.code(buf);
        out.write((char*) (block.data()), block.size());
    } while (in);
}

void decode(ifstream &in, ofstream &out) {
    info inf;
    inf.file_size = 0;
    for (int i = 0; i < 8; i++) in.get(inf.fz[i]);
    unsigned char tmp = 1;
    in.get((char&) tmp);
    inf.tree.resize(tmp);
    in.get((char&) tmp);
    inf.symbols.resize(tmp + 1);
    in.read((char*) (inf.symbols.data()), inf.symbols.size());
    in.read((char*) (inf.tree.data()), inf.tree.size());
    if (in.fail()) {
        throw runtime_error("Not enough control data");
    }
    decoder z;
    z.build(inf);

    vector<unsigned char> buf(LENGTH), block;
    do {
        read_buf(in, buf);
        block = z.decode(buf);
        out.write((char*) (block.data()), block.size());
    } while (in);
    if (!z.is_all_data_decoded()) {
        throw runtime_error("Expected more data");
    }
}

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);
    if (argc != 4) {
        cerr << "Correct use: -[c|d] src dst\n";
        return 0;
    }
    ifstream in(argv[2], ios::binary);
    ofstream out(argv[3], ios::binary);
    try {
        if (in.fail()) {
            throw runtime_error("Opening file failed");
        }
        if (string(argv[1]).compare("-c") == 0) {
            code(in, out);
        } else if (string(argv[1]).compare("-d") == 0) {
            decode(in, out);
        } else {
            cerr << "Correct use: -[c|d] src dst\n";
        }

    } catch (runtime_error &e) {
        cerr << "Error occurred:\n";
        cerr << e.what() << '\n';
    }
}
