// 11/10/2014: Hexc, Olesya: Made sure that the default magnetic field settings in the messenger are 
//             consistent with respect to the default parameters in ECRSMagneticField constructor.
//
#include "G4UnitsTable.hh"

#include"ECRSMagneticField.hh"
#include"ECRSFieldMessenger.hh"
#include"G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithABool.hh"

#include <strstream>

using namespace CLHEP;

ECRSFieldMessenger::ECRSFieldMessenger (ECRSMagneticField* aField )
{ 
  theField = aField;

  G4UIparameter* param;
  G4String candidates;
  G4String guidance;
 
 //directories
  mainDir = new G4UIdirectory("/ECRS/");
  mainDir->SetGuidance("ECRS application control");
  
  IntegrationDir= new G4UIdirectory("/ECRS/INTEGRATION/");
  IntegrationDir->SetGuidance("Integration control");
  
  MagnetoDir = new G4UIdirectory("/ECRS/BFIELD/");
  MagnetoDir->SetGuidance("Magnetosphere control"); 
  
  // Integration commands
  
  SetEpsilonCmd = new
    G4UIcmdWithADouble("/ECRS/INTEGRATION/SetPrecision",this);
  SetEpsilonCmd->SetGuidance("Set the relative precision for integration");
  SetEpsilonCmd->SetParameterName("precision",false);
  SetEpsilonCmd->SetRange("precision <= 1.e-3  && precision >=1.e-8");
  SetEpsilonCmd->AvailableForStates(G4State_PreInit,G4State_Idle); 
  
  
  SetG4DeltaChordCmd = new
    G4UIcmdWithADoubleAndUnit("/ECRS/INTEGRATION/SetG4MaxStep",this);
  SetG4DeltaChordCmd->SetGuidance("Set the maximal integrating step allowed for G4 integration");
  SetG4DeltaChordCmd->SetParameterName("DeltaChord",false);
  SetG4DeltaChordCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetG4DeltaChordCmd->SetUnitCategory("Length");
  
  SetDeltaIntersectionCmd = new              
    G4UIcmdWithADoubleAndUnit("/ECRS/INTEGRATION/SetDeltaIntersection",this);
  SetDeltaIntersectionCmd->SetGuidance("Set the precision for crossing boundary");
  SetDeltaIntersectionCmd->SetParameterName("DeltaIntersection",false);
  SetDeltaIntersectionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetDeltaIntersectionCmd->SetUnitCategory("Length");
  
  SetBSMaxStepCmd = new
    G4UIcmdWithADoubleAndUnit("/ECRS/INTEGRATION/SetBSMaxStep",this);
  SetBSMaxStepCmd->SetGuidance("Set the maximal integrating step allowed for BS integration");
  SetBSMaxStepCmd->SetParameterName("BSMaxStep",false);
  SetBSMaxStepCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetBSMaxStepCmd->SetUnitCategory("Length");
  
  ResetIntegrationParametersCmd= 
    new G4UIcmdWithoutParameter("/ECRS/INTEGRATION/SetDefaultIntegrationParameters",this);
  ResetIntegrationParametersCmd->SetGuidance("Set the integration parameters to their default values");
  ResetIntegrationParametersCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SelectBulirshStoerMethodCmd= 
    new G4UIcmdWithoutParameter("/ECRS/INTEGRATION/SelectBulirshStoerMethod",this);
  
  SelectBulirshStoerMethodCmd->SetGuidance("The Bulirsh Stoer Method of integration is selected");
  SelectBulirshStoerMethodCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SelectG4IntegrationMethodCmd= 
    new G4UIcmdWithoutParameter("/ECRS/INTEGRATION/SelectG4IntegrationMethod",this);
  
  SelectG4IntegrationMethodCmd->SetGuidance("The Integration methods available in G4 are selected");
  SelectG4IntegrationMethodCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  SetStepperCmd = new
    G4UIcmdWithAString("/ECRS/INTEGRATION/SetStepperModel",this);
  SetStepperCmd
    ->SetGuidance("Set the stepper model for the G4Integration method ");
  SetStepperCmd->SetParameterName("choice",true);
  SetStepperCmd->SetDefaultValue("CashKarpRKF45");
  SetStepperCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  candidates = "ExplicitEuler ImplicitEuler SimpleRunge ClassicalRK4 ";
  candidates += "CashKarpRKF45 RKG3_Stepper";
  SetStepperCmd->SetCandidates(candidates);
  
  // magnetic field model commands
  
  SetTimeOfBCmd = new
    G4UIcmdWithADoubleAndUnit("/ECRS/BFIELD/SetTimeOfB",this);
  SetTimeOfBCmd->SetGuidance("Set the time from the start date at which B is computed ");
  SetTimeOfBCmd->SetParameterName("time",false);
  SetTimeOfBCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetTimeOfBCmd->SetUnitCategory("Time");
  
  
  SetStartDateCmd = new
    G4UIcommand("/ECRS/BFIELD/SetStartDate",this);
  SetStartDateCmd
    ->SetGuidance("Set the start reference date");
  SetStartDateCmd
    ->SetGuidance("[usage] /ECRS/BFIELD/SetStartDate Year Month Day Hour Minute Second"); 
  SetStartDateCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  param = new G4UIparameter("Year",'i',false);
  SetStartDateCmd->SetParameter(param);
  
  param = new G4UIparameter("Month",'i',false);
  param->SetParameterRange("Month <13 && Month>0");
  SetStartDateCmd->SetParameter(param);
  
  param = new G4UIparameter("Day",'i',false);
  param->SetParameterRange("Day <32 && Day>0");
  SetStartDateCmd->SetParameter(param);
  
  param = new G4UIparameter("Hour",'i',true);
  param->SetDefaultValue("0");
  param->SetParameterRange("Hour<24");
  SetStartDateCmd->SetParameter(param);
  
  param = new G4UIparameter("Minute",'i',true);
  param->SetDefaultValue("0");
  param->SetParameterRange("Minute<60");
  SetStartDateCmd->SetParameter(param);
  
  
  param = new G4UIparameter("Second",'i',true);
  param->SetDefaultValue("0");
  param->SetParameterRange("Second<60");
  SetStartDateCmd->SetParameter(param);
 
  Setnm_igrfCmd = new
    G4UIcmdWithAnInteger("/ECRS/BFIELD/SetNmaxForIGRF",this);
  guidance ="Set the maximum order of the spherical harmonics coefficients";
  guidance +=" used for computing the IGRF/DGRF magnetic field"; 
  Setnm_igrfCmd->SetGuidance(guidance);
  Setnm_igrfCmd->SetParameterName("Nmax",false);
  Setnm_igrfCmd->SetRange("Nmax <14 && Nmax >0");
  Setnm_igrfCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SetExternalFieldCmd = new
    G4UIcmdWithAString("/ECRS/BFIELD/SetExternalFieldModel",this);
  SetExternalFieldCmd
    ->SetGuidance("Set the model of the external magnetospheric magnetic field ");
  SetExternalFieldCmd->SetGuidance("  Choice : NOFIELD, TSY89, TSY96, TSY2001");
  SetExternalFieldCmd->SetParameterName("choice",true);
  SetExternalFieldCmd->SetDefaultValue("TSY89");
  SetExternalFieldCmd->SetCandidates("NOFIELD TSY89 TSY96 TSY2001");
  SetExternalFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle); 
  
  SetInternalFieldCmd = new
    G4UIcmdWithAString("/ECRS/BFIELD/SetGeomagneticFieldModel",this);
  SetInternalFieldCmd
    ->SetGuidance("Set the model of the internal magnetic field");
  SetInternalFieldCmd->SetGuidance("  Choice : IGRF DIPOLE NOFIELD");
  SetInternalFieldCmd->SetParameterName("choice",true);
  SetInternalFieldCmd->SetDefaultValue("IGRF");
  SetInternalFieldCmd->SetCandidates("IGRF DIPOLE NOFIELD"); 
  
  SetInternalFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle); 
  
  /* SetMagnetopauseModelCmd = new
     G4UIcmdWithAString("/ECRS/BFIELD/SetMagnetopauseModel",this);
     SetMagnetopauseModelCmd->SetGuidance("Set the magnetopause model ");
     SetMagnetopauseModelCmd->SetGuidance("  Choice : TSY2001, TSY89, IGRF");
     SetMagnetopauseModelCmd->SetParameterName("choice",true);
     SetMagnetopauseModelCmd->SetDefaultValue("IGRF");
     SetMagnetopauseModelCmd->AvailableForStates(G4State_PreInit,G4State_Idle); */
  
  SetDipoleB0Cmd = new
    G4UIcmdWithADoubleAndUnit("/ECRS/BFIELD/SetDipoleB0",this);
  SetDipoleB0Cmd->SetGuidance("Set the magnetic moment B0 of the geomagnetic  dipole");
  SetDipoleB0Cmd->SetParameterName("B0",false);
  SetDipoleB0Cmd->SetRange("B0 > 0.");
  SetDipoleB0Cmd->SetUnitCategory("Magnetic flux density");
  SetDipoleB0Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SetDipolePSCmd = new
    G4UIcmdWithADoubleAndUnit("/ECRS/BFIELD/SetDipoleTiltAngle",this);
  SetDipolePSCmd->SetGuidance("Set the dipole tilt angle  of the geomagnetic  dipole");
  SetDipolePSCmd->SetParameterName("PS",false);
  SetDipolePSCmd->SetRange("PS > 0. && PS <180.*degree");
  
  SetDipolePSCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetDipolePSCmd->SetUnitCategory("Angle");
  
  SetDipoleShiftCmd =
    new G4UIcmdWith3VectorAndUnit("/ECRS/BFIELD/SetDipoleCenter",this);
  SetDipoleShiftCmd->SetGuidance("Define the center  of the geomagnetic dipole in GEO coordinates");
  SetDipoleShiftCmd->SetParameterName("X","Y","Z",false);
  SetDipoleShiftCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetDipoleShiftCmd->SetUnitCategory("Length");
  
  SetDipoleAxisCmd = new G4UIcommand("/ECRS/BFIELD/SetDipoleAxis",this);
  SetDipoleAxisCmd->SetGuidance("Set the  axis of the geomagnetic dipole");
  SetDipoleAxisCmd
    ->SetGuidance("[usage] /ECRS/BFIELD/SetDipoleAxis   Theta Phi Unit" ); 
  SetDipoleAxisCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  param = new G4UIparameter("Theta",'d',false);
  param->SetDefaultValue("0");
  SetDipoleAxisCmd->SetParameter(param);
  
  param = new G4UIparameter("Phi",'d',false);
  param->SetDefaultValue("0.");
  SetDipoleAxisCmd->SetParameter(param);
  
  param = new G4UIparameter("Unit",'s',true);
  param->SetDefaultValue("degree");
  param->SetParameterCandidates("degree deg rad radian mrad milliradian");
  SetDipoleAxisCmd->SetParameter(param);
  
  SetConsiderDipoleShiftCmd =
    new G4UIcmdWithABool("/ECRS/BFIELD/SetConsiderDipoleShift",this);
  SetConsiderDipoleShiftCmd->SetGuidance("When set to true the dipole shift is considered in the  Tsyganenko models");
  SetConsiderDipoleShiftCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  SetTiltedDipoleParameterFromIGRFCmd= 
    new G4UIcmdWithoutParameter("/ECRS/BFIELD/SetNonshiftedGeodipoleFromIGRF",this);
  SetTiltedDipoleParameterFromIGRFCmd
    ->SetGuidance("Compute the parameters of the tilted geomagnetic dipole from the IGRF coefficients");
  SetTiltedDipoleParameterFromIGRFCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  SetEccentricDipoleParameterFromIGRFCmd= 
    new G4UIcmdWithoutParameter("/ECRS/BFIELD/SetShiftedGeodipoleFromIGRF",this);
  SetEccentricDipoleParameterFromIGRFCmd
    ->SetGuidance("Compute the parameters of the eccentric geomagnetic dipole from the IGRF coefficients");
  SetEccentricDipoleParameterFromIGRFCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  //Magnetic activity
  //-----------------
  
  SetIoptCmd = new
    G4UIcmdWithAnInteger("/ECRS/BFIELD/SetIopt",this);
  SetIoptCmd->SetGuidance("Set the Iopt (kp level) parameter for the TSY89 model");
  SetIoptCmd->SetParameterName("Iopt",false);
  SetIoptCmd->SetRange("Iopt >0 Iopt<8");
  SetIoptCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SetPdynCmd = new
    G4UIcmdWithADouble("/ECRS/BFIELD/SetPdyn",this);
  SetPdynCmd->SetGuidance("Set the Pdyn (sw dynamic pressure in nPa) parameter for TSY96 and TSY2001");
  SetPdynCmd->SetParameterName("Pdyn",false);
  SetPdynCmd->SetRange("Pdyn > 0");
  SetPdynCmd->AvailableForStates(G4State_PreInit,G4State_Idle); 
  
  
  SetDstCmd = new
    G4UIcmdWithADoubleAndUnit("/ECRS/BFIELD/SetDst",this);
  SetDstCmd->SetGuidance("Set the Dst parameter for the Tsy96 and Tsy2001 models");
  SetDstCmd->SetParameterName("Dst",false);
  SetDstCmd->SetUnitCategory("Magnetic flux density");
  SetDstCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  SetImfyCmd = new
    G4UIcmdWithADoubleAndUnit("/ECRS/BFIELD/SetImfBy",this);
  SetImfyCmd->SetGuidance("Set the ImfY parameter for the Tsy96 and Tsy2001 models");
  SetImfyCmd->SetParameterName("Imfy",false);
  SetImfyCmd->SetUnitCategory("Magnetic flux density");
  SetImfyCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  SetImfzCmd = new
    G4UIcmdWithADoubleAndUnit("/ECRS/BFIELD/SetImfBz",this);
  SetImfzCmd->SetGuidance("Set the Imfz parameter for the Tsy96 and Tsy2001 models");
  SetImfzCmd->SetParameterName("Imfz",false);
  SetImfzCmd->SetUnitCategory("Magnetic flux density");
  SetImfzCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  SetG1Cmd = new
    G4UIcmdWithADouble("/ECRS/BFIELD/SetG1",this);
  SetG1Cmd->SetGuidance("Set the the G1  parameter for the TSY2001 model");
  SetG1Cmd->SetParameterName("G1",false);
  SetG1Cmd->AvailableForStates(G4State_PreInit,G4State_Idle); 
  
  SetG2Cmd = new
    G4UIcmdWithADouble("/ECRS/BFIELD/SetG2",this);
  SetG2Cmd->SetGuidance("Set the G2 parameter for the TSY2001 model");
  SetG2Cmd->SetParameterName("G2",false);
  SetG2Cmd->AvailableForStates(G4State_PreInit,G4State_Idle); 
  
  ReadTSYParameterCmd = new
    G4UIcmdWithAString("/ECRS/BFIELD/ReadTSYParameters",this);
  
  guidance = "Read in a file the values of the magnetic activity and solar ";
  guidance +="wind parameters in function of time";
  ReadTSYParameterCmd->SetGuidance(guidance);
  ReadTSYParameterCmd->SetParameterName("filename",true);
  ReadTSYParameterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  PrintTSYParameterCmd= 
    new G4UIcmdWithoutParameter("/ECRS/BFIELD/PrintTSYParameters",this);
  
  guidance = "Print the actual magnetic activity and solar wind parameters ";
  guidance +="used in  the Tsyganenko models";
  PrintTSYParameterCmd->SetGuidance(guidance);
}
////////////////////////////////////////////////////////////////////////////////
//
ECRSFieldMessenger::~ECRSFieldMessenger()
{ delete SetEpsilonCmd;
  delete SetG4DeltaChordCmd; 
  delete SetDeltaIntersectionCmd;
  delete SetBSMaxStepCmd;
  delete ResetIntegrationParametersCmd;
  delete SelectBulirshStoerMethodCmd;
  delete SelectG4IntegrationMethodCmd;
  delete SetStepperCmd;
  delete SetTimeOfBCmd;
  delete SetStartDateCmd;
  delete Setnm_igrfCmd;
  delete SetExternalFieldCmd;
  delete SetInternalFieldCmd;
  delete SetMagnetopauseModelCmd;
  delete SetDipoleB0Cmd;
  delete SetDipolePSCmd;
  delete SetDipoleShiftCmd; 
  delete SetDipoleAxisCmd; 
  delete SetConsiderDipoleShiftCmd; 
  delete SetTiltedDipoleParameterFromIGRFCmd;
  delete SetEccentricDipoleParameterFromIGRFCmd;
  delete SetIoptCmd;
  delete SetPdynCmd;
  delete SetDstCmd;
  delete SetImfyCmd;
  delete SetImfzCmd;
  delete SetG1Cmd;
  delete SetG2Cmd;
  delete ReadTSYParameterCmd;
  delete PrintTSYParameterCmd; 
  delete mainDir;
  delete IntegrationDir;
  delete MagnetoDir;
}		  
////////////////////////////////////////////////////////////////////////////////
//
void ECRSFieldMessenger::SetNewValue(G4UIcommand * command,G4String newValues)
{  
  
  // integration parameters command 
  
  if (command == SetEpsilonCmd) 
    theField->SetEpsilon(SetEpsilonCmd->GetNewDoubleValue(newValues)); 
  
  else if (command == SetG4DeltaChordCmd) 
    theField->SetDeltaChord(SetG4DeltaChordCmd->GetNewDoubleValue(newValues));   
  
  else if (command == SetBSMaxStepCmd) 
    theField->SetBSMaxStep(SetBSMaxStepCmd->GetNewDoubleValue(newValues)); 
  
  else if (command == SetDeltaIntersectionCmd) 
    theField->SetDeltaIntersection(SetDeltaIntersectionCmd->GetNewDoubleValue(newValues));   
  
  else if (command == ResetIntegrationParametersCmd) 
    theField->ResetIntegrationParameters();  
  else if (command == SelectBulirshStoerMethodCmd) 
    theField->SelectBulirshStoerMethod();  
  
  else if (command == SelectG4IntegrationMethodCmd) 
    theField->SelectG4IntegrationMethods();  			    			    
  
  else if ( command == SetStepperCmd )
    theField->SetStepper(newValues);			     
  // magnetic field model parameters 
  
  else if( command == SetTimeOfBCmd)
    theField->SetTimeOfB( SetTimeOfBCmd->GetNewDoubleValue(newValues));
  // theField->SetTimeOfB( SetTimeOfBCmd->GetNewDoubleValue(newValues)/s);   // "/s" causing compiler problem
  
  else if ( command == SetStartDateCmd ){
    const char* paramString=newValues;
    G4int  Year,Month,Day,Hour,Minute,Second;
    std::istrstream is((char*)paramString);
    is >> Year >> Month >> Day >> Hour >> Minute >> Second;
    /*G4cout<<"Test"<<std::endl;
      G4cout<<Minute<<std::endl;
      G4cout<<Second<<std::endl;*/
    theField->SetStartDate(Year,Month,Day,Hour,Minute,Second);
  }	    
  
  else if( command == Setnm_igrfCmd ) 
    theField->Setnm_igrf(Setnm_igrfCmd->GetNewIntValue(newValues));
  
  
  else if ( command == SetExternalFieldCmd )
    theField->SetExternalField(newValues);
  
  else if ( command == SetInternalFieldCmd )
    theField->SetInternalField(newValues);
  
  else if( command == SetDipoleB0Cmd ) 
    theField->SetDipoleB0(SetDipoleB0Cmd->GetNewDoubleValue(newValues));
  
  else if( command == SetDipolePSCmd ) 
    theField->SetDipolePS(SetDipolePSCmd->GetNewDoubleValue(newValues));
  
  else if( command == SetDipoleShiftCmd){
    G4ThreeVector vec=SetDipoleShiftCmd->GetNew3VectorValue(newValues);
    theField->SetDipoleSchift(vec);
  }
  
  else if( command == SetConsiderDipoleShiftCmd)
    theField->SetConsiderDipoleShift(
				     SetConsiderDipoleShiftCmd->GetNewBoolValue(newValues)); 
  
  else if ( command == SetDipoleAxisCmd ){
    const char* paramString=newValues;
    G4double  Theta,Phi;
    G4String AngleUnit;
    std::istrstream is((char*)paramString);
    is >> Theta >> Phi;
    is >>AngleUnit;
    G4double angle_unit = G4UnitDefinition::GetValueOf(AngleUnit);
    Theta *=angle_unit;
    Phi *=angle_unit;
    theField->SetDipoleAxis(Theta,Phi);
  } 
  
  else if( command == SetTiltedDipoleParameterFromIGRFCmd)
    theField-> SetTiltedDipoleParameterFromIGRF(); 
  
  else if( command == SetEccentricDipoleParameterFromIGRFCmd)
    theField-> SetEccentricDipoleParameterFromIGRF(); 
  
  
  // magnetic activity command
  
  else if( command == SetIoptCmd)
    theField->SetIopt(SetIoptCmd->GetNewIntValue(newValues));   	    
  
  
  else if (command == SetPdynCmd) 
    theField->SetPdyn(SetPdynCmd->GetNewDoubleValue(newValues));   
  
  else if (command == SetDstCmd) 
    theField->SetDst(SetDstCmd->GetNewDoubleValue(newValues));
  
  else if (command == SetImfyCmd) 
    theField->SetImfy(SetImfyCmd->GetNewDoubleValue(newValues));   
  
  else if (command == SetImfzCmd) 
    theField->SetImfz(SetImfzCmd->GetNewDoubleValue(newValues));
  
  else if (command == SetG1Cmd) 
    theField->SetG1(SetG1Cmd->GetNewDoubleValue(newValues));   
  
  else if (command == SetG2Cmd) 
    theField->SetG2(SetG2Cmd->GetNewDoubleValue(newValues));
  
  else if( command == ReadTSYParameterCmd)
    theField->ReadTSYParameter(newValues);	       	  
  
  else if( command == PrintTSYParameterCmd)
    theField->PrintStormParameter();	  	
  
}












