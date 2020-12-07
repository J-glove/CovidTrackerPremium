#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include "State.h"
#include "County.h"
#include "Date.h"

#include <chrono>
using namespace std;

/*====Global Variables=====*/
map<string, County> countiesMap;
map<string, map<Date, pair<int, int> > > resultsMap;
set<pair<string, set<Date> > > resultsSet;
set<State> states;
bool case4Redir = false;
//A special condition for redirecting the user straight to case 3 from case 4


/*=====Function Declarations=======*/
void compareResults(set<Date> firstSet, set<Date> secondSet);

/*=== Search ===*/
map<Date, pair<int, int> >  searchMap(string countyName, Date from, Date to);
//searchSet(string countyName, Date from, Date to)
set<Date> searchSet(string name, Date from, Date to);

/*==== Insertion ===*/
void parseEntryMap(string line);
//takes a line and initializes a county instance
void buildSet();
//builds the nested set structure from the map structure

/*=== Save ===*/
bool saveResults(string fileName);

/*=== Helper Functions ===*/
void printFormatted(string line);
//prints a variable in the format of the menu

int printResults();
//has integer return type so the main function knows what the max index available is

void determineDateError(int e);
//This function determines what to print in the event that the user input an invalid date


int main() {
    /*======Variables=======*/
    string dataset;
    ifstream file;
    ifstream& fileRef = file;

    bool firstIt = true;
    //firstIt displays "Dataset Loaded" when the program first executes, then stays false to prevent output
    bool exit = false;
    //exit kills program when set true
    bool badOp = false;
    //badOp modifies the output of the menu in the case that an incorrect operation was chosen


    /*====Menu, main function====*/
    //Entry screen, prompts user for dataset
    cout << "-" <<"---------------------------------------------------------" << "-" << endl;
    cout << "|" <<"            Welcome to COVID Tracker Premium!            " << "|" << endl;
    cout << "|" <<"                                                         " << "|" << endl;
    cout << "|" <<"                By Glove Technologies Ltd.               " << "|" << endl;
    cout << "|" <<"                                                         " << "|" << endl;
    cout << "|" <<"                                                         " << "|" << endl;
    cout << "|" <<"                                                         " << "|" << endl;
    cout << "|" <<"                                                         " << "|" << endl;
    cout << "|" <<"            To begin, please load the dataset.           " << "|" << endl;
    cout << "|" <<"                                                         " << "|" << endl;
    cout << "|" <<"                                                         " << "|" << endl;
    cout << "|" <<"                                                         " << "|" << endl;
    cout << "-" <<"---------------------------------------------------------" << "-" << endl;

    cin>> dataset;
    fileRef.open(dataset);
    while(!fileRef.is_open()){
        cout << "-" << "---------------------------------------------------------" << "-" << endl;
        cout << "|" << "                 ***********************                 " << "|" << endl;
        cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
        cout << "|" << "                 ***********************                 " << "|" << endl;
        cout << "|" << "                                                         " << "|" << endl;
        cout << "|" << " ERROR: File not found.                                  " << "|" << endl;
        cout << "|" << " Please load a different file.                           " << "|" << endl;
        cout << "|" << "                                                         " << "|" << endl;
        cout << "-" << "---------------------------------------------------------" << "-" << endl;
        cin>> dataset;
        fileRef.open(dataset);
    }
    int count = 0;
    string rand;
    getline(fileRef, rand);
    auto start = chrono::high_resolution_clock().now();
    while (!fileRef.eof()){
        string entry;
        getline(fileRef, entry);
        parseEntryMap(entry);
        count ++;
    }
    auto end = chrono::high_resolution_clock().now();
    auto duration = chrono::duration_cast<chrono::microseconds> (end-start);
    cout<< "Time to load map: " << fixed << setprecision(2) << (((float)duration.count())/1000000.00) << " seconds." << endl;
    fileRef.close();

    start = chrono::high_resolution_clock().now();
    buildSet();
    end = chrono::high_resolution_clock().now();
    duration = chrono::duration_cast<chrono::microseconds> (end-start);
    cout<< "Time to load set: " << fixed << setprecision(2) << (((float)duration.count())/1000000.00)<< " seconds." << endl;
    //loop below is main menu
    while (!exit) {
        int operation;

        if(!case4Redir) {
            cout << "-" << "---------------------------------------------------------" << "-" << endl;
            cout << "|" << "                 ***********************                 " << "|" << endl;
            cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
            cout << "|" << "                 ***********************                 " << "|" << endl;
            cout << "|" << "                                                         " << "|" << endl;
            cout << "|" << "                                                         " << "|" << endl;
            if (firstIt) {
                cout << "|" << " Dataset Loaded.                                         " << "|" << endl;
                firstIt = false;
            } else if (badOp) {
                cout << "|" << " Invalid operation! Please enter a number from 1-4.      " << "|" << endl;
                badOp = false;
            } else cout << "|" << "                                                         " << "|" << endl;
            cout << "|" << " ***************************                             " << "|" << endl;
            cout << "|" << " Please select an operation:                             " << "|" << endl;
            cout << "|" << " 1. Conduct a Search                                     " << "|" << endl;
            cout << "|" << " 2. Compare Results                                      " << "|" << endl;
            cout << "|" << " 3. Save Results                                         " << "|" << endl;
            cout << "|" << " 4. Exit                                                 " << "|" << endl;
            cout << "|" << " ***************************                             " << "|" << endl;
            cout << "|" << "                                                         " << "|" << endl;
            cout << "|" << "                                                         " << "|" << endl;
            cout << "-" << "---------------------------------------------------------" << "-" << endl;
            string op;
            cin >> op;
            try {
                //use try/catch for operation in order to ensure operation is purely numerical. otherwise, infinite loop occurs
                operation = stoi(op);
            } catch (exception e) {
                operation = -1;
            }
        }
        /*===Case 1 (Search) Variables===*/
        string name;
        string from;
        string to;
        //input variables

        Date fromD;
        Date toD;
        //Date instances of input values

        bool countyFound = false;
        //bool firstIteration = true;
        bool errorCaught = false;
        //Error catching

        int casesFromMap;
        int deathsFromMap;
        int casesToMap;
        int deathsToMap;

        int casesFromSet;
        int deathsFromSet;
        int casesToSet;
        int deathsToSet;
        float changeCases;
        float changeDeaths;
        //Returned values for each date

        stringstream s;
        string percentOut;
        //Special variables for printing formatted percentages

        auto durationMap = duration;
        auto durationSet = duration;
        //initialized here just to satisfy the requirements for auto keyword

        map<Date, pair<int, int>> result;




        /*=== Case 2 (Compare) Variables===*/
        set<Date> res1;
        set<Date> res2;
        string firstRes;
        string secondRes;

        bool firstGood = false;
        bool secondGood = false;
        int firstResInt;
        int secondResInt;
        int index;
        int max;

        /*=== Case 3 (Save) Variables ===*/
        bool resultsSaved;
        bool validFile = false;
        string fileName;
        int delimInd;

        /*=== Case 4 (Exit) Variables ===*/
        string choice;
        bool opRead = false;

        switch (operation){

            case 1:
                //Conduct a Search

                cout << "-" << "---------------------------------------------------------" << "-" << endl;
                cout << "|" << "                 ***********************                 " << "|" << endl;
                cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                cout << "|" << "                 ***********************                 " << "|" << endl;
                cout << "|" << "                                                         " << "|" << endl;
                cout << "|" << " Search:                                                 " << "|" << endl;
                cout << "|" << " *******                                                 " << "|" << endl;
                cout << "|" << "                                                         " << "|" << endl;
                cout << "|" << "                                                         " << "|" << endl;
                cout << "|" << "                                                         " << "|" << endl;
                cout << "|" << "                                                         " << "|" << endl;
                cout << "|" << " Please enter the county and state in the form:          " << "|" << endl;
                cout << "|" << " \"county, state\"                                         " << "|" << endl;
                cout << "|" << "                                                         " << "|" << endl;
                cout << "|" << "                                                         " << "|" << endl;
                cout << "-" << "---------------------------------------------------------" << "-" << endl;
                //cin.clear();


                while (!countyFound) {

                    while (name.length() == 0) {
                        //originally, the "county not found error" would print every time
                        //info on how to fix this taken from: https://www.geeksforgeeks.org/how-to-use-getline-in-c-when-there-are-black-lines-in-input/
                        //Accessed on December 4, 2020
                        getline(cin, name);
                    }
                    if (countiesMap.find(name) != countiesMap.end()) {
                        //if the county is found
                        countyFound = true;
                    }
                    if (!countyFound) {
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << " Search:                                                 " << "|" << endl;
                        cout << "|" << " *******                                                 " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        printFormatted("ERROR: No data found for \"" + name + "\"" + ".");
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << " Please enter the county and state in the form:          " << "|" << endl;
                        cout << "|" << " \"county, state\"                                         " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        name.clear();
                    }
                }




                if (countyFound) {
                    cout << "-" << "---------------------------------------------------------" << "-" << endl;
                    cout << "|" << "                 ***********************                 " << "|" << endl;
                    cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                    cout << "|" << "                 ***********************                 " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "|" << " Search:                                                 " << "|" << endl;
                    cout << "|" << " *******                                                 " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "|" << " County found!                                           " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "|" << " Enter a time frame (Mon D, YYYY)                        " << "|" << endl;
                    cout << "|" << " From:                                                   " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        //cin >> from;
                    do {
                        //use do while loop to ensure the program reads input once before determining if a date error was caught
                        cin.clear();
                        getline(cin, from);
                        try {
                            fromD = Date(from);
                            errorCaught = false;
                        } catch (int e) {
                            //reprints the menu with the appropriate error message displayed
                            cout << "-" << "---------------------------------------------------------" << "-" << endl;
                            cout << "|" << "                 ***********************                 " << "|" << endl;
                            cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                            cout << "|" << "                 ***********************                 " << "|" << endl;
                            cout << "|" << "                                                         " << "|" << endl;
                            cout << "|" << " Search:                                                 " << "|" << endl;
                            cout << "|" << " *******                                                 " << "|" << endl;
                            cout << "|" << "                                                         " << "|" << endl;
                            printFormatted("County: " + name);
                            determineDateError(e);
                            //prints the proper error message depending on the exception thrown
                            cout << "|" << "                                                         " << "|" << endl;
                            cout << "|" << " Enter a time frame (Mon D, YYYY)                        " << "|" << endl;
                            cout << "|" << " From:                                                   " << "|" << endl;
                            cout << "|" << "                                                         " << "|" << endl;
                            cout << "|" << "                                                         " << "|" << endl;
                            cout << "|" << "                                                         " << "|" << endl;
                            cout << "-" << "---------------------------------------------------------" << "-" << endl;
                            errorCaught = true;
                        }
                    } while (errorCaught);

                    cout << "-" << "---------------------------------------------------------" << "-" << endl;
                    cout << "|" << "                 ***********************                 " << "|" << endl;
                    cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                    cout << "|" << "                 ***********************                 " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "|" << " Search:                                                 " << "|" << endl;
                    cout << "|" << " *******                                                 " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    printFormatted("County: " + name);
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "|" << " Enter a time frame (Mon D, YYYY)                        " << "|" << endl;
                    printFormatted("From: " + from);
                    cout << "|" << " To:                                                     " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "-" << "---------------------------------------------------------" << "-" << endl;

                    do {
                        //use do while loop to ensure the program reads input once before determining if a date error was caught
                        cin.clear();
                        getline(cin, to);
                        try {
                            toD = Date(to);
                            errorCaught = false;
                            if (toD < fromD){
                                //prevent to date from being earlier than the from date
                                throw 15;
                            }
                        } catch (int e) {
                            cout << "-" << "---------------------------------------------------------" << "-" << endl;
                            cout << "|" << "                 ***********************                 " << "|" << endl;
                            cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                            cout << "|" << "                 ***********************                 " << "|" << endl;
                            cout << "|" << "                                                         " << "|" << endl;
                            cout << "|" << " Search:                                                 " << "|" << endl;
                            cout << "|" << " *******                                                 " << "|" << endl;
                            cout << "|" << "                                                         " << "|" << endl;
                            printFormatted("County: " + name);
                            determineDateError(e);
                            //prints the proper error message depending on the exception thrown
                            cout << "|" << "                                                         " << "|" << endl;
                            cout << "|" << " Enter a time frame (Mon D, YYYY)                        " << "|" << endl;
                            printFormatted("From: " + from);
                            cout << "|" << " To:                                                     " << "|" << endl;
                            cout << "|" << "                                                         " << "|" << endl;
                            cout << "-" << "---------------------------------------------------------" << "-" << endl;
                            errorCaught = true;
                        }
                    } while (errorCaught);


                    cout << "-" << "---------------------------------------------------------" << "-" << endl;
                    cout << "|" << "                 ***********************                 " << "|" << endl;
                    cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                    cout << "|" << "                 ***********************                 " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "|" << " Search:                                                 " << "|" << endl;
                    cout << "|" << " *******                                                 " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    printFormatted("County: " + name);
                    printFormatted("From: " + from);
                    printFormatted("To: " + to);
                    cout << "|" << " Searching...                                            " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "-" << "---------------------------------------------------------" << "-" << endl;

                    start = chrono::high_resolution_clock().now();
                    result = searchMap(name, fromD, toD);
                        if (resultsMap.find(name) == resultsMap.end()) {
                            //this if/else statement accounts for a search of the same county
                            //if the county hasn't been searched yet, just add to the results set
                            resultsMap.emplace(name, result);
                            //add the result to the "resultsMap" map for saving later
                        } else {
                            //otherwise add the returned dates to the already existing entry
                            for (auto mem: result){
                                resultsMap.find(name)->second.emplace(mem);
                            }
                        }
                    end = chrono::high_resolution_clock().now();
                    durationMap = chrono::duration_cast<chrono::microseconds> (end-start);

                    start = chrono::high_resolution_clock().now();
                    cout << name << endl;
                    resultsSet.emplace(pair<string, set<Date>>(name, (searchSet(name, fromD, toD))));
                    end = chrono::high_resolution_clock().now();
                    durationSet = chrono::duration_cast<chrono::microseconds> (end-start);
                    //Get the numbers for the specified dates
                    for (auto mem : resultsSet){

                        if (mem.first == name){
                            //cout << "Set, county "<< mem.first <<"found" << endl;
                            if ((fromD == *mem.second.begin())) {
                                //if/else statement essentially checks for where in the date set the time frame begins
                                //this accounts for multiple searches of the same county
                                casesFromSet = mem.second.begin()->cases;
                                deathsFromSet = mem.second.begin()->deaths;

                                casesToSet = (--mem.second.end())->cases;
                                deathsToSet = (--mem.second.end())->deaths;
                            }

                        }

                    }

                    if ((fromD < resultsMap.find(name)->second.begin()->first) || (fromD == resultsMap.find(name)->second.begin()->first)){
                        casesFromMap = resultsMap.find(name)->second.begin()->second.first;
                        deathsFromMap = resultsMap.find(name)->second.begin()->second.second;
                    } else{
                        casesFromMap = resultsMap.find(name)->second.find(fromD)->second.first;
                        deathsFromMap = resultsMap.find(name)->second.find(fromD)->second.second;
                    }

                    if ((toD > (--resultsMap.find(name)->second.end())->first) || (toD == (--resultsMap.find(name)->second.end())->first)){
                        casesToMap = (--resultsMap.find(name)->second.end())->second.first;
                        deathsToMap = (--resultsMap.find(name)->second.end())->second.second;
                    } else {
                        casesToMap = resultsMap.find(name)->second.find(toD)->second.first;
                        deathsToMap = resultsMap.find(name)->second.find(toD)->second.second;
                    }


                    changeCases = (float) 100.00 * ((((float)casesToMap) - ((float)casesFromMap)) / ((float)casesFromMap));
                    if (deathsFromMap != 0) {
                        //to account for divide by 0
                        changeDeaths = (float) 100.00 * ((((float) deathsToMap) - ((float) deathsFromMap)) / ((float) deathsFromMap));
                    } else changeDeaths = (float) 100.00 * ((((float) deathsToMap) - ((float) deathsFromMap)) / 1.0);


                    s << "Percent Change:  Cases: " << fixed << setprecision(2) << changeCases << "%  Deaths: " << changeDeaths << "%";
                    percentOut = s.str();
                    //I had to format the percent output this way in order to get the proper number of decimal points
                    //Info on this approach found here: https://stackoverflow.com/questions/29200635/convert-float-to-string-with-precision-number-of-decimal-digits-specified
                    //Accessed on December 1, 2020

                    cout << "-" << "---------------------------------------------------------" << "-" << endl;
                    cout << "|" << "                 ***********************                 " << "|" << endl;
                    cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                    cout << "|" << "                 ***********************                 " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    printFormatted("Results for " + name +":");
                    printFormatted("Map: ");
                    printFormatted(from + ": Cases: " + to_string(casesFromMap) + "  Deaths: " + to_string(deathsFromMap));
                    //info on concatenating int value to string found here: https://stackoverflow.com/questions/64782/how-do-you-append-an-int-to-a-string-in-c
                    //Accessed on November 30, 2020
                    printFormatted(to + ": Cases: " + to_string(casesToMap) + "  Deaths: " + to_string(deathsToMap));
                    cout << "|" << "                                                         " << "|" << endl;
                    printFormatted("Set: ");
                    printFormatted(from + ": Cases: " + to_string(casesFromSet) + "  Deaths: " + to_string(deathsFromSet));
                    printFormatted(to + ": Cases: " + to_string(casesToSet) + "  Deaths: " + to_string(deathsToSet));
                    cout << "|" << "                                                         " << "|" << endl;
                    printFormatted(percentOut);
                    cout << "|" << "                                                         " << "|" << endl;
                    printFormatted("Search Time: ");
                    printFormatted("Map: " + to_string(durationMap.count()) + " microseconds");
                    printFormatted("Set: " + to_string(durationSet.count()) + " microseconds");
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "-" << "---------------------------------------------------------" << "-" << endl;
                    resultsSaved = false;
                    //initialize this bool here to ensure a search has been conducted before saying results have not been saved

                }
                break;
            case 2:
                //Compare ResultsMap
                if (resultsSet.empty()){
                    //Ensure there are even resultsMap to compare
                    cout << "-" << "---------------------------------------------------------" << "-" << endl;
                    cout << "|" << "                 ***********************                 " << "|" << endl;
                    cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                    cout << "|" << "                 ***********************                 " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "|" << " ERROR: No results to compare!                           " << "|" << endl;
                    cout << "-" << "---------------------------------------------------------" << "-" << endl;
                    break;
                }

                cout << "-" << "---------------------------------------------------------" << "-" << endl;
                cout << "|" << "                 ***********************                 " << "|" << endl;
                cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                cout << "|" << "                 ***********************                 " << "|" << endl;
                cout << "|" << "                                                         " << "|" << endl;
                cout << "|" << " Compare Results                                         " << "|" << endl;
                cout << "|" << " ***************                                         " << "|" << endl;
                cout << "|" << "                                                         " << "|" << endl;
                cout << "|" << " Results:                                                " << "|" << endl;
                max = printResults();
                cout << "|" << "                                                         " << "|" << endl;
                cout << "|" << " What is the first result you would like to compare?     " << "|" << endl;
                cout << "-" << "---------------------------------------------------------" << "-" << endl;

                do {
                    cin >> firstRes;
                    //take in the first value as integer
                    try {
                        firstResInt = stoi(firstRes);
                        //atempt conversion to int, if fails print the message below, prompt user for another input
                        firstGood = true;
                    } catch (exception e) {
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << " Compare Results                                         " << "|" << endl;
                        cout << "|" << " ***************                                         " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        printFormatted("ERROR: Invalid input.");
                        cout << "|" << " Results:                                                " << "|" << endl;
                        max = printResults();
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << " What is the first result you would like to compare?     " << "|" << endl;
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        firstGood = false;
                    }
                    if (firstResInt < 1 || firstResInt > max){
                        //ensure the requested index is in the range
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << " Compare Results                                         " << "|" << endl;
                        cout << "|" << " ***************                                         " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        printFormatted("ERROR: Invalid input.");
                        cout << "|" << " Results:                                                " << "|" << endl;
                        max = printResults();
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << " What is the first result you would like to compare?     " << "|" << endl;
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        firstGood = false;
                    }
                } while (!firstGood);

                cout << "-" << "---------------------------------------------------------" << "-" << endl;
                cout << "|" << "                 ***********************                 " << "|" << endl;
                cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                cout << "|" << "                 ***********************                 " << "|" << endl;
                cout << "|" << "                                                         " << "|" << endl;
                cout << "|" << " Compare Results                                         " << "|" << endl;
                cout << "|" << " ***************                                         " << "|" << endl;
                cout << "|" << "                                                         " << "|" << endl;
                cout << "|" << " Results:                                                " << "|" << endl;
                max = printResults();
                cout << "|" << "                                                         " << "|" << endl;
                cout << "|" << " What is the second result you would like to compare?    " << "|" << endl;
                cout << "-" << "---------------------------------------------------------" << "-" << endl;

                do {
                    //same as the error catching for first result with additional if statement
                    cin >> secondRes;
                    try {
                        secondResInt = stoi(secondRes);
                        secondGood = true;
                    } catch (exception e) {
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << " Compare Results                                         " << "|" << endl;
                        cout << "|" << " ***************                                         " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        printFormatted("ERROR: Invalid input.");
                        cout << "|" << " Results:                                                " << "|" << endl;
                        max = printResults();
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << " What is the second result you would like to compare?     " << "|" << endl;
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        secondGood = false;
                    }
                    if (secondResInt < 1 || secondResInt > max){
                        //ensure the requested index is in the range
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << " Compare Results                                         " << "|" << endl;
                        cout << "|" << " ***************                                         " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        printFormatted("ERROR: Invalid input.");
                        cout << "|" << " Results:                                                " << "|" << endl;
                        max = printResults();
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << " What is the first result you would like to compare?     " << "|" << endl;
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        secondGood = false;
                    }
                    if (secondResInt == firstResInt){
                        //ensures the user does not try to compare the same exact entry
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << " Compare Results                                         " << "|" << endl;
                        cout << "|" << " ***************                                         " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        printFormatted("ERROR: Second result cannot be the same as the first");
                        cout << "|" << " Results:                                                " << "|" << endl;
                        max = printResults();
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << " What is the second result you would like to compare?     " << "|" << endl;
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        secondGood = false;
                    }
                } while (!secondGood);


                index = 1;
                //initialize to 1 to match the numbers printed with the results
                for (auto mem: resultsSet){
                    //search the results set for the entries
                    //do it this way since the number of results is variable, so just match the requested index with the
                    //variable "index"
                    if (index == firstResInt){
                        //once the first entry is found, initialize res1
                        res1 = mem.second;
                    }
                    if (index == secondResInt){
                        //same for the second
                        res2 = mem.second;
                    }
                    index++;
                }

                cout << "-" << "---------------------------------------------------------" << "-" << endl;
                cout << "|" << "                 ***********************                 " << "|" << endl;
                cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                cout << "|" << "                 ***********************                 " << "|" << endl;
                cout << "|" << "                                                         " << "|" << endl;
                cout << "|" << " Compare Results                                         " << "|" << endl;
                cout << "|" << " ***************                                         " << "|" << endl;
                cout << "|" << "                                                         " << "|" << endl;
                cout << "|" << " Results:                                                " << "|" << endl;
                max = printResults();
                cout << "|" << "                                                         " << "|" << endl;
                printFormatted("Showing results for " + firstRes + " and " + secondRes);
                cout << "|" << "                                                         " << "|" << endl;
                compareResults(res1, res2);
                cout << "-" << "---------------------------------------------------------" << "-" << endl;
                break;
            case 3:
                //Save ResultsMap
                if (case4Redir){
                    case4Redir = false;
                }
                cout << "-" << "---------------------------------------------------------" << "-" << endl;
                cout << "|" << "                 ***********************                 " << "|" << endl;
                cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                cout << "|" << "                 ***********************                 " << "|" << endl;
                cout << "|" << "                                                         " << "|" << endl;
                cout << "|" << " Save Results                                            " << "|" << endl;
                cout << "|" << " ************                                            " << "|" << endl;
                cout << "|" << "                                                         " << "|" << endl;
                cout << "|" << " What would you like to name the file?                   " << "|" << endl;
                cout << "|" << " Note: file name must end in .csv                        " << "|" << endl;
                cout << "|" << "                                                         " << "|" << endl;
                cout << "-" << "---------------------------------------------------------" << "-" << endl;

                cin.clear();
                while (!validFile) {
                    while (fileName.length() == 0) {
                        getline(cin, fileName);
                    }
                    delimInd = fileName.find('.');
                    if (delimInd == -1 || fileName.substr(delimInd) != ".csv") {
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << " Save Results                                            " << "|" << endl;
                        cout << "|" << " ************                                            " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << " ERROR: File name not formatted correctly.               " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << " What would you like to name the file?                   " << "|" << endl;
                        cout << "|" << " Note: file name must end in .csv                        " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        fileName.clear();
                    } else {
                        validFile = true;
                    }
                }

                resultsSaved = saveResults(fileName);
                if (resultsSaved){
                    cout << "-" << "---------------------------------------------------------" << "-" << endl;
                    cout << "|" << "                 ***********************                 " << "|" << endl;
                    cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                    cout << "|" << "                 ***********************                 " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "|" << " Save Results                                            " << "|" << endl;
                    cout << "|" << " ************                                            " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "|" << " Results saved!                                          " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "-" << "---------------------------------------------------------" << "-" << endl;
                } else {
                    cout << "-" << "---------------------------------------------------------" << "-" << endl;
                    cout << "|" << "                 ***********************                 " << "|" << endl;
                    cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                    cout << "|" << "                 ***********************                 " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "|" << " Save Results                                            " << "|" << endl;
                    cout << "|" << " ************                                            " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "|" << " ERROR: Results could not be saved                       " << "|" << endl;
                    cout << "|" << "                                                         " << "|" << endl;
                    cout << "-" << "---------------------------------------------------------" << "-" << endl;
                }
                break;
            case 4:
                do {
                    if (!resultsSaved) {
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << " HOLD UP! You have results you haven't saved             " << "|" << endl;
                        cout << "|" << " Do you want to exit without saving?                     " << "|" << endl;
                        cout << "|" << " Type \"yes\" or \"no\"                                      " << "|" << endl;
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        cin >> choice;
                        if (choice == "yes") {
                            opRead = true;
                            operation = 3;
                            case4Redir = true;
                            //take user directly to the save results page
                            break;
                        } else if (choice == "no") {
                            opRead = true;
                            cout << "-" << "---------------------------------------------------------" << "-" << endl;
                            cout << "|" << "                 ***********************                 " << "|" << endl;
                            cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                            cout << "|" << "                 ***********************                 " << "|" << endl;
                            cout << "|" << "                                                         " << "|" << endl;
                            cout << "|" << " Thank you for using COVID Tracker Premium!              " << "|" << endl;
                            cout << "|" << " Goodbye.                                                " << "|" << endl;
                            cout << "-" << "---------------------------------------------------------" << "-" << endl;
                            exit = true;
                        } else {
                            cout << "-" << "---------------------------------------------------------" << "-" << endl;
                            cout << "|" << "                 ***********************                 " << "|" << endl;
                            cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                            cout << "|" << "                 ***********************                 " << "|" << endl;
                            cout << "|" << "                                                         " << "|" << endl;
                            cout << "|" << " ERROR: Bad operation                                    " << "|" << endl;
                            cout << "|" << "                                                         " << "|" << endl;
                            cout << "-" << "---------------------------------------------------------" << "-" << endl;
                            opRead = false;
                        }
                    } else {
                        opRead = true;
                        //set this true to terminate loop
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                 *COVID Tracker Premium*                 " << "|" << endl;
                        cout << "|" << "                 ***********************                 " << "|" << endl;
                        cout << "|" << "                                                         " << "|" << endl;
                        cout << "|" << " Thank you for using COVID Tracker Premium!              " << "|" << endl;
                        cout << "|" << " Goodbye.                                                " << "|" << endl;
                        cout << "-" << "---------------------------------------------------------" << "-" << endl;
                        exit = true;
                    }
                } while (!opRead);


                break;
            default:
                badOp = true;
                break;
        }
    }
    return 0;
}
/*=== End Main Function ===*/




