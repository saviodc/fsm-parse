#include <map>
#include <functional>
#include <stack>
#include "labfunctions.h"
#include "out.h"
using namespace std;

struct state {
    string name;
    map<string, state*> transitions; 
	bool accepting = false, init = false;
	
    state(string n) : name(n) {}
	state(){}
    void add(const string& k, state* v) {
        transitions[k] = v; 
    }

    state(const state&) = delete;
    
    state* transition(string s){
		return transitions.at(s);
	}
};
enum parse_state{
	R, A, S, I, T, It, D, N
};

vector<string> code;
map<string, state>all_states;
stack<string> braces;//checks if braces are closed 
state editing;
int i = 0;
string input;
string transition_key;
parse_state cur_state = R;

class ParseError : public std::exception {
private:
    std::string message; 

public:
    ParseError(const std::string& cause) : message(cause) {}

    const char* what() const noexcept override {
        return message.c_str();  // Convert the string to a C-style string
    }
};

void parse(string str){
    switch(cur_state){
        case R: {
            if(str.length() != 1) throw ParseError("Illegal amount of args @ R: " + str);
            char c = str[0];
            if(c == 's') cur_state = S;
            else if(c == 'a') cur_state = A;
            else if(c == 'i') cur_state = I;
            else throw ParseError("Bad char passed to case R: " + str);
            if(input[i++] != '{') throw ParseError("Missing { @ R:");
            break;
        }
        case A: {
            auto it = all_states.find(str);  // Use str instead of "5"
            if (it != all_states.end()) {
                it->second.accepting = true;
            } else {
                state to_add(str);  // Use parentheses instead of equals
                to_add.accepting = true;  // Set the accepting state
                all_states[str] = to_add;             
             }
              if(input[i++] != '}') throw ParseError("Missing } @ A:");
              cur_state = R;
            break;
        }
        case S: {
			 if(str.length() != 1) throw ParseError("Illegal amount of args @ R: " + str);
			 editing = state();
			 char c = str[0];
			 if(c == 't') cur_state = S;
			 else if(c == 'n') cur_state = A;
             else throw ParseError("Bad char passed to case S: " + str);
			 if(input[i++] != '{') throw ParseError("Missing { @ S:");
             break;
        }
        case I: {
            auto it = all_states.find(str);
            if (it != all_states.end()) {
                it->second.init = true;
            } else {
                state to_add(str);
                to_add.init = true;
                all_states[str] = to_add;             
            }
            if(input[i++] != '}') throw ParseError("Missing } @ I:");
            cur_state = R;
            break;
        }
        case T:{
			if(str.length() != 1) throw ParseError("Illegal amount of args @ R: " + str);
			char c = str[0];
			if(c == 'i') cur_state = It;
			else throw ParseError("Bad char passed to case S: " + str);
			if(input[i++] != '{') throw ParseError("Missing { @ S:");
		}
        case It:{
			transition_key = str;
			if(input[i++] != '}') throw ParseError("Missing } @ It:");
			if(input[i++] != 'd') throw ParseError("Missing d @ It:");
			if(input[i++] != '{') throw ParseError("Missing { @ It:");
			cur_state = D;
			break;
		}
        case D: {
			auto it = all_states.find(str);
            if (it != all_states.end()) {
				editing.add(transition_key, &(it->first));
            } else {
                editing.name = str;
                all_states[str] = editing;   
                
            }
            if(input[i++] != '{') throw ParseError("Missing { @ I:");
            cur_state = R;
			break;
		}
        case N:
            break;
    }
}

void parse_string(){
	parse(string(1,input[i++]));
	while(i < input.length()){
		string cur = "";
		//char to_process = input[i++];
		//to_process = input[i++];
		while(input[i]!= '{'||input[i] != '}'){
			cur.push_back(input[i++]);
		}
		//if(to_process != '}')throw ParseError("Expected {, found :" + to_process);
		parse(cur);
	}
}


int main(){
	string fromFile = ReadSpecFile(getString());
	cout<<fromFile;
	parse_string();
	return 0;
}
