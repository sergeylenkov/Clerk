#include "Settings.h"

void Settings::Open(char *configName) {
	wxFileName path(wxStandardPaths::Get().GetUserDataDir(), configName);
	fileName = path.GetFullPath();

	selectedAccountId = -1;
	selectedTab = 0;
	windowWidth = 1000;
	windowHeight = 800;
	windowIsMaximized = false;
	activeDisplay = 0;
	baseCurrencyId = 180;
	convertCurrency = false;
	loadExchangeRates = true;

	selectedExchangeRates.push_back(180);
	selectedExchangeRates.push_back(62);

	RestoreDefaultColumns();	

	if (wxFile::Exists(fileName)) {
		FILE *fp = fopen(fileName.char_str(), "rb");
		char readBuffer[65536]{0};
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		Document json;
		json.ParseStream(is);

		fclose(fp);

		if (json.HasMember("SelectedAccount") && json["SelectedAccount"].IsInt()) {
			selectedAccountId = json["SelectedAccount"].GetInt();
		}

		if (json.HasMember("SelectedTab") && json["SelectedTab"].IsInt()) {
			selectedTab = json["SelectedTab"].GetInt();
		}

		if (json.HasMember("WindowWidth") && json["WindowWidth"].IsInt()) {
			windowWidth = json["WindowWidth"].GetInt();
		}

		if (json.HasMember("WindowHeight") && json["WindowHeight"].IsInt()) {
			windowHeight = json["WindowHeight"].GetInt();
		}

		if (json.HasMember("WindowIsMaximized") && json["WindowIsMaximized"].IsBool()) {
			windowIsMaximized = json["WindowIsMaximized"].GetBool();
		}

		if (json.HasMember("ActiveDisplay") && json["ActiveDisplay"].IsInt()) {
			activeDisplay = json["ActiveDisplay"].GetInt();
		}

		if (json.HasMember("BaseCurrency") && json["BaseCurrency"].IsInt()) {
			baseCurrencyId = json["BaseCurrency"].GetInt();
		}

		if (json.HasMember("ConvertCurrency") && json["ConvertCurrency"].IsBool()) {
			convertCurrency = json["ConvertCurrency"].GetBool();
		}

		if (json.HasMember("LoadExchangeRates") && json["LoadExchangeRates"].IsBool()) {
			loadExchangeRates = json["LoadExchangeRates"].GetBool();
		}

		if (json.HasMember("ExpandedMenu") && json["ExpandedMenu"].IsArray()) {
			const Value &array = json["ExpandedMenu"];
			
			for (SizeType i = 0; i < array.Size(); i++) {
				expandedMenu[array[i].GetInt()] = true;
			}
		}

		if (json.HasMember("Tabs") && json["Tabs"].IsArray()) {
			const Value &values = json["Tabs"];

			for (SizeType i = 0; i < values.Size(); i++) {
				const Value &value = values[i];

				AddTab(value["Type"].GetInt(), value["Id"].GetInt());
			}
		}

		if (json.HasMember("TransactionsListFilters") && json["TransactionsListFilters"].IsArray()) {
			const Value &array = json["TransactionsListFilters"];

			for (SizeType i = 0; i < array.Size(); i++) {
				const Value &filter = array[i];

				wxDateTime fromDate = wxDateTime::Now();
				wxDateTime toDate = wxDateTime::Now();

				fromDate.ParseISODate(wxString::FromUTF8(filter["FromDate"].GetString()));
				toDate.ParseISODate(wxString::FromUTF8(filter["ToDate"].GetString()));

				int type = filter["Type"].GetInt();
				int id = filter["Id"].GetInt();
				int period = filter["Period"].GetInt();

				ListFilterSettings value = { type, id, period, fromDate, toDate };
				transactionListFilterSettings.push_back(value);
			}
		}

		if (json.HasMember("TransactionsListColumns") && json["TransactionsListColumns"].IsArray()) {
			const Value &types = json["TransactionsListColumns"];

			for (SizeType i = 0; i < types.Size(); i++) {
				const Value &type = types[i];

				int columnsType = type["Type"].GetInt();
				const Value &values = type["Columns"];

				std::vector<ListColumnsSettings> columns;

				for (SizeType i = 0; i < values.Size(); i++) {
					const Value &value = values[i];

					columns.push_back({ value["Index"].GetInt(), value["Order"].GetInt(), wxString::FromUTF8(value["Title"].GetString()), value["Width"].GetInt(), value["Sorted"].GetBool() });
				}

				transactionsListColumnsSettings[columnsType] = columns;
			}
		}

		if (json.HasMember("ReportFilters") && json["ReportFilters"].IsArray()) {
			const Value &array = json["ReportFilters"];

			for (SizeType i = 0; i < array.Size(); i++) {
				const Value &filter = array[i];

				wxDateTime fromDate = wxDateTime::Now();
				wxDateTime toDate = wxDateTime::Now();

				fromDate.ParseISODate(wxString::FromUTF8(filter["FromDate"].GetString()));
				toDate.ParseISODate(wxString::FromUTF8(filter["ToDate"].GetString()));

				int id = filter["Id"].GetInt();				
				int period = filter["Period"].GetInt();
				wxString accountIds = "-1";
				bool average = false;

				if (filter.HasMember("AccountIds")) {
					accountIds = wxString::FromUTF8(filter["AccountIds"].GetString());
				}

				if (filter.HasMember("Average")) {
					average = filter["Average"].GetBool();
				}

				ReportFilterSettings value = { id, accountIds, period, fromDate, toDate, average };

				reportFilterSettings.push_back(value);
			}
		}

	}
}