/*=== Begin Function Definitions ===*/

void compareResults(set<Date> firstSet, set<Date> secondSet){
    Date firstFrom = *firstSet.begin();
    Date firstTo = *(--firstSet.end());
    Date secondFrom = *secondSet.begin();
    Date secondTo = *(--secondSet.end());

    //Calculate percent change for the cases and deaths of each set
    float pChangeCasesFirst = (float) 100.00 * ((((float)firstTo.cases) - ((float) firstFrom.cases)) / ((float) firstFrom.cases));
    float pChangeCasesSecond = (float) 100.00 * ((((float)secondTo.cases) - ((float) secondFrom.cases)) / ((float) secondFrom.cases));
    float pChangeDeathsFirst;
    float pChangeDeathsSecond;

    //account for divide by 0
    if (firstFrom.deaths != 0) {
        pChangeDeathsFirst = (float) 100.00 * ((((float) firstTo.deaths) - ((float) firstFrom.deaths)) / ((float) firstFrom.deaths));
    } else {
        pChangeDeathsFirst = (float) 100.00 * ((((float) firstTo.deaths) - ((float) firstFrom.deaths)) / 1.00);
    }

    if (secondFrom.deaths != 0){
        pChangeDeathsSecond = (float) 100.00 * ((((float)secondTo.deaths) - ((float) secondFrom.deaths)) / ((float) secondFrom.deaths));
    } else {
        pChangeDeathsSecond = (float) 100.00 * ((((float)secondTo.deaths) - ((float) secondFrom.deaths)) / 1.00);
    }

    float casesChangeCompare = 100.00 * (pChangeCasesFirst / pChangeCasesSecond);
    float deathsChangeCompare = 100.00 * (pChangeDeathsFirst / pChangeDeathsSecond);

    stringstream s;
    string sOut;
    string test;
    // for formatting the proper decimal points

    printFormatted("1st Result:");
    printFormatted("Percent Change:");
    s << "Cases: " << fixed <<setprecision(2) << pChangeCasesFirst << "%";
    sOut = s.str();
    printFormatted(sOut);

    s.str("");
    //information on clearing the stringstream to recycle the variable taken from: https://stackoverflow.com/questions/20731/how-do-you-clear-a-stringstream-variable#:~:text=The%20clear()%20member%20function,str(%22%22)%3B
    //Accessed on Dec 6, 2020
    s << "Deaths: " << fixed << setprecision(2) << pChangeDeathsFirst << "%";
    sOut = s.str();
    printFormatted(sOut);
    cout << "|" << "                                                         " << "|" << endl;
    printFormatted("2nd Result:");
    printFormatted("Percent Change: ");

    s.str("");
    s <<  "Cases: " << fixed << setprecision(2) << pChangeCasesSecond << "%";
    sOut = s.str();
    printFormatted(sOut);

    s.str("");
    s << "Deaths: " << fixed << setprecision(2) << pChangeDeathsSecond << "%";
    sOut = s.str();
    printFormatted(sOut);
    cout << "|" << "                                                         " << "|" << endl;

    s.str("");
    s << "The first result's cases grew at " << fixed << setprecision(2) << casesChangeCompare << "%  the rate of the second result.";
    sOut = s.str();
    printFormatted(sOut);

    s.str("");
    s << "The first result's deaths grew at " << fixed << setprecision(2) << deathsChangeCompare << "% the rate of the second result.";
    sOut = s.str();
    printFormatted(sOut);


}

