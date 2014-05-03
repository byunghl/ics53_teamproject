#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;

};


#endif // COMMAND_HPP