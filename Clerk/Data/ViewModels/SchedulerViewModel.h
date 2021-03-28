#pragma once

#include <wx/wx.h>
#include "../Models/SchedulerModel.h"

namespace Clerk {
	namespace Data {
		class SchedulerViewModel {
		public:
			SchedulerViewModel(SchedulerModel& scheduler);

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
