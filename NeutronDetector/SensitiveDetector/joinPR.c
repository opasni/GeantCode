#include <iostream>
#include <string>
#include "TROOT.h"
#include "TRint.h"
#include "TCanvas.h"
#include "TFile.h"
//#include ROOT
int joinPR()
{
	TString endfile = "InvBetaOff.root";
	Double_t events = 10000;
	// cin >> events;
	// cin >> endfile;
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
	Char_t name_var_n;

	TString start = TString("Project_t");
	TString ending = TString(".root");

	TH1D *neDEn = new TH1D("Energy", "",80,0,110);
	TH1D *neDT = new TH1D("Time", "",80,0,200);
	TH1D *neDVZ = new TH1D("VertexPositionZ", "",80,0,500);
	TH1D *neDPr = new TH1D("Process", "",9,0,9);
	TH1D *neDNa = new TH1D("Name", "",6,0,6);

	TH2D *neDEnT = new TH2D("EnergyTime", "",80,0,110,80,0,200);
	TH2D *neDEnV = new TH2D("EnergyVertex", "",80,0,110,80,0,500);
	TH2D *neDEnPr = new TH2D("EnergyProcess", "",80,0,110,9,0,9);
	TH2D *neDEnN = new TH2D("EnergyName", "",80,0,110,6,0,6);

	TH2D *neDTV = new TH2D("TimeVertex", "",80,0,200,80,0,500);
	TH2D *neDTPr = new TH2D("TimeProcess", "",80,0,200,9,0,9);
	TH2D *neDTN = new TH2D("TimeName", "",80,0,200,6,0,6);

	TH2D *neDVPr = new TH2D("VertexProcess", "",80,0,500,9,0,9);
	TH2D *neDVN = new TH2D("VertexName", "",80,0,500,6,0,6);

	TH2D *neDPrN = new TH2D("ProcessName", "",9,0,9,6,0,6);

	std::vector<std::vector<TString>> fName;

	Int_t j;
	for (int j = 0; j < n; j++) {
		TString number = "";
		number += j;
		TString input = start + number + ending;
		cout << input << endl;

		std::vector<TString> fNameCol;

		TFile file0(input);
		TTree *treeD0n; file0.GetObject("Hits",treeD0n);

		Int_t nentries0n = Int_t(treeD0n->GetEntries());
		treeD0n->SetBranchAddress("Name",&name_var_n);
		for (Int_t i=0; i<nentries0n; i++) {
			treeD0n->GetEntry(i);
			neDNa->Fill(name_var_n);
			fNameCol.push_back(&name_var_n);
		}
		fName.push_back(fNameCol);
	}


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

			neDEnT->Fill(energy_var_n,time_var_n);
			neDEnV->Fill(energy_var_n,vertexz_var_n);
			neDEnPr->Fill(energy_var_n,&process_var_n,1);
			neDEnN->Fill(energy_var_n,fName[j][i],1);

			neDTV->Fill(time_var_n,vertexz_var_n);
			neDTPr->Fill(time_var_n,&process_var_n,1);
			neDTN->Fill(time_var_n,fName[j][i],1);

			neDVPr->Fill(vertexz_var_n,&process_var_n,1);
			neDVN->Fill(vertexz_var_n,fName[j][i],1);

			neDPrN->Fill(&process_var_n,fName[j][i],1);
		}
	}

	neDEn->Scale(rate);	neDT->Scale(rate);	neDVZ->Scale(rate);	neDPr->Scale(rate);	neDNa->Scale(rate);
	neDEnT->Scale(rate);	neDEnV->Scale(rate);	neDEnPr->Scale(rate);	neDEnN->Scale(rate);
	neDTV->Scale(rate);	neDTPr->Scale(rate);	neDTN->Scale(rate);
	neDVPr->Scale(rate);	neDVN->Scale(rate);
	neDPrN->Scale(rate);

	Double_t min;
	min = neDEnT->GetMinimum(0); neDEnT->SetMinimum(min);
	min = neDEnV->GetMinimum(0); neDEnV->SetMinimum(min);
	min = neDEnPr->GetMinimum(0); neDEnPr->SetMinimum(min);
	min = neDEnN->GetMinimum(0); neDEnN->SetMinimum(min);
	min = neDTV->GetMinimum(0); neDTV->SetMinimum(min);
	min = neDTPr->GetMinimum(0); neDTPr->SetMinimum(min);
	min = neDTN->GetMinimum(0); neDTN->SetMinimum(min);
	min = neDVPr->GetMinimum(0); neDVPr->SetMinimum(min);
	min = neDVN->GetMinimum(0); neDVN->SetMinimum(min);
	min = neDPrN->GetMinimum(0); neDPrN->SetMinimum(min);

	gStyle->SetOptStat(0);

	neDEn->SetXTitle("Energy [MeV]");	neDEn->SetYTitle("Rate [s^{-1}]");
	neDT->SetXTitle("Time [ns]");	neDT->SetYTitle("Rate [s^{-1}]");
	neDVZ->SetXTitle("Vertex Position Z [nm]");	neDVZ->SetYTitle("Rate [s^{-1}]");
	neDPr->SetXTitle("Process name");	neDPr->SetYTitle("Rate [s^{-1}]");
	neDNa->SetXTitle("Particle name");	neDNa->SetYTitle("Rate [s^{-1}]");

	neDEnT->SetXTitle("Energy [MeV]");	neDEnT->SetYTitle("Time [ns]");	neDEnT->SetZTitle("Rate [s^{-1}]");
	neDEnV->SetXTitle("Energy [MeV]");	neDEnV->SetYTitle("Vertex Position Z [nm]");	neDEnV->SetZTitle("Rate [s^{-1}]");
	neDEnPr->SetXTitle("Energy [MeV]");	neDEnPr->SetYTitle("Process name");	neDEnPr->SetZTitle("Rate [s^{-1}]");
	neDEnN->SetXTitle("Energy [MeV]");	neDEnN->SetYTitle("Particle name");	neDEnN->SetZTitle("Rate [s^{-1}]");
	neDTV->SetXTitle("Time [ns]");	neDTV->SetYTitle("Vertex Position Z [nm]");	neDTV->SetZTitle("Rate [s^{-1}]");
	neDTPr->SetXTitle("Time [ns]");	neDTPr->SetYTitle("Process name");	neDTPr->SetZTitle("Rate [s^{-1}]");
	neDTN->SetXTitle("Time [ns]");	neDTN->SetYTitle("Particle name");	neDTN->SetZTitle("Rate [s^{-1}]");
	neDVPr->SetXTitle("Vertex Position Z [nm]");	neDVPr->SetYTitle("Process name");	neDVPr->SetZTitle("Rate [s^{-1}]");
	neDVN->SetXTitle("Vertex Position Z [nm]");	neDVN->SetYTitle("Particle name");	neDVN->SetZTitle("Rate [s^{-1}]");
	neDPrN->SetXTitle("Process name");	neDPrN->SetYTitle("Particle name");	neDPrN->SetZTitle("Rate [s^{-1}]");


	TFile fnew(endfile,"recreate");

	neDEn->Write();	neDT->Write();	neDVZ->Write();	neDPr->Write();	neDNa->Write();
	neDEnT->Write(); neDEnV->Write();	neDEnPr->Write();	neDEnN->Write();
	neDTV->Write();	neDTPr->Write();	neDTN->Write();
	neDVPr->Write();	neDVN->Write();
	neDPrN->Write();

	fnew.Close();
	return 0;
}
