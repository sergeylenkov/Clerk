#include "TransactionEditViewModel.h"

using namespace Clerk::UI;

TransactionEditViewModel::TransactionEditViewModel(AccountsService& accountsService, TransactionsService& transactionsService, ExchangeRatesRepository& exchangeRatesRepository, TagsService& tagsService):
	_accountsService(accountsService),
	_transactionsService(transactionsService),
	_exchangeRatesRepository(exchangeRatesRepository),
	_tagsService(tagsService) {
	_splitId = -1;
	_id = -1;
	_fromAmount = 0;
	_toAmount = 0;
	_date = wxDateTime::Now();
	
	_subscriptionId = _accountsService.Subscribe([&]() {
		Update();
	});

	Update();
}

TransactionEditViewModel::~TransactionEditViewModel() {
	_accountsService.Unsubscribe(_subscriptionId);
}

void TransactionEditViewModel::SetTransactionId(int id) {
	auto transaction = _transactionsService.GetById(id);

	if (transaction) {
		_id = transaction->id;
		_fromAccount = transaction->fromAccount;
		_toAccount = transaction->toAccount;
		_fromAmount = transaction->fromAmount;
		_toAmount = transaction->toAmount;
		_note = transaction->note;
		_tags = transaction->tags;

		Update();
	}
}
void TransactionEditViewModel::SetSplitTransactionId(int id) {
	auto transaction = _transactionsService.GetById(id);

	if (transaction) {
		_splitId = transaction->id;
		_id = -1;
		_fromAccount = transaction->fromAccount;
		_toAccount = transaction->toAccount;
		_fromAmount = transaction->fromAmount;
		_toAmount = 0;
		_note = transaction->note;
		_date = transaction->date;

		Update();
	}
}

void TransactionEditViewModel::SetCopyTransactionId(int id) {
	auto transaction = _transactionsService.GetById(id);

	if (transaction) {
		_id = -1;
		_fromAccount = transaction->fromAccount;
		_toAccount = transaction->toAccount;
		_fromAmount = 0;
		_toAmount = 0;
		_note = transaction->note;
		_tags = transaction->tags;

		Update();
	}
}

void TransactionEditViewModel::SetAccountId(int id) {
	_fromAccount = _accountsService.GetById(id);

	_id = -1;
	_fromAmount = 0;
	_toAmount = 0;
	_note = "";

	if (_fromAccount) {
		_toAccount = _accountsService.GetPairAccount(*_fromAccount);
	}
	else {
		_fromAccount = _accountsService.GetLastUsedAccount();
		_toAccount = _accountsService.GetPairAccount(*_fromAccount);
	}

	Update();
}

void TransactionEditViewModel::Update() {
	UpdateFromAccounts();

	if (!_fromAccount) {
		_fromAccount = _fromAccounts[0];
	}

	UpdateToAccounts();

	if (!_toAccount) {
		_toAccount = _toAccounts[0];
	}
}

void TransactionEditViewModel::UpdateFromAccounts() {
	auto receipts = _accountsService.GetByType(AccountType::Receipt);
	auto deposits = _accountsService.GetByType(AccountType::Deposit);

	_fromAccounts.clear();

	std::vector<std::shared_ptr<AccountPresentationModel>> filtered;

	filtered.insert(filtered.end(), receipts.begin(), receipts.end());
	filtered.insert(filtered.end(), deposits.begin(), deposits.end());

	std::copy_if(filtered.begin(), filtered.end(), std::back_inserter(_fromAccounts), [&](const std::shared_ptr<AccountPresentationModel>& account) {
		return _toAccount ? account->id != _toAccount->id : true;
	});
}

void TransactionEditViewModel::UpdateToAccounts() {
	auto deposits = _accountsService.GetByType(AccountType::Deposit);
	auto virtuals = _accountsService.GetByType(AccountType::Virtual);
	auto expenses = _accountsService.GetByType(AccountType::Expens);
	auto debts = _accountsService.GetByType(AccountType::Debt);

	_toAccounts.clear();

	std::vector<std::shared_ptr<AccountPresentationModel>> filtered;

	filtered.insert(filtered.end(), deposits.begin(), deposits.end());
	filtered.insert(filtered.end(), virtuals.begin(), virtuals.end());

	if (_fromAccount->type == AccountType::Deposit) {
		filtered.insert(filtered.end(), expenses.begin(), expenses.end());
		filtered.insert(filtered.end(), debts.begin(), debts.end());
	}

	std::copy_if(filtered.begin(), filtered.end(), std::back_inserter(_toAccounts), [&](const std::shared_ptr<AccountPresentationModel>& account) {
		return _fromAccount ? account->id != _fromAccount->id : true;
	});
}

