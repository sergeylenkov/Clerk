#pragma once

#include "../PresentationModels/BudgetPresentationModel.h"
#include "../PresentationModels/AccountPresentationModel.h"
#include "../../Data/Services/BudgetsService.h"
#include "../../Data/Services/AccountsService.h"
#include "./Enums.h"
#include "../../Utils/Types.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class BudgetViewModel {
		public:
			BudgetViewModel(BudgetsService& budgetService, AccountsService& accountsService);

			void SetBudgetId(int id);
			bool IsNew();
			void SetName(wxString name);
			wxString GetName();
			void SetPeriod(BudgetPeriod period);
			BudgetPeriod GetPeriod();
			void SetDate(wxDateTime date);
			wxDateTime GetDate();
			void SetAmount(float amount);
			float GetAmount();
			void SetAccountsIds(std::vector<int> ids);
			std::vector<int> GetAccountsIds();
			shared_vector<AccountPresentationModel> GetAccounts();

			void Save();
			std::function<void(BudgetViewModelField field)> OnUpdate;

		private:
			BudgetsService& _budgetsService;
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