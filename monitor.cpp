#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <iomanip>
using namespace std;

struct CPUdata{
  string linee;
  long user, nice, system, idle, iowait, irq, softirq, steal;
};

CPUdata leer_stats(){
  ifstream file("/proc/stat");
  string line;
  CPUdata d = {"", 0, 0, 0, 0, 0, 0, 0, 0}; 
  if(getline(file, line)){      
    stringstream cc(line);
    cc >> d.linee;
    cc >> d.user >> d.nice >> d.system >> d.idle >> d.iowait >> d.irq >> d.softirq >> d.steal;
  return d;
  }
}

double leer_cpu(){
  CPUdata m1 = leer_stats();
  long total1 = m1.user + m1.nice + m1.system + m1.idle + m1.iowait + m1.irq + m1.softirq + m1.steal;
  long idle1 = m1.idle + m1.iowait;
  usleep(1000000);
  CPUdata m2 = leer_stats();
  long total2 = m2.user + m2.nice + m2.system + m2.idle + m2.iowait + m2.irq + m2.softirq + m2.steal;
  long idle2 = m2.idle + m2.iowait;
  double diffTotal = (double)total2 - total1;
  double diffIdle = (double)idle2 - idle1;

  if (diffTotal <= 0) return 0.0;
  return 100.0 * (1.0 - (diffIdle / diffTotal));
  
}

struct RAMdata{
  double porcentaje, capacidad;
};

RAMdata ram_data(){
  ifstream file("/proc/meminfo");
  string line, lbl;
  RAMdata d = {0, 0};
  long total = 0, disp = 0, val;
  if (file.is_open()) {
    while(getline(file, line)){
      stringstream cc(line);
      cc >> lbl >> val;
      if(lbl == "MemTotal:") total = val;
      if(lbl == "MemAvailable:") {disp = val; break; }
    }
    file.close(); 
  }
  d.porcentaje = 100 * ((total - (double)disp)/total);
  d.capacidad = (total / 1048576);
  return d;
}

double temp(){
  ifstream file("/sys/class/thermal/thermal_zone0/temp");
  string line;
  getline(file, line);
  return (stod(line)/1000);
}

double hz(){
  double sumaFrecuencias = 0;
    int nucleosDetectados = 0;

    for (int i = 0; i < 16; i++) { // Intenta leer hasta 16 núcleos (ajusta según tu CPU)
        string path = "/sys/devices/system/cpu/cpu" + to_string(i) + "/cpufreq/scaling_cur_freq";
        ifstream file(path);
        string line;
        if (file.is_open() && getline(file, line)) {
            sumaFrecuencias += stod(line);
            nucleosDetectados++;
        } else {
            break; // Si no encuentra el archivo del siguiente núcleo, termina
        }
    }

    if (nucleosDetectados == 0) return 0;
    
    // Promedio de todos los núcleos convertido a GHz
    return (sumaFrecuencias / nucleosDetectados) / 1000000.0;
}

double SpeedFan(){
  ifstream file("/sys/class/hwmon/hwmon3/fan1_input");
  string line;
  getline(file, line);
  return (stod(line));
}

double level(){
  ifstream file("/sys/class/hwmon/hwmon3/pwm1");
  string line;
  getline(file, line);
  if (SpeedFan() <= 4800) {
    return (stod(line));
  }
  else { return 256; }
}
int main(){
  
  while (true) {
    RAMdata rama;
    rama = ram_data();
    double ram = (double)rama.porcentaje;
    double Cra = ((double)rama.capacidad) * (ram / 100);
    double cpu = leer_cpu();
    double tem = temp();
    double Ghz = hz();
    double RPM = SpeedFan();
    double PWM = level();
    cout << "\033[2J\033[H";
    cout << "\rCPU:       " << int(cpu) << "%      " << fixed << setprecision(0) << tem << "C     " << fixed << setprecision(2) << Ghz << "Ghz " << endl << endl;
    cout << "RAM:       " << int(ram) << "%     " << fixed << setprecision(2) << Cra << "GB " << endl << endl;
    if (PWM == 256) cout << "FanSpeed:  " << "MAX";
    else cout << "FanSpeed:  " << fixed << setprecision(0) << RPM << "rpm";


    cout << flush;

  }
  return 0;
}
