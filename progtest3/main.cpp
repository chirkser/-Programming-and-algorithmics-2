#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */


bool isYearLeap (int year) {
    if ( (year % 4 == 0) && (year % 100 != 0 || (year % 400 == 0)) )
    {
        return true;
    } else
    {
        return false;
    }
}

int daysInMonth(int year, int month) {
    switch (month)
    {
        case 2: return isYearLeap(year) ? 29 : 28;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        default:
            return 31;
    }
}

bool isValidData(int year, int month, int day)
{
    return (year >= 2000 && year <= 2030) && (month >= 1 && month <= 12) && day >= 1 && day <= daysInMonth(year, month);
}

void daysAddition(int & year, int & month, int & day, int days)
{
    while (days)
    {
        if (day >= daysInMonth(year, month) )
        {
            if (month == 12)
            {
                day = 1;
                month = 1;
                year++;
            }
            else
            {
                day = 1;
                month++;
            }
        } else
            day++;
        days--;
    }
}

void daysSubstraction(int & year, int & month, int & day, int days)
{
    while (days)
    {
        if (day == 1)
        {
            if (month == 1)
            {
                year--;
                month = 12;
                day = daysInMonth(year,month);
            }
            else
            {
                month--;
                day = daysInMonth(year, month);
            }
        } else
            day--;
        days--;
    }
}

void daysInInterval(int Lyear, int Lmonth, int Lday, int Ryear, int Rmonth, int Rday, int & cnt)
{
    // days from data to next year
    if (Lyear != Ryear)
    {
        bool flag = false;
        while (!flag)
        {
            if (Lday >= daysInMonth(Lyear, Lmonth) )
            {
                if (Lmonth == 12)
                {
                    Lday = 1;
                    Lmonth = 1;
                    Lyear++;
                    flag = true;
                }
                else
                {
                    Lday = 1;
                    Lmonth++;
                }
            } else
                Lday++;
            cnt++;
        }
        while (Lyear != Ryear)
        {
            cnt+= isYearLeap(Lyear) ? 366 : 365;
            Lyear++;
        }
        while (Lday != Rday || Lmonth != Rmonth)
        {
            Lday++;
            cnt++;
            if (Lday >= daysInMonth(Lyear, Lmonth) )
            {
                Lday = 1;
                Lmonth++;
            }
        }
    } else
    {
        while (Lday != Rday || Lmonth != Rmonth)
        {
            Lday++;
            cnt++;
            if (Lday > daysInMonth(Lyear, Lmonth) )
            {
                Lday = 1;
                Lmonth++;
            }
        }
    }
}

void fixDate(int & year, int & month, int & day, bool flag)
{
    if (flag)
    {
        if (day >= daysInMonth(year, month) )
        {
            if (month == 12)
            {
                day = 1;
                month = 1;
                year++;
            }
            else
            {
                day = 1;
                month++;
            }
        }
    } else
    {
        if (day == 1)
        {
            if (month == 1)
            {
                year--;
                month = 12;
                day = daysInMonth(year,month);
            }
            else
            {
                month--;
                day = daysInMonth(year, month);
            }
        }
    }

}

//=================================================================================================
// a dummy exception class, keep this implementation
class InvalidDateException : public invalid_argument
{
public:
    InvalidDateException ( )
            : invalid_argument ( "invalid date or format" )
    {
    }
};
//=================================================================================================
// date_format manipulator - a dummy implementation. Keep this code unless you implement your
// own working manipulator.
ios_base & ( * date_format ( const char * fmt ) ) ( ios_base & x )
{
    return [] ( ios_base & ios ) -> ios_base & { return ios; };
}
//=================================================================================================
class CDate
{
public:
    CDate(int year, int month, int day);

    ~CDate() = default;

    void setYear(int year);

    void setMonth(int month);

    void setDay(int day);

    friend ostream & operator<<(ostream & oss, const CDate & tmp);

    CDate operator+(int days);

    CDate operator-(int days);

    int operator-(CDate & tmp);

    bool operator<(const CDate & tmp) const;

    bool operator>(const CDate & tmp) const;

    bool operator==(const CDate & tmp) const;

    bool operator!=(const CDate & tmp) const;

    bool operator<=(const CDate & tmp) const;

    bool operator>=(const CDate & tmp) const;
    // prefix
    CDate  operator++();
    //postfix
    CDate operator++(int);
    //prefix
    CDate  operator--();
    //postfix
    CDate  operator--(int);

    friend istream & operator>>(istream & iss, const CDate & tmp);

private:
    int day;
    int month;
    int year;
};

