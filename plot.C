#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TRandom3.h"

/**
 *
 * Data from ntuple is given in mm!
 *
 **/

/*
 * The first method is skipped by the ROOT compiler, apparently. This is 
 * to compile the distance() method
 */
void why() 
{
}

/*
 * Euclidean metric defined distance between two points
 */
double distance(double x1, double y1, double x2, double y2)
{
  x1 = (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
  return sqrt(x1);
}

/*
 *Crude estimation as to whether events overlap or not
 */
bool checkOverlap(double X[], double Y[], int length, double rad)
{
  double x1, x2, y1, y2;
  for (int a=0; a<(length-1); a++){
    for (int b=a+1; b<length;b++){
      x1 = X[a]; y1 = Y[a]; x2 = X[b]; y2 = Y[b];
      if (((x1==0)&&(y1==0))||((x2==0)&&(y2==0))) //don't look at misses
	{
         continue;}
      double dist = distance(x1, y1, x2, y2);
      if (dist < 2*rad)
         return true;
        }
      }
  return false;
}

void plot(){

  cout << "...Plotting begin..." << endl;

  double xPos, yPos;

  double radius = 1.5*50; //crystal face is 5 cm long, 3 crystal consideration

  TRandom3* randomGen = new TRandom3(12191982);
 
  TFile* file= new TFile("complete.root");

  TTree* tree = (TTree *)file->Get("ntuples/Gamma_Info");

  int nEvents =  tree->GetEntries();

  tree->SetBranchAddress("photon1xPos",&xPos);
  tree->SetBranchAddress("photo1yPos", &yPos);

  TH1D* histo_prob = new TH1D("histo_prob", "(prob of >=1 overlap) vs. (avg. number of photons in detector == mu", 20, 0.5, 20.5);

  TH2D* det = new TH2D("det", "det", 100, -1000, 1000, 100, -1000, 1000);

  int nMax = 1000;

  //tests mu values 1...20
  
 for (int mu = 1; mu < 21; mu++ ) 
    {  
      for (int i=0; i<nMax; i++) //run a thousand tries
      {
	int n = randomGen->Poisson(mu); //number of photons found in detector in one memory time

	//Jim's method is much cleaner

	if (n<2) {continue;} //easy enough to distinguish?

	const int N = n;
	double* xPosition = NULL;
	double* yPosition = NULL;
	xPosition = new double[N];
	yPosition = new double[N];

	while(n>0) {

       	  tree->GetEntry(int(randomGen->Uniform(0, nEvents)));
	  xPosition[N-n] = xPos;
      	  yPosition[N-n] = yPos;
	  det->Fill(xPos, yPos);
	  n--;
	}
	if (checkOverlap(xPosition, yPosition, N, radius))
	  {histo_prob->Fill((double)mu);
	  }
	delete [] xPosition;
	delete [] yPosition;
	xPosition = NULL;
	yPosition = NULL;
      }


    }
  
  TCanvas* canvas = new TCanvas("canvas", "canvas", 1000, 500);
  canvas->Divide(2,2);
  double entries = histo_prob->GetEntries();  

  histo_prob->Scale(1./(entries));
  histo_prob->SetMaximum(1.0);
  canvas->cd(1); histo_prob->DrawCopy(); delete histo_prob;
  canvas->cd(2); det->DrawCopy(); delete det;

}
