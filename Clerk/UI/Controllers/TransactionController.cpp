#include "TransactionController.h"

using namespace Clerk::UI;

TransactionController::TransactionController(DataContext& context, Icons& icons) : _context(context), _icons(icons) {

}

void TransactionController::SetMainWindow(wxFrame* frame) {
	_parent = frame;
}

void TransactionController::New(int id) {
	auto account = _context.GetAccountsService().GetById(id);
	
	auto transaction = std::make_shared<TransactionViewModel>();

	if (account) {
		transaction->fromAccount = account;
		transaction->toAccount = _context.GetAccountsService().GetPairAccount(*account);
	}
	else {
		transaction->fromAccount = _context.GetAccountsService().GetLastUsedAccount();
		transaction->toAccount = _context.GetAccountsService().GetPairAccount(*transaction->fromAccount);
	}

	ShowDialog(transaction);
}

void TransactionController::Copy(int id) {
	auto transaction = _context.GetTransactionsService().GetById(id);

	if (transaction) {
		auto copy = std::make_shared<TransactionViewModel>();

		copy->fromAccount = transaction->fromAccount;
		copy->toAccount = transaction->toAccount;
		copy->fromAmount = 0;
		copy->toAmount = 0;
		copy->note = transaction->note;
		copy->tags = transaction->tags;

		ShowDialog(copy);
	}
}

void TransactionController::Split(int id) {
	auto transaction = _context.GetTransactionsService().GetById(id);

	if (transaction) {
		auto copy = std::make_shared<TransactionViewModel>();

		copy->fromAccount = transaction->fromAccount;
		copy->toAccount = transaction->toAccount;
		copy->fromAmount = transaction->fromAmount;
		copy->toAmount = 0;
		copy->note = transaction->note;
		copy->tags = transaction->tags;

		ShowDialog(copy);
	}
}

void TransactionController::Edit(int id) {
	auto transaction = _context.GetTransactionsService().GetById(id);

	if (transaction) {
		ShowDialog(transaction);
	}
}

void TransactionController::Delete(int id) {
	auto transaction = _context.GetTransactionsService().GetById(id);

	if (transaction) {
		_context.GetTransactionsService().Delete(*transaction);
	}
}

void TransactionController::ShowDialog(std::shared_ptr<TransactionViewModel> transaction) {
	TransactionDialog* transactionDialog = new TransactionDialog(_parent, wxT("Transaction"), 0, 0, 450, 350, _icons);

	transactionDialog->SetTransaction(transaction);
	transactionDialog->Show(true);
	transactionDialog->CenterOnParent();
}