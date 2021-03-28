#pragma once

namespace Clerk {
	namespace Data {
		class Model
		{
		public:
			int id;
			bool operator== (const Model& model);
		};
	}
}