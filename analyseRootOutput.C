#include <TFile.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

#include <iostream>

//  use output TTree for short summary
void shortsummary(TString fname) {
  if (fname.IsNull()) fname = "xrayout.root";
  TFile *fin = new TFile(fname.Data(), "READ");
  TTreeReader nt1("Score", fin);
  TTreeReaderValue<int>    evid1(nt1, "EventID");
   
  TTreeReader nt2("Watch", fin);
  TTreeReaderValue<int>    evid2(nt2, "EventID");
      
  // event loop
  int counterh = 0;
  int counterw = 0;
  int prevev   = -1;
  while (nt1.Next())
  {
    if (*evid1 != prevev) { // primary particle has id=1
      counterh++;
      prevev = *evid1;
    }
  }
  std::cout << "counted gas hits: " << counterh << std::endl;
}

//  use output TTree for summary
void summary(TString fname) {
  if (fname.IsNull()) fname = "xrayout.root";
  TFile *fin = new TFile(fname.Data(), "READ");
  TTreeReader nt1("Score", fin);
  TTreeReaderValue<int>    evid1(nt1, "EventID");
  TTreeReaderValue<double> edep(nt1, "Edep");
  TTreeReaderValue<double> px(nt1, "Posx");
  TTreeReaderValue<double> py(nt1, "Posy");
  TTreeReaderValue<double> pz(nt1, "Posz");

  // event loop
  while (nt1.Next())
  {
    std::cout << "<<< event " << *evid1 << std::endl;
    std::cout << "deposited energy: " << *edep << std::endl;
    std::cout << "pos x : " << *px << std::endl;
    std::cout << "pos y : " << *py << std::endl;
    std::cout << "pos z : " << *pz << std::endl;
  }
}
