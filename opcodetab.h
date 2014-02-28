/*
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    opcodetab.h
    CS530, Spring 2014
*/

#ifndef OPCODETAB_H
#define OPCODETAB_H

#include <string>
#include <cstdlib>
#include <iostream>
#include <map>

using namespace std;

class opcodetab {
    public:
        // ctor
        // creates a new dictionary structure and loads all of the opcodes for 
        // the SIC/XE architecture into the table.  Use the STL map for this.
        opcodetab(); 
        
        // takes a SIC/XE opcode and returns the machine code 
        // equivalent as a two byte string in hexadecimal.
        // Example:  get_machine_code("ADD") returns the value 18
        // Note that opcodes may be prepended with a '+'.
        // throws an opcode_error_exception if the opcode is not 
        // found in the table.
        string get_machine_code(string);  
        
        // takes a SIC/XE opcode and returns the number of bytes 
        // needed to encode the instruction, which is an int in
        // the range 1..4.
        // NOTE: the opcode must be prepended with a '+' for format 4.
        // throws an opcode_error_exception if the opcode is not 
        // found in the table.        
        int get_instruction_size(string);
                        
    private:
        // your variables and private methods go here
	static string codes[59];
	static string hex_codes[59];
	static string format[59];
	map<string, pair<string, string> > opcode_map;
	map<string, pair<string, string> >::iterator m_iter;
	
	bool opcode_exists(string);
};

#endif
