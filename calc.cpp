#include "calc.hpp"
#include <fstream>
#include <regex>
#include <iostream>
#include <math.h>
#include <exception>
#include <fstream>
#include <sstream>

#define PI M_PI
#define EULER 2.71828182

calc::calc()
{

}


//This function runs the commands that are found in a file. As input it takes
//the name of the file to open. It returns true if sucessful and false if it encounters errors
bool calc::runFile(string fileName)
{

    //Open the file and make sure it is open
    ifstream file;
    file.open(fileName);

    if(!file.is_open())
    {
        cout << "Error: File not found." << endl;

        return false;
    }

    //Define useful variables
    variables["pi"] = PI;
    variables["e"] = EULER;

    string line;
    float ans;


    //Loope until the file is finished or an error occurs
    while(!file.eof())
    {
        try{    
            getline(file, line);
            line = line.substr(0, line.size());

            //If an exit command is reached then leave the loop
            if(line == "exit")
            {
                break;
            }
            //If the line was an assignment operation then move on to the next line
            else if(assign(line))
            {
                continue;
            }
            //If the line was a math operation then move on to the next line
            else if(mathSolver(line, ans))
            {
                cout<<"=> "<<ans<<endl;
            }
        }
        //If an exception string was caught then print it and return false
        catch(string e)
        {
            cout<<e<<endl;

            return false;
        }
    }

    //When the loop is done close the file and return true
    file.close();

    return true;

}

//This function runs the commands that the user enters. It takes no input parameters.
//It returns true if sucessful and false if it encounters errors
bool calc::runShell()
{

    //Define useful variables
    variables["pi"] = PI;
    variables["e"] = EULER;

    string line;
    float ans;

    //Loop until the user types the exit command or an error occurs
    while(1)
    {
        try{    
            getline(cin, line);
            line = line.substr(0, line.size());
            
            //If an exit command is given then leave the loop
            if(line == "exit")
            {
                break;
            }
            //If the command was an assignment operation then move on to the next line
            else if(assign(line))
            {
                continue;
            }
            //If the command was a math operation then move on to the next line
            else if(mathSolver(line, ans))
            {
                cout<<"=> "<<ans<<endl;
            }
        }
        //If an exception string was caught then print it and return false
        catch(string e)
        {
            cout<<e<<endl;
        }
    }

    return true;

}


bool calc::assign(string codeLine)
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

bool calc::mathSolver(string math, float &ans)
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

            //If the end of the string is reached and the mathching parentheses was not found
            //then throw and error
            if(index >= math.size() && pCount != 0)
            {
                throw string("ERROR: Parentheses mismatch.");
            }
            else
            {
                //Extract the command inside the parentheses
                string pSub = math.substr(pPos + 1, index - pPos - 2);
                
                //Check if it is not blank
                if(pSub == "")
                {
                    throw string("Error: Empty parentheses.");
                }


                //Solve the math and replace the parentheses with the answer
                mathSolver(pSub, ans);
                math.erase(pPos, index-pPos);
                math.insert(pPos, to_string(ans));
            }

        }
    }

    //Run the math operation functions
    mathOp(math, "^", this->solveExponent);
    mathOp(math, "*", this->solveMultiplication);
    mathOp(math, "/", this->solveDivision);
    mathOp(math, "+", this->solveAddition);
    mathOp(math, "-", this->solveSubtraction);

    //Run the sine solver functions
    mathFunc(math, "asinr", solveAsinRadians);
    mathFunc(math, "sinr", solveSinRadians);
    mathFunc(math, "asin", solveAsinDegrees);
    mathFunc(math, "sin", solveSinDegrees);

    //Run the cosine solver functions
    mathFunc(math, "acosr", solveAcosRadians);
    mathFunc(math, "cosr", solveCosRadians);
    mathFunc(math, "acos", solveAcosDegrees);
    mathFunc(math, "cos", solveCosDegrees);

    //Run the tangent solver functions
    mathFunc(math, "atanr", solveAtanRadians);
    mathFunc(math, "tanr", solveTanRadians);
    mathFunc(math, "atan", solveAtanDegrees);
    mathFunc(math, "tan", solveTanDegrees);

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

