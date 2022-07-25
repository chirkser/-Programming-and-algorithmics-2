#ifndef __PROGTEST__
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stdexcept>
#include <algorithm>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CDataType
{
public:
    virtual size_t getSize() const
    {
        return 0;
    }
    virtual bool equalDataTypes(const CDataType &b) const
    {
        return false;
    }
    virtual bool nonEqualDataTypes(const CDataType &b) const
    {
        return true;
    }
    virtual string printType() const
    {
        return "none";
    }
    virtual shared_ptr<CDataType> sharedData() const
    {
        return make_shared<CDataType>(*this);
    }

    friend ostream& operator << (ostream & output, const CDataType& a )
    {
        output << a.printType();
        return output;
    }

    friend bool operator == (const CDataType &a, const CDataType& b )
    {
        return a.equalDataTypes(b);
    }
    friend bool operator != (const CDataType &a, const CDataType& b )
    {
        return a.nonEqualDataTypes(b);
    }

};

class CDataTypeInt : public CDataType
{
public:
    size_t getSize() const
    {
        return 4;
    }

    string printType() const
    {
        return "int";
    }

    bool equalDataTypes(const CDataType &b) const
    {
        return "int" == b.printType();
    }
    bool nonEqualDataTypes(const CDataType &b) const
    {
        return !equalDataTypes(b);
    }

    virtual shared_ptr<CDataType> sharedData() const {
        return make_shared<CDataTypeInt>(*this);
    }
private:
    // todo
};
class CDataTypeDouble: public CDataType
{
public:
    size_t getSize() const
    {
        return 8;
    }

    string printType() const
    {
        return "double";
    }

    bool equalDataTypes(const CDataType &b) const
    {
        return "double" == b.printType();
    }
    bool nonEqualDataTypes(const CDataType &b) const
    {
        return !equalDataTypes(b);
    }

    virtual shared_ptr<CDataType> sharedData() const {

        return make_shared<CDataTypeDouble>(*this);
    }
};
class CDataTypeEnum: public CDataType
{
public:
    size_t getSize() const
    {
        return 4;
    }

    CDataTypeEnum& add(string newField)
    {
        if (find(fields.begin(), fields.end(), newField) != fields.end())
            throw invalid_argument("Duplicate enum value: " + newField);

        fields.push_back(newField);
        return *this;
    }

    string printType() const
    {
        ostringstream oss;
        oss << " enum\n {\n";
        for (auto field = fields.begin(); field != fields.end(); field++)
        {
            oss <<"    " << *field;
            if (fields.end() - 1 == field)
                oss << "\n";
            else
                oss << ",\n";
        }
        oss << "  }";
        return oss.str();
    }

    bool equalDataTypes(const CDataType &b) const
    {
        CDataTypeEnum const  * other = dynamic_cast<CDataTypeEnum const*>(&b);
        if (other == nullptr || fields.size() != other->fields.size())
        {
            return false;
        }


        auto itB = other->fields.begin();
        for (auto it = fields.begin(); it != fields.end(); it++, itB++)
        {
            if (*it != *itB)
            {
                return false;
            }

        }
        return true;
    }
    bool nonEqualDataTypes(const CDataType &b) const
    {
        return !equalDataTypes(b);
    }

    virtual shared_ptr<CDataType> sharedData() const
    {
        return make_shared<CDataTypeEnum>(*this);
    }
private:
    vector<string> fields;
};

class CDataTypeStruct: public CDataType
{
public:
    size_t getSize() const
    {
        size_t size = 0;
        for (auto & field : fields)
        {
            size+= field.second.get()->getSize();
        }
        return size;
    }

    CDataTypeStruct& addField( string name, const CDataType& type)
    {
        for (auto it = fields.begin(); it != fields.end(); it++)
        {
            if (it->first == name)
                throw invalid_argument("Duplicate field: "+ name);
        }
        fields.push_back(make_pair(name,type.sharedData()));
        return *this;
    }

    CDataType& field(string name) const
    {
        bool flag = false;
        auto it = fields.begin();
        for (; it != fields.end(); it++)
        {
            if (it->first == name)
            {
                flag = true;
                break;
            }
        }
        if (!flag)
            throw invalid_argument("Unknown field: " + name);
        return *it->second;
    }
    string printType() const
    {
        ostringstream oss;
        oss << "struct\n{\n";
        for (auto & field : fields)
        {
            oss<< field.second.get()->printType() << " " << field.first  << ";\n";
        }
        oss << "}";
        return oss.str();
    }

    bool equalDataTypes(const CDataType &b) const
    {
        CDataTypeStruct const  * other = dynamic_cast<CDataTypeStruct const*>(&b);
        if (other == nullptr || fields.size() != other->fields.size())
            return false;

        auto itB = other->fields.begin();
        for (auto it = fields.begin(); it != fields.end(); it++, itB++)
        {
            if (*it->second != *itB->second)
                return false;
        }
        return true;
    }

    bool nonEqualDataTypes(const CDataType &b) const
    {
        return !equalDataTypes(b);
    }

