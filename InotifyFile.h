#ifndef INOTIFILE_H
#define INOTIFILE_H

#include <unistd.h>
#include <sys/inotify.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <memory>


namespace Event{
  
#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024*(EVENT_SIZE+16))
  
class InotifyFile{
  
public:
  
  InotifyFile(const char* f):m_pfile(f),bRun(false){
    
  }
  
  void Start();

  void Stop();
  
  
private:
  
  void Run();
  
  bool inotifyInit();
  
private:
  std::unique_ptr<std::thread> m_th;
  std::unique_ptr<const char> m_pfile;
  bool bRun;
  int m_fd; 
  int m_wd;

};

}

#endif
