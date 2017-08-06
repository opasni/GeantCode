#include <iostream>
#include <string>
#include "TROOT.h"
#include "TRint.h"
#include "TCanvas.h"
#include "TFile.h"
//#include ROOT
int getNumber()
{
	int n = 8;
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
		cout << input << endl;

		TFile file0(input);
		TString particle;
		// if (invbeta == "yes") particle = "Neutron";
		particle = "TargetWall";
		TTree *treeD0n; file0.GetObject(particle,treeD0n);

		Int_t nentries0n = Int_t(treeD0n->GetEntries());
		treeD0n->SetBranchAddress("PartID",&part_var_n);

		treeD0n->SetBranchAddress("Process",&name_var_n);
		for (Int_t i=0; i<nentries0n; i++) {
			treeD0n->GetEntry(i);
			// cout << &name_var_n << ' ' << part[k] << endl;
			k++;
		}
	}
	cout << k << endl;
	return 0;
}
