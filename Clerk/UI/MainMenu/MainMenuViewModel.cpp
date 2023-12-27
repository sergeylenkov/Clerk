#include "MainMenuViewModel.h"

using namespace Clerk::UI;

MainMenuViewModel::MainMenuViewModel(TransactionsService& transactionsService) : _transactionsService(transactionsService)
{
	_eventEmitter = new EventEmitter();

	_subscriptionId = _transactionsService.Subscribe([&]() {
		_eventEmitter->Emit();
	});
}

MainMenuViewModel::~MainMenuViewModel() {
	_transactionsService.Unsubscribe(_subscriptionId);
	delete _eventEmitter;
}

void MainMenuViewModel::OnUpdate(std::function<void()> fn) {
	_eventEmitter->Subscribe(fn);
}

shared_vector<TransactionPresentationModel> MainMenuViewModel::GetRecents() {
	return _transactionsService.GetRecents(10);
}