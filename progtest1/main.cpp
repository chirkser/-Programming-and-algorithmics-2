#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

class Node
{
public:
    void setLeaf(char s)
    {
        leaf = true;
        symbol = s;
    }

    char getSymbol()
    {
        return symbol;
    }

    bool isLeaf()
    {
        return leaf;
    }

    bool isChildFree()
    {
        return  ((left == nullptr && right == nullptr) && !leaf );
    }

    void createLeftChild()
    {
        left = new Node();
    }

    void createRightChild()
    {
        right = new Node();
    }

    Node * getLeftChild()
    {
        return left;
    }

    Node * getRightChild()
    {
        return right;
    }
private:
    bool leaf = false;
    char symbol;
    Node * left = nullptr, * right = nullptr;
};

class Tree
{
public:

    ~Tree()
    {
        queue <Node*> qq;
        for (qq.push(root); !qq.empty(); qq.pop()) {
            Node * temp_node = qq.front();
            if (temp_node -> getRightChild()) {
                qq.push(temp_node -> getRightChild());
            }
            if (temp_node-> getLeftChild()) {
                qq.push(temp_node->getLeftChild());
            }
            delete temp_node;
        }
    }
    void createRoot()
    {
        root = new Node();
    }

    Node * getRoot()
    {
        return root;
    }

    bool isFull()
    {
        queue<Node*> qq;
        if (!root) {
            return false;
        }
        for (qq.push(root); !qq.empty(); qq.pop()) {
            Node * temp_node = qq.front();
            if (temp_node->isChildFree())
            {
                return false;
            }
//            if (temp_node->isLeaf())
//            {
//                continue;
//            }
            if (temp_node -> getRightChild()) {
                qq.push(temp_node -> getRightChild());
            }
            if (temp_node-> getLeftChild()) {
                qq.push(temp_node->getLeftChild());
            }

        }
        return true;
    }

    pair <bool, char> decode(queue <uint8_t> & binData)
    {

        bool flag = true;
        Node * tmp = root;
        while (!tmp->isLeaf())
        {
            if (binData.empty())
            {
                flag = false;
                return make_pair(flag, '\0');
            }
            if (binData.front() == 0)
            {
                tmp = tmp->getLeftChild();
                binData.pop();
            } else if (binData.front() == 1)
            {
                tmp = tmp->getRightChild();
                binData.pop();
            }
        }
        return make_pair(flag, tmp->getSymbol());
    }

private:
    Node * root = nullptr;
};

bool convertByte(char symbol, queue <uint8_t> & binData)
{
    for (int i = 7; i >= 0; i--)
    {
        binData.push( (symbol >> i) & 1);
    }
    return true;
}

bool readData(ifstream & in, queue <uint8_t> & binData)
{
    char symbol;
    while (in.get(symbol).good())
    {
        convertByte(symbol, binData);
    }
    return true;
}

int readTwelveBits(queue <uint8_t> & binData)
{
    if (binData.empty() || binData.size() < 12)
    {
        return false;
    }
    int result = 0;
    int base = 2048;
    for (int i = 0; i < 12; ++i)
    {
        result += binData.front() * base;
        base/=2;
        binData.pop();
    };

    return result;
}

bool readLastChunk(queue <uint8_t> & binData, int size, Tree * tree, ofstream & out)
{
    pair <bool, char> error;
    if( size == 0) return true;
    if (binData.empty())
    {
        return false;
    }
    for (int i = 0; i < size; i++)
    {
        // cout << "last pre : " << binData.size() <<"  -----  ";
        error = tree->decode(binData);
        // cout << " after : " << binData.size() <<endl;
        if (!error.first)
        {
            return false;
        }
        if(!(out << error.second))
        {
            return false;
        }
    }
//    for ( auto & x : binData)  MY BYDEM ZNAT` GDE
//    {
//        if (x == 1) return false;
//    }
    return true;
}

