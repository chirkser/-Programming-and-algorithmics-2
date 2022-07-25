#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */


/** class Company
 * serves to store the firm's data
 */
class CCompany
{
public:
    /**
     * Default constructor
     */
    CCompany () = default;
    /**
     * Class constructor
     * Sets up company data
     * @param[in] name Company name
     * @param[in] addr Company address
     * @param[in] id Company ID
     */
    CCompany(const string & name, const string & addr, const string & id);
    /**
     * Default destructor
     */
    ~CCompany() = default;
    /**
     * Returns the company name
     */
    string  getName() const;
    /**
     * Returns the company address
     */
    string  getAddress() const;
    /**
     * Returns the company ID
     */
    string  getId() const;
    /**
     * Sets the company name
     */
    void setName(const string & name);
    /**
     * Sets the company address
     */
    void setAddr(const string & addr);
    /**
     * Sets the company ID
     */
    void setID(const string & id);
    /**
     * Adds amount to the company's income and saves the invoice
     * @param amount invoice amount
     */
    void addInvoice(unsigned int amount);
    /**
     * returns the company's income
     * @return company's income
     */
    unsigned int getAudit() const;
    /**
     * Displaying a list of invoices
     */
    void printInvoice();
private:
    string name;
    string addr;
    string id;
    unsigned int sum = 0;
    // container for storing invoice amounts
    vector <unsigned int> profit;
};

// ------------------------------Start implementation---------------------------------------------- //

CCompany::CCompany(const string & name, const string & addr, const string & id)
{
    this->name = name;
    this->addr = addr;
    this->id = id;
    this->sum = 0;
}

string CCompany::getName() const
{
    return name;
}

string CCompany::getAddress() const
{
    return addr;
}

string CCompany::getId() const
{
    return id;
}

void CCompany::setName(const string &name)
{
    this->name = name;
}

void CCompany::setAddr(const string &addr)
{
    this->addr = addr;
}

void CCompany::setID(const string &id)
{
    this->id = id;
}

void CCompany::addInvoice(unsigned int amount)
{
    sum+=amount;
    profit.push_back(amount);
}

unsigned int CCompany::getAudit() const
{
    return sum;
}

void CCompany::printInvoice()
{
    int cnt = 0;
    for (auto & i: profit)
    {
        cout << "Invoice[" << cnt << "] = " << i << " | ";
        cnt++;
    }
    cout << endl;
}

// ---------------------------------End implementation---------------------------------------------- //

// ----------------------------------Helpful functions---------------------------------------------- //

/**
 * Converts a string to lowercase
 * @param any string
 * @return lower case string
 */
string toLower (const string & str)
{
    string temp = str;
    transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
    return temp;
}

/**
 * Function to compare companies by their name and address
 */
bool cmpNameAddress (const CCompany & left, const CCompany & right)
{
    if ( !strcmp(toLower(left.getName()).c_str(), toLower(right.getName()).c_str()) )
    {
        return strcmp(toLower(left.getAddress()).c_str() , toLower(right.getAddress()).c_str() )< 0;
    }
    return strcmp(toLower(left.getName()).c_str(), toLower(right.getName()).c_str()) < 0;
}

/**
 * Function to compare companies by their ID
 */
bool cmpId (const CCompany & left, const CCompany & right)
{
    return strcmp(left.getId().c_str(), right.getId().c_str()) < 0;
}

// ------------------------------------------------------------------------------------------------- //


/** class CVATRegister
 * implement a database of VAT control reports
 */
class CVATRegister
{
public:
    /**
     * Default constructor
     */
    CVATRegister   ( void ) = default;
    /**
     * Default destructor
     */
    ~CVATRegister  ( void ) = default;
    /**
     * Adds a new company record to the database
     * @param name Company name
     * @param addr Company address
     * @param taxID Company ID
     * @return true If added successfully
     * @return false if such an entry already exists in the database
     */
    bool          newCompany     ( const string    & name,
                                   const string    & addr,
                                   const string    & taxID );

    /**
     * Removes a company from the database by name and address
     * @param name Company name
     * @param addr Company address
     * @return true If the company is successful removed
     * @return false If such a company did not exist in the records
     */
    bool          cancelCompany  ( const string    & name,
                                   const string    & addr );
    /**
     * Removes a company from the database by ID
     * @param taxID Company ID
     * @return true If the company is successful removed
     * @return false If such a company did not exist in the records
     */
    bool          cancelCompany  ( const string    & taxID );

