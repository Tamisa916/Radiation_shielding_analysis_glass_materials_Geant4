#include "ActionInitialization.hh"

#include "RunAction.hh"

#include "PrimaryGeneratorAction.hh"

namespace TC {
	void ActionInitialization::Build() const {
		SetUserAction(new PrimaryGeneratorAction);
                SetUserAction(new RunAction());
	}

	void ActionInitialization::BuildForMaster() const {

	}
}