#pragma once

#include "../../Data/Services/AccountsService.h"
#include "../../Data/Services/TagsService.h"
#include "../../Data/Services/CurrenciesService.h"
#include "../../Data/Services/TransactionsService.h"
#include "../../Utils/Settings/Settings.h"
#include "../../Utils/Types.h"
#include "./Enums.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class TransactionViewModel {
		public:
			TransactionViewModel(TransactionsService& transactionsService, AccountsService& accountsService, CurrenciesService& currenciesService, TagsService& tagsService);
			~TransactionViewModel();

			void SetTransactionId(int id);
			void SetCopyTransactionId(int id);
			void SetSplitTransactionId(int id);
			void SetAccountId(int id);
			shared_vector<AccountPresentationModel> GetFromAccounts();
			shared_vector<AccountPresentationModel> GetToAccounts();
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
			shared_vector<TagPresentationModel> GetTags();			
			void SetTags(shared_vector<TagPresentationModel> tags);

			void Save();
			std::function<void(TransactionViewModelField field)> OnUpdate;

		private:
			TransactionsService& _transactionsService;
			AccountsService& _accountsService;
			CurrenciesService& _currenciesService;			
			TagsService& _tagsService;
			unsigned int _subscriptionId;
			int _splitId;
			int _id;
			shared_vector<AccountPresentationModel> _fromAccounts;
			shared_vector<AccountPresentationModel> _toAccounts;
			std::shared_ptr<AccountPresentationModel> _fromAccount;
			std::shared_ptr<AccountPresentationModel> _toAccount;
			float _fromAmount;
			float _toAmount;
			wxString _note;
			wxDateTime _date;
			shared_vector<TagPresentationModel> _tags;

			void Update();
			void UpdateFromAccounts();
			void UpdateToAccounts();
		};
	}
}