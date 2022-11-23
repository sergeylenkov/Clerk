#include "pch.h"

#include "../Clerk/Data/Repositories/ExchangeRatesRepository.h"
#include "Fixture.cpp"

class ExchangeRatesRepositoryTest : public Fixture {
public:
    void SetUp() override {
        repository = new ExchangeRatesRepository(*connection);
    }

    void TearDown() {}

protected:
    ExchangeRatesRepository* repository;
};

TEST_F(ExchangeRatesRepositoryTest, GetRateRubToUsd) {
    auto rate = repository->GetExchangeRate(180, 152);

    EXPECT_EQ(static_cast<int>(rate), 75);
}