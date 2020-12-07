//
// Created by Jason Glover on 11/29/20.
//

#include "County.h"

County::County(string _county, string _state) {
    this->county = _county;
    this->state = _state;
}

void County::insertEntry(Date _date, int cases, int deaths) {
    //simple function to place the entry into the "entries" map for the county
    this->entriesMap.insert(pair<Date, pair< int, int> >(_date, pair<int, int>(cases, deaths)));
}

bool County::operator==(const County& c) const {
    if ((this->state == c.state) && (this->county == c.county)){
        return true;
    }
    return false;
}

bool County::operator<(const County& c) const {
    if (this->county < c.county){
        return true;
    }
    return false;
}