void Settings::Save() {
	Document json;

	json.SetObject();

	json.AddMember("WindowWidth", windowWidth, json.GetAllocator());
	json.AddMember("WindowHeight", windowHeight, json.GetAllocator());
	json.AddMember("WindowIsMaximized", windowIsMaximized, json.GetAllocator());	
	json.AddMember("ActiveDisplay", activeDisplay, json.GetAllocator());
	json.AddMember("SelectedAccount", selectedAccountId, json.GetAllocator());
	json.AddMember("SelectedTab", selectedTab, json.GetAllocator());	
	json.AddMember("BaseCurrency", baseCurrencyId, json.GetAllocator());
	json.AddMember("ConvertCurrency", convertCurrency, json.GetAllocator());
	json.AddMember("LoadExchangeRates", loadExchangeRates, json.GetAllocator());

	Value menuJson(kArrayType);

	for (const auto &value : expandedMenu)
	{
		if (value.second) {
			menuJson.PushBack(value.first, json.GetAllocator());
		}
	}

	json.AddMember("ExpandedMenu", menuJson, json.GetAllocator());

	if (tabs.size() > 0) {
		Value tabsJson(kArrayType);

		for (auto& tab : tabs) {
			Value tabJson(kObjectType);

			tabJson.AddMember("Type", tab.type, json.GetAllocator());
			tabJson.AddMember("Id", tab.id, json.GetAllocator());

			tabsJson.PushBack(tabJson, json.GetAllocator());
		}

		json.AddMember("Tabs", tabsJson, json.GetAllocator());
	}

	if (transactionListFilterSettings.size() > 0) {
		Value settingsJson(kArrayType);

		for (auto& settings : transactionListFilterSettings)
		{
			Value filterJson(kObjectType);

			filterJson.AddMember("Type", settings.type, json.GetAllocator());
			filterJson.AddMember("Id", settings.id, json.GetAllocator());
			filterJson.AddMember("Period", settings.period, json.GetAllocator());

			wxString dateString = settings.fromDate.FormatISODate();			
			Value string(dateString.c_str(), json.GetAllocator());

			filterJson.AddMember("FromDate", string, json.GetAllocator());

			dateString = settings.toDate.FormatISODate();			
			string.SetString(dateString.c_str(), json.GetAllocator());

			filterJson.AddMember("ToDate", string, json.GetAllocator());

			settingsJson.PushBack(filterJson, json.GetAllocator());
		}

		json.AddMember("TransactionsListFilters", settingsJson, json.GetAllocator());
	}

	Value settingsJson(kArrayType);

	for (const auto &value : transactionsListColumnsSettings)
	{
		auto& columns = transactionsListColumnsSettings[value.first];

		Value columnsTypeJson(kObjectType);
		Value columnsJson(kArrayType);

		for (auto& column : columns)
		{
			Value columnJson(kObjectType);

			Value string(column.title.c_str(), json.GetAllocator());

			columnJson.AddMember("Index", column.index, json.GetAllocator());
			columnJson.AddMember("Order", column.order, json.GetAllocator());
			columnJson.AddMember("Title", string, json.GetAllocator());
			columnJson.AddMember("Width", column.width, json.GetAllocator());
			columnJson.AddMember("Sorted", column.sorted, json.GetAllocator());

			columnsJson.PushBack(columnJson, json.GetAllocator());
		}

		columnsTypeJson.AddMember("Type", value.first, json.GetAllocator());
		columnsTypeJson.AddMember("Columns", columnsJson, json.GetAllocator());

		settingsJson.PushBack(columnsTypeJson, json.GetAllocator());
	}

	json.AddMember("TransactionsListColumns", settingsJson, json.GetAllocator());

	if (reportFilterSettings.size() > 0) {
		Value settingsJson(kArrayType);

		for (auto& settings : reportFilterSettings)
		{
			Value filterJson(kObjectType);
			
			filterJson.AddMember("Id", settings.id, json.GetAllocator());

			Value string(settings.accountIds.c_str(), json.GetAllocator());
			filterJson.AddMember("AccountIds", string, json.GetAllocator());

			filterJson.AddMember("Period", settings.period, json.GetAllocator());

			wxString dateString = settings.fromDate.FormatISODate();
			string.SetString(dateString.c_str(), json.GetAllocator());

			filterJson.AddMember("FromDate", string, json.GetAllocator());

			dateString = settings.toDate.FormatISODate();
			string.SetString(dateString.c_str(), json.GetAllocator());

			filterJson.AddMember("ToDate", string, json.GetAllocator());
			filterJson.AddMember("Average", settings.average, json.GetAllocator());

			settingsJson.PushBack(filterJson, json.GetAllocator());
		}

		json.AddMember("ReportFilters", settingsJson, json.GetAllocator());
	}

	FILE *fp = fopen(fileName.char_str(), "wb"); 
	char writeBuffer[65536]{0};

	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);

	json.Accept(writer);

	fclose(fp);
}

