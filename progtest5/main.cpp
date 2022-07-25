#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CDate
{
public:
    CDate() {};
    CDate(int y, int m, int d);
    CDate(const CDate & other);
    bool operator==(const CDate & other) const;
    bool operator<(const CDate & other) const;
    bool operator>(const CDate & other) const;
    friend ostream & operator << (ostream & os, const CDate & tmp);

private:
    int year;
    int month;
    int day;
};

CDate::CDate(int y, int m, int d) : year(y), month(m), day(d) {}

CDate::CDate(const CDate &other)
{
    this->year = other.year;
    this->month = other.month;
    this->day = other.day;
}

bool CDate::operator==(const CDate &other) const
{
    if (this->year == other.year &&
        this->month == other.month &&
        this->day == other.day)
        return true;
    else
        return false;
}

bool CDate::operator<(const CDate &other) const
{
    if (this->year < other.year)
    {
        return true;
    }
    else if (this->year == other.year)
    {
        if (this->month < other.month)
        {
            return true;
        }
        else if (this->month == other.month)
        {
            if (this->day < other.day)
            {
                return true;
            }
        }
    }
    return false;
}

bool CDate::operator>(const CDate &other) const
{
    return !(*this < other);
}

ostream & operator << (ostream & os, const CDate & tmp)
{
    os << "[" << tmp.day << "." << tmp.month << "." << tmp.year << "]";
    return os;
}

class CSupermarket
{
public:
    CSupermarket() = default;
    CSupermarket & store (const string& name, const CDate & expiryDate, int count);
    void printStock();
    list<pair<string,int> > expired (const CDate & date) const;
    void sell (list<pair<string,int>> & products);
private:
    struct cmpMap {
        bool operator()(const CDate & a, const CDate & b) const {
            return a < b;
        }
    };

    void sellProduct(pair<string, int> & product);
    unordered_map<string, map <CDate, int, cmpMap>> stock;
};




bool isNull (const pair <string, int> & value) { return value.second == 0; }

void CSupermarket::printStock()
{
    for (auto & i: stock)
        for (auto & j: i.second)
        {
            cout << "Name: " << i.first << " (" << j.first << ") " << j.second << endl;
        }
    cout << endl;
}


CSupermarket &CSupermarket::store(const string& name, const CDate &expiryDate, int count)
{
    if (stock.find(name) == stock.end())
    {
        map <CDate, int, cmpMap> tmp;
        tmp.insert( {expiryDate, count} );
        stock[name] = tmp;
    } else
    {
        stock[name][expiryDate] = count;
    }
    return *this;
}

bool cmpPieces(const pair<string, int> & A, const pair<string, int> & B)
{
    return A.second > B.second;
}

list <pair<string, int>> CSupermarket::expired(const CDate &date) const
{
    list <pair<string, int>> tmp;
    for (auto & i : stock)
    {
        int pieces = 0;
        for (auto j = i.second.begin(); j != i.second.end(); j++)
        {
           if (j->first < date)
           {
               pieces += j->second;
           }
        }
        if (pieces != 0)
            tmp.emplace_back(i.first, pieces);
    }
    tmp.sort(cmpPieces);
    return tmp;
}

bool compareStrings(const string & A, const string & B)
{
    int cnt = 0;
    if (A.length() != B.length())
        return false;
    else
    {
        for (int i = 0; i < (int)A.length(); i++)
        {
            if (A[i] != B[i])
                cnt++;
        }
        if (cnt > 1)
            return false;
    }
    return true;
}


void CSupermarket::sell(list <pair<string, int>> &products)
{
    for (auto & i: products)
    {
        if (stock.find(i.first) != stock.end())
        {
            sellProduct(i);
        }
        else
        {
            int cnt = 0;
            for (auto & j: stock)
            {
                if (compareStrings(i.first, j.first)) {
                    cnt++;
                    if (cnt < 2)
                    {
                        sellProduct(i);
                    }

                }
            }
        }
    }
    products.remove_if(isNull);
    for (auto i = stock.begin();  i != stock.end(); )
    {
        if (i->second.empty())
        {
            i = stock.erase(i);
        }
        else
        {
            i++;
        }
    }
}

void CSupermarket::sellProduct(pair<string, int> & product)
{
    string name = product.first;
    int amount = product.second;

    unordered_map <string, map<CDate, int, cmpMap>> :: iterator tmp;
    int cnt = 0;
    if (stock.find(name) == stock.end())
    {
        for (auto & j: stock)
        {
            if (compareStrings(j.first, name))
            {
                cnt++;
                if (cnt < 2)
                {
                    tmp = stock.find(j.first);
                }
            }
        }
        if (cnt >= 2)
        {
            return;
        }
    } else
    {
        tmp = stock.find(name);
    }
    map <CDate, int, cmpMap> & potato = tmp->second;
    for (auto i = potato.begin(); i != potato.end(); )
    {
        if (amount == 0)
            break;
        if (i->second <= amount)
        {
            amount -= i->second;
            i = potato.erase(i);
        }
        else
        {
            i->second -= amount;
            amount = 0;
            i++;
        }
    }
    product.second = amount;
}



