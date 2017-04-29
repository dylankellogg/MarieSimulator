//////////////////////////
//Marie Simulator in C++//
//Dylan Kellogg///////////

//This program can interpret, analyze, and execute any .mas file that is
//syntax correct in the MarieSim environment. No special attention 
//needed for comments (given with //, as in c++) or spaces, as the interpreter
//will automatically ignore them. Special attention was given to ensure the
//following program will preform much in the same fashion as MarieSim, and
//any file that will compile into a MEX file in Marie simulator will work
//in this program. Enjoy!



#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <map>
#include <string>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions

unsigned short hexStringToShort(const std::string& hex) {
    unsigned short value = 0;
    for (size_t i = 0; i < hex.length(); i++) {
        value <<= 4;
        if ('a' <= hex.at(i) && hex.at(i) <= 'f') {value += (hex.at(i) - 'a') + 10;}
        else if ('0' <= hex.at(i) && hex.at(i) <= '9') {value += hex.at(i) - '0';}
    }
    return value;
}

unsigned short decStringToShort(const std::string& dec) {
    unsigned short value = 0;
    bool isNegatve = false;
    size_t i = 0;
    if (dec.at(0) == '-') {isNegatve = true; i++;}
    for (; i < dec.length(); i++) {
        value *= 10;
        if ('0' <= dec.at(i) && dec.at(i) <= '9') {value += dec.at(i) - '0';}
    }
    if (isNegatve) {
        value *= -1;
    }
    return value;
}

unsigned short octStringToShort(const std::string& oct) {
    unsigned short value = 0;
    for (size_t i = 0; i < oct.length(); i++) {
        value <<= 3;
        if ('0' <= oct.at(i) && oct.at(i) <= '7') {value += oct.at(i) - '0';}
    }
    return value;
}

string convertInt(int number)
{
    stringstream ss;
    ss << std::hex << number;
    return ss.str();
}

