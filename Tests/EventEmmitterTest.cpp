#include "pch.h"

#include "../Clerk/Utils/EventEmitter.h"

class EventEmitterTest : public ::testing::Test {
public:
    EventEmitterTest() {
        testId = 0;
        eventEmitter = new EventEmitter();
    }

    ~EventEmitterTest() {
        delete eventEmitter;
    }

protected:
    EventEmitter* eventEmitter;
    int testId;
};

TEST_F(EventEmitterTest, Emit) {
    testId = 2;

    eventEmitter->Emit();

    eventEmitter->Subscribe([&]() {
        ASSERT_EQ(testId, 2);
    });
}