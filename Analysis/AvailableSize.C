// header for statvfs
#include <sys/statvfs.h>

int AvailableSize(){
  struct statvfs stat;

  string path = "/home/ggarcia/Programs/geant4/G4SKspallation2-build/"; //choose the plae where you want to know the size
  
  if (statvfs(path.c_str(), &stat) != 0) {
    // error happens, just quits here
    return -1;
  }

  // the available size is f_bsize * f_bavail
  cout <<  stat.f_bsize * stat.f_bavail << " bytes" <<endl;
  
  return 1;
}
