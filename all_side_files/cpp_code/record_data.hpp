#ifndef DATA_RECORD_HPP
#define DATA_RECORD_HPP

#include <string>
#include <fstream>
#include "classes.hpp"

using namespace std;
/**
 * TODO: Recreate this function for every class //DONE
 */

void FormRouteSchwarz(string *Route, int amntNodes, int amntSubdomains)
{
    std::string sep = "_";
    std::string size = std::to_string(amntNodes - 1);
    std::string AS = std::to_string(amntSubdomains);

    if (amntNodes < 10)
    {
        size = "00" + size;
    }
    else if (amntNodes >= 10 && amntNodes < 100)
    {
        size = "0" + size;
    }
    if (amntSubdomains < 2)
    {
        *Route +=size + ".dat";
    }
    else
    {
        *Route +=size + sep + AS + ".dat";
    }
}

void Record_AddData(int amntNodes, string Route, int amntSubdomains, int Counter, double stopCriteria, double Coef_Overflow)
{

    std::string sep = "_";
    std::string size = std::to_string(amntNodes - 1);
    std::string name = "AddData";
    std::string AS = std::to_string(amntSubdomains);

    if (amntNodes < 10)
    {
        size = "00" + size;
    }
    else if (amntNodes >= 10 && amntNodes < 100)
    {
        size = "0" + size;
    }
    if (amntSubdomains < 2)
    {
        Route += name + sep + size + ".dat";
    }
    else
    {
        Route += name + sep + size + sep + AS + ".dat";
    }
    ofstream ofile(Route);
    ofile << amntNodes - 1;
    ofile << endl;
    ofile << amntSubdomains;
    ofile << endl;
    ofile << Counter;
    ofile << endl;
    ofile << stopCriteria;
    ofile << endl;
    ofile << Coef_Overflow;
    ofile << endl;
}

#endif