/*=== Search Functions ===*/
map<Date, pair<int, int> > searchMap(string countyName, Date from, Date to){
    auto searchCounty = countiesMap.find(countyName);
    //find the specified county
    if (from < searchCounty->second.entriesMap.begin()->first) {
        from = searchCounty->second.entriesMap.begin()->first;
    }
    if ((--searchCounty->second.entriesMap.end())->first < to) {
        //How to find the last element of the map taken from: https://stackoverflow.com/questions/289715/last-key-in-a-stdmap
        //Accessed on November 30, 2020
        to = (--searchCounty->second.entriesMap.end())->first;
    }
    //The above if statements determine whether the from and to dates are out of the range of entriesMap
    //if so, they default to the oldest or newest entriesMap respectively

    if (searchCounty->second.entriesMap.find(from) == searchCounty->second.entriesMap.end()){
        //if the specified from date could not be found
        while(searchCounty->second.entriesMap.find(from) != searchCounty->second.entriesMap.begin()){
            //if date not found, search for the closest date preceding it, use that for search
            if (searchCounty->second.entriesMap.find(from) != searchCounty->second.entriesMap.end()){
                break;
            } else from = from - 1;
        }
    }
    if (searchCounty->second.entriesMap.find(to) == searchCounty->second.entriesMap.end()){
        //if the specified to date could not be found
        while(searchCounty->second.entriesMap.find(to) != (--searchCounty->second.entriesMap.end())){
            //if date not found, search for the closest date proceeding it, use that for search
            if (searchCounty->second.entriesMap.find(to) != searchCounty->second.entriesMap.end()){
                break;
            } else to = to + 1;
        }
    }

    map<Date, pair<int, int> > returned;
    //create a new map to be returned containing the date
    //the code below builds the map
    auto iterate = searchCounty->second.entriesMap.find(from);
    while (iterate != searchCounty->second.entriesMap.find(to)){
        //records every entry in the given time frame
        returned.emplace(iterate->first, iterate->second);
        iterate++;
    }

    //Add the last set of entriesMap
    //I did it this way since I could not figure out how to get the loop to go all the way to the final date, so I just do
    //after here
    returned.emplace(searchCounty->second.entriesMap.find(to)->first, searchCounty->second.entriesMap.find(to)->second);

    return returned;

}

