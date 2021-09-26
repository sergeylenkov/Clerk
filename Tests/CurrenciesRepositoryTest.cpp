#include "pch.h"

#include "../Clerk/Data/Repositories/CurrenciesRepository.h"
#include "Fixture.cpp"

class CurrenciesRepositoryTest : public Fixture {
public:
    void SetUp() override {
        SetUpConnection();
        repository = new Clerk::Data::CurrenciesRepository(*connection);
    }

    void TearDown() override {
        Fixture::TearDown();
    }

protected:
    CurrenciesRepository* repository;
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