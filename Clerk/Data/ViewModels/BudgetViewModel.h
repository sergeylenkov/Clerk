#pragma once

#include <wx/wx.h>
#include <sstream>
#include "../Models/BudgetModel.h"

namespace Clerk {
	namespace Data {
		class BudgetViewModel {
		public:
			BudgetViewModel(BudgetModel& budget);

			int id;
			wxString name;
			float amount;
			float balance;
			BudgetPeriod period;
			wxString periodName;
			wxDateTime periodDate;
			wxDateTime date;
			std::vector<int> accountIds;
		};
	}
}
