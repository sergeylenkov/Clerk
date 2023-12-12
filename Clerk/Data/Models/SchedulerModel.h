#pragma once

#include <string>
#include "Model.h"
#include "../Enums.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace Data {
		class SchedulerModel : public Model
		{
		public:
			SchedulerModel();
			
			std::wstring name;
			SchedulerType type;
			int day;
			int week;
			int month;
			int fromAccountId;
			int toAccountId;
			float fromAmount;
			float toAmount;
			std::wstring tagsIds;
			std::wstring previousDate;
			std::wstring nextDate;
			bool active;
			std::wstring created;
		};
	}
}