unsigned short M(unsigned short Memory) {
    Memory = 1;
    return Memory;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Main

int main() {
    map<string, unsigned short> symbolTable;
    unsigned short bufferLength = 4096;
    unsigned short dataField[bufferLength];
    string starterString;    
    string filename;                                                               
    string line;    
    ifstream inFile;                                                
    int lineValue = 0;                                                                   
    int newLine = 0;
    int linecountthing = 0;
    int starterInt = 0;
    int remainder = 0;
    int warning = 0;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Open File

    cout << "Please enter file name: ";                                  
    cin  >> filename;                                                
    inFile.open(filename.c_str());
    if (inFile.fail()) {
        cout << "\n\nFile not Located, Error.\n" << endl;                                               
        exit(1);
    }
    cout << "\n\nFile successfully opened." << endl;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Find Symbols

    while (getline(inFile,line))  {
        int symbolFinder = 0;
        string wordz[10];        
        int p = 0;        
        stringstream ssin(line);
        while (ssin.good() && p < 10){
            ssin >> wordz[p];
            ++p;
        }
        for(p = 0; p < 10; p++){
            use_facet < ctype<char> > (locale()).toupper(&wordz[p][0], &wordz[p][0] + wordz[p].size());
            if(wordz[0] == "ORG") {
                starterInt = hexStringToShort(wordz[1]) - 1;
                lineValue = lineValue + starterInt;
                starterInt = lineValue;
                break;
            }
            if(!(wordz[0] == "ADD"         ||
                        wordz[0] == "ADDI"        ||
                        wordz[0] == "CLEAR"       ||
                        wordz[0] == "INPUT"       ||          
                        wordz[0] == "HALT"        ||          
                        wordz[0] == "JUMP"        ||          
                        wordz[0] == "JUMPI"       ||          
                        wordz[0] == "JNS"         ||          
                        wordz[0] == "LOAD"        ||          
                        wordz[0] == "LOADI"       ||          
                        wordz[0] == "OUTPUT"      ||          
                        wordz[0] == "SKIPCOND"    ||          
                        wordz[0] == "STORE"       ||          
                        wordz[0] == "STOREI"      ||          
                        wordz[0] == "SUBT"        ||
                        wordz[0] == "ORG"         ||
                        wordz[0] == ""            ||
                        wordz[0].substr(0, 1)=="/"))
            {
                wordz[0].erase(remove(wordz[0].begin(), wordz[0].end(), ','), wordz[0].end());
                symbolTable[wordz[0]] = lineValue;
                break;
            }
        }
        if(wordz[0].substr(0, 1)=="/"||wordz[0] == ""){  
        } else {
            lineValue++;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Create Data Table

    inFile.close();
    inFile.open(filename.c_str());
    lineValue = 0;
    while (getline(inFile,line))  {
        string words[10];
        int i = 0;
        stringstream ssin(line);
        while (ssin.good() && i < 10){
            ssin >> words[i];
            ++i;
        }
        i = 0;
        use_facet < ctype<char> > (locale()).toupper(&words[i][0], &words[i][0] + words[i].size());
        for(int i = 0; i < 3; i++ ) {
            use_facet < ctype<char> > (locale()).toupper(&words[i][0], &words[i][0] + words[i].size());
            if(words[0] == "ORG") {
                lineValue = starterInt;
            }
            if (words[i] == "HEX") {
                dataField[lineValue] = 0x0000 + hexStringToShort(words[i+1]);
            }
            if (words[i] == "DEC") {
                dataField[lineValue] = 0x0000 + decStringToShort(words[i+1]);
            }
            if (words[i] == "OCT") {
                dataField[lineValue] = 0x0000 + octStringToShort(words[i+1]);
            }
            if (words[i] == "JNS") {
                dataField[lineValue] = 0x0000;
                for(map<string, unsigned short>::const_iterator it = symbolTable.begin(); it != symbolTable.end(); ++it) {
                    use_facet < ctype<char> > (locale()).toupper(&words[i+1][0], &words[i+1][0] + words[i+1].size());                
                    if(words[i+1] == it->first) {
                        dataField[lineValue] = dataField[lineValue] + it->second;
                    }
                }
            }
            if (words[i] == "LOAD") {
                dataField[lineValue] = 0x1000;
                for(map<string, unsigned short>::const_iterator it = symbolTable.begin(); it != symbolTable.end(); ++it) {
                    use_facet < ctype<char> > (locale()).toupper(&words[i+1][0], &words[i+1][0] + words[i+1].size());                
                    if(words[i+1] == it->first) {
                        dataField[lineValue] = dataField[lineValue] + it->second;
                    }
                }
            }
            if (words[i] == "STORE") {
                dataField[lineValue] = 0x2000;
                for(map<string, unsigned short>::const_iterator it = symbolTable.begin(); it != symbolTable.end(); ++it) {
                    use_facet < ctype<char> > (locale()).toupper(&words[i+1][0], &words[i+1][0] + words[i+1].size());                
                    if(words[i+1] == it->first) {
                        dataField[lineValue] = dataField[lineValue] + it->second;
                    }
                }
            }
            if (words[i] == "ADD") {
                dataField[lineValue] = 0x3000;
                for(map<string, unsigned short>::const_iterator it = symbolTable.begin(); it != symbolTable.end(); ++it) {
                    use_facet < ctype<char> > (locale()).toupper(&words[i+1][0], &words[i+1][0] + words[i+1].size());                
                    if(words[i+1] == it->first) {
                        dataField[lineValue] = dataField[lineValue] + it->second;
                    }
                }
            }
            if (words[i] == "SUBT") {
                dataField[lineValue] = 0x4000;
                for(map<string, unsigned short>::const_iterator it = symbolTable.begin(); it != symbolTable.end(); ++it) {
                    use_facet < ctype<char> > (locale()).toupper(&words[i+1][0], &words[i+1][0] + words[i+1].size());                
                    if(words[i+1] == it->first) {
                        dataField[lineValue] = dataField[lineValue] + it->second;
                    }
                }
            }
            if (words[i] == "INPUT") {
                dataField[lineValue] = 0x5000;
            }
            if (words[i] == "OUTPUT") {
                dataField[lineValue] = 0x6000;
            }
            if (words[i] == "HALT") {
                dataField[lineValue] = 0x7000;
            }
            if (words[i] == "SKIPCOND") {
                dataField[lineValue] = 0x8000 + hexStringToShort(words[i+1]);
            }
            if (words[i] == "JUMP") {
                dataField[lineValue] = 0x9000;
                for(map<string, unsigned short>::const_iterator it = symbolTable.begin(); it != symbolTable.end(); ++it) {
                    use_facet < ctype<char> > (locale()).toupper(&words[i+1][0], &words[i+1][0] + words[i+1].size());                
                    if(words[i+1] == it->first) {
                        dataField[lineValue] = dataField[lineValue] + it->second;
                    }
                }
            }
            if (words[i] == "CLEAR") {
                dataField[lineValue] = 0xA000;
            }
            if (words[i] == "ADDI") {
                dataField[lineValue] = 0xB000;
                for(map<string, unsigned short>::const_iterator it = symbolTable.begin(); it != symbolTable.end(); ++it) {
                    use_facet < ctype<char> > (locale()).toupper(&words[i+1][0], &words[i+1][0] + words[i+1].size());                
                    if(words[i+1] == it->first) {
                        dataField[lineValue] = dataField[lineValue] + it->second;
                    }
                }
            }
            if (words[i] == "JUMPI") {
                dataField[lineValue] = 0xC000;
                for(map<string, unsigned short>::const_iterator it = symbolTable.begin(); it != symbolTable.end(); ++it) {
                    use_facet < ctype<char> > (locale()).toupper(&words[i+1][0], &words[i+1][0] + words[i+1].size());                
                    if(words[i+1] == it->first) {
                        dataField[lineValue] = dataField[lineValue] + it->second;
                    }
                }
            }
            if (words[i] == "LOADI") {
                dataField[lineValue] = 0xD000;
                for(map<string, unsigned short>::const_iterator it = symbolTable.begin(); it != symbolTable.end(); ++it) {
                    use_facet < ctype<char> > (locale()).toupper(&words[i+1][0], &words[i+1][0] + words[i+1].size());                
                    if(words[i+1] == it->first) {
                        dataField[lineValue] = dataField[lineValue] + it->second;
                    }
                }
            }
            if (words[i] == "STOREI") {
                dataField[lineValue] = 0xE000;
                for(map<string, unsigned short>::const_iterator it = symbolTable.begin(); it != symbolTable.end(); ++it) {
                    use_facet < ctype<char> > (locale()).toupper(&words[i+1][0], &words[i+1][0] + words[i+1].size());                
                    if(words[i+1] == it->first) {
                        dataField[lineValue] = dataField[lineValue] + it->second;
                    }
                }
            }
        }
        if(words[0].substr(0, 1)=="/"||words[0] == ""){  
        } else {
            lineValue++;
        }
    }
    unsigned short finalTick = lineValue;

    if(newLine < bufferLength){
        remainder = bufferLength - newLine;
        for(int k = 0; k < remainder; k++) {
            dataField[lineValue] = 0x0000;
            lineValue++;  
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Print Symbol Table

    cout << "\n\nSymbol Table:" << endl;
    cout << "-------------" << endl;
    for(map<string, unsigned short>::const_iterator it = symbolTable.begin(); it != symbolTable.end(); ++it) {
        cout << it->first << " => " << std::hex << std::uppercase << it->second << "\n";
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Print Data Table

    unsigned short userLength;
    int lengthChoice;
    do{
        cout << "\n\nHow much of the data table would you like to print?\n[1] 256 values\n[2] 4096 values\n[3] Up to last value with data\n[4] Custom value\n ";
        cin >> lengthChoice;
    }while(lengthChoice < 1 || lengthChoice > 4);
    if(lengthChoice == 1){
        userLength = 256;
    }else if(lengthChoice == 2){
        userLength = 4096;
    }else if(lengthChoice == 3){
        userLength = finalTick;
    }else if(lengthChoice == 4){
        cout << "Length: ";
        cin >> userLength;    
        while(userLength < 1 || userLength > 4096){
            cout << "Length must be above 0 and below 4096.\nLength:";
            cin >> userLength;
        }
    }
    cout << "      | +0 || +1 || +2 || +3 || +4 || +5 || +6 || +7 || +8 || +9 || +A || +B || +C || +D || +E || +F |" << endl;
    cout << "    +-------------------------------------------------------------------------------------------------" << endl;
    newLine = 0;
    lineValue = 0;
    linecountthing = 0;
    for(int i = 0; lineValue < userLength; i++) {
        if(newLine % 16 == 0 && newLine >= 1) {
            linecountthing++;
            cout << endl << '|' << setfill('0') << setw(2) << std::hex << std::uppercase << linecountthing  << '0' << "| ";
        } else if(newLine == 0) {
            cout << "|000| ";
        }
        newLine++;
        cout << " " << std::setfill('0') << std::setw(4) << std::hex <<dataField[lineValue] << " ";
        lineValue++;
    }
    cout << endl;  

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Execute

    short AC = 0;
    short MAR = 0;
    short MBR = 0;
    short PC = starterInt;    
    short InReg = 0;
    short OutReg = 0;
    unsigned short code = 0;    
    unsigned short parameter = 0;
    unsigned short parameter2 = 0;
    unsigned short dechexoct;

    do{
        cout << "\n\nHow would you like your output to print?\n[1] Dec values\n[2] Hex values\n[3] Oct values\n[4] ASCII values\n ";
        cin >> dechexoct;
    }while(dechexoct < 1 || dechexoct > 4);

    while (PC < bufferLength){
        code = dataField[PC] >> 12;
        parameter = dataField[PC] & 0xFFF;
        PC++;

        if (code == 0 && parameter != 0) {
            MBR = PC;
            MAR = parameter;
            dataField[parameter] = MBR;
            MBR = parameter;
            AC = 1;
            AC = AC + MBR;
            PC = AC;
        }
        else if (code == 1) {
            AC = dataField[parameter];
        }
        else if (code == 2) {
            dataField[parameter] = AC;
        }
        else if (code == 3) {
            MAR = dataField[parameter];
            AC = AC + MAR;
        }
        else if (code == 4) {
            MAR = dataField[parameter];
            AC = AC - MAR;
        }
        else if (code == 5) {
            cout << "Input: ";
            cin >> InReg;
            AC = InReg;
        }
        else if (code == 6) {
            OutReg = AC;
            if(dechexoct == 1){
                cout << "Output: " << std::dec << OutReg << endl;
            }else if(dechexoct == 2){
                cout << "Output: " << std::hex << OutReg << endl;
            }else if(dechexoct == 3){
                cout << "Output: " << std::oct << OutReg << endl;
            }else if(dechexoct == 4){
                cout << "Output: " << (char) OutReg << endl;
            }
        }
        else if (code == 7) {
            cout << "Program halted normally." << endl;
            PC = bufferLength;
            exit(1);            
        }
        else if (code == 8) {
            if(parameter == 0) {
                if(AC < 0) {
                    PC = PC + 1;
                } 
            }else if(parameter == 1024) {
                if(AC == 0) {
                    PC = PC + 1;
                }
            }else if(parameter == 2048) {
                if(AC > 0) {
                    PC = PC + 1;
                }
            }
        }
        else if (code == 9) {
            PC = parameter;
        }
        else if (code == 10) {
            AC = 0;
        }
        else if (code == 11) {
            MAR = dataField[dataField[parameter]];
            AC = AC + MAR;
        }
        else if (code == 12) {
            PC = dataField[parameter] & 0xFFF;
        }
        else if (code == 13) {
            AC = dataField[dataField[parameter]];
        }
        else if (code == 14) {
            dataField[dataField[parameter]] = AC;
        }
    }
}