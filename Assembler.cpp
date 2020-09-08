// File: Assembler.cpp
// Author: Charles Dinges
// Date: 04/07/2020
// Section: 511
// E-mail: treydinges@tamu.edu
// Description: 
//      The content of this file implements Assembler.cpp

#include <iostream>

#include "functions.h"

using namespace std;

int main (int argc, char *argv[]) {
    string fileName = argv[1];

    Assembler file;

    file.convertFile(fileName);

    return 0;
}