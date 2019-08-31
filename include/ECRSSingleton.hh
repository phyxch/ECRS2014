#ifndef ECRSSingleton_hh
#define ECRSSingleton_hh 1

#include <iostream>
#include <fstream>
#include <string>
#include "globals.hh"

class ECRSSingleton
{
  char* ofileName;
  ECRSSingleton(){}

  public:
    std::ofstream fout;

    static ECRSSingleton* instance()
      {
        static ECRSSingleton* singleSingleton = new ECRSSingleton();
	return singleSingleton;
      }
    void Fopen(char* myFilename)
      {
        ofileName = new char[strlen(myFilename)];
	strcpy(ofileName,myFilename);
	fout.open(ofileName);
      }
    void Fclose()
      {
	fout.close();
      }
};

#endif
