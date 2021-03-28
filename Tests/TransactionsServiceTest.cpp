#include "pch.h"

#include "../Clerk/Data/Repositories/TransactionsRepository.h"
#include "../Clerk/Data/Repositories/AccountsRepository.h"
#include "../Clerk/Data/Repositories/CurrenciesRepository.h"
#include "../Clerk/Data/Repositories/TagsRepository.h"
#include "../Clerk/Data/Services/AccountsService.h"
#include "../Clerk/Data/Services/TransactionsService.h"
#include "../Clerk/Data/Services/TagsService.h"

class TransactionsServiceTest : public ::testing::Test {
public:
    TransactionsServiceTest() {
        std::string path("D:\\Projects\\Clerk\\Tests\\Database.sqlite");
        connection = new Clerk::Data::DataConnection(std::move(path));

        accountsRepository = new Clerk::Data::AccountsRepository(*connection);
        currenciesRepository = new Clerk::Data::CurrenciesRepository(*connection);
        transactionsRepository = new Clerk::Data::TransactionsRepository(*connection);
        tagsRepository = new Clerk::Data::TagsRepository(*connection);

        accountsService = new Clerk::Data::AccountsService(*accountsRepository, *currenciesRepository);
        tagsService = new Clerk::Data::TagsService(*tagsRepository);

        service = new Clerk::Data::TransactionsService(*transactionsRepository, *accountsService, *tagsService);
    }

    ~TransactionsServiceTest() {
        delete connection;
        delete transactionsRepository;
        delete accountsRepository;
        delete currenciesRepository;
        delete tagsRepository;
        delete accountsService;
        delete tagsService;
        delete service;
    }

    void SetUp() {
        connection->Open();
    }

    void TearDown() {
        connection->Close();
    }

protected:
    Clerk::Data::DataConnection* connection;
    Clerk::Data::TransactionsRepository* transactionsRepository;
    Clerk::Data::TransactionsService* service;
    Clerk::Data::AccountsRepository* accountsRepository;
    Clerk::Data::AccountsService* accountsService;
    Clerk::Data::CurrenciesRepository* currenciesRepository;
    Clerk::Data::TagsRepository* tagsRepository;
    Clerk::Data::TagsService* tagsService;
};

TEST_F(TransactionsServiceTest, GetForPeriod) {
    wxDateTime fromDate = wxDateTime::Now();
    fromDate.SetYear(2021);
    fromDate.SetMonth(wxDateTime::Jan);
    fromDate.SetDay(1);

    wxDateTime toDate = wxDateTime::Now();
    toDate.SetYear(2021);
    toDate.SetMonth(wxDateTime::Jan);
    toDate.SetDay(31);

    auto transactions = service->GetForPeriod(fromDate, toDate);

    EXPECT_EQ(transactions.size(), 90);
}

TEST_F(TransactionsServiceTest, GetByIdExists) {
    auto transaction = service->GetById(14670);

    EXPECT_TRUE(transaction != nullptr);
    EXPECT_EQ(transaction->id, 14670);
}

TEST_F(TransactionsServiceTest, GetByIdNotExists) {
    auto transaction = service->GetById(100000);

    EXPECT_TRUE(transaction == nullptr);
}

TEST_F(TransactionsServiceTest, GetRecents) {
    auto transactions = service->GetRecents(10);

    EXPECT_EQ(transactions.size(), 10);
}

TEST_F(TransactionsServiceTest, GetRecentsForAccount) {
    auto model = accountsRepository->GetById(2);

    AccountViewModel* account = new AccountViewModel(*model);
    account->id = 2;

    auto transactions = service->GetRecents(*account, 10);

    EXPECT_EQ(transactions.size(), 10);

    delete account;
}

TEST_F(TransactionsServiceTest, GetDeleted) {
    auto transactions = service->GetDeleted();

    EXPECT_EQ(transactions.size(), 4);
}