#ifndef ECRSPar_h
#define ECRSPar_h 1

#include <iostream>
#include <fstream>
#include <string>

class ECRSPar
{
  public:
    char* ifileName;
    ECRSPar() {}
    ~ECRSPar() {}

    std::ifstream fin;

    void Fopen(char* myFilename)
    {
      ifileName = new char[strlen(myFilename)];
      strcpy(ifileName,myFilename);
      fin.open(ifileName);
    }
    void Fclose()
    {
      fin.close();
    }
};

#endif
