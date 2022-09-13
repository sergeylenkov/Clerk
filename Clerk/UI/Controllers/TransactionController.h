#pragma once

#include <wx/wx.h>
#include "../../Data/DataContext.h"
#include "../../Utils/Icons.h"
#include "../Transactions/TransactionDialog.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class TransactionController {
		public:
			TransactionController(DataContext& context, Icons& icons);
			void SetMainWindow(wxFrame* frame);

			void New(int id);
			void Copy(int id);
			void Split(int id);
			void Edit(int id);
			void Delete(int id);

		private:
			DataContext& _context;
			Icons& _icons;
			wxFrame* _parent;

			void ShowDialog(std::shared_ptr<TransactionViewModel> transaction);
		};
	}
}
