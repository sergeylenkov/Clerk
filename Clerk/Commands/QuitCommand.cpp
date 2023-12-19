#include "QuitCommand.h"

using namespace Clerk::Commands;

QuitCommand::QuitCommand(wxFrame& frame):
	_frame(frame) {
}

void QuitCommand::Execute() {
	_frame.Close();
}