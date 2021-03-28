#pragma once

#include <wx/datetime.h>

namespace Clerk {
	namespace Data {
		struct DateValueViewModel {
			wxDateTime date;
			float value;
		};
	}
}
