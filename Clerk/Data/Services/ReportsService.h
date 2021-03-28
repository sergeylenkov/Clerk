#pragma once

#include "../Repositories/ReportsRepository.h"
#include "../ViewModels/ReportViewModel.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace Data {
		class ReportsService {
		public:
			ReportsService(ReportsRepository& reportsRepository);

			std::vector<std::shared_ptr<ReportViewModel>> GetAll();
			std::shared_ptr<ReportViewModel> GetById(int id);

		private:
			ReportsRepository& _reportsRepository;
		};
	}
}


