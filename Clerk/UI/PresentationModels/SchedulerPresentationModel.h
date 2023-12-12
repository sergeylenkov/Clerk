#pragma once

#include <wx/wx.h>
#include "../../Data/Models/SchedulerModel.h"
#include "../../Utils/Types.h"
#include "../../Utils/Utils.h"
#include "./TagPresentationModel.h"
#include "./AccountPresentationModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class SchedulerPresentationModel {
		public:
			SchedulerPresentationModel();
			SchedulerPresentationModel(SchedulerModel& scheduler);

			int id;
			wxString name;
			SchedulerType type;			
			wxDateTime previousDate;
			wxDateTime nextDate;			
			int day;
			int week;
			int month;
			bool isActive;
			std::shared_ptr<AccountPresentationModel> fromAccount;
			std::shared_ptr<AccountPresentationModel> toAccount;
			float fromAmount;
			float toAmount;
			shared_vector<TagPresentationModel> tags;
			wxDateTime created;
			wxString typeName;

			operator SchedulerModel& ();

		private:
			SchedulerModel& GetModel();
		};
	}
}
