#pragma once

#include "../Repositories/ReportsRepository.h"
#include "../../UI/PresentationModels/ReportPresentationModel.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class ReportsService {
		public:
			ReportsService(ReportsRepository& reportsRepository);

			std::vector<std::shared_ptr<ReportPresentationModel>> GetAll();
			std::shared_ptr<ReportPresentationModel> GetById(int id);

		private:
			ReportsRepository& _reportsRepository;
		};
	}
}


