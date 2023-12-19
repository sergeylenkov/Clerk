#pragma once

#include <wx/wx.h>
#include "../../Data/Models/BudgetModel.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

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
			float remainAmount;
			float remainPercent;

			operator BudgetModel& ();

		private:
			BudgetModel& GetModel();
		};
	}
}
