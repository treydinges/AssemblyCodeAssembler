// File: functions.h
// Author: Charles Dinges
// Date: 04/07/2020
// Section: 511
// E-mail: treydinges@tamu.edu
// Description: 
//      The content of this file implements functions.h

#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Assembler {
    public:
        void convertFile(string fileName);
        void convertInstructions();
        void processInstruction(string instruction);
        void convertAInstruction(string instruction);
        void convertCInstruction(string instruction);
        string strip(string commentLine);
        int getAddress(string symbol);
        void addSymbol(string symbol, int address);
        bool contains(string symbol);
        void writeToFile(string fileName);
    
    private:
        vector<string> instructions;
        vector<string> binaryInstructions;
        int nextAvailiableAddress = 16;
        map<string, int> AInstSymbolTable = {{"SP",   0},       {"LCL",  1},
                                             {"ARG",  2},       {"THIS", 3},
                                             {"THAT", 4},
                                             {"R0",   0},       {"R1",   1},
                                             {"R2",   2},       {"R3",   3},
                                             {"R4",   4},       {"R5",   5},
                                             {"R6",   6},       {"R7",   7},
                                             {"R8",   7},       {"R9",   9},
                                             {"R10", 10},       {"R11", 11},
                                             {"R12", 12},       {"R13", 13},
                                             {"R14", 14},       {"R15", 15},
                                             {"SCREEN", 16384}, {"KBD", 24576}};

        map<string, string> compSymbolTable = {{"0",   "0101010"}, {"1",  "0111111"},
                                              {"-1",  "0111010"}, {"D",   "0001100"},
                                              {"A",   "0110000"}, {"!D",  "0001101"},
                                              {"!A",  "0110001"}, {"-D",  "0001111"},
                                              {"-A",  "0110011"}, {"D+1", "0011111"},
                                              {"A+1", "0110111"}, {"D-1", "0001110"},
                                              {"A-1", "0110010"}, {"D+A", "0000010"},
                                              {"D-A", "0010011"}, {"A-D", "0000111"},
                                              {"D&A", "0000000"}, {"D|A", "0010101"},
                                              {"M",   "1110000"}, {"!M",  "1110001"},
                                              {"-M",  "1110011"}, {"M+1", "1110111"},
                                              {"M-1", "1110010"}, {"D+M", "1000010"},
                                              {"D-M", "1010011"}, {"M-D", "1000111"},
                                              {"D&M", "1000000"}, {"D|M", "1010101"}};

        map<string, string> destSymbolTable = {{"M",   "001"},
                                              {"D",  "010"}, {"MD",  "011"},
                                              {"A",  "100"}, {"AM",  "101"},
                                              {"AD", "110"}, {"AMD", "111"}};
        
        map<string, string> jumpSymbolTable = {{"JGT", "001"},
                                              {"JEQ", "010"}, {"JGE", "011"},
                                              {"JLT", "100"}, {"JNE", "101"},
                                              {"JLE", "110"}, {"JMP", "111"}};
};