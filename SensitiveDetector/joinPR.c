#include <iostream>
#include <string>
#include "TROOT.h"
#include "TRint.h"
#include "TCanvas.h"
#include "TFile.h"
//#include ROOT
int joinPR()
{
	TString endfile;
	Double_t events;
	cin >> events;
	cin >> endfile;
	cout << events << endl;
	cout << endfile << endl;
	// std::string endfile ("NuOffMagnet.root");
	int n = 8;
	// Double_t events = TMath::Power(10, 6);
	Double_t rate = (100*TMath::Power(10, -6))/(208*events*1.602*TMath::Power(10, -19));

	Double_t energy_var_n;
	Double_t time_var_n;
	Double_t vertexz_var_n;
	Char_t process_var_n;

	TString start = TString("Project_t");
	TString ending = TString(".root");

	TH1D *neDEn = new TH1D("Energy", "Particles Distribution",80,0,110);
	TH1D *neDT = new TH1D("Time", "Particles Distribution",80,0,200);
	TH1D *neDVZ = new TH1D("VertexPositionZ", "Particles Distribution",80,0,500);
	TH1D *neDPr = new TH1D("Process", "Particles Distribution",10,0,10);

	Int_t j;
	for (int j = 0; j < n; j++) {
		TString number = "";
		number += j;
		TString input = start + number + ending;
		cout << input << endl;

		TFile file0(input);
		TTree *treeD0n; file0.GetObject("Hits",treeD0n);

		Int_t nentries0n = Int_t(treeD0n->GetEntries());
		treeD0n->SetBranchAddress("Energy",&energy_var_n);
		treeD0n->SetBranchAddress("Time",&time_var_n);
		treeD0n->SetBranchAddress("VertexPositionZ",&vertexz_var_n);
		treeD0n->SetBranchAddress("Process",&process_var_n);
		for (Int_t i=0; i<nentries0n; i++) {
			treeD0n->GetEntry(i);
			neDEn->Fill(energy_var_n);
			neDT->Fill(time_var_n);
			neDVZ->Fill(vertexz_var_n);
			neDPr->Fill(&process_var_n,1);
		}
	}

	neDEn->Scale(rate);
	neDT->Scale(rate);
	neDVZ->Scale(rate);
	neDPr->Scale(rate);

	neDEn->SetXTitle("Energy [MeV]");
	neDEn->SetYTitle("Rate [s^{-1}]");

	neDT->SetXTitle("Time [ns]");
	neDT->SetYTitle("Rate [s^{-1}]");

	neDVZ->SetXTitle("Vertex Position Z [nm]");
	neDVZ->SetYTitle("Rate [s^{-1}]");

	neDPr->SetXTitle("Process name");
	neDPr->SetYTitle("Rate [s^{-1}]");


	TFile fnew(endfile,"recreate");

	neDEn->Write();
	neDT->Write();
	neDVZ->Write();
	neDPr->Write();

	fnew.Close();
	return 0;
}
