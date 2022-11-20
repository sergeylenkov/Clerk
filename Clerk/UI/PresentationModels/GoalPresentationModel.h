#pragma once

#include <wx/wx.h>
#include <sstream>
#include "../../Data/Models/GoalModel.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class GoalPresentationModel {
		public:
			GoalPresentationModel(GoalModel& goal);

			int id;
			wxString name;
			float amount;
			float balance;			
			wxDateTime created;
			wxDateTime date;
			std::vector<int> accountIds;
		};
	}
}

