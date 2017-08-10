#include <iostream>
#include <string>
#include "TROOT.h"
#include "TRint.h"
#include "TCanvas.h"
#include "TFile.h"
//#include ROOT
void getMean()
{
	int n = 4;
	Double_t energy_var_n;
	Int_t part_var_n;
	Char_t name_var_n;

	std::vector<Int_t> part;

	TString input = TString("Project.root");
	TString particleG = TString("PartNumG");
	TString particleN = TString("PartNumN");
	TFile *file = new TFile(input);
	TH1D *numG = (TH1D *)file->Get(particleG);
	TH1D *numN = (TH1D *)file->Get(particleN);

	cout << "Gamma mean: " << numG->GetMean() << " and Neutron mean: " << numN->GetMean() << endl;

}