set<Date> searchSet(string name, Date from, Date to){
    int index = name.find(',');
    string county = name.substr(0, index);
    string state = name.substr(index + 2);
    cout << county << endl;
    cout << state << endl;
    //parse the string "name" for further evaluation
    State tempState = State(state);
    County tempCounty = County(county, state);
    set<Date> returned;

    auto mem = states.begin();
    while (mem != states.end()){
        if (tempState == *mem){
            break;
        }
        mem++;
    }
    if (mem != states.end()){
        //if the state was found
        auto memC = mem->counties.begin();
        while (memC != mem->counties.end()){
            if (tempCounty == *memC){
                break;
            }
            memC++;
        }
        if (memC != mem->counties.end()){
            //if the county was found
            if (from < (*memC->entriesSet.begin())) {
                //info on accessing the value the iterator points taken from https://stackoverflow.com/questions/36959516/c-how-to-return-the-value-the-iterator-of-a-set-is-pointing-to
                //Accessed on December 3, 2020
                from = (*memC->entriesSet.begin());
            }
            if (to > (*(--memC->entriesSet.end()))){
                to = (*(--memC->entriesSet.end()));
            }
            //Above if statements check if the date is outside the range

            //Below if statements check whether the specific date exists, if it is missing, it finds the nearest available date
            if (memC->entriesSet.find(from) == memC->entriesSet.end()){
                while(memC->entriesSet.find(from) != memC->entriesSet.begin()){
                    if (memC->entriesSet.find(from) != memC->entriesSet.end()){
                        break;
                    } else from = from - 1;
                }
            }

            if (memC->entriesSet.find(to) == memC->entriesSet.end()){
                while (memC->entriesSet.find(to) != (--memC->entriesSet.end())){
                    if (memC->entriesSet.find(to) != memC->entriesSet.end()){
                        break;
                    } else to = to + 1;
                }
            }

            //the code below inserts all dates in the timeframe in the returned set
            //same logic as the insertion in searchMap
            cout << from.getDate() << endl;
            cout << to.getDate() << endl;

            auto iter = memC->entriesSet.find(from);
            while (iter != memC->entriesSet.find(to)){
                cout << iter->cases << " " << iter->deaths << endl;
                returned.insert(*iter);
                iter++;
            }

            returned.insert(*memC->entriesSet.find(to));
        }
    }
    return returned;
}

