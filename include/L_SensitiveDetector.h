/*
 * L_SensitiveDetector.h
 *
 *  Created on: Oct 3, 2018
 *      Author: vsevolod
 */

#pragma once

#include <G4VSensitiveDetector.hh>
#include "L_Hit.h"
#include "HitDataStructure.hh"
#include "L_EventAction.h"

#include "MCTruthManager.hh"

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class L_SensitiveDetector: public G4VSensitiveDetector {
public:
    L_SensitiveDetector(G4String name);
    virtual ~L_SensitiveDetector();


	void Initialize(G4HCofThisEvent*);

    G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    G4bool ProcessHitsL(const G4Step*, G4TouchableHistory*);

	void EndOfEvent(G4HCofThisEvent*);
private:

    L_HitsCollection* _Collection;
	G4String SDname;

    MCTruthManager *mcTruthMan;

};

// The method stolen from internet for separation words inside the string
// and returning a vector containig string of words
template <class Container>
void splitName(const std::string& str, Container& cont)
{
    std::istringstream iss(str);
    std::copy(std::istream_iterator<std::string>(iss),
         std::istream_iterator<std::string>(),
         std::back_inserter(cont));
}
