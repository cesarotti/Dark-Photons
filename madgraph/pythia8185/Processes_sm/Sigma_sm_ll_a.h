//==========================================================================
// This file has been automatically generated for Pythia 8
// MadGraph5_aMC@NLO v. 2.1.1, 2014-03-31
// By the MadGraph5_aMC@NLO Development Team
// Visit launchpad.net/madgraph5 and amcatnlo.web.cern.ch
//==========================================================================

#ifndef Pythia8_Sigma_sm_ll_a_H
#define Pythia8_Sigma_sm_ll_a_H

#include <complex> 

#include "SigmaProcess.h"
#include "Parameters_sm.h"

using namespace std; 

namespace Pythia8 
{
//==========================================================================
// A class for calculating the matrix elements for
// Process: e+ e- > a QED=99 QCD=99 @1
//--------------------------------------------------------------------------

class Sigma_sm_ll_a : public Sigma1Process 
{
  public:

    // Constructor.
    Sigma_sm_ll_a() {}

    // Initialize process.
    virtual void initProc(); 

    // Calculate flavour-independent parts of cross section.
    virtual void sigmaKin(); 

    // Evaluate sigmaHat(sHat).
    virtual double sigmaHat(); 

    // Select flavour, colour and anticolour.
    virtual void setIdColAcol(); 

    // Evaluate weight for decay angles.
    virtual double weightDecay(Event& process, int iResBeg, int iResEnd); 

    // Info on the subprocess.
    virtual string name() const {return "ll_a (sm)";}

    virtual int code() const {return 10101;}

    virtual string inFlux() const {return "ffbarSame";}

    virtual int resonanceA() const {return 22;}
    // Tell Pythia that sigmaHat returns the ME^2
    virtual bool convertM2() const {return true;}

  private:

    // Private functions to calculate the matrix element for all subprocesses
    // Calculate wavefunctions
    void calculate_wavefunctions(const int perm[], const int hel[]); 
    static const int nwavefuncs = 3; 
    std::complex<double> w[nwavefuncs][18]; 
    static const int namplitudes = 1; 
    std::complex<double> amp[namplitudes]; 
    double matrix_1_epem_a(); 

    // Constants for array limits
    static const int nexternal = 3; 
    static const int nprocesses = 1; 

    // Store the matrix element value from sigmaKin
    double matrix_element[nprocesses]; 

    // Color flows, used when selecting color
    double * jamp2[nprocesses]; 

    // Pointer to the model parameters
    Parameters_sm * pars; 

}; 

}  // end namespace Pythia8

#endif  // Pythia8_Sigma_sm_ll_a_H
