#include <iostream>
#include <string>
#include "TROOT.h"
#include "TRint.h"
#include "TCanvas.h"
#include "TFile.h"
//#include ROOT

std::array<std::vector<TString>, 4> nameAll;
std::array<std::vector<Int_t>, 4> nameI;

int dataMine()
{
	// Double_t events /*= 10000*/;
	// cin >> events;
	// cout << events << endl;
	TString endfile = "ThetaHistograms.root";
	// TFile * outf = new TFile(endfile,"recreate");

	// cin >> endfile;
	// cout << endfile << endl;
	// TString invbeta /*= "InvBetaOff.root"*/;
	// cin >> invbeta;
	// cout << invbeta << endl;
	// std::string endfile ("NuOffMagnet.root");

	//Double_t invbetaNorm;
	//if (invbeta == "yes") invbetaNorm = TMath::Power(10, 13);
	//else invbetaNorm = 1;
	//Double_t rate = (100*TMath::Power(10, -6))/(208*events*1.602*TMath::Power(10, -19))/invbetaNorm;

	Double_t energy_var_n;
	Double_t time_var_n;
	Double_t theta_var_n;
	Char_t name_var_n;

	TString start = TString("Project_t");
	TString ending = TString(".root");

	int n = 8;
	TString particle[4] = {"Gamma","Electron","Proton","Neutron"};
	for (int i_par=0; i_par<4; i_par++){
		Int_t j;
		for (int j = 0; j < n; j++) {
			TString number = "";
			number += j;
			TString input = start + number + ending;

			TFile file0(input);
			TTree *treeD0n; file0.GetObject(particle[i_par],treeD0n);

			Int_t nentries0n = Int_t(treeD0n->GetEntries());
			treeD0n->SetBranchAddress("Process",&name_var_n);
			for (Int_t i=0; i<nentries0n; i++) {
				treeD0n->GetEntry(i);
				TString name_var = &name_var_n;
				// TString* point_name = &name_var;
				// name[i_par].push_back(name_var);
				if(std::find(nameAll[i_par].begin(), nameAll[i_par].end(), name_var) != nameAll[i_par].end()) int mn=0;
				else {
					nameAll[i_par].push_back(name_var);
				}
				for (int i_proc=0; i_proc<nameAll[i_par].size(); i_proc++){
					if (name_var==nameAll[i_par][i_proc]) nameI[i_par].push_back(i_proc);
				}
			}
		}
	}
	Int_t number_of_proc=0;
	Int_t hist_begin[4];
	for (int t=0; t<nameAll.size(); t++) {
		hist_begin[t]=number_of_proc;
		number_of_proc+=nameAll[t].size();
	}
	TH1D *theta_hist[number_of_proc];
	TH1D *energy_hist[number_of_proc];
	TH1D *time_hist[number_of_proc];
	for (int i_par=0; i_par<4; i_par++) {
		cout << nameAll[i_par].size() << endl;
		for (int i_proc=0; i_proc<nameAll[i_par].size(); i_proc++) {
			TString histnameTh = "Th_";TString histnameTi = "Ti_";TString histnameEn = "En_";
			TString name1 = nameAll[i_par][i_proc];
			histnameTh +=particle[i_par]+'_' + name1;
			histnameTi +=particle[i_par]+'_' + name1;
			histnameEn +=particle[i_par]+'_' + name1;
			theta_hist[hist_begin[i_par]+i_proc] = new TH1D(histnameTh, "", 50, 0., 3.14);
			time_hist[hist_begin[i_par]+i_proc] = new TH1D(histnameTi, "", 50, 0., 70);
			energy_hist[hist_begin[i_par]+i_proc] = new TH1D(histnameEn, "", 50, 0., 110);
		}
	}

	for (int i_par=0; i_par<4; i_par++){
		Int_t j;
		Int_t k=0;
		for (int j = 0; j < n; j++) {
			TString number = "";
			number += j;
			TString input = start + number + ending;

			TFile file0(input);
			TTree *treeD0n; file0.GetObject(particle[i_par],treeD0n);

			Int_t nentries0n = Int_t(treeD0n->GetEntries());
			treeD0n->SetBranchAddress("Theta",&theta_var_n);
			treeD0n->SetBranchAddress("Time",&time_var_n);
			treeD0n->SetBranchAddress("KE",&energy_var_n);
			for (Int_t i=0; i<nentries0n; i++) {
				treeD0n->GetEntry(i);
				theta_hist[hist_begin[i_par]+nameI[i_par][k]]->Fill(theta_var_n);
				time_hist[hist_begin[i_par]+nameI[i_par][k]]->Fill(time_var_n);
				energy_hist[hist_begin[i_par]+nameI[i_par][k]]->Fill(energy_var_n);
				k++;
			}
		}
	}

	TFile * outf = new TFile(endfile,"recreate");

	for (int m=0; m<nameAll.size(); m++){
		for (int n=0; n<nameAll[m].size(); n++) cout << nameAll[m][n] << endl;
	}
	for (int i=0; i<number_of_proc; i++) {
		theta_hist[i]->Write();
		time_hist[i]->Write();
		energy_hist[i]->Write();
	}

	outf->Close();
	return 0;
}
