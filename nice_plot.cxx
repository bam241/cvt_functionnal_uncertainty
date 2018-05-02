map<string, double> p_max;
map<string, double> err_max;
int time_max = 180;

string file[6] = {"cooling", "burnup", "cycle", "assay", "power", "separation"};
int color[6] = {4, 2, 8, 9, 48, 38};


TGraphErrors* GetOneSigmaDistribution(string filename, string param) {
  TFile* file1 = TFile::Open(filename.c_str());
  TH2F* hpxpy = new TH2F("hpxpy", "py ps px", time_max * 12, 0, time_max, 300,
                         0, p_max[param]);
  string command = param + "/1000.:T/12>>hpxpy";

  TTree* tree = (TTree*)file1->Get("myTTree");
  tree->Draw(command.c_str(), "", "");
  Double_t x[time_max * 12];
  Double_t y[time_max * 12];
  Double_t y_sg_p[time_max * 12];
  Double_t y_sg_m[time_max * 12];
  Double_t y_sg[time_max * 12];

  for (int i = 0; i < time_max * 12; i++) {
    x[i] = (double)i / 12.;
    TH1D* ttt = hpxpy->ProjectionY("_py", i, i + 1);
    y[i] = ttt->GetMean();
    y_sg[i] = ttt->GetStdDev();
    delete ttt;
  }
  TGraphErrors* gr_err = new TGraphErrors(time_max * 12, x, y, 0, y_sg);
  return gr_err;
};

TGraph* GetOneSigmaGraph(string filename, string param, int bin) {
  TFile* file1 = TFile::Open(filename.c_str());
  TH2F* hpxpy = new TH2F("hpxpy", "py ps px", time_max * 12, 0, time_max, bin,
                         0, p_max[param]);
  string command = param + "/1000.:T/12>>hpxpy";
  TCanvas* tmp = new TCanvas();
  tmp->cd();

  TTree* tree = (TTree*)file1->Get("myTTree");
  tree->Draw(command.c_str(), "", "");
  Double_t x[time_max * 12];
  Double_t y[time_max * 12];
  Double_t y_sg[time_max * 12];

  for (int i = 0; i < time_max * 12; i++) {
    x[i] = (double)i / 12.;
    TH1D* ttt = hpxpy->ProjectionY("_py", i, i + 1);
    y[i] = ttt->GetMean();
    y_sg[i] = ttt->GetStdDev() / ttt->GetMean();
    delete ttt;
  }
  delete tree;
  delete tmp;

  TGraph* gr = new TGraph(time_max * 12, x, y_sg);
  return gr;
};

TGraph* add_rel_uncertainty(string name, string param, TCanvas* C, int color_) {
  string filename = "systematic_" + name + "/tree.root";
  TGraph* gr_ = GetOneSigmaGraph(filename, param, 1200);
  gr_->GetXaxis()->SetRangeUser(0, time_max);
  // gr_cool->GetYaxis()->SetRangeUser(0,200);
  gr_->SetTitle(name.c_str());
  gr_->SetLineColor(color_);
  C->cd(2);
  gr_->Draw("same");
  return gr_;
}

void my_plot(string param) {
  TCanvas* C1 = new TCanvas();
  C1->Divide(1, 2);

  C1->cd(1);
  TGraphErrors* gr_err =
      GetOneSigmaDistribution("systematic/old/tree.root", param);
  gr_err->GetXaxis()->SetRangeUser(0, time_max);
  gr_err->SetFillColor(4);
  gr_err->SetFillStyle(3002);
  gr_err->Draw("a4");
  C1->cd(1)->BuildLegend();

  TFile* file2 = TFile::Open("ref/tree.root");
  TTree* tree = (TTree*)file2->Get("myTTree");
  TGraph* h = new TGraph();
  tree->SetLineColor(1);
  string command = param + "/1000.:T/12>>h";
  tree->Draw(command.c_str(), "", "L same");

  TGraph* gr_full = GetOneSigmaGraph("systematic/old/tree.root", param, 300);
  gr_full->GetXaxis()->SetRangeUser(0, time_max);
  gr_full->GetYaxis()->SetRangeUser(0, err_max[param]);
  gr_full->SetLineColor(1);
  gr_full->SetTitle("Full Systematic Uncertainty");
  C1->cd(2);
  gr_full->Draw("AL");

  vector<TGraph*> v_gr;
  for (int i = 0; i < 5; i++) {
    v_gr.push_back(add_rel_uncertainty(file[i], param, C1, color[i]));
  }
  C1->cd(2)->BuildLegend();
}


void nice_plot() {
  p_max["P"] = 100;
  err_max["P"] = 0.1;

  p_max["B6"] = 1200;
  err_max["B6"] = 0.6;
  p_max["B2"] = 12000;
  err_max["B2"] = 1;
  p_max["B2_cumul"] = 12000;
  err_max["B2_cumul"] = 1;

  my_plot("P");
  my_plot("B6");
  my_plot("B2_cumul");
}