bool readChunk(queue <uint8_t> & binData, int size, Tree * tree, ofstream & out)
{
    if (binData.empty())
    {
        return false;
    }
    pair <bool, char> tmp;
    for (int i = 0; i < size; i++)
    {
        // cout << " pre : " << binData.size() <<"  -----  ";
        tmp = tree->decode(binData);
        // cout << " after : " << binData.size() <<endl;
        if (!tmp.first)
        {
            return false;
        }
        if(!(out << tmp.second))
        {
            return false;
        }

    }
    return true;
}

pair <bool,char> readChar(queue <uint8_t> & binData)
{
    pair <bool, char> tmp;
    tmp.first = true;
    if (binData.empty())
    {
        tmp.first = false;
        return tmp;
    }
    int result = 0;
    int base = 128;
    for (int i = 0; i < 8; ++i)
    {
        if (binData.empty())
        {
            tmp.first = false;
            return tmp;
        }
        result += binData.front() * base;
        base/=2;
        binData.pop();
    }
    tmp.second = (char)(result);
    return tmp;
}


bool readTree(queue <uint8_t> & binData, Tree * tree, Node * root)
{
    pair <bool, char> tmp;
    if (binData.empty())
    {
        return false;
    }
    if (tree->isFull())
    {
        return true;
    }
    if (binData.front() == 0)
    {
        binData.pop();
        root->createLeftChild();
        readTree(binData, tree, root->getLeftChild());
        root->createRightChild();
        readTree(binData, tree, root->getRightChild());
    } else if (binData.front() == 1)
    {
        binData.pop();
        tmp = readChar(binData);
        if (!tmp.first)
        {
            return false;
        }
        root->setLeaf(tmp.second);
    }
    return true;
}


bool decompressFile ( const char * inFileName, const char * outFileName )
{
    queue <uint8_t> binData;
    Tree * tree = new Tree();
    tree->createRoot();
    ifstream in(inFileName,  ios::binary);
    if ( !(in.is_open()) || !in.good() )
    {
        in.close();
        delete tree;
        return false;
    }
    if (!readData(in, binData))
    {
        in.close();
        delete tree;
        return false;
    }
    if (!readTree(binData, tree, tree->getRoot()))
    {
        in.close();
        delete tree;
        return false;
    }
    int size_of_last = 0;
    ofstream out(outFileName, ios::binary);
    if (!out.is_open() || !out.good())
    {
        in.close();
        out.close();
        delete tree;
        return false;
    }
    while (true)
    {
        if (binData.empty())
        {
            in.close();
            out.close();
            delete tree;
            return false;
        }
        if (binData.front() == 0)
        {
            binData.pop();
            size_of_last = readTwelveBits(binData);
            if (!readLastChunk(binData, size_of_last, tree, out))
            {
                in.close();
                out.close();
                delete tree;
                return false;
            }
            break;
        } else if (binData.front() == 1)
        {
            binData.pop();
            if (!readChunk(binData, 4096, tree, out))
            {
                in.close();
                out.close();
                delete tree;
                return false;
            }
        }
    }
    in.close();
    out.close();
    delete tree;
    return true;
}

bool compressFile ( const char * inFileName, const char * outFileName )
{
    // keep this dummy implementation (no bonus) or implement the compression (bonus)
    return false;
}

#ifndef __PROGTEST__
bool identicalFiles ( const char * fileName1, const char * fileName2 )
{
    // todo
    return false;
}

int main ( void )
{
    assert ( decompressFile ( "tests/test0.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test0.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/test1.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test1.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/test2.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test2.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/test3.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test3.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/test4.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test4.orig", "tempfile" ) );

    assert ( ! decompressFile ( "tests/test5.huf", "tempfile" ) );


    assert ( decompressFile ( "tests/extra0.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra0.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra1.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra1.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra2.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra2.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra3.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra3.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra4.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra4.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra5.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra5.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra6.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra6.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra7.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra7.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra8.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra8.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra9.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra9.orig", "tempfile" ) );

    return 0;
}
#endif /* __PROGTEST__ */
