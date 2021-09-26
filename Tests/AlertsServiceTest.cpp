#include "pch.h"

#include "../Clerk/Data/Services/AlertsService.h"
#include "Fixture.cpp"

class AlertsServiceTest : public Fixture {
public:
    void SetUp() override {
        Fixture::SetUp();

        service = new AlertsService(context->GetAlertsRepository());
    }

    void TearDown() override {
        Fixture::TearDown();
        delete service;
    }

protected:
    AlertsService* service;
};

TEST_F(AlertsServiceTest, GetAll) {
    auto alerts = service->GetAll();

    EXPECT_EQ(alerts.size(), 1);
}

TEST_F(AlertsServiceTest, GetByIdExists) {
    auto alert = service->GetById(1);

    EXPECT_TRUE(alert != nullptr);
    EXPECT_EQ(alert->id, 1);
}

TEST_F(AlertsServiceTest, GetByIdNotExists) {
    auto alert = service->GetById(10000);

    EXPECT_TRUE(alert == nullptr);
}