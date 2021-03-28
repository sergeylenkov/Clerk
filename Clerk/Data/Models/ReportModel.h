#pragma once

#include <string>
#include "Model.h"

namespace Clerk {
	namespace Data {
		class ReportModel : public Model
		{
		public:
			ReportModel();

			std::string name;
		};
	}
}