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
    void mathFunc(string &math, string symbol, const function<float(float)> &f);

    static float solveExponent(float leftVal, float rightVal);
    static float solveMultiplication(float leftVal, float rightVal);
    static float solveDivision(float leftVal, float rightVal);
    static float solveAddition(float leftVal, float rightVal);
    static float solveSubtraction(float leftVal, float rightVal);

    static float solveSinDegrees(float f);
    static float solveSinRadians(float f);
    static float solveAsinDegrees(float f);
    static float solveAsinRadians(float f);
    static float solveCosDegrees(float f);
    static float solveCosRadians(float f);
    static float solveAcosDegrees(float f);
    static float solveAcosRadians(float f);
    static float solveTanDegrees(float f);
    static float solveTanRadians(float f);
    static float solveAtanDegrees(float f);
    static float solveAtanRadians(float f);


    //Helper Functions
    bool stringISnumber(string s);
    float getVarMath(string varName);
    void removeSpaces(string &s);



    bool assign(string codeLine);


    public:
        calc();
        bool runShell();
        bool runFile(string fileName);
};

#endif