#include "TransactionsMenuViewModel.h"

using namespace Clerk::UI;

TransactionsMenuViewModel::TransactionsMenuViewModel(TransactionsService& transactionsService) : _transactionsService(transactionsService)
{
	_eventEmitter = new EventEmitter();

	_subscriptionId = _transactionsService.Subscribe([&]() {
		_eventEmitter->Emit();
	});
}

TransactionsMenuViewModel::~TransactionsMenuViewModel() {
	_transactionsService.Unsubscribe(_subscriptionId);
	delete _eventEmitter;
}

void TransactionsMenuViewModel::OnUpdate(std::function<void()> fn) {
	_eventEmitter->Subscribe(fn);
}

shared_vector<TransactionPresentationModel> TransactionsMenuViewModel::GetRecents() {
	return _transactionsService.GetRecents(10);
}