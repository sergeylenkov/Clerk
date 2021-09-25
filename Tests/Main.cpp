#include "pch.h"
#include "Environment.cpp"

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	::testing::AddGlobalTestEnvironment(&Environment::Instance());

	return RUN_ALL_TESTS();
}