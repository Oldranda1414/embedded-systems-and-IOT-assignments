#ifndef __DATAMANAGER__
#define __DATAMANAGER__

class DataManager {
  
  //variables that show the state of the SLS
  int SLSState;

  //variables that show the state of the Monitoring Sistem
  int MSState;
  float waterLevel;
  int valveOpening;

public:
  DataManager();

  
  void setSLSState(int state);
  int getSLSState();
  
  void setMSState(int state);
  int getMSState();
  void setCurrentWL(float value);
  float getCurrentWL();
  void setCurrentValveOpening(int value);
  int getCurrentValveOpening();
};

#endif