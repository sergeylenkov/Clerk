﻿#include "Settings.h"

void Settings::Open(char *configName) {
	wxFileName path(wxStandardPaths::Get().GetUserDataDir(), configName);
	_fileName = path.GetFullPath();

	_selectedAccountId = -1;
	_selectedTab = 0;
	_windowWidth = 1000;
	_windowHeight = 800;
	_windowIsMaximized = false;
	_activeDisplay = 0;
	_treeMenuWidth = 300;
	_baseCurrencyId = 180;
	_convertCurrency = false;
	_loadExchangeRates = true;
	
	_selectedExchangeRates.push_back(180);
	_selectedExchangeRates.push_back(62);

	_languages.push_back({ wxLANGUAGE_ENGLISH, wxT("English") });
	_languages.push_back({ wxLANGUAGE_RUSSIAN, wxT("Русский") });

	_language = wxLocale::GetSystemLanguage();

	if (_language != wxLANGUAGE_ENGLISH && _language != wxLANGUAGE_RUSSIAN) {
		_language = wxLANGUAGE_ENGLISH;
	}

	RestoreDefaultColumns();	

	if (wxFile::Exists(_fileName)) {
		FILE *fp = fopen(_fileName.char_str(), "rb");
		char readBuffer[65536]{0};
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		Document json;
		json.ParseStream(is);

		fclose(fp);

		if (json.HasMember("SelectedAccount") && json["SelectedAccount"].IsInt()) {
			_selectedAccountId = json["SelectedAccount"].GetInt();
		}

		if (json.HasMember("SelectedTab") && json["SelectedTab"].IsInt()) {
			_selectedTab = json["SelectedTab"].GetInt();
		}

		if (json.HasMember("WindowWidth") && json["WindowWidth"].IsInt()) {
			_windowWidth = json["WindowWidth"].GetInt();
		}

		if (json.HasMember("WindowHeight") && json["WindowHeight"].IsInt()) {
			_windowHeight = json["WindowHeight"].GetInt();
		}

		if (json.HasMember("WindowIsMaximized") && json["WindowIsMaximized"].IsBool()) {
			_windowIsMaximized = json["WindowIsMaximized"].GetBool();
		}

		if (json.HasMember("ActiveDisplay") && json["ActiveDisplay"].IsInt()) {
			_activeDisplay = json["ActiveDisplay"].GetInt();
		}

		if (json.HasMember("TreeMenuWidth") && json["TreeMenuWidth"].IsInt()) {
			_treeMenuWidth = json["TreeMenuWidth"].GetInt();
		}

		if (json.HasMember("BaseCurrency") && json["BaseCurrency"].IsInt()) {
			_baseCurrencyId = json["BaseCurrency"].GetInt();
		}

		if (json.HasMember("ConvertCurrency") && json["ConvertCurrency"].IsBool()) {
			_convertCurrency = json["ConvertCurrency"].GetBool();
		}

		if (json.HasMember("LoadExchangeRates") && json["LoadExchangeRates"].IsBool()) {
			_loadExchangeRates = json["LoadExchangeRates"].GetBool();
		}

		if (json.HasMember("Language") && json["Language"].IsInt()) {
			_language = json["Language"].GetInt();
		}

		if (json.HasMember("ExpandedMenu") && json["ExpandedMenu"].IsArray()) {
			const Value &array = json["ExpandedMenu"];
			
			for (SizeType i = 0; i < array.Size(); i++) {
				_expandedMenu[array[i].GetInt()] = true;
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
				_transactionListFilterSettings.push_back(value);
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

				_transactionsListColumnsSettings[columnsType] = columns;
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

				_reportFilterSettings.push_back(value);
			}
		}

	}
}

