//==========================================================================
// This file has been automatically generated for Pythia 8 by
// MadGraph5_aMC@NLO v. 2.1.1, 2014-03-31
// By the MadGraph5_aMC@NLO Development Team
// Visit launchpad.net/madgraph5 and amcatnlo.web.cern.ch
//==========================================================================

#include "Sigma_sm_ll_all.h"
#include "HelAmps_sm.h"

using namespace Pythia8_sm; 

namespace Pythia8 
{

//==========================================================================
// Class member functions for calculating the matrix elements for
// Process: e+ e- > a e+ e- QED=99 QCD=99 @1

//--------------------------------------------------------------------------
// Initialize process.

void Sigma_sm_ll_all::initProc() 
{
  // Instantiate the model class and set parameters that stay fixed during run
  pars = Parameters_sm::getInstance(); 
  pars->setIndependentParameters(particleDataPtr, couplingsPtr, slhaPtr); 
  pars->setIndependentCouplings(); 
  // Set massive/massless matrix elements for c/b/mu/tau
  mcME = 0.; 
  mbME = particleDataPtr->m0(5); 
  mmuME = 0.; 
  mtauME = particleDataPtr->m0(15); 
  jamp2[0] = new double[1]; 
}

//--------------------------------------------------------------------------
// Evaluate |M|^2, part independent of incoming flavour.

void Sigma_sm_ll_all::sigmaKin() 
{
  // Set the parameters which change event by event
  pars->setDependentParameters(particleDataPtr, couplingsPtr, slhaPtr, alpS); 
  pars->setDependentCouplings(); 
  // Reset color flows
  for(int i = 0; i < 1; i++ )
    jamp2[0][i] = 0.; 

  // Local variables and constants
  const int ncomb = 32; 
  static bool goodhel[ncomb] = {ncomb * false}; 
  static int ntry = 0, sum_hel = 0, ngood = 0; 
  static int igood[ncomb]; 
  static int jhel; 
  double t[nprocesses]; 
  // Helicities for the process
  static const int helicities[ncomb][nexternal] = {{-1, -1, -1, -1, -1}, {-1,
      -1, -1, -1, 1}, {-1, -1, -1, 1, -1}, {-1, -1, -1, 1, 1}, {-1, -1, 1, -1,
      -1}, {-1, -1, 1, -1, 1}, {-1, -1, 1, 1, -1}, {-1, -1, 1, 1, 1}, {-1, 1,
      -1, -1, -1}, {-1, 1, -1, -1, 1}, {-1, 1, -1, 1, -1}, {-1, 1, -1, 1, 1},
      {-1, 1, 1, -1, -1}, {-1, 1, 1, -1, 1}, {-1, 1, 1, 1, -1}, {-1, 1, 1, 1,
      1}, {1, -1, -1, -1, -1}, {1, -1, -1, -1, 1}, {1, -1, -1, 1, -1}, {1, -1,
      -1, 1, 1}, {1, -1, 1, -1, -1}, {1, -1, 1, -1, 1}, {1, -1, 1, 1, -1}, {1,
      -1, 1, 1, 1}, {1, 1, -1, -1, -1}, {1, 1, -1, -1, 1}, {1, 1, -1, 1, -1},
      {1, 1, -1, 1, 1}, {1, 1, 1, -1, -1}, {1, 1, 1, -1, 1}, {1, 1, 1, 1, -1},
      {1, 1, 1, 1, 1}};
  // Denominators: spins, colors and identical particles
  const int denominators[nprocesses] = {4}; 

  ntry = ntry + 1; 

  // Reset the matrix elements
  for(int i = 0; i < nprocesses; i++ )
  {
    matrix_element[i] = 0.; 
    t[i] = 0.; 
  }

  // Define permutation
  int perm[nexternal]; 
  for(int i = 0; i < nexternal; i++ )
  {
    perm[i] = i; 
  }

  // For now, call setupForME() here
  id1 = -11; 
  id2 = 11; 
  if( !setupForME())
  {
    return; 
  }

  if (sum_hel == 0 || ntry < 10)
  {
    // Calculate the matrix element for all helicities
    for(int ihel = 0; ihel < ncomb; ihel++ )
    {
      if (goodhel[ihel] || ntry < 2)
      {
        calculate_wavefunctions(perm, helicities[ihel]); 
        t[0] = matrix_1_epem_aepem(); 

        double tsum = 0; 
        for(int iproc = 0; iproc < nprocesses; iproc++ )
        {
          matrix_element[iproc] += t[iproc]; 
          tsum += t[iproc]; 
        }
        // Store which helicities give non-zero result
        if (tsum != 0. && !goodhel[ihel])
        {
          goodhel[ihel] = true; 
          ngood++; 
          igood[ngood] = ihel; 
        }
      }
    }
    jhel = 0; 
    sum_hel = min(sum_hel, ngood); 
  }
  else
  {
    // Only use the "good" helicities
    for(int j = 0; j < sum_hel; j++ )
    {
      jhel++; 
      if (jhel >= ngood)
        jhel = 0; 
      double hwgt = double(ngood)/double(sum_hel); 
      int ihel = igood[jhel]; 
      calculate_wavefunctions(perm, helicities[ihel]); 
      t[0] = matrix_1_epem_aepem(); 

      for(int iproc = 0; iproc < nprocesses; iproc++ )
      {
        matrix_element[iproc] += t[iproc] * hwgt; 
      }
    }
  }

  for (int i = 0; i < nprocesses; i++ )
    matrix_element[i] /= denominators[i]; 



}

//--------------------------------------------------------------------------
// Evaluate |M|^2, including incoming flavour dependence.

double Sigma_sm_ll_all::sigmaHat() 
{
  // Select between the different processes
  if(id1 == -11 && id2 == 11)
  {
    // Add matrix elements for processes with beams (-11, 11)
    return matrix_element[0]; 
  }
  else
  {
    // Return 0 if not correct initial state assignment
    return 0.; 
  }
}

//--------------------------------------------------------------------------
// Select identity, colour and anticolour.

void Sigma_sm_ll_all::setIdColAcol() 
{
  if(id1 == -11 && id2 == 11)
  {
    // Pick one of the flavor combinations (22, -11, 11)
    int flavors[1][3] = {{22, -11, 11}}; 
    vector<double> probs; 
    double sum = matrix_element[0]; 
    probs.push_back(matrix_element[0]/sum); 
    int choice = rndmPtr->pick(probs); 
    id3 = flavors[choice][0]; 
    id4 = flavors[choice][1]; 
    id5 = flavors[choice][2]; 
  }
  setId(id1, id2, id3, id4, id5); 
  // Pick color flow
  int ncolor[1] = {1}; 
  if(id1 == -11 && id2 == 11 && id3 == 22 && id4 == -11 && id5 == 11)
  {
    setColAcol(0, 0, 0, 0, 0, 0); 
  }
}

//--------------------------------------------------------------------------
// Evaluate weight for angles of decay products in process

double Sigma_sm_ll_all::weightDecay(Event& process, int iResBeg, int iResEnd) 
{
  // Just use isotropic decay (default)
  return 1.; 
}

//==========================================================================
// Private class member functions

//--------------------------------------------------------------------------
// Evaluate |M|^2 for each subprocess

void Sigma_sm_ll_all::calculate_wavefunctions(const int perm[], const int hel[])
{
  // Calculate wavefunctions for all processes
  double p[nexternal][4]; 
  int i; 

  // Convert Pythia 4-vectors to double[]
  for(i = 0; i < nexternal; i++ )
  {
    p[i][0] = pME[i].e(); 
    p[i][1] = pME[i].px(); 
    p[i][2] = pME[i].py(); 
    p[i][3] = pME[i].pz(); 
  }

  // Calculate all wavefunctions
  oxxxxx(p[perm[0]], mME[0], hel[0], -1, w[0]); 
  ixxxxx(p[perm[1]], mME[1], hel[1], +1, w[1]); 
  vxxxxx(p[perm[2]], mME[2], hel[2], +1, w[2]); 
  ixxxxx(p[perm[3]], mME[3], hel[3], -1, w[3]); 
  oxxxxx(p[perm[4]], mME[4], hel[4], +1, w[4]); 
  FFV1P0_3(w[1], w[0], pars->GC_3, pars->ZERO, pars->ZERO, w[5]); 
  FFV1_2(w[3], w[2], pars->GC_3, pars->ZERO, pars->ZERO, w[6]); 
  FFV2_4_3(w[1], w[0], pars->GC_50, pars->GC_59, pars->mdl_MZ, pars->mdl_WZ,
      w[7]);
  FFV1_1(w[4], w[2], pars->GC_3, pars->ZERO, pars->ZERO, w[8]); 
  FFV1_1(w[0], w[2], pars->GC_3, pars->ZERO, pars->ZERO, w[9]); 
  FFV1P0_3(w[1], w[4], pars->GC_3, pars->ZERO, pars->ZERO, w[10]); 
  FFV2_4_3(w[1], w[4], pars->GC_50, pars->GC_59, pars->mdl_MZ, pars->mdl_WZ,
      w[11]);
  FFV1P0_3(w[3], w[4], pars->GC_3, pars->ZERO, pars->ZERO, w[12]); 
  FFV2_4_3(w[3], w[4], pars->GC_50, pars->GC_59, pars->mdl_MZ, pars->mdl_WZ,
      w[13]);
  FFV1P0_3(w[3], w[0], pars->GC_3, pars->ZERO, pars->ZERO, w[14]); 
  FFV1_2(w[1], w[2], pars->GC_3, pars->ZERO, pars->ZERO, w[15]); 
  FFV2_4_3(w[3], w[0], pars->GC_50, pars->GC_59, pars->mdl_MZ, pars->mdl_WZ,
      w[16]);

  // Calculate all amplitudes
  // Amplitude(s) for diagram number 0
  FFV1_0(w[6], w[4], w[5], pars->GC_3, amp[0]); 
  FFV2_4_0(w[6], w[4], w[7], pars->GC_50, pars->GC_59, amp[1]); 
  FFV1_0(w[3], w[8], w[5], pars->GC_3, amp[2]); 
  FFV2_4_0(w[3], w[8], w[7], pars->GC_50, pars->GC_59, amp[3]); 
  FFV1_0(w[3], w[9], w[10], pars->GC_3, amp[4]); 
  FFV2_4_0(w[3], w[9], w[11], pars->GC_50, pars->GC_59, amp[5]); 
  FFV1_0(w[1], w[9], w[12], pars->GC_3, amp[6]); 
  FFV2_4_0(w[1], w[9], w[13], pars->GC_50, pars->GC_59, amp[7]); 
  FFV1_0(w[15], w[4], w[14], pars->GC_3, amp[8]); 
  FFV2_4_0(w[15], w[4], w[16], pars->GC_50, pars->GC_59, amp[9]); 
  FFV1_0(w[1], w[8], w[14], pars->GC_3, amp[10]); 
  FFV2_4_0(w[1], w[8], w[16], pars->GC_50, pars->GC_59, amp[11]); 
  FFV1_0(w[15], w[0], w[12], pars->GC_3, amp[12]); 
  FFV2_4_0(w[15], w[0], w[13], pars->GC_50, pars->GC_59, amp[13]); 
  FFV1_0(w[6], w[0], w[10], pars->GC_3, amp[14]); 
  FFV2_4_0(w[6], w[0], w[11], pars->GC_50, pars->GC_59, amp[15]); 


}
double Sigma_sm_ll_all::matrix_1_epem_aepem() 
{
  int i, j; 
  // Local variables
  const int ngraphs = 16; 
  const int ncolor = 1; 
  std::complex<double> ztemp; 
  std::complex<double> jamp[ncolor]; 
  // The color matrix;
  static const double denom[1] = {1.}; 
  static const double cf[1][1] = {1.}; 

  // Calculate color flows
  jamp[0] = -amp[0] - amp[1] - amp[2] - amp[3] + amp[4] + amp[5] - amp[6] -
      amp[7] + amp[8] + amp[9] + amp[10] + amp[11] - amp[12] - amp[13] +
      amp[14] + amp[15];

  // Sum and square the color flows to get the matrix element
  double matrix = 0; 
  for(i = 0; i < ncolor; i++ )
  {
    ztemp = 0.; 
    for(j = 0; j < ncolor; j++ )
      ztemp = ztemp + cf[i][j] * jamp[j]; 
    matrix = matrix + real(ztemp * conj(jamp[i]))/denom[i]; 
  }

  // Store the leading color flows for choice of color
  for(i = 0; i < ncolor; i++ )
    jamp2[0][i] += real(jamp[i] * conj(jamp[i])); 

  return matrix; 
}


}  // end namespace Pythia8
