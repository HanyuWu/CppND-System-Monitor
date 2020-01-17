#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  float result_;
  long active_, sum_;
  active_ = LinuxParser::ActiveJiffies();
  sum_ = LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
  result_ = (active_*1.0) / sum_;
  return result_;
}