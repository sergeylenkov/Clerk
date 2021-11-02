#pragma once

#include <wx/tokenzr.h>
#include "./AccountViewModel.h"
#include "../Services/AccountsService.h"
#include "../Services/TransactionsService.h"
#include "../Repositories/ExchangeRatesRepository.h"
#include "../Settings.h"
#include "./TagViewModel.h"

namespace Clerk {
	namespace Data {
		class TransactionEditViewModel {
		public:
			TransactionEditViewModel(AccountsService& accountsService, TransactionsService& transactionsService, ExchangeRatesRepository& exchangeRatesRepository);
			
			void SetTransactionId(int id);
			std::vector<std::shared_ptr<AccountViewModel>> GetFromAccounts();
			std::vector<std::shared_ptr<AccountViewModel>> GetToAccounts();
			int GetFromAccountIndex();
			void SetFromAccount(int index);
			int GetToAccountIndex();
			void SetToAccount(int index);
			float GetFromAmount();
			void SetFromAmount(float amount);
			float GetToAmount();
			void SetToAmount(float amount);
			void SetNote(wxString note);
			wxString GetNote();
			void SetDate(wxDateTime date);
			wxDateTime GetDate();
			std::vector<std::shared_ptr<TagViewModel>> GetTags();
			wxString GetTagsString();
			void SetTagsString(wxString tags);
			void AddTag(std::shared_ptr<TagViewModel> tag);

			void Save();
			std::function<void()> OnUpdate;

		private:
			AccountsService& _accountsService;
			ExchangeRatesRepository& _exchangeRatesRepository;
			TransactionsService& _transactionsService;
			int _id;
			std::vector<std::shared_ptr<AccountViewModel>> _fromAccounts;
			std::vector<std::shared_ptr<AccountViewModel>> _toAccounts;
			std::shared_ptr<AccountViewModel> _fromAccount;
			std::shared_ptr<AccountViewModel> _toAccount;
			float _fromAmount;
			float _toAmount;
			wxString _note;
			wxDateTime _date;
			std::vector<std::shared_ptr<TagViewModel>> _tags;

			void Update();
		};
	}
}