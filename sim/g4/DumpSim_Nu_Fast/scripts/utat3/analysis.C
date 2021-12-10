Bool_t IsNDTouched(
    Double_t x,
    Double_t y,
    Double_t z,
    Double_t px,
    Double_t py,
    Double_t pz)
{
  // coordinate origin is the center of gravity of the target geometry.
  // target geometry has 1.7 cm diameter, 1.5 m long.

  Bool_t result = kFALSE;

  const Double_t distance = 304; // distance between the target downstream endpoint to the ND front suface in meter.
  const Double_t target_length = 1.5; // the length of the target in meter.
  const Double_t nd_x = 4; // size of ND in x-axis (m) (wide)
  const Double_t nd_y = 3; // size of ND in y-axis (m) (tall)
  const Double_t nd_z = 5; // size of ND in z-axis (m) (deep)

  Double_t t = 0; // space line parameter
  Double_t p = sqrt(px*px + py*py + pz*pz);

  // calculate t-parameter at ND surface.
  t = ((distance + 0.5*target_length) - z)/(pz/p);

  // now project the line to the ND surface and find x- and y-coordinates at z = 574 m + 0.5 * target_length
  Doubl_t proj_x, proj_y;
  proj_x = t * px / p + x;
  proj_y = t * py / p + y;

  // determine the particle hit the ND or not.
  if( proj_x > -0.5*nd_x && proj_x < 0.5*nd_x && proj_y > -0.5*nd_y && proj_y < 0.8*nd_y )
  {
    cout << "the incident beam hits the detector!" << endl;
    cout << "incident beam profile is: " << endl;
    cout << " p = ( " << px << ", " << py << ", " << pz << ") [GeV/c]" << endl;
    cout << " vx = ( " << x << ", " << y << ", " << z << ") [m]" << endl;
    cout << "hit position at the ND front surface is: " << endl;
    cout << " hit = ( " << proj_x << ", " << proj_y << ", " << distance + 0.5*target_length << " ) [m]" << endl;
    result = kTRUE;
  }
  else
    result = kFALSE;

  return result;
}

