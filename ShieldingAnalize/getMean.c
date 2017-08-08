#include <iostream>
#include <string>
#include "TROOT.h"
#include "TRint.h"
#include "TCanvas.h"
#include "TFile.h"
//#include ROOT
int getNumber()
{
	int n = 4;
	Double_t energy_var_n;
	Int_t part_var_n;
	Char_t name_var_n;

	std::vector<Int_t> part;

	TString start = TString("Project_t");
	TString ending = TString(".root");


	Int_t j;
	Int_t k=0;
	for (int j = 0; j < n; j++) {
		TString number = "";
		number += j;
		TString input = start + number + ending;

		TFile file0(input);
		TString particle;
		particle = "Gamma";
		TTree *treeD0n; file0.GetObject(particle,treeD0n);

		Int_t nentries0n = Int_t(treeD0n->GetEntries());
		k+=nentries0n;
		}

	cout << "Gamma: " << k << endl;

	k=0;
	for (int j = 0; j < n; j++) {
		TString number = "";
		number += j;
		TString input = start + number + ending;

		TFile file0(input);
		TString particle;
		particle = "Neutron";
		TTree *treeD0n; file0.GetObject(particle,treeD0n);

		Int_t nentries0n = Int_t(treeD0n->GetEntries());
		k+=nentries0n;
	}
	cout << "Neutron: " << k << endl;
	return 0;
}
