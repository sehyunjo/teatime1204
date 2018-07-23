#include "InotifyFile.h"

void Event::InotifyFile::Start(){
  if(!bRun){
      //초기화 해주고
    if(inotifyInit()){
    
	//쓰레드 생성 
      bRun = true;
      m_th.reset(new std::thread([this](){
      this->Run();
	
      }));
    }
  }
}

void Event::InotifyFile::Stop(){

  if(bRun){
    if(m_th->joinable()){
      std::cout << " notify Stop() " << std::endl;
      bRun = false;
      inotify_rm_watch(m_fd,m_wd);
      close(m_fd);
      m_th->join(); 
    }
  } 
}

void Event::InotifyFile::Run(){
std::cout << "notify Start() " << std::endl;
    char m_buffer[EVENT_BUF_LEN];
    std::size_t length=0;
    std::size_t size =0;
    
    while(bRun){	
	  
	  m_wd = inotify_add_watch(m_fd,m_pfile.get(),IN_ATTRIB);  //IN_ATTRIB : 파일이 수정이 되면 
	  
	  //이벤트를 기다리는데 비동기식으로 바꾸고 싶음
	  
	  if((length = read(m_fd,m_buffer,EVENT_BUF_LEN))<0){
	      std::cerr << " Run() read length < 0 " << std::endl;
	      break;
	  }
	  
	  
	  while(size<length){
	    struct inotify_event *event = ( struct inotify_event * ) &m_buffer[ size ];
	    
	    if(event->mask & IN_ATTRIB){  //IN_ATTRIB이벤트가 발생하면 
	      std::cout << " IN_ATTRIB FILE " << std::endl;
	      
	    }
	    
	    size +=EVENT_SIZE + event->len;
	    
	  }
	  size = 0;
    }
  
}


bool Event::InotifyFile::inotifyInit(){
  
    m_fd = inotify_init();
    if(m_fd < 0 ){
      std::cerr << " inotify_init() error " << std::endl;
      return false;
    }
    
    return true;
  
}