#pragma once

#include "../../Utils/Types.h"
#include "../Repositories/ReportsRepository.h"
#include "../PresentationModels/ReportPresentationModel.h"
#include "HashService.h"

namespace Clerk {
	namespace Data {
		class ReportsService : HashService<std::shared_ptr<ReportPresentationModel>> {
		public:
			ReportsService(ReportsRepository& reportsRepository);

			std::shared_ptr<ReportPresentationModel> GetById(int id);
			shared_vector<ReportPresentationModel> GetAll();			

		private:
			ReportsRepository& _reportsRepository;
		};
	}
}


