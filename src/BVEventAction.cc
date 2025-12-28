#include "BVEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

// Constructor: Inisialisasi variabel hitungan ke 0
BVEventAction::BVEventAction()
 : G4UserEventAction(), fPhotonCount(0), fTotalEnergy(0.)
{}

BVEventAction::~BVEventAction()
{}

void BVEventAction::BeginOfEventAction(const G4Event*)
{
  fPhotonCount = 0;     // Reset jumlah foton jadi 0
  fTotalEnergy = 0.;    // Reset energi total jadi 0
}

void BVEventAction::EndOfEventAction(const G4Event*)
{
  auto analysisManager = G4AnalysisManager::Instance();

  // 1. Isi Ntuple (Data mentah untuk Python)
  // Kolom 0: Jumlah Foton
  analysisManager->FillNtupleIColumn(0, fPhotonCount);
  // Kolom 1: Total Energi
  analysisManager->FillNtupleDColumn(1, fTotalEnergy);
  // Tambahkan baris baru di tabel
  analysisManager->AddNtupleRow();

  // 2. Isi Histogram (Data visual untuk ROOT Viewer)
  // Histogram ID 0: Photon Count
  analysisManager->FillH1(0, fPhotonCount);

  // Histogram ID 1: Total Energy
  analysisManager->FillH1(1, fTotalEnergy);
}