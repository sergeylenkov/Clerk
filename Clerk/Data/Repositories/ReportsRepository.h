#pragma once

#include <vector>
#include <memory>
#include <map>
#include "BaseRepository.h"
#include "../Models/ReportModel.h"

namespace Clerk {
	namespace Data {
		class ReportsRepository : BaseRepository {
		public:
			using BaseRepository::BaseRepository;

			std::vector<ReportModel*> GetAll();
			ReportModel* GetById(int id);
		};
	}
}