    /**
     * Adds the profit of the company, the search occurs by company ID
     * @param taxID Company ID
     * @param amount Company profit
     * @return true If the company profit is successful added
     * @return false If such a company did not exist in the records
     */
    bool          invoice        ( const string    & taxID,
                                   unsigned int      amount );
    /**
     * Adds the profit of the company, the search occurs by the name of the company and its address
     * @param name Company name
     * @param addr Company address
     * @return true If the company profit is successful added
     * @return false If such a company did not exist in the records
     */
    bool          invoice        ( const string    & name,
                                   const string    & addr,
                                   unsigned int      amount );

    /**
     * Searches for a company by name and address, and then saves the value of its earnings in the out parameter
     * @param name Company name
     * @param addr Company address
     * @param sumIncome Variable to save the search result
     * @return true If company successfully found
     * @return false If no such company was found
     */
    bool          audit          ( const string    & name,
                                   const string    & addr,
                                   unsigned int    & sumIncome ) const;
    /**
     * Searches for a company by ID, and then saves the value of its earnings in the out parameter
     * @param taxID Company ID
     * @param sumIncome Variable to save the search result
     * @return true If company successfully found
     * @return false If no such company was found
     */
    bool          audit          ( const string    & taxID,
                                   unsigned int    & sumIncome ) const;
    /**
     * Saves the name and address of the first company in database to the out parameters
     * @param name Company name
     * @param addr Company address
     * @return true If a company exists
     * @return false If the database is empty
     */
    bool          firstCompany   ( string          & name,
                                   string          & addr ) const;
    /**
     * Saves the name and address of the company to the out parameters
     * @param name Company name
     * @param addr Company address
     * @return true If a company exists
     * @return false If the database is empty
     */
    bool          nextCompany    ( string          & name,
                                   string          & addr ) const;
    /**
     * Searches for the median of all invoices in the database
     * @return Median value If the values are counted successfully
     * @return 0 If database empty
     * @return -1 In indefinite cases
     */
    unsigned int  medianInvoice  ( void ) const;
    /**
     * Writes to the console two vectors sorted by (name and address) and by (id)
     */
    void printVector();
    /**
     * Writes to the console values of all invoices in the database
     */
    void printInvoices();
private:
    // Sorted vector by name and address
    vector <CCompany> idxNameAddr;
    // Sorted vector by TaxId
    vector <CCompany> idxTaxId;
    // Vector for invoices
    vector <unsigned int> invoices;
    vector <CCompany> ::const_iterator iter;
    CCompany tmp;
};

// ------------------------------Start implementation---------------------------------------------- //

bool CVATRegister::newCompany(const string &name, const string &addr, const string &taxID)
{
    tmp = CCompany(name, addr, taxID);
    // finding the right position
    auto itNameAddress = lower_bound(idxNameAddr.begin(), idxNameAddr.end(), tmp, cmpNameAddress);
    auto itId = lower_bound(idxTaxId.begin(), idxTaxId.end(), tmp, cmpId);
    // check duplicate name && address
    if (!(itNameAddress == idxNameAddr.end()) &&
        toLower((*itNameAddress).getName()) == toLower(tmp.getName()) &&
        toLower((*itNameAddress).getAddress()) == toLower(tmp.getAddress())) {
        return false;
    }
    // check duplicate TaxId
    if (!(itId == idxTaxId.end()) &&
        (*itId).getName() == tmp.getName() &&
        (*itId).getAddress() == tmp.getAddress()) {
        return false;
    }

    idxNameAddr.insert(itNameAddress, tmp);
    idxTaxId.insert(itId, tmp);
    return true;
}

bool CVATRegister::cancelCompany(const string &name, const string &addr)
{
    tmp = CCompany(name, addr, "");
    // finding the company by name && address
    auto itNameAddress = lower_bound(idxNameAddr.begin(), idxNameAddr.end(), tmp, cmpNameAddress);
    // existence check
    if (itNameAddress == idxNameAddr.end() ||
        toLower((*itNameAddress).getName()) != toLower(tmp.getName()) ||
        toLower((*itNameAddress).getAddress()) != toLower(tmp.getAddress())) {
        return false;
    }

    tmp.setID((*itNameAddress).getId());
    // finding the company by ID
    auto itId = lower_bound(idxTaxId.begin(), idxTaxId.end(), tmp, cmpId);
    // existence check
    if (itId == idxTaxId.end() ||
        (*itId).getId() != tmp.getId()) {
        return false;
    }

    idxNameAddr.erase(itNameAddress);
    idxTaxId.erase(itId);
    return true;
}