/*=== Save ===*/

bool saveResults(string fileName){
    try {
        ofstream resultFile(fileName);
        resultFile << "date, county, state, confirmed cases, deaths\n";
        for (auto mem:resultsSet) {
            for (auto mem2: mem.second) {
                resultFile << "\"" << mem2.getDate() <<"\""<< ", " << mem.first << ", " << mem2.cases << ", " << mem2.deaths << "\n";
                // Info on ignoring "," when writing csv file found here https://stackoverflow.com/questions/46804146/c-how-can-i-ignore-comma-from-csv-char
                //Accessed on Dec 6, 2020
            }
        }
        return true;
    } catch (exception e){
        return false;
    }
}



/*=== Insertion ===*/

void parseEntryMap(string line){
    //This function parses the passed in line, then creates a county instance
    int index;

    /* Find date */
    index = line.find(',');
    line.erase(line.begin() + index);
    //the above lines account for the ',' used in the date format
    index = line.find(',');
    string _date = line.substr(1, index - 2);
    line = line.substr(index + 1);

    /* Find County */
    index = line.find (',');
    string county = line.substr(0, index);
    line = line.substr(index + 1);

    /* Find State */
    index = line.find(',');
    string state = line.substr(0, index);
    line = line.substr(index + 1);

    /* Find Total Cases */
    index = line.find(',');
    int cases = stoi(line.substr(0, index));
    line = line.substr(index + 1);

    /* Find Deaths */
    int deaths;
    try {
        deaths = stoi(line);
    } catch (exception& e){
        //some deaths entries have the value "null" make this 0 for these entries
        deaths = 0;
    }

    Date date = Date(_date, cases, deaths);

    string countyName = county + ", " + state;
    bool countyFound = false;

    auto mem = countiesMap.find(countyName);
    if (mem != countiesMap.end()){
        countyFound = true;
        mem->second.insertEntry(date, cases, deaths);
    }

    //below statement only executes if the specified county has not been created yet
    if (!countyFound){
        County temp = County(county, state);
        //create a new county
        temp.insertEntry(date, cases, deaths);
        //add the entry to the county
        countiesMap.emplace(pair<string, County> (countyName, temp));
        //add the county to the countiesMap map
    }
}


