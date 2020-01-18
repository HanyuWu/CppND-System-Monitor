#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int i): pid(i){};

// Return this process's ID
int Process::Pid() { return pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() const{
    return LinuxParser::ActiveJiffies(pid)*1.0/LinuxParser::UpTime(pid);
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() { return string(); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(pid)/sysconf(_SC_CLK_TCK); }

// Overload the "less than" comparison operator for Process objects
// Arrange the Process list from big to small in the aspect of CPU utilization
bool Process::operator<(Process const& a) const { 
    return (CpuUtilization() > a.CpuUtilization()); 
    }