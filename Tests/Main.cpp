#include "pch.h"
//#include "Environment.cpp"

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	//testing::Environment* const foo =  ::testing::AddGlobalTestEnvironment(new Environment);
	
	return RUN_ALL_TESTS();
}