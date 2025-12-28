#include "BVDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

BVDetectorConstruction::BVDetectorConstruction() : G4VUserDetectorConstruction(), fScoringVolume(nullptr) {}

BVDetectorConstruction::~BVDetectorConstruction() {}

G4VPhysicalVolume* BVDetectorConstruction::Construct()
{
  G4NistManager* nist = G4NistManager::Instance();
  G4bool checkOverlaps = true;

  // --- 1. MATERIALS ---
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* si_mat = nist->FindOrBuildMaterial("G4_Si");
  
  // Tritium Gas (Approximated as Hydrogen for simplicity)
  G4Material* tritium_mat = nist->FindOrBuildMaterial("G4_H");

  // Phosphor (ZnS)
  G4Element* Zn = nist->FindOrBuildElement("Zn");
  G4Element* S  = nist->FindOrBuildElement("S");
  G4Material* phosphor_mat = new G4Material("ZnS", 4.09*g/cm3, 2);
  phosphor_mat->AddElement(Zn, 1);
  phosphor_mat->AddElement(S, 1);

  // --- 2. OPTICAL PROPERTIES (CRITICAL) ---
  // We need to define how materials behave with light.
  
  // A. Phosphor Properties (Scintillator)
  std::vector<G4double> phos_Energy = { 2.0*eV, 2.5*eV, 3.0*eV }; // Visible light range
  std::vector<G4double> phos_RIndex = { 2.36, 2.36, 2.36 };       // Refractive Index of ZnS
  std::vector<G4double> phos_Abs    = { 1.0*m, 1.0*m, 1.0*m };    // Transparent to its own light
  std::vector<G4double> phos_Scint  = { 1.0, 1.0, 1.0 };          // Relative spectrum intensity

  G4MaterialPropertiesTable* mptPhosphor = new G4MaterialPropertiesTable();
  mptPhosphor->AddProperty("RINDEX", phos_Energy, phos_RIndex);
  mptPhosphor->AddProperty("ABSLENGTH", phos_Energy, phos_Abs);
  mptPhosphor->AddProperty("SCINTILLATIONCOMPONENT1", phos_Energy, phos_Scint);
  mptPhosphor->AddConstProperty("SCINTILLATIONYIELD", 50000./MeV); // High yield!
  mptPhosphor->AddConstProperty("RESOLUTIONSCALE", 1.0);
  mptPhosphor->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 0.0*ns);
  mptPhosphor->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
  phosphor_mat->SetMaterialPropertiesTable(mptPhosphor);

  // B. Silicon Properties (Absorber)
  std::vector<G4double> si_RIndex = { 3.4, 3.4, 3.4 }; 
  std::vector<G4double> si_Abs    = { 0.001*mm, 0.001*mm, 0.001*mm }; // Absorbs light immediately
  G4MaterialPropertiesTable* mptSi = new G4MaterialPropertiesTable();
  mptSi->AddProperty("RINDEX", si_RIndex, si_RIndex);
  mptSi->AddProperty("ABSLENGTH", si_RIndex, si_Abs);
  si_mat->SetMaterialPropertiesTable(mptSi);

  // C. Tritium Properties (Transparent gas)
  std::vector<G4double> gas_RIndex = { 1.0, 1.0, 1.0 };
  G4MaterialPropertiesTable* mptGas = new G4MaterialPropertiesTable();
  mptGas->AddProperty("RINDEX", phos_Energy, gas_RIndex);
  tritium_mat->SetMaterialPropertiesTable(mptGas);


  // --- 3. DIMENSIONS ---
  // Solar Panel: 3.5 cm x 1.5 cm
  G4double panelX = 3.5*cm;
  G4double panelY = 1.5*cm;
  G4double panelZ = 0.5*mm; // Thickness of panel

  // Tritium Volume: 466.29 mm3
  // Area = 35mm * 15mm = 525 mm2
  // Thickness = Volume / Area = 466.29 / 525 = 0.888 mm
  G4double tritiumZ = 0.888*mm;

  G4double phosphorZ = 0.05*mm; // 50 micron coating

  // --- 4. VOLUMES ---
  
  // World
  G4Box* solidWorld = new G4Box("World", 5*cm, 5*cm, 5*cm);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps);

  // 1. Solar Panel Bottom
  G4Box* solidPanel = new G4Box("Panel", panelX/2, panelY/2, panelZ/2);
  G4LogicalVolume* logicPanel = new G4LogicalVolume(solidPanel, si_mat, "SolarPanel_LV");
  fScoringVolume = logicPanel; // We count hits here

  new G4PVPlacement(0, G4ThreeVector(0,0, -panelZ/2 - phosphorZ - tritiumZ/2), logicPanel, "Panel_Bottom", logicWorld, false, 0, checkOverlaps);

  // 2. Phosphor Bottom
  G4Box* solidPhos = new G4Box("Phosphor", panelX/2, panelY/2, phosphorZ/2);
  G4LogicalVolume* logicPhos = new G4LogicalVolume(solidPhos, phosphor_mat, "Phosphor_LV");
  
  new G4PVPlacement(0, G4ThreeVector(0,0, -phosphorZ/2 - tritiumZ/2), logicPhos, "Phosphor_Bottom", logicWorld, false, 0, checkOverlaps);

  // 3. Tritium Gas (Center)
  G4Box* solidTritium = new G4Box("Tritium", panelX/2, panelY/2, tritiumZ/2);
  G4LogicalVolume* logicTritium = new G4LogicalVolume(solidTritium, tritium_mat, "Tritium_LV");
  
  new G4PVPlacement(0, G4ThreeVector(0,0,0), logicTritium, "Tritium_Layer", logicWorld, false, 0, checkOverlaps);

  // 4. Phosphor Top
  new G4PVPlacement(0, G4ThreeVector(0,0, phosphorZ/2 + tritiumZ/2), logicPhos, "Phosphor_Top", logicWorld, false, 1, checkOverlaps);

  // 5. Solar Panel Top
  new G4PVPlacement(0, G4ThreeVector(0,0, panelZ/2 + phosphorZ + tritiumZ/2), logicPanel, "Panel_Top", logicWorld, false, 1, checkOverlaps);
  
  // A. World Volume (Buat invisible atau wireframe tipis)
  G4VisAttributes* worldVisAtt = new G4VisAttributes(G4Color(1.0, 1.0, 1.0)); // Putih
  worldVisAtt->SetVisibility(false); // Sembunyikan world box agar tidak mengganggu
  // Jika ingin terlihat sebagai kotak kawat tipis, ganti baris atas dengan:
  // worldVisAtt->SetForceWireframe(true);
  logicWorld->SetVisAttributes(worldVisAtt);

  // B. Tritium Gas (Merah, Wireframe agar transparan)
  // G4Color(Merah, Hijau, Biru, Alpha/Transparansi)
  G4VisAttributes* tritiumVisAtt = new G4VisAttributes(G4Color(1.0, 0.0, 0.0, 0.8)); // Merah agak transparan
  tritiumVisAtt->SetForceWireframe(true); // Paksa jadi kerangka kawat
  logicTritium->SetVisAttributes(tritiumVisAtt);

  // C. Lapisan Fosfor (Hijau Kekuningan 'Lime', Solid)
  G4VisAttributes* phosphorVisAtt = new G4VisAttributes(G4Color(0.5, 1.0, 0.0)); // Lime Green
  phosphorVisAtt->SetVisibility(true);
  phosphorVisAtt->SetForceSolid(true); // Paksa terlihat padat berisi
  logicPhos->SetVisAttributes(phosphorVisAtt);

  // D. Panel Surya (Biru Tua, Solid)
  G4VisAttributes* panelVisAtt = new G4VisAttributes(G4Color(0.0, 0.0, 0.8)); // Dark Blue
  panelVisAtt->SetVisibility(true);
  panelVisAtt->SetForceSolid(true); // Paksa terlihat padat berisi
  logicPanel->SetVisAttributes(panelVisAtt);

  return physWorld;
}