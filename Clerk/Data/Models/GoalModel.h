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

			std::wstring name;
			float amount;
			std::wstring date;
			std::wstring created;
			std::wstring accountIds;
		};
	}
}
