void add_cumul_variable(string file, string var) {
  std::cout << file << endl;
  TFile *myfile = TFile::Open(file.c_str(), "update");
  if (!myfile || myfile->IsZombie()) { 
    cout <<"The file could not be opened!"; 
    delete myfile;
    return;
  }  
  std::cout << __LINE__ << endl;
  TTree* in_tree = (TTree*)myfile->Get("myTTree"); 
  string new_branch_name = var + "_cumul";
  if (in_tree->GetBranch(new_branch_name.c_str())) {
    std::cout << file << ": Already processed for variable " << var << " !"
              << std::endl;
    return;
  }
  TTree *loop_tree = in_tree->CloneTree();
  vector<double> my_cumul_var;
  vector<double> *pMy_cumul_var = &my_cumul_var;
  TBranch *cumul_branch = in_tree->Branch(
      new_branch_name.c_str(), "std::vector<double>", &pMy_cumul_var);


  vector<double> *my_var;
  loop_tree->SetBranchAddress(var.c_str(), &my_var);
  Long64_t nentries =
      in_tree->GetEntries();  // read the number of entries in the t3

  for (int i = 0; i < nentries; i++) {
  std::cout << __LINE__ << endl;
    loop_tree->GetEntry(i);
  std::cout << __LINE__ << endl;
    pMy_cumul_var->clear();
    pMy_cumul_var->reserve(my_var->size());
    double my_cumul = 0;
  std::cout << __LINE__ << endl;
    for (int j = 0; j < my_var->size(); j++) {
      my_cumul += (*my_var)[j];
      
  std::cout << __LINE__ << endl;
      pMy_cumul_var->push_back(my_cumul);
    }
    cumul_branch->Fill();
  std::cout << __LINE__ << endl;
  }
  in_tree->Write("", TObject::kOverwrite);
  std::cout << __LINE__ << endl;
 delete myfile; 
}

void add_variable() {
  //add_cumul_variable("systematic/old/tree.root", "B1");
  //add_cumul_variable("systematic_no_cooling_no_cycle/tree.root", "B1");
  //add_cumul_variable("ref/tree.root", "B1");
  string file__[6] = {"cooling", "burnup", "cycle", "assay", "power", "separation"};
  for(int i = 5 ; i < 6; i++){
    string filename = "systematic_" + file__[i] + "/tree.root";
    add_cumul_variable(filename ,"B1");
  }
}
