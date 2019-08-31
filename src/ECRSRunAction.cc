// Oct 1, 2014: Hexc, Olesya - Adding code to write root output for data
//    analysis.
// Dec 8, 2014: Hexc, Olesya - Adding code to write root output for hits arriving at
//    the syrface of the Earth
//
#include "ECRSRunAction.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"

#include "g4root.hh"

ECRSRunAction::ECRSRunAction()
{;}

ECRSRunAction::~ECRSRunAction()
{;}

void ECRSRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  // Book histograms, ntuple
  
  // Create analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in Analysis.hh

  G4cout << "##### Create ECRS analysis manager " << "  " << this << G4endl;
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  G4cout << "Using " << analysisManager->GetType() << " analysis manager" << G4endl;

  // Create directories 
  
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  
  // Open an output file
  
  G4String fileName = "ECRS2014_Shower";
  analysisManager->OpenFile(fileName);

  analysisManager->SetFirstNtupleId(1);

  // Creating ntuple
  
  analysisManager->CreateNtuple("ECRS_NewParticle", "cosmicShower");
  fNtColID[0] = analysisManager->CreateNtupleDColumn(1, "pID");
  fNtColID[1] = analysisManager->CreateNtupleDColumn(1, "kE");
  fNtColID[2] = analysisManager->CreateNtupleDColumn(1, "x");
  fNtColID[3] = analysisManager->CreateNtupleDColumn(1, "y");
  fNtColID[4] = analysisManager->CreateNtupleDColumn(1, "z");
  fNtColID[5] = analysisManager->CreateNtupleDColumn(1, "px");
  fNtColID[6] = analysisManager->CreateNtupleDColumn(1, "py");
  fNtColID[7] = analysisManager->CreateNtupleDColumn(1, "pz");
  fNtColID[8] = analysisManager->CreateNtupleDColumn(1, "gTime");
  fNtColID[9] = analysisManager->CreateNtupleDColumn(1, "localTime");
  fNtColID[10] = analysisManager->CreateNtupleDColumn(1, "properTime");
  fNtColID[11] = analysisManager->CreateNtupleDColumn(1, "evtID");        // added on 2/10/2015
  analysisManager->FinishNtuple(1);

  // Creating ntuple for storing hits information on the photodetector
  analysisManager->CreateNtuple("ECRS_SurfaceHits", "showerDistribution");
  fNtColID[12] = analysisManager->CreateNtupleDColumn(2, "pID");
  fNtColID[13] = analysisManager->CreateNtupleDColumn(2, "kE");
  fNtColID[14] = analysisManager->CreateNtupleDColumn(2, "x");
  fNtColID[15] = analysisManager->CreateNtupleDColumn(2, "y");
  fNtColID[16] = analysisManager->CreateNtupleDColumn(2, "z");
  fNtColID[17] = analysisManager->CreateNtupleDColumn(2, "px");
  fNtColID[18] = analysisManager->CreateNtupleDColumn(2, "py");
  fNtColID[19] = analysisManager->CreateNtupleDColumn(2, "pz");
  fNtColID[20] = analysisManager->CreateNtupleDColumn(2, "gTime");
  fNtColID[21] = analysisManager->CreateNtupleDColumn(2, "localTime");
  fNtColID[22] = analysisManager->CreateNtupleDColumn(2, "evtID");        // added on 2/10/2015
  analysisManager->FinishNtuple(2);

  // Creating ntuple for storing primary particle information
  analysisManager->CreateNtuple("ECRS_Primary", "primaryParticle");
  fNtColID[23] = analysisManager->CreateNtupleDColumn(3, "primID");
  fNtColID[24] = analysisManager->CreateNtupleDColumn(3, "eP");
  fNtColID[25] = analysisManager->CreateNtupleDColumn(3, "xP");
  fNtColID[26] = analysisManager->CreateNtupleDColumn(3, "yP");
  fNtColID[27] = analysisManager->CreateNtupleDColumn(3, "zP");
  fNtColID[28] = analysisManager->CreateNtupleDColumn(3, "pxP");
  fNtColID[29] = analysisManager->CreateNtupleDColumn(3, "pyP");
  fNtColID[30] = analysisManager->CreateNtupleDColumn(3, "pzP");
  fNtColID[31] = analysisManager->CreateNtupleDColumn(3, "evtID");        // added on 3/2/2015
  analysisManager->FinishNtuple(3);

}

void ECRSRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int nofEvents = aRun->GetNumberOfEvent();
  if ( nofEvents == 0 ) return;
  
  // print histogram statistics
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  // save histograms 
  
  analysisManager->Write();
  analysisManager->CloseFile();
  
  // complete cleanup
  
  delete G4AnalysisManager::Instance();

  /* commented on 10/6/2014
  if (G4VVisManager::GetConcreteInstance())
  {
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
  }
  */
}
