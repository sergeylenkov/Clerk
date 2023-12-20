#pragma once

#include <wx/wx.h>
#include "../../Data/Models/AlertModel.h"
#include "../../Data/Enums.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class AlertPresentationModel {
		public:
			AlertPresentationModel();
			AlertPresentationModel(AlertModel& model);

			int id;
			wxString name;
			wxString message;
			AlertType type;
			AlertPeriod period;
			AlertCondition condition;
			AlertImportance importance;
			float amount;
			float balance;
			wxDateTime created;
			wxString typeName;
			wxString periodName;
			wxString conditionName;
			wxString importanceName;
			std::vector<int> accountsIds;

			operator AlertModel& ();

		private:
			AlertModel& GetModel();
		};
	}
}