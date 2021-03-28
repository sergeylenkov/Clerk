#include "pch.h"

#include "../Clerk/Data/Repositories/ExchangeRatesRepository.h"

class ExchangeRatesRepositoryTest : public ::testing::Test {
public:
    ExchangeRatesRepositoryTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        repository = new Clerk::Data::ExchangeRatesRepository(*connection);
    }

    ~ExchangeRatesRepositoryTest() {
        delete connection;
        delete repository;
    }

    void SetUp() {
        connection->Open();
    }

    void TearDown() {
        connection->Close();
    }

protected:
    Clerk::Data::DataConnection* connection;
    Clerk::Data::ExchangeRatesRepository* repository;
};

TEST_F(ExchangeRatesRepositoryTest, GetRateRubToUsd) {
    auto rate = repository->GetExchangeRate(180, 152);

    EXPECT_EQ(static_cast<int>(rate), 75);
}