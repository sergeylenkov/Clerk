#include "pch.h"

#include "../Clerk/Data/DataConnection.h"

class TestConnection : public ::testing::Test {
public:
    TestConnection() {}
    ~TestConnection() {}
    void SetUp() {
        existingPath = std::string("Assets//Test.sqlite");
        nonExistentPath = std::string("No.sqlite");
    }
    void TearDown() {}

protected:
    std::string existingPath;
    std::string nonExistentPath;
};

TEST_F(TestConnection, DataConnectionSuccess) {
	Clerk::Data::DataConnection* connection = new Clerk::Data::DataConnection(std::move(existingPath));

	EXPECT_TRUE(connection->GetConnection() != NULL);

	connection->Close();
}

TEST_F(TestConnection, DataConnectionFail) {
    Clerk::Data::DataConnection* connection = new Clerk::Data::DataConnection(std::move(nonExistentPath));

    EXPECT_TRUE(connection->GetConnection() == NULL);
}