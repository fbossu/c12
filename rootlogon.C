struct axis {
  const char *title; int nbins; float binl; float binh;
};

axis* createAxis( const char *title, int nbins, float binl, float binh ){
  axis *x = new axis();
  x->title = title;
  x->nbins = nbins;
  x->binl  = binl;
  x->binh  = binh;
  return x;
}


TH1F* createHisto( const char *name, const char *title, axis *x ){
  TH1F *h = new TH1F(name,title,x->nbins,x->binl,x->binh);
  h->SetXTitle(x->title);
  return h;
}

TH2F* create2DHisto( const char *name, const char *title, axis *x, axis *y ){
  TH2F *h = new TH2F(name,title,x->nbins,x->binl,x->binh,y->nbins,y->binl,y->binh);
  h->SetXTitle(x->title);
  h->SetYTitle(y->title);
  return h;
}

void rootlogon(){


  Printf("rootlogon");

  gROOT->ProcessLine(".x $HOME/Macros/style.C");

}

