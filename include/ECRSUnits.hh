#ifndef ECRSUNITS_HH
#define ECRSUNITS_HH 
// DESCRIPTION
// -----------
//
// This interface define new units of length and magtnetic field used in
// magnetocosmics. 
// 
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include "globals.hh"
//#include "G4UnitsTable.hh"

static const G4double re=6371.2*CLHEP::km;
static const G4double Re=6371.2*CLHEP::km;
static const G4double nT=1.e-9*CLHEP::tesla;
static const G4double nanotesla=1.e-9*CLHEP::tesla;
static const G4double GV=CLHEP::GeV;
static const G4double MV=CLHEP::MeV;

#endif