Settings::~Settings() {
	
}

void Settings::RestoreDefaultColumns() {
	std::vector<ListColumnsSettings> columns;

	columns.push_back({ 0, 0, wxT("From Account"), 100, false });
	columns.push_back({ 1, 1, wxT("To Account"), 100, false });
	columns.push_back({ 2, 2, wxT("Tags"), 100, false });
	columns.push_back({ 3, 3, wxT("Note"), 100, false });
	columns.push_back({ 4, 4, wxT("Date"), 100, true });
	columns.push_back({ 5, 5, wxT("Amount"), 100, false });

	transactionsListColumnsSettings[static_cast<int>(ListColumnsTypes::All)] = columns;

	std::vector<ListColumnsSettings> columns2;

	columns2.push_back({ 0, 0, wxT("To Account"), 100, false });	
	columns2.push_back({ 1, 1, wxT("Tags"), 100, false });
	columns2.push_back({ 2, 2, wxT("Note"), 100, false });
	columns2.push_back({ 3, 3, wxT("Date"), 100, true });
	columns2.push_back({ 4, 4, wxT("Amount"), 100, false });

	transactionsListColumnsSettings[static_cast<int>(ListColumnsTypes::Receipts)] = columns2;

	std::vector<ListColumnsSettings> columns3;

	columns3.push_back({ 0, 0, wxT("From Account"), 100, false });
	columns3.push_back({ 1, 1, wxT("Tags"), 100, false });
	columns3.push_back({ 2, 2, wxT("Note"), 100, false });
	columns3.push_back({ 3, 3, wxT("Date"), 100, true });
	columns3.push_back({ 4, 4, wxT("Amount"), 100, false });

	transactionsListColumnsSettings[static_cast<int>(ListColumnsTypes::Expenses)] = columns3;

	std::vector<ListColumnsSettings> columns4;

	columns4.push_back({ 0, 0, wxT("From Account"), 100, false });
	columns4.push_back({ 1, 1, wxT("To Account"), 100, false });
	columns4.push_back({ 2, 2, wxT("Tags"), 100, false });
	columns4.push_back({ 3, 3, wxT("Note"), 100, false });
	columns4.push_back({ 4, 4, wxT("Date"), 100, true });
	columns4.push_back({ 5, 5, wxT("Amount"), 100, false });

	transactionsListColumnsSettings[static_cast<int>(ListColumnsTypes::Deposits)] = columns4;
}

int Settings::GetSelectedAccountId() {
	return selectedAccountId;
}

void Settings::SetSelectedAccountId(int id) {
	selectedAccountId = id;
}

int Settings::GetWindowWidth() {
	return windowWidth;
}

int Settings::GetWindowHeight() {
	return windowHeight;
}

void Settings::SetWindowWidth(int width) {
	windowWidth = width;
}

