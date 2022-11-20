#pragma once

#include <wx/wx.h>
#include "../../Data/Models/SchedulerModel.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class SchedulerPresentationModel {
		public:
			SchedulerPresentationModel(SchedulerModel& scheduler);

			int id;
			wxString name;
			SchedulerType type;
			wxString typeName;
			wxDateTime previousDate;
			wxDateTime nextDate;			
			int day;
			int week;
			int month;
			bool isActive;
			float fromAmount;
			float toAmount;
			float amount;
		};
	}
}
