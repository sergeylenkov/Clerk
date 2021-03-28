#include "pch.h"

#include "../Clerk/Commands/ICommandsReceiver.h"
#include "../Clerk/Commands/CommandsInvoker.h"
#include "../Clerk/Commands/QuitCommand.h"
#include "../Clerk/Commands/PreferencesCommand.h"
#include "../Clerk/Commands/AboutCommand.h"

using namespace Clerk::Commands;

class MockCommandsReceiver : public ICommandsReceiver {
public:
    void Quit() override {
        ASSERT_TRUE(1);
    }

    void OpenPreferencesDialog() override {
        ASSERT_TRUE(1);
    }

    void OpenAboutDialog() override {
        ASSERT_TRUE(1);
    }
};

class CommandsTest : public ::testing::Test {
public:
    CommandsTest() {
        commandsReceiver = new MockCommandsReceiver();

        QuitCommand* quitCommand = new QuitCommand(commandsReceiver);
        PreferencesCommand* preferencesCommand = new PreferencesCommand(commandsReceiver);
        AboutCommand* aboutCommand = new AboutCommand(commandsReceiver);

        commandsInvoker = new CommandsInvoker(*quitCommand, *preferencesCommand, *aboutCommand);
    }

    ~CommandsTest() {
        delete commandsReceiver;
        delete commandsInvoker;
    }

    void SetUp() {
        
    }

    void TearDown() {
        
    }

protected:
    MockCommandsReceiver* commandsReceiver;
    CommandsInvoker* commandsInvoker;
};

TEST_F(CommandsTest, QuitCommand) {
    commandsInvoker->OnQuit();
}

TEST_F(CommandsTest, PreferencesCommand) {
    commandsInvoker->OnPreferences();
}

TEST_F(CommandsTest, AboutCommand) {
    commandsInvoker->OnAbout();
}