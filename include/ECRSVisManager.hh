#ifndef ECRSVisManager_H
#define ECRSVisManager_H 1

#ifdef G4VIS_USE

#include "G4VisManager.hh"

class ECRSVisManager : public G4VisManager
{
  public:
    ECRSVisManager();

  private:
    void RegisterGraphicsSystems();
};

#endif

#endif
