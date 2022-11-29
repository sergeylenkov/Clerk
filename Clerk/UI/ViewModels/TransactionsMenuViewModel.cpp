#include "TransactionsMenuViewModel.h"

using namespace Clerk::UI;

TransactionsMenuViewModel::TransactionsMenuViewModel(TransactionsService& transactionsService) : _transactionsService(transactionsService)
{
	_eventEmitter = new EventEmitter();

	_transactionsService.OnUpdate([=]() {
		_eventEmitter->Emit();
	});
}

TransactionsMenuViewModel::~TransactionsMenuViewModel() {
	delete _eventEmitter;
}

void TransactionsMenuViewModel::OnUpdate(std::function<void()> fn) {
	_eventEmitter->Subscribe(fn);
}

std::vector<std::shared_ptr<TransactionPresentationModel>> TransactionsMenuViewModel::GetRecents() {
	return _transactionsService.GetRecents(10);
}