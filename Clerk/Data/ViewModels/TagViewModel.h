#pragma once

#include <wx/wx.h>
#include "../Models/TagModel.h"

namespace Clerk {
	namespace Data {
		class TagViewModel {
		public:
			TagViewModel(TagModel& tag);

			int id;
			wxString name;
			int count;
		};
	}
}