

void plotDist( const char* cuts="trueQ>0") {
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11);

  int argc = gApplication->Argc();
  const char* fname = gApplication->Argv()[ argc-1 ];
  TFile *fout = TFile::Open( fname );

  TNtuple *ntr = (TNtuple*) fout->Get("ntr");
  TNtuple *nmc = (TNtuple*) fout->Get("nmc");



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
  axis *DP    = createAxis( "#DeltaP/P",  150,-.4,.4);
  axis *DZ    = createAxis( "#DeltaZ (mm)",    300,-25,45);

  axis *d0    = createAxis( "d_{0} (mm)",    200,-10,10);
  axis *x0    = createAxis( "x_{0} (mm)",    100,-5,5);
  axis *y0    = createAxis( "y_{0} (mm)",    100,-5,5);

  // creation of the histograms
  // --------------------------
  TH1F *hmcp      = createHisto(  "hmcp",      "MC momentum", P);
  TH1F *hmcpt     = createHisto(  "hmcpt",     "MC momentum", Pt);
  TH1F *hmctheta  = createHisto(  "hmctheta",  "MC theta", Theta);
  TH1F *hmcphi    = createHisto(  "hmcphi",    "MC phi", Phi);
  
  TH1F *htrp      = createHisto(  "htrp",      "REC momentum", P);
  TH1F *htrpt     = createHisto(  "htrpt",     "REC momentum", Pt);
  TH1F *htrtheta  = createHisto(  "htrtheta",  "MC theta", Theta);
  TH1F *htrphi    = createHisto(  "htrphi",    "MC phi", Phi);

  TH1F *hchi2     = createHisto(  "hchi2",    "#Chi^{2}", Chi2);
  TH1F *hchi2ndf  = createHisto(  "hchi2ndf",    "#Chi^{2}/ndf", Chi2Ndf);
  TH1F *hndf      = createHisto(  "hndf",    "ndf", Ndf);
  TH1F *hsvtcrs   = createHisto(  "hsvtcrs",    "Clusters", Crs);
  TH1F *hbmtcrs   = createHisto(  "hbmtcrs",    "Clusters", Crs);


  // resolutions
  TH1F *hDTheta = createHisto( "hDTheta", "Theta res.", DTheta );
  TH1F *hDPhi = createHisto( "hDPhi", "Phi res.", DPhi );
  TH1F *hDP = createHisto( "hDP", "P res.", DP );
  TH1F *hDZ = createHisto( "hDZ", "Z res.", DZ );

  TH1F *hd0 = createHisto( "hd0", "d0", d0 );

  // 2D histos
  TH2F *hDPvsP     = create2DHisto( "hDPvsP", "P res vs P", P, DP);
  TH2F *hDPvsTheta = create2DHisto( "hDPvsTheta", "P res vs Theta", Theta, DP);
  TH2F *hDPvsPhi   = create2DHisto( "hDPvsPhi", "P res vs Phi", Phi, DP);
  TH2F *hDPvsZ     = create2DHisto( "hDPvsZ",  "P res vs Z", Z, DP);

  TH2F *hDThetavsZ     = create2DHisto( "hDThetavsZ",  "Theta res vs Z", Z, DTheta);
  TH2F *hDThetavsP     = create2DHisto( "hDThetavsP",  "Theta res vs P", P, DTheta);
  TH2F *hDThetavsTheta     = create2DHisto( "hDThetavsTheta",  "Theta res vs Theta", Theta, DTheta);
  TH2F *hDThetavsPhi     = create2DHisto( "hDThetavsPhi",  "Theta res vs Phi", Phi, DTheta);

  TH2F *hDPhivsZ     = create2DHisto( "hDPhivsZ",  "Phi res vs Z", Z, DPhi);
  TH2F *hDPhivsP     = create2DHisto( "hDPhivsP",  "Phi res vs P", P, DPhi);
  TH2F *hDPhivsTheta   = create2DHisto( "hDPhivsTheta",  "Phi res vs Theta", Theta, DPhi);
  TH2F *hDPhivsPhi     = create2DHisto( "hDPhivsPhi",  "Phi res vs Phi", Phi, DPhi);

  TH2F *hmcPvsTheta = create2DHisto("hmcPvsTheta","P vs Theta", Theta, P );
  hmcPvsTheta->Rebin2D(6,6);
  TH2F *htrPvsTheta = create2DHisto("htrPvsTheta","P vs Theta", Theta, P );
  htrPvsTheta->Rebin2D(6,6);

  TH2F *hx0vsPhi   = create2DHisto( "hx0vsPhi",  "x0 vs phi", Phi, x0);
  TH2F *hy0vsPhi   = create2DHisto( "hy0vsPhi",  "y0 vs phi", Phi, y0);
  TH2F *hd0vsPhi   = create2DHisto( "hd0vsPhi",  "d0 vs phi", Phi, d0);

  // fill histos
  //TString acuts = "match>0 && ";
  TString acuts = "match>0 && q==trueQ &&";
  acuts.Append( cuts );
  const char* allcuts = acuts.Data();

  nmc->Project( "hmcp"  ,"trueP", cuts  );
  nmc->Project( "hmcpt" ,"trueP*sin(0.017453*trueTheta)", cuts );
  nmc->Project( "hmctheta"  ,"trueTheta", cuts  );
  nmc->Project( "hmcphi"  ,"truePhi", cuts  );

  nmc->Project( "htrp"  ,"P",  allcuts  ) ;
  nmc->Project( "htrpt","PT",  allcuts   );
  nmc->Project( "htrtheta"  ,"theta",  allcuts  );
  nmc->Project( "htrphi"  ,"phi",  allcuts  );

  nmc->Project( "hmcPvsTheta", "trueP:trueTheta", Form("truePhi > 40 && truePhi < 120 && %s",cuts)); 
  nmc->Project( "htrPvsTheta", "P:theta", Form("truePhi > 40 && truePhi < 120 && match>0 && %s",cuts)); 

  nmc->Project("hchi2","chi2", allcuts );
  nmc->Project("hchi2ndf","chi2/ndf", allcuts );
  nmc->Project("hndf","ndf", allcuts );
  nmc->Project("hsvtcrs","nSVT", allcuts );
  nmc->Project("hbmtcrs","nBMT", allcuts );

  nmc->Project( "hx0vsPhi", "d0*sin(phi*0.017453):phi", allcuts );
  nmc->Project( "hy0vsPhi", "-d0*cos(phi*0.017453):phi", allcuts );
  nmc->Project( "hd0vsPhi", "d0:phi", allcuts );
  nmc->Project("hd0","d0", allcuts );

  // fill resolution
  nmc->Project( "hDTheta", "theta - trueTheta",  allcuts );
  nmc->Project( "hDPhi", "phi - truePhi",  allcuts );
  nmc->Project( "hDP", "(P - trueP)/trueP",  allcuts );
  nmc->Project( "hDZ", "Z - trueZ",  allcuts ); // trueZ is in CM, while Z is in MM
  
  nmc->Project( "hDPvsP", "(P - trueP)/trueP:trueP",  allcuts );
  nmc->Project( "hDPvsTheta", "(P - trueP)/trueP:trueTheta",  allcuts );
  nmc->Project( "hDPvsPhi", "(P - trueP)/trueP:truePhi",  allcuts );
  nmc->Project( "hDPvsZ", "(P - trueP)/trueP:trueZ",  allcuts );

  nmc->Project( "hDThetavsPhi", "(theta - trueTheta):truePhi",  allcuts );
  nmc->Project( "hDThetavsZ", "(theta - trueTheta):trueZ",  allcuts );
  nmc->Project( "hDThetavsP", "(theta - trueTheta):trueP",  allcuts );
  nmc->Project( "hDThetavsTheta", "(theta - trueTheta):trueTheta",  allcuts );

  nmc->Project( "hDPhivsPhi", "(phi - truePhi):truePhi",  allcuts );
  nmc->Project( "hDPhivsZ", "(phi - truePhi):trueZ",  allcuts );
  nmc->Project( "hDPhivsP", "(phi - truePhi):trueP",  allcuts );
  nmc->Project( "hDPhivsTheta", "(phi - truePhi):trueTheta",  allcuts );

  TCanvas *cV = new TCanvas("cV","cV");
  cV->Divide(2,2);
  cV->cd(1);
  hx0vsPhi->Draw("colz");
  cV->cd(2);
  hy0vsPhi->Draw("colz");
  cV->cd(3);
  hd0->Fit("gaus","","",-1,1);
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
  TLegend *l = new TLegend( .6,.2,.7,.4);
  l->AddEntry(hbmtcrs,"BMT","l");
  l->AddEntry(hsvtcrs,"SVT","l");
  l->Draw();
  cT->cd(4)->SetLogy();
  hndf->Draw();
  cT->Update();
  cT->SaveAs( "track.pdf" );
  
 
  TCanvas *cP = new TCanvas("cP","kine distributions");
  cP->Divide(2,2);
  cP->cd(1);
  hmcp->Draw();
  htrp->SetLineColor(kRed);
  htrp->Draw("same");
 
  cP->cd(2);
  hmcpt->Draw();
  htrpt->SetLineColor(kRed);
  htrpt->Draw("same");
 
  
  cP->cd(3);
  hmctheta->Draw();
  htrtheta->SetLineColor(kRed);
  htrtheta->Draw("same");
 
  cP->cd(4);
  hmcphi->Draw();
  htrphi->SetLineColor(kRed);
  htrphi->Draw("same");

  cP->Update();
  cP->SaveAs( "distributions.pdf" );

  float pTot = hmcp->Integral();
  float pNum = htrp->Integral();
  cout << " === INTEGRATED EFFICIENCY === " << endl;
  cout << " Eff: " << pNum / pTot << endl << endl;

  TCanvas *cE1 = new TCanvas("cE1","efficiencies 1D");
  cE1->Divide(2,2);
  cE1->cd(1);
  TH1F *hep = (TH1F*) htrp->Clone("hep");
  hep->Divide(hmcp);
  hep->SetMaximum(1.15);
  hep->Draw();
 
  cE1->cd(2);
  TH1F *hetheta = (TH1F*) htrtheta->Clone("hetheta");
  hetheta->Divide(hmctheta);
  hetheta->SetMaximum(1.15);
  hetheta->Draw();
 
  cE1->cd(3);
  TH1F *hephi = (TH1F*) htrphi->Clone("hephi");
  hephi->Divide(hmcphi);
  hephi->SetMaximum(1.15);
  hephi->Draw();

  cE1->cd(4);
  TH2F *hepvstheta = (TH2F*) htrPvsTheta->Clone("hepvstheta");
  hepvstheta->Divide( hmcPvsTheta);
  hepvstheta->SetMaximum(1.15);
  hepvstheta->Draw("colz");

  cE1->Update();
  cE1->SaveAs( "efficiencies.pdf" );

  // resolutions
  //gStyle->SetStatY(0.9);
  gStyle->SetStatX(1);
  gStyle->SetFitFormat("1.2e");
  gStyle->SetStatW(0.4);
  //gStyle->SetStatH(0.2);
  TCanvas *cR = new TCanvas("cR","resolutions 1D");
  cR->Divide(2,2);
  cR->cd(1);
  hDTheta->Fit("gaus","","",-0.4,0.4);
  hDTheta->GetFunction("gaus")->SetLineColor(kRed);
  hDTheta->SetMaximum( 1.6* hDTheta->GetMaximum() );
  hDTheta->Draw();

  cR->cd(2);
  hDPhi->Fit("gaus","","",-0.9,0.9);
  hDPhi->GetFunction("gaus")->SetLineColor(kRed);
  hDPhi->SetMaximum( 1.6* hDPhi->GetMaximum() );
  hDPhi->Draw();

  cR->cd(3);
  hDP->Fit("gaus","","",-0.1,0.1);
  hDP->GetFunction("gaus")->SetLineColor(kRed);
  hDP->SetMaximum( 1.6* hDP->GetMaximum() );
  hDP->Draw();

  cR->cd(4);
  hDZ->Fit("gaus");
  hDZ->GetFunction("gaus")->SetLineColor(kRed);
  hDZ->SetMaximum( 1.6* hDZ->GetMaximum() );
  hDZ->Draw();

  cR->Update();
  cR->SaveAs( "resolution1D.pdf" );

  TCanvas *cR2 = new TCanvas("cR2","momentum resolution 2D");
  cR2->Divide(2,2);
  cR2->cd(1);
  hDPvsP->GetYaxis()->SetRangeUser(-0.4,0.4);
  hDPvsP->Draw("colz");

  cR2->cd(2);
  hDPvsTheta->GetYaxis()->SetRangeUser(-0.4,0.4);
  hDPvsTheta->Draw("colz");

  cR2->cd(3);
  hDPvsPhi->GetYaxis()->SetRangeUser(-0.4,0.4);
  hDPvsPhi->Draw("colz");
  
  cR2->cd(4);
  hDPvsZ->GetYaxis()->SetRangeUser(-0.4,0.4);
  hDPvsZ->Draw("colz");
  
  cR2->Update();
  cR2->SaveAs( "resolution2D.pdf" );
 
  TCanvas *cR2Zeta = new TCanvas("cR2Zeta","theta resolution 2D");
  cR2Zeta->Divide(2,2);
  cR2Zeta->cd(1);
  hDThetavsP->GetYaxis()->SetRangeUser(-2,2);
  hDThetavsP->Draw("colz");

  cR2Zeta->cd(2);
  hDThetavsTheta->GetYaxis()->SetRangeUser(-2,2);
  hDThetavsTheta->Draw("colz");

  cR2Zeta->cd(3);
  hDThetavsPhi->GetYaxis()->SetRangeUser(-2,2);
  hDThetavsPhi->Draw("colz");
  
  cR2Zeta->cd(4);
  hDThetavsZ->GetYaxis()->SetRangeUser(-2,2);
  hDThetavsZ->Draw("colz");
  
  cR2Zeta->Update();
  cR2Zeta->SaveAs( "theta_resolution2D.pdf" );
 
  TCanvas *cR2Phi = new TCanvas("cR2Phi","phi resolution 2D");
  cR2Phi->Divide(2,2);
  cR2Phi->cd(1);
  hDPhivsP->GetYaxis()->SetRangeUser(-2,2);
  hDPhivsP->Draw("colz");

  cR2Phi->cd(2);
  hDPhivsTheta->GetYaxis()->SetRangeUser(-2,2);
  hDPhivsTheta->Draw("colz");

  cR2Phi->cd(3);
  hDPhivsPhi->GetYaxis()->SetRangeUser(-2,2);
  hDPhivsPhi->Draw("colz");
  
  cR2Phi->cd(4);
  hDPhivsZ->GetYaxis()->SetRangeUser(-2,2);
  hDPhivsZ->Draw("colz");
  
  cR2Phi->Update();
  cR2Phi->SaveAs( "phi_resolution2D.pdf" );


 // --------------------------------------------------------------------------
 //  Momentum mean and sigmas
 // --------------------------------------------------------------------------
  TCanvas *ccc = new TCanvas("ccc","momentum resolution - means and sigmas");
  ccc->Divide(2,2);

  TH2 *lh[4] = { hDPvsP, hDPvsTheta, hDPvsPhi, hDPvsZ };

  for( int j = 0; j < 4; j++ ){
    ccc->cd(j+1)->SetGridy();
    TH1 *f = ccc->cd(j+1)->DrawFrame( lh[j]->GetXaxis()->GetBinLowEdge(1), -.08, lh[j]->GetXaxis()->GetBinUpEdge( lh[j]->GetNbinsX()-1), .18);
    f->SetXTitle( lh[j]->GetXaxis()->GetTitle());
    f->SetYTitle( "#Delta p/p" );
    // fit resolution vs variable
    TGraphErrors *grSigma = new TGraphErrors();
    grSigma->SetName( Form("gr_p_sig%d",j) );
    TGraphErrors *grMean = new TGraphErrors();
    grMean->SetName( Form("gr_p_mean%d",j) );
    int pcounter = 0;
    for( int i=1; i<= lh[j]->GetNbinsX(); i++){

      TH1 *h = lh[j]->ProjectionY("h", i, i+1 );
      if( h->GetEntries() < 10 ) continue;
      TFitResultPtr f = h->Fit("gaus","Q0S");
      if( f.Get() == 0x0 ) continue;
      grSigma->SetPoint(pcounter, lh[j]->GetXaxis()->GetBinCenter(i), f->Parameter(2) );
      grSigma->SetPointError(pcounter, 0, f->ParError(2) );
      grMean->SetPoint(pcounter, lh[j]->GetXaxis()->GetBinCenter(i), f->Parameter(1) );
      grMean->SetPointError(pcounter, 0, f->ParError(1) );
      pcounter++;
    }
    

    TLegend *leg = new TLegend( .7,.7,.9,.9);
    grSigma->Draw("P*");
    
    leg->AddEntry( grSigma,"sigma","lp");

    grMean->SetMarkerStyle(2);
    grMean->SetMarkerColor(2);
    grMean->SetLineColor(2);
    grMean->Draw("P");
    leg->AddEntry( grMean,"mean","lp");
    leg->Draw();
  }
  ccc->SaveAs("meansAndSigmas.pdf");


 // --------------------------------------------------------------------------
 //  Theta mean and sigmas
 // --------------------------------------------------------------------------
  TCanvas *ccResTheta = new TCanvas("ccResTheta","momentum resolution - means and sigmas");
  ccResTheta->Divide(2,2);

  TH2 *lhTheta[4] = { hDThetavsP, hDThetavsTheta, hDThetavsPhi, hDThetavsZ };

  for( int j = 0; j < 4; j++ ){
    ccResTheta->cd(j+1)->SetGridy();
    TH1 *f = ccResTheta->cd(j+1)->DrawFrame( lhTheta[j]->GetXaxis()->GetBinLowEdge(1), -.1, lhTheta[j]->GetXaxis()->GetBinUpEdge( lhTheta[j]->GetNbinsX()-1), 1.);
    f->SetXTitle( lhTheta[j]->GetXaxis()->GetTitle());
    f->SetYTitle( "#Delta #theta" );
    // fit resolution vs variable
    TGraphErrors *grSigma = new TGraphErrors();
    grSigma->SetName( Form("gr_t_sig%d",j) );
    TGraphErrors *grMean = new TGraphErrors();
    grMean->SetName( Form("gr_t_mean%d",j) );
    int pcounter = 0;
    for( int i=1; i<= lhTheta[j]->GetNbinsX(); i++){

      TH1 *h = lhTheta[j]->ProjectionY("h", i, i+1 );
      if( h->GetEntries() < 10 ) continue;
      TFitResultPtr f = h->Fit("gaus","Q0S");
      if( f.Get() == 0x0 ) continue;
      grSigma->SetPoint(pcounter, lhTheta[j]->GetXaxis()->GetBinCenter(i), f->Parameter(2) );
      grSigma->SetPointError(pcounter, 0, f->ParError(2) );
      grMean->SetPoint(pcounter, lhTheta[j]->GetXaxis()->GetBinCenter(i), f->Parameter(1) );
      grMean->SetPointError(pcounter, 0, f->ParError(1) );
      pcounter++;
    }

    TLegend *leg = new TLegend( .7,.7,.9,.9);
    grSigma->Draw("P*");
    
    leg->AddEntry( grSigma,"sigma","lp");

    grMean->SetMarkerStyle(2);
    grMean->SetMarkerColor(2);
    grMean->SetLineColor(2);
    grMean->Draw("P");
    leg->AddEntry( grMean,"mean","lp");
    leg->Draw();
  }
  ccResTheta->SaveAs("theta_meansAndSigmas.pdf");


 // --------------------------------------------------------------------------
 //  Phi mean and sigmas
 // --------------------------------------------------------------------------
  TCanvas *ccResPhi = new TCanvas("ccResPhi","phi resolution - means and sigmas");
  ccResPhi->Divide(2,2);

  TH2 *lhPhi[4] = { hDPhivsP, hDPhivsTheta, hDPhivsPhi, hDPhivsZ };

  for( int j = 0; j < 4; j++ ){
    ccResPhi->cd(j+1)->SetGridy();
    TH1 *f = ccResPhi->cd(j+1)->DrawFrame( lhPhi[j]->GetXaxis()->GetBinLowEdge(1), -.3, lhPhi[j]->GetXaxis()->GetBinUpEdge( lhPhi[j]->GetNbinsX()-1), 1.);
    f->SetXTitle( lhPhi[j]->GetXaxis()->GetTitle());
    f->SetYTitle( "#Delta #varphi" );
    // fit resolution vs variable
    TGraphErrors *grSigma = new TGraphErrors();
    grSigma->SetName( Form("gr_f_sig%d",j) );
    TGraphErrors *grMean = new TGraphErrors();
    grMean->SetName( Form("gr_f_mean%d",j) );
    int pcounter = 0;
    for( int i=1; i<= lhPhi[j]->GetNbinsX(); i++){

      TH1 *h = lhPhi[j]->ProjectionY("h", i, i+1 );
      if( h->GetEntries() < 10 ) continue;
      TFitResultPtr f = h->Fit("gaus","Q0S");
      if( f.Get() == 0x0 ) continue;
      grSigma->SetPoint(pcounter, lhPhi[j]->GetXaxis()->GetBinCenter(i), f->Parameter(2) );
      grSigma->SetPointError(pcounter, 0, f->ParError(2) );
      grMean->SetPoint(pcounter, lhPhi[j]->GetXaxis()->GetBinCenter(i), f->Parameter(1) );
      grMean->SetPointError(pcounter, 0, f->ParError(1) );
      pcounter++;
    }

    TLegend *leg = new TLegend( .7,.7,.9,.9);
    grSigma->Draw("P*");
    
    leg->AddEntry( grSigma,"sigma","lp");

    grMean->SetMarkerStyle(2);
    grMean->SetMarkerColor(2);
    grMean->SetLineColor(2);
    grMean->Draw("P");
    leg->AddEntry( grMean,"mean","lp");
    leg->Draw();
  }
  ccResPhi->SaveAs("phi_meansAndSigmas.pdf");

  //return;

  // =======================================
  
  TCanvas *cbeam = new TCanvas("cbeam","beam position");
  TH1 *hframe = cbeam->DrawFrame(-182,-10,182,10);
  hframe->SetXTitle("#phi (deg)");
  hframe->SetYTitle("d_{0} (mm)");

    TGraphErrors *grSigmaB = new TGraphErrors();
    TGraphErrors *grMeanB = new TGraphErrors();
    for( int i=1; i<= hd0vsPhi->GetNbinsX(); i++){

      TH1 *h =hd0vsPhi->ProjectionY("h", i, i+1 );
      TFitResultPtr f = h->Fit("gaus","Q0S");
      if( f.Get() == 0x0 ) continue;
      if( TMath::Abs( f->Parameter(1) ) > 15. ) continue;
      grSigmaB->SetPoint(i-1, hd0vsPhi->GetXaxis()->GetBinCenter(i), f->Parameter(2) );
      grSigmaB->SetPointError(i-1, 0, f->ParError(2) );
      grMeanB->SetPoint(i-1,  hd0vsPhi->GetXaxis()->GetBinCenter(i), f->Parameter(1) );
      grMeanB->SetPointError(i-1, 0, f->Parameter(2) );
      //grMeanB->SetPointError(i-1, 0, f->ParError(1) );
      //break;
    }

    grMeanB->Draw("p*");
    TF1 *fbeam = new TF1("fbeam","[A]*sin(x*0.017453293 +[Phi0])",-180.,180.);
    fbeam->SetLineColor(kRed);
    grMeanB->Fit(fbeam,"R");

  cbeam->SaveAs("beamPosition.pdf");
}

