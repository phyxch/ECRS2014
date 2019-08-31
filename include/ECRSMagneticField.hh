#ifndef ECRSMAGNETICFIELD_HH
#define ECRSMAGNETICFIELD_HH 
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// DESCRIPTION
// -----------
//
// This class defines the magnetic field used in the simulation. 
// 
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// PUBLIC MEMBER FUNCTIONS
// -----------------------
//
// ECRSMagnetiField()
//    Constructor.
//
// ~ECRSMagnetiField()
//    Destructor.
//
// void GetFieldValue(const G4double yTrack[] ,G4double B[]) const
//    Define the total magnetic field vector B at the position defined by yTrack
//
// bool OutsideMagnetosphere(G4ThreeVector pos)
//    return true if the position defined by pos is outside the magnetosphere
//    return false if the position definde by pos is inside the magnetosphere
//
// void SetTimeOfB(G4double val);
//    Define TimeOfB
//    TimeOfB  represents the number of second after the StartDate 
//    at which the magnetic field will be computed   	
// 
// void SetStartDate(G4int year, G4int month,G4int day,
//	                       G4int hour, G4int minute,G4int second );
//    Define StartDate 	
//
// void SetIopt(int iopt)
//    Set the iopt parameter used  in the tsyganenko89 model, 
//    defining the geomagnetic activity level
//    IOPT=  1       2        3        4        5        6      7
//                 corresponds to :
//    KP= 0,0+  1-,1,1+  2-,2,2+  3-,3,3+  4-,4,4+  5-,5,5+  > =6-
//    Default value is 1
// 
// void SetInternalField(G4String aString)
//    Defines the geomagnetic field model
//    Possible values for aString are IGRF, and GEODIPOLE
//    
// void SetExternalField(G4String aString)
//    Defines the Earth's external magnetospheric magnetic field model
//    Possible values for aString are NOFIELD, TSY89, TSY96 and TSY2001
//
// void SetMagnetopauseModel(G4String aString)
//    Defines the magnetopause model 
//    Possible values are IGRF, TSY89, TSY96 and TSY2001  
//  
// void Setnm_igrf(G4int n)
//     Defines the maximum  order n of harmonics to be considered in IGRF
//     min value 1
//     max value 10
//     Default is 10
//
// void SetDipoleB0(G4double val)
//     Set the geomagnetic dipole momentum BO
//      
// void SetDipolePS(G4double aVal)
//     Set the geomagnetic dipole tilt angle, the dipole axis is computed
//     according to this value
// 
// void SetDipoleAxis(G4double Theta,G4double Phi)
//     Set the geomagnetic dipole axis, the tilt angle is computed according
//     to this value 
//
// void SetDipoleSchift(G4ThreeVector aVec)
//     The center of the geomagnetic dipole is schifted from the Earth's
//     centrum by aVec in GEO coordinate 
// 
// void SetTiltedDipoleParameterFromIGRF()
//     The geomagnetic dipole parameter: Bo, axis and tilt angle are deduced  
//     from the 1st order IGRF cooeficientd for the date define by StarDate and
//     timeOfB. The Geomagnetic dipole is not schifted from the Earth's center
//
// void SetEccentricDipoleParameterFromIGRF()
//     The geomagnetic dipole parameter: Bo, dipoleAxis and tilt_angle 
//     are dededuced from the 1st order IGRF cooeficients for the date define by StarDate and
//     timeOfB. The Geomagnetic dipole is translated from the Earth's center
//     by a vector eddeduced from the 1st and 2nd order IGRF cooeficients.
//
// void SetPdyn(G4double aVal)
//     Set the solar wind dynamic pressure Pdyn (nV^2) parameters used in the tsyganenko
//     96 and 2001 model.
//
// void SetDst(G4double aVal)
//     Set the disturbance storm index (Dst) prameter used in the Tsyganenko 96
//     and 2001 model
// 
// void SetImfy(G4double aVal)
//     Set the GSM y component of the interplanetary magnetic field. This
//     parameter is used in the Tsyganenko96 and 2001 model.
//
// void SetImfz(G4double aVal)
//     Set the GSM z component of the interplanetary magnetic field. This
//     parameter is used in the Tsyganenko96 and 2001 model.
// 
// void SetG1(G4double aVal)
//     Set the G1 parameter used in the Tsyganenko 2001 model
// 
// void SetG2(G4double aVal)
//     Set the G2 parameter used in the Tsyganenko 2001 model
//
// void SetEpsilon(G4double aVal)
//     Set the maximum relative error for integration.
//
// void SetDeltaIntersection(G4double aVal)
//     Set the precison for crossing boundary when integrating the equation
//     of motion.
//
// void SetDeltaChord(G4double aVal)
//     Set the maximum infinitesimal step allowed by the ChordFinder.
//
// void ResetIntegrationParameters()
//     Reset the integration parameters to their default values
//  
// void PrintStormParameter()
//     Print the value of the parameters iopt, Pdyn, Dst, Imfy, Imfz, G1,G2 
//     used in the Tsyganenko 96 and 2001 model 
//
// void ReadTSYParameter(G4string filename)
//     Read the  start date and series of the parameters iopt, Pdyn, Dst, Imfy, Imfz, 
//     G1 and G2 in function of time t. The time t is defined in second 
//     form the start date
//
// void ReadTSYParameter(G4string filename)
//     Read the  start date and series of the parameters iopt, Pdyn, Dst, Imfy, Imfz, 
//     G1 and G2 in function of time t. The time t is defined in second 
//     form the start date 
    
  
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// CHANGE HISTORY
// --------------
//
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
////////////////////////////////////////////////////////////////////////////////
//