std::vector<std::shared_ptr<AccountPresentationModel>> TransactionEditViewModel::GetFromAccounts() {
	return _fromAccounts;
}

std::vector<std::shared_ptr<AccountPresentationModel>> TransactionEditViewModel::GetToAccounts() {
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

	const auto result = std::find_if(_toAccounts.begin(), _toAccounts.end(), [&](const std::shared_ptr<AccountPresentationModel>& account) {
		return account->id == _fromAccount->id;
	});

	if (result != _toAccounts.end()) {
		UpdateToAccounts();
		OnUpdate(TransactionEditViewModelField::FromAccount);
	}
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

	const auto result = std::find_if(_fromAccounts.begin(), _fromAccounts.end(), [&](const std::shared_ptr<AccountPresentationModel>& account) {
		return account->id == _toAccount->id;
	});

	if (result != _fromAccounts.end()) {
		UpdateFromAccounts();
		OnUpdate(TransactionEditViewModelField::ToAccount);
	}
}

float TransactionEditViewModel::GetFromAmount() {
	return _fromAmount;
}

void TransactionEditViewModel::SetFromAmount(float amount) {
	_fromAmount = amount;

	if (_toAmount == 0) {
		float rate = 1;

		if (Settings::GetInstance().IsConvertCurrency()) {
			rate = _exchangeRatesRepository.GetExchangeRate(_fromAccount->currency->id, _toAccount->currency->id);
		}

		_toAmount = _fromAmount * rate;
	}

	if (_fromAmount == amount) {
		OnUpdate(TransactionEditViewModelField::FromAmount);
	}
}

float TransactionEditViewModel::GetToAmount() {
	return _toAmount;
}

void TransactionEditViewModel::SetToAmount(float amount) {
	_toAmount = amount;

	if (_toAmount == amount) {
		OnUpdate(TransactionEditViewModelField::ToAmount);
	}
}

void TransactionEditViewModel::SetNote(wxString note) {
	_note = note;

	OnUpdate(TransactionEditViewModelField::Note);
}

wxString TransactionEditViewModel::GetNote() {
	return _note;
}

void TransactionEditViewModel::SetDate(wxDateTime date) {
	_date = date;

	OnUpdate(TransactionEditViewModelField::Date);
}

wxDateTime TransactionEditViewModel::GetDate() {
	return _date;
}

std::vector<std::shared_ptr<TagPresentationModel>> TransactionEditViewModel::GetTags() {
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
				auto newTag = std::make_shared<TagPresentationModel>();
				newTag->name = token;

				_tags.push_back(newTag);
			}
		}
	}

	OnUpdate(TransactionEditViewModelField::Tags);
}

void TransactionEditViewModel::AddTag(std::shared_ptr<TagPresentationModel> tag) {
	_tags.push_back(tag);

	OnUpdate(TransactionEditViewModelField::Tags);
}

std::vector<std::shared_ptr<TagPresentationModel>> TransactionEditViewModel::SearchTagsByString(wxString search) {
	return _tagsService.GetBySearch(search);
}

void TransactionEditViewModel::Save() {
	auto transaction = _transactionsService.GetById(_id);

	if (!transaction) {
		transaction = std::make_shared<TransactionPresentationModel>();
	}

	transaction->fromAccount = _fromAccount;
	transaction->toAccount = _toAccount;
	transaction->fromAmount = _fromAmount;
	transaction->toAmount = _toAmount;
	transaction->note = _note;
	transaction->tags = _tags;
	transaction->date = _date;
	
	_transactionsService.Save(*transaction);

	auto splitTransaction = _transactionsService.GetById(_splitId);

	if (splitTransaction) {
		splitTransaction->fromAmount = splitTransaction->fromAmount - transaction->fromAmount;
		splitTransaction->toAmount = splitTransaction->toAmount - transaction->toAmount;

		_transactionsService.Split(*splitTransaction, *transaction);
	}
}