void buildSet(){
    //due to the "const" nature of set elements, the set structure is build from the map structure since elements are not
    //modifiable after insertion

    map<string, State> stateHolder;
    //use map to hold all the states temporarily

    for (auto memCounty: countiesMap){
        //iterates through countiesMap, so all county instances are already built

        for (auto memEntry: memCounty.second.entriesMap){
            //add each date to the date set for the county
            memCounty.second.entriesSet.insert(memEntry.first);
        }
        string stateOfCounty = memCounty.second.state;
        //save name of the state the current county is in, then check
        auto memState = stateHolder.find(stateOfCounty);
        if (memState != stateHolder.end()){
            //if the state was found, just add the county to the appropriate state
            memState->second.counties.insert(memCounty.second);
        } else {
            State temp = State(stateOfCounty);
            //otherwise create a new state instance
            temp.counties.insert(memCounty.second);
            stateHolder.emplace(stateOfCounty, temp);
        }
    }

    for (auto memState: stateHolder){
        //after building everything in "stateHolder", transfer the elements to the set container
        states.insert(memState.second);

    }
}

/*=== Helper Functions ===*/
void printFormatted(string line){
    //this function allows the program to print string of random length while keeping it formatted to the menu
    string out;
    out = "| " + line;
    if (out.size() > 58){
        //this if statement prints another line if the out line has too many characters for the formatted interface
        string sub = out.substr(0, 58);
        int lastSpace = sub.find_last_of(' ');
        //Information of the "find_last_of" function taken from https://www.geeksforgeeks.org/stdstringfind_last_of-in-c-with-examples/#:~:text=The%20std%3A%3Astring%3A%3A,it%20returns%20string%3A%3Anpos.
        //Accessed on November 30, 2020
        sub = out.substr(0, lastSpace);
        out = "| " + out.substr(lastSpace + 1);
        //the above substrings split the passed in line after the final word fitting within the 59 character limit
        //use 58 to account for the "|" character which is added

        while (sub.size() < 58) {
            //add spaces until the size is 58
            sub += " ";
        }
        sub += "|";
        cout << sub << endl;

        //Do the same for the next line
        while (out.size() < 58) {
            out += " ";
        }
        out += "|";
        cout << out << endl;


    } else {
        //for a single line, just format then print
        while (out.size() < 58) {
            out += " ";
        }
        out += "|";
        cout << out << endl;
    }
}


