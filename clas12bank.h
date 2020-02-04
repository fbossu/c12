#pragma once 
#include "bank.h"
#include "reader.h"

#include <unordered_map>


class clas12bank : public hipo::bank {
  public:
    void init( const char *, hipo::reader &);
   
    int getInt( std::string b, int i ) { return bank::getInt( order[b], i); }
    int getLong( std::string b, int i ) { return bank::getLong( order[b], i); }
    float getFloat( std::string b, int i) { return bank::getFloat( order[b], i); };
    double getDouble( std::string b, int i) { return bank::getDouble( order[b], i); };

    void addBranch( std::string );
  private:
    std::unordered_map<std::string, int> order;
};


void clas12bank::init( const char *bname, hipo::reader &r ){
  initBranches( bname, r );
}

void clas12bank::addBranch( std::string b ){
  order[b] = getEntryOrder(b.c_str());
}
