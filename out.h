#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>



/*int SaveCodeFile(std::string fileName){
  std::ofstream codeOutFile( fileName );
  // write whole code to the file
   for( unsigned int i = 0 ; i < code.size(); i++ ) {
      //cout<<code.at(i)<<endl;
      codeOutFile<<code.at(i)<<std::endl;
    } 
    codeOutFile.close();
    return 0;
}*/

// returns whole specification file as one string
// whitespaces and line ending characters are removed
std::string ReadSpecFile(std::string fileName){
  std::cout<<"Reading specifications file "<<fileName<<std::endl;
  std::string out;
  std::ifstream inputFile( fileName );
  std::string line;
  out = "";
  std::string n1;
  while(getline(inputFile,line)) { 
   //std::cout<<"from file="<<line<<std::endl;
   // remove all new line characters;
   line.erase( std::remove(line.begin(), line.end(),'\r'), line.end() );
   line.erase( std::remove(line.begin(), line.end(),'\n'), line.end() );
   line.erase( std::remove(line.begin(), line.end(),' '), line.end() );
   out.append(line);
  }
  //cout<<" input="<< input<<endl;
  return out;
}

struct Transition{
  std::string input_signal;
  std::string dest_state;
};
struct State{
  std::string name;
  std::vector<Transition> transitions;
};
struct FSM_out{
   std::vector<State> states;
   std::string accepting_name;
   std::string initial_name;
   State state_in_progress;
};
