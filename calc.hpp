#ifndef CALC_HPP
#define CALC_HPP

#include <map>
#include <vector>
#include <string>
#include <functional>

using namespace std;

class calc
{
    map<string, float> variables;


    //Math solver functions
    bool mathSolver(string math, float &ans);
    void mathOp(string &math, string symbol, const function<float(float, float)> &f);
    static float solveExponent(float leftVal, float rightVal);
    static float solveMultiplication(float leftVal, float rightVal);
    static float solveDivision(float leftVal, float rightVal);
    static float solveAddition(float leftVal, float rightVal);
    static float solveSubtraction(float leftVal, float rightVal);


    //Helper Functions
    bool stringISnumber(string s);
    float getVarMath(string varName);
    void removeSpaces(string &s);



    bool assign(string codeLine);


    public:
        calc();
        bool run();
};

#endif