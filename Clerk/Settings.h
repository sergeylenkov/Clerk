#ifndef Settings_h
#define Settings_h

#include <wx/stdpaths.h>
#include <wx/fileconf.h>

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
	void SetSelectedAccountId(int id);
	void SetWindowWidth(int width);
	void SetWindowHeight(int height);

private:
	wxFileConfig *config;
	int selectedAccountId;
	int windowWidth;
	int windowHeight;
};

#endif