#include "pch.h"

#include "../Clerk/Commands/ICommandsReceiver.h"
#include "../Clerk/Commands/CommandsInvoker.h"
#include "../Clerk/Commands/QuitCommand.h"
#include "../Clerk/Commands/PreferencesCommand.h"
#include "../Clerk/Commands/AboutCommand.h"
#include "../Clerk/Commands/NewTransactionCommand.h"

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

    void OpenTransactionDialog(int id, bool isSplit) override {
        ASSERT_EQ(id, 100);
    }
};

class CommandsTest : public ::testing::Test {
public:
    CommandsTest() {
        commandsReceiver = new MockCommandsReceiver();

        QuitCommand* quitCommand = new QuitCommand(commandsReceiver);
        PreferencesCommand* preferencesCommand = new PreferencesCommand(commandsReceiver);
        AboutCommand* aboutCommand = new AboutCommand(commandsReceiver);
        NewTransactionCommand* newTransactionCommand = new NewTransactionCommand(commandsReceiver);

        commandsInvoker = new CommandsInvoker(*quitCommand, *preferencesCommand, *aboutCommand, *newTransactionCommand);
    }

    ~CommandsTest() {
        delete commandsReceiver;
        delete commandsInvoker;
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

TEST_F(CommandsTest, NewTransactionCommand) {
    commandsInvoker->OnNewTransaction(100);
}