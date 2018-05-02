void merge_tree_in_file(string file) {
  
  
  
  
  
   
  TFile *myfile = TFile::Open(file.c_str(), "update");
  if (!myfile || myfile->IsZombie()) { 
    cout <<"The file could not be opened!"; 
    delete myfile;
    return;
  }
  int version = 1;
  for (int i = 1; i< 100; i++){
    string in_name = "myTTree;" + std::to_string(i);
    TTree* in_tree = (TTree*)myfile->Get(in_name.c_str());
    if (in_tree == nullptr){
      break;
    }
    version++;
    TList* myList = new TList();
    myList->Add(in_tree);
    TTree *newtree = TTree::MergeTrees(myList);
    string out_name = "T" + std::to_string(i);
    newtree->SetName(out_name.c_str()); 
    newtree->Write();
  }
  TList* myList = new TList();
  for (int i = 1; i < version; i++){
    string in_name = "T" + std::to_string(i);
    myList->Add((TTree*)myfile->Get(in_name.c_str()));
  }
  if(version == 1){
    return;
  }

  for (int i = 1; i < version; i++){
    string in_name = "myTTree;" + std::to_string(i);
    myfile->Delete(in_name.c_str());
  }
  TTree *newtree = TTree::MergeTrees(myList);
  newtree->SetName("myTTree");
  newtree->Write(); 
  for (int i = 1; i < version; i++){
    string in_name = "T" + std::to_string(i);
    myfile->Delete(in_name.c_str());
  }
  
}

void merge_tree() {
  merge_tree_in_file("systematic/old/tree.root");
  merge_tree_in_file("ref/tree.root");
  string file__[6] = {"cooling", "burnup", "cycle", "assay", "power", "separation"};
  for(int i = 0 ; i < 6; i++){
    string filename = "systematic_" + file__[i] + "/tree.root";
    merge_tree_in_file(filename );
  }
}
