#include <iostream>
#include <string>
#include "TROOT.h"
#include "TRint.h"
#include "TCanvas.h"
#include "TFile.h"
//#include ROOT
void joinPR(TString endfile)
{
	// TString endfile = "GammaFlash.root";
	cout << endfile << endl;
	int n = 8;

	Double_t energy_var_n;
	Double_t theta_var_n;
	Int_t part_var_n;
	Char_t process_var_n;
	Char_t name_var_n;

	TString start = TString("Project_t");
	TString ending = TString(".root");

	TH1D *hN = new TH1D("Name", "",6,0,6);

	Int_t j;
	for (int j = 0; j < n; j++) {
		TString number = "";
		number += j;
		TString input = start + number + ending;
		cout << input << endl;

		TFile file0(input);
		TTree *treeD0n; file0.GetObject("TargetWall",treeD0n);

		Int_t nentries0n = Int_t(treeD0n->GetEntries());

		treeD0n->ResetBranchAddresses();
		treeD0n->SetBranchAddress("Name",&name_var_n);
		for (Int_t i=0; i<nentries0n; i++) {
			treeD0n->GetEntry(i);
			hN->Fill(name_var_n)
			// cout << &name_var_n << ' ' << fProcess[i]<< endl;
		}
	}
	TFile fnew(endfile,"recreate");

	hN->Write();
	fnew.Close();



	//neDEn->Scale(rate);	neDT->Scale(rate);	neDVZ->Scale(rate);	neDVZN->Scale(rate);
	//neDEnT->Scale(rate);	neDEnV->Scale(rate);
	//neDTV->Scale(rate);

	// neDVZN->Divide(neDVZNo);


	// Double_t min;
	// min = neDEnT->GetMinimum(0); neDEnT->SetMinimum(min);
	// min = neDEnV->GetMinimum(0); neDEnV->SetMinimum(min);
	// min = neDTV->GetMinimum(0); neDTV->SetMinimum(min);
	//
	// gStyle->SetOptStat(0);
	//
	// neDEn->SetXTitle("Energy [MeV]");	neDEn->SetYTitle("Rate [s^{-1}]");
	// neDT->SetXTitle("Time [ns]");	neDT->SetYTitle("Rate [s^{-1}]");
	// neDVZ->SetXTitle("Theta [rad]");	neDVZ->SetYTitle("Rate [s^{-1}]");
	// neDVZN->SetXTitle("Theta [rad]");	neDVZN->SetYTitle("Rate [s^{-1}/cm^2]");
	//
	// neDEnT->SetXTitle("Energy [MeV]");	neDEnT->SetYTitle("Time [ns]");	neDEnT->SetZTitle("Rate [s^{-1}]");
	// neDEnV->SetXTitle("Energy [MeV]");	neDEnV->SetYTitle("Theta [rad]");	neDEnV->SetZTitle("Rate [s^{-1}]");
	// neDTV->SetXTitle("Time [ns]");	neDTV->SetYTitle("Theta [rad]");	neDTV->SetZTitle("Rate [s^{-1}]");


	// TFile fnew(endfile,"recreate");
	//
	// h1E->Write();
	// h1T->Write();
	// h1W->Write();
	// h1D->Write();
	// h1Th->Write();
	// h1H->Write();
	// h2ET->Write();
	// h2EW->Write();
	// h2ETh->Write();
	// h2EH->Write();
	// h2TW->Write();
	// h2TTh->Write();
	// h2TH->Write();
	// h2WD->Write();
	// h2WH->Write();
	//
	// fnew.Close();
}