int printResults(){
    int index = 1;
    for (auto member: resultsSet){
        printFormatted(to_string(index) + ".) " + member.first + ": ");
        printFormatted("From: " + member.second.begin()->getDate() + "    To: " + (--member.second.end())->getDate());

        index++;
    }
    return index;
    //return index to let main function know the max number to accept when choosing results to compare

}


//A function for determining which error message to print
void determineDateError(int e){
    switch (e) {
        case 0:
            printFormatted("ERROR: The date entered is a negative number.");
            break;
        case 1:
            printFormatted("ERROR: The month was not formatted properly.");
            break;
        case 2:
            printFormatted("ERROR: February does not have that many days.");
            break;
        case 3:
            printFormatted("ERROR: February does not have that many days.");
            break;
        case 4:
            printFormatted("ERROR: January does not have that many days.");
            break;
        case 5:
            printFormatted("ERROR: March does not have that many days.");
            break;
        case 6:
            printFormatted("ERROR: May does not have that many days.");
            break;
        case 7:
            printFormatted("ERROR: July does not have that many days.");
            break;
        case 8:
            printFormatted("ERROR: August does not have that many days.");
            break;
        case 9:
            printFormatted("ERROR: October does not have that many days.");
            break;
        case 10:
            printFormatted("ERROR: December does not have that many days.");
            break;
        case 11:
            printFormatted("ERROR: April does not have that many days.");
            break;
        case 12:
            printFormatted("ERROR: June does not have that many days.");
            break;
        case 13:
            printFormatted("ERROR: September does not have that many days.");
            break;
        case 14:
            printFormatted("ERROR: November does not have that many days.");
            break;
        case 15:
            printFormatted("ERROR: To date is earlier than From date.");
            break;
        case 16:
            printFormatted("ERROR: Date not formatted correctly.");
            break;
        default:
            printFormatted("ERROR: Invalid input.");

    }
}
