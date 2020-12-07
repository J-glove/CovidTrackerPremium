//
// Created by Jason Glover on 12/3/20.
//

#ifndef DSPROJ3_STATE_H
#define DSPROJ3_STATE_H

#include <set>
#include "County.h"
class State {
public:
    string name;
    set<County> counties;


    State(string _name = " ");
    void insertCounty(County c);
    bool operator==(const State& s) const;
    bool operator<(const State& s) const;
};

#endif //DSPROJ3_STATE_H
