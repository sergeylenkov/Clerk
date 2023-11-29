#pragma once

#include "../Repositories/ReportsRepository.h"
#include "../../UI/PresentationModels/ReportPresentationModel.h"
#include "../../Utils/Types.h"
#include "./HashService.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class ReportsService : HashService<std::shared_ptr<ReportPresentationModel>> {
		public:
			ReportsService(ReportsRepository& reportsRepository);

			std::shared_ptr<ReportPresentationModel> GetById(int id);
			shared_vector<ReportPresentationModel> GetAll();			

		private:
			ReportsRepository& _reportsRepository;

			boolean _isLoading;
		};
	}
}


