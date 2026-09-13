#include <iostream>
#include <string>
#include <unordered_map>

std::unordered_map <std::string, int> builtins{
  {"type", 1},
  {"exit", 1},
  {"echo", 1}
};

void type(std::string argument){
  if(builtins[argument] == 1) std::cout<<argument<<" is a shell builtin"<<"\n";
  else std::cout<<argument<<": not found"<<"\n";
}

void echo(std::string argument){
    std::cout<<argument<<"\n";
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // TODO: Uncomment the code below to pass the first stage
  std::string commandLine;
  std::string command;
  std::string argument;
  
  while(true){
    std::cout<<"$ ";
    commandLine = "";
    std::getline(std::cin, commandLine);

    int size = commandLine.length();
    int index = 0;

    command = "";
    while(commandLine[index] != ' ' && index < size){
      command += commandLine[index++];
    }

    index++;
    argument = "";
    while(index < size){
      argument += commandLine[index++];
    }

    if(command == "exit") break;
    else if(command == "echo") echo(argument);
    else if(command == "type") type(argument);
    else std::cout<<command<<": command not found"<<"\n";
  }
}
