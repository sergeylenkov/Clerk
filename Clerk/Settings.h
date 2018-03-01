#ifndef Settings_h
#define Settings_h

#include <wx/stdpaths.h>
#include <wx/fileconf.h>

struct TabSettings {
	int type;
	int id;
};

class Settings
{
public:
	~Settings();

	static Settings& GetInstance() {
		static Settings instance;
		return instance;
	}

	void Open(char *configName);
	void Save();

	int GetSelectedAccountId();
	int GetWindowWidth();
	int GetWindowHeight();
	wxDateTime Settings::GetFromPeriodDate();
	wxDateTime Settings::GetToPeriodDate();
	void SetSelectedAccountId(int id);
	void SetWindowWidth(int width);
	void SetWindowHeight(int height);
	void SetFromPeriodDate(wxDateTime date);
	void SetToPeriodDate(wxDateTime date);
	void AddTab(int type, int accountId);
	std::vector<TabSettings> GetTabs();

private:
	wxFileConfig *config;
	int selectedAccountId;
	int windowWidth;
	int windowHeight;
	wxDateTime fromPeriodDate;
	wxDateTime toPeriodDate;
	std::vector<TabSettings> tabs;
};

#endif