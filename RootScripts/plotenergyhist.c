#include <iostream>
#include <string>
#include "TROOT.h"
#include "TRint.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLegend.h"

int plotEnergyTheta()
{
	TString filegammaname = TString("Gamma.root");
	TString fileelectronname = TString("NeutronIB.root");
	TString filenevtronname = TString("NeutronTar.root");
	TString energyname = TString("EnergyTheta");

	gStyle->SetOptStat(0);

	TFile *filegamma = new TFile(filegammaname);
	TH2D *energyG = (TH2D *)filegamma->Get(energyname);
	// energyG->SetLineColor(kGreen);

	TFile *fileelectron = new TFile(fileelectronname);
	TH2D *energyE = (TH2D *)fileelectron->Get(energyname);
	// energyE->SetLineColor(kRed);

	TFile *filenevtron = new TFile(filenevtronname);
	TH2D *energyN = (TH2D *)filenevtron->Get(energyname);
	// energyE->SetLineColor(kRed);


	TAxis *x10w = energyG->GetXaxis();
	TAxis *x20w = energyE->GetXaxis();
	TAxis *x30w = energyN->GetXaxis();
	x10w->SetTitleOffset(1.6);
	x20w->SetTitleOffset(1.6);
	x30w->SetTitleOffset(1.6);

	TAxis *y10w = energyG->GetYaxis();
	TAxis *y20w = energyE->GetYaxis();
	TAxis *y30w = energyN->GetYaxis();
	y10w->SetTitleOffset(1.6);
	y20w->SetTitleOffset(1.6);
	y30w->SetTitleOffset(1.6);

	TAxis *z10w = energyG->GetZaxis();
	TAxis *z20w = energyE->GetZaxis();
	TAxis *z30w = energyN->GetZaxis();
	z10w->SetTitleOffset(1.6);
	z20w->SetTitleOffset(1.6);
	z30w->SetTitleOffset(1.6);

	energyG->SetTitle("");
	energyE->SetTitle("");
	energyN->SetTitle("");

	Double_t max;
	max = energyG->GetMaximum(); energyG->Scale(1/max);
	max = energyE->GetMaximum(); energyE->Scale(1/max);
	max = energyN->GetMaximum(); energyN->Scale(1/max);

	// Double_t min;
	// min = energyG->GetMinimum(0); energyG->SetMinimum(min);
	// min = energyE->GetMinimum(0); energyE->SetMinimum(min);
	// min = energyN->GetMinimum(0); energyN->SetMinimum(min);

	energyG->SetMaximum(1);
	energyG->SetMinimum(pow(10,-6));

	energyE->SetMaximum(1);
	energyE->SetMinimum(pow(10,-6));

	energyN->SetMaximum(1);
	energyN->SetMinimum(pow(10,-6));


	TCanvas *cT = new TCanvas;
	cT->SetCanvasSize(780,700);
	// cT->SetLogx();
	// cT->SetLogy();
	cT->SetLogz();

	// energyG->Draw("COLZ");
	energyE->Draw("COLZ");
	// energyN->Draw("COLZ");
	// fa2->Draw();
	// energyEl->Draw("same");

	// leg = new TLegend(0.1,0.75,0.4,0.9);
	// leg->AddEntry(energyG,"Gamma","l");
	// leg->AddEntry(energyE,"Electron","l");
	// leg->Draw();
	return 0;
}
