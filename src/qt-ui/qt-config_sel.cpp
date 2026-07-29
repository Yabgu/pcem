#include "ibm.h"
#include "config.h"
#include "qt-utils.h"
#include <filesystem>
#include <string>
#include "paths.h"
#include "qt-hostconfig.h"

static int active_config = -1;

extern int config_open(void *hwnd);

char *validate_config_name(const char *name) {
        char notAllowed[] = {'/', '<', '>', ':', '"', '\\', '|', '?', '*'};
        int i;

        for (i = 0; i < sizeof(notAllowed) / sizeof(char); i++) {
                char *ret = strchr(name, notAllowed[i]);
                if (ret != NULL)
                        return ret;
        }

        return NULL;
}

static void select_config(void *hdlg, char *name) {
        char s[512];
        int num, c;
        void *h;

        h = wx_getdlgitem(hdlg, WX_ID("IDC_LIST"));
        num = wx_sendmessage(h, WX_LB_GETCOUNT, 0, 0);

        for (c = 0; c < num; c++) {
                wx_sendmessage(h, WX_LB_GETTEXT, c, (LONG_PARAM)s);
                if (!strcmp(s, name)) {
                        wx_sendmessage(h, WX_LB_SETCURSEL, c, 0);
                        return;
                }
        }
}

static void config_list_update(void *hdlg) {
        char s[512];
        int num, p;
        void *h;

        std::filesystem::path glob_path = std::filesystem::path(configs_path) / "*.cfg";
        auto glob_str = glob_path.string();
        strcpy(s, glob_str.c_str());
        pclog("Dir %s\n", s);

        h = wx_getdlgitem(hdlg, WX_ID("IDC_LIST"));
        p = wx_sendmessage(h, WX_LB_GETCURSEL, 0, 0);

        wx_dlgdirlist(hdlg, s, WX_ID("IDC_LIST"), 0, 0);

        num = wx_sendmessage(h, WX_LB_GETCOUNT, 0, 0);

        if (num > 0) {
                if (p >= num)
                        p = num - 1;
                else if (p < 0)
                        p = 0;
                wx_sendmessage(h, WX_LB_SETCURSEL, p, 0);
        }
        wx_enablewindow(wx_getdlgitem(hdlg, WX_ID("IDC_RENAME")), num);
        wx_enablewindow(wx_getdlgitem(hdlg, WX_ID("IDC_DELETE")), num);
        wx_enablewindow(wx_getdlgitem(hdlg, WX_ID("IDC_CONFIG")), num);
        wx_enablewindow(wx_getdlgitem(hdlg, wxID_OK), num);
}

static int run(void *hdlg) {
        char s[512];

        int ret = wx_dlgdirselectex(hdlg, (LONG_PARAM)s, 512, WX_ID("IDC_LIST"));

        pclog("wx_dlgdirselectex returned %i %s\n", ret, s);
        if (s[0]) {
                active_config = wx_sendmessage(wx_getdlgitem(hdlg, WX_ID("IDC_LIST")), WX_LB_GETCURSEL, 0, 0);

                std::filesystem::path cfg_path = std::filesystem::path(configs_path) / (std::string(s) + "cfg");
                auto cfg_str = cfg_path.string();
                pclog("Config name %s\n", cfg_str.c_str());

                config_file_default = cfg_str;
                config_name = s;
                if (!config_name.empty() && config_name.back() == '.')
                        config_name.pop_back();

                wx_enddialog(hdlg, 1);
                //                                        pause = 0;
                return TRUE;
        }
        return FALSE;
}

static int config_selection_dlgproc(void *hdlg, int message, INT_PARAM wParam, LONG_PARAM lParam) {
        // void* h;
        int done = 0;

        switch (message) {
        case WX_INITDIALOG: {
                //                pause = 1;
                config_list_update(hdlg);

                if (active_config >= 0)
                        wx_sendmessage(wx_getdlgitem(hdlg, WX_ID("IDC_LIST")), WX_LB_SETCURSEL, active_config, 0);

                //                h = wx_getdlgitem(hdlg, IDC_LIST);
                //                wx_sendmessage(h, LB_ADDSTRING, 0, (LONG_PARAM)(LPCSTR)"AAA");
                //                wx_sendmessage(h, LB_ADDSTRING, 0, (LONG_PARAM)(LPCSTR)"BBB");
                return TRUE;
        }
        case WX_COMMAND: {
                if (wParam == wxID_OK)
                        return run(hdlg);
                else if (wParam == wxID_CANCEL) {
                        wx_enddialog(hdlg, 0);
                        //                        pause = 0;
                        return TRUE;
                } else if (wParam == WX_ID("IDC_NEW")) {
                        char name[64];
                        name[0] = 0;

                        while (!done) {
                                if (wx_textentrydialog(hdlg, "Enter name:", "New config", name, 1, 64, (LONG_PARAM)name)) {
                                        if (validate_config_name(name) != NULL) {
                                                wx_simple_messagebox("Invalid name", "The following characters cannot be in the "
                                                                                     "name: /, <, >, :, \", \\, |, ?, *");
                                        } else {

                                                strcpy(cfg, configs_path);
                                                put_backslash(cfg);
                                                strcat(cfg, name);
                                                strcat(cfg, ".cfg");

                                                pclog("Config %s\n", cfg);

                                                if (!std::filesystem::exists(cfg)) {
                                                        /* Load from the new (empty) file to reset
                                                           globals to defaults for a fresh config */
                                                        loadconfig(cfg);
                                                        if (config_open(hdlg)) {
                                                                saveconfig(cfg);

                                                                config_list_update(hdlg);
                                                                select_config(hdlg, name);
                                                        }
                                                        done = 1;
                                                } else
                                                        wx_simple_messagebox("Already exists",
                                                                             "A configuration with that name already exists.");
                                        }
                                } else
                                        done = 1;
                        }

                        return TRUE;
                } else if (wParam == WX_ID("IDC_CONFIG")) {
                        char s[512];

                        int ret = wx_dlgdirselectex(hdlg, (LONG_PARAM)s, 512, WX_ID("IDC_LIST"));

                        pclog("wx_dlgdirselectex returned %i %s\n", ret, s);
                        if (s[0]) {
                                std::string prev_cfg;

                                strcpy(cfg, configs_path);
                                put_backslash(cfg);
                                strcat(cfg, s);
                                strcat(cfg, "cfg");
                                pclog("Config name %s\n", cfg);

                                /* Save current config path so we can restore on cancel */
                                prev_cfg = config_file_default;
                                loadconfig(cfg);
                                if (config_open(hdlg)) {
                                        saveconfig(cfg);
                                } else {
                                        /* Cancelled — restore previous config state */
                                        if (!prev_cfg.empty())
                                                loadconfig(prev_cfg);
                                }
                        }

                        return TRUE;
                } else if (wParam == WX_ID("IDC_RENAME")) {
                        char name[64];
                        char old_name[64];
                        void *h;
                        int c;

                        h = wx_getdlgitem(hdlg, WX_ID("IDC_LIST"));
                        c = wx_sendmessage(h, WX_LB_GETCURSEL, 0, 0);
                        if (c >= 0) {
                                wx_sendmessage(h, WX_LB_GETTEXT, c, (LONG_PARAM)old_name);
                                strcpy(name, old_name);

                                while (!done) {
                                        if (wx_textentrydialog(hdlg, "Enter name:", "New name", name, 1, 64, (LONG_PARAM)name)) {

                                                std::filesystem::path old_path =
                                                    std::filesystem::path(configs_path) / (std::string(old_name) + ".cfg");
                                                std::filesystem::path new_path =
                                                    std::filesystem::path(configs_path) / (std::string(name) + ".cfg");
                                                auto old_str = old_path.string();
                                                auto new_str = new_path.string();
                                                pclog("Rename %s to %s\n", old_str.c_str(), new_str.c_str());

                                                if (!std::filesystem::exists(new_path)) {
                                                        rename(old_str.c_str(), new_str.c_str());

                                                        config_list_update(hdlg);
                                                        done = 1;
                                                } else
                                                        wx_simple_messagebox("Already exists",
                                                                             "A configuration with that name already exists.");
                                        } else
                                                done = 1;
                                }
                        }

                        return TRUE;
                } else if (wParam == WX_ID("IDC_COPY")) {
                        char name[64];
                        char old_name[64];
                        void *h;
                        int c;

                        h = wx_getdlgitem(hdlg, WX_ID("IDC_LIST"));
                        c = wx_sendmessage(h, WX_LB_GETCURSEL, 0, 0);
                        if (c >= 0) {
                                wx_sendmessage(h, WX_LB_GETTEXT, c, (LONG_PARAM)old_name);
                                strcpy(name, old_name);

                                while (!done) {
                                        if (wx_textentrydialog(hdlg, "Enter name:", "New name", name, 1, 64, (LONG_PARAM)name)) {

                                                std::filesystem::path old_path =
                                                    std::filesystem::path(configs_path) / (std::string(old_name) + ".cfg");
                                                std::filesystem::path new_path =
                                                    std::filesystem::path(configs_path) / (std::string(name) + ".cfg");
                                                auto old_str = old_path.string();
                                                auto new_str = new_path.string();
                                                pclog("Copy %s to %s\n", old_str.c_str(), new_str.c_str());

                                                if (!std::filesystem::exists(new_path)) {
                                                        std::filesystem::copy_file(old_path, new_path, std::filesystem::copy_options::overwrite_existing);

                                                        config_list_update(hdlg);
                                                        done = 1;
                                                } else
                                                        wx_simple_messagebox("Already exists",
                                                                             "A configuration with that name already exists.");
                                        } else
                                                done = 1;
                                }
                        }

                        return TRUE;
                } else if (wParam == WX_ID("IDC_DELETE")) {
                        char name[64];
                        void *h;
                        int c;
                        h = wx_getdlgitem(hdlg, WX_ID("IDC_LIST"));
                        c = wx_sendmessage(h, WX_LB_GETCURSEL, 0, 0);
                        if (c >= 0) {
                                char s[512];
                                wx_sendmessage(h, WX_LB_GETTEXT, c, (LONG_PARAM)name);
                                sprintf(s, "Do you want to delete \"%s\"?", name);
                                if (wx_messagebox(NULL, s, "PCem", WX_MB_OKCANCEL) == WX_IDOK) {
                                        std::filesystem::path del_path =
                                            std::filesystem::path(configs_path) / (std::string(name) + ".cfg");
                                        auto del_str = del_path.string();
                                        strcpy(s, del_str.c_str());

                                        remove(s);

                                        config_list_update(hdlg);
                                }
                        }
                } else if (wParam == WX_ID("IDC_LIST")) {
                        if (lParam == WX_LBN_DBLCLK)
                                run(hdlg);
                }
#ifdef USE_NETWORKING
                else if (wParam == WX_ID("IDC_CONFIG_HOST")) {
                        hostconfig_open(hdlg);
                }
#endif
        } break;
        }
        return FALSE;
}

int config_selection_open(void *hwnd, int inited) {
        int ret;

        has_been_inited = inited;

        ret = wx_dialogbox(hwnd, "ConfigureSelectionDlg", config_selection_dlgproc);

        has_been_inited = 1;

        return ret;
}
