

void plotDist( const char* cuts="q>0") {
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11);

  int argc = gApplication->Argc();
  const char* fname = gApplication->Argv()[ argc-1 ];
  TFile *fout = TFile::Open( fname );

  TNtuple *nt = (TNtuple*) fout->Get("nt");



  // definition of the axes
  // ----------------------
  axis *Pt = createAxis( "Pt (GeV/c)",60,0,4);
  axis *P  = createAxis( "P  (GeV/c)",60,0,4);

  axis *Theta  = createAxis( "#Theta (deg)",140,20,160);
  axis *Phi    = createAxis( "#varphi (deg)",  95,-184,184);
  axis *Z      = createAxis( "Z (mm)",  100,-60,60);

  axis *Chi2   = createAxis( "#chi^{2}",  100,0,50);
  axis *Ndf    = createAxis( "ndf",  10,0,10);
  axis *Chi2Ndf= createAxis( "#chi^{2}/ndf",  100,0,15);
  axis *Crs    = createAxis( "N cross",  8,0,8);

  axis *DTheta  = createAxis( "#Delta#Theta (deg)",300,-2,2);
  axis *DPhi    = createAxis( "#Delta#varphi (deg)",  300,-2,2);
  axis *DP    = createAxis( "#DeltaP/P",  100,-.5,.5);
  axis *DZ    = createAxis( "#DeltaZ (mm)",    300,-15,45);

  axis *d0    = createAxis( "d_{0} (mm)",    100,-12,12);
  axis *x0    = createAxis( "x_{0} (mm)",    100,-12,12);
  axis *y0    = createAxis( "y_{0} (mm)",    100,-12,12);

  // creation of the histograms
  // --------------------------
  
  TH1F *htrp      = createHisto(  "htrp",      "REC momentum", P);
  TH1F *htrpt     = createHisto(  "htrpt",     "REC momentum", Pt);
  TH1F *htrtheta  = createHisto(  "htrtheta",  "MC theta", Theta);
  TH1F *htrphi    = createHisto(  "htrphi",    "MC phi", Phi);

  TH1F *hchi2     = createHisto(  "hchi2",    "#Chi^{2}", Chi2);
  TH1F *hchi2ndf  = createHisto(  "hchi2ndf",    "#Chi^{2}/ndf", Chi2Ndf);
  TH1F *hndf      = createHisto(  "hndf",    "ndf", Ndf);
  TH1F *hsvtcrs   = createHisto(  "hsvtcrs",    "Clusters", Crs);
  TH1F *hbmtcrs   = createHisto(  "hbmtcrs",    "Clusters", Crs);

  TH1F *hd0 = createHisto( "hd0", "d0", d0 );

  // 2D histos
  TH2F *htrPvsTheta = create2DHisto("htrPvsTheta","P vs Theta", Theta, P );
  htrPvsTheta->Rebin2D(6,6);

  TH2F *hx0vsPhi   = create2DHisto( "hx0vsPhi",  "x0 vs phi", Phi, x0);
  TH2F *hy0vsPhi   = create2DHisto( "hy0vsPhi",  "y0 vs phi", Phi, y0);
  TH2F *hd0vsPhi   = create2DHisto( "hd0vsPhi",  "d0 vs phi", Phi, d0);

  // fill histos
  TString acuts = "";
  acuts.Append( cuts );
  const char* allcuts = acuts.Data();

  nt->Project( "htrp"  ,"P",  allcuts  ) ;
  nt->Project( "htrpt","PT",  allcuts   );
  nt->Project( "htrtheta"  ,"theta",  allcuts  );
  nt->Project( "htrphi"  ,"phi",  allcuts  );

  nt->Project("hchi2","chi2", allcuts );
  nt->Project("hchi2ndf","chi2/ndf", allcuts );
  nt->Project("hndf","ndf", allcuts );
  nt->Project("hsvtcrs","nSVT", allcuts );
  nt->Project("hbmtcrs","nBMT", allcuts );

  nt->Project( "hx0vsPhi", "d0*sin(phi*0.017453):phi", allcuts );
  nt->Project( "hy0vsPhi", "-d0*cos(phi*0.017453):phi", allcuts );
  nt->Project( "hd0vsPhi", "d0:phi", allcuts );
  nt->Project( "hd0","d0", allcuts );

  TCanvas *cV = new TCanvas("cV","cV");
  cV->Divide(2,2);
  cV->cd(1);
  hx0vsPhi->Draw("colz");
  cV->cd(2);
  hy0vsPhi->Draw("colz");
  cV->cd(3);
  //hd0->Fit("gaus");
  hd0->Draw();
  cV->cd(4);
  hd0vsPhi->Draw("colz");
  cV->Update();
  cV->SaveAs( "vertex.pdf" );
 
  TCanvas *cT = new TCanvas("cCROSSES", "crosses");
  cT->Divide(2,2);
  cT->cd(1)->SetLogy();
  hchi2->Draw(); 
  cT->cd(2)->SetLogy();
  hchi2ndf->Draw();
  cT->cd(3)->SetLogy();
  float hmax = hbmtcrs->GetMaximum();
  if( hmax < hsvtcrs->GetMaximum() ) hmax = hsvtcrs->GetMaximum();
  hbmtcrs->GetYaxis()->SetRangeUser(0.8,hmax*1.1);
  hbmtcrs->SetLineColor(kRed);
  hbmtcrs->Draw();
  hsvtcrs->Draw("same");
  TLegend *l = new TLegend( .15,.5,.35,.8);
  l->AddEntry(hbmtcrs,"BMT","l");
  l->AddEntry(hsvtcrs,"SVT","l");
  l->Draw();
  cT->cd(4)->SetLogy();
  hndf->Draw();
  cT->Update();
  cT->SaveAs( "track.pdf" );
  
 
  TCanvas *cP = new TCanvas();
  cP->Divide(2,2);
  cP->cd(1);
  htrp->SetLineColor(kRed);
  htrp->Draw();
 
  cP->cd(2);
  htrpt->SetLineColor(kRed);
  htrpt->Draw();
 
  
  cP->cd(3);
  htrtheta->SetLineColor(kRed);
  htrtheta->Draw();
 
  cP->cd(4);
  htrphi->SetLineColor(kRed);
  htrphi->Draw();

  cP->Update();
  cP->SaveAs( "distributions.pdf" );

 
}

