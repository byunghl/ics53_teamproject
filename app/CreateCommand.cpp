#ifndef CREATECOMMAND_CPP
#define CREATECOMMAND_CPP

#include "Command.hpp"

class CreateCommand : public Command {
public:
	virtual void execute() {
		std::cout << "CreateCommand is executed" << std::endl;
	}
};


#endif