// Updated on 5/29/2014, Hexc, Olesya
//            6/20/2014, Hexc, Olesya: Remove the verbose level
//            2/11/2015, Hexc, Olesya: Add additional arguments for the stepping actions
//            4/5/2015, Hexc, switch to QGSP_BERT_HP physics processes since
//                      FTFP_BERT seems killing neutron tracking at low energies.
//            6/12/2015, Hexc, Olesya: Modified for running ECRS in BNL RCF nodes.
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

// Use FTFP_BERT physics process implementation
//#include "FTFP_BERT.hh"

// 4/5/2015
#include "QGSP_BERT_HP.hh"

#include "ECRSMagneticField.hh"
#include "ECRSDetectorConstruction.hh"
#include "ECRSPrimaryGeneratorAction.hh"
#include "ECRSVisManager.hh"
#include "ECRSRunAction.hh"
#include "ECRSEventAction.hh"
#include "ECRSStackingAction.hh"
#include "ECRSTrackingAction.hh"
#include "ECRSSteppingAction.hh"
#include "ECRSSingleton.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

/*
#if defined(G4UI_USE_TCSH)
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#elif defined(G4UI_USE_XM)
#include "G4UIXm.hh"
#elif defined(G4UI_USE_WIN32)
#include "G4UIWin32.hh"
#elif defined(G4UI_USE_QT)
#include "G4UIQt.hh"
#include "G4Qt.hh"
#else
#include "G4UIterminal.hh"
#endif
*/

#include <time.h> 

#include "G4ProcessTable.hh"
#include "ECRSUnits.hh"
#include "G4UnitsTable.hh"

using namespace CLHEP;

int main(int argc, char** argv) {

  // Units
  // Definition of new units in the unit table should be defined 
  // at the beginning before the
  // instantiation of the runManager and should be followed by 
  // G4UnitDefinition::BuildUnitsTable()  
  
  G4cout << "Building our own units ... " << G4endl;

  new G4UnitDefinition("earth radii","re","Length",re);
  new G4UnitDefinition("earth radii 1","Re","Length",re);
  new G4UnitDefinition("earth radii 2","RE","Length",re);
  new G4UnitDefinition("hour","hour","Time",3600.*s);
  new G4UnitDefinition("minute","minute","Time",60.*s);
  new G4UnitDefinition("day","day","Time",24.*3600.*s);
  new G4UnitDefinition("nanotesla","nT","Magnetic flux density",nT);
  new G4UnitDefinition("gigavolt","GV","Electric potential",1000.*megavolt);
  
  G4UnitDefinition::BuildUnitsTable();

  // The following code reads in parameters from the file earth.par
  // The parameters read are: seed index for random number generator
  // and the name of the output file for data generated.
  // The code also opens the output file.
  
  srand(time(NULL));
  
  G4cout << "Open output file (as a singleton) ... " << G4endl;
  ECRSSingleton* myOut = ECRSSingleton::instance();
  //  char Ofilename[100];
  //  G4int seed_index=10;

  G4int seed_index;

  // Select different random number sequence
  //    argc == 1 --- call rand() function for seed_index
  //    argc == 2 --- get the seed_index from the ECRS_500kRand.txt
  //          argv[1] is the index of the random numbers sequence in the ECRS_500kRand.txt
  //
  G4long ECRS_Rand[10000];
  if (argc == 1) {
    seed_index = (int)rand();  
  } else if ((argc == 2) || (argc == 3)) {
    std::ifstream ECRS_RandNumFile("ECRS_500kRand.txt");
    for ( int i = 0; i < 10000; i++) {
      ECRS_RandNumFile >> ECRS_Rand[i];
    }
    ECRS_RandNumFile.close();
    seed_index = ECRS_Rand[atol(argv[1])];
  } else {
    G4cout << " You forgot picking a proper random seed index" << G4endl;
    exit (1);
  }

  myOut->Fopen("Cosmic_Output.dat");
  
  // Select the RanecuEngine random number generator with seeds defined above
  
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  G4long Myseeds[2];
  Myseeds[0] = (int)rand();
  Myseeds[1] = (int)rand(); 
  CLHEP::HepRandom::setTheSeeds(Myseeds,seed_index);
  
  // Run manager
  G4RunManager * runManager = new G4RunManager;
  G4cout << "Run manager is created ... " << G4endl;
  
  // Mandatory initialization classes
  G4cout << "Construct the detector ... " << G4endl;
  ECRSDetectorConstruction* detector = new ECRSDetectorConstruction;
  G4cout << "Detector construction is done!  " << G4endl;
  
  runManager->SetUserInitialization(detector);
  
  //  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  G4VModularPhysicsList* physicsList = new QGSP_BERT_HP;
  runManager->SetUserInitialization(physicsList);
  
  ECRSRunAction* runAction = new ECRSRunAction();
  runManager->SetUserAction(runAction);

  ECRSEventAction* eventAction = new ECRSEventAction();
  runManager->SetUserAction(eventAction);
  
  // set mandatory user action class
  ECRSPrimaryGeneratorAction* primaryGenAction = new ECRSPrimaryGeneratorAction(detector, runAction, eventAction);
  runManager->SetUserAction(primaryGenAction);
  
  // set optional user action classes
  runManager->SetUserAction(new ECRSStackingAction);
  runManager->SetUserAction(new ECRSSteppingAction(detector, runAction, eventAction));
  runManager->SetUserAction(new ECRSTrackingAction(runAction, eventAction));

  runManager->Initialize(); 

  //G4cout << " I am here ... " << G4endl;
    
  G4ProcessTable::GetProcessTable()->SetProcessActivation("MYTransportation",false);
  G4ProcessTable::GetProcessTable()->SetProcessActivation("Transportation",true);
  
#ifdef G4VIS_USE
  // Visualization, if you choose to have it!
  // Use geant4 default visualization manager
  //  G4VisManager* visManager = new ECRSVisManager;
  G4VisManager* visManager = new G4VisExecutive;
  G4cout << "Initializing visualization manatger ... " << G4endl;
  visManager->Initialize();
#endif
  
  //  G4UIsession* session = 0;
  G4UIExecutive* session = 0;

  //User interactions  
  G4UImanager* UI = G4UImanager::GetUIpointer();
  UI->ApplyCommand("/tracking/verbose 0");

  if ((argc==1) || (argc ==2)) { 
    
    // G4UIterminal is a (dumb) terminal.
    /*
      #ifdef G4UI_USE_XM
      session = new G4UIXm(argc,argv);
      #else           
      #ifdef G4UI_USE_TCSH
      session = new G4UIterminal(new G4UItcsh);      
      #else
      session = new G4UIterminal();
      #endif
      #endif
    */
    session = new G4UIExecutive(argc, argv);
    session->SessionStart();
    delete session;
  }
  else {
    G4String command = "/control/execute ";
    G4String fileName = argv[2];
    UI->ApplyCommand(command+fileName);

    session = new G4UIExecutive(argc, argv);
    //session->SessionStart();
    delete session;
  }
  
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;
  G4cout << "RunManager deleted." << G4endl;
  
  myOut->Fclose();
  
  return 0;
}
