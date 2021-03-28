#pragma once

#include <string>
#include "Model.h"

namespace Clerk {
	namespace Data {
		class TagModel : public Model
		{
		public:
			TagModel();

			std::string name;
		};

	}
}