void Settings::SetWindowHeight(int height) {
	windowHeight = height;
}

bool Settings::GetWindowIsMaximized() {
	return windowIsMaximized;
}

void Settings::SetWindowIsMaximized(bool maximized) {
	windowIsMaximized = maximized;
}

int Settings::GetActiveDisplay() {
	return activeDisplay;
}

void Settings::SetActiveDisplay(int index) {
	activeDisplay = index;
}

int Settings::GetBaseCurrencyId() {
	return baseCurrencyId;
}

void Settings::SetBaseCurrencyId(int id) {
	baseCurrencyId = id;
}

bool Settings::IsConvertCurrency() {
	return convertCurrency;
}

void Settings::SetConvertCurrency(bool convert) {
	convertCurrency = convert;
}

bool Settings::IsLoadExchangeRates() {
	return loadExchangeRates;
}

void Settings::SetLoadExchangeRates(bool load) {
	loadExchangeRates = load;
}

std::vector<int> Settings::GetSelectedExchangeRates() {
	return selectedExchangeRates;
}

void Settings::ClearTabs() {
	tabs.clear();
}

void Settings::AddTab(int type, int id) {
	TabSettings tab = { type, id };
	tabs.push_back(tab);
}

std::vector<TabSettings> Settings::GetTabs() {
	return tabs;
}

void Settings::SetSelectedTab(int id) {
	selectedTab = id;
}

int Settings::GetSelectedTab() {
	return selectedTab;
}

void Settings::AddExpandedMenu(int type) {
	expandedMenu[type] = true;
}

void Settings::RemoveExpandedMenu(int type) {
	expandedMenu[type] = false;
}

bool Settings::IsMenuExpanded(int type) {
	if (expandedMenu.find(type) != expandedMenu.end()) {
		return expandedMenu[type];
	}

	return false;
}

void Settings::SetListFilterSettings(int type, int id, int period, wxDateTime fromDate, wxDateTime toDate) {
	bool found = false;

	for (auto &settings : transactionListFilterSettings)
	{		
		if (settings.type == type && settings.id == id) {
			settings.period = period;
			settings.fromDate = fromDate;
			settings.toDate = toDate;

			found = true;
			break;
		}
	}

	if (!found) {
		transactionListFilterSettings.push_back({ type, id, period, fromDate, toDate });
	}
}

ListFilterSettings Settings::GetListFilterSettings(int type, int id) {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	Periods::Calculate(Periods::Type::CurrentWeek, fromDate, toDate);

	ListFilterSettings result = { 0, 0, 0, fromDate, toDate };

	for (auto &settings : transactionListFilterSettings)
	{
		if (settings.type == type && settings.id == id) {
			return settings;
		}
	}

	return result;
}

std::vector<ListColumnsSettings> Settings::GetTransactionsListColumns(ListColumnsTypes type) {
	auto& columns = transactionsListColumnsSettings[static_cast<int>(type)];

	std::sort(columns.begin(), columns.end(), [this](const ListColumnsSettings& v1, const ListColumnsSettings& v2) {
		return v1.order < v2.order;
	});

	return columns;
}

void Settings::SetTransactionsListColumns(ListColumnsTypes type, std::vector<ListColumnsSettings> columns) {
	transactionsListColumnsSettings[static_cast<int>(type)] = columns;
}

ReportFilterSettings Settings::GetReportFilterSettings(int id) {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.Subtract(wxDateSpan::wxDateSpan(0, 3, 0, 0));	
	fromDate.SetDay(1);

	ReportFilterSettings result = { 0, "-1", 0, fromDate, toDate, false };

	for (auto &settings : reportFilterSettings)
	{
		if (settings.id == id) {
			return settings;
		}
	}

	return result;
}

void Settings::SetReportFilterSettings(int id, wxString accountIds, int period, wxDateTime fromDate, wxDateTime toDate) {
	SetReportFilterSettings(id, accountIds, period, fromDate, toDate, false);
}

void Settings::SetReportFilterSettings(int id, wxString accountIds, int period, wxDateTime fromDate, wxDateTime toDate, bool average) {
	bool found = false;

	for (auto &settings : reportFilterSettings)
	{
		if (settings.id == id) {
			settings.accountIds = accountIds;
			settings.period = period;
			settings.fromDate = fromDate;
			settings.toDate = toDate;
			settings.average = average;

			found = true;

			break;
		}
	}

	if (!found) {
		reportFilterSettings.push_back({ id, accountIds, period, fromDate, toDate, average });
	}
}