#include "OpenReportTabCommand.h"

using namespace Clerk::Commands;

OpenReportTabCommand::OpenReportTabCommand(ICommandsReceiver& receiver):
	_receiver(receiver), 
	_reportId(-1) {
}

void OpenReportTabCommand::SetReportId(int id) {
	_reportId = id;
}

void OpenReportTabCommand::Execute() {
	_receiver.OpenReportTab(_reportId);
}