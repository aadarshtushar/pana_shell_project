#include <iostream>
#include <string>

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
    if(command == "echo") echo(argument);
    else std::cout<<commandLine<<": command not found"<<"\n";
  }
}
