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
			
			std::string name;
			SchedulerType type;
			int day;
			int week;
			int month;
			int fromAccountId;
			int toAccountId;
			float fromAmount;
			float toAmount;
			std::string tags;
			std::string previousDate;
			std::string nextDate;
			bool active;
		};
	}
}