#pragma once

#include <wx/wx.h>
#include <sstream>
#include "../Models/AlertModel.h"
#include "../Enums.h"

namespace Clerk {
	namespace Data {
		class AlertViewModel {
		public:
			AlertViewModel(AlertModel& account);

			int id;
			wxString name;
			AlertType type;
			AlertPeriod period;
			AlertCondition condition;
			float amount;
			float balance;
			wxDateTime date;
			wxString typeName;
			wxString periodName;
			wxString conditionName;
			std::vector<int> accountIds;
		};
	}
}