bool CVATRegister::cancelCompany(const string &taxID)
{
    tmp = CCompany(" ", " ", taxID);
    // finding the company by ID
    auto itId = lower_bound(idxTaxId.begin(), idxTaxId.end(), tmp, cmpId);
    // existence check
    if (itId == idxTaxId.end() ||
        (*itId).getId() != tmp.getId()) {
        return false;
    }

    tmp.setName((*itId).getName());
    tmp.setAddr((*itId).getAddress());
    // finding the company by name && address
    auto itNameAddress = lower_bound(idxNameAddr.begin(), idxNameAddr.end(), tmp, cmpNameAddress);
    // existence check
    if (itNameAddress == idxNameAddr.end() ||
        toLower((*itNameAddress).getName()) != toLower(tmp.getName()) ||
        toLower((*itNameAddress).getAddress()) != toLower(tmp.getAddress())) {
        return false;
    }
    idxNameAddr.erase(itNameAddress);
    idxTaxId.erase(itId);
    return true;
}

bool CVATRegister::invoice(const string &taxID, unsigned int amount)
{
    tmp = CCompany(" ", " ", taxID);
    // finding the company by taxID
    auto itId = lower_bound(idxTaxId.begin(), idxTaxId.end(), tmp, cmpId);
    // existence check
    if (itId == idxTaxId.end() ||
        (*itId).getId() != tmp.getId()) {
        return false;
    }
    tmp.setName((*itId).getName());
    tmp.setAddr((*itId).getAddress());

    // finding the company by name && address
    auto itNameAddress = lower_bound(idxNameAddr.begin(), idxNameAddr.end(), tmp, cmpNameAddress);
    // existence check
    if (itNameAddress == idxNameAddr.end() ||
        toLower((*itNameAddress).getName()) != toLower(tmp.getName()) ||
        toLower((*itNameAddress).getAddress()) != toLower(tmp.getAddress())) {
        return false;
    }
    (*itId).addInvoice(amount);
    (*itNameAddress).addInvoice(amount);
    auto itInvoices = lower_bound(invoices.begin(), invoices.end(), amount);
    invoices.insert(itInvoices, amount);
    return true;
}

bool CVATRegister::invoice(const string &name, const string &addr, unsigned int amount)
{
    tmp = CCompany(name, addr, " ");
    // finding the company by name && address
    auto itNameAddress = lower_bound(idxNameAddr.begin(), idxNameAddr.end(), tmp, cmpNameAddress);
    // existence check
    if (itNameAddress == idxNameAddr.end() ||
        toLower((*itNameAddress).getName()) != toLower(tmp.getName()) ||
        toLower((*itNameAddress).getAddress()) != toLower(tmp.getAddress())) {
        return false;
    }
    // finding the company by ID
    tmp.setID((*itNameAddress).getId());
    auto itId = lower_bound(idxTaxId.begin(), idxTaxId.end(), tmp, cmpId);
    // existence check
    if (itId == idxTaxId.end() ||
        (*itId).getId() != tmp.getId()) {
        return false;
    }

    (*itNameAddress).addInvoice(amount);
    (*itId).addInvoice(amount);
    auto itInvoices = lower_bound(invoices.begin(), invoices.end(), amount);
    invoices.insert(itInvoices, amount);
    return true;
}

bool CVATRegister::audit(const string &name, const string &addr, unsigned int &sumIncome) const
{
    CCompany temp = CCompany(name, addr, " ");
    // finding the company by name && address
    auto itNameAddress = lower_bound(idxNameAddr.begin(), idxNameAddr.end(), temp, cmpNameAddress);
    // existence check
    if (itNameAddress == idxNameAddr.end() ||
        toLower((*itNameAddress).getName()) != toLower(temp.getName()) ||
        toLower((*itNameAddress).getAddress()) != toLower(temp.getAddress())) {
        return false;
    }
    sumIncome = (*itNameAddress).getAudit();
    return true;
}

bool CVATRegister::audit(const string &taxID, unsigned int &sumIncome) const
{
    CCompany temp = CCompany(" ", " ", taxID);
    // finding the company by ID
    auto itId = lower_bound(idxTaxId.begin(), idxTaxId.end(), temp, cmpId);
    // existence check
    if (itId == idxTaxId.end() ||
        (*itId).getId() != temp.getId()) {
        return false;
    }
    sumIncome = (itId)->getAudit();
    return true;
}

bool CVATRegister::firstCompany(string &name, string &addr) const
{
    if (idxNameAddr.empty()) return false;
    name = (idxNameAddr.begin())->getName();
    addr = (idxNameAddr.begin())->getAddress();
    return true;
}

bool CVATRegister::nextCompany(string &name, string &addr) const
{
    CCompany temp = CCompany(name, addr, " ");
    // finding the company by name && address
    auto itName = lower_bound(idxNameAddr.begin(), idxNameAddr.end(), temp, cmpNameAddress);
    if(itName == idxNameAddr.end() || (++itName) == idxNameAddr.end()) return false;
    name = (*itName).getName();
    addr = (*itName).getAddress();
    return true;
}

