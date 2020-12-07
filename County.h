//
// Created by Jason Glover on 11/28/20.
//
#include <set>
#include <map>
#include <string>
#include "Date.h"
using namespace std;
#ifndef DSPROJ3_COUNTY_H
#define DSPROJ3_COUNTY_H


class County {
public:
    //pair<string, string> name;
    string county;
    string state;
    map<Date, pair<int, int> > entriesMap;
    set<Date> entriesSet;

    /*=========Constructors==========*/
    County(string _county, string _state);
    //^^A constructor like this would mean having to parse the entries in the main function, probably not a big deal tho right??

    void insertEntry(Date _date, int cases, int deaths);
    //^^This function builds the "entries" map for the county instance.

    /*=========Helper Functions=======*/

     bool operator==(const County& c) const;
     bool operator<(const County& c) const;
     //== operator added for unordered set evaluations, info on this found here: https://www.geeksforgeeks.org/how-to-create-an-unordered_set-of-user-defined-class-or-struct-in-c/
     //Accessed on Dec 3, 2020

private:
};


#endif //DSPROJ3_COUNTY_H
