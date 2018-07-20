void add_cumul_variable(string file, string var) {
  std::cout << file << endl;
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
  TTree *loop_tree = in_tree->CloneTree();
  vector<double> pMy_cumul_var ;
  TBranch *cumul_branch = in_tree->Branch(
      new_branch_name.c_str(), "std::vector<double>", &pMy_cumul_var);


  vector<double> *my_var;
  loop_tree->SetBranchAddress(var.c_str(), &my_var);
  Long64_t nentries =
      in_tree->GetEntries();  // read the number of entries in the t3
  cout << __LINE__ << endl;
  for (int i = 0; i < nentries; i++) {
  cout << __LINE__ << endl;
    loop_tree->GetEntry(i);
  cout << __LINE__ << endl;
    pMy_cumul_var.clear();
  cout << __LINE__ << endl;
    pMy_cumul_var.reserve(my_var->size());
    double my_cumul =0;
  cout << __LINE__ << endl;
    for (int j = 0; j < my_var->size(); j++) {
      my_cumul += (*my_var)[j];
      pMy_cumul_var.push_back(my_cumul);
  cout << __LINE__ << endl;
    }
    cumul_branch->Fill();
  cout << __LINE__ << endl;
  }
  cout << __LINE__ << endl;
  in_tree->Write("", TObject::kOverwrite);
 delete myfile; 
}

void add_variable() {
  add_cumul_variable("all/tree.root", "B1");
  //add_cumul_variable("systematic_no_cooling_no_cycle/tree.root", "B1");
  //add_cumul_variable("ref/tree.root", "B1");
  //string file__[6] = {"cooling", "burnup", "cycle", "assay", "power", "separation"};
  //for(int i = 5 ; i < 6; i++){
  //  string filename = "systematic_" + file__[i] + "/tree.root";
  //  add_cumul_variable(filename ,"B1");
  //}
}