void Settings::Save() {
	Document json;

	json.SetObject();

	json.AddMember("WindowWidth", _windowWidth, json.GetAllocator());
	json.AddMember("WindowHeight", _windowHeight, json.GetAllocator());
	json.AddMember("WindowIsMaximized", _windowIsMaximized, json.GetAllocator());	
	json.AddMember("ActiveDisplay", _activeDisplay, json.GetAllocator());
	json.AddMember("TreeMenuWidth", _treeMenuWidth, json.GetAllocator());
	json.AddMember("SelectedAccount", _selectedAccountId, json.GetAllocator());
	json.AddMember("SelectedTab", _selectedTab, json.GetAllocator());	
	json.AddMember("BaseCurrency", _baseCurrencyId, json.GetAllocator());
	json.AddMember("ConvertCurrency", _convertCurrency, json.GetAllocator());
	json.AddMember("LoadExchangeRates", _loadExchangeRates, json.GetAllocator());
	json.AddMember("Language", _language, json.GetAllocator());

	Value menuJson(kArrayType);

	for (const auto &value : _expandedMenu)
	{
		if (value.second) {
			menuJson.PushBack(value.first, json.GetAllocator());
		}
	}

	json.AddMember("ExpandedMenu", menuJson, json.GetAllocator());

	if (_tabs.size() > 0) {
		Value tabsJson(kArrayType);

		for (auto& tab : _tabs) {
			Value tabJson(kObjectType);

			tabJson.AddMember("Type", tab.type, json.GetAllocator());
			tabJson.AddMember("Id", tab.id, json.GetAllocator());

			tabsJson.PushBack(tabJson, json.GetAllocator());
		}

		json.AddMember("Tabs", tabsJson, json.GetAllocator());
	}

	if (_transactionListFilterSettings.size() > 0) {
		Value settingsJson(kArrayType);

		for (auto& settings : _transactionListFilterSettings)
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

	for (const auto &value : _transactionsListColumnsSettings)
	{
		auto& columns = _transactionsListColumnsSettings[value.first];

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

	if (_reportFilterSettings.size() > 0) {
		Value settingsJson(kArrayType);

		for (auto& settings : _reportFilterSettings)
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

	FILE *fp = fopen(_fileName.char_str(), "wb"); 
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

	_transactionsListColumnsSettings[static_cast<int>(TransactionsListType::All)] = columns;

	std::vector<ListColumnsSettings> columns2;

	columns2.push_back({ 0, 0, wxT("To Account"), 100, false });	
	columns2.push_back({ 1, 1, wxT("Tags"), 100, false });
	columns2.push_back({ 2, 2, wxT("Note"), 100, false });
	columns2.push_back({ 3, 3, wxT("Date"), 100, true });
	columns2.push_back({ 4, 4, wxT("Amount"), 100, false });

	_transactionsListColumnsSettings[static_cast<int>(TransactionsListType::Receipts)] = columns2;

	std::vector<ListColumnsSettings> columns3;

	columns3.push_back({ 0, 0, wxT("From Account"), 100, false });
	columns3.push_back({ 1, 1, wxT("Tags"), 100, false });
	columns3.push_back({ 2, 2, wxT("Note"), 100, false });
	columns3.push_back({ 3, 3, wxT("Date"), 100, true });
	columns3.push_back({ 4, 4, wxT("Amount"), 100, false });

	_transactionsListColumnsSettings[static_cast<int>(TransactionsListType::Expenses)] = columns3;

	std::vector<ListColumnsSettings> columns4;

	columns4.push_back({ 0, 0, wxT("From Account"), 100, false });
	columns4.push_back({ 1, 1, wxT("To Account"), 100, false });
	columns4.push_back({ 2, 2, wxT("Tags"), 100, false });
	columns4.push_back({ 3, 3, wxT("Note"), 100, false });
	columns4.push_back({ 4, 4, wxT("Date"), 100, true });
	columns4.push_back({ 5, 5, wxT("Amount"), 100, false });

	_transactionsListColumnsSettings[static_cast<int>(TransactionsListType::Deposits)] = columns4;
}

int Settings::GetSelectedAccountId() {
	return _selectedAccountId;
}

void Settings::SetSelectedAccountId(int id) {
	_selectedAccountId = id;
}

int Settings::GetWindowWidth() {
	return _windowWidth;
}

int Settings::GetWindowHeight() {
	return _windowHeight;
}

void Settings::SetWindowWidth(int width) {
	_windowWidth = width;
}

void Settings::SetWindowHeight(int height) {
	_windowHeight = height;
}

bool Settings::GetWindowIsMaximized() {
	return _windowIsMaximized;
}

void Settings::SetWindowIsMaximized(bool maximized) {
	_windowIsMaximized = maximized;
}

int Settings::GetActiveDisplay() {
	return _activeDisplay;
}

void Settings::SetActiveDisplay(int index) {
	_activeDisplay = index;
}

int Settings::GetTreeMenuWidth() {
	return _treeMenuWidth;
}

void Settings::SetTreeMenuWidth(int width) {
	_treeMenuWidth = width;
}

int Settings::GetBaseCurrencyId() {
	return _baseCurrencyId;
}

void Settings::SetBaseCurrencyId(int id) {
	_baseCurrencyId = id;
}

bool Settings::IsConvertCurrency() {
	return _convertCurrency;
}

void Settings::SetConvertCurrency(bool convert) {
	_convertCurrency = convert;
}

bool Settings::IsLoadExchangeRates() {
	return _loadExchangeRates;
}

void Settings::SetLoadExchangeRates(bool load) {
	_loadExchangeRates = load;
}

std::vector<int> Settings::GetSelectedExchangeRates() {
	return _selectedExchangeRates;
}

void Settings::ClearTabs() {
	_tabs.clear();
}

void Settings::AddTab(int type, int id) {
	TabSettings tab = { type, id };
	_tabs.push_back(tab);
}

std::vector<TabSettings> Settings::GetTabs() {
	return _tabs;
}

void Settings::SetSelectedTab(int id) {
	_selectedTab = id;
}

int Settings::GetSelectedTab() {
	return _selectedTab;
}

void Settings::AddExpandedMenu(int type) {
	_expandedMenu[type] = true;
}

void Settings::RemoveExpandedMenu(int type) {
	_expandedMenu[type] = false;
}

bool Settings::IsMenuExpanded(int type) {
	if (_expandedMenu.find(type) != _expandedMenu.end()) {
		return _expandedMenu[type];
	}

	return false;
}

void Settings::SetListFilterSettings(int type, int id, int period, wxDateTime fromDate, wxDateTime toDate) {
	bool found = false;

	for (auto &settings : _transactionListFilterSettings)
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
		_transactionListFilterSettings.push_back({ type, id, period, fromDate, toDate });
	}
}

ListFilterSettings Settings::GetListFilterSettings(int type, int id) {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	Periods::Calculate(Periods::Type::CurrentWeek, fromDate, toDate);

	ListFilterSettings result = { 0, 0, 0, fromDate, toDate };

	for (auto &settings : _transactionListFilterSettings)
	{
		if (settings.type == type && settings.id == id) {
			return settings;
		}
	}

	return result;
}

std::vector<ListColumnsSettings> Settings::GetTransactionsListColumns(TransactionsListType type) {
	auto& columns = _transactionsListColumnsSettings[static_cast<int>(type)];

	std::sort(columns.begin(), columns.end(), [this](const ListColumnsSettings& v1, const ListColumnsSettings& v2) {
		return v1.order < v2.order;
	});

	return columns;
}

void Settings::SetTransactionsListColumns(TransactionsListType type, std::vector<ListColumnsSettings> columns) {
	_transactionsListColumnsSettings[static_cast<int>(type)] = columns;
}

ReportFilterSettings Settings::GetReportFilterSettings(int id) {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.Subtract(wxDateSpan::wxDateSpan(0, 3, 0, 0));	
	fromDate.SetDay(1);

	ReportFilterSettings result = { 0, "-1", 0, fromDate, toDate, false };

	for (auto &settings : _reportFilterSettings)
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

	for (auto &settings : _reportFilterSettings)
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
		_reportFilterSettings.push_back({ id, accountIds, period, fromDate, toDate, average });
	}
}

std::vector<Language> Settings::GetLanguages() {
	return _languages;
}

int Settings::GetLanguage() {
	return _language;
}

void Settings::SetLanguage(int id) {
	_language = id;
}