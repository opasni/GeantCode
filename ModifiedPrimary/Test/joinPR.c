#include <iostream>
#include <string>
#include "TROOT.h"
#include "TRint.h"
#include "TCanvas.h"
#include "TFile.h"
//#include ROOT
void joinPR()
{
	TString endfile = "Gamma.root";
	int n = 8;

	Double_t energy_var_n;
	Double_t time_var_n;
	Double_t theta_var_n;
	// Char_t name_var_n;

	TString start = TString("Project_t");
	TString ending = TString(".root");

	TH1D *neDEn = new TH1D("Energy", "",80,0,110);
	TH1D *neDT = new TH1D("Time", "",80,0,200);
	TH1D *neDVZ = new TH1D("Theta", "",50,0.165,0.515);

	TH2D *neDEnT = new TH2D("EnergyTime", "",80,0,110,80,0,200);
	TH2D *neDEnV = new TH2D("EnergyTheta", "",30,0,110,30,0.165,0.515);

	TH2D *neDTV = new TH2D("TimeTheta", "",80,0,200,100,0.165,0.515);


	Int_t j;
	for (int j = 0; j < n; j++) {
		TString number = "";
		number += j;
		TString input = start + number + ending;
		cout << input << endl;

		TFile file0(input);
		TString particle = "Gamma";
		// TString particle = "Neutron";
		TTree *treeD0n; file0.GetObject(particle,treeD0n);

		Int_t nentries0n = Int_t(treeD0n->GetEntries());
		treeD0n->SetBranchAddress("KE",&energy_var_n);
		treeD0n->SetBranchAddress("Time",&time_var_n);
		treeD0n->SetBranchAddress("Theta",&theta_var_n);
		for (Int_t i=0; i<nentries0n; i++) {
			treeD0n->GetEntry(i);
			neDEn->Fill(energy_var_n);
			neDT->Fill(time_var_n);
			neDVZ->Fill(theta_var_n);

			neDEnT->Fill(energy_var_n,time_var_n);
			neDEnV->Fill(energy_var_n,theta_var_n);

			neDTV->Fill(time_var_n,theta_var_n);
		}
	}

	//neDEn->Scale(rate);	neDT->Scale(rate);	neDVZ->Scale(rate);	neDVZN->Scale(rate);
	//neDEnT->Scale(rate);	neDEnV->Scale(rate);
	//neDTV->Scale(rate);

	// neDVZN->Divide(neDVZNo);


	Double_t min;
	min = neDEnT->GetMinimum(0); neDEnT->SetMinimum(min);
	min = neDEnV->GetMinimum(0); neDEnV->SetMinimum(min);
	min = neDTV->GetMinimum(0); neDTV->SetMinimum(min);

	gStyle->SetOptStat(0);

	neDEn->SetXTitle("Energy [MeV]");	neDEn->SetYTitle("Rate [s^{-1}]");
	neDT->SetXTitle("Time [ns]");	neDT->SetYTitle("Rate [s^{-1}]");
	neDVZ->SetXTitle("Theta [rad]");	neDVZ->SetYTitle("Rate [s^{-1}]");

	neDEnT->SetXTitle("Energy [MeV]");	neDEnT->SetYTitle("Time [ns]");	neDEnT->SetZTitle("Rate [s^{-1}]");
	neDEnV->SetXTitle("Energy [MeV]");	neDEnV->SetYTitle("Theta [rad]");	neDEnV->SetZTitle("Rate [s^{-1}]");
	neDTV->SetXTitle("Time [ns]");	neDTV->SetYTitle("Theta [rad]");	neDTV->SetZTitle("Rate [s^{-1}]");


	TFile fnew(endfile,"recreate");

	neDEn->Write();	neDT->Write();	neDVZ->Write();
	neDEnT->Write(); neDEnV->Write();
	neDTV->Write();

	fnew.Close();
}
