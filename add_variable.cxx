void add_cumul_variable(string file, string var) {
  TFile *myfile = TFile::Open(file.c_str(), "update");
  if (!myfile || myfile->IsZombie()) { 
    cout <<"The file could not be opened!"; 
    delete myfile;
    return;
  }  
  TTree* in_tree = (TTree*)myfile->Get("myTTree"); 

  string new_branch_name = var + "_cumul";
  if (in_tree->GetBranch(new_branch_name.c_str())) {
    std::cout << file << ": Already processed for variable " << var << " !"
              << std::endl;
    return;
  }
  //std::cout << "Adding " << var << " Cumul in " << file << std::endl;
  vector<double> my_cumul_var;
  vector<double> *pMy_cumul_var = &my_cumul_var;
  TBranch *cumul_branch = in_tree->Branch(
      new_branch_name.c_str(), "std::vector<double>", &pMy_cumul_var);

  TTree *loop_tree = in_tree->CloneTree();

  vector<double> *my_var;
  loop_tree->SetBranchAddress(var.c_str(), &my_var);
  Long64_t nentries =
      in_tree->GetEntries();  // read the number of entries in the t3
  std::cout << nentries << endl;

  for (int i = 0; i < nentries; i++) {
    loop_tree->GetEntry(i);
    pMy_cumul_var->clear();
    pMy_cumul_var->reserve(my_var->size());
    std::cout << i << endl;
    double my_cumul = 0;
    for (int j = 0; j < my_var->size(); j++) {
      my_cumul += (*my_var)[i];
      pMy_cumul_var->push_back(my_cumul);
    }
    std::cout << i << endl;
    cumul_branch->Fill();
  }
  in_tree->Write("", TObject::kOverwrite);
  myfile->Close("R");
  
}

void add_variable() {
  add_cumul_variable("systematic/tree.root", "B1");
  add_cumul_variable("ref/tree.root", "B1");
  string file__[6] = {"cooling", "burnup", "cycle", "assay", "power", "separation"};
  for(int i = 0 ; i < 6; i++){
    string filename = "systematic_" + file__[i] + "/tree.root";
    add_cumul_variable(filename ,"B1");
  }
}
