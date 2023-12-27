#pragma once

#include <wx/wx.h>
#include "../../Utils/Utils.h"
#include "../Models/GoalModel.h"

using namespace Clerk::Utils;

namespace Clerk {
	namespace Data {
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

			operator GoalModel& ();

		private:
			GoalModel& GetModel();
		};
	}
}

