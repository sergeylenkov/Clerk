#include "pch.h"

#include "../Clerk/Commands/ICommandsReceiver.h"
#include "../Clerk/Commands/CommandsInvoker.h"
#include "../Clerk/Commands/QuitCommand.h"
#include "../Clerk/Commands/PreferencesCommand.h"
#include "../Clerk/Commands/AboutCommand.h"
#include "../Clerk/Commands/NewTransactionCommand.h"
#include "../Clerk/Commands/CopyTransactionCommand.h"
#include "../Clerk/Commands/SplitTransactionCommand.h"
#include "../Clerk/Commands/EditTransactionCommand.h"
#include "../Clerk/Commands/DeleteTransactionCommand.h"
#include "../Clerk/Commands/NewAccountCommand.h"
#include "../Clerk/Commands/EditAccountCommand.h"
#include "../Clerk/Commands/NewTabCommand.h"

using namespace Clerk::Commands;
using namespace Clerk::Data;
using namespace Clerk::UI;

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

    void OpenNewTransactionDialog(int id) override {        
        testId = id;
    }

    void OpenCopyTransactionDialog(int id) override {
        testId = id;
    }

    void OpenSplitTransactionDialog(int id) override {
        testId = id;
    }

    void OpenEditTransactionDialog(int id) override {
        testId = id;
    }

    void DeleteTransaction(int id) override {
        testId = id;
    }

    void OpenNewAccountDialog(AccountType type) override {
        testAccountType = type;
    }

    void OpenEditAccountDialog(int id) override {
        testId = id;
    }

    void OpenNewTab(TabType type) override {
        testTabType = type;
    }

    int testId;
    AccountType testAccountType;
    TabType testTabType;
};

class CommandsTest : public ::testing::Test {
public:
    CommandsTest() {
        commandsReceiver = new MockCommandsReceiver();

        QuitCommand* quitCommand = new QuitCommand(commandsReceiver);
        PreferencesCommand* preferencesCommand = new PreferencesCommand(commandsReceiver);
        AboutCommand* aboutCommand = new AboutCommand(commandsReceiver);
        NewTransactionCommand* newTransactionCommand = new NewTransactionCommand(commandsReceiver);
        CopyTransactionCommand* copyTransactionCommand = new CopyTransactionCommand(commandsReceiver);
        SplitTransactionCommand* splitTransactionCommand = new SplitTransactionCommand(commandsReceiver);
        EditTransactionCommand* editTransactionCommand = new EditTransactionCommand(commandsReceiver);
        DeleteTransactionCommand* deleteTransactionCommand = new DeleteTransactionCommand(commandsReceiver);
        NewAccountCommand* newAccountCommand = new NewAccountCommand(commandsReceiver);
        EditAccountCommand* editAccountCommand = new EditAccountCommand(commandsReceiver);
        NewTabCommand* newTabCommand = new NewTabCommand(commandsReceiver);

        commandsInvoker = new CommandsInvoker(*quitCommand, *preferencesCommand, *aboutCommand,
                                              *newTransactionCommand, *copyTransactionCommand,
                                              *splitTransactionCommand, *editTransactionCommand, deleteTransactionCommand,
                                              *newAccountCommand, *editAccountCommand, *newTabCommand);
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
    commandsInvoker->OnNewTransaction(1);
    ASSERT_EQ(commandsReceiver->testId, 1);
}

TEST_F(CommandsTest, CopyTransactionCommand) {
    commandsInvoker->OnCopyTransaction(2);
    ASSERT_EQ(commandsReceiver->testId, 2);
}

TEST_F(CommandsTest, SplitTransactionCommand) {
    commandsInvoker->OnSplitTransaction(2);
    ASSERT_EQ(commandsReceiver->testId, 2);
}

TEST_F(CommandsTest, EditTransactionCommand) {
    commandsInvoker->OnEditTransaction(2);
    ASSERT_EQ(commandsReceiver->testId, 2);
}

TEST_F(CommandsTest, DeleteTransactionCommand) {
    commandsInvoker->OnDeleteTransaction(2);
    ASSERT_EQ(commandsReceiver->testId, 2);
}

TEST_F(CommandsTest, NewAccountCommand) {
    commandsInvoker->OnNewAccount(AccountType::Deposit);
    ASSERT_EQ(commandsReceiver->testAccountType, AccountType::Deposit);
}

TEST_F(CommandsTest, EditAccountCommand) {
    commandsInvoker->OnEditAccount(1);
    ASSERT_EQ(commandsReceiver->testId, 1);
}

TEST_F(CommandsTest, NewTabCommand) {
    commandsInvoker->OnNewTab(TabType::Dashboard);
    ASSERT_EQ(commandsReceiver->testTabType, TabType::Dashboard);
}