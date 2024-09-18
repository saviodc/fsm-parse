#include <map>
#include <functional>
#include <stack>
#include "labfunctions.h"
#include "out.h"
using namespace std;

struct transition{
	string in= "";
	string destination= "";
	
	bool input_found = false, destination_found= false;
	bool fin(){
		return input_found && destination_found;
	}
	transition(string k, string v){in = k; destination = v;}
	transition();
	void set_key(string s){in = s;input_found=true;}
	void set_val(string s){destination = s; destination_found = true;} 
};

struct state {
    string name;
    vector<transition> transitions;
	bool accepting = false, init = false;
	
    state(string n) : name(n) {}
	state(){}
    void add(transition tr) {
        transitions.push_back(tr);
    }

    //state(const state&) = delete;
};


enum parse_state{
	R, A, S, I, T, It, D, N, OBs, OBt, OBd, COLd,OBit, COLit, OBn, COLn,OBa, COLa, OBi, COLi
};

map<string, state>all_states;
state editing;
transition editing_trans;
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

string input;
void parse(char c){
    switch(cur_state){
        case R: 
            if(c == 's') cur_state = S;
            else if(c == 'a') cur_state = A;
            else if(c == 'i') cur_state = I;
            else throw ParseError("Bad char passed to case R: " + string(1,c));
        break;
        
        case A: 
            if(c == '{')cur_state = OBa;
            else throw ParseError("Bad char passed to case A: " + string(1,c));
        break;
        
        case S: 
			 editing = state();
			 if(c == '{')cur_state = OBs;
			 else throw ParseError("Bad char passed to case S: " + string(1,c));
        break;
        
        case I: 
			editing_trans = transition();
            if(c == '{')cur_state = OBi;
			else throw ParseError("Bad char passed to case I: " + string(1,c));
        break;
        
        case T:
			if(c == '{')cur_state = OBt;
			else throw ParseError("Bad char passed to case T: " + string(1,c));
        break;
		
        case It:
			if(c == '{')cur_state = OBit;
			else throw ParseError("Bad char passed to case It: " + string(1,c));
        break;
		
        case D: 
			if(c == '{')cur_state = OBd;
			else throw ParseError("Bad char passed to case D: " + string(1,c));
        break;
		
        case N:
			if(c == '{')cur_state = OBn;
			else throw ParseError("Bad char passed to case N: " + string(1,c));
        break;
        
        case OBa:
			input = "";
			if(c != '}' || c != '}') cur_state = COLa;
			else throw ParseError("Bad char passed to case OBa: " + string(1,c));
		break;
		
		case OBi:
			input = "";
			if(c != '}' || c != '}') cur_state = COLi;
			else throw ParseError("Bad char passed to case OBi: " + string(1,c));
		break;
		
		case OBit:
			input = "";
			if(c != '}' || c != '}') cur_state = COLit;
			else throw ParseError("Bad char passed to case OBit: " + string(1,c));
		break;
		
		case OBd:
			input = "";
			if(c != '}' || c != '}') cur_state = COLd;
			else throw ParseError("Bad char passed to case OBd: " + string(1,c));
		break;
		
		case OBn:
			input = "";
			if(c != '}' || c != '}') cur_state = COLn;
			else throw ParseError("Bad char passed to case OBn: " + string(1,c));
		break;
		
		case OBs:
			if(c == 't') cur_state = T;
			else if(c == 'n') cur_state = N;
			else throw ParseError("Bad char passed to case OBs: " + string(1,c));
		break;
		
		case COLi:
			if(c =='{')throw ParseError("Errant '{' passed to case COLi");
			else if(c == '}'){
				if(input == "")throw ParseError("No args passed to case COLi");
				auto it = all_states.find(input);  
				if (it != all_states.end()) {
					it->second.init = true;
				} else {
					state to_add(input);  
					to_add.init = true;  
					all_states[input] = to_add;             
				}
				cur_state = R;
			}else{
				input.push_back(c);
			}
		break;
		
		case COLa:
			if(c =='{')throw ParseError("Errant '{' passed to case COLi");
			else if(c == '}'){
				if(input == "")throw ParseError("No args passed to case COLi");
				auto it = all_states.find(input);  
				if (it != all_states.end()) {
					it->second.accepting = true;
				} else {
					state to_add(input);  
					to_add.accepting = true;  
					all_states[input] = to_add;             
				}
				cur_state = R;
			}else{
				input.push_back(c);
			}
		break;
		
		case COLn:
			if(c =='{')throw ParseError("Errant '{' passed to case COLn");
			else if(c == '}'){
				if(input == "")throw ParseError("No args passed to case COLn");
				if(editing.name != "")throw ParseError("Attempted name overrite at COLn");
				editing.name = input;
				input = "";
				cur_state = OBs;
			}else{
				input.push_back(c);
			}
		break;
		
		case OBt:
			if(editing_trans.fin()){
				if(c == '}'){
					editing.add(editing_trans);
				}else throw ParseError("Errant } recieved when transition not poluater at OBt");
			}else{
				if(c == 'i'){
					//if(editing_trans.
					cur_state= I;
				}else if(c == 'd'){
					cur_state = D;
				}
			}
		break;
		
	}
}

void parse_string(string str){
	for(char c : str){
		parse(c);
	}
}


int main(){
	string fromFile = ReadSpecFile(getString());
	cout<<fromFile;
	parse_string(fromFile);
	return 0;
}