#include "globals.hh"
#include "G4ios.hh"

#include "G4MagneticField.hh"
#include "G4Mag_EqRhs.hh"
#include "G4ThreeVector.hh"
#include "vector"
#include "G4strstreambuf.hh"
#include "DateAndTime.hh"

class ECRSEquationOfMotion;
class ECRSFieldMessenger;
class G4ChordFinder;
class G4MagIntegratorStepper;


class ECRSMagneticField : public G4MagneticField
{
public:
  //constructor destructor	       
  ECRSMagneticField() ;
  
  ~ECRSMagneticField() ;
  
  
  // Gives the magnetic field B at a given position defined by yTrack
  void GetFieldValue(const G4double yTrack[],G4double B[]) const;
  G4ThreeVector GetFieldValue(const G4ThreeVector GEOposition) const; 
  // magnetopause
  bool OutsideMagnetosphere(G4ThreeVector pos) const;
  G4ThreeVector FindPositionOnMagnetopause(G4double theta,
					   G4double xgsm,
					   G4double precision) const;
  G4ThreeVector FindStandOffPosition(G4double precision) const;
  
  //Set methods 
  void SetTimeOfB(G4double val);	
  void SetStartDate(G4int Year, G4int Month,G4int Day,
		    G4int Hour, G4int Minute,G4int Second );
  void SetIopt(G4int val); 
  void SetInternalField(G4String aString);
  void SetExternalField(G4String aString);
  void SetMagnetopauseModel(G4String aString);
  inline void Setnm_igrf(G4int aVal) {nm_igrf=aVal;}
  void SetDipoleB0(G4double aVal);
  inline void SetDipolePS(G4double aVal){DipolePS=aVal;}
  inline void SetDipoleAxis(G4double Theta,G4double Phi)
  {
    DipoleTheta=Theta;
    DipolePhi=Phi;
  }
  inline void SetDipoleSchift(G4ThreeVector aVec){DipoleSchift=aVec;}
  void SetStepper(G4String aString); 
  
  // Define the geomagntic dipole parameter according to IGRF
  // coefficients
  void SetTiltedDipoleParameterFromIGRF();
  void SetEccentricDipoleParameterFromIGRF();		 
  
  //Method for geomagnetic and sw parameters used in the Tsyganenko
  //models 
  inline void SetPdyn(G4double aVal){Pdyn = aVal;}
  inline void SetDst(G4double aVal){Dst = aVal;}
  inline void SetImfy(G4double aVal){Imfy = aVal;}
  inline void SetImfz(G4double aVal){Imfz = aVal;}
  inline void SetG1(G4double aVal){G1 = aVal;}
  inline void SetG2(G4double aVal){G2 = aVal;}
  
  void ResetIntegrationParameters();
  void SelectBulirshStoerMethod();
  void SelectG4IntegrationMethods();
  void SetEpsilon(G4double aVal);
  void SetDeltaChord(G4double aVal);
  void SetBSMaxStep(G4double aVal);
  void SetDeltaIntersection(G4double aVal); 
  