void analysis(
    string inputFileName="DUNE_DumpSim1.root",
    string outputPrefix="DUNE_DumpSim1")
{
  // open the file
  TFile fInput(inputFileName.c_str(), "read");
  // read the TTree
  TTree* t = (TTree*)fInput.Get("DUNE_DumpSim");
  double KE;
  double px, py, pz;
  char pid[32];
  char proc[64];

  // particle counter variables
  ULong64_t cnt_gamma = 0;
  ULong64_t cnt_pizero = 0;
  ULong64_t cnt_piplus = 0;
  ULong64_t cnt_piminus = 0;
  ULong64_t cnt_kaonzero = 0;
  ULong64_t cnt_kaonplus = 0;
  ULong64_t cnt_kaonminus = 0;
  ULong64_t cnt_eta = 0;
  ULong64_t cnt_nu_e = 0;
  ULong64_t cnt_nu_mu = 0;
  ULong64_t cnt_anti_nu_e = 0;
  ULong64_t cnt_anti_nu_mu = 0;
  ULong64_t cnt_proton = 0;
  ULong64_t cnt_neutron = 0;
  ULong64_t cnt_ele = 0;
  ULong64_t cnt_pos = 0;

  // assign variables to leaves of TTree
  t->SetBranchAddress("KE", &KE);
  t->SetBranchAddress("px", &px);
  t->SetBranchAddress("py", &py);
  t->SetBranchAddress("pz", &pz);
  t->SetBranchAddress("pid", pid);
  t->SetBranchAddress("proc", proc);

  const ULong64_t nEntries = t->GetEntries();

  // Prepare output filestreams
  ofstream fout_pizero;
  ofstream fout_piplus;
  ofstream fout_piminus;
  ofstream fout_kaonzero;
  ofstream fout_kaonplus;
  ofstream fout_kaonminus;
  ofstream fout_eta;
  ofstream fout_gamma_eBrem;
  ofstream fout_gamma;
  ofstream fout_nu_e;
  ofstream fout_nu_mu;
  ofstream fout_anti_nu_e;
  ofstream fout_anti_nu_mu;
  ofstream fout_ele;
  ofstream fout_pos;

  TString outFile_pizero(outputPrefix);
  TString outFile_piplus(outputPrefix);
  TString outFile_piminus(outputPrefix);
  TString outFile_kaonzero(outputPrefix);
  TString outFile_kaonplus(outputPrefix);
  TString outFile_kaonminus(outputPrefix);
  TString outFile_eta(outputPrefix);
  TString outFile_gamma_eBrem(outputPrefix);
  TString outFile_gamma(outputPrefix);
  TString outFile_nu_e(outputPrefix);
  TString outFile_nu_mu(outputPrefix);
  TString outFile_anti_nu_e(outputPrefix);
  TString outFile_anti_nu_mu(outputPrefix);
  TString outFile_ele(outputPrefix);
  TString outFile_pos(outputPrefix);
  TString outFile_root(outputPrefix);

  outFile_pizero      = outFile_pizero + "_pizero.txt";
  outFile_piplus      = outFile_piplus + "_piplus.txt";
  outFile_piminus     = outFile_piminus+ "_piminus.txt";
  outFile_kaonzero    = outFile_kaonzero+ "_kaonzero.txt";
  outFile_kaonplus    = outFile_kaonplus+ "_kaonplus.txt";
  outFile_kaonminus   = outFile_kaonminus+ "_kaonminus.txt";
  outFile_eta         = outFile_eta    + "_eta.txt";
  outFile_gamma_eBrem = outFile_gamma_eBrem + "_gamma_eBrem.txt";
  outFile_gamma       = outFile_gamma  + "_gamma.txt";
  outFile_nu_e        = outFile_nu_e   + "_nu_e.txt";
  outFile_nu_mu       = outFile_nu_mu  + "_nu_mu.txt";
  outFile_anti_nu_e   = outFile_anti_nu_e   + "_anti_nu_e.txt";
  outFile_anti_nu_mu  = outFile_anti_nu_mu  + "_anti_nu_mu.txt";
  outFile_ele         = outFile_ele   + "_ele.txt";
  outFile_pos         = outFile_pos  + "_pos.txt";
  outFile_root        = outFile_root   + "_analysis.root";

  fout_pizero.open(outFile_pizero);
  fout_piplus.open(outFile_piplus);
  fout_piminus.open(outFile_piminus);
  fout_kaonzero.open(outFile_kaonzero);
  fout_kaonplus.open(outFile_kaonplus);
  fout_kaonminus.open(outFile_kaonminus);
  fout_eta.open(outFile_eta);
  fout_gamma_eBrem.open(outFile_gamma_eBrem);
  fout_gamma.open(outFile_gamma);
  fout_nu_e.open(outFile_nu_e);
  fout_nu_mu.open(outFile_nu_mu);
  fout_anti_nu_e.open(outFile_anti_nu_e);
  fout_anti_nu_mu.open(outFile_anti_nu_mu);
  fout_ele.open(outFile_ele);
  fout_pos.open(outFile_pos);

  // Prepare histogram objects
  TH2D* h_angular2d_pizero = new TH2D("h_angular2d_pizero", ";Energy (GeV);cos(#theta)", 120, 0, 120, 100, 0, 1);
  TH1D* h_energy_pizero = new TH1D("h_energy_pizero", ";Energy (GeV);Entries/POT", 120, 0, 120);
  TH1D* h_angle_pizero = new TH1D("h_angle_pizero", ";cos(#theta)", 100, 0, 1);
  TH2D* h_angular2d_piplus = new TH2D("h_angular2d_piplus", ";Energy (GeV);cos(#theta)", 120, 0, 120, 100, 0, 1);
  TH1D* h_energy_piplus = new TH1D("h_energy_piplus", ";Energy (GeV);Entries/POT", 120, 0, 120);
  TH1D* h_angle_piplus = new TH1D("h_angle_piplus", ";cos(#theta)", 100, 0, 1);
  TH2D* h_angular2d_piminus = new TH2D("h_angular2d_piminus", ";Energy (GeV);cos(#theta)", 120, 0, 120, 100, 0, 1);
  TH1D* h_energy_piminus = new TH1D("h_energy_piminus", ";Energy (GeV);Entries/POT", 120, 0, 120);
  TH1D* h_angle_piminus = new TH1D("h_angle_piminus", ";cos(#theta)", 100, 0, 1);
  TH2D* h_angular2d_kaonzero = new TH2D("h_angular2d_kaonzero", ";Energy (GeV);cos(#theta)", 120, 0, 120, 100, 0, 1);
  TH1D* h_energy_kaonzero = new TH1D("h_energy_kaonzero", ";Energy (GeV);Entries/POT", 120, 0, 120);
  TH1D* h_angle_kaonzero = new TH1D("h_angle_kaonzero", ";cos(#theta)", 100, 0, 1);
  TH2D* h_angular2d_kaonplus = new TH2D("h_angular2d_kaonplus", ";Energy (GeV);cos(#theta)", 120, 0, 120, 100, 0, 1);
  TH1D* h_energy_kaonplus = new TH1D("h_energy_kaonplus", ";Energy (GeV);Entries/POT", 120, 0, 120);
  TH1D* h_angle_kaonplus = new TH1D("h_angle_kaonplus", ";cos(#theta)", 100, 0, 1);
  TH2D* h_angular2d_kaonminus = new TH2D("h_angular2d_kaonminus", ";Energy (GeV);cos(#theta)", 120, 0, 120, 100, 0, 1);
  TH1D* h_energy_kaonminus = new TH1D("h_energy_kaonminus", ";Energy (GeV);Entries/POT", 120, 0, 120);
  TH1D* h_angle_kaonminus = new TH1D("h_angle_kaonminus", ";cos(#theta)", 100, 0, 1);
  TH2D* h_angular2d_eta = new TH2D("h_angular2d_eta", ";Energy (GeV);cos(#theta)", 120, 0, 120, 100, 0, 1);
  TH1D* h_energy_eta = new TH1D("h_energy_eta", ";Energy (GeV);Entries/POT", 120, 0, 120);
  TH1D* h_angle_eta = new TH1D("h_angle_eta", ";cos(#theta)", 100, 0, 1);
  TH2D* h_angular2d_gamma_eBrem = new TH2D("h_angular2d_gamma_eBrem", ";Energy (GeV);cos(#thgamma_eBrem)", 120, 0, 120, 100, 0, 1);
  TH1D* h_energy_gamma_eBrem = new TH1D("h_energy_gamma_eBrem", ";Energy (GeV);Entries/POT", 120, 0, 120);
  TH1D* h_angle_gamma_eBrem = new TH1D("h_angle_gamma_eBrem", ";cos(#thgamma_eBrem)", 100, 0, 1);
  TH2D* h_angular2d_gamma = new TH2D("h_angular2d_gamma", ";Energy (GeV);cos(#thgamma)", 120, 0, 120, 100, 0, 1);
  TH1D* h_energy_gamma = new TH1D("h_energy_gamma", ";Energy (GeV);Entries/POT", 120, 0, 120);
  TH1D* h_angle_gamma = new TH1D("h_angle_gamma", ";cos(#thgamma)", 100, 0, 1);
  TH2D* h_angular2d_nu_e = new TH2D("h_angular2d_nu_e", ";Energy (GeV);cos(#thnu_e)", 200, 0, 20, 100, 0, 1);
  TH1D* h_energy_nu_e = new TH1D("h_energy_nu_e", ";Energy (GeV);Entries/POT", 200, 0, 20);
  TH1D* h_angle_nu_e = new TH1D("h_angle_nu_e", ";cos(#thnu_e)", 100, 0, 1);
  TH2D* h_angular2d_nu_e_lowE = new TH2D("h_angular2d_nu_e_lowE", ";Energy (GeV);cos(#theta)", 200, 0, 1, 100, 0, 1);
  TH1D* h_energy_nu_e_lowE = new TH1D("h_energy_nu_e_lowE", ";Energy (GeV);Entries/POT", 200, 0, 1);
  TH1D* h_angle_nu_e_lowE = new TH1D("h_angle_nu_e_lowE", ";cos(#theta)", 100, 0, 1);
  TH2D* h_angular2d_nu_mu = new TH2D("h_angular2d_nu_mu", ";Energy (GeV);cos(#theta)", 400, 0, 40, 100, 0, 1);
  TH1D* h_energy_nu_mu = new TH1D("h_energy_nu_mu", ";Energy (GeV);Entries/POT", 400, 0, 40);
  TH1D* h_angle_nu_mu = new TH1D("h_angle_nu_mu", ";cos(#theta)", 100, 0, 1);
  TH2D* h_angular2d_nu_mu_lowE = new TH2D("h_angular2d_nu_mu_lowE", ";Energy (GeV);cos(#theta)", 200, 0, 1, 100, 0, 1);
  TH1D* h_energy_nu_mu_lowE = new TH1D("h_energy_nu_mu_lowE", ";Energy (GeV);Entries/POT", 200, 0, 1);
  TH1D* h_angle_nu_mu_lowE = new TH1D("h_angle_nu_mu_lowE", ";cos(#theta)", 100, 0, 1);
  TH2D* h_angular2d_anti_nu_e = new TH2D("h_angular2d_anti_nu_e", ";Energy (GeV);cos(#thanti_nu_e)", 200, 0, 20, 100, 0, 1);
  TH1D* h_energy_anti_nu_e = new TH1D("h_energy_anti_nu_e", ";Energy (GeV);Entries/POT", 200, 0, 20);
  TH1D* h_angle_anti_nu_e = new TH1D("h_angle_anti_nu_e", ";cos(#thanti_nu_e)", 100, 0, 1);
  TH2D* h_angular2d_anti_nu_e_lowE = new TH2D("h_angular2d_anti_nu_e_lowE", ";Energy (GeV);cos(#thanti_nu_e)", 200, 0, 20, 100, 0, 1);
  TH1D* h_energy_anti_nu_e_lowE = new TH1D("h_energy_anti_nu_e_lowE", ";Energy (GeV);Entries/POT", 200, 0, 20);
  TH1D* h_angle_anti_nu_e_lowE = new TH1D("h_angle_anti_nu_e_lowE", ";cos(#thanti_nu_e)", 100, 0, 1);
  TH2D* h_angular2d_anti_nu_mu = new TH2D("h_angular2d_anti_nu_mu", ";Energy (GeV);cos(#thanti_nu_mu)", 400, 0, 40, 100, 0, 1);
  TH1D* h_energy_anti_nu_mu = new TH1D("h_energy_anti_nu_mu", ";Energy (GeV);Entries/POT", 400, 0, 40);
  TH1D* h_angle_anti_nu_mu = new TH1D("h_angle_anti_nu_mu", ";cos(#thanti_nu_mu)", 100, 0, 1);
  TH2D* h_angular2d_anti_nu_mu_lowE = new TH2D("h_angular2d_anti_nu_mu_lowE", ";Energy (GeV);cos(#thanti_nu_mu)", 200, 0, 1, 100, 0, 1);
  TH1D* h_energy_anti_nu_mu_lowE = new TH1D("h_energy_anti_nu_mu_lowE", ";Energy (GeV);Entries/POT", 200, 0, 1);
  TH1D* h_angle_anti_nu_mu_lowE = new TH1D("h_angle_anti_nu_mu_lowE", ";cos(#thanti_nu_mu)", 100, 0, 1);
  TH2D* h_angular2d_ele = new TH2D("h_angular2d_ele", ";Energy (GeV);cos(#theta)", 120, 0, 120, 100, 0, 1);
  TH1D* h_energy_ele = new TH1D("h_energy_ele", ";Energy (GeV);Entries/POT", 120, 0, 120);
  TH1D* h_angle_ele = new TH1D("h_angle_ele", ";cos(#thele)", 100, 0, 1);
  TH2D* h_angular2d_pos = new TH2D("h_angular2d_pos", ";Energy (GeV);cos(#theta)", 120, 0, 120, 100, 0, 1);
  TH1D* h_energy_pos = new TH1D("h_energy_pos", ";Energy (GeV);Entries/POT", 120, 0, 120);
  TH1D* h_angle_pos = new TH1D("h_angle_pos", ";cos(#thpos)", 100, 0, 1);

  // alphanumeric binning histgram
  const int nChannelLabel = 16;
  TString channelLabel[nChannelLabel] =
  {
    "gamma",
    "pi0",
    "pi+",
    "pi-",
    "kaon0",
    "kaon+",
    "kaon-",
    "eta",
    "nu_e",
    "nu_mu",
    "anti_nu_e",
    "anti_nu_mu",
    "proton",
    "neutron",
    "e-",
    "e+"
  };
  TH1D* h = new TH1D("h", "produced particles", nChannelLabel, 0., (Double_t)nChannelLabel);
  h->SetStats(0);
  for(int i = 1; i <= nChannelLabel; i++) h->GetXaxis()->SetBinLabel(i, channelLabel[i-1]);


  Double_t cos_theta;
  Double_t p;

  for(ULong64_t i = 0; i < nEntries; i++)
  {
    t->GetEntry(i);
    p = sqrt(px*px+py*py+pz*pz);
    cos_theta = pz/p;

    if( i % 100000 == 0 ) cout << "Processing " << i << " out of " << nEntries << " ... (" << Double_t(i)/nEntries*100. << " %)." << endl;
    if( strcmp(pid, "gamma") == 0 )
    {
      fout_gamma << px << " " << py << " " << pz << " " << KE << endl;
      h_angular2d_gamma->Fill(KE, cos_theta);
      h_energy_gamma->Fill(KE);
      h_angle_gamma->Fill(cos_theta);
      cnt_gamma++;
    }
    if( strcmp(pid, "pi0") == 0 )
    {
      fout_pizero << px << " " << py << " " << pz << " " << KE << endl;
      h_angular2d_pizero->Fill(KE, cos_theta);
      h_energy_pizero->Fill(KE);
      h_angle_pizero->Fill(cos_theta);
      cnt_pizero++;
    }
    if( strcmp(pid, "pi+") == 0 )
    {
      fout_piplus << px << " " << py << " " << pz << " " << KE << endl;
      h_angular2d_piplus->Fill(KE, cos_theta);
      h_energy_piplus->Fill(KE);
      h_angle_piplus->Fill(cos_theta);
      cnt_piplus++;
    }
    if( strcmp(pid, "pi-") == 0 )
    {
      fout_piminus << px << " " << py << " " << pz << " " << KE << endl;
      h_angular2d_piminus->Fill(KE, cos_theta);
      h_energy_piminus->Fill(KE);
      h_angle_piminus->Fill(cos_theta);
      cnt_piminus++;
    }
    if( strcmp(pid, "kaon0S") == 0  || strcmp(pid, "kaon0L") == 0 )
    {
      fout_kaonzero << px << " " << py << " " << pz << " " << KE << endl;
      h_angular2d_kaonzero->Fill(KE, cos_theta);
      h_energy_kaonzero->Fill(KE);
      h_angle_kaonzero->Fill(cos_theta);
      cnt_kaonzero++;
    }
    if( strcmp(pid, "kaon+") == 0 )
    {
      fout_kaonplus << px << " " << py << " " << pz << " " << KE << endl;
      h_angular2d_kaonplus->Fill(KE, cos_theta);
      h_energy_kaonplus->Fill(KE);
      h_angle_kaonplus->Fill(cos_theta);
      cnt_kaonplus++;
    }
    if( strcmp(pid, "kaon-") == 0 )
    {
      fout_kaonminus << px << " " << py << " " << pz << " " << KE << endl;
      h_angular2d_kaonminus->Fill(KE, cos_theta);
      h_energy_kaonminus->Fill(KE);
      h_angle_kaonminus->Fill(cos_theta);
      cnt_kaonminus++;
    }
    if( strcmp(pid, "eta") == 0 )
    {
      fout_eta << px << " " << py << " " << pz << " " << KE << endl;
      h_angular2d_eta->Fill(KE, cos_theta);
      h_energy_eta->Fill(KE);
      h_angle_eta->Fill(cos_theta);
      cnt_eta++;
    }
    if( strcmp(pid, "gamma") == 0 && strcmp(proc, "eBrem") == 0 )
    {
      fout_gamma_eBrem << px << " " << py << " " << pz << " " << KE << endl;
      h_angular2d_gamma_eBrem->Fill(KE, cos_theta);
      h_energy_gamma_eBrem->Fill(KE);
      h_angle_gamma_eBrem->Fill(cos_theta);
    }
    if( strcmp(pid, "nu_e") == 0 )
    {
      fout_nu_e << px << " " << py << " " << pz << " " << KE << endl;
      h_angular2d_nu_e->Fill(KE, cos_theta);
      h_energy_nu_e->Fill(KE);
      h_angle_nu_e->Fill(cos_theta);
      h_angular2d_nu_e_lowE->Fill(KE, cos_theta);
      h_energy_nu_e_lowE->Fill(KE);
      h_angle_nu_e_lowE->Fill(cos_theta);
      cnt_nu_e++;
    }
    if( strcmp(pid, "nu_mu") == 0 )
    {
      fout_nu_mu << px << " " << py << " " << pz << " " << KE << endl;
      h_angular2d_nu_mu->Fill(KE, cos_theta);
      h_energy_nu_mu->Fill(KE);
      h_angle_nu_mu->Fill(cos_theta);
      h_angular2d_nu_mu_lowE->Fill(KE, cos_theta);
      h_energy_nu_mu_lowE->Fill(KE);
      h_angle_nu_mu_lowE->Fill(cos_theta);
      cnt_nu_mu++;
    }
    if( strcmp(pid, "anti_nu_e") == 0 )
    {
      fout_anti_nu_e << px << " " << py << " " << pz << " " << KE << endl;
      h_angular2d_anti_nu_e->Fill(KE, cos_theta);
      h_energy_anti_nu_e->Fill(KE);
      h_angle_anti_nu_e->Fill(cos_theta);
      h_angular2d_anti_nu_e_lowE->Fill(KE, cos_theta);
      h_energy_anti_nu_e_lowE->Fill(KE);
      h_angle_anti_nu_e_lowE->Fill(cos_theta);
      cnt_anti_nu_e++;
    }
    if( strcmp(pid, "anti_nu_mu") == 0 )
    {
      fout_anti_nu_mu << px << " " << py << " " << pz << " " << KE << endl;
      h_angular2d_anti_nu_mu->Fill(KE, cos_theta);
      h_energy_anti_nu_mu->Fill(KE);
      h_angle_anti_nu_mu->Fill(cos_theta);
      h_angular2d_anti_nu_mu_lowE->Fill(KE, cos_theta);
      h_energy_anti_nu_mu_lowE->Fill(KE);
      h_angle_anti_nu_mu_lowE->Fill(cos_theta);
      cnt_anti_nu_mu++;
    }
    if( strcmp(pid, "e-") == 0 )
    {
      fout_ele << px << " " << py << " " << pz << " " << KE << endl;
      h_angular2d_ele->Fill(KE, cos_theta);
      h_energy_ele->Fill(KE);
      h_angle_ele->Fill(cos_theta);
      cnt_ele++;
    }
    if( strcmp(pid, "e+") == 0 )
    {
      fout_pos << px << " " << py << " " << pz << " " << KE << endl;
      h_angular2d_pos->Fill(KE, cos_theta);
      h_energy_pos->Fill(KE);
      h_angle_pos->Fill(cos_theta);
      cnt_pos++;
    }
  }

  h->SetBinContent(1, cnt_gamma);
  h->SetBinContent(2, cnt_pizero);
  h->SetBinContent(3, cnt_piplus);
  h->SetBinContent(4, cnt_piminus);
  h->SetBinContent(5, cnt_kaonzero);
  h->SetBinContent(6, cnt_kaonplus);
  h->SetBinContent(7, cnt_kaonminus);
  h->SetBinContent(8, cnt_eta);
  h->SetBinContent(9, cnt_nu_e);
  h->SetBinContent(10, cnt_nu_mu);
  h->SetBinContent(11, cnt_anti_nu_e);
  h->SetBinContent(12, cnt_anti_nu_mu);
  h->SetBinContent(13, cnt_ele);
  h->SetBinContent(14, cnt_pos);

  TFile* fOutput = new TFile(outFile_root, "RECREATE");

  h_angular2d_pizero->Write();
  h_energy_pizero->Write();
  h_angle_pizero->Write();
  h_angular2d_piplus->Write();
  h_energy_piplus->Write();
  h_angle_piplus->Write();
  h_angular2d_piminus->Write();
  h_energy_piminus->Write();
  h_angle_piminus->Write();
  h_angular2d_kaonzero->Write();
  h_energy_kaonzero->Write();
  h_angle_kaonzero->Write();
  h_angular2d_kaonplus->Write();
  h_energy_kaonplus->Write();
  h_angle_kaonplus->Write();
  h_angular2d_kaonminus->Write();
  h_energy_kaonminus->Write();
  h_angle_kaonminus->Write();
  h_angular2d_eta->Write();
  h_energy_eta->Write();
  h_angle_eta->Write();
  h_angular2d_gamma_eBrem->Write();
  h_energy_gamma_eBrem->Write();
  h_angle_gamma_eBrem->Write();
  h_angular2d_gamma->Write();
  h_energy_gamma->Write();
  h_angle_gamma->Write();
  h_angular2d_nu_e->Write();
  h_energy_nu_e->Write();
  h_angle_nu_e->Write();
  h_angular2d_nu_e_lowE->Write();
  h_energy_nu_e_lowE->Write();
  h_angle_nu_e_lowE->Write();
  h_angular2d_nu_mu->Write();
  h_energy_nu_mu->Write();
  h_angle_nu_mu->Write();
  h_angular2d_nu_mu_lowE->Write();
  h_energy_nu_mu_lowE->Write();
  h_angle_nu_mu_lowE->Write();
  h_angular2d_anti_nu_e->Write();
  h_energy_anti_nu_e->Write();
  h_angle_anti_nu_e->Write();
  h_angular2d_anti_nu_e_lowE->Write();
  h_energy_anti_nu_e_lowE->Write();
  h_angle_anti_nu_e_lowE->Write();
  h_angular2d_anti_nu_mu->Write();
  h_energy_anti_nu_mu->Write();
  h_angle_anti_nu_mu->Write();
  h_angular2d_anti_nu_mu_lowE->Write();
  h_energy_anti_nu_mu_lowE->Write();
  h_angle_anti_nu_mu_lowE->Write();
  h_angular2d_ele->Write();
  h_energy_ele->Write();
  h_angle_ele->Write();
  h_angular2d_pos->Write();
  h_energy_pos->Write();
  h_angle_pos->Write();
  h->Write();

  fout_pizero.close();
  fout_piplus.close();
  fout_piminus.close();
  fout_kaonzero.close();
  fout_kaonplus.close();
  fout_kaonminus.close();
  fout_eta.close();
  fout_gamma_eBrem.close();
  fout_gamma.close();
  fout_nu_e.close();
  fout_nu_mu.close();
  fout_anti_nu_e.close();
  fout_anti_nu_mu.close();
  fout_ele.close();
  fout_pos.close();

  fOutput->Close();
  fInput.Close();

}