#ifndef __PROGTEST__
int main ( void )
{
    CSupermarket s;
    s . store ( "bread", CDate ( 2016, 4, 30 ), 100 )
            . store ( "butter", CDate ( 2016, 5, 10 ), 10 )
            . store ( "beer", CDate ( 2016, 8, 10 ), 50 )
            . store ( "bread", CDate ( 2016, 4, 25 ), 100 )
            . store ( "okey", CDate ( 2016, 7, 18 ), 5 );

    list<pair<string,int> > l0 = s . expired ( CDate ( 2018, 4, 30 ) );
    assert ( l0 . size () == 4 );
    assert ( ( l0 == list<pair<string,int> > { { "bread", 200 }, { "beer", 50 }, { "butter", 10 }, { "okey", 5 } } ) );

    list<pair<string,int> > l1 { { "bread", 2 }, { "Coke", 5 }, { "butter", 20 } };
    s . sell ( l1 );
    assert ( l1 . size () == 2 );
    assert ( ( l1 == list<pair<string,int> > { { "Coke", 5 }, { "butter", 10 } } ) );

    list<pair<string,int> > l2 = s . expired ( CDate ( 2016, 4, 30 ) );
    assert ( l2 . size () == 1 );
    assert ( ( l2 == list<pair<string,int> > { { "bread", 98 } } ) );

    list<pair<string,int> > l3 = s . expired ( CDate ( 2016, 5, 20 ) );
    assert ( l3 . size () == 1 );
    assert ( ( l3 == list<pair<string,int> > { { "bread", 198 } } ) );

    list<pair<string,int> > l4 { { "bread", 105 } };
    s . sell ( l4 );
    assert ( l4 . size () == 0 );
    assert ( ( l4 == list<pair<string,int> > {  } ) );

    list<pair<string,int> > l5 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l5 . size () == 3 );
    assert ( ( l5 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 } } ) );

    s . store ( "Coke", CDate ( 2016, 12, 31 ), 10 );

    list<pair<string,int> > l6 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 }, { "Cokes", 1 } };
    s . sell ( l6 );
    assert ( l6 . size () == 3 );
    assert ( ( l6 == list<pair<string,int> > { { "cake", 1 }, { "cuke", 1 }, { "Cokes", 1 } } ) );

    list<pair<string,int> > l7 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l7 . size () == 4 );
    assert ( ( l7 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 7 }, { "okey", 5 } } ) );

    s . store ( "cake", CDate ( 2016, 11, 1 ), 5 );

    list<pair<string,int> > l8 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 } };
    s . sell ( l8 );
    assert ( l8 . size () == 2 );
    assert ( ( l8 == list<pair<string,int> > { { "Cake", 1 }, { "coke", 1 } } ) );

    list<pair<string,int> > l9 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l9 . size () == 5 );
    assert ( ( l9 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 }, { "cake", 3 } } ) );

    list<pair<string,int> > l10 { { "cake", 15 }, { "Cake", 2 } };
    s . sell ( l10 );
    assert ( l10 . size () == 2 );
    assert ( ( l10 == list<pair<string,int> > { { "cake", 12 }, { "Cake", 2 } } ) );

    list<pair<string,int> > l11 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l11 . size () == 4 );
    assert ( ( l11 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 } } ) );

    list<pair<string,int> > l12 { { "Cake", 4 } };
    s . sell ( l12 );
    assert ( l12 . size () == 0 );
    assert ( ( l12 == list<pair<string,int> > {  } ) );

    list<pair<string,int> > l13 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l13 . size () == 4 );
    assert ( ( l13 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 }, { "Coke", 2 } } ) );

    list<pair<string,int> > l14 { { "Beer", 20 }, { "Coke", 1 }, { "bear", 25 }, { "beer", 10 } };
    s . sell ( l14 );
    assert ( l14 . size () == 1 );
    assert ( ( l14 == list<pair<string,int> > { { "beer", 5 } } ) );

    s . store ( "ccccb", CDate ( 2019, 3, 11 ), 100 )
            . store ( "ccccd", CDate ( 2019, 6, 9 ), 100 )
            . store ( "dcccc", CDate ( 2019, 2, 14 ), 100 );

    list<pair<string,int> > l15 { { "ccccc", 10 } };
    s . sell ( l15 );
    assert ( l15 . size () == 1 );
    assert ( ( l15 == list<pair<string,int> > { { "ccccc", 10 } } ) );

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
