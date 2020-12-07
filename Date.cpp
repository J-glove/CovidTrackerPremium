//
// Created by Jason Glover on 11/29/20.
//

#include <iostream>
#include "Date.h"
#include <exception>
using namespace std;

//Maps used for evaluating in functions
const map<string, int> _months = {
        {"Jan", 1},
        {"Feb", 2},
        {"Mar", 3},
        {"Apr", 4},
        {"May", 5},
        {"Jun", 6},
        {"Jul", 7},
        {"Aug", 8},
        {"Sep", 9},
        {"Oct", 10},
        {"Nov", 11},
        {"Dec", 12},


};

const map<int, int> maxDays = {
        {1,31},
        {2, 28},
        {3, 31},
        {4, 30},
        {5, 31},
        {6, 30},
        {7, 31},
        {8, 31},
        {9, 30},
        {10, 31},
        {11, 30},
        {12,31},
        {13, 29},
};

Date::Date(string _date, int _cases, int _deaths) {
    if (_date == "-1"){
        return;
    } else {
        this->cases = _cases;
        this->deaths = _deaths;
        //In the case that the set is making new dates

        //Find month
        int index = _date.find(' ');
        string theMonth = _date.substr(0, index);
        this->Month = getMonth(theMonth);
        _date = _date.substr(index + 1);

        //Find Day
        index = _date.find(' ');
        try {
            this->Day = stoi(_date.substr(0, index));
        } catch (exception& e){
            //ensures the program does not terminate if conversion is not viable
            throw 16;
        }
        _date = _date.substr(index);

        //Find Year
        try {
            this->Year = stoi(_date);
        } catch (exception& e){
            //ensures the program does not terminate if conversion is not viable
            throw 16;
        }
        if (this->Year % 4 == 0) {
            this->isLeapYear = true;
        } else this->isLeapYear = false;

        /*===Exceptions for verification===*/
        //exceptions are thrown since the user inputs dates at some point, need to check if these dates are properly formatted

        //If the day is below 1
        if (this->Day < 1) {
            throw 0;
        }
        //If the Month is not formatted properly
        if (this->Month == 0) {
            throw 1;
        }
        //If user puts in a date too high for February on a leap year
        if (this->Month == 2 && this->isLeapYear && this->Day > 29) {
            throw 2;
        }
        //If the user puts in date too high for February on non leap year
        if (this->Month == 2 && !this->isLeapYear && this->Day > 28) {
            throw 3;
        }
        //Months with 31 Days
        if (this->Month == 1 && this->Day > 31) {
            throw 4;
        }
        if (this->Month == 3 && this->Day > 31) {
            throw 5;
        }
        if (this->Month == 5 && this->Day > 31) {
            throw 6;
        }
        if (this->Month == 7 && this->Day > 31) {
            throw 7;
        }
        if (this->Month == 8 && this->Day > 31) {
            throw 8;
        }
        if (this->Month == 10 && this->Day > 31) {
            throw 9;
        }
        if (this->Month == 12 && this->Day > 31) {
            throw 10;
        }

        //Months with 30 days
        if (this->Month == 4 && this->Day > 30) {
            throw 11;
        }
        if (this->Month == 6 && this->Day > 30) {
            throw 12;
        }
        if (this->Month == 9 && this->Day > 30) {
            throw 13;
        }
        if (this->Month == 11 && this->Day > 30) {
            throw 14;
        }


    }
}

int Date::getMonth(string _month) {
    return _months.find(_month)->second;
    //returns the corresponding numerical value for the passed in month
}

string Date::getDate() const {
    string dateOut;
    for (auto member: _months){
        //search the _months map by value to get the respective the letter month
        if (member.second == this->Month){
            dateOut += member.first + " ";
            break;
        }
    }
    dateOut += to_string(this->Day) + ", " + to_string(this->Year);
    return dateOut;
    //returns a date in the same format as the dataset uses
}

/*=== Overloaded operators ===*/
bool Date::operator<(const Date d) const {
    //cout << d.Month << endl;
    //cout << d.Day << endl;
    if (this->Year < d.Year){
        return true;
    }
    if ((this->Year == d.Year) && (this->Month < d.Month)){
        return true;
    }
    if ((this->Month == d.Month) && (this->Day < d.Day)){
        //in the case that two dates are in the same month
        return true;
    }
    return false;
}

bool Date::operator>(const Date d) const {
    if (this->Year > d.Year){
        return true;
    } else if ((this->Year == d.Year) && this->Month > d.Month){
        return true;
    } else if ((this->Month == d.Month) && this->Day > d.Day){
        return true;
    }
    return false;
}

bool Date::operator==(const Date d) const{
    if ((this->Year == d.Year) && (this->Month == d.Month) && (this->Day == d.Day)){
        return true;
    }
    return false;
}
Date& Date::operator=(const Date d) {
    this->Year = d.Year;
    this->Day = d.Day;
    this->Month = d.Month;
    this->isLeapYear = d.isLeapYear;
    this->cases = d.cases;
    this->deaths = d.deaths;
    return *this;
}

Date &Date::operator+(const int daysToAdd) {
    int maxD;
    if (this->Month == 2 && this->isLeapYear){
        maxD = maxDays.find(13)->second;
    } else maxD = maxDays.find(this->Month)->second;

    if ((this->Day + daysToAdd) > maxD){
        //if the days being added puts the date into a new month
        if(this->Month == 12){
            //the case in which the current month is December
            this->Month = 1;
            this->Year++;
            if (this->Year % 4 == 0){
                this->isLeapYear = true;
            } else this->isLeapYear = false;

        } else {
            //otherwise, just update the month
            this->Month ++;
            this->Day = (this->Day + daysToAdd) - maxD;
            //above equation returns the proper number of days in the new month
        }
    } else this->Day += daysToAdd;
    //otherwise just add the days
    return *this;
}

Date &Date::operator-(const int daysToSub) {
    int maxD;
    if (this->Day - daysToSub < 1) {
        //the case in which taking days away goes into the previous month
        if (this->Month == 1) {
            //in the case that going to the previous month results in changing years
            this->Month = 12;
            this->Year--;
            if (this->Year % 4 == 0) {
                this->isLeapYear = true;
            } else this->isLeapYear = false;

        } else {
            //Otherwise just update the month
            this->Month--;
            maxD = maxDays.find(this->Month)->second;
            this->Day = (this->Day - daysToSub) + maxD;
            //Above equation finds the correct date in the previous month
        }
    } else this->Day -= daysToSub;
    //Otherwise just subtract the requested number of days
    return *this;

}
