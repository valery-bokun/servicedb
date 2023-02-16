#include "logger.h"

#include <iomanip>
#include <chrono>

namespace logs
{

using namespace std;
using namespace chrono;

template <typename T>
class keeper
{
public:
  set<T> &refs() { return *refs_; }
  keeper() { refs_ = new set<T>; }
  ~keeper() {
    while ( refs_->size())
      delete *refs_->begin();
    delete refs_;
  }
protected:
  set<T> *refs_;
};

class Streams
{
public:
  static Streams *get_instance();
  ostream &stream() { return file_; }
  thread::id thread_id() { return thread_id_; }
  ~Streams();
private:
  Streams();
  Streams(Streams &);
  Streams &operator=(Streams &);

  static keeper<Streams *> krefs;
  static mutex refs_mutex;
  thread::id thread_id_;
  ofstream file_;
};

mutex Streams::refs_mutex;
keeper<Streams *> Streams::krefs;

Streams::Streams()
{
  thread_id_ = this_thread::get_id();
  {
    lock_guard<mutex> lock(refs_mutex);
    krefs.refs().insert(this);
  }

  stringstream fname;
  fname << "log" << '[' << thread_id_ << "].txt";

  file_.open(fname.str(), ios::out | ios::app);

  if (!file_)
    cerr << "Error opening " << fname.str() << endl;
}

Streams::~Streams()
{
  file_ << "~Logger() file.close() " << this << '\n' << endl;
  file_.close();

  lock_guard<mutex> lock(refs_mutex);
  krefs.refs().erase(this);
}

Streams *Streams::get_instance()
{
  {
    lock_guard<mutex> lock(refs_mutex);
    for (auto v : krefs.refs())
        if ( v->thread_id() == this_thread::get_id())
          return v;
  }

  Streams *new_ = new Streams();
  return new_;
}

LOG::LOG(typelog type)
  :is_open(false), type_(type), stream_(Streams::get_instance()->stream())
{
  auto t = time(nullptr);
  auto tm = *localtime(&t);

  auto now = chrono::system_clock::now();
  auto ms = chrono::duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

  if (!stream_)
    cerr << "Error logging " << endl;

  stream_ << put_time(&tm, "%Y%m%d-%H:%M:%S-")
          << setfill('0') << setw(3) << ms.count()
          << '[' << get_label(type) << ']'
          << '[' << this_thread::get_id() << "] ";
}

LOG::~LOG()
{
  if (!is_open) return;
  if(type_==logERROR || type_==logFATAL || !stream_)
    cerr << endl;
  
  if(!stream_) return;

  if(type_==logERROR || type_==logFATAL || type_==logDEBUG )
    stream_ << endl;
  else
    stream_ << '\n';
}

const char *LOG::get_label(typelog type)
{
  switch(type) {
    case logTRACE: return "TRACE";
    case logDEBUG: return "DEBUG";
    case logINFO:  return "INFO ";
    case logWARN:  return "WARN ";
    case logERROR: return "ERROR";
    case logFATAL: return "FATAL";
    default: return " ";
    }
}

} // namespace logs
