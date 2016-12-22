#include <iostream>
#include <string>
#include "TROOT.h"
#include "TRint.h"
#include "TCanvas.h"
#include "TFile.h"
//#include ROOT
int joinhist()
{

	std::string endfile ("Detector.root");
	int n = 4;
	Double_t column_var_e;  
	Double_t row_var_e; 
	Double_t column_var_g;  
	Double_t row_var_g; 
	Double_t column_var_n;  
	Double_t row_var_n; 
	std::string start ("Project_t");
	std::string ending (".root");
	std::string number;
	std::string input;


	TTree treeDe("d0e","Electron");
    treeDe.Branch("RowID", &row_var_e, "RowID/D");
    treeDe.Branch("ColumnID", &column_var_e, "ColumnID/D");
	for (int i = 0; i < n; i++) {
		number = std::to_string(i);
		input = start + number + ending;
		cout << input << endl;

		const char *inputfile = input.c_str();

		TFile file0(inputfile);
		TTree *treeD0e; file0.GetObject("d0e",treeD0e);
		TTree *treeD0g; file0.GetObject("d0g",treeD0g);
		TTree *treeD0n; file0.GetObject("d0n",treeD0n);

		Int_t nentries0e = Int_t(treeD0e->GetEntries());
		treeD0e->SetBranchAddress("RowID",&row_var_e);
		treeD0e->SetBranchAddress("ColumnID",&column_var_e);
		for (Int_t i=0; i<nentries0e; i++) {
			treeD0e->GetEntry(i);
			treeDe.Fill();
		}
	}


	TTree treeDg("d0g","Gamma");
    treeDg.Branch("RowID", &row_var_g, "RowID/D");
    treeDg.Branch("ColumnID", &column_var_g, "ColumnID/D");
	for (int i = 0; i < n; i++) {
		number = std::to_string(i);
		input = start + number + ending;
		cout << input << endl;

		const char *inputfile = input.c_str();

		TFile file0(inputfile);
		TTree *treeD0g; file0.GetObject("d0g",treeD0g);

		Int_t nentries0g = Int_t(treeD0g->GetEntries());
		treeD0g->SetBranchAddress("RowID",&row_var_g);
		treeD0g->SetBranchAddress("ColumnID",&column_var_g);
		for (Int_t i=0; i<nentries0g; i++) {
			treeD0g->GetEntry(i);
			treeDg.Fill();
		}
	}


	TTree treeDn("d0n","Neutron");
    treeDn.Branch("RowID", &row_var_n, "RowID/D");
    treeDn.Branch("ColumnID", &column_var_n, "ColumnID/D");
	for (int i = 0; i < n; i++) {
		number = std::to_string(i);
		input = start + number + ending;
		cout << input << endl;

		const char *inputfile = input.c_str();

		TFile file0(inputfile);
		TTree *treeD0n; file0.GetObject("d0n",treeD0n);

		Int_t nentries0n = Int_t(treeD0n->GetEntries());
		treeD0n->SetBranchAddress("RowID",&row_var_n);
		treeD0n->SetBranchAddress("ColumnID",&column_var_n);
		for (Int_t i=0; i<nentries0n; i++) {
			treeD0n->GetEntry(i);
			treeDn.Fill();
		}
	}


	TFile fnew(endfile.c_str(),"recreate");

	treeDe.Write(); 
	treeDg.Write(); 
	treeDn.Write(); 
	fnew.Close();
   return 0;
}

