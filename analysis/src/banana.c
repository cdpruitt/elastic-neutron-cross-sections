{

  TObject *obj= obj = gPad->WaitPrimitive("CUTG","CutG");
  CUTG->SetName("bananas");
  bananas->Print();
  int n = bananas->GetN();
  double *x = bananas->GetX();
  double *y = bananas->GetY();
  //open file for output of bananas
  ofstream outfile("banana.dat",ios::out);
  outfile << n << endl;
  for (int i=0;i<n;i++)outfile << *x++ << " " << *y++ << 
    endl;
  outfile.close();
  obj->Delete();
  //delete obj;

  return;
}
