#pragma once

class ICommand {
public:
	virtual void execute() = 0;
};

class UndoCommand : ICommand {
	void execute() {}
};