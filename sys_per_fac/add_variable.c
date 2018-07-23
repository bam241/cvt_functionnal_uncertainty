{
  TTree* in_tree = (TTree*)_file0->Get("myTTree");
  TTree *loop_tree = in_tree->CloneTree();
  vector<double> my_cumul_var; vector<double> *pMy_cumul_var = &my_cumul_var;
  TBranch *cumul_branch = in_tree->Branch("B1_cumul", "std::vector<double>", &pMy_cumul_var);
  
  vector<double> *my_var;
  loop_tree->SetBranchAddress("B1", &my_var);
  Long64_t nentries =loop_tree->GetEntry();
  
  for (int i = 0; i < nentries; i++) {loop_tree->GetEntry(i);pMy_cumul_var->clear();pMy_cumul_var->reserve(my_var->size());double my_cumul = 0;for (int j = 0; j < my_var->size(); j++) {my_cumul += (*my_var)[j];pMy_cumul_var->push_back(my_cumul);}cumul_branch->Fill();}
  
  in_tree->Write("", TObject::kOverwrite);
}
