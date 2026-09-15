#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <vector>

std::unordered_map <std::string, int> builtins{
  {"type", 1},
  {"exit", 1},
  {"echo", 1}
};

void type(std::string argument, std::vector<std::string> envDirectories){
  if(builtins[argument] == 1) std::cout<<argument<<" is a shell builtin"<<"\n";
  else{
    bool flag = false;

    for(auto i: envDirectories){

      if(flag) break;

      std::filesystem::directory_iterator itr (i);
      for(auto j: itr){
        std::filesystem::perms p = j.status().permissions();

        std::filesystem::perms check = std::filesystem::perms::owner_exec |
        std::filesystem::perms::group_exec |
        std::filesystem::perms:: others_exec;

        if(p & check){
          std::cout<<argument<<" is "<<j.path()<<"\n";
          flag = true;
          break;
        }

      }
    }

    if(!flag) std::cout<<argument<<": not found"<<"\n";
  }
}

void echo(std::string argument){
    std::cout<<argument<<"\n";
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string path = std::getenv("PATH");
  std::vector<std::string> envDirectories;
  std::string dir = "";
  for(auto i: path){
    if(i == ':'){
      envDirectories.push_back(dir);
      dir = "";
    }else{
      dir += i;
    }
  }
  envDirectories.push_back(dir);

  //Components of command line
  std::string commandLine;
  std::string command;
  std::string argument;

  
  while(true){

    std::cout<<"$ ";
    commandLine = "";
    std::getline(std::cin, commandLine);

    //seperating command and argument from commandline
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
    else if(command == "type") type(argument, envDirectories);
    else std::cout<<command<<": command not found"<<"\n";
  }
}
