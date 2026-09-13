#include <iostream>
#include <string>

void echo(std::string argument){
    cout<<argument<<"\n";
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
    std::getline(std::cin, commandLine);

    int size = command.length();
    int index = 0;

    while(commandLine[index] != ' '){
      command += commandLine[index++];
    }

    index++;
    while(index < size){
      argument += commandLine[index++];
    }



    if(command == "exit") break;
    if(command == "echo") echo(argument);
  }
}
