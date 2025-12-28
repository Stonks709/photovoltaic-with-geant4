#include "BVRunAction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

BVRunAction::BVRunAction() : G4UserRunAction()
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetNtupleMerging(true);
  analysisManager->SetVerboseLevel(1);
  
  // --- 1. MEMBUAT NTUPLE (DATA MENTAH UNTUK PYTHON) ---
  analysisManager->CreateNtuple("SolarData", "Photons Captured");
  analysisManager->CreateNtupleIColumn("PhotonCount");
  analysisManager->CreateNtupleDColumn("TotalEnergy");
  analysisManager->FinishNtuple();

  // --- 2. MEMBUAT HISTOGRAM (UNTUK VISUALISASI ROOT) ---
  
  // Histogram ID 0: Photon Count
  // Judul: "Photon Count from Radioluminescence"
  // Bins: 100, Range: 0 sampai 500 (Estimasi dari hasil sebelumnya 177)
  analysisManager->CreateH1("hPhotonCount", "Photon Count from Radioluminescence", 100, 0, 500);
  analysisManager->SetH1XAxisTitle(0, "Photon Count");
  analysisManager->SetH1YAxisTitle(0, "Frequency");

  // Histogram ID 1: Total Energy
  // Judul: "Total Energy Recorded"
  // Bins: 100, Range: 0 sampai 0.001 MeV (Estimasi dari hasil sebelumnya 0.0004 MeV)
  analysisManager->CreateH1("hTotalEnergy", "Total Energy Recorded", 100, 0, 0.001*MeV);
  analysisManager->SetH1XAxisTitle(1, "Total Energy (MeV)");
  analysisManager->SetH1YAxisTitle(1, "Frequency");
}

BVRunAction::~BVRunAction()
{
}

void BVRunAction::BeginOfRunAction(const G4Run*)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile("Betavoltaic_Output.root");
}

void BVRunAction::EndOfRunAction(const G4Run* run)
{
  auto analysisManager = G4AnalysisManager::Instance();
  
  // Tulis dan Tutup File
  // Di Master Thread, fungsi ini akan melakukan MERGING otomatis
  analysisManager->Write();
  analysisManager->CloseFile();
}