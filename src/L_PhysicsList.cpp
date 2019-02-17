#include "L_PhysicsList.h"



L_PhysicsList::L_PhysicsList() : QGSP_BERT()
{
    theCerenkovProcess = 0;
    defaultCutValue = 1.0*mm;
    SetVerboseLevel(0);
}

L_PhysicsList::~L_PhysicsList()
{
    delete theCerenkovProcess;
}

// Construct particle and physics process
void L_PhysicsList::ConstructParticle()
{
    QGSP_BERT::ConstructParticle();
    G4OpticalPhoton::OpticalPhotonDefinition();
}


void L_PhysicsList::ConstructProcess()
{
    QGSP_BERT::ConstructProcess();
    ConstructOp();
}
//void SetCuts();


void L_PhysicsList::ConstructOp()
{


    G4ParticleTable::G4PTblDicIterator *theParticleIterator = this->GetParticleIterator();

    G4cout<<" 000 "<<G4endl;

    // Optical Photon Processes
    theCerenkovProcess = new G4Cerenkov("Cerenkov");

    G4cout<<" 111 "<<G4endl;

    SetVerbose(0);

    theCerenkovProcess->SetMaxNumPhotonsPerStep(300);
    theCerenkovProcess->SetTrackSecondariesFirst(true);
    G4cout<<" 111 "<<G4endl;

    theParticleIterator->reset();
    while( (*theParticleIterator)() ) {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();
        if (theCerenkovProcess->IsApplicable(*particle)) {
            G4cout << "Add Cerenkov process to " << particleName << G4endl;
            pmanager->AddProcess(theCerenkovProcess);
            pmanager->SetProcessOrdering(theCerenkovProcess, idxPostStep);
        }
        if (particleName == "opticalphoton") {
            G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
            pmanager->AddDiscreteProcess(new G4OpAbsorption());
            pmanager->AddDiscreteProcess(new G4OpRayleigh());
            pmanager->AddDiscreteProcess(new G4OpBoundaryProcess());
        }
    }
}


void L_PhysicsList::SetVerbose(G4int verbose)
{
    theCerenkovProcess->SetVerboseLevel(verbose);

}

