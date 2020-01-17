#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// An example of how to read data from the filesystem
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

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  long memtotal(1), memfree(1);
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "MemTotal:") {
          linestream >> memtotal;
        } else if (key == "MemFree:") {
          linestream >> memfree;
          return (memtotal - memfree) * 1.0 / (memtotal);
        }
      }
    }
  }
  return (memtotal - memfree) * 1.0 / (memtotal);
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime(0);
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return uptime;
  }
  return uptime;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line, cpu;
  long part, sum_ = 0;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu;
    while (linestream >> part) {
      sum_ += part;
    }
    return sum_;
  }
  return sum_;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line, temp;
  long user, nice, system, irq, softirq, steal, sum_ = 0;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> temp >> user >> nice >> system >> temp >> temp >> irq >>
        softirq >> steal;
    sum_ = user + nice + system + irq + softirq + steal;
    return sum_;
  }
  return sum_;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line, temp;
  long idle, iowait, sum_ = 0;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> temp >> temp >> temp >> temp >> idle >> iowait;
    sum_ = idle + iowait;
    return sum_;
  }
  return sum_;
}

// I don't think I should utilize this function...
// TODO: Read and return CPU utilization
// vector<string> LinuxParser::CpuUtilization() { return {}; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { return LinuxParser::Pids().size(); }

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  vector<int> process_dict = LinuxParser::Pids();
  string current_pid;
  int running_process = 0;
  for (int i = 0; i < int(process_dict.size()); i++) {
    current_pid = to_string(process_dict[i]);
    string dict = kProcDirectory + current_pid + kStatusFilename;
    std::ifstream stream(dict);
    string line, key, flag;
    if (stream.is_open()) {
      while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        linestream >> key >> flag;
        if (key == "State:") {
          if (flag == "R") {
            running_process++;
            break;
          }
        }
      }
    }
  }
  return running_process;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string cmdl;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, cmdl);
    return cmdl;
  }
  return string();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }