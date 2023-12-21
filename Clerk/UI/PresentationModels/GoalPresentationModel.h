#pragma once

#include <wx/wx.h>
#include "../../Data/Models/GoalModel.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class GoalPresentationModel {
		public:
			GoalPresentationModel();
			GoalPresentationModel(GoalModel& goal);

			int id;
			wxString name;
			float amount;
			float balance;			
			wxDateTime created;
			wxDateTime date;
			std::vector<int> accountsIds;
			float remainAmount;
			float remainPercent;
		};
	}
}

