#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <vector>
#include <sstream>
#include <utility>
#include <functional>

std::unordered_map <std::string, int> builtins{
  {"echo", 1},
  {"type", 1},
  {"exit", 1},
  {"pwd", 1},
  {"cd", 1}
};

std::vector<std::string> envDirectories = []() {
  
  std::string paths = std::getenv("PATH");

  std::istringstream pathStream (paths);

  std::vector<std::string> directories;

  std::string dir;
  while(std::getline(pathStream, dir, ':')){
    directories.push_back(dir);
  }

  return directories;
}();

std::filesystem::path programFinder(std::string program){
  for(std::string i: envDirectories){
    std::filesystem::directory_iterator itr (i);

    for(std::filesystem::directory_entry j: itr){
      if(j.path().stem() != program) continue;

      std::filesystem::perms p = j.status().permissions();

      std::filesystem::perms check = std::filesystem::perms::owner_exec |
        std::filesystem::perms::group_exec |
        std::filesystem::perms::others_exec;
      
      if((p & check) != std::filesystem::perms::none) return j.path();
    }
  }

  return std::filesystem::path();
}

void type(std::string argument){
  if(builtins[argument]) std::cout<<argument<<" is a shell builtin"<<"\n";
  else{
    std::filesystem::path programPath = programFinder(argument);
    if(!programPath.empty()) std::cout<<argument<<" is "<<programPath.string()<<"\n";
    else std::cout<<argument<<": not found"<<"\n";
  }
}

bool run(std::string program, std::string argument){
  std::filesystem::path programPath = programFinder(program);

  if(programPath.empty()) return false;

  std::string systemCmd = "\"" + programPath.filename().string() + "\" " + argument;

  system(systemCmd.c_str());

  return true;
}

void echo(std::string argument){
    std::cout<<argument<<"\n";
}

void pwd(std::string argument){
  if(!argument.empty()) {
    //logic to avoid arguments for argumentless commands
  }
  std::cout<<std::filesystem::current_path().string()<<"\n";
}

void cd(std::string argument){

  if(argument == "~"){
    std::filesystem::path p (std::getenv("HOME"));
    std::filesystem::current_path(p);
  }else{
    std::filesystem::directory_entry p (argument);

    if(p.exists())std::filesystem::current_path(p.path());
    else std::cout<<"cd: " + argument + ": No such file or directory"<<"\n";
  }

}

std::unordered_map<std::string, std::function<void(std::string)>> invoker {
  {"type", type},
  {"echo", echo},
  {"pwd", pwd},
  {"cd", cd}
};

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
    commandLine = "";
    std::getline(std::cin, commandLine);

    // seperating command and argument from commandline
    int size = commandLine.length();
    int index = 0;

    command = "";
    while(commandLine[index] != ' ' && index < size){
      command += commandLine[index++];
    }

    argument = "";
    index++;
    while(index < size){
      if(commandLine == "\'"){
        argument += "\'";
        index++;
        continue;
      }
      argument += commandLine[index++];
    }

    if(command == "exit") break;

    std::unordered_map<std::string, std::function<void(std::string)>>::iterator it = invoker.find(command);
    if(it != invoker.end()){
      it->second(argument);
    }
    else{
      if(!run(command, argument))
        std::cout<<command<<": command not found"<<"\n";
    }
  }
}
