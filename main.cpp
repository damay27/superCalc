#include <iostream>
#include <regex>
#include <string>
#include "calc.hpp"

using namespace std;

int main(int argc, char *argv[])
{

        calc c;
    
    if(argc > 1)
    {
        string fileName = argv[1];

        if(!c.runFile(fileName))
        {
            cout<<"Error"<<endl;
            return -1;
        }
        
        cout<<"***Finished running file***"<<endl;

    }


    cout<<"Welcome to superCalc. Pi(pi) and Eulers number(e) are already loaded for you."<<endl;
    cout<<"To close sueprCalc type \"exit\"."<<endl;
    cout<<"Here is a list a avalible math functions"<<endl;
    cout<<"    sin() -> Sine in degrees"<<endl;
    cout<<"    cos() -> Cosine in degrees"<<endl;
    cout<<"    tan() -> Tangent in degrees"<<endl;
    cout<<"    asin() -> Arc Sine in degrees"<<endl;
    cout<<"    acos() -> Arc Cosine in degrees"<<endl;
    cout<<"    atan() -> Arc Tangent in degrees"<<endl;
    cout<<"    sinr() -> Sine in radians"<<endl;
    cout<<"    cosr() -> Cosine in radians"<<endl;
    cout<<"    tanr() -> Tangent in radians"<<endl;
    cout<<"    asinr() -> Arc Sine in radians"<<endl;
    cout<<"    acosr() -> Arc Cosine in radians"<<endl;
    cout<<"    atanr() -> Arc Tangent in radians"<<endl;

    if(!c.runShell())
    {
        cout<<"Error"<<endl;
        return -1;
    }


}