#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <fstream>
#include "labfunctions.h"
using namespace std;

struct state {
    string name;
    map<string, state*> transitions; 
	bool accepting = false;
	
    state(string n) : name(n) {}

    void add(const string& k, state* v) {
        transitions.emplace(k,v); 
    }

    state(const state&) = delete;
    
    state* transition(string s){
		return transitions.at(s);
	}
};



string read_spec_file(string filename){
  cout<<"Reading specifications file "<<filename<<endl;
  string out;
  ifstream inputFile(filename);
  string line;
  out = "";
  string n1;
  while(getline(inputFile,line)) { 
     //std::cout<<"from file="<<line<<std::endl;
     // remove all new line characters;
     line.erase( remove(line. begin(), line.end(),'\r'), line.end() );
     line.erase( remove(line. begin(), line.end(),'\n'), line.end() );
     line.erase( remove(line. begin(), line.end(),' '), line.end() );
     out.append(line);
  }
  //cout<<" input="<< input<<endl;
  return out;
}

void parse_string(string input){
	

}


int main(){
	string fromFile = read_spec_file(getString());
	return 0;
}
