#include "TransactionEditViewModel.h"

using namespace Clerk::Data;

TransactionEditViewModel::TransactionEditViewModel(AccountsService& accountsService, TransactionsService& transactionsService, ExchangeRatesRepository& exchangeRatesRepository):
	_accountsService(accountsService),
	_transactionsService(transactionsService),
	_exchangeRatesRepository(exchangeRatesRepository) {
	_fromAmount = 0;
	_toAmount = 0;
	_date = wxDateTime::Now();

	Update();
}

void TransactionEditViewModel::SetTransactionId(int id) {
	auto transaction = _transactionsService.GetById(id);

	if (transaction) {
		_fromAccount = transaction->fromAccount;
		_toAccount = transaction->toAccount;
		_fromAmount = 0;
		_toAmount = 0;
		_note = transaction->note;
		_tags = transaction->tags;

		Update();
	}
}

void TransactionEditViewModel::Update() {
	auto receipts = _accountsService.GetByType(AccountType::Receipt);
	auto deposits = _accountsService.GetByType(AccountType::Deposit);

	_fromAccounts.clear();
	_fromAccounts.insert(_fromAccounts.end(), receipts.begin(), receipts.end());
	_fromAccounts.insert(_fromAccounts.end(), deposits.begin(), deposits.end());
	
	if (!_fromAccount) {
		_fromAccount = _fromAccounts[0];
	}

	auto virtuals = _accountsService.GetByType(AccountType::Virtual);
	auto expenses = _accountsService.GetByType(AccountType::Expens);
	auto debts = _accountsService.GetByType(AccountType::Debt);

	_toAccounts.clear();

	std::vector<std::shared_ptr<AccountViewModel>> filtered;

	filtered.insert(filtered.end(), deposits.begin(), deposits.end());
	filtered.insert(filtered.end(), virtuals.begin(), virtuals.end());

	if (_fromAccount->type == AccountType::Deposit) {
		filtered.insert(filtered.end(), expenses.begin(), expenses.end());
		filtered.insert(filtered.end(), debts.begin(), debts.end());
	}	

	std::copy_if(filtered.begin(), filtered.end(), std::back_inserter(_toAccounts), [=](const std::shared_ptr<AccountViewModel>& account) {
		return account->id != _fromAccount->id;
	});

	if (!_toAccount) {
		_toAccount = _toAccounts[0];
	}

	if (_toAmount == 0) {
		float rate = 1;
		
		if (Settings::GetInstance().IsConvertCurrency()) {
			rate = _exchangeRatesRepository.GetExchangeRate(_fromAccount->currency->id, _toAccount->currency->id);
		}

		_toAmount = _fromAmount * rate;	
	}	

	if (OnUpdate) {
		OnUpdate();
	}
}

std::vector<std::shared_ptr<AccountViewModel>> TransactionEditViewModel::GetFromAccounts() {	
	return _fromAccounts;
}

std::vector<std::shared_ptr<AccountViewModel>> TransactionEditViewModel::GetToAccounts() {
	return _toAccounts;
}

int TransactionEditViewModel::GetFromAccountIndex() {
	for (unsigned int i = 0; i < _fromAccounts.size(); i++) {
		if (_fromAccount->id == _fromAccounts[i]->id) {
			return i;
		}
	}

	return 0;
}

void TransactionEditViewModel::SetFromAccount(int index) {
	_fromAccount = _fromAccounts[index];
	Update();
}

int TransactionEditViewModel::GetToAccountIndex() {
	for (unsigned int i = 0; i < _toAccounts.size(); i++) {
		if (_toAccount->id == _toAccounts[i]->id) {
			return i;
		}
	}

	return 0;
}

void TransactionEditViewModel::SetToAccount(int index) {
	_toAccount = _toAccounts[index];
	Update();
}

float TransactionEditViewModel::GetFromAmount() {
	return _fromAmount;
}

void TransactionEditViewModel::SetFromAmount(float amount) {
	_fromAmount = amount;
	Update();
}

float TransactionEditViewModel::GetToAmount() {
	return _toAmount;
}

void TransactionEditViewModel::SetToAmount(float amount) {
	_toAmount = amount;
}

void TransactionEditViewModel::SetNote(wxString note) {
	_note = note;
}

wxString TransactionEditViewModel::GetNote() {
	return _note;
}

void TransactionEditViewModel::SetDate(wxDateTime date) {
	_date = date;
}

wxDateTime TransactionEditViewModel::GetDate() {
	return _date;
}

std::vector<std::shared_ptr<TagViewModel>> TransactionEditViewModel::GetTags() {
	return _tags;
}

wxString TransactionEditViewModel::GetTagsString() {
	wxString result = "";

	if (_tags.size() > 0) {
		for (unsigned int i = 0; i < _tags.size() - 1; i++) {
			result.Append(_tags[i]->name);
			result.Append(", ");
		}

		result.Append(_tags[_tags.size() - 1]->name);
	}

	return result;
}

void TransactionEditViewModel::SetTagsString(wxString tags) {
	wxStringTokenizer tokenizer(tags, ",");
	std::vector<wxString> tokens;

	while (tokenizer.HasMoreTokens()) {
		wxString token = tokenizer.GetNextToken().Trim(true).Trim(false);

		if (!token.empty()) {
			auto result = std::find_if(begin(_tags), end(_tags), [token](auto tag) {
				return tag->name == token;
			});

			if (result == end(_tags)) {
				auto newTag = std::make_shared<TagViewModel>();
				newTag->name = token;

				_tags.push_back(newTag);
			}
		}
	}

	Update();
}

void TransactionEditViewModel::AddTag(std::shared_ptr<TagViewModel> tag) {
	_tags.push_back(tag);
	Update();
}

void TransactionEditViewModel::Save() {

}