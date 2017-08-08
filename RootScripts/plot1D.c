#include <iostream>
#include <string>
#include "TROOT.h"
#include "TRint.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLegend.h"


TString energyname = TString("Time");
void plot1DEnergy()
{
	TString filegammaname = TString("GammaFlash.root");
	TString fileibname = TString("IBNeutron.root");
	TString filetarname = TString("TargetWallNeutron.root");
	TString energyname40 = TString("Energy60");


	gStyle->SetOptStat(0);

	TFile *filegamma = new TFile(filegammaname);
	TH2D *energyG40 = (TH2D *)filegamma->Get(energyname40);

	TFile *fileib = new TFile(fileibname);
	TH2D *energyIB40 = (TH2D *)fileib->Get(energyname40);

	TFile *fileTAR = new TFile(filetarname);
	TH2D *energyTAR40 = (TH2D *)fileTAR->Get(energyname40);

	TCanvas *cT = new TCanvas;
	cT->SetCanvasSize(700,700);
	cT->SetLogy();

	Double_t eventsG=pow(10,7); Double_t eventsTAR=pow(10,7); Double_t eventsIB=pow(10,7);
	Double_t cutG=53; Double_t cutTAR=10; Double_t cutIB=1;
	energyG40->Scale(2.6*pow(10,12)/(eventsG*cutG));
	energyIB40->Scale(6.6*pow(10,-2)/(eventsIB*cutIB));
	energyTAR40->Scale(3.3*pow(10,5)/(eventsTAR*cutTAR));

	energyG40->SetLineColor(kRed);
	energyIB40->SetLineColor(kGreen);
	energyTAR40->SetLineColor(kBlue);

	TAxis *x10w = energyG40->GetXaxis();
	x10w->SetTitle("Energy [MeV]");
	x10w->SetTitleOffset(1.3);

	TAxis *y10w = energyG40->GetYaxis();
	y10w->SetTitle("Rate [s^{-1}]");
	y10w->SetTitleOffset(1.6);

	energyG40->SetMaximum(pow(10,7));
	energyG40->SetMinimum(pow(10,-5));

	energyG40->Draw("hist");
	energyIB40->Draw("same");
	energyTAR40->Draw("same");

	leg = new TLegend(0.1,0.75,0.4,0.9);
	leg->AddEntry(energyG40,"GammaFlash","l");
	leg->AddEntry(energyIB40,"Neutron IB","l");
	leg->AddEntry(energyTAR40,"Neutron TAR","l");
	leg->Draw();
}

void plot1DTime()
{
	TString filegammaname = TString("GammaFlash.root");
	TString fileibname = TString("IBNeutron.root");
	TString filetarname = TString("TargetWallNeutron.root");
	TString energyname = TString("Time");
	gStyle->SetOptStat(0);

	TFile *filegamma = new TFile(filegammaname);
	TH2D *energyG = (TH2D *)filegamma->Get(energyname);

	TFile *fileib = new TFile(fileibname);
	TH2D *energyIB = (TH2D *)fileib->Get(energyname);

	TFile *fileTAR = new TFile(filetarname);
	TH2D *energyTAR = (TH2D *)fileTAR->Get(energyname);

	TCanvas *cT = new TCanvas;
	cT->SetCanvasSize(700,700);
	cT->SetLogy();

	Double_t eventsG=pow(10,9); Double_t eventsTAR=pow(10,7); Double_t eventsIB=pow(10,7);
	Double_t cutG=53; Double_t cutTAR=10; Double_t cutIB=1;
	energyG->Scale(2.6*pow(10,12)/(eventsG*cutG));
	energyIB->Scale(6.6*pow(10,-2)/(eventsIB*cutIB));
	energyTAR->Scale(3.3*pow(10,5)/(eventsTAR*cutTAR));

	energyG->SetLineColor(kRed);
	energyIB->SetLineColor(kGreen);
	energyTAR->SetLineColor(kBlue);

	TAxis *x10w = energyG->GetXaxis();
	x10w->SetTitle("Time [ns]");
	x10w->SetTitleOffset(1.3);

	TAxis *y10w = energyG->GetYaxis();
	y10w->SetTitle("Rate [s^{-1}]");
	y10w->SetTitleOffset(1.6);

	energyG->SetMaximum(pow(10,8));
	energyG->SetMinimum(pow(10,-8));

	energyG->Draw("hist");
	energyIB->Draw("same");
	energyTAR->Draw("same");

	leg = new TLegend(0.1,0.75,0.4,0.9);
	leg->AddEntry(energyG,"GammaFlash","l");
	leg->AddEntry(energyIB,"Neutron IB","l");
	leg->AddEntry(energyTAR,"Neutron TAR","l");
	leg->Draw();
}


void plot1DWidth()
{
	TString filegammaname = TString("GammaFlash.root");
	TString fileibname = TString("IBNeutron.root");
	TString filetarname = TString("TargetWallNeutron.root");
	TString energyname = TString("Width");
	gStyle->SetOptStat(0);

	TFile *filegamma = new TFile(filegammaname);
	TH2D *energyG = (TH2D *)filegamma->Get(energyname);

	TFile *fileib = new TFile(fileibname);
	TH2D *energyIB = (TH2D *)fileib->Get(energyname);

	TFile *fileTAR = new TFile(filetarname);
	TH2D *energyTAR = (TH2D *)fileTAR->Get(energyname);

	TCanvas *cT = new TCanvas;
	cT->SetCanvasSize(700,700);
	cT->SetLogy();

	// Double_t eventsG=pow(10,9); Double_t eventsTAR=pow(10,7); Double_t eventsIB=pow(10,7);
	// Double_t cutG=53; Double_t cutTAR=10; Double_t cutIB=1;
	// energyG->Scale(2.6*pow(10,12)/(eventsG*cutG));
	// energyIB->Scale(6.6*pow(10,-2)/(eventsIB*cutIB));
	// energyTAR->Scale(3.3*pow(10,5)/(eventsTAR*cutTAR));
	Double_t maxG, maxIB, maxTAR;
	maxG = energyG->GetMaximum();
	maxIB = energyIB->GetMaximum();
	maxTAR = energyTAR->GetMaximum();
	energyG->Scale(1/maxG);
	energyIB->Scale(1/maxIB);
	energyTAR->Scale(1/maxTAR);

	energyG->SetLineColor(kRed);
	energyIB->SetLineColor(kGreen);
	energyTAR->SetLineColor(kBlue);

	TAxis *x10w = energyG->GetXaxis();
	x10w->SetTitle("Width [mm]");
	x10w->SetTitleOffset(1.3);

	TAxis *y10w = energyG->GetYaxis();
	y10w->SetTitle("Rate [s^{-1}]");
	y10w->SetTitleOffset(1.6);

	energyG->SetMaximum(2*pow(10,0));
	energyG->SetMinimum(pow(10,-1));

	energyG->Draw("hist");
	energyIB->Draw("same");
	energyTAR->Draw("same");

	leg = new TLegend(0.1,0.75,0.4,0.9);
	leg->AddEntry(energyG,"GammaFlash","l");
	leg->AddEntry(energyIB,"Neutron IB","l");
	leg->AddEntry(energyTAR,"Neutron TAR","l");
	leg->Draw();
}
