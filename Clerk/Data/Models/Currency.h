#pragma once

#include <wx/wx.h>
#include <memory>
#include "Model.h"

namespace Clerk {
	namespace Data {
		class Currency : public Model
		{
		public:
			Currency();

			std::shared_ptr<wxString> name;
			std::shared_ptr<wxString> shortName;
			std::shared_ptr<wxString> sign;
		};
	}
}