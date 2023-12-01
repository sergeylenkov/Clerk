#pragma once

#include <wx/wx.h>
#include <sstream>
#include "../../Data/Models/BudgetModel.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class BudgetPresentationModel {
		public:
			BudgetPresentationModel(BudgetModel& budget);

			int id;
			wxString name;
			float amount;
			float balance;
			BudgetPeriod period;
			wxString periodName;
			wxDateTime periodDate;
			wxDateTime date;
			std::vector<int> accountsIds;

			operator BudgetModel& ();

			wxString GetAccountsIdsString();
		private:
			BudgetModel& GetModel();
		};
	}
}
