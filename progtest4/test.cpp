#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>
using namespace std;
#endif /* __PROGTEST__ */

using namespace std;

class CData
{
  public:
        CData ();
        CData(const CData &other);
        ~CData();
        void resize();
        uint8_t * getData();
        uint32_t getCurrentPos() const;
        bool setCurrentPos(uint32_t offset);
        uint32_t getSize() const;
        void setSize();
        uint32_t getElemsCount() const;
        void setElemsCount(uint32_t n_cnt);
        uint32_t read(uint8_t *dst, uint32_t bytes);
        uint32_t write(const uint8_t * src,  uint32_t bytes);
        void print()
{

  for (uint32_t i = 0; i < m_El_Cnt;++i)
    {

      cout << (int)m_Data[i] << "---";
      
      
    }

  cout << endl;
  
}
        
  private:
        uint8_t * m_Data = nullptr;
        uint32_t m_Size = 0;
        uint32_t m_El_Cnt = 0;
        uint32_t m_Pos = 0;
};

CData::CData()
{
    m_Data = new uint8_t[50];
    m_Size = 50;
    m_Pos = 0;
    m_El_Cnt = 0;
}

CData::~CData()
{
    delete[] m_Data;
}

void CData::resize()
{
    uint32_t tmp_Size = m_Size;
    m_Size = (m_Size < 200) ? m_Size + 100 : (int)(m_Size * 1.5);
    uint8_t * tmp = new uint8_t[m_Size];
    memcpy(tmp, m_Data, tmp_Size*sizeof(uint8_t));
    delete[] m_Data;
    m_Data = tmp;
}

uint32_t CData::getElemsCount() const
{

  return m_El_Cnt;
  
}

void CData::setElemsCount(uint32_t n_cnt)
{

  m_El_Cnt = n_cnt;
  
}
uint32_t CData::getCurrentPos() const
{

  return m_Pos;
  
}

bool CData::setCurrentPos(uint32_t offset)
{

  if (offset < 0 || offset > m_El_Cnt)
        return false;

  m_Pos = offset;

  return true;
  
}

uint32_t CData::read(uint8_t *dst, uint32_t bytes)
{

   uint32_t read = 0;
  
    for ( ; read < bytes && m_Pos < m_El_Cnt; ++m_Pos, ++read)
    {
        dst[read] = m_Data[m_Pos];
 
    }
  
    return read;
  
}
uint32_t CData::write(const uint8_t * src,  uint32_t bytes)
{

  while (m_Size < m_Pos + bytes)
        resize();
  
  uint32_t endPos = bytes + m_Pos;
  uint32_t written = 0;

  for ( ; m_Pos < endPos; ++m_Pos, ++written)
    {
        if( m_El_Cnt <= m_Pos) ++m_El_Cnt;
        m_Data[m_Pos] = (int)src[written];
    }


    // cout << "Position " << m_Pos<< ", count " << m_El_Cnt << endl;

  return written;
  
}



CData::CData(const CData &other)
{
    m_Data = new uint8_t[50];
    m_Size = 50;
    write(other.m_Data, other.m_El_Cnt);

    m_Pos = other.m_Pos;
}

class CCommit
{
  public:
        CCommit();
        CCommit(const CCommit &other);
        ~CCommit();
        CCommit * newVersion();
        CCommit * undo();
        CCommit * getPrev();
        void truncate();
        void setPrev(CCommit * n_prev);
        bool seek(uint32_t offset);
        uint32_t getSize() const;
        void cleanVersions();
        uint32_t read(uint8_t *dst, uint32_t bytes);
        uint32_t write(const uint8_t * src,  uint32_t bytes);
        void print()
{

  data->print();
  
}
  private:
        CData * data = nullptr;
        CCommit * prev = nullptr;
};
   
CCommit::CCommit(): prev(nullptr) 
{

  data = new CData();
  
}

CCommit::CCommit(const CCommit &other )
{

  data = new CData(*other.data);

}


CCommit::~CCommit()
{
    delete data;
}

CCommit * CCommit::newVersion()
{

  return new CCommit(*this);
  
}

bool CCommit::seek(uint32_t offset)
{

  return data->setCurrentPos(offset);
  
}
uint32_t CCommit::getSize() const
{

  return data->getElemsCount();
  
}
uint32_t CCommit::read(uint8_t *dst, uint32_t bytes)
{

  return data->read(dst, bytes);
  
}
uint32_t CCommit::write(const uint8_t * src,  uint32_t bytes)
{

  return data->write(src, bytes);
  
}
CCommit * CCommit::getPrev()
{

  return prev;
  
}
void CCommit::setPrev(CCommit * n_prev)
{

    prev = n_prev;
  
}

