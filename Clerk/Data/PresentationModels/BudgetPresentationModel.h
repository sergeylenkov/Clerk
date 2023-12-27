#pragma once

#include <wx/wx.h>
#include "../../Utils/Utils.h"
#include "../Models/BudgetModel.h"

using namespace Clerk::Utils;

namespace Clerk {
	namespace Data {
		class BudgetPresentationModel {
		public:
			BudgetPresentationModel();
			BudgetPresentationModel(BudgetModel& model);

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
			wxDateTime created;

			operator BudgetModel& ();

		private:
			BudgetModel& GetModel();
		};
	}
}