CDate::CDate(int year, int month, int day)
{
    if (!isValidData(year, month, day))
    {
        throw InvalidDateException();
    }
    this->year = year;
    this->month = month;
    this->day = day;
}

void CDate::setYear(int year)
{
    this->year = year;
}

void CDate::setMonth(int month)
{
    this->month = month;
}

void CDate::setDay(int day)
{
    this->day = day;
}
ostream & operator<<(ostream & oss, const CDate & tmp)
{
    oss << tmp.year << "-" << setfill('0') << setw(2) << tmp.month << "-" << setfill('0') << setw(2) << tmp.day;
    return oss;
}


istream & operator>>(istream & iss, CDate & tmp)
{
    int year, month, day;
    char firstDash, secondDash;
    iss >> year >> firstDash >> month >> secondDash >> day;
    if (!isValidData(year, month, day) ||
        (firstDash != '-' && secondDash != '-') ||
        !iss.eof() ||
        iss.fail())
    {
        iss.setstate(ios::failbit);
        return iss;
    }
    tmp.setYear(year);
    tmp.setMonth(month);
    tmp.setDay(day);
    return iss;
}

CDate CDate::operator+(int days)
{
    if (days >= 0)
        daysAddition( (*this).year, (*this).month, (*this).day, days);
    else
        daysSubstraction( (*this).year, (*this).month, (*this).day, abs(days) );
    return *this;
}

CDate CDate::operator-(int days)
{
    if (days >= 0)
        daysSubstraction( (*this).year, (*this).month, (*this).day, days);
    else
        daysAddition( (*this).year, (*this).month, (*this).day, abs(days) );
    return *this;
}

int CDate::operator-(CDate &tmp)
{
    int cnt = 0;
    if (*this < tmp)
        daysInInterval(this->year, this->month, this->day,
                       tmp.year, tmp.month, tmp.day, cnt);
    else
        daysInInterval(tmp.year, tmp.month, tmp.day,
                       this->year, this->month, this->day, cnt);
    return cnt;
}

bool CDate::operator<(const CDate &tmp) const
{
    if (this->year == tmp.year)
    {
        if (this->month == tmp.month)
        {
            if (this->day == tmp.day)
                return false;
            else if (this->day > tmp.day)
                return false;
            else
                return true;
        }
        else if (this->month > tmp.month)
            return false;
        else
            return true;
    }
    else if (this->year > tmp.year)
        return false;
    else
        return true;
}

bool CDate::operator>(const CDate &tmp) const
{
    return tmp < (*this);
}

bool CDate::operator==(const CDate &tmp) const
{
    if (this->year == tmp.year && this->month == tmp.month && this->day == tmp.day)
        return true;
    else
        return false;
}

bool CDate::operator!=(const CDate &tmp) const
{
    return !( (*this) == tmp );
}

bool CDate::operator<=(const CDate &tmp) const
{
    return (*this) < tmp || (*this) == tmp;
}

bool CDate::operator>=(const CDate &tmp) const
{
    return tmp <= (*this);
}

CDate  CDate::operator++()
{
    this->day++;
    if ( !isValidData(this->year, this->month, this->day) )
        fixDate(this->year, this->month, this->day, true);
    return *this;
}

CDate CDate::operator++(int)
{
    auto old = *this;
    this->day++;
    if ( !isValidData(this->year, this->month, this->day) )
        fixDate(this->year, this->month, this->day, true);
    return old;
}

CDate CDate::operator--()
{
    this->day--;
    if (!isValidData(this->year, this->month, this->day))
        fixDate(this->year, this->month, this->day, false);
    return *this;
}

CDate CDate::operator--(int)
{
    auto old = *this;
    this->day--;
    if (!isValidData(this->year, this->month, this->day))
        fixDate(this->year, this->month, this->day, false);
    return old;
}



