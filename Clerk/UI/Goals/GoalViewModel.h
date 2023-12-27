#pragma once

#include "../../Data/Services/GoalsService.h"
#include "../../Data/Services/AccountsService.h"
#include "../../Utils/Types.h"
#include "./Enums.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class GoalViewModel {
		public:
			GoalViewModel(GoalsService& goalsService, AccountsService& accountsService);

			void SetGoalId(int id);
			bool IsNew();
			void SetName(wxString name);
			wxString GetName();
			void SetDate(wxDateTime date);
			wxDateTime GetDate();
			void SetAmount(float amount);
			float GetAmount();
			void SetAccountsIds(std::vector<int> ids);
			std::vector<int> GetAccountsIds();
			shared_vector<AccountPresentationModel> GetAccounts();

			void Save();
			std::function<void(GoalViewModelField field)> OnUpdate;

		private:
			GoalsService& _goalsService;
			AccountsService& _accountsService;

			int _id;
			wxString _name;
			BudgetPeriod _period;
			wxDateTime _date;
			float _amount;
			std::vector<int> _accountsIds;
		};
	}
}