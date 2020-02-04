void beamSpot(){

  TFile *f = TFile::Open("tracks.root");
  TNtuple *nt = (TNtuple*) f->Get("nt");

  
  axis *d0    = createAxis( "d_{0} (mm)",    100,-10,10);
  axis *Phi    = createAxis( "#varphi (deg)", 90,-180,180);

  TH2F *hd0vsPhi = create2DHisto("hbeam","beam spot", Phi, d0);


  nt->Project("hbeam", "d0:phi");
 

  gStyle->SetOptFit(11);

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


