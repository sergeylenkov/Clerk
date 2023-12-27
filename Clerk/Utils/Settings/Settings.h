#pragma once

#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <map>
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include <rapidjson/writer.h>
#include "../Utils.h"
#include "Types.h"

using namespace rapidjson;

namespace Clerk {
	namespace Utils {
		class Settings
		{
		public:
			static Settings& GetInstance() {
				static Settings instance;
				return instance;
			}

			void Open(char* configName);
			void Save();

			int GetSelectedAccountId();
			void SetSelectedAccountId(int id);
			int GetWindowWidth();
			void SetWindowWidth(int width);
			int GetWindowHeight();
			void SetWindowHeight(int height);
			bool GetWindowIsMaximized();
			void SetWindowIsMaximized(bool maximized);
			int GetActiveDisplay();
			void SetActiveDisplay(int index);
			int GetTreeMenuWidth();
			void SetTreeMenuWidth(int width);
			int GetBaseCurrencyId();
			void SetBaseCurrencyId(int id);
			bool IsConvertCurrency();
			void SetConvertCurrency(bool convert);
			bool IsLoadExchangeRates();
			void SetLoadExchangeRates(bool load);
			std::vector<int> GetSelectedExchangeRates();
			void ClearTabs();
			void AddTab(int type, int accountId);
			std::vector<TabSettings> GetTabs();
			void SetSelectedTab(int id);
			int GetSelectedTab();
			void AddExpandedMenu(int type);
			void RemoveExpandedMenu(int type);
			bool IsMenuExpanded(int type);
			std::vector<Language> GetLanguages();
			int GetLanguage();
			void SetLanguage(int id);
			void SetListFilterSettings(int type, int id, int period, wxDateTime fromDate, wxDateTime toDate);
			ListFilterSettings GetListFilterSettings(int type, int id);
			std::vector<ListColumnsSettings> GetTransactionsListColumns(TransactionsListType type);
			void SetTransactionsListColumns(TransactionsListType type, std::vector<ListColumnsSettings> columns);
			ReportFilterSettings GetReportFilterSettings(int id);
			void SetReportFilterSettings(int id, wxString accountIds, int period, wxDateTime fromDate, wxDateTime toDate);
			void SetReportFilterSettings(int id, wxString accountIds, int period, wxDateTime fromDate, wxDateTime toDate, bool average);
			std::vector<ListColumnsSettings> GetAlertsListColumns();
			void SetAlertsListColumns(std::vector<ListColumnsSettings> columns);
			std::vector<ListColumnsSettings> GetSchedulersListColumns();
			void SetSchedulersListColumns(std::vector<ListColumnsSettings> columns);
			std::vector<ListColumnsSettings> GetBudgetsListColumns();
			void SetBudgetsListColumns(std::vector<ListColumnsSettings> columns);
			std::vector<ListColumnsSettings> GetGoalsListColumns();
			void SetGoalsListColumns(std::vector<ListColumnsSettings> columns);

		private:
			wxString _fileName;
			int _selectedAccountId;
			int _windowWidth;
			int _windowHeight;
			bool _windowIsMaximized;
			int _activeDisplay;
			int _treeMenuWidth;
			int _baseCurrencyId;
			bool _convertCurrency;
			bool _loadExchangeRates;
			std::vector<int> _selectedExchangeRates;
			std::vector<TabSettings> _tabs;
			std::map<int, bool> _expandedMenu;
			int _selectedTab;
			std::vector<Language> _languages;
			int _language;
			std::vector<ListFilterSettings> _transactionListFilterSettings;
			std::map<int, std::vector<ListColumnsSettings>> _transactionsListColumnsSettings;
			std::vector<ReportFilterSettings> _reportFilterSettings;
			std::vector<ListColumnsSettings> _alertsListColumnsSettings;
			std::vector<ListColumnsSettings> _schedulersListColumnsSettings;
			std::vector<ListColumnsSettings> _budgetsListColumnsSettings;
			std::vector<ListColumnsSettings> _goalsListColumnsSettings;

			void RestoreDefaultColumns();
			std::vector<ListColumnsSettings> ReadColumnsFromJson(const Value& values);
			Value WriteColumnsToJson(Document& json, std::vector<ListColumnsSettings> columns);
		};
	}
}