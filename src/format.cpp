#include "format.h"
#include <string>
using std::string;
using std::to_string;

// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  string format_time, str_hour, str_min, str_sec;
  long hour, min, sec;
  hour = seconds / 3600;
  seconds -= hour * 3600;
  min = seconds / 60;
  seconds -= min * 60;
  sec = seconds;
  str_hour = to_string(hour);
  if (str_hour.length()==1) {
    str_hour = "0"+str_hour;
  }
  str_min = to_string(min);
  if (str_min.length()==1) {
    str_min = "0"+str_min;
  }
  str_sec = to_string(sec);
  if (str_sec.length()==1) {
    str_sec = "0"+str_sec;
  }
  format_time = str_hour+":"+str_min+":"+str_sec;
  return format_time;
}