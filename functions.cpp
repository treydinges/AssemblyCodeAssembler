// File: functions.cpp
// Author: Charles Dinges
// Date: 04/07/2020
// Section: 511
// E-mail: treydinges@tamu.edu
// Description: 
//      The content of this file implements functions.cpp

#include "functions.h"
#include <sstream>
#include <fstream>

void Assembler::convertFile(string fileName) {
    ifstream ifs(fileName);
	if (!ifs.is_open()) { // error check filename
		cout << "Unable to open file: " << fileName << endl;
		return;
	}

    // this parser will first find all of the (VARIABLES) in the file and create
    // addresses for them corresponding to the line in the file and add the rest of
    // the instructions to a private vector data member
    int lineNumber = 0;
    bool fileStarted = false;
	while (true) {
		string line;
		getline(ifs, line);

        // removes whitespace before and after the instruction
        // will also delete comments at the end of lines
        line = strip(line);

        // looks for the first line in the file and then starts counting lines
        if (line[0] == '@') {
            fileStarted = true;
        }

        // if it finds a (VARIABLE) will remove the () and add the name to a map of 
        // A-Instruction Symbols
        // else if it finds a line that is not started by a comment, it pushes the
        // instruction to the vector "instructions"
		if (line[0] == '(') { 
            line.erase(0,1);
            line.erase(line.length()-1, 1);
            addSymbol(line, lineNumber);
            lineNumber--;
        }
        else if (line.length() != 0) {
            if (line[0] != '/' && line[1] != '/') {
                instructions.push_back(line);
            }
		}

        // check for end of file
		if (ifs.eof()) {
			break;
		}

        // only count lines if we are past the fist line of the file
        if (fileStarted) {
            lineNumber++;
        }
	}

    // convert the instructions in the vector now that all symbols have been added
    convertInstructions();
    writeToFile(fileName);
}

void Assembler::convertInstructions() {
    // iterate through the vector and process each instruction
    for (int i = 0; i < instructions.size(); i++) {
        processInstruction(instructions[i]);
    }
}

void Assembler::processInstruction(string instruction) {
    if (instruction[0] == '@') {
        // delete the "@" in the A instruction
        instruction.erase(0,1);
        convertAInstruction(instruction);
    }
    else {
       convertCInstruction(instruction);
    }
}

void Assembler::convertAInstruction(string instruction) {
    int decimal;
    string binaryInstruction = "0000000000000000";

    // instruction = strip(instruction);

    // if the first char in the string is a digit, we will convert the whole thing to an int
    // else we will find the corresponding address for that variable name
    if (isdigit(instruction[0])) {
        decimal = std::stoi(instruction);
    }
    else {
        decimal = getAddress(instruction);
    }

    // converts the number "decimal" to binary digit by applying x mod 2
    // then that result is saved as the least significant bit of the instruction
    // then integer division is preformed on the number
    int i = 0;
    while (decimal > 0) {
        int digit = decimal % 2;

        int index = (binaryInstruction.length() - 1) - i;
        if (digit == 1) {
            binaryInstruction[index] = '1';
        }

        decimal = decimal / 2;
        i++;
    }

    // after the instruction has been completed, push it to the vector of binary instructions
    binaryInstructions.push_back(binaryInstruction);
}

void Assembler::convertCInstruction(string instruction) {
    // all c instructions will start with 111
    string binaryInstruction = "111";
    istringstream ss(instruction);

    string dest;
    string comp;
    string jump;

    bool isdest = false;
    bool isjump = false;

    // since dest can be omitted, we want to look for an '=' in the instruction
    for (int i = 0; i < instruction.length(); i++) {
        if (instruction[i] == '=') {
            isdest = true;
        }
    }

    // if an '=' was found, take everything up to that as dest
    if (isdest) {
        getline(ss, dest, '=');
    }

    // comp will never be omitted so we take everything to the ';'
    getline(ss, comp, ';');

    // we can take jump as everything else in the string, if it is omitted,
    // this string will be empty
    getline(ss, jump, ' ');

    // if the jump string is not empty, set flag to true
    if (jump.length() != 0) {
        isjump = true;
    }

    // find the corresponding values for each piece of the instruction
    // an empty string corresponds to "000"
    string destb;
    string compb = compSymbolTable[comp];
    string jumpb;

    if (isdest) {
        destb = destSymbolTable[dest];
    }
    else {
        destb = "000";
    }

    if (isjump) {
        jumpb = jumpSymbolTable[jump];
    }
    else {
        jumpb = "000";
    }

    // append the different pieces of the instruction to the binary
    // instruction string in this specific order
    binaryInstruction.append(compb);
    binaryInstruction.append(destb);
    binaryInstruction.append(jumpb);

    // after the instruction has been completed, push it to the vector of binary instructions
    binaryInstructions.push_back(binaryInstruction);
}

string Assembler::strip(string line) {
    // remove whitespace before the instruction
    while(true) {
        if (line[0] == ' ') {
            line.erase(0,1);
        }
        else {
            break;
        }
    }

    // remove everything starting at the next space
    // this implementation is assuming the writer of the .asm file
    // did not put spaces in between the dest=comp;jump instruction
    istringstream ss(line);
    getline(ss, line, ' ');
    return line;
}

int Assembler::getAddress(string symbol) {
    // if the table does not contain the symbol add it using the next availiable address
    // this value starts at 16 and will count up by one each time it is used
    if (!contains(symbol)) {
        addSymbol(symbol, nextAvailiableAddress);
        nextAvailiableAddress++;
    }
    return AInstSymbolTable[symbol];
}

void Assembler::addSymbol(string symbol, int address) {
    // create a pair and add it to the symbol table
    AInstSymbolTable.insert(pair<string, int>(symbol, address));
}

bool Assembler::contains(string symbol){
    // if the symbol is found the find symbol will not be at the end
    // of the table and this will return true
    return AInstSymbolTable.find(symbol) != AInstSymbolTable.end();
}

void Assembler::writeToFile(string fileName) {
    // take the "asm" off of the end of the file and add "hack"
    fileName.erase(fileName.length()-3, fileName.length());
    fileName.append("hack");

    // write each instruction in the binary instruction vector to the file
    ofstream outFile;
    outFile.open(fileName);
    for (int i = 0; i < binaryInstructions.size(); i++) {
        outFile << binaryInstructions[i] << endl;
    }
    outFile.close();
}