void compareResiduals( TString fno = "no_align_out.root", TString fsi = "align_out.root"){

  gStyle->SetOptStat(0);

  gStyle->SetPadRightMargin(0.08); // increase for colz plots
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.13);

  TFile *file_no = TFile::Open( fno.Data() );
  TFile *file_si = TFile::Open( fsi.Data() );


  TCanvas *c = new TCanvas( "res","residuals",900,500);
  //c->Divide(3,6);
  c->Divide(2,1); int i = 1;
  for( int l=1;l<=6;l++){
    for( int s=1;s<=3;s++){
      
      if( 
          //(l==1 && s==2) == false &&
          (l==2 && s==2) == false &&
          (l==5 && s==2) == false 
        ) continue;

      TH1 *hno = (TH1*) file_no->Get( Form( "residualL%dS%d",l,s  ) );
      TH1 *hsi = (TH1*) file_si->Get( Form( "residualL%dS%d",l,s  ) );

      c->cd( i )->SetGridx(); i++;
      //c->cd( s + (l-1) * 3 );
      hno->SetMaximum( hsi->GetMaximum() * 1.1 );
      hno->SetLineColor(kAzure+1);
      hno->SetFillColorAlpha(kAzure+1, 0.5);
      if( hno->GetEntries() < 10 ) continue;
      TFitResultPtr fit = hno->Fit("gaus","S0Q");
      const double *pars = fit->GetParams();
      const double *errs = fit->GetErrors();
      cout << l << " " << s << " ";
      cout << pars[1] << " " << pars[2] << " ";
      hno->Draw();
      hno->GetXaxis()->SetTitleSize(0.08);
      hno->GetXaxis()->SetLabelSize(0.06);
      hno->GetYaxis()->SetLabelSize(0.07);
      hno->SetXTitle("residual (mm)");

      hsi->SetLineColor(kOrange+1);
      hsi->SetFillColorAlpha(kOrange+1, 0.5);
      fit = hsi->Fit("gaus","S0Q");
      pars = fit->GetParams();
      errs = fit->GetErrors();
      cout << pars[1] << " " << pars[2] << endl;
      hsi->Draw("same");

      if( i == 3 ){
        TLegend *le = new TLegend(0.55,0.65,0.95,0.9);
        //le->SetHeader(Form("S%dL%d",s,l));
        le->AddEntry(hno,"before","fl");
        le->AddEntry(hsi,"after","fl");
        le->Draw();
      }
    }
  }

  c->SaveAs("c1.pdf");

}