#ifndef __PROGTEST__
int main ( void )
{
    ostringstream oss;
    istringstream iss;
    CDate a ( 2000, 1, 2 );
    CDate b ( 2010, 2, 3 );
    CDate c ( 2004, 2, 10 );
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2000-01-02" );
    oss . str ("");
    oss << b;
    assert ( oss . str () == "2010-02-03" );
    oss . str ("");
    oss << c;
    assert ( oss . str () == "2004-02-10" );
    a = a + 1500;
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2004-02-10" );
    b = b - 2000;
    oss . str ("");
    oss << b;
    assert ( oss . str () == "2004-08-13" );
    assert ( b - a == 185 );
    assert ( ( b == a ) == false );
    assert ( ( b != a ) == true );
    assert ( ( b <= a ) == false );
    assert ( ( b < a ) == false );
    assert ( ( b >= a ) == true );
    assert ( ( b > a ) == true );
    assert ( ( c == a ) == true );
    assert ( ( c != a ) == false );
    assert ( ( c <= a ) == true );
    assert ( ( c < a ) == false );
    assert ( ( c >= a ) == true );
    assert ( ( c > a ) == false );
    a = ++c;
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-11 2004-02-11" );
    a = --c;
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-10 2004-02-10" );
    a = c++;
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-10 2004-02-11" );
    a = c--;
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-11 2004-02-10" );
    iss . clear ();
    iss . str ( "2015-09-03" );
    assert ( ( iss >> a ) );
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2015-09-03" );
    a = a + 70;
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2015-11-12" );

    CDate d ( 2000, 1, 1 );
    try
    {
        CDate e ( 2000, 32, 1 );
        assert ( "No exception thrown!" == nullptr );
    }
    catch ( ... )
    {
    }
    iss . clear ();
    iss . str ( "2000-12-33" );
    assert ( ! ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-01-01" );
    iss . clear ();
    iss . str ( "2000-11-31" );
    assert ( ! ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-01-01" );
    iss . clear ();
    iss . str ( "2000-02-29" );
    assert ( ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-02-29" );
    iss . clear ();
    iss . str ( "2001-02-29" );
    assert ( ! ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-02-29" );

    //-----------------------------------------------------------------------------
    // bonus test examples
    //-----------------------------------------------------------------------------
    /*
    CDate f ( 2000, 5, 12 );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2000-05-12" );
    oss . str ("");
    oss << date_format ( "%Y/%m/%d" ) << f;
    assert ( oss . str () == "2000/05/12" );
    oss . str ("");
    oss << date_format ( "%d.%m.%Y" ) << f;
    assert ( oss . str () == "12.05.2000" );
    oss . str ("");
    oss << date_format ( "%m/%d/%Y" ) << f;
    assert ( oss . str () == "05/12/2000" );
    oss . str ("");
    oss << date_format ( "%Y%m%d" ) << f;
    assert ( oss . str () == "20000512" );
    oss . str ("");
    oss << date_format ( "hello kitty" ) << f;
    assert ( oss . str () == "hello kitty" );
    oss . str ("");
    oss << date_format ( "%d%d%d%d%d%d%m%m%m%Y%Y%Y%%%%%%%%%%" ) << f;
    assert ( oss . str () == "121212121212050505200020002000%%%%%" );
    oss . str ("");
    oss << date_format ( "%Y-%m-%d" ) << f;
    assert ( oss . str () == "2000-05-12" );
    iss . clear ();
    iss . str ( "2001-01-1" );
    assert ( ! ( iss >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2000-05-12" );
    iss . clear ();
    iss . str ( "2001-1-01" );
    assert ( ! ( iss >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2000-05-12" );
    iss . clear ();
    iss . str ( "2001-001-01" );
    assert ( ! ( iss >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2000-05-12" );
    iss . clear ();
    iss . str ( "2001-01-02" );
    assert ( ( iss >> date_format ( "%Y-%m-%d" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2001-01-02" );
    iss . clear ();
    iss . str ( "05.06.2003" );
    assert ( ( iss >> date_format ( "%d.%m.%Y" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2003-06-05" );
    iss . clear ();
    iss . str ( "07/08/2004" );
    assert ( ( iss >> date_format ( "%m/%d/%Y" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2004-07-08" );
    iss . clear ();
    iss . str ( "2002*03*04" );
    assert ( ( iss >> date_format ( "%Y*%m*%d" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2002-03-04" );
    iss . clear ();
    iss . str ( "C++09format10PA22006rulez" );
    assert ( ( iss >> date_format ( "C++%mformat%dPA2%Yrulez" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2006-09-10" );
    iss . clear ();
    iss . str ( "%12%13%2010%" );
    assert ( ( iss >> date_format ( "%%%m%%%d%%%Y%%" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2010-12-13" );

    CDate g ( 2000, 6, 8 );
    iss . clear ();
    iss . str ( "2001-11-33" );
    assert ( ! ( iss >> date_format ( "%Y-%m-%d" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "29.02.2003" );
    assert ( ! ( iss >> date_format ( "%d.%m.%Y" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "14/02/2004" );
    assert ( ! ( iss >> date_format ( "%m/%d/%Y" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "2002-03" );
    assert ( ! ( iss >> date_format ( "%Y-%m" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "hello kitty" );
    assert ( ! ( iss >> date_format ( "hello kitty" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "2005-07-12-07" );
    assert ( ! ( iss >> date_format ( "%Y-%m-%d-%m" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "20000101" );
    assert ( ( iss >> date_format ( "%Y%m%d" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-01-01" );
    */
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
