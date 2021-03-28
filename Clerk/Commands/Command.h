#pragma once

namespace Clerk {
	namespace Commands {
		class Command {
		public:
			virtual void Execute() = 0;
		};
	}
}