    virtual shared_ptr<CDataType> sharedData() const {
        return make_shared<CDataTypeStruct>(*this);
    }
private:
    vector<pair<string, shared_ptr<CDataType>>> fields;
};
#ifndef __PROGTEST__
static bool        whitespaceMatch                         ( const string    & a,
                                                             const string    & b )
{
    // todo
    return true;
}
template <typename T_>
static bool        whitespaceMatch                         ( const T_        & x,
                                                             const string    & ref )
{
    ostringstream oss;
    oss << x;
    return whitespaceMatch ( oss . str (), ref );
}
int main ( void )
{

    CDataTypeStruct  a = CDataTypeStruct () .
            addField ( "m_Length", CDataTypeInt () ) .
            addField ( "m_Status", CDataTypeEnum () .
            add ( "NEW" ) .
            add ( "FIXED" ) .
            add ( "BROKEN" ) .
            add ( "DEAD" ) ).
            addField ( "m_Ratio", CDataTypeDouble () );


    CDataTypeStruct b = CDataTypeStruct () .
            addField ( "m_Length", CDataTypeInt () ) .
            addField ( "m_Status", CDataTypeEnum () .
            add ( "NEW" ) .
            add ( "FIXED" ) .
            add ( "BROKEN" ) .
            add ( "READY" ) ).
            addField ( "m_Ratio", CDataTypeDouble () );

    CDataTypeStruct c =  CDataTypeStruct () .
            addField ( "m_First", CDataTypeInt () ) .
            addField ( "m_Second", CDataTypeEnum () .
            add ( "NEW" ) .
            add ( "FIXED" ) .
            add ( "BROKEN" ) .
            add ( "DEAD" ) ).
            addField ( "m_Third", CDataTypeDouble () );

    CDataTypeStruct  d = CDataTypeStruct () .
            addField ( "m_Length", CDataTypeInt () ) .
            addField ( "m_Status", CDataTypeEnum () .
            add ( "NEW" ) .
            add ( "FIXED" ) .
            add ( "BROKEN" ) .
            add ( "DEAD" ) ).
            addField ( "m_Ratio", CDataTypeInt () );
    assert ( whitespaceMatch ( a, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Status;\n"
                                  "  double m_Ratio;\n"
                                  "}") );

    assert ( whitespaceMatch ( b, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    READY\n"
                                  "  } m_Status;\n"
                                  "  double m_Ratio;\n"
                                  "}") );

    assert ( whitespaceMatch ( c, "struct\n"
                                  "{\n"
                                  "  int m_First;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Second;\n"
                                  "  double m_Third;\n"
                                  "}") );

    assert ( whitespaceMatch ( d, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Status;\n"
                                  "  int m_Ratio;\n"
                                  "}") );

    assert ( a != b );
    assert ( a == c );
    assert ( a != d );
    assert ( a . field ( "m_Status" ) == CDataTypeEnum () . add ( "NEW" ) . add ( "FIXED" ) . add ( "BROKEN" ) . add ( "DEAD" ) );
    assert ( a . field ( "m_Status" ) != CDataTypeEnum () . add ( "NEW" ) . add ( "BROKEN" ) . add ( "FIXED" ) . add ( "DEAD" ) );
    assert ( a != CDataTypeInt() );
    assert ( whitespaceMatch ( a . field ( "m_Status" ), "enum\n"
                                                         "{\n"
                                                         "  NEW,\n"
                                                         "  FIXED,\n"
                                                         "  BROKEN,\n"
                                                         "  DEAD\n"
                                                         "}") );

    CDataTypeStruct aOld = a;
    b . addField ( "m_Other", CDataTypeDouble ());

    a . addField ( "m_Sum", CDataTypeInt ());

    assert ( a != aOld );
    assert ( a != c );
    assert ( aOld == c );
    assert ( whitespaceMatch ( a, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Status;\n"
                                  "  double m_Ratio;\n"
                                  "  int m_Sum;\n"
                                  "}") );

    assert ( whitespaceMatch ( b, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    READY\n"
                                  "  } m_Status;\n"
                                  "  double m_Ratio;\n"
                                  "  double m_Other;\n"
                                  "}") );

    c . addField ( "m_Another", a . field ( "m_Status" ));

    assert ( whitespaceMatch ( c, "struct\n"
                                  "{\n"
                                  "  int m_First;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Second;\n"
                                  "  double m_Third;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Another;\n"
                                  "}") );

    d . addField ( "m_Another", a . field ( "m_Ratio" ));

    assert ( whitespaceMatch ( d, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Status;\n"
                                  "  int m_Ratio;\n"
                                  "  double m_Another;\n"
                                  "}") );

    assert ( a . getSize () == 20 );
    assert ( b . getSize () == 24 );
    try
    {
        a . addField ( "m_Status", CDataTypeInt () );
        assert ( "addField: missing exception!" == nullptr );
    }
    catch ( const invalid_argument & e )
    {
        assert ( e . what () == "Duplicate field: m_Status"sv );
    }
    try
    {
        cout << a . field ( "m_Fail" ) << endl;
        assert ( "field: missing exception!" == nullptr );
    }
    catch ( const invalid_argument & e )
    {
        assert ( e . what () == "Unknown field: m_Fail"sv );
    }
    try
    {
        CDataTypeEnum en;
        en . add ( "FIRST" ) .
                add ( "SECOND" ) .
                add ( "FIRST" );
        assert ( "add: missing exception!" == nullptr );
    }
    catch ( const invalid_argument & e )
    {
        assert ( e . what () == "Duplicate enum value: FIRST"sv );
    }

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */