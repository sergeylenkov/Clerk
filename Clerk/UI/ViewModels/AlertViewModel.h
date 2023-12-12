#pragma once

#include "../PresentationModels/AlertPresentationModel.h"
#include "../PresentationModels/AccountPresentationModel.h"
#include "../../Data/Services/AlertsService.h"
#include "../../Data/Services/AccountsService.h"
#include "./Enums.h"
#include "../../Utils/Types.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class AlertViewModel {
		public:
			AlertViewModel(AlertsService& alertsService, AccountsService& accountsService);

			void SetAlertId(int id);
			bool IsNew();
			void SetName(wxString name);
			wxString GetName();
			void SetMessage(wxString message);
			wxString GetMessage();
			void SetType(AlertType type);
			AlertType GetType();
			void SetPeriod(AlertPeriod period);
			AlertPeriod GetPeriod();
			void SetCondition(AlertCondition condition);
			AlertCondition GetCondition();
			void SetImportance(AlertImportance importance);
			AlertImportance GetImportance();
			void SetAmount(float amount);
			float GetAmount();
			void SetAccountsIds(std::vector<int> ids);
			std::vector<int> GetAccountsIds();
			shared_vector<AccountPresentationModel> GetAccounts();

			void Save();
			std::function<void(AlertViewModelField field)> OnUpdate;

		private:
			AlertsService& _alertsService;
			AccountsService& _accountsService;

			int _id;
			wxString _name;
			wxString _message;
			AlertType _type;
			AlertPeriod _period;
			AlertCondition _condition;
			AlertImportance _importance;
			float _amount;
			std::vector<int> _accountsIds;
		};
	}
}