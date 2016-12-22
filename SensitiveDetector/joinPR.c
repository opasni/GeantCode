#include <iostream>
#include <string>
#include "TROOT.h"
#include "TRint.h"
#include "TCanvas.h"
#include "TFile.h"
//#include ROOT
int joinhist()
{
	std::string endfile ("NuOffMagnet.root");
	int n = 4;
	Double_t column_var_n;  
	Double_t row_var_n; 
	std::string start ("Project_t");
	std::string ending (".root");
	std::string number;
	std::string input;

	TH1F *neDCn = new TH1F("ColumnDn", "Particles Distribution",40,0,40);
	TH1F *neDRn = new TH1F("RowDn", "Particles Distribution",40,0,40);

	TH2F *ne2DN = new TH2F("Neutron", "Particles Distribution",40,0,40,40,0,40);

	//TTree treeDe("d0e","Electron");
    //treeDe.Branch("RowID", &row_var_e, "RowID/D");
    //treeDe.Branch("ColumnID", &column_var_e, "ColumnID/D");
	for (int i = 0; i < n; i++) {
		number = std::to_string(i);
		input = start + number + ending;
		cout << input << endl;

		const char *inputfile = input.c_str();

		TFile file0(inputfile);
		TTree *treeD0n; file0.GetObject("d0n",treeD0n);

		Int_t nentries0n = Int_t(treeD0n->GetEntries());
		treeD0n->SetBranchAddress("ColumnID",&column_var_n);
		treeD0n->SetBranchAddress("RowID",&row_var_n);
		for (Int_t i=0; i<nentries0n; i++) {
			treeD0n->GetEntry(i);
			neDCn->Fill(column_var_n);
			neDRn->Fill(row_var_n);
			ne2DN->Fill(row_var_n,column_var_n);
		}
	}

	TFile fnew(endfile.c_str(),"recreate");

	neDCn->Write(); 
	neDRn->Write(); 

	ne2DN->Write(); 
	fnew.Close();
	return 0;
}

