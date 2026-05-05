#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int locate_dir(){
  string knowDir = "/sys/class/hwmon/hwmon";
  for(int i=0; i<10; i++){
    string PosDir = knowDir + to_string(i) + "/name";
    ifstream file(PosDir);
    if(file.is_open()){
      string line;
      getline(file, line);
      if(line == "thinkpad"){
        return i;
      }
      file.close();
    }
  }
  return -1;
}

int main(){
  int dir = locate_dir();
  cout << dir;
  return 0;
}
