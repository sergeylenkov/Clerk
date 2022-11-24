#include "pch.h"

#include "../Clerk/Data/Services/CurrenciesService.h"
#include "Fixture.cpp"

class CurrenciesServiceTest : public Fixture {
public:
    void SetUp() override {
        service = new CurrenciesService(context->GetCurrenciesRepository());
        service->SetBaseCurrency(152);
    }

    void TearDown() override {
        delete service;
    }

protected:
    CurrenciesService* service;
};

TEST_F(CurrenciesServiceTest, GetAll) {
    auto currencies = service->GetAll();

    EXPECT_EQ(currencies.size(), 204);
}

TEST_F(CurrenciesServiceTest, GetByIdExists) {
    auto currency = service->GetById(1);

    ASSERT_TRUE(currency != nullptr);
    EXPECT_EQ(currency->id, 1);
}

TEST_F(CurrenciesServiceTest, GetByIdNotExists) {
    auto currency = service->GetById(-1);

    EXPECT_TRUE(currency == nullptr);
}

TEST_F(CurrenciesServiceTest, GetBaseCurrency) {
    auto currency = service->GetBaseCurrency();

    EXPECT_EQ(currency->id, 152);
}