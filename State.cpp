//
// Created by Jason Glover on 12/3/20.
//

#include "State.h"

State::State(string _name) {
    this->name = _name;
}


void State::insertCounty(County c) {
    this->counties.emplace(c);
}

bool State::operator==(const State& s) const {
    if (this->name == s.name){
        return true;
    }
    return false;
}

bool State::operator<(const State& s) const {
    if (this->name < s.name){
        return true;
    }
    return true;
}
