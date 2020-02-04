{
  //gStyle->SetPalette(kRainBow);

 //for( int i=1;i<=6; i++){
   //TCanvas *c = new TCanvas();
   //c->Divide(1,3);
   //for( int j=1;j<=3;j++){
     //c->cd(j);
     //TH2 *h2m = (TH2*)gDirectory->Get( Form("sizeL%d",i) );
     //TH1 *hm = h2m->ProjectionY( Form("m%d_%d",i,j),j,j);
     //hm->Draw("");

   //}

 //}


 //TCanvas *c = new TCanvas;
 //TH1 *cf = c->DrawFrame(-0.5,0.001,20,.5);
 //TLegend *cl = new TLegend(0.6,0.6,0.9,0.9);
 //float max = 0;
 //for( int i=1;i<=6;i++){
   //TH2 *h = (TH2*) gDirectory->Get( Form("L%d",i) );
   //TH1 *hp = h->ProjectionY(Form("hh%d",i),2,2);
   //hp->SetLineColor(i);
   //hp->SetLineWidth(2);
   ////hp->Scale(1./hp->Integral());
   //hp->Draw("same");
   //cl->AddEntry(hp,Form("L%d",i),"l");
   //if( max < hp->GetMaximum() ) max = hp->GetMaximum() * 1.1;
 //}
 //cf->SetXTitle("# clusters");
 ////cf->SetYTitle("a.u.");
 //cf->SetMaximum(max);
 //cl->Draw();

 TCanvas *cs = new TCanvas;
 TH1 *csf = cs->DrawFrame(0,0.001,20,.5);
 TLegend *csl = new TLegend(0.6,0.6,0.9,0.9);
 for( int i=1;i<=6;i++){
   TH2 *h = (TH2*) gDirectory->Get( Form("sizeL%d",i) );
   //TH1 *hp = h->ProjectionY();
   TH1 *hp = h->ProjectionY(Form("hhh%d",i),2,2);
   hp->SetLineColor(i);
   hp->SetLineWidth(2);
   hp->Scale(1./hp->Integral());
   hp->Draw("same");
   csl->AddEntry(hp,Form("L%d",i),"l");
 }
 csf->SetXTitle("clusters size");
 csf->SetYTitle("a.u.");
 csl->Draw();

  //TCanvas *css = new TCanvas("CCC","CCC");
  //TH1 *cssf = css->DrawFrame(-0.5,0.001,20,.5);
  //TLegend *cssl = new TLegend(0.6,0.6,0.9,0.9);
  //TH2* hh = ((TH2*)gDirectory->Get( Form("sizeL%d",1) ));
  //TH1* htmpl= hh->ProjectionY("htmpl");
  //htmpl->Reset();
  ////htmpl->Draw();
  ////return;

  //TH1 *hcss[3];
  //for( int i=0;i<3;i++) hcss[i] = (TH1*) htmpl->Clone(Form("sizeS%d",i+1));

  //for( int i=3;i<=3;i++){
    //TH2 *h = (TH2*) gDirectory->Get( Form("sizeL%d",i) );
    //for(int l=1;l<=3;l++){
      //if( i==1 && l==2) continue;
      //if( i==5 && l==3) continue;
      //TH1 *hp = h->ProjectionY("h",l,l);
      //hcss[l-1]->Add(hp);
    //}
  //}
  
  //for(int l=1;l<=3;l++){
    //hcss[l-1]->SetLineColor(2*l);
    //hcss[l-1]->SetLineWidth(2);
    ////hcss[l-1]->Scale(1./hcss[l-1]->Integral());
    //hcss[l-1]->Draw("same");
    //cssl->AddEntry( hcss[l-1], Form("Sector %d",l),"l");
    //cssf->SetMaximum( hcss[l-1]->GetMaximum() );
  //}
  //cssf->SetXTitle("clusters size");
  //cssf->SetYTitle("a.u.");
  //cssl->Draw();

  gStyle->SetPaperSize(20,26);
  //gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadRightMargin(0.10); // increase for colz plots
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetTextSize(0.09);


 float maxs[] = { 900, 645, 645, 1030, 771, 1155 };
 gStyle->SetOptStat(0);
 TCanvas *cZ = new TCanvas("ccentroid","",1100,600);
 //cZ->Divide(1,1);
 cZ->Divide(3,2);
 int l = 1;
 for( int i=1;i<=6;i++){
   if( i==1 || i==3 || i==5 || i==6 ) continue;
   for( int j=3;j>=1;j--){
     cZ->cd(l);  l++;
     TH1 *h = (TH1*) gDirectory->Get( Form("centL%dS%d",i,j) );
     h->Rebin(2);
     h->SetTitle( Form( "Sector %d - Layer %d",j,i));
     h->SetTitleSize( 0.08 );
     h->SetMaximum(280);
     h->SetFillColor( kAzure+1 );
     h->GetXaxis()->SetLabelSize(0.07);
     h->GetXaxis()->SetTitleSize(0.07);
     h->GetYaxis()->SetLabelSize(0.07);
     h->GetYaxis()->SetTitleSize(0.07);
     h->GetXaxis()->SetRangeUser( 0, maxs[i-1] );
     h->Draw();
   }

 }

  TCanvas *cC = new TCanvas();
  cC->Divide(3,6);
  l = 1;
  for( int i=1;i<=6;i++){
    //if( i==2 || i==3 || i==5 ) continue;
    for( int j=1;j<=3;j++){
      cC->cd(l);  l++;
      TH1 *h = (TH1*) gDirectory->Get( Form("residualL%dS%d",i,j) );
      h->SetFillColor( kAzure+1 );
      h->SetTitle( Form( "S%dL%d",j,i));
      h->GetXaxis()->SetRangeUser(-1.5,1.5);
      h->Draw();
    }

  }
  cC->SaveAs("residuals.pdf");


  TCanvas *cT = new TCanvas();
  cT->Divide(3,2);
  l = 1;
  //for( int i=1;i<=6;i++){
  //if( i==2 || i==3 || i==5 ) continue;
  for( int j=1;j<=3;j++){
    cT->cd(l);  l++;
    TH2 *h = (TH2*) gDirectory->Get( Form("timeL%d",j) );
    h->Draw("colz");
  }

  for( int j=1;j<=3;j++){
    cT->cd(l);  l++;
    TH2 *h = (TH2*) gDirectory->Get( Form("bkgtimeL%d",j) );
    h->Draw("colz");
  }
  //}


  //TCanvas *cTime = new TCanvas();
  //cTime->Divide(2,1);
  //cTime->cd(1);
  //TH2 *ht = (TH2*) gDirectory->Get( Form("timeL%d",2) );
  //ht->Draw("colz");

  //cTime->cd(2);
  //TH2 *hb = (TH2*) gDirectory->Get( Form("bkgtimeL%d",2) );
  //hb->Draw("colz");
}


