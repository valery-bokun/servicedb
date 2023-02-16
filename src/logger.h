#ifndef LOGGER_H
#define LOGGER_H

#include <thread>
#include <mutex>
#include <set>
#include <fstream>
#include <sstream>
#include <iostream>

namespace logs
{

using namespace std;

enum typelog { logTRACE, logDEBUG, logINFO, logWARN, logERROR, logFATAL };

class LOG
{
public:
  explicit LOG(typelog type);
  ~LOG();
  template <typename T>
  LOG &operator<<(const T msg);

private:
  bool is_open;
  typelog type_;
  ostream &stream_;
  inline const char *get_label(typelog type);
};

template <typename T>
LOG &LOG::operator<<(const T msg)
{
  is_open = true;
  if(type_==logERROR || type_==logFATAL || !stream_)
    cerr << msg;

  if (stream_)
    stream_ << msg;
  
  return *this;
}

} // namespace logs

#endif // LOGGER_H
