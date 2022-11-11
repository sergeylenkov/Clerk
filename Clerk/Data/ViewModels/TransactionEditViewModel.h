#pragma once

#include <wx/tokenzr.h>
#include "./AccountViewModel.h"
#include "../Services/AccountsService.h"
#include "../Services/TransactionsService.h"
#include "../Services/TagsService.h"
#include "../Repositories/ExchangeRatesRepository.h"
#include "../Settings.h"
#include "./TagViewModel.h"
#include "./Enums.h"

namespace Clerk {
	namespace Data {
		class TransactionEditViewModel {
		public:
			TransactionEditViewModel(AccountsService& accountsService, TransactionsService& transactionsService, ExchangeRatesRepository& exchangeRatesRepository, TagsService& tagsService);
			
			void SetTransactionId(int id);
			void SetCopyTransactionId(int id);
			void SetSplitTransactionId(int id);
			void SetAccountId(int id);
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
			std::vector<std::shared_ptr<TagViewModel>> SearchTagsByString(wxString search);

			void Save();
			std::function<void(TransactionEditViewModelField fiel)> OnUpdate;

		private:
			AccountsService& _accountsService;
			ExchangeRatesRepository& _exchangeRatesRepository;
			TransactionsService& _transactionsService;
			TagsService& _tagsService;
			int _splitId = -1;
			int _id = -1;
			std::vector<std::shared_ptr<AccountViewModel>> _fromAccounts;
			std::vector<std::shared_ptr<AccountViewModel>> _toAccounts;
			std::shared_ptr<AccountViewModel> _fromAccount;
			std::shared_ptr<AccountViewModel> _toAccount;
			float _fromAmount =- 0.0;
			float _toAmount = 0.0;
			wxString _note;
			wxDateTime _date;
			std::vector<std::shared_ptr<TagViewModel>> _tags;

			void Update();
			void UpdateFromAccounts();
			void UpdateToAccounts();
		};
	}
}