//Runs the given math operation function on the math string. As input it takes a referance to
//the math string, the symbol for the operator, and a funciton pointer to the functions that will
//be used to actually solve the operator. This function returns nothing.
void calc::mathOp(string &math, string symbol, const function<float(float, float)> &f)
{
    //Make a regex to find all the instances of the operator
    smatch sm;
    regex reg("[\\w.]+[ ]?\\"+symbol+"[ ]?[\\w.]+");

    //Find and loop through all the instances
    while(regex_search(math, sm, reg))
    {
        string sub = sm[0];

        removeSpaces(sub);

        int splitIndex = sub.find(symbol);

        //Get the data on the left and right side of the operator
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


        //Solve the math for the operator using the given function
        float ans = f(leftVal, rightVal);

        //Replace the operator and the values to its left and right with the answer
        int startPos = sm.position(0);
        int len = sm[0].str().size();
        math.erase(startPos, len);
        math.insert(startPos, to_string(ans));

    }
}


//Functions for solve the math operations
float calc::solveExponent(float leftVal, float rightVal)
{
    return pow(leftVal, rightVal);
}

float calc::solveMultiplication(float leftVal, float rightVal)
{
    return leftVal*rightVal;
}

float calc::solveDivision(float leftVal, float rightVal)
{
    return leftVal/rightVal;
}

float calc::solveAddition(float leftVal, float rightVal)
{
    return leftVal+rightVal;
}

float calc::solveSubtraction(float leftVal, float rightVal)
{
    return leftVal-rightVal;
}



//Functions for solving sine
float calc::solveSinDegrees(float f)
{
    return sin( (f*PI)/180.0 );
}

float calc::solveSinRadians(float f)
{
    return sin(f);
}

float calc::solveAsinDegrees(float f)
{
    return (asin(f)*180.0)/PI;
}

float calc::solveAsinRadians(float f)
{
    return asin(f);
}



//Functions for solving cosine
float calc::solveCosDegrees(float f)
{
    return cos( (f*PI)/180.0 );
}

float calc::solveCosRadians(float f)
{
    return cos(f);
}

float calc::solveAcosDegrees(float f)
{
    return (acos(f)*180.0)/PI;
}

float calc::solveAcosRadians(float f)
{
    return acos(f);
}



//Functions for solving tangent
float calc::solveTanDegrees(float f)
{
    return tan( (f*PI)/180.0 );
}

float calc::solveTanRadians(float f)
{
    return tan(f);
}

float calc::solveAtanDegrees(float f)
{
    return (atan(f)*180.0)/PI;
}

float calc::solveAtanRadians(float f)
{
    return atan(f);
}



//Runs the given math operation function on the math string. As input it takes a referance to
//the math string, the symbol for the function being solved such as "sin" or "acosr", and a 
//funciton pointer to the functions that will be used to actually solve the math function. 
//This function returns nothing.
void calc::mathFunc(string &math, string symbol, const function<float(float)> &f)
{   
    //Create the regex that will actually find the given functio "symbol" such as sin
    //NOTE: This relies on the parentheses solver already having resolved the value in the 
    //parenthese. For example "sin(2+2)" will already have been resolved to "sin4"
    smatch sm;
    regex reg(symbol+"([\\w.]+)");

    //Find all the instances of the "symbol" and loop through all of them
    while(regex_search(math, sm, reg))
    {
        string value = sm[0].str().substr(symbol.size());
        removeSpaces(value);
        float floatValue;

        //Check if the right side is a variable and if it is access
        //its value from one of the maps.
        if(stringISnumber(value))
        {
            floatValue = stof(value);
        }
        else if(variables.find(value) != variables.end())
        {
            floatValue = getVarMath(value);
        }

        //Solve for the value using the given function pointer
        float ans = f(floatValue);

        //Check and make sure the value return was not a nan. A nan is returned if the input
        //for a given function does not make mathematical sense such as passing 180 degrees 
        //to arcsine
        if(isnan(ans))
        {
            throw string("ERROR: Invalid function operation");
        }

        //Repalce the operator and its value with the answer
        int startPos = sm.position(0);
        int len = sm[0].str().size();
        math.erase(startPos, len);
        math.insert(startPos, to_string(ans));

    }
}


/*
    #################Helper Functions
*/

//Retreive a variable value as a float for use in the math solver
float calc::getVarMath(string varName)
{
    if(variables.find(varName) != variables.end())
    {
        return variables[varName];
    }
    else
    {
        throw string("ERROR: Variable not found.");
    }
}


//Checks if the given string only contains numbers
bool calc::stringISnumber(string s)
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

//Uses a reges to remove all space from the given string
void calc::removeSpaces(string &s)
{
    regex r("[ ]+");

    s = regex_replace(s, r, "");
}