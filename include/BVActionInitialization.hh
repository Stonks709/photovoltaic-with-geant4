#ifndef BVActionInitialization_h
#define BVActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class BVActionInitialization : public G4VUserActionInitialization
{
  public:
    BVActionInitialization();
    virtual ~BVActionInitialization();

    // Fungsi untuk thread utama (Master)
    virtual void BuildForMaster() const;

    // Fungsi untuk thread pekerja (Worker) - di sini logika utama didaftarkan
    virtual void Build() const;
};

#endif