#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <vector>
#include <sstream>
#include <utility>

std::unordered_map <std::string, std::function<void(std::string)>> builtins{
  {"echo", echo},
  {"type", type},
  {"exit", exit}
};

void exit(std:: string argument){
  return;
}

std::vector<std::string> envDirectories = []() {
  
  std::string paths = std::getenv("PATH");

  std::istringstream pathStream (paths);

  std::vector<std::string> directories;

  std::string dir;
  while(std::getline(ss, dir, ':')){
    directories.push_back(dir);
  }

  return directories;
}

void type(std::string argument){
  if(builtins.find(argument) != builtins.end()) std::cout<<argument<<" is a shell builtin"<<"\n";
  else{

    for(std::string i: envDirectories){
      std::filesystem::directory_iterator itr (i);

      for(std::filesystem::directory_entry j: i){
        if(j.path().stem() != argument) continue;

        std::filesystem::perms p = j.status().permissions();

        std::filesystem::perms check = std::filesystem::perms::owner_exec |
          std::filesystem::perms::group_exec |
          std::filesystem::perms::others_exec;
        
        if((p & check) != std::filesystem::perms::none){
          std::string filepath = j.path();
          std::cout<<argument<<" is "<<filepath<<"\n";
          return;
        }
      }
    }

    std::cout<<argument<<": not found"<<"\n";
  }
}

void echo(std::string argument){
    std::cout<<argument<<"\n";
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Components of command line
  std::string commandLine;
  std::string command;
  std::string argument;

  
  while(true){

    std::cout<<"$ ";
    std::getline(std::cin, commandLine);

    // seperating command and argument from commandline
    int size = commandLine.length();
    int index = 0;

    while(commandLine[index] != ' ' && index < size){
      command += commandLine[index++];
    }

    index++;
    while(index < size){
      argument += commandLine[index++];
    }

    if(command == "exit") break;

    std::pair<std::string, std::function<void(std::string)>>::iterator it = builtins.find(command);
    if(it != builtins.end()){
      it->second(argument);
    }else{
      std::cout<<command<<": command not found"<<"\n";
    }
  }
}
