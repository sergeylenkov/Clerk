#pragma once

#include <wx/wx.h>
#include <sstream>
#include "../../Data/Models/AlertModel.h"
#include "../../Data/Enums.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class AlertPresentationModel {
		public:
			AlertPresentationModel(AlertModel& account);

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
			bool isDismissed;

			operator AlertModel& ();

		private:
			AlertModel& GetModel();
		};
	}
}