#ifndef Reaction_Messenger_h
#define Reaction_Messenger_h 1

#include "Reaction.hh"
#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"

class Reaction_Messenger: public G4UImessenger
{
public:
  Reaction_Messenger(Reaction *);
  ~Reaction_Messenger();
  
  void SetNewValue(G4UIcommand*, G4String);
  
private:
  Reaction* aReaction;  
  G4UIdirectory             *ReactionDir;
  G4UIcmdWithADouble        *SetBmaxCmd;
  G4UIcmdWithoutParameter   *RepBmaxCmd;
};

#endif

