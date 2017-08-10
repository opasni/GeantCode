#include <iostream>
#include <string>
#include "TROOT.h"
#include "TRint.h"
#include "TCanvas.h"
#include "TFile.h"
//#include ROOT
int joinPRMod2()
{
	// Double_t events /*= 10000*/;
	// cin >> events;
	// cout << events << endl;
	TString endfile = "TargetWall.root";
	// cin >> endfile;
	// cout << endfile << endl;
	// TString invbeta /*= "InvBetaOff.root"*/;
	// cin >> invbeta;
	// cout << invbeta << endl;
	// std::string endfile ("NuOffMagnet.root");
	int n = 8;
	//Double_t invbetaNorm;
	//if (invbeta == "yes") invbetaNorm = TMath::Power(10, 13);
	//else invbetaNorm = 1;
	//Double_t rate = (100*TMath::Power(10, -6))/(208*events*1.602*TMath::Power(10, -19))/invbetaNorm;

	Double_t energy_var_n;
	Int_t part_var_n;
	Char_t name_var_n;

	std::vector<Int_t> part;

	TString start = TString("Project_t");
	TString ending = TString(".root");

	TH2D *neDEnT = new TH2D("PartName", "",4,1,5,6,1,6);



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
		for (Int_t i=0; i<nentries0n; i++) {
			treeD0n->GetEntry(i);
			// cout << part_var_n << endl;
			part.push_back(part_var_n);
			// neDEnT->Fill(name_var_n,part_var_n);
		}
		treeD0n->SetBranchAddress("Name",&name_var_n);
		for (Int_t i=0; i<nentries0n; i++) {
			treeD0n->GetEntry(i);
			// cout << &name_var_n << ' ' << part[k] << endl;
			neDEnT->Fill(&name_var_n,part[k],1);
			k++;
		}
	}

	//neDEn->Scale(rate);	neDT->Scale(rate);	neDVZ->Scale(rate);	neDVZN->Scale(rate);
	//neDEnT->Scale(rate);	neDEnV->Scale(rate);
	//neDTV->Scale(rate);

	// neDVZN->Divide(neDVZNo);

	//
	// Double_t min;
	// min = neDEnT->GetMinimum(0); neDEnT->SetMinimum(min);
	// min = neDEnV->GetMinimum(0); neDEnV->SetMinimum(min);
	// min = neDTV->GetMinimum(0); neDTV->SetMinimum(min);

	gStyle->SetOptStat(0);

	// neDEn->SetXTitle("Energy [MeV]");	neDEn->SetYTitle("Rate [s^{-1}]");
	// neDT->SetXTitle("Time [ns]");	neDT->SetYTitle("Rate [s^{-1}]");
	// neDVZ->SetXTitle("Theta [rad]");	neDVZ->SetYTitle("Rate [s^{-1}]");
	// neDVZN->SetXTitle("Theta [rad]");	neDVZN->SetYTitle("Rate [s^{-1}/cm^2]");
	//
	// neDEnT->SetXTitle("Energy [MeV]");	neDEnT->SetYTitle("Time [ns]");	neDEnT->SetZTitle("Rate [s^{-1}]");
	// neDEnT2->SetXTitle("Energy [MeV]");	neDEnT2->SetYTitle("Time [ns]");	neDEnT2->SetZTitle("Rate [s^{-1}]");
	// neDEnV->SetXTitle("Energy [MeV]");	neDEnV->SetYTitle("Theta [rad]");	neDEnV->SetZTitle("Rate [s^{-1}]");
	// neDTV->SetXTitle("Time [ns]");	neDTV->SetYTitle("Theta [rad]");	neDTV->SetZTitle("Rate [s^{-1}]");


	TFile fnew(endfile,"recreate");

	neDEnT->Write();

	fnew.Close();
	return 0;
}
