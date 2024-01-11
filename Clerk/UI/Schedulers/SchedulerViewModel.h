#pragma once

#include "../../Data/Services/AccountsService.h"
#include "../../Data/Services/TagsService.h"
#include "../../Data/Services/CurrenciesService.h"
#include "../../Data/Services/SchedulersService.h"
#include "../../Utils/Settings/Settings.h"
#include "../../Utils/Types.h"
#include "./Enums.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class SchedulerViewModel {
		public:
			SchedulerViewModel(SchedulersService& schedulersService, AccountsService& accountsService, CurrenciesService& currenciesService, TagsService& tagsService);

			void SetSchedulerId(int id);
			bool IsNew();
			void SetName(wxString name);
			wxString GetName();
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
			shared_vector<TagPresentationModel> GetTags();
			void SetTags(shared_vector<TagPresentationModel> tags);
			SchedulerType GetSchedulerType();
			void SetSchedulerType(SchedulerType type);

			void Save();
			std::function<void(SchedulerViewModelField field)> OnUpdate;

		private:
			SchedulersService& _schedulersService;
			AccountsService& _accountsService;
			CurrenciesService& _currenciesService;
			TagsService& _tagsService;
			int _id;
			wxString _name;
			shared_vector<AccountPresentationModel> _fromAccounts;
			shared_vector<AccountPresentationModel> _toAccounts;
			std::shared_ptr<AccountPresentationModel> _fromAccount;
			std::shared_ptr<AccountPresentationModel> _toAccount;
			float _fromAmount = 0.0;
			float _toAmount = 0.0;
			shared_vector<TagPresentationModel> _tags;
			SchedulerType _type;

			void Update();
			void UpdateFromAccounts();
			void UpdateToAccounts();
		};
	}
}