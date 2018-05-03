map<string, string> legend;
map<string, double> p_max;
map<string, double> err_max;
int time_max = 180;
string full_file = "systematic_no_separation/tree.root";
//string ref_file = "systematic_no_cooling_no_cycle/tree.root";
string file[6] = {"cooling", "burnup", "cycle", "assay/0.01", "power", "separation/0.01"};
string label[6] = {"Cooling Time", "Fuel enrichment", "Cycle Time", "Tail Assay", "Thermal Power", "Separation efficiency"};
int color[6] = {4, 2, 8, 9, 48, 38};


TGraphErrors* GetOneSigmaDistribution(string filename, string param) {
  TFile* file1 = TFile::Open(filename.c_str());
  TH2F* hpxpy = new TH2F("hpxpy", "py ps px", time_max * 12, 0, time_max, 3000,
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
    TH1D* ttt = hpxpy->ProjectionY("_py", i, i);
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
    TH1D* ttt = hpxpy->ProjectionY("_py", i, i );
    y[i] = ttt->GetMean();
    y_sg[i] = ttt->GetStdDev() / ttt->GetMean();
    if(ttt->GetMaximum() == ttt->GetSum()){y_sg[i] = 0;}
    delete ttt;
  }
  delete tree;
  delete tmp;

  TGraph* gr = new TGraph(time_max * 12, x, y_sg);
  return gr;
};

TGraph* add_rel_uncertainty(string name, string label, string param, TCanvas* C, int color_) {
  string filename = "systematic_" + name + "/tree.root";
  TGraph* gr_ = GetOneSigmaGraph(filename, param, 3000);
  gr_->GetXaxis()->SetRangeUser(0, time_max);
  // gr_cool->GetYaxis()->SetRangeUser(0,200);
  gr_->SetTitle(label.c_str());
  gr_->SetLineColor(color_);
  C->cd();
  gr_->Draw("same");
  return gr_;
}

void my_plot(string param) {
  TCanvas* C1 = new TCanvas();
  //C1->Divide(1, 2);

  C1->cd();
  TGraphErrors* gr_err =
      GetOneSigmaDistribution(full_file.c_str(), param);
  gr_err->GetXaxis()->SetRangeUser(0, time_max);
  gr_err ->SetTitle("Uncertainty at 1 #sigma");
  gr_err->SetFillColor(4);
  gr_err->SetFillStyle(3002);
  gr_err->GetHistogram()->GetXaxis()->SetTitle("Time [y]");
  gr_err->GetHistogram()->GetYaxis()->SetTitle(legend[param].c_str());
  gr_err->GetHistogram()->GetXaxis()->SetTitleSize(0.05);
  gr_err->GetHistogram()->GetXaxis()->SetTitleOffset(1);
  gr_err->GetHistogram()->GetXaxis()->SetLabelSize(0.05);
  gr_err->GetHistogram()->GetYaxis()->SetTitleSize(0.05);
  gr_err->GetHistogram()->GetYaxis()->SetTitleOffset(1);
  gr_err->GetHistogram()->GetYaxis()->SetLabelSize(0.05);
  
  gr_err->Draw("a4");

  TFile* file2 = TFile::Open("ref/tree.root");
  TTree* tree = (TTree*)file2->Get("myTTree");
  TGraph* h = new TGraph();
  h->SetNameTitle("Reference", "Reference");
  tree->SetLineColor(1);
  tree->SetMarkerStyle(20);
  tree->SetMarkerSize(0.1);
  string command = param + "/1000.:T/12>>Reference";
  tree->Draw(command.c_str(), "", "same");
  C1->BuildLegend();

  TCanvas* C2 = new TCanvas();
  TGraph* gr_full = GetOneSigmaGraph(full_file.c_str(), param, 3000);
  gr_full->GetXaxis()->SetRangeUser(0, time_max);
  gr_full->GetYaxis()->SetRangeUser(0, err_max[param]);
  gr_full->SetLineColor(1);
  gr_full->SetTitle("Full Systematic Uncertainty");
  gr_full->GetHistogram()->GetXaxis()->SetTitle("Time [y]");
  gr_full->GetHistogram()->GetYaxis()->SetTitle("Relative Uncertainty [n.a.]");
  gr_full->GetHistogram()->GetXaxis()->SetTitleSize(0.05);
  gr_full->GetHistogram()->GetXaxis()->SetTitleOffset(1);
  gr_full->GetHistogram()->GetXaxis()->SetLabelSize(0.05);
  gr_full->GetHistogram()->GetYaxis()->SetTitleSize(0.05);
  gr_full->GetHistogram()->GetYaxis()->SetTitleOffset(1);
  gr_full->GetHistogram()->GetYaxis()->SetLabelSize(0.05);
  C2->cd();
  gr_full->Draw("AL");

  vector<TGraph*> v_gr;
  for (int i = 0; i < 6; i++) {
    std::cout << "working with " << file[i] << std::endl;
    v_gr.push_back(add_rel_uncertainty(file[i], label[i], param, C2, color[i]));
  }
  C2->BuildLegend();
}


void nice_plot() {
  p_max["P"] = 100;
  err_max["P"] = 0.1;
  legend["P"] = "Generated Power [GW]";

  p_max["B6"] = 1200;
  err_max["B6"] = 0.6;
  legend["B6"] = "Fissile Inventory [t]";
  
  p_max["B2"] = 12000;
  err_max["B2"] = 1;
  
  p_max["B1_cumul"] = 500000;
  err_max["B1_cumul"] = 0.1;
  legend["B1_cumul"] = "U-Nat. Consumption (cumul.) [t]";

  my_plot("P");
  my_plot("B6");
  my_plot("B1_cumul");
}
