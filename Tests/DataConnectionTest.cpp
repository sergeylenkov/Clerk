#include "pch.h"

#include "../Clerk/Data/DataConnection.h"

using namespace Clerk::Data;

namespace Clerk {
    class TestConnection : public testing::Test {
    public:
        TestConnection() {}
        ~TestConnection() {}
        void SetUp() {
            existingPath = std::string("..//..//Tests//Assets//Test.sqlite");
            nonExistentPath = std::string("No.sqlite");
        }
        void TearDown() {}

    protected:
        std::string existingPath;
        std::string nonExistentPath;
    };

    TEST_F(TestConnection, TestConnectionSuccess) {
        DataConnection* connection = new DataConnection(std::move(existingPath));

        EXPECT_TRUE(connection->GetConnection() != NULL);

        connection->Close();
    }

    TEST_F(TestConnection, DataConnectionFail) {
        DataConnection* connection = new DataConnection(std::move(nonExistentPath));

        EXPECT_TRUE(connection->GetConnection() == NULL);
    }
}