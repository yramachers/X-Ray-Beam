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
   
  // event loop
  int counterh = 0;
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

void projectXY(std::string fnamelist) {
  std::vector<std::string> rootfiles;
  std::string fname;
  ifstream infile; // read list of root files from file
  infile.open(fnamelist.data());
  if (!infile) {
    cout << "Error file list " << fnamelist.data() << endl;
    exit(1);
  }
  while (infile >> fname)
    rootfiles.push_back(fname);
  infile.close();

  TChain* chScore = new TChain("Score");
  for (auto fn : rootfiles) {
    chScore->Add(fn.data());
  }

  TTreeReader nt1(chScore);

  TTreeReaderValue<double> px(nt1, "Posx");
  TTreeReaderValue<double> py(nt1, "Posy");
    
  // event loop
  TCanvas c1("c1", "Picture");
  TH2D* hist = new TH2D("profile", "2D Profile", 100, -0.02, 0.02, 100, -0.02, 0.02);
  while (nt1.Next())
  {
    hist->Fill(*px, *py);
  }
  hist->GetXaxis()->SetTitle("x-position [mm]");
  hist->GetYaxis()->SetTitle("y-position [mm]");
  hist->GetZaxis()->SetTitle("Entries");
  hist->GetXaxis()->SetTitleOffset(1.5);
  hist->GetYaxis()->SetTitleOffset(1.8);
  hist->GetZaxis()->SetTitleOffset(1.4);
  hist->Draw("surf3");
  c1.SaveAs("pic.png");

}

void projectZ(std::string fnamelist)
{
  std::vector<std::string> rootfiles;
  std::string fname;
  ifstream infile; // read list of root files from file
  infile.open(fnamelist.data());
  if (!infile) {
    cout << "Error file list " << fnamelist.data() << endl;
    exit(1);
  }
  while (infile >> fname)
    rootfiles.push_back(fname);
  infile.close();

  TChain* chScore = new TChain("Score");
  for (auto fn : rootfiles) {
    chScore->Add(fn.data());
  }
    
  TTreeReader nt1(chScore);
     
  TTreeReaderValue<double> pz(nt1, "Posz");

  // event loop 
  TCanvas c1("c1", "Picture");
  TH1D* hist = new TH1D("profilez", "Z Profile", 100, -0.29, 0.29);
  while (nt1.Next())
  {
    hist->Fill(*pz);
  }
  hist->GetXaxis()->SetTitle("z-position [mm]");
  hist->GetYaxis()->SetTitle("Entries");
  hist->SetStats(0);
  hist->Draw();  
  c1.SaveAs("picZ.png");
    
}

