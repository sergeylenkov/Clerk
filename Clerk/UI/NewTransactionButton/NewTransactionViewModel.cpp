#include "NewTransactionViewModel.h"

using namespace Clerk::UI;

NewTransactionViewModel::NewTransactionViewModel(TransactionsService& transactionsService) : _transactionsService(transactionsService)
{
	_eventEmitter = new EventEmitter();

	_subscriptionId = _transactionsService.Subscribe([&]() {
		_eventEmitter->Emit();
	});
}

NewTransactionViewModel::~NewTransactionViewModel() {
	_transactionsService.Unsubscribe(_subscriptionId);
	delete _eventEmitter;
}

void NewTransactionViewModel::OnUpdate(std::function<void()> fn) {
	_eventEmitter->Subscribe(fn);
}

shared_vector<TransactionPresentationModel> NewTransactionViewModel::GetRecents() {
	return _transactionsService.GetRecents(15);
}