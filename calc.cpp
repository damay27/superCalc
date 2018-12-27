#include "program.hpp"
#include <fstream>
#include <regex>
#include <iostream>
#include <math.h>
#include <exception>

program::program()
{

}




bool program::run()
{
    cout<<"Welcome to superCalc. Pi(pi) and Eulers number(e) are already loaded for you."<<endl;
    cout<<"To close sueprCalc type \"exit\"."<<endl;

    variables["pi"] = 3.14159265;
    variables["e"] = 2.71828182;

    string line;
    float ans;

    while(1)
    {
        getline(cin, line);
        line = line.substr(0, line.size());

        if(line == "exit")
        {
            break;
        }
        else if(assign(line))
        {
            continue;
        }
        else if(mathSolver(line, ans))
        {
            cout<<"=> "<<ans<<endl;
        }
    }

    return 0;

}


bool program::assign(string codeLine)
{
    //Regex for matching strings and for matching math/variable expressions
    regex mathRegex("[\\w]+[ ]?=[ ]?[^\\s]+");

    smatch match;

    /*
        NOTE: The math regex will catch both the string assignment and the string error case.
              There for it needs to be the last thing check in the if else block.
    */

    if(regex_search(codeLine, match, mathRegex))
    {
        codeLine = match[0];

        
        //Split the string at "LET" and the "="
        int equalsIndex = codeLine.find("=");
        //The variable name is the value between "LET" and "=". The math comes after the "="
        string varName = codeLine.substr(0, equalsIndex);
        string math = codeLine.substr(equalsIndex+1);

        //There may be spaces left in the variable name so remove them. The math solver
        //already removes the spaces so there is no need to do it for the math string
        removeSpaces(varName);

        float ans;
        
        mathSolver(math, ans);

        variables[varName] = ans;

        return true;
    }
    //If the expression did not match either regex then return false
    else
    {
        return false;
    }
}

/*
    #################Math Functions
*/

bool program::mathSolver(string math, float &ans)
{
    //Loops over all of the string
    for(long unsigned int i=0; i<math.size(); i++)
    {
        //Check for parentheses and if they are found solve the
        //math inside them and then replace the parentheses with the result
        if(math.at(i) == '(')
        {
            //This holds the parentheses count. A '(' increaes the cound by one
            //while a ')' decreases the cound by one. When pCount equals zero you know that you 
            //have found the right parentheses that matches your current left parentheses.
            int pCount = 1;

            //Position where the left parenteseis was found.
            int pPos = i;

            //Index to start looking for the right parentheses
            long unsigned int index = i+1;

            //loop until either the end of the string or the right parentheses was found
            while(index<math.size() && pCount != 0)
            {
                if(math.at(index) == '(') {pCount++;}
                if(math.at(index) == ')') {pCount--;}
                index++;
            }

            if(index >= math.size() && pCount != 0)
            {
                throw runtime_error("ERROR: Parentheses mismatch.");
            }
            else
            {
                string pSub = math.substr(pPos + 1, index - pPos - 2);
                mathSolver(pSub, ans);
                math.erase(pPos, index-pPos);
                math.insert(pPos, to_string(ans));
            }

        }
    }

    mathOp(math, "^", this->solveExponent);
    mathOp(math, "*", this->solveMultiplication);
    mathOp(math, "/", this->solveDivision);
    mathOp(math, "+", this->solveAddition);
    mathOp(math, "-", this->solveSubtraction);

    removeSpaces(math);

    if(stringISnumber(math))
    {
        ans = stof(math);
    }
    else
    {
        ans = getVarMath(math);
    }

    return true;


}


void program::mathOp(string &math, string symbol, const function<float(float, float)> &f)
{
    smatch sm;
    regex reg("[\\w.]+[ ]?\\"+symbol+"[ ]?[\\w.]+");

    while(regex_search(math, sm, reg))
    {
        string sub = sm[0];

        removeSpaces(sub);

        int splitIndex = sub.find(symbol);

        string left = sub.substr(0, splitIndex);
        string right = sub.substr(splitIndex+1);

        float rightVal, leftVal;

        //Check if the right side is a variable and if it is access
        //its value from one of the maps.
        if(stringISnumber(right))
        {
            rightVal = stof(right);
        }
        else if(variables.find(right) != variables.end())
        {
            rightVal = getVarMath(right);
        }


        //Check if the left side is a variable and if it is access
        //its value from one of the maps.
        if(stringISnumber(left))
        {
            leftVal = stof(left);
        }
        else
        {
            leftVal = getVarMath(left);
        }


        float ans = f(leftVal, rightVal);

        int startPos = sm.position(0);
        int len = sm[0].str().size();

        math.erase(startPos, len);
        math.insert(startPos, to_string(ans));

    }
}

float program::solveExponent(float leftVal, float rightVal)
{
    return pow(leftVal, rightVal);
}

float program::solveMultiplication(float leftVal, float rightVal)
{
    return leftVal*rightVal;
}

float program::solveDivision(float leftVal, float rightVal)
{
    return leftVal/rightVal;
}

float program::solveAddition(float leftVal, float rightVal)
{
    return leftVal+rightVal;
}

float program::solveSubtraction(float leftVal, float rightVal)
{
    return leftVal-rightVal;
}


/*
    #################Helper Functions
*/

//Retreive a variable value as a float for use in the math solver
float program::getVarMath(string varName)
{
    if(variables.find(varName) != variables.end())
    {
        return variables[varName];
    }
    else
    {
        cout<<"Error variable not found: "<<varName<<endl;
        throw runtime_error("ERROR: Variable not found.");
    }
}


//Checks if the given string only contains numbers
bool program::stringISnumber(string s)
{
    for(long unsigned int i = 0; i<s.size(); i++)
    {
        if(isdigit(s.at(i)) == false && s.at(i) != '.')
        {
            return false;
        }
    }

    return true;
}


void program::removeSpaces(string &s)
{
    regex r("[ ]+");

    s = regex_replace(s, r, "");
}