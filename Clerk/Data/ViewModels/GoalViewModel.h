#pragma once

#include <wx/wx.h>
#include <sstream>
#include "../Models/GoalModel.h"

namespace Clerk {
	namespace Data {
		class GoalViewModel {
		public:
			GoalViewModel(GoalModel& goal);

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

