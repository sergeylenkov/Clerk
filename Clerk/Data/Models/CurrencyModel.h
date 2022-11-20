#pragma once

#include <wx/wx.h>
#include <memory>
#include "Model.h"

namespace Clerk {
	namespace Data {
		class CurrencyModel : public Model
		{
		public:
			CurrencyModel();

			std::string name;
			std::string shortName;
			std::string sign;
		};
	}
}