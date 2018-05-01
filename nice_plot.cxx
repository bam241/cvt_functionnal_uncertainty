TGraphErrors* GetOneSigmaDistribution(string filename, string param){
  
  TFile *file1 = TFile::Open(filename.c_str()); 
  TH2F *hpxpy = new TH2F("hpxpy","py ps px",180*12,0,180,300,0,1200);
  string command = param+"/1000.:T/12>>hpxpy";
  
  TTree* tree = (TTree*)file1->Get("myTTree"); 
  tree->Draw(command.c_str(),"" ,"");
  Double_t x[180*12];
  Double_t y[180*12];
  Double_t y_sg_p[180*12];
  Double_t y_sg_m[180*12];
  Double_t y_sg[180*12];

  for(int i = 0; i < 180*12; i++){
    x[i] = (double)i/12.;
    TH1D *ttt= hpxpy->ProjectionY("_py",i,i+1);
    //ttt->Fit("gaus", "Mq");
    //TF1 *myfunc=ttt->GetFunction("gaus");
    //y[i] = myfunc->GetParameter(1);
   // y_sg[i] = myfunc->GetParameter(2);
    y[i] = ttt-> GetMean();
    y_sg[i] = ttt->GetStdDev();
   // delete myfunc;
    delete ttt;
  }
  TGraphErrors* gr_err = new TGraphErrors(180*12,x,y,0,y_sg );
  return gr_err;

};

TGraph* GetOneSigmaGraph(string filename, string param, int bin){
  
  TFile *file1 = TFile::Open(filename.c_str()); 
  TH2F *hpxpy = new TH2F("hpxpy","py ps px",180*12,0,180,bin,0,1200);
  string command = param+"/1000.:T/12>>hpxpy";
  TCanvas* tmp  = new TCanvas();
  tmp->cd();
  
  TTree* tree = (TTree*)file1->Get("myTTree"); 
  tree->Draw(command.c_str(),"" ,"");
  Double_t x[180*12];
  Double_t y[180*12];
  Double_t y_sg[180*12];

  for(int i = 0; i < 180*12; i++){
    x[i] = (double)i/12.;
    TH1D *ttt= hpxpy->ProjectionY("_py",i,i+1);
    //ttt->Fit("gaus", "Mq");
    //TF1 *myfunc=ttt->GetFunction("gaus");
    //y[i] = myfunc->GetParameter(1);
   // y_sg[i] = myfunc->GetParameter(2);
    y[i] = ttt-> GetMean();
    y_sg[i] = ttt->GetStdDev() / ttt-> GetMean();

  //  delete myfunc;
    delete ttt;
  }
  delete tree;
  delete tmp;
  
  TGraph* gr = new TGraph(180*12,x,y_sg );
  return gr;

};

void mystuff(){
  TCanvas* C1 = new TCanvas();
  C1->Divide(1,2);
  
  C1->cd(1);
  TGraphErrors* gr_err = GetOneSigmaDistribution("systematic/tree.root", "B6");
  gr_err->GetXaxis()->SetRangeUser(0,180);
  gr_err->SetFillColor(4);
  gr_err->SetFillStyle(3002);
  gr_err->Draw("a4");


  TFile *file2 = TFile::Open("ref/tree.root"); 
  TTree* tree = (TTree*)file2->Get("myTTree"); 
  TGraph *h= new TGraph();
  tree->SetLineColor(1);
  tree->Draw("(B6)/1000.:T/12>>h","" ,"L same");

  TGraph* gr_full = GetOneSigmaGraph("systematic/tree.root", "B6", 300);
  gr_full->GetXaxis()->SetRangeUser(0,180);
  gr_full->GetYaxis()->SetRangeUser(0,0.6);
  gr_full->SetLineColor(1);
  C1->cd(2);
  gr_full->Draw("AL");

  TGraph* gr_cool = GetOneSigmaGraph("systematic_cooling/tree.root", "B6", 1200);
  gr_cool->GetXaxis()->SetRangeUser(0,180);
  //gr_cool->GetYaxis()->SetRangeUser(0,200);
  gr_cool->SetLineColor(4);
  C1->cd(2);
  gr_cool->Draw("same");

  TGraph* gr_bu = GetOneSigmaGraph("systematic_burnup/tree.root", "B6", 1200);
  gr_bu->GetXaxis()->SetRangeUser(0,180);
  //gr_bu->GetYaxis()->SetRangeUser(0,200);
  gr_bu->SetLineColor(2);
  C1->cd(2);
  gr_bu->Draw("same");
  TGraph* gr_cycle = GetOneSigmaGraph("systematic_cycle/tree.root", "B6", 1200);
  gr_cycle->GetXaxis()->SetRangeUser(0,180);
  //gr_cycle->GetYaxis()->SetRangeUser(0,200);
  gr_cycle->SetLineColor(8);
  C1->cd(2);
  gr_cycle->Draw("same");
  gr_full->GetXaxis()->SetRangeUser(0,180);
  gr_full->GetYaxis()->SetRangeUser(0,0.6);
  C1->Update();
}

