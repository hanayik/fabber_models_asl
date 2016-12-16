/*  fwdmodel_asl_pvc.h - Partial Volume Correction resting state ASL model (Buxton)

    Michael Chappell, FMRIB Image Analysis Group

    Copyright (C) 2009 University of Oxford  */

/*  CCOPYRIGHT */

#include "fabber_core/fwdmodel.h"
#include "fabber_core/inference.h"
#include <string>
using namespace std;

class ASL_PVC_FwdModel : public FwdModel {
public: 
  // Virtual function overrides
  virtual void Evaluate(const ColumnVector& params, 
			      ColumnVector& result) const;
  static void ModelUsage();
  virtual string ModelVersion() const;
                  
  virtual void DumpParameters(const ColumnVector& vec,
                                const string& indents = "") const;
                                
  virtual void NameParams(vector<string>& names) const;     
  virtual int NumParams() const 
  { return (infertiss?2:0) - (singleti?1:0) + (infertiss?(infertau?1:0):0) + (inferart?2:0) + (infert1?2:0) + (infertaub?1:0)  + (inferwm?(2+(infertau?1:0)+(infert1?1:0)+(usepve?2:0)):0);  

    //return 2 - (singleti?1:0) + (infertau?1:0) + (inferart?2:0) + (infert1?2:0) + (inferinveff?1:0) + (infertrailing?1:0) + (infertaub?1:0); 
  } 

  virtual ~ASL_PVC_FwdModel() { return; }

  virtual void HardcodedInitialDists(MVNDist& prior, MVNDist& posterior) const;

  using FwdModel::SetupARD;
  virtual void SetupARD(const MVNDist& posterior, MVNDist& prior, double& Fard);
  virtual void UpdateARD(const MVNDist& posterior, MVNDist& prior, double& Fard) const;

  // Constructor
  ASL_PVC_FwdModel(ArgsType& args);


protected: // Constants

  // Lookup the starting indices of the parameters
  int tiss_index() const {return (infertiss?1:0);} //main tissue parameters: ftiss and delttiss alway come first

  int tau_index() const {  return (infertiss?2:0) + (infertiss?(infertau?1:0):0);  }

  int art_index() const {  return (infertiss?2:0) + (infertiss?(infertau?1:0):0) + (inferart?1:0); }

  int t1_index() const { return (infertiss?2:0) + (infertiss?(infertau?1:0):0) + (inferart?2:0) + (infert1?1:0); }
  
  //int inveff_index() const { return 2 + (infertau?1:0) + (inferart?2:0) + (infert1?2:0) +(inferinveff?1:0); }

  //int trailing_index() const { return 2 + (infertau?1:0) + (inferart?2:0) + (infert1?2:0) + (infertrailing?1:0); }

  //int taub_index() const { return 2 + (infertau?1:0) + (inferart?2:0) + (infert1?2:0) + (inferinveff?1:0) + (infertrailing?1:0) + (infertaub?1:0);}

  int taub_index() const { return (infertiss?2:0) + (infertiss?(infertau?1:0):0) + (inferart?2:0) + (infert1?2:0) + (infertaub?1:0);}

  //int R_index() const { return 2 + (infertau?1:0) + (inferart?2:0) + (infert1?2:0) + (infertaub?1:0) + (inferart?1:0);}
  int wm_index() const { return (infertiss?2:0) + (infertiss?(infertau?1:0):0) + (inferart?2:0) + (infert1?2:0) + (infertaub?1:0)  + (inferwm?1:0); }

  int pv_index() const { return (infertiss?2:0) + (infertiss?(infertau?1:0):0) + (inferart?2:0) + (infert1?2:0) + (infertaub?1:0)  + (inferwm?(2 + (infertau?1:0) + (infert1?1:0) ):0) + (usepve?1:0); }

  // vector indices for the parameters to expereicne ARD
  vector<int> ard_index;


  // scan parameters
  double seqtau; //bolus length as set by the sequence
  double setdelt;

  int repeats;
  double t1;
  double t1b;
  double t1wm;
  double lambda;
  double pretisat;
  bool grase; //to indicate data was collected with GRASE-ASL
 double slicedt;
 bool casl;

  bool infertiss;
  bool singleti; //specifies that only tissue perfusion should be inferred
  bool infertau;
  bool infertaub;
  bool inferart;
  bool infert1;
  bool inferwm;
  bool usepve;
  //bool inferinveff;
  //bool infertrailing;

  // ard flags
  bool doard;
  bool tissard;
  bool artard;
  bool wmard;

  ColumnVector tis;
  Real timax;


};
