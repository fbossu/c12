void plotMass(){

  gStyle->SetOptStat(0);

  TFile *file = TFile::Open("lambda.root");
  TNtuple *nt = (TNtuple*) file->Get("nt");



  TH2F *hmass = new TH2F("hmass", "mass vs vertex position",100,0,80,100,0.7,2.5);
  hmass->SetXTitle( "Vr MC (mm)");
  hmass->SetYTitle( "Inv. Mass REC (Gev/c^{2})");
  TCanvas *cm = new TCanvas();
  nt->Project( "hmass", "Ml:Rmc");
  hmass->Draw("colz");

  cm->SaveAs("Mlambda0_vs_vR.pdf");

  TCanvas *cc = new TCanvas();
  nt->Draw("Ml>>(100,0.7,2.5)");
  cc->SaveAs("Mlambda0.pdf");
}

