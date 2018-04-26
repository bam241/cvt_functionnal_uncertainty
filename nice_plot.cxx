{
    //TFile *file1 = TFile::Open("random/tree.root") 
    TH2F *hpxpy = new TH2F("hpxpy","py ps px",180*12,0,180,150,0,1200);

    myTTree->Draw("(B6)/1000.:T/12>>hpxpy","" ,"");
    Double_t x[180*12];
    Double_t y[180*12];
    Double_t y_sg_p[180*12];
    Double_t y_sg_m[180*12];
    Double_t y_sg[180*12];

    for(int i = 0; i < 180*12; i++){
      x[i] = (double)i/12.;
      TH1D *ttt= hpxpy->ProjectionY("_py",i,i+1);
      ttt->Fit("gaus", "q");
      TF1 *myfunc=ttt->GetFunction("gaus");
      y[i] = myfunc->GetParameter(1);
      y_sg_p[i] = myfunc->GetParameter(1) + myfunc->GetParameter(2);
      y_sg_m[i] = myfunc->GetParameter(1) - myfunc->GetParameter(2);
      y_sg[i] = myfunc->GetParameter(2);

      delete myfunc;
      delete ttt;
    }
   TGraph* gr = new TGraph(180*12,x,y);
   TGraph* gr_p = new TGraph(180*12,x,y_sg_p);
   TGraph* gr_m = new TGraph(180*12,x,y_sg_m);
   gr->Draw("AL");      
   gr_m->Draw("same");      
   gr_p->Draw("same");      
   TGraphErrors* gr_err = new TGraphErrors(180*12,x,y,0,y_sg );
   gr_err->Draw("ap");
   gr_err->SetFillColor(4);
   gr_err->SetFillStyle(3010);

}
