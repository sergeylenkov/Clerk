#include "InterestAccrualCommand.h"

using namespace Clerk::Commands;

InterestAccrualCommand::InterestAccrualCommand(ICommandsReceiver& receiver) :
	_receiver(receiver),
	_accountId(-1) {
}

void InterestAccrualCommand::SetAccountId(int id) {
	_accountId = id;
}

void InterestAccrualCommand::Execute() {
	_receiver.OpenInterestAccrualDialog(_accountId);
}