void CCommit::truncate()
{

 data->setElemsCount(data->getCurrentPos());
  // cout << data->getElemsCount();
  
}

class CFile
{
  public:
                             CFile                         ( void );
                             CFile                         (const CFile &other);
                             ~CFile();
                             CFile & operator = (const CFile & other);
    void                     deleteCommits();
                             // copy cons, dtor, op=
    bool                     seek                          ( uint32_t          offset );
    uint32_t                 read                          ( uint8_t         * dst,
                                                             uint32_t          bytes );
    uint32_t                 write                         ( const uint8_t   * src,
                                                             uint32_t          bytes );
    void                     truncate                      ( void );
    uint32_t                 fileSize                      ( void ) const;
    void                     addVersion                    ( void );
    bool                     undoVersion                   ( void );
    bool isNew() const;
  private:
    // todo
  
    CCommit * lastCommit = nullptr;
};

CFile::CFile() : lastCommit(nullptr) {}

void CFile::deleteCommits()
{
  
   CCommit * tmp = lastCommit;
  
  while(lastCommit)
  {
    tmp = lastCommit->getPrev();

    delete lastCommit;

    lastCommit = tmp;
     
  }
  
}
CFile::CFile  (const CFile &other)
{
  if(!other.isNew())
  {

  deleteCommits();
    
  CCommit * tmp = other.lastCommit->getPrev();

  lastCommit = new CCommit(*other.lastCommit);

  CCommit * cur = lastCommit;
  
  while(tmp)
  {

    cur->setPrev(new CCommit(*tmp));

    cur = cur->getPrev();

    tmp = tmp->getPrev();
     
  }
  
    }
}

CFile & CFile::operator = (const CFile & other)
{

  if (this == &other) return *this;

  if(!other.isNew())
  {
    
  deleteCommits();
    
  // CCommit * tmp = other.lastCommit->getPrev();

  // lastCommit = new CCommit(*other.lastCommit);

  // CCommit * cur = lastCommit;
  
  // while(tmp)
  // {

  //   cur->setPrev(new CCommit(*tmp));

  //   cur = cur->getPrev();

  //   tmp = tmp->getPrev();
     
  // }
   // cout << " = "<<  (cur == nullptr) << endl;
  }
  return *this;
  
}

CFile::~CFile()
{

  deleteCommits();

}

bool CFile::isNew() const
{

  return !lastCommit;
  
}

uint32_t CFile::read(uint8_t *dst, uint32_t bytes)
{
    if (isNew() || bytes <= 0 || !dst)
    {
        return 0;
    }
  
    return lastCommit->read(dst, bytes);
}

uint32_t CFile::write(const uint8_t * src,  uint32_t bytes)
{

  if ( !src || bytes <= 0)
  {
    return 0;
  }
  if(isNew())
  {
    lastCommit = new CCommit();
    
  }
  
  return lastCommit->write(src, bytes);
  
}

 uint32_t CFile::fileSize ( void ) const
{

  return lastCommit->getSize();
  
}

bool CFile::seek ( uint32_t offset )
{

  if( isNew() )
  {

    lastCommit = new CCommit();
    
  }

  return lastCommit->seek( offset );
  
}

void CFile::addVersion ( void )
{

  CCommit * tmp = lastCommit->newVersion();

  tmp->setPrev(lastCommit);

  lastCommit = tmp;
  // cout << "Prev:\n";
  // lastCommit->getPrev()->print();
  // cout << "Last:\n";
  // lastCommit->print();
}

void CFile::truncate()
{
    if (isNew() )
        return;

    lastCommit->truncate();
}

bool CFile::undoVersion ( void )
{

  if(isNew()) return false;
  if(!lastCommit->getPrev()) return false;
  
  CCommit * tmp = lastCommit;

  lastCommit = lastCommit->getPrev();

  delete tmp;

  return true;
  
}

#ifndef __PROGTEST__
bool               writeTest                               ( CFile           & x,
                                                             const initializer_list<uint8_t> & data,
                                                             uint32_t          wrLen )
{
  return x . write ( data . begin (), data . size () ) == wrLen;
}

bool               readTest                                ( CFile           & x,
                                                             const initializer_list<uint8_t> & data,
                                                             uint32_t          rdLen )
{
  uint8_t  tmp[100];
  uint32_t idx = 0;

  if ( x . read ( tmp, rdLen ) != data . size ())
    return false;
  for ( auto v : data )
    if ( tmp[idx++] != v )
      return false;
  return true;
}

int main ( void )
{

  
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */