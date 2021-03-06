/*
 * HACK: PuttyTray / Nutty
 * Hyperlink stuff: CORE FILE! Don't forget to COPY IT TO THE NEXT VERSION
 */
#include "urlhack.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "putty.h"
#include <shellapi.h>
#include <windows.h>
#ifdef __cplusplus
}
#endif
#include <vector>
#include <sstream>

extern int urlhack_mouse_old_x = -1, urlhack_mouse_old_y = -1, urlhack_current_region = -1;

static std::vector<text_region> link_regions;
static std::wstring browser_app;

extern const char* urlhack_default_regex =
    "("
	"(((https?|ftp):\\/\\/)|www\\.)"
	"("
	    "([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)" // 127.0.0.1
	    "|("
		"([a-zA-Z0-9\\-]+\\.)*[a-zA-Z0-9\\-]+\\." // ab-c.de-f.qrs.tuv.
		// popular tlds, and anything that could be a country
		"(aero|asia|biz|cat|com|coop|kim|info|int|jobs"
		    "|mobi|museum|name|net|org|post|pro|tel"
		    "|travel|xxx|edu|gov|mil|[a-zA-Z][a-zA-Z])"
	    ")"
	    "|([a-z]+[0-9]*)" // http://foo
	")"
	"(:[0-9]+)?" // :8080
	"((\\/|\\?)[^ \"]*[^ ,;\\.:\">)])?"
    ")"
    "|(spotify:[^ ]+:[^ ]+)"
    ;

void urlhack_clear_link_regions()
{
    link_regions.clear();
}



int urlhack_is_in_link_region(int x, int y)
{
    std::vector<text_region>::iterator i = link_regions.begin();
    int loopCounter = 1;

    while (i != link_regions.end()) {
	text_region r = *i;

	if ((r.y0 == r.y1 && y == r.y0 && y == r.y1 && x >= r.x0 && x < r.x1) ||
	    (r.y0 != r.y1 && ((y == r.y0 && x >= r.x0) || (y == r.y1 && x < r.x1) || (y > r.y0 && y < r.y1))))
	    return loopCounter; //Changed: was return true

	loopCounter++;
	i++;
    }

    return false;
}



int urlhack_is_in_this_link_region(text_region r, int x, int y)
{
    if ((r.y0 == r.y1 && y == r.y0 && y == r.y1 && x >= r.x0 && x < r.x1) || (r.y0 != r.y1 && ((y == r.y0 && x >= r.x0) || (y == r.y1 && x < r.x1) || (y > r.y0 && y < r.y1)))) {
	return true;
    }

    return false;
}



text_region urlhack_get_link_bounds(int x, int y)
{
    std::vector<text_region>::iterator i = link_regions.begin();

    while (i != link_regions.end()) {
	text_region r = *i;

	if ((r.y0 == r.y1 && y == r.y0 && y == r.y1 && x >= r.x0 && x < r.x1) ||
	    (r.y0 != r.y1 && ((y == r.y0 && x >= r.x0) || (y == r.y1 && x < r.x1) || (y > r.y0 && y < r.y1))))
	    return *i;

	i++;
    }

    text_region region;
    region.x0 = region.y0 = region.x1 = region.y1 = -1;
    return region;
}



text_region urlhack_get_link_region(int index)
{
    text_region region;

    if (index < 0 || index >= (int)link_regions.size()) {
	region.x0 = region.y0 = region.x1 = region.y1 = -1;
	return region;
    }
    else {
	return link_regions.at(index);
    }
}



void urlhack_add_link_region(int x0, int y0, int x1, int y1)
{
    text_region region;

    region.x0 = x0;
    region.y0 = y0;
    region.x1 = x1;
    region.y1 = y1;

    link_regions.insert(link_regions.end(), region);
}



void urlhack_launch_url(const wchar_t *app, const wchar_t *url)
{
    if (app) {
	ShellExecuteW(NULL, NULL, app, url, NULL, SW_SHOW);
	return;
    }

    if (browser_app.size() == 0) {
	// Find out the default app
	HKEY key;
	DWORD dwValue;
	wchar_t *str;
	std::wstring lookup;

	if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\Shell\\Associations\\UrlAssociations\\http\\UserChoice", 0, KEY_READ, &key) == ERROR_SUCCESS) {
	    if (RegQueryValueExW(key, L"Progid", NULL, NULL, NULL, &dwValue) == ERROR_SUCCESS)
	    {
		str = new wchar_t[dwValue + 1];

		RegQueryValueExW(key, L"Progid", NULL, NULL, (BYTE*)str, &dwValue);
		RegCloseKey(key);

		std::wstringstream buffer;
		buffer << str << L"\\shell\\open\\command";
		lookup = buffer.str();

		delete [] str;
	    }
	}

	if (RegOpenKeyExW(HKEY_CLASSES_ROOT, lookup.length() > 0 ? lookup.c_str() : L"HTTP\\shell\\open\\command", 0, KEY_READ, &key) == ERROR_SUCCESS) {
	    if (!RegQueryValueExW(key, NULL, NULL, NULL, NULL, &dwValue) == ERROR_SUCCESS) return;

	    str = new wchar_t[dwValue + 1];

	    RegQueryValueExW(key, NULL, NULL, NULL, (BYTE*)str, &dwValue);
	    RegCloseKey(key);

	    browser_app = str;
	    delete[] str;

	    // Drop all stuff from the path and leave only the executable and the path

	    if (browser_app.at(0) == '"') {
		browser_app.erase(0, 1);

		if (browser_app.find('"') > 0)
		    browser_app.resize(browser_app.find('"'));
	    }
	    else {
		if (browser_app.find(' ') > 0)
		    browser_app.resize(browser_app.find(' '));
	    }
	}
	else {
	    MessageBoxW(NULL, L"Could not find your default browser.", L"PuTTY Tray Error", MB_OK | MB_ICONINFORMATION);
	}
    }

    std::wstring u = url;

    if (u.find(L"http://") == std::wstring::npos && u.find(L"https://") == std::wstring::npos &&
	u.find(L"ftp://") == std::wstring::npos && u.find(L"ftps://") == std::wstring::npos) {
	if (u.find(L"ftp.") != std::wstring::npos)
	    u.insert(0, L"ftp://");
	else
	    u.insert(0, L"http://");
    }

    ShellExecuteW(NULL, NULL, browser_app.c_str(), u.c_str(), NULL, SW_SHOW);
}






int urlhack_is_ctrl_pressed()
{
    return HIWORD(GetAsyncKeyState(VK_CONTROL));
}



// Regular expression stuff

static int urlhack_disabled = 0;
static int is_regexp_compiled = 0;
static regexp* urlhack_rx;
static std::wstring text_mass;


void urlhack_reset()
{
    text_mass.clear();
}

void urlhack_putchar(wchar_t ch)
{
    text_mass.append(&ch);
}

static void rtfm(char *error)
{
    wchar_t* werror;
    std::wstring error_msg = L"The following error occured when compiling the regular expression\n" \
			     L"for the hyperlink support. Hyperlink detection is disabled during\n" \
			     L"this session (restart PuTTY Tray to try again).\n\n";

    std::wstring actual_error_msg = error_msg;

    werror = short_mb_to_wc(CP_ACP, 0, error, strlen(error));
    actual_error_msg.append(werror);
    free(werror);

    MessageBoxW(0, actual_error_msg.c_str(), L"PuTTY Tray Error", MB_OK);
}



void urlhack_set_regular_expression(const char* expression)
{
    is_regexp_compiled = 0;
    urlhack_disabled = 0;

    set_regerror_func(rtfm);
    urlhack_rx = regcomp(const_cast<char*>(expression));

    if (urlhack_rx == 0) {
	urlhack_disabled = 1;
    }

    is_regexp_compiled = 1;
}



void urlhack_go_find_me_some_hyperlinks(int screen_width)
{
    if (urlhack_disabled != 0) return;

    if (is_regexp_compiled == 0) {
	urlhack_set_regular_expression(urlhack_default_regex);
    }

    urlhack_clear_link_regions();

    // Assuming that URL is composed of only non-wide characters.
    std::string text_serialized;
    unsigned int i;
    for (i = 0; i < text_mass.length(); i++) {
	int w = mk_wcwidth(text_mass[i]);
	if (w > 1) {
	    // "Skip" wide characters to make re_lib work properly
	    text_serialized.append(" ");
	} else {
	    char t[2] = {(char)(text_mass[i] & 0xFF), '\0'};
	    text_serialized.append(t);
	}
    }
    char *stext = const_cast<char*>(text_serialized.c_str());
    char *stext_pos = stext;

    while (regexec(urlhack_rx, stext_pos) == 1) {
	char* start_pos = *urlhack_rx->startp[0] == ' ' ? urlhack_rx->startp[0] + 1: urlhack_rx->startp[0];

	int x0 = (start_pos - stext) % screen_width;
	int y0 = (start_pos - stext) / screen_width;
	int x1 = (urlhack_rx->endp[0] - stext) % screen_width;
	int y1 = (urlhack_rx->endp[0] - stext) / screen_width;

	if (x0 >= screen_width) x0 = screen_width - 1;
	if (x1 >= screen_width) x1 = screen_width - 1;

	urlhack_add_link_region(x0, y0, x1, y1);

	stext_pos = urlhack_rx->endp[0] + 1;
    }
}

// vim: ts=8 sts=4 sw=4 noet cino=\:2\=2(0u0
