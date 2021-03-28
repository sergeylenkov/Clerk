#pragma once

#include <vector>
#include <memory>
#include <map>
#include "HashRepository.h"
#include "../Models/ReportModel.h"

namespace Clerk {
	namespace Data {
		class ReportsRepository : HashRepository<ReportModel> {
		public:
			ReportsRepository(DataConnection& connection);

			std::vector<std::shared_ptr<ReportModel>> GetAll();
			std::shared_ptr<ReportModel> GetById(int id);
		};
	}
}
