void plot() {

  gStyle->SetOptStat(0);

  TFile *f = TFile::Open("el.root");
 
  TH2F *hmc = (TH2F*) f->Get( "h2MCThetaVsP" );
  if(hmc->GetEntries() < 1 ){
    TFile *fmc = TFile::Open("/home/fbossu/Data/Tracking/sim/validation/elastic/E6.5/nobkg_woS2L1/el.root");
    hmc = (TH2F*) fmc->Get( "h2MCThetaVsP" );
  }


  TCanvas *c0 = new TCanvas();
  c0->Divide(3,2);
  for( int i=1;i<=6; i++ ){
    c0->cd(i)->SetGridx();
    f->Get( Form("h1dPSec%d",i) )->Draw();
  }
  c0->SaveAs("el_deltaP_perDCSec.pdf");


  TCanvas *c1 = new TCanvas();
  c1->Divide(3,2);
  for( int i=1;i<=6; i++ ){
    c1->cd(i)->SetGridx();
    f->Get( Form("h1dPhiSec%d",i) )->Draw();
  }
  c1->SaveAs("el_deltaPhi_perDCSec.pdf");

  
  TCanvas *c2 = new TCanvas();
  c2->Divide(3,2);
  for( int i=1;i<=6; i++ ){
    c2->cd(i)->SetGridx();
    TH1 *h = (TH1*) f->Get( Form("h1dThetaSec%d",i) );
    h->SetTitle( Form("%s, DC Sec %d",h->GetTitle(),i));
    h->Draw();
  }
  c2->SaveAs("el_deltaTheta_perDCSec.pdf");

  TCanvas *cv = new TCanvas();
  cv->Divide(2,2);
  cv->cd(1);
  f->Get( "hVeVpZ" )->Draw("colz");
  cv->cd(2);
  f->Get( "hVeVpX" )->Draw("colz");
  cv->cd(3);
  f->Get( "hVeVpY" )->Draw("colz");
  cv->cd(4);
  f->Get( "hVeVpd0" )->Draw("colz");

  TCanvas *cTvsP = new TCanvas();
  cTvsP->Divide(3,2);
  for( int i=1;i<=6; i++ ){
    cTvsP->cd(i);
    TH2F *h = (TH2F*) f->Get( Form("h2ThetaVsP%d",i) );
    h->SetTitle( Form( "DC Sector %d ", i ) );
    h->Draw("col");

    //TH2F *hmc = (TH2F*) f->Get( "h2MCThetaVsP" );
    //if(hmc->GetEntries() > 0 ) hmc->Draw( "colsame" );
    hmc->Draw("colsame");
  }
  cTvsP->SaveAs("el_TvsP.pdf");

  TCanvas *cplots = new TCanvas();
  cplots->Divide(2,2);
  cplots->cd(1);
  f->Get("h2TeTp")->Draw("col");
  cplots->cd(2);
  f->Get("h2FeFp")->Draw("col");
  cplots->cd(3);
  f->Get("hQ2W")->Draw("col");

  cplots->SaveAs("el_distributions.pdf");


  TCanvas *ceff = new TCanvas();
  ceff->Divide(1,3);
  ceff->cd(1);
  TH1 *h1CountE = (TH1*) f->Get("h1CountEl");

  h1CountE->DrawClone();

  ceff->cd(2);
  TH1 *h1CountP = (TH1*) f->Get("h1CountP");
  h1CountP->DrawClone();

  ceff->cd(3)->SetGridy();
  h1CountP->Sumw2();
  h1CountP->Divide(h1CountE);
  h1CountP->SetMaximum(1.15);
  h1CountP->SetMinimum(0.);
  h1CountP->SetMarkerStyle(20);
  h1CountP->SetMarkerSize(0.6);
  h1CountP->SetLineColor(46);
  h1CountP->Draw();

  ceff->SaveAs("el_eff.pdf");
}

