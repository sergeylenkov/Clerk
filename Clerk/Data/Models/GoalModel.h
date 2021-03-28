#pragma once

#include <wx/wx.h>
#include <memory>
#include "Model.h"

namespace Clerk {
	namespace Data {
		class GoalModel : public Model
		{
		public:
			GoalModel();

			std::string name;
			float amount;
			std::string date;
			std::string created;
			std::string accountIds;
		};
	}
}
