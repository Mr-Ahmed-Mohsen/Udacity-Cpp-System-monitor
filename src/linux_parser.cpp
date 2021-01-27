#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{
  string line;
  string key;
  float value;
  vector<float> MemElements;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    for (int loop = 0; loop<3 ;loop++)
    {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> key >> value;
      MemElements.push_back(value);
    }
  }
   return ((MemElements.at(2)-MemElements.at(1)-MemElements.at(0)) / MemElements.at(2)) ; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  
  string system, idle;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {

      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> system >> idle;      
    }
  
  return std::stol(system); 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  
  std::vector<string> time;
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        linestream >> time.push_back();
        if ( time.size() == 16 ) {break;}        
    } 
  }
  
  return stol(time.at(13)) + stol(time.at(14)) + stol(time.at(15)) + stol(time.at(16)) ; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization() { 
  long long total, total_idle, total_notidle;
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string cpu, line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {

      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;     
    }
  
  total_idle = idle + iowait;
  total_notidle = user + nice + system + irq + softirq + steal + guest + guest_nice;
  total = total_idle + total_notidle;

  return (total - total_idle) / total ;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  
  int processes;
  string line, key;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {

        std::istringstream linestream(line);
        linestream >> key >> processes;
        if ( key == "processes") {break;}        
    }      
  }  
  return processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int procs_running;
  string line, key;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {

        std::istringstream linestream(line);
        linestream >> key >> procs_running;
        if ( key == "procs_running") {break;}        
    }      
  }  
  return procs_running;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
    if (filestream.is_open()) {
      std::getline(filestream, line);     
    }
   return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  int memUsage;
  string line, key;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {

        std::istringstream linestream(line);
        linestream >> key >> memUsage;
        if ( key == "VmSize:") {break;}        
    }  
  }
  return std::to_string(memUsage); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  int uid;
  string line, key;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {

        std::istringstream linestream(line);
        linestream >> key >> uid;
        if ( key == "Uid:") {break;}        
    } 
  }
 return std::to_string(uid); 
}
// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  
  string user, x, id, line;
  string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        linestream >> user >> x >> id;
        if ( uid == id ) {break;}        
    } 
  }  
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  
  std::vector<string> time;
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        linestream >> time.push_back();
        if ( time.size() == 22 ) {break;}        
    } 
  } 
  return stol(time.at(21)) / sysconf(_SC_CLK_TCK) ;
}