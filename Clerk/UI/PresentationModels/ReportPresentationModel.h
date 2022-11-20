#pragma once

#include <wx/wx.h>
#include "../../Data/Models/ReportModel.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class ReportPresentationModel {
		public:
			ReportPresentationModel(ReportModel& report);

			int id;
			wxString name;
		};
	}
}