  void PrintStormParameter();
  void ReadTSYParameter(G4String nameFile);
  void ReadIgrfTable(G4String name_file);
  void ComputeIgrfCoefAccordingToTime();
  void PrintBfield(G4ThreeVector geo_pos) const; 
  
  inline ECRSEquationOfMotion* GetEquationOfMotion()
  {
    return fEquationOfMotion;
  }
  
  inline G4ThreeVector GetDipoleSchift()
  {
    return DipoleSchift;
  }
  
  inline G4double GetDipoleB0()
  {
    return DipoleB0;
  }
  
  inline G4double GetDipolePhi()
  {
    return DipolePhi;
  }

  inline G4double GetDipoleTheta()
  {
    return DipoleTheta;
  }
  
  inline void SetConsiderDipoleShift(G4bool abool)
  {
    ConsiderDipoleShift = abool;
  }
	  			       				       				
	 
protected:

private:

  // messenger
  ECRSFieldMessenger* theFieldMessenger;
  
  //Magnetic field model parameters  
  G4double TimeOfB;
  G4double year_igrf;
  DateAndTime StartDate,ReferenceDate;
  G4bool External,Internal,ConsiderDipoleShift; 
  G4int iopt;
  G4double DipoleB0,DipoleTheta,DipolePhi,DipolePS;
  G4ThreeVector DipoleSchift;
  G4double *times_of_data, *Pdyn_data, *Dst_data, *Imf_gsm_y_data, 
    *Imf_gsm_z_data,
    *g1_tsy_param_data,*g2_tsy_param_data;
  G4int n_tsy_data,nm_igrf;
  G4double Pdyn,Dst,Imfy,Imfz,G1,G2;
  
  //Igrf coefficient table
  std::vector<float>  igrf_year;
  std::vector< std::vector<float > > h_nm;
  std::vector< std::vector<float > > g_nm;
  std::vector<float>  dh_nm;
  std::vector<float>  dg_nm;
  
  //coefficient for the recurrence equation of legendre assocated function
  std::vector<float> recurrence_coef;
  G4int nmax;
  
  //Igrf coefficient according to reference time 
  std::vector<float> hh_nm;
  std::vector<float> gg_nm;
  
  //equation of motion 
  ECRSEquationOfMotion* fEquationOfMotion; 
  
  //attribute for the integration method 
  G4ChordFinder* fChordFinder;
  G4MagIntegratorStepper* fStepper;
  G4double DefaultDeltaChord,DefaultBSMaxStep;
  G4double DeltaChord;
  G4double DefaultDeltaIntersection;
  G4double DefaultEpsilon;
  
  //private methods       
private:     
  //IGRF model
  G4ThreeVector  GetIGRFFortran(G4ThreeVector pos) const; 
  G4ThreeVector  GetIGRF(G4ThreeVector pos) const;  
  
  //Geomagnetic dipole in GEO coordinate
  G4ThreeVector  GetGEODipole(G4ThreeVector pos) const;
  
  //Tsyganenko89 model 
  G4ThreeVector  GetTSY89(G4ThreeVector pos) const;
  G4ThreeVector  GetTSYBOB89(G4ThreeVector pos) const;
  
  //Tsyganenko96 model 
  G4ThreeVector  GetTSY96(G4ThreeVector pos) const;
  
  //Tsyganenko2001 model
  G4ThreeVector  GetTSY2001(G4ThreeVector pos) const;
  
  //pointers on the selected Internal and External field model
  G4ThreeVector (ECRSMagneticField::* GetInternalField)(G4ThreeVector) const;
  G4ThreeVector (ECRSMagneticField::* GetExternalField)(G4ThreeVector) const;
  
  //pointer on magnetopause model
  bool (ECRSMagneticField::*SelectedOutsideMagnetosphere)
  (G4ThreeVector pos) const;
  
  //IGRF and geomagnetic magnetopause model r>25. re is outside
  //magnetosphere
  bool IGRFOutsideMagnetosphere(G4ThreeVector pos) const; 
  
  
  //tsy89 magnetopause model
  bool TSY89OutsideMagnetosphere(G4ThreeVector pos) const;
  
  //tsy89 magnetopause model
  bool TSY96OutsideMagnetosphere(G4ThreeVector pos) const;
  
  //tsy2001 magnetopause model
  bool TSY2001OutsideMagnetosphere(G4ThreeVector pos) const;
  
  void ComputeTSYParameter(G4double t);	   
} ;

#endif
