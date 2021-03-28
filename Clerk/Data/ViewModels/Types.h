#pragma once

#include <wx/wx.h>

namespace Clerk {
	namespace Data {
		struct DashboardProgressValue
		{
			wxString name;
			wxString amount;
			wxString currentAmount;
			wxString remainAmount;
			float percent;
		};
	}
}