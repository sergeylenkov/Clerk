#pragma once

#include <string>
#include "Model.h"
#include "../Enums.h"

namespace Clerk {
	namespace Data {
		class AlertModel : public Model
		{
		public:
			AlertModel();

			std::wstring name;
			std::wstring message;
			AlertType type;
			AlertPeriod period;
			AlertCondition condition;
			AlertImportance importance;
			float amount;
			std::wstring accountIds;			
			std::wstring created;
		};
	}
}
