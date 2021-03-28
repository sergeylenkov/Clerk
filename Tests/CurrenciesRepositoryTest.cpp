#include "pch.h"

#include "../Clerk/Data/Repositories/CurrenciesRepository.h"

class CurrenciesRepositoryTest : public ::testing::Test {
public:
    CurrenciesRepositoryTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        repository = new Clerk::Data::CurrenciesRepository(*connection);
    }

    ~CurrenciesRepositoryTest() {
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
    Clerk::Data::CurrenciesRepository* repository;
};

TEST_F(CurrenciesRepositoryTest, GetAll) {
    auto currencies = repository->GetAll();

    EXPECT_EQ(currencies.size(), 204);
}

TEST_F(CurrenciesRepositoryTest, GetByIdExists) {
    auto currency = repository->GetById(1);

    EXPECT_TRUE(currency != nullptr);
    EXPECT_EQ(currency->id, 1);
}

TEST_F(CurrenciesRepositoryTest, GetByIdNotExists) {
    auto currency = repository->GetById(10000);

    EXPECT_TRUE(currency == nullptr);
}