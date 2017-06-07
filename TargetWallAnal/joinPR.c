#include <iostream>
#include <string>
#include "TROOT.h"
#include "TRint.h"
#include "TCanvas.h"
#include "TFile.h"
//#include ROOT
void joinPR()
{
	TString endfile = "GammaFlash.root";
	cout << endfile << endl;
	int n = 8;

	Double_t energy_var_n;
	Double_t theta_var_n;
	Int_t part_var_n;
	Char_t process_var_n;
	Char_t name_var_n;

	TString start = TString("Project_t");
	TString ending = TString(".root");

	// TH1D *h1E = new TH1D("Energy", "",40,17,19.5);
	// TH1D *h1T = new TH1D("Time", "",80,66,80);
	// TH1D *h1W = new TH1D("Width", "",40,10,120);
	// TH1D *h1D = new TH1D("Depth", "",80,0,500);
	// TH1D *h1Th = new TH1D("Theta", "",50,0.165,0.515);
	// TH1I *h1H = new TH1I("Hits", "",10,0,9);
	//
	// TH2D *h2ET = new TH2D("EnergyTime", "",40,17,19.5,80,66,80);
	// TH2D *h2EW = new TH2D("EnergyWidth", "",40,17,19.5,40,10,120);
	// TH2D *h2ETh = new TH2D("EnergyTheta", "",40,17,19.5,50,0.165,0.515);
	// TH2D *h2EH = new TH2D("EnergyHit", "",40,17,19.5,10,0,9);
	//
	// TH2D *h2TW = new TH2D("TimeWidth", "",80,66,80,40,10,120);
	// TH2D *h2TTh = new TH2D("TimeTheta", "",80,66,80,50,0.165,0.515);
	// TH2D *h2TH = new TH2D("TimeHit", "",480,66,80,10,0,9);
	//
	// TH2D *h2WD = new TH2D("WidthDepth", "",40,10,120,80,0,500);
	// TH2D *h2WH = new TH2D("WidthHits", "",40,10,120,10,0,9);

	Int_t j;
	for (int j = 0; j < n; j++) {
		TString number = "";
		number += j;
		TString input = start + number + ending;
		cout << input << endl;

		TFile file0(input);
		TTree *treeD0n; file0.GetObject("TargetWall",treeD0n);

		Int_t nentries0n = Int_t(treeD0n->GetEntries());

		// std::vector<Char_t> fProcess;
		std::vector<TString> fProcess;
		treeD0n->SetBranchAddress("Process",&process_var_n);
		for (Int_t i=0; i<nentries0n; i++) {
			treeD0n->GetEntry(i);
			cout << &process_var_n << endl;
			fProcess.push_back(&process_var_n);
		}
		treeD0n->ResetBranchAddresses();
		treeD0n->SetBranchAddress("KE",&energy_var_n);
		treeD0n->SetBranchAddress("Theta",&theta_var_n);
		treeD0n->SetBranchAddress("PartID",&part_var_n);
		treeD0n->SetBranchAddress("Name",&name_var_n);
		for (Int_t i=0; i<nentries0n; i++) {
			treeD0n->GetEntry(i);
			cout << &name_var_n << ' ' << fProcess[i]<< endl;
		}
	}
	// for (Int_t i=0; i<nentries0n; i++) {
	// 	treeD0n->GetEntry(i);
	// 	h1E->Fill(energy_var_n);
	// 	h1T->Fill(time_var_n);
	// 	h1W->Fill(width_var_n);
	// 	h1D->Fill(depth_var_n);
	// 	h1Th->Fill(theta_var_n);
	// 	h1H->Fill(hit_var_n);
	//
	// 	h2ET->Fill(energy_var_n,time_var_n);
	// 	h2EW->Fill(energy_var_n,width_var_n);
	// 	h2ETh->Fill(energy_var_n,theta_var_n);
	// 	h2EH->Fill(energy_var_n,hit_var_n);
	// 	h2TW->Fill(time_var_n,width_var_n);
	// 	h2TTh->Fill(time_var_n,theta_var_n);
	// 	h2TH->Fill(time_var_n,hit_var_n);
	// 	h2WD->Fill(width_var_n,depth_var_n);
	// 	h2WH->Fill(width_var_n,hit_var_n);
	// }





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
