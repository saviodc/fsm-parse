#include <map>
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
	transition(){}
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
	R, A, S, I, T, It, D, N, OBs, OBt, COLd, COLit, COLn, COLa,  COLi, fromT
};

map<string, state>all_states;
state editing;
transition editing_trans;
parse_state cur_state = R;
string seen = "";// + string(1,c)+" "   +seen debug
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
	seen+=string(1,c);
    switch(cur_state){
        case R: 
            if(c == 's') cur_state = S;
            else if(c == 'a') cur_state = A;
            else if(c == 'i') cur_state = I;
            else throw ParseError("Bad char passed to case R: " + string(1,c));
        break;
        
        case A: 
			input = "";
            if(c == '{')cur_state = COLa;
            else throw ParseError("Bad char passed to case A: " + string(1,c));
        break;
		
        case fromT:
			if(c == '}'){
				if(editing.name=="")throw ParseError("No name parsed");
				//if(editing.transitions.empty())throw ParseError("No transition(s) parsed");
				auto it = all_states.find(editing.name);  
				if (it != all_states.end()) {
					editing.accepting =it->second.accepting;
					editing.init = it->second.init;
					
				}
				all_states[editing.name] = editing;
				cur_state = R;
			}else{
				if(c == 't') cur_state = T;
				else if(c == 'n') cur_state = N;
				else throw ParseError("Bad char passed to case fromT: ");
			}
        break;
        
        case S: 
			 editing = state();
			 if(c == '{')cur_state = OBs;
			 else throw ParseError("Bad char passed to case S: " + string(1,c));
        break;
        
        case I: 
			input = "";
            if(c == '{')cur_state = COLi;
			else throw ParseError("Bad char passed to case I: " + string(1,c));
        break;
        
        case T:
			editing_trans = transition();//switched from I
			if(c == '{')cur_state = OBt;
			else throw ParseError("Bad char passed to case T: " + string(1,c));
        break;
		
        case It:
			input = "";
			if(c == '{')cur_state = COLit;
			else throw ParseError("Bad char passed to case It: " + string(1,c));
        break;
		
		
        case D: 
			input = "";
			if(c == '{')cur_state = COLd;
			else throw ParseError("Bad char passed to case D: " + string(1,c));
        break;
		
        case N:
			input = "";
			if(c == '{')cur_state = COLn;
			else throw ParseError("Bad char passed to case N: " + string(1,c));
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
				cur_state = fromT;
			}else{
				input.push_back(c);
			}
		break;
		
		case OBt:
			input ="";
			if(editing_trans.fin()){
				if(c == '}'){
					editing.add(editing_trans);
					editing_trans= transition();
					cur_state = fromT;
				}else throw ParseError("Errant } recieved when transition not poluater at OBt");
			}else{
				if(c == 'i'){
					if(editing_trans.input_found)throw ParseError("Trying to find input when already established ");
					cur_state= It;
				}else if(c == 'd'){
					if(editing_trans.destination_found)throw ParseError("Trying to find input when already established ");
					cur_state = D;
				}else throw ParseError("Bad char passed to case OBt: " + string(1,c));
			}
		break;
		
		case COLd:
			if(c =='{')throw ParseError("Errant '{' passed to case COLd");
			else if(c == '}'){
				if(input == "")throw ParseError("No args passed to case COLd");
				if(editing_trans.destination != "")throw ParseError("Attempted name overrite at COLd");
				editing_trans.set_val(input);
				input = "";
				cur_state = OBt;
			}else{
				input.push_back(c);
			}
		break;
		
		case COLit:
			if(c =='{')throw ParseError("Errant '{' passed to case COLit");
			else if(c == '}'){
				if(input == "")throw ParseError("No args passed to case COLit");
				if(editing_trans.in != "")throw ParseError("Attempted name overrite at COLit");
				editing_trans.set_key(input);
				input = "";
				cur_state = OBt;
			}else{
				input.push_back(c);
			}
		break;
	}
}

void parse_string(string str){
	for(char c : str){
		parse(c);
	}
}
string save_init, save_acc;

void checkAll(){
	bool found_init = false, found_acc = false; 
	for (auto it = all_states.begin(); it != all_states.end(); ++it) {
		if(it->first != it->second.name||it->first== "")throw ParseError("Error");
		if(it->second.init){
			if(found_init)throw ParseError("Error");
			found_init = true;
			save_init = it->first;
			cout<<"init"<<save_init;
		}
		if(it->second.accepting){
			if(found_acc)throw ParseError("Error");
			found_acc = true;
			save_acc = it->first;
			cout<<"init"<<save_acc;
		}
		for (auto sec_it = it->second.transitions.begin(); sec_it != it->second.transitions.end(); ++sec_it) {
			if(all_states.find((*sec_it).destination)==all_states.end())throw ParseError("Error");
		}
    }
    if(!found_init || !found_acc)throw ParseError("Didnt find init | acc");
    
}
void parseStates(){
	ofstream output_file("output.cpp");
	output_file<<"#include <iostream>"<<endl;//potential new line - could be additional function
	output_file<<"#include <string>"<<endl;
	output_file<<"#include <stdexcept>"<<endl;
	output_file<<"using namespace std;"<<endl;
	output_file<<"enum STATE {"<<endl;
	auto its = all_states.begin();
	output_file<<its->first;
	its++;
	for (; its != all_states.end(); ++its) {
		output_file<<(", " +its->first);
	}
	output_file<<"\n};"<<endl;
	output_file<<"STATE state = "<<save_init<<";"<<endl<<endl;
	output_file<<"void manage_state(string str){"<<endl;
	output_file<<"switch(state){"<<endl;
	
	for (auto it = all_states.begin(); it != all_states.end(); ++it) {
		output_file<<"case "<<it->first<<":"<<endl;	
		auto sec_it = it->second.transitions.begin();
		if(sec_it != it->second.transitions.end()){
			
		
		output_file<<"if(str == \""<<(*sec_it).in<<"\")";
		output_file<<"state = "<<(*sec_it).destination<<";"<<endl;
		sec_it++;
		}
		for (; sec_it != it->second.transitions.end(); ++sec_it) {
			output_file<<"else if(str == \""<<(*sec_it).in<<"\")";
			output_file<<"state = "<<(*sec_it).destination<<";"<<endl;
		}
		//output_file<<"else throw invalid_argument(\"Invalid input\");"<<endl;
		output_file<<"break;"<<endl<<endl;
	}
	
	output_file<<"}"<<endl;
	output_file<<"}"<<endl;
	
	output_file<<"int main(){"<<endl;
	output_file<<"while(1){"<<endl;
	output_file<<"string input;"<<endl;
	output_file<<"cout<<\"current state:\"<<state<<\" enter signal: \";"<<endl;
	output_file<<"cin>>input;"<<endl;
	output_file<<"manage_state(input);"<<endl;
	output_file<<"string accepted = state == "<<save_acc<<"? \"true\": \"false\";"<<endl;
	output_file<<"cout<<\" new state= \"<<state<<\"  accepted: \"<<accepted<<endl;"<<endl;
	output_file<<"}"<<endl<<"}"<<endl;
	output_file.close();
}

int main(){
	string fromFile = ReadSpecFile("test.txt");//getString());
	cout<<fromFile;
	parse_string(fromFile);
	checkAll();
	parseStates();
	return 0;
}
