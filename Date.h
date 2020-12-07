//
// Created by Jason Glover on 11/28/20.
//

#ifndef DSPROJ3_DATE_H
#define DSPROJ3_DATE_H

#include <map>
#include <string>
using namespace std;

/* The purpose of this class is to simply reformat the passed in date into integer values so that the evaluation within other
 * files is much nicer and easier to carry out.
 * This class should ALSO be able to return the dates in the "Mon D, YYYY" format which is used in the original file.
 * */
class Date {
public:
    int Month;
    int Day;
    int Year;
    bool isLeapYear;
    //add this for more precise error throwing in date construction

    //the variables below are for the set implementation, to allow for easier insertion/comparison
    int cases;
    int deaths;


    Date(string _date = "-1", int _cases = 0, int _deaths = 0);
    //A constructor, this parses the passed in string and creates the saved "date".

    int getMonth(string _month);
    //A helper function to translate the string variable month into an integer.
    string getDate() const;
    //A helper function to translate the date instance back to a string for printing

    bool operator<(const Date d) const;
    bool operator>(const Date d) const;
    bool operator==(const Date d) const;
    Date& operator=(const Date d);
    Date& operator+(const int daysToAdd);
    Date& operator-(const int daysToSub);
private:

};


#endif //DSPROJ3_DATE_H
