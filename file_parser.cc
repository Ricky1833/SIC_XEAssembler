/*  
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    file_parser.cc
    CS530, Spring 2014
*/

#include "file_parser.h"
#include "file_parse_exception.h"

using namespace std;

//constructor
file_parser::file_parser(string file_name) {
    in_file_name = file_name;
    v_counter=0;
    contents.reserve(500);
}

//destructor
file_parser::~file_parser() {
}

void file_parser::read_file() {
//parsing code and storage into vector<struct>
    infile.open(in_file_name.c_str(), std::ifstream::in);
    if (!infile) {
    	throw file_parse_exception("source code file does not exist.");
    }
	
    while (!infile.eof()) {
        getline(infile, line);
        int operand_set=0; //Checks if operand has been tokenized
        int opcode_set=0; //Checks if opcode has been tokenized
        int had_single_quote=0; //Checks if a single quote has been processed
        stringstream ss_error;
        string comment,label,opcode,operand;
        contents.push_back(parsed_line()); //Creates each row
        for(unsigned int i=0; i< line.size();i++){
	    if(is_comment(line,i)){
                comment = line.substr(i,line.size()-i);
                contents.at(v_counter).comment = comment;                
                break;
            }
            if(is_label(line,i)){
	    	if(!isalpha(line[i])){
		    throw_error(" 'Labels cannot start with a number'",ss_error);
		}
                while(!isspace(line[i])){
		    //Check to make sure contents are only alphanumeric or the null character
		    if(!isalnum(line[i])&&line[i]!=0){
		        throw_error(" 'Labels can only contain letters and numbers'",ss_error);
		    }
		    i++;
		}
		if(i>8){
                    label = line.substr(0,8);
                    contents.at(v_counter).label = label;
		    continue;
		}
                label = line.substr(0,i);
                contents.at(v_counter).label = label;
                continue;
            }
            if(is_opcode(line,i,opcode_set)){
                int start = i;
                while(!isspace(line[i]) && line[i]!='.'){
                    i++;                                
                }
                opcode = line.substr(start,i-start);
                contents.at(v_counter).opcode = opcode;
                opcode_set=1;
                //reverse and let comment case collect rest of line
                if(line[i]=='.')i--;
                continue;
            }
            if(is_operand(line,i,opcode_set,operand_set)){
                int start = i;
                while(line[i]!='.' && (!isspace(line[i])||(had_single_quote==1))){
                    if(line[i]=='\''){
                        had_single_quote++;
                    }
                    i++;               
                }
                operand = line.substr(start,i-start);
                contents.at(v_counter).operand = operand;
                operand_set=1;
                //reverse and let comment case collect rest of line
                if(line[i]=='.')i--;
                continue;
            }
            if(isspace(line[i])){
                continue;
            }
            if(has_too_many_tokens(line,i)){
	    	throw_error(" 'Too Many Tokens'",ss_error);
            }
        }
        v_counter++;
    }
    infile.close(); 
}

string file_parser::get_token(unsigned int r, unsigned int c) {
//gets the token located at
    unsigned int column = c;
    unsigned int row = r;
    string token;
    //Prevents possible segmentation fault if wrong row selected
    if(contents.size()-1 < row){
        stringstream ss_error;
        ss_error<<"no such row: "<<row<<" in file "<<in_file_name;
        throw file_parse_exception( ss_error.str());
    }

    switch (column) {
    	case 0:
		return contents.at(row).label;
	case 1:
		return contents.at(row).opcode;
	case 2:
		return contents.at(row).operand;
	case 3:
		return contents.at(row).comment;
	default:
		return "";
    }
}

void file_parser::print_file() {
//prints the input file in proper format
    for (int i = 0; i < v_counter; i++) {
	cout << contents.at(i).label << "\t";
	cout << contents.at(i).opcode << "\t";
	cout << contents.at(i).operand << "\t";
	cout << contents.at(i).comment << endl;
    }
}

int file_parser::size() {
//returns the number of lines in the source code file
    return contents.size();
}

 
 //Creates and throws a file_parse_exception
 void file_parser::throw_error(string error, stringstream& stream){
    stream<<"at line: "<<v_counter+1<<", in file "<<in_file_name<<error;
    throw file_parse_exception(stream.str());
 }
 
 //Checks if character is start of a comment
 int file_parser::is_comment(string line, int i){
    if(line[i] == '.'){
        return 1;
    }
    return 0;
 }
 
 //Checks if character should be an opcode
 int file_parser::is_opcode(string line, int i, int opcode_set){
    if((!isspace(line[i]))&&(!opcode_set)){
        return 1;
    }
    return 0;
 }
 
 //Checks if character should be an operand
 int file_parser::is_operand(string line,int i,int opcode_set,int operand_set){
    if((!isspace(line[i]))&&(opcode_set)&&(!operand_set)){
        return 1;
    }
    return 0;
 }
 
 //Checks if character is start of a label
 int file_parser::is_label(string line, int i){
    if((!isspace(line[i])) && (i==0)){
        return 1;
    }
    return 0;
 }
 
 //Checks if the line has too many tokens
 int file_parser::has_too_many_tokens(string line, int i){
    if((!is_comment(line,i))&&(!isspace(line[i]))){
        return 1;
    }
    return 0;
 }