unsigned int CVATRegister::medianInvoice() const
{
    if (invoices.empty())
    {
        return 0;
    }
    // median conditions for an even number of invoices
    if (invoices.size() % 2 == 0)
    {
        return (invoices[invoices.size() / 2 - 1] > invoices[invoices.size() / 2]) ? invoices[invoices.size() / 2 - 1] : invoices[invoices.size() / 2];
    }
    // median conditions for an odd number of invoices
    else if (invoices.size() % 2 == 1)
    {
        return invoices[invoices.size() / 2];
    } else
    {
        return -1;
    }
}

void CVATRegister::printVector()
{
    for (auto & i: idxNameAddr)
    {
        cout << "Name: " << i.getName() << endl << "Address: " << i.getAddress() << endl << "ID: " << i.getId() << endl;
        i.printInvoice();
        cout << "--------------------------------------" << endl;
    }
    cout << endl << "............Start: By ID................. " << endl;
    for (auto & i: idxTaxId)
    {
        cout << "Name: " << i.getName() << endl << "Address: " << i.getAddress() << endl << "ID: " << i.getId() << endl;
        i.printInvoice();
        cout << "--------------------------------------" << endl;
    }
}

void CVATRegister::printInvoices()
{
    for (auto & i: invoices)
    {
        cout << i << " ";
    }
    cout << endl;
}

// ---------------------------------End implementation---------------------------------------------- //

#ifndef __PROGTEST__
int               main           ( void )
{
    string name, addr;
    unsigned int sumIncome;

    CVATRegister b1;
    assert ( b1 . newCompany ( "ACME", "Thakurova", "666/666" ) );
    assert ( b1 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
    assert ( b1 . newCompany ( "Dummy", "Thakurova", "123456" ) );
    assert ( b1 . invoice ( "666/666", 2000 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "666/666/666", 3000 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 4000 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "aCmE", "Kolejni", 5000 ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
    assert ( b1 . audit ( "123456", sumIncome ) && sumIncome == 4000 );
    assert ( b1 . firstCompany ( name, addr ) && name == "ACME" && addr == "Kolejni" );
    assert ( b1 . nextCompany ( name, addr ) && name == "ACME" && addr == "Thakurova" );
    assert ( b1 . nextCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
    assert ( ! b1 . nextCompany ( name, addr ) );
    assert ( b1 . cancelCompany ( "ACME", "KoLeJnI" ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . cancelCompany ( "666/666" ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . invoice ( "123456", 100 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 300 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 200 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 230 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 830 ) );
    assert ( b1 . medianInvoice () == 830 );
    assert ( b1 . invoice ( "123456", 1830 ) );
    assert ( b1 . medianInvoice () == 1830 );
    assert ( b1 . invoice ( "123456", 2830 ) );
    assert ( b1 . medianInvoice () == 1830 );
    assert ( b1 . invoice ( "123456", 2830 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 3200 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . firstCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
    assert ( ! b1 . nextCompany ( name, addr ) );
    assert ( b1 . cancelCompany ( "123456" ) );
    assert ( ! b1 . firstCompany ( name, addr ) );

    CVATRegister b2;
    assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
    assert ( b2 . newCompany ( "Dummy", "Kolejni", "123456" ) );
    assert ( ! b2 . newCompany ( "AcMe", "kOlEjNi", "1234" ) );
    assert ( b2 . newCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
    assert ( b2 . medianInvoice () == 0 );
    assert ( b2 . invoice ( "ABCDEF", 1000 ) );
    assert ( b2 . medianInvoice () == 1000 );
    assert ( b2 . invoice ( "abcdef", 2000 ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( b2 . invoice ( "aCMe", "kOlEjNi", 3000 ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( ! b2 . invoice ( "1234567", 100 ) );
    assert ( ! b2 . invoice ( "ACE", "Kolejni", 100 ) );
    assert ( ! b2 . invoice ( "ACME", "Thakurova", 100 ) );
    assert ( ! b2 . audit ( "1234567", sumIncome ) );
    assert ( ! b2 . audit ( "ACE", "Kolejni", sumIncome ) );
    assert ( ! b2 . audit ( "ACME", "Thakurova", sumIncome ) );
    assert ( ! b2 . cancelCompany ( "1234567" ) );
    assert ( ! b2 . cancelCompany ( "ACE", "Kolejni" ) );
    assert ( ! b2 . cancelCompany ( "ACME", "Thakurova" ) );
    assert ( b2 . cancelCompany ( "abcdef" ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( ! b2 . cancelCompany ( "abcdef" ) );
    assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
    assert ( b2 . cancelCompany ( "ACME", "Kolejni" ) );
    assert ( ! b2 . cancelCompany ( "ACME", "Kolejni" ) );


    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */