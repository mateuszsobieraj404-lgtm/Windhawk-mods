// ==WindhawkMod==
// @id              clipboard-tweaks
// @name            Clipboard Tweaks
// @description     A modern clipboard manager that replaces Win+V: unlimited, encrypted history with search and one-click paste
// @description:pl-PL Nowoczesny schowek zamiast Win+V: historia bez limitu, szyfrowana, z wyszukiwaniem i wklejaniem jednym kliknięciem
// @version         0.1.0
// @author          Mateusz Sobieraj
// @github          https://github.com/mateuszsobieraj404-lgtm
// @homepage        https://github.com/mateuszsobieraj404-lgtm/Windhawk-mods/tree/main/clipboard-tweaks
// @include         windhawk.exe
// @compilerOptions -ld2d1 -ldwrite -ldwmapi -lwindowscodecs -lbcrypt -lcrypt32 -lole32 -loleacc -luuid -luxtheme -lshcore -lversion -lgdi32 -lshell32
// @license         MIT
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# Clipboard Tweaks

A modern clipboard manager that replaces the Windows Win+V panel with a Fluent
2 window. It keeps everything you copy, without a limit and across restarts,
and removes items only when you tell it to.

> **Pre-release (0.1.0).** This version brings the history window. Pinned
> items, templates, item details and the settings page come in the next
> versions.

## Features

- **Replaces Win+V**: Win+V opens Clipboard Tweaks instead of the built-in
  panel. Turn the mod off in Windhawk and the built-in panel comes back.
- **History without a limit**: text, formatted text (HTML, RTF), links,
  colors, images in their original resolution and copied files. Nothing
  disappears on its own, and copying the same thing again moves it to the top
  instead of adding a duplicate.
- **Search**: click the magnifier in the title bar or press Ctrl+F to filter
  the history as you type.
- **Paste with one click**: click an item or press Enter to paste it into the
  window you were working in. Shift+Enter pastes plain text, and Ctrl+1…9
  pastes the n-th item.
- **Clear history with undo**: **Clear history** removes the history at once,
  and **Undo** brings it back within 10 seconds.
- **Pause saving**: pause saving new copies from the **…** menu or the tray
  icon.
- **Window that fits your workflow**: resize it from any edge, and keep it on
  top with the pin button. It follows the Windows light or dark mode and
  accent color, with a translucent acrylic background.
- **Private by design**: items are stored encrypted with your Windows account,
  and content that apps mark as private, such as passwords from a password
  manager, is never saved.

## How to use

| Shortcut | Action |
|---|---|
| Win+V | Open or close Clipboard Tweaks |
| Enter | Paste the selected item |
| Shift+Enter | Paste as plain text |
| Ctrl+1…9 | Paste the n-th item |
| Ctrl+F | Search |
| Esc | Close the search or the window |

## Data and privacy

The history is stored in `%LOCALAPPDATA%\Clipboard Tweaks`, encrypted with a
key protected by your Windows account, so other users of the computer can't
read it. Removing the mod doesn't delete it.

## Settings

- **Turn off Windows clipboard history** (on by default): while the mod runs,
  the built-in Windows clipboard history is turned off so that the two don't
  overlap. Its previous state comes back when the mod is turned off.

## Compatibility

- Windows 11 22H2 or newer.
- Windhawk 1.7.3 or newer.

## Known issues

- Pasting into an app that runs as administrator doesn't work, because Windows
  doesn't let a regular app send keystrokes to it. The item is still put on
  the clipboard, so you can paste it with Ctrl+V.
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- disableWindowsHistory: true
  $name: Turn off Windows clipboard history
  $name:pl-PL: Wyłącz historię schowka Windows
  $description: >-
    While the mod runs, the built-in Windows clipboard history is turned off so
    that the two don't overlap. Its previous state comes back when the mod is
    turned off.
  $description:pl-PL: >-
    Na czas działania moda wyłącza wbudowaną historię schowka Windows, żeby oba
    schowki się nie gryzły. Po wyłączeniu moda wraca jej poprzedni stan.
*/
// ==/WindhawkModSettings==

#include <windows.h>
#include <windowsx.h>

#include <bcrypt.h>
#include <d2d1.h>
#include <dpapi.h>
#include <dwmapi.h>
#include <dwrite.h>
#include <oleacc.h>
#include <shellapi.h>
#include <shellscalingapi.h>
#include <shlobj.h>
#include <uxtheme.h>
#include <wincodec.h>

#include <algorithm>
#include <cmath>
#include <deque>
#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
// Constants

// DWM window attributes, by number so that older SDK headers compile too.
constexpr DWORD kDwmUseImmersiveDarkMode = 20;
constexpr DWORD kDwmWindowCornerPreference = 33;
constexpr DWORD kDwmSystemBackdropType = 38;
constexpr DWORD kDwmCornerRound = 2;
constexpr DWORD kDwmBackdropNone = 1;
constexpr DWORD kDwmBackdropAcrylic = 3;

// Marks keyboard input sent by this mod, so that the hook ignores it.
constexpr ULONG_PTR kInputTag = 0x43547770;
// Unassigned virtual key. Sent after the swallowed Win+V, it keeps the Start
// menu closed when Win is released and lets this process take the foreground.
constexpr WORD kDummyVk = 0xE8;

enum : UINT {
    WM_APP_TOGGLE = WM_APP + 1,  // Win+V, from the hook thread.
    WM_APP_LOADED,               // lParam: LoadResult*, from the worker.
    WM_APP_CAPTURED,             // lParam: Captured*, from the worker.
    WM_APP_TRAY,                 // Tray icon callback.
    WM_APP_SAVE,                 // Coalesced index save.
    WM_APP_QUIT,                 // Mod unload.
};

enum : UINT_PTR { kTimerScroll = 1, kTimerUndo, kTimerClipboardRetry };

// Layout, in DIPs.
constexpr float kTitleHeight = 40;
constexpr float kTabsHeight = 40;
constexpr float kFooterHeight = 40;
constexpr float kRowHeight = 40;
constexpr float kRowGap = 2;
constexpr float kHeaderHeight = 30;
constexpr float kDefaultWidth = 600;
constexpr float kDefaultHeight = 580;
constexpr float kMinWidth = 360;
constexpr float kMinHeight = 320;
// 30% background transparency, the default of the future settings page.
constexpr float kBackgroundOpacity = 0.70f;
constexpr UINT kUndoMs = 10000;
constexpr int kThumbSize = 32;

// Segoe Fluent Icons / Segoe MDL2 Assets glyphs.
constexpr wchar_t kIconApp = 0xE77F;
constexpr wchar_t kIconSearch = 0xE721;
constexpr wchar_t kIconMore = 0xE712;
constexpr wchar_t kIconPin = 0xE718;
constexpr wchar_t kIconPinned = 0xE840;
constexpr wchar_t kIconClose = 0xE8BB;
constexpr wchar_t kIconCancel = 0xE711;
constexpr wchar_t kIconInfo = 0xE946;
constexpr wchar_t kIconDelete = 0xE74D;
constexpr wchar_t kIconText = 0xE8E4;
constexpr wchar_t kIconLink = 0xE71B;
constexpr wchar_t kIconCode = 0xE943;
constexpr wchar_t kIconImage = 0xEB9F;
constexpr wchar_t kIconFiles = 0xE8B7;

////////////////////////////////////////////////////////////////////////////////
// Data model

enum class ItemType : BYTE { Text, Link, Code, Color, Image, Files };

struct Item {
    UINT64 id = 0;
    UINT64 time = 0;  // FILETIME, UTC.
    ItemType type = ItemType::Text;
    std::wstring source;  // Name of the app the item was copied from.
    std::wstring text;    // CF_UNICODETEXT.
    std::string html;     // "HTML Format".
    std::string rtf;      // "Rich Text Format".
    std::wstring files;   // CF_HDROP paths, one per line.
    UINT32 imageWidth = 0;  // 0 when the item has no image.
    UINT32 imageHeight = 0;
    UINT16 thumbWidth = 0;
    UINT16 thumbHeight = 0;
    std::string thumb;  // Premultiplied BGRA pixels.
    BYTE hash[32] = {};
    // Derived, not saved.
    std::wstring preview;
    UINT32 color = 0;  // 0xRRGGBB, for ItemType::Color.
};

// Raw clipboard content, read on the UI thread and processed by the worker.
struct Capture {
    UINT64 time = 0;
    std::wstring sourcePath;
    std::wstring text;
    std::string html;
    std::string rtf;
    std::wstring files;
    std::string png;
    std::string dib;
};

struct Captured {
    Item item;
    std::string png;
};

struct LoadResult {
    std::vector<Item> items;
    std::map<std::wstring, INT64> settings;
    UINT64 nextId = 1;
};

// One line of the list: a day header or an item.
struct Row {
    int item = -1;  // Index into g_items, or -1 for a header.
    std::wstring header;
    float y = 0;
    float height = 0;
};

struct Theme {
    bool dark = false;
    bool highContrast = false;
    bool lightTaskbar = false;
    bool backdrop = false;
    D2D1_COLOR_F accent, text, text2, window, layer, layerStroke, hover,
        pressed, control, controlStroke, divider, flyout, strong, search;
    COLORREF editText, editBack, editPlaceholder;
};

enum class Hit {
    None,
    Caption,
    Search,
    Menu,
    Pin,
    Close,
    SearchBox,
    SearchClear,
    List,
    Row,
    Scrollbar,
    Info,
    Clear,
    Undo,
};

struct HitInfo {
    Hit what = Hit::None;
    int row = -1;
    bool operator==(const HitInfo&) const = default;
};

struct Layout {
    D2D1_RECT_F search, menu, pin, close;
    D2D1_RECT_F searchBox, searchClear, edit;
    D2D1_RECT_F tab, card, list, footer, info, clear, undoBar, undo, scrollbar;
};

////////////////////////////////////////////////////////////////////////////////
// Globals

std::wstring g_dataDir;
std::string g_masterKey;  // Set by the worker before WM_APP_LOADED.

HANDLE g_uiThread;
HANDLE g_uiReady;
HANDLE g_hookThread;
DWORD g_hookThreadId;
HANDLE g_workerThread;
HWINEVENTHOOK g_foregroundHook;

HWND g_hwnd;
HWND g_edit;
WNDPROC g_editProc;
HFONT g_editFont;
HBRUSH g_editBrush;
UINT g_dpi = 96;
UINT g_taskbarCreatedMsg;
bool g_placing;  // Set while the window is moved by code: see WM_DPICHANGED.
bool g_quitting;

// Graphics objects. Plain pointers on purpose: they are released by the UI
// thread when it exits, never by global destructors at process exit.
ID2D1Factory* g_d2d;
IDWriteFactory* g_dwrite;
IWICImagingFactory* g_wic;
ID2D1HwndRenderTarget* g_rt;
ID2D1SolidColorBrush* g_brush;
std::unordered_map<UINT64, ID2D1Bitmap*> g_thumbs;
const wchar_t* g_uiFont = L"Segoe UI";
const wchar_t* g_iconFont = L"Segoe Fluent Icons";
float g_tabWidth, g_clearWidth, g_undoWidth;  // Fixed labels, measured once.

struct {
    IDWriteTextFormat *title, *tab, *row, *meta, *header, *small, *smallBold,
        *key, *empty, *icon16, *icon14, *icon12;
} g_fonts;

Theme g_theme;
HICON g_trayIcon;

// State, UI thread only.
std::vector<Item> g_items;  // Newest first.
std::vector<Item> g_cleared;  // Kept while "Undo" is offered.
std::vector<Row> g_rows;
std::map<std::wstring, INT64> g_settings;
UINT64 g_nextId = 1;
bool g_loaded;
bool g_paused;
bool g_pinned;
bool g_searchOpen;
bool g_undoVisible;
size_t g_undoCount;
std::wstring g_query;
UINT64 g_selectedId;
float g_scroll;
float g_scrollTarget;
float g_contentHeight;
ULONGLONG g_scrollTick;
bool g_draggingScrollbar;
float g_dragOffset;
D2D1_POINT_2F g_mouse;
bool g_mouseInside;
HitInfo g_hot;
HitInfo g_pressed;
D2D1_SIZE_F g_sizeDip{kDefaultWidth, kDefaultHeight};
HWND g_prevForeground;
ULONGLONG g_hiddenTick;
DWORD g_ownClipboardSeq;
int g_clipboardRetries;
UINT g_cfHtml, g_cfRtf, g_cfPng, g_cfExclude, g_cfCanInclude, g_cfViewerIgnore;

// Keyboard hook state, hook thread only.
bool g_swallowV;

// Worker queue.
SRWLOCK g_workLock = SRWLOCK_INIT;
CONDITION_VARIABLE g_workCv = CONDITION_VARIABLE_INIT;
std::deque<std::function<void()>> g_work;
bool g_workQuit;
// Newest unsaved content of each data file, written by one worker job.
SRWLOCK g_saveLock = SRWLOCK_INIT;
std::map<std::wstring, std::string> g_pendingSaves;

////////////////////////////////////////////////////////////////////////////////
// Small helpers

std::string_view Bytes(const std::wstring& s) {
    return {reinterpret_cast<const char*>(s.data()), s.size() * sizeof(wchar_t)};
}

UINT64 NowFileTime() {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    return (UINT64)ft.dwHighDateTime << 32 | ft.dwLowDateTime;
}

// Approximate boot time. A different value means that the computer restarted.
INT64 BootTime() {
    return (INT64)(NowFileTime() - GetTickCount64() * 10000);
}

SYSTEMTIME LocalTime(UINT64 fileTime) {
    FILETIME utc{(DWORD)fileTime, (DWORD)(fileTime >> 32)}, local;
    SYSTEMTIME st{};
    FileTimeToLocalFileTime(&utc, &local);
    FileTimeToSystemTime(&local, &st);
    return st;
}

UINT64 LocalDay(UINT64 fileTime) {
    FILETIME utc{(DWORD)fileTime, (DWORD)(fileTime >> 32)}, local;
    FileTimeToLocalFileTime(&utc, &local);
    return ((UINT64)local.dwHighDateTime << 32 | local.dwLowDateTime) /
           864000000000ULL;
}

std::wstring FormatTime(UINT64 fileTime) {
    SYSTEMTIME st = LocalTime(fileTime);
    WCHAR buf[64];
    if (!GetTimeFormatEx(LOCALE_NAME_USER_DEFAULT, TIME_NOSECONDS, &st,
                         nullptr, buf, ARRAYSIZE(buf))) {
        return {};
    }
    return buf;
}

std::wstring FormatDay(UINT64 fileTime, UINT64 today) {
    UINT64 day = LocalDay(fileTime);
    if (day == today) {
        return L"Dzisiaj";
    }
    if (day + 1 == today) {
        return L"Wczoraj";
    }
    SYSTEMTIME st = LocalTime(fileTime), now = LocalTime(NowFileTime());
    WCHAR buf[64];
    if (!GetDateFormatEx(LOCALE_NAME_USER_DEFAULT, 0, &st,
                         st.wYear == now.wYear ? L"d MMMM" : L"d MMMM yyyy",
                         buf, ARRAYSIZE(buf), nullptr)) {
        return {};
    }
    return buf;
}

// "element", "elementy" or "elementów", as Polish counts need.
const wchar_t* ElementsWord(size_t n) {
    size_t ones = n % 10, tens = n % 100;
    if (n == 1) {
        return L"element";
    }
    if (ones >= 2 && ones <= 4 && !(tens >= 12 && tens <= 14)) {
        return L"elementy";
    }
    return L"elementów";
}

bool ContainsNoCase(const std::wstring& text, const std::wstring& word) {
    return !text.empty() &&
           FindNLSStringEx(LOCALE_NAME_USER_DEFAULT,
                           FIND_FROMSTART | LINGUISTIC_IGNORECASE |
                               LINGUISTIC_IGNOREDIACRITIC,
                           text.c_str(), (int)text.size(), word.c_str(),
                           (int)word.size(), nullptr, nullptr, nullptr,
                           0) >= 0;
}

DWORD RegDword(HKEY root, const wchar_t* key, const wchar_t* name, DWORD def) {
    DWORD value, size = sizeof(value);
    return RegGetValueW(root, key, name, RRF_RT_REG_DWORD, nullptr, &value,
                        &size) == ERROR_SUCCESS
               ? value
               : def;
}

D2D1_COLOR_F Rgba(UINT32 rgb, float alpha = 1) {
    return D2D1::ColorF(((rgb >> 16) & 0xFF) / 255.f, ((rgb >> 8) & 0xFF) / 255.f,
                        (rgb & 0xFF) / 255.f, alpha);
}

D2D1_COLOR_F FromColorRef(COLORREF c, float alpha = 1) {
    return D2D1::ColorF(GetRValue(c) / 255.f, GetGValue(c) / 255.f,
                        GetBValue(c) / 255.f, alpha);
}

COLORREF ToColorRef(D2D1_COLOR_F c) {
    return RGB((BYTE)(c.r * 255 + 0.5f), (BYTE)(c.g * 255 + 0.5f),
               (BYTE)(c.b * 255 + 0.5f));
}

int Px(float dip) {
    return (int)std::lround(dip * g_dpi / 96.f);
}

template <class T>
void SafeRelease(T*& p) {
    if (p) {
        p->Release();
        p = nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Encryption: a random master key protected with DPAPI (the Windows account),
// and AES-256-GCM for every data file.

constexpr char kFileMagic[4] = {'C', 'T', 'W', '1'};
constexpr size_t kNonceSize = 12;
constexpr size_t kTagSize = 16;
constexpr size_t kHeaderSize = sizeof(kFileMagic) + kNonceSize + kTagSize;

bool AesGcm(bool encrypt, BYTE* nonce, BYTE* tag, const BYTE* in, BYTE* out,
            ULONG size) {
    BCRYPT_KEY_HANDLE key;
    if (BCryptGenerateSymmetricKey(BCRYPT_AES_GCM_ALG_HANDLE, &key, nullptr, 0,
                                   (PUCHAR)g_masterKey.data(),
                                   (ULONG)g_masterKey.size(), 0) < 0) {
        return false;
    }
    BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO info;
    BCRYPT_INIT_AUTH_MODE_INFO(info);
    info.pbNonce = nonce;
    info.cbNonce = kNonceSize;
    info.pbTag = tag;
    info.cbTag = kTagSize;
    ULONG done = 0;
    NTSTATUS status =
        encrypt ? BCryptEncrypt(key, (PUCHAR)in, size, &info, nullptr, 0, out,
                                size, &done, 0)
                : BCryptDecrypt(key, (PUCHAR)in, size, &info, nullptr, 0, out,
                                size, &done, 0);
    BCryptDestroyKey(key);
    return status >= 0;
}

std::string Encrypt(std::string_view plain) {
    std::string out(kHeaderSize + plain.size(), '\0');
    BYTE* p = (BYTE*)out.data();
    memcpy(p, kFileMagic, sizeof(kFileMagic));
    BYTE* nonce = p + sizeof(kFileMagic);
    if (BCryptGenRandom(nullptr, nonce, kNonceSize,
                        BCRYPT_USE_SYSTEM_PREFERRED_RNG) < 0 ||
        !AesGcm(true, nonce, nonce + kNonceSize, (const BYTE*)plain.data(),
                p + kHeaderSize, (ULONG)plain.size())) {
        return {};
    }
    return out;
}

bool Decrypt(const std::string& data, std::string* plain) {
    if (data.size() < kHeaderSize ||
        memcmp(data.data(), kFileMagic, sizeof(kFileMagic)) != 0) {
        return false;
    }
    BYTE* p = (BYTE*)data.data();
    plain->resize(data.size() - kHeaderSize);
    return AesGcm(false, p + sizeof(kFileMagic),
                  p + sizeof(kFileMagic) + kNonceSize, p + kHeaderSize,
                  (BYTE*)plain->data(), (ULONG)plain->size());
}

void Sha256(std::initializer_list<std::string_view> parts, BYTE out[32]) {
    BCRYPT_HASH_HANDLE hash;
    if (BCryptCreateHash(BCRYPT_SHA256_ALG_HANDLE, &hash, nullptr, 0, nullptr,
                         0, 0) < 0) {
        return;
    }
    for (std::string_view part : parts) {
        UINT32 size = (UINT32)part.size();
        BCryptHashData(hash, (PUCHAR)&size, sizeof(size), 0);
        BCryptHashData(hash, (PUCHAR)part.data(), size, 0);
    }
    BCryptFinishHash(hash, out, 32, 0);
    BCryptDestroyHash(hash);
}

////////////////////////////////////////////////////////////////////////////////
// Files

std::wstring DataPath(const wchar_t* name) {
    return g_dataDir + L"\\" + name;
}

std::wstring ImagePath(UINT64 id) {
    return g_dataDir + L"\\images\\" + std::to_wstring(id) + L".bin";
}

bool ReadFileBytes(const std::wstring& path, std::string* out) {
    HANDLE file = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ,
                              nullptr, OPEN_EXISTING, 0, nullptr);
    if (file == INVALID_HANDLE_VALUE) {
        return false;
    }
    LARGE_INTEGER size;
    bool ok = GetFileSizeEx(file, &size) && size.QuadPart < 0x7FFFFFFF;
    if (ok) {
        out->resize((size_t)size.QuadPart);
        DWORD read = 0;
        ok = ReadFile(file, out->data(), (DWORD)out->size(), &read, nullptr) &&
             read == out->size();
    }
    CloseHandle(file);
    return ok;
}

// Writes a temporary file, then swaps it in. The previous version stays as
// "<path>.bak", so a power cut never leaves a half-written file behind.
bool WriteFileAtomic(const std::wstring& path, const std::string& data) {
    std::wstring temp = path + L".tmp";
    HANDLE file = CreateFileW(temp.c_str(), GENERIC_WRITE, 0, nullptr,
                              CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) {
        return false;
    }
    DWORD written = 0;
    bool ok = WriteFile(file, data.data(), (DWORD)data.size(), &written,
                        nullptr) &&
              written == data.size() && FlushFileBuffers(file);
    CloseHandle(file);
    if (ok) {
        ok = GetFileAttributesW(path.c_str()) != INVALID_FILE_ATTRIBUTES
                 ? ReplaceFileW(path.c_str(), temp.c_str(),
                                (path + L".bak").c_str(),
                                REPLACEFILE_IGNORE_MERGE_ERRORS, nullptr,
                                nullptr)
                 : MoveFileExW(temp.c_str(), path.c_str(),
                               MOVEFILE_WRITE_THROUGH);
    }
    if (!ok) {
        Wh_Log(L"Saving %s failed: %u", path.c_str(), GetLastError());
        DeleteFileW(temp.c_str());
    }
    return ok;
}

bool WriteEncrypted(const std::wstring& path, std::string_view plain) {
    std::string data = Encrypt(plain);
    return !data.empty() && WriteFileAtomic(path, data);
}

// Reads a data file, falling back to its backup. Returns 1 when loaded, 0 when
// neither file exists, and -1 when they exist but can't be read.
int ReadEncrypted(const std::wstring& path,
                  const std::function<bool(const std::string&)>& parse) {
    bool found = false;
    for (const std::wstring& candidate : {path, path + L".bak"}) {
        std::string data, plain;
        if (!ReadFileBytes(candidate, &data)) {
            continue;
        }
        found = true;
        if (Decrypt(data, &plain) && parse(plain)) {
            return 1;
        }
        Wh_Log(L"Can't read %s", candidate.c_str());
    }
    return found ? -1 : 0;
}

// Moves an unreadable file aside instead of overwriting it, so that nothing is
// lost even when it can't be decrypted (for example after a profile restore).
void SetAside(const std::wstring& path) {
    for (const std::wstring& candidate : {path, path + L".bak"}) {
        std::wstring target =
            candidate + L".unreadable-" + std::to_wstring(NowFileTime());
        MoveFileExW(candidate.c_str(), target.c_str(), 0);
    }
}

// Keeps unreadable data and its images together, so that a later cleanup of
// unused images never touches them.
void SetAsideData() {
    SetAside(DataPath(L"index.dat"));
    SetAside(DataPath(L"settings.dat"));
    std::wstring images = DataPath(L"images");
    MoveFileExW(images.c_str(),
                (images + L".unreadable-" + std::to_wstring(NowFileTime())).c_str(), 0);
    CreateDirectoryW(images.c_str(), nullptr);
}

bool LoadMasterKey() {
    std::wstring path = DataPath(L"key.bin");
    std::string blob;
    if (ReadFileBytes(path, &blob)) {
        DATA_BLOB in{(DWORD)blob.size(), (BYTE*)blob.data()}, out{};
        bool ok = CryptUnprotectData(&in, nullptr, nullptr, nullptr, nullptr,
                                     CRYPTPROTECT_UI_FORBIDDEN, &out) &&
                  out.cbData == 32;
        if (ok) {
            g_masterKey.assign((const char*)out.pbData, 32);
        }
        if (out.pbData) {
            SecureZeroMemory(out.pbData, out.cbData);
            LocalFree(out.pbData);
        }
        if (ok) {
            return true;
        }
        Wh_Log(L"Can't unlock the key, starting with new data");
        SetAside(path);
        SetAsideData();
    }

    g_masterKey.resize(32);
    BCryptGenRandom(nullptr, (PUCHAR)g_masterKey.data(), 32,
                    BCRYPT_USE_SYSTEM_PREFERRED_RNG);
    DATA_BLOB in{32, (BYTE*)g_masterKey.data()}, out{};
    if (!CryptProtectData(&in, L"Clipboard Tweaks", nullptr, nullptr, nullptr,
                          CRYPTPROTECT_UI_FORBIDDEN, &out)) {
        return false;
    }
    bool ok = WriteFileAtomic(path, std::string((char*)out.pbData, out.cbData));
    LocalFree(out.pbData);
    return ok;
}

////////////////////////////////////////////////////////////////////////////////
// Serialization

struct Writer {
    std::string s;
    void Raw(const void* p, size_t n) { s.append((const char*)p, n); }
    template <class T>
    void Num(T v) {
        Raw(&v, sizeof(v));
    }
    void Str(std::string_view v) {
        Num<UINT32>((UINT32)v.size());
        Raw(v.data(), v.size());
    }
    void WStr(const std::wstring& v) { Str(Bytes(v)); }
};

struct Reader {
    const char* p;
    const char* end;
    bool ok = true;
    explicit Reader(const std::string& s) : p(s.data()), end(s.data() + s.size()) {}
    bool Raw(void* out, size_t n) {
        if (!ok || (size_t)(end - p) < n) {
            return ok = false;
        }
        memcpy(out, p, n);
        p += n;
        return true;
    }
    template <class T>
    T Num() {
        T v{};
        Raw(&v, sizeof(v));
        return v;
    }
    std::string Str() {
        UINT32 n = Num<UINT32>();
        if (!ok || (size_t)(end - p) < n) {
            ok = false;
            return {};
        }
        std::string v(p, n);
        p += n;
        return v;
    }
    std::wstring WStr() {
        std::string b = Str();
        if (b.size() % 2) {
            ok = false;
            return {};
        }
        return std::wstring((const wchar_t*)b.data(), b.size() / 2);
    }
};

constexpr char kIndexMagic[4] = {'C', 'T', 'I', 'X'};
constexpr UINT32 kIndexVersion = 1;
constexpr char kSettingsMagic[4] = {'C', 'T', 'S', 'T'};

std::string SerializeIndex() {
    Writer w;
    w.Raw(kIndexMagic, 4);
    w.Num<UINT32>(kIndexVersion);
    w.Num<UINT64>(g_nextId);
    w.Num<UINT32>((UINT32)g_items.size());
    for (const Item& it : g_items) {
        w.Num<UINT64>(it.id);
        w.Num<UINT64>(it.time);
        w.Num<BYTE>((BYTE)it.type);
        w.WStr(it.source);
        w.WStr(it.text);
        w.Str(it.html);
        w.Str(it.rtf);
        w.WStr(it.files);
        w.Num<UINT32>(it.imageWidth);
        w.Num<UINT32>(it.imageHeight);
        w.Num<UINT16>(it.thumbWidth);
        w.Num<UINT16>(it.thumbHeight);
        w.Str(it.thumb);
        w.Raw(it.hash, sizeof(it.hash));
    }
    return std::move(w.s);
}

void FinishItem(Item& it);

bool ParseIndex(const std::string& data, LoadResult* r) {
    Reader rd(data);
    char magic[4];
    if (!rd.Raw(magic, 4) || memcmp(magic, kIndexMagic, 4) != 0 ||
        rd.Num<UINT32>() != kIndexVersion) {
        return false;
    }
    r->nextId = rd.Num<UINT64>();
    UINT32 count = rd.Num<UINT32>();
    std::vector<Item> items;
    for (UINT32 i = 0; i < count && rd.ok; i++) {
        Item it;
        it.id = rd.Num<UINT64>();
        it.time = rd.Num<UINT64>();
        it.type = (ItemType)rd.Num<BYTE>();
        it.source = rd.WStr();
        it.text = rd.WStr();
        it.html = rd.Str();
        it.rtf = rd.Str();
        it.files = rd.WStr();
        it.imageWidth = rd.Num<UINT32>();
        it.imageHeight = rd.Num<UINT32>();
        it.thumbWidth = rd.Num<UINT16>();
        it.thumbHeight = rd.Num<UINT16>();
        it.thumb = rd.Str();
        rd.Raw(it.hash, sizeof(it.hash));
        if (it.thumb.size() != (size_t)it.thumbWidth * it.thumbHeight * 4) {
            it.thumb.clear();
            it.thumbWidth = it.thumbHeight = 0;
        }
        FinishItem(it);
        items.push_back(std::move(it));
    }
    if (!rd.ok) {
        return false;
    }
    r->items = std::move(items);
    return true;
}

std::string SerializeSettings() {
    Writer w;
    w.Raw(kSettingsMagic, 4);
    w.Num<UINT32>((UINT32)g_settings.size());
    for (const auto& [key, value] : g_settings) {
        w.WStr(key);
        w.Num<INT64>(value);
    }
    return std::move(w.s);
}

bool ParseSettings(const std::string& data, LoadResult* r) {
    Reader rd(data);
    char magic[4];
    if (!rd.Raw(magic, 4) || memcmp(magic, kSettingsMagic, 4) != 0) {
        return false;
    }
    UINT32 count = rd.Num<UINT32>();
    for (UINT32 i = 0; i < count && rd.ok; i++) {
        std::wstring key = rd.WStr();
        r->settings[key] = rd.Num<INT64>();
    }
    return rd.ok;
}

////////////////////////////////////////////////////////////////////////////////
// Worker thread: decoding, encryption and disk writes, so that the window never
// waits for them.

void Enqueue(std::function<void()> job) {
    AcquireSRWLockExclusive(&g_workLock);
    g_work.push_back(std::move(job));
    ReleaseSRWLockExclusive(&g_workLock);
    WakeConditionVariable(&g_workCv);
}

IWICImagingFactory* g_workerWic;  // Worker thread only.

DWORD WINAPI WorkerProc(void*) {
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                     IID_PPV_ARGS(&g_workerWic));
    for (;;) {
        AcquireSRWLockExclusive(&g_workLock);
        while (g_work.empty() && !g_workQuit) {
            SleepConditionVariableSRW(&g_workCv, &g_workLock, INFINITE, 0);
        }
        if (g_work.empty()) {
            ReleaseSRWLockExclusive(&g_workLock);
            break;
        }
        std::function<void()> job = std::move(g_work.front());
        g_work.pop_front();
        ReleaseSRWLockExclusive(&g_workLock);
        job();
    }
    SafeRelease(g_workerWic);
    CoUninitialize();
    return 0;
}

void StopWorker() {
    if (!g_workerThread) {
        return;
    }
    AcquireSRWLockExclusive(&g_workLock);
    g_workQuit = true;
    ReleaseSRWLockExclusive(&g_workLock);
    WakeConditionVariable(&g_workCv);
    WaitForSingleObject(g_workerThread, 30000);
    CloseHandle(g_workerThread);
    g_workerThread = nullptr;
}

void WritePendingSaves() {
    AcquireSRWLockExclusive(&g_saveLock);
    std::map<std::wstring, std::string> saves = std::move(g_pendingSaves);
    g_pendingSaves.clear();
    ReleaseSRWLockExclusive(&g_saveLock);
    for (const auto& [name, plain] : saves) {
        WriteEncrypted(DataPath(name.c_str()), plain);
    }
}

// Only the newest snapshot of a file is written, however often it changes.
void QueueSave(const wchar_t* name, std::string plain) {
    AcquireSRWLockExclusive(&g_saveLock);
    bool schedule = g_pendingSaves.empty();
    g_pendingSaves[name] = std::move(plain);
    ReleaseSRWLockExclusive(&g_saveLock);
    if (schedule) {
        Enqueue(WritePendingSaves);
    }
}

void SaveIndexNow() {
    if (g_loaded) {
        // ponytail: the whole index is rewritten on every change; switch to
        // incremental writes if it ever holds tens of thousands of items.
        QueueSave(L"index.dat", SerializeIndex());
    }
}

void SaveIndex() {
    // Coalesces changes made while handling one burst of messages.
    PostMessageW(g_hwnd, WM_APP_SAVE, 0, 0);
}

void SaveSettings() {
    if (g_loaded) {
        QueueSave(L"settings.dat", SerializeSettings());
    }
}

void DeleteImagesLater(std::vector<UINT64> ids) {
    if (!ids.empty()) {
        Enqueue([ids = std::move(ids)] {
            for (UINT64 id : ids) {
                DeleteFileW(ImagePath(id).c_str());
            }
        });
    }
}

// Removes image files that no item refers to, left by a crash or by a history
// clear that wasn't undone.
void DeleteOrphanImages(const std::vector<Item>& items) {
    std::unordered_set<UINT64> used;
    for (const Item& it : items) {
        if (it.imageWidth) {
            used.insert(it.id);
        }
    }
    WIN32_FIND_DATAW fd;
    HANDLE find =
        FindFirstFileW((g_dataDir + L"\\images\\*.bin").c_str(), &fd);
    if (find == INVALID_HANDLE_VALUE) {
        return;
    }
    do {
        UINT64 id = wcstoull(fd.cFileName, nullptr, 10);
        if (!used.contains(id)) {
            DeleteFileW(ImagePath(id).c_str());
        }
    } while (FindNextFileW(find, &fd));
    FindClose(find);
}

void LoadJob() {
    auto* r = new LoadResult;
    CreateDirectoryW(g_dataDir.c_str(), nullptr);
    CreateDirectoryW((g_dataDir + L"\\images").c_str(), nullptr);
    if (LoadMasterKey()) {
        int index = ReadEncrypted(DataPath(L"index.dat"), [r](const std::string& s) {
            return ParseIndex(s, r);
        });
        if (index < 0) {
            SetAsideData();
        } else {
            DeleteOrphanImages(r->items);
        }
        ReadEncrypted(DataPath(L"settings.dat"), [r](const std::string& s) {
            return ParseSettings(s, r);
        });
    } else {
        Wh_Log(L"Can't create the encryption key, history won't be saved");
    }
    if (!PostMessageW(g_hwnd, WM_APP_LOADED, 0, (LPARAM)r)) {
        delete r;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Images (WIC)

IWICBitmapSource* DecodeImage(IWICImagingFactory* wic, const std::string& data) {
    IWICStream* stream = nullptr;
    IWICBitmapDecoder* decoder = nullptr;
    IWICBitmapFrameDecode* frame = nullptr;
    if (SUCCEEDED(wic->CreateStream(&stream)) &&
        SUCCEEDED(stream->InitializeFromMemory((BYTE*)data.data(),
                                               (DWORD)data.size())) &&
        SUCCEEDED(wic->CreateDecoderFromStream(
            stream, nullptr, WICDecodeMetadataCacheOnDemand, &decoder))) {
        decoder->GetFrame(0, &frame);
    }
    SafeRelease(decoder);
    SafeRelease(stream);
    return frame;
}

// Turns a CF_DIB into a BMP file in memory, which WIC decodes in all its
// variants (bit depths, palettes, bottom-up or top-down).
std::string DibToBmp(const std::string& dib) {
    if (dib.size() < sizeof(BITMAPINFOHEADER)) {
        return {};
    }
    const auto* bih = (const BITMAPINFOHEADER*)dib.data();
    DWORD offset = bih->biSize;
    if (bih->biSize == sizeof(BITMAPINFOHEADER) && bih->biCompression == BI_BITFIELDS) {
        offset += 12;
    }
    DWORD colors = bih->biClrUsed;
    if (!colors && bih->biBitCount <= 8) {
        colors = 1u << bih->biBitCount;
    }
    offset += colors * 4;
    BITMAPFILEHEADER bfh{};
    bfh.bfType = 0x4D42;
    bfh.bfSize = (DWORD)(sizeof(bfh) + dib.size());
    bfh.bfOffBits = (DWORD)sizeof(bfh) + offset;
    std::string bmp((const char*)&bfh, sizeof(bfh));
    bmp += dib;
    return bmp;
}

bool EncodePng(IWICImagingFactory* wic, IWICBitmapSource* source, std::string* png) {
    IStream* stream = nullptr;
    IWICBitmapEncoder* encoder = nullptr;
    IWICBitmapFrameEncode* frame = nullptr;
    bool ok = SUCCEEDED(CreateStreamOnHGlobal(nullptr, TRUE, &stream)) &&
              SUCCEEDED(wic->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder)) &&
              SUCCEEDED(encoder->Initialize(stream, WICBitmapEncoderNoCache)) &&
              SUCCEEDED(encoder->CreateNewFrame(&frame, nullptr)) &&
              SUCCEEDED(frame->Initialize(nullptr)) &&
              SUCCEEDED(frame->WriteSource(source, nullptr)) &&
              SUCCEEDED(frame->Commit()) && SUCCEEDED(encoder->Commit());
    if (ok) {
        HGLOBAL mem;
        STATSTG stat;
        ok = SUCCEEDED(GetHGlobalFromStream(stream, &mem)) &&
             SUCCEEDED(stream->Stat(&stat, STATFLAG_NONAME));
        if (ok) {
            png->assign((const char*)GlobalLock(mem), (size_t)stat.cbSize.QuadPart);
            GlobalUnlock(mem);
        }
    }
    SafeRelease(frame);
    SafeRelease(encoder);
    SafeRelease(stream);
    return ok;
}

// Converts any image to premultiplied BGRA, optionally scaled.
bool ToPixels(IWICImagingFactory* wic, IWICBitmapSource* source, UINT width,
              UINT height, WICPixelFormatGUID format, std::string* pixels) {
    IWICBitmapScaler* scaler = nullptr;
    IWICFormatConverter* converter = nullptr;
    UINT w, h;
    source->GetSize(&w, &h);
    IWICBitmapSource* input = source;
    bool ok = true;
    if (w != width || h != height) {
        ok = SUCCEEDED(wic->CreateBitmapScaler(&scaler)) &&
             SUCCEEDED(scaler->Initialize(source, width, height,
                                          WICBitmapInterpolationModeFant));
        input = scaler;
    }
    ok = ok && SUCCEEDED(wic->CreateFormatConverter(&converter)) &&
         SUCCEEDED(converter->Initialize(input, format, WICBitmapDitherTypeNone,
                                         nullptr, 0,
                                         WICBitmapPaletteTypeCustom));
    if (ok) {
        pixels->resize((size_t)width * height * 4);
        ok = SUCCEEDED(converter->CopyPixels(nullptr, width * 4,
                                             (UINT)pixels->size(),
                                             (BYTE*)pixels->data()));
    }
    SafeRelease(converter);
    SafeRelease(scaler);
    return ok;
}

// Fills the image fields of an item. The PNG keeps the original resolution
// and is lossless; a PNG from the clipboard is kept byte for byte.
bool ProcessImage(IWICImagingFactory* wic, Capture* c, Item* it, std::string* png) {
    std::string bmp;
    if (c->png.empty()) {
        bmp = DibToBmp(c->dib);
    }
    IWICBitmapSource* source = DecodeImage(wic, c->png.empty() ? bmp : c->png);
    if (!source) {
        return false;
    }
    UINT w = 0, h = 0;
    source->GetSize(&w, &h);
    bool ok = w && h && (!c->png.empty() || EncodePng(wic, source, png));
    if (ok && !c->png.empty()) {
        *png = std::move(c->png);
    }
    if (ok) {
        float scale = std::min(1.f, (float)kThumbSize / std::max(w, h));
        UINT tw = std::max(1u, (UINT)std::lround(w * scale));
        UINT th = std::max(1u, (UINT)std::lround(h * scale));
        if (ToPixels(wic, source, tw, th, GUID_WICPixelFormat32bppPBGRA, &it->thumb)) {
            it->thumbWidth = (UINT16)tw;
            it->thumbHeight = (UINT16)th;
        }
        it->imageWidth = w;
        it->imageHeight = h;
    }
    source->Release();
    return ok;
}

// A 32 bpp bottom-up CF_DIB, which every app that takes images understands.
bool PngToDib(const std::string& png, std::string* dib) {
    IWICBitmapSource* source = DecodeImage(g_wic, png);
    if (!source) {
        return false;
    }
    UINT w = 0, h = 0;
    source->GetSize(&w, &h);
    std::string pixels;
    bool ok = ToPixels(g_wic, source, w, h, GUID_WICPixelFormat32bppBGRA, &pixels);
    source->Release();
    if (!ok) {
        return false;
    }
    BITMAPINFOHEADER bih{sizeof(bih), (LONG)w, (LONG)h, 1, 32, BI_RGB};
    dib->assign((const char*)&bih, sizeof(bih));
    dib->reserve(sizeof(bih) + pixels.size());
    for (UINT y = h; y-- > 0;) {
        dib->append(pixels, (size_t)y * w * 4, (size_t)w * 4);
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Item processing

std::wstring Trim(const std::wstring& s) {
    size_t a = s.find_first_not_of(L" \t\r\n");
    if (a == std::wstring::npos) {
        return {};
    }
    size_t b = s.find_last_not_of(L" \t\r\n");
    return s.substr(a, b - a + 1);
}

bool StartsWithNoCase(const std::wstring& s, const wchar_t* prefix) {
    size_t n = wcslen(prefix);
    return s.size() >= n && _wcsnicmp(s.c_str(), prefix, n) == 0;
}

// ponytail: naive heuristic; a real language detector isn't worth its code.
bool LooksLikeCode(const std::wstring& s) {
    static const wchar_t* const kStarts[] = {
        L"SELECT ", L"INSERT ", L"UPDATE ", L"DELETE ", L"CREATE ",
        L"#include", L"import ", L"function ", L"def ", L"class ",
        L"const ", L"let ", L"var ", L"<?", L"<!DOCTYPE", L"<html", L"{", L"//"};
    for (const wchar_t* start : kStarts) {
        if (StartsWithNoCase(s, start)) {
            return true;
        }
    }
    size_t symbols = std::count_if(s.begin(), s.end(), [](wchar_t c) {
        return wcschr(L"{}();=<>[]", c) != nullptr;
    });
    return s.size() >= 8 && symbols * 12 >= s.size();
}

bool ParseColor(const std::wstring& s, UINT32* rgb) {
    if ((s.size() != 7 && s.size() != 4) || s[0] != L'#' ||
        !std::all_of(s.begin() + 1, s.end(), iswxdigit)) {
        return false;
    }
    UINT32 v = wcstoul(s.c_str() + 1, nullptr, 16);
    if (s.size() == 4) {
        v = (v & 0xF00) * 0x1100 | (v & 0x0F0) * 0x110 | (v & 0x00F) * 0x11;
    }
    *rgb = v;
    return true;
}

ItemType DetectType(const Item& it) {
    if (it.imageWidth) {
        return ItemType::Image;
    }
    if (!it.files.empty()) {
        return ItemType::Files;
    }
    std::wstring s = Trim(it.text);
    UINT32 rgb;
    if (ParseColor(s, &rgb)) {
        return ItemType::Color;
    }
    bool oneWord = s.find_first_of(L" \t\r\n") == std::wstring::npos;
    if (oneWord && (StartsWithNoCase(s, L"http://") ||
                    StartsWithNoCase(s, L"https://") ||
                    StartsWithNoCase(s, L"ftp://") ||
                    StartsWithNoCase(s, L"www.") ||
                    StartsWithNoCase(s, L"mailto:"))) {
        return ItemType::Link;
    }
    return LooksLikeCode(s) ? ItemType::Code : ItemType::Text;
}

// Fills the fields that are derived from the content and not saved.
void FinishItem(Item& it) {
    if (it.imageWidth) {
        it.preview = L"Obraz · " + std::to_wstring(it.imageWidth) + L" × " +
                     std::to_wstring(it.imageHeight);
        return;
    }
    const std::wstring& src = it.text.empty() ? it.files : it.text;
    it.preview.clear();
    bool space = false;
    for (wchar_t c : src) {
        if (it.preview.size() >= 300) {
            break;
        }
        if (iswspace(c)) {
            space = !it.preview.empty();
            continue;
        }
        if (space) {
            it.preview += L' ';
            space = false;
        }
        it.preview += c;
    }
    if (it.type == ItemType::Color) {
        ParseColor(Trim(it.text), &it.color);
    }
}

std::wstring AppName(const std::wstring& path) {
    static std::map<std::wstring, std::wstring> cache;
    if (auto found = cache.find(path); found != cache.end()) {
        return found->second;
    }
    std::wstring name;
    DWORD size = GetFileVersionInfoSizeW(path.c_str(), nullptr);
    std::vector<BYTE> info(size);
    struct Translation {
        WORD language, codePage;
    }* tr;
    UINT len;
    if (size && GetFileVersionInfoW(path.c_str(), 0, size, info.data()) &&
        VerQueryValueW(info.data(), L"\\VarFileInfo\\Translation", (void**)&tr,
                       &len) &&
        len >= sizeof(Translation)) {
        WCHAR query[64];
        swprintf_s(query, L"\\StringFileInfo\\%04x%04x\\FileDescription",
                   tr->language, tr->codePage);
        WCHAR* description;
        if (VerQueryValueW(info.data(), query, (void**)&description, &len) && len > 1) {
            name = Trim(description);
        }
    }
    if (name.empty()) {
        size_t slash = path.find_last_of(L'\\');
        name = path.substr(slash == std::wstring::npos ? 0 : slash + 1);
        if (StartsWithNoCase(name.size() > 4 ? name.substr(name.size() - 4) : L"",
                             L".exe")) {
            name.resize(name.size() - 4);
        }
    }
    return cache[path] = name;
}

void ProcessCapture(Capture* c) {
    auto* r = new Captured;
    Item& it = r->item;
    it.time = c->time;
    it.text = std::move(c->text);
    it.html = std::move(c->html);
    it.rtf = std::move(c->rtf);
    it.files = std::move(c->files);
    if (!c->sourcePath.empty()) {
        it.source = AppName(c->sourcePath);
    }
    if ((!c->png.empty() || !c->dib.empty()) && g_workerWic &&
        !ProcessImage(g_workerWic, c, &it, &r->png)) {
        Wh_Log(L"Can't read the copied image");
    }
    delete c;
    if (it.text.empty() && it.files.empty() && !it.imageWidth) {
        delete r;
        return;
    }
    it.type = DetectType(it);
    FinishItem(it);
    Sha256({Bytes(it.text), it.html, it.rtf, Bytes(it.files), r->png}, it.hash);
    if (!PostMessageW(g_hwnd, WM_APP_CAPTURED, 0, (LPARAM)r)) {
        delete r;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Clipboard

std::string ClipboardBytes(UINT format) {
    HANDLE h = GetClipboardData(format);
    void* p = h ? GlobalLock(h) : nullptr;
    if (!p) {
        return {};
    }
    std::string s((const char*)p, GlobalSize(h));
    GlobalUnlock(h);
    return s;
}

// Formats which mark content that must not be kept, such as passwords from a
// password manager.
bool IsPrivateClipboard() {
    if (IsClipboardFormatAvailable(g_cfExclude) ||
        IsClipboardFormatAvailable(g_cfViewerIgnore)) {
        return true;
    }
    if (IsClipboardFormatAvailable(g_cfCanInclude)) {
        std::string v = ClipboardBytes(g_cfCanInclude);
        return v.size() >= sizeof(DWORD) && *(const DWORD*)v.data() == 0;
    }
    return false;
}

std::wstring ProcessPath(HWND owner) {
    DWORD pid = 0;
    if (!owner || !GetWindowThreadProcessId(owner, &pid) || !pid) {
        return {};
    }
    HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!process) {
        return {};
    }
    WCHAR path[MAX_PATH * 2];
    DWORD size = ARRAYSIZE(path);
    bool ok = QueryFullProcessImageNameW(process, 0, path, &size);
    CloseHandle(process);
    return ok ? path : L"";
}

void OnClipboardUpdate() {
    if (g_paused || GetClipboardSequenceNumber() == g_ownClipboardSeq) {
        return;
    }
    if (!OpenClipboard(g_hwnd)) {
        // Another app still holds the clipboard; try again shortly.
        if (g_clipboardRetries++ < 5) {
            SetTimer(g_hwnd, kTimerClipboardRetry, 50, nullptr);
        }
        return;
    }
    g_clipboardRetries = 0;
    auto* c = new Capture;
    if (!IsPrivateClipboard()) {
        std::string text = ClipboardBytes(CF_UNICODETEXT);
        c->text.assign((const wchar_t*)text.data(), text.size() / 2);
        c->text.resize(wcsnlen(c->text.c_str(), c->text.size()));
        c->html = ClipboardBytes(g_cfHtml);
        c->html.resize(strnlen(c->html.c_str(), c->html.size()));
        c->rtf = ClipboardBytes(g_cfRtf);
        c->rtf.resize(strnlen(c->rtf.c_str(), c->rtf.size()));
        if (HDROP drop = (HDROP)GetClipboardData(CF_HDROP)) {
            UINT count = DragQueryFileW(drop, 0xFFFFFFFF, nullptr, 0);
            for (UINT i = 0; i < count; i++) {
                WCHAR path[MAX_PATH * 4];
                if (DragQueryFileW(drop, i, path, ARRAYSIZE(path))) {
                    c->files += (c->files.empty() ? L"" : L"\n");
                    c->files += path;
                }
            }
        }
        // Office apps add a picture of every text or cell selection; with
        // RTF present the text is what was copied.
        if (c->rtf.empty()) {
            c->png = ClipboardBytes(g_cfPng);
            if (c->png.empty()) {
                c->dib = ClipboardBytes(CF_DIB);
            }
        }
    }
    HWND owner = GetClipboardOwner();
    CloseClipboard();
    if (c->text.empty() && c->files.empty() && c->png.empty() && c->dib.empty()) {
        delete c;
        return;
    }
    c->time = NowFileTime();
    c->sourcePath = ProcessPath(owner);
    Enqueue([c] { ProcessCapture(c); });
}

void PutGlobal(UINT format, const void* data, size_t size) {
    HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE, size);
    if (!h) {
        return;
    }
    memcpy(GlobalLock(h), data, size);
    GlobalUnlock(h);
    if (!SetClipboardData(format, h)) {
        GlobalFree(h);
    }
}

bool LoadImagePng(UINT64 id, std::string* png) {
    std::string data;
    return ReadFileBytes(ImagePath(id), &data) && Decrypt(data, png);
}

bool PutOnClipboard(const Item& it, bool plain) {
    std::string png;
    if (it.imageWidth && !LoadImagePng(it.id, &png)) {
        Wh_Log(L"Can't read image %llu", it.id);
        return false;
    }
    bool opened = false;
    for (int i = 0; i < 10 && !(opened = OpenClipboard(g_hwnd)); i++) {
        Sleep(10);
    }
    if (!opened) {
        return false;
    }
    EmptyClipboard();
    std::wstring text = it.text.empty() && plain ? it.files : it.text;
    if (!text.empty()) {
        PutGlobal(CF_UNICODETEXT, text.c_str(), (text.size() + 1) * sizeof(wchar_t));
    }
    if (!plain) {
        if (!it.html.empty()) {
            PutGlobal(g_cfHtml, it.html.c_str(), it.html.size() + 1);
        }
        if (!it.rtf.empty()) {
            PutGlobal(g_cfRtf, it.rtf.c_str(), it.rtf.size() + 1);
        }
        if (!it.files.empty()) {
            std::wstring list = it.files;
            std::replace(list.begin(), list.end(), L'\n', L'\0');
            list.append(2, L'\0');
            std::string drop(sizeof(DROPFILES), '\0');
            auto* df = (DROPFILES*)drop.data();
            df->pFiles = sizeof(DROPFILES);
            df->fWide = TRUE;
            drop += Bytes(list);
            PutGlobal(CF_HDROP, drop.data(), drop.size());
        }
    }
    if (!png.empty()) {
        PutGlobal(g_cfPng, png.data(), png.size());
        std::string dib;
        if (PngToDib(png, &dib)) {
            PutGlobal(CF_DIB, dib.data(), dib.size());
        }
    }
    CloseClipboard();
    g_ownClipboardSeq = GetClipboardSequenceNumber();
    return true;
}

void SendCtrlV() {
    std::vector<INPUT> inputs;
    auto key = [&](WORD vk, bool up) {
        INPUT in{INPUT_KEYBOARD};
        in.ki.wVk = vk;
        in.ki.dwFlags = up ? KEYEVENTF_KEYUP : 0;
        in.ki.dwExtraInfo = kInputTag;
        inputs.push_back(in);
    };
    // Releases held modifiers, so that Shift+Enter or Alt don't turn Ctrl+V
    // into another shortcut. The dummy key keeps an Alt or Win release from
    // opening a menu.
    key(kDummyVk, false);
    key(kDummyVk, true);
    for (WORD vk : {VK_LSHIFT, VK_RSHIFT, VK_LMENU, VK_RMENU, VK_LWIN, VK_RWIN}) {
        if (GetAsyncKeyState(vk) < 0) {
            key(vk, true);
        }
    }
    bool ctrl = GetAsyncKeyState(VK_CONTROL) < 0;
    if (!ctrl) {
        key(VK_CONTROL, false);
    }
    key('V', false);
    key('V', true);
    if (!ctrl) {
        key(VK_CONTROL, true);
    }
    SendInput((UINT)inputs.size(), inputs.data(), sizeof(INPUT));
}

////////////////////////////////////////////////////////////////////////////////
// Windows clipboard history switch (the mod's only Windhawk setting)

constexpr wchar_t kClipboardKey[] = L"Software\\Microsoft\\Clipboard";

void SetWindowsHistory(int value) {
    if (value < 0) {
        RegDeleteKeyValueW(HKEY_CURRENT_USER, kClipboardKey, L"EnableClipboardHistory");
    } else {
        DWORD v = value;
        RegSetKeyValueW(HKEY_CURRENT_USER, kClipboardKey, L"EnableClipboardHistory",
                        REG_DWORD, &v, sizeof(v));
    }
}

// The previous value is kept in the mod's storage, so it's restored even after
// a crash.
void ApplyWindowsHistorySetting(bool disable) {
    bool overridden = Wh_GetIntValue(L"historyOverridden", 0) != 0;
    if (disable) {
        if (!overridden) {
            DWORD v = RegDword(HKEY_CURRENT_USER, kClipboardKey,
                               L"EnableClipboardHistory", 0xFFFFFFFF);
            Wh_SetIntValue(L"historyPrevious", v == 0xFFFFFFFF ? -1 : (int)v);
            Wh_SetIntValue(L"historyOverridden", 1);
        }
        SetWindowsHistory(0);
    } else if (overridden) {
        SetWindowsHistory(Wh_GetIntValue(L"historyPrevious", -1));
        Wh_SetIntValue(L"historyOverridden", 0);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Theme

void LoadTheme() {
    Theme& t = g_theme;
    const wchar_t* personalize =
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
    t.dark = RegDword(HKEY_CURRENT_USER, personalize, L"AppsUseLightTheme", 1) == 0;
    t.lightTaskbar =
        RegDword(HKEY_CURRENT_USER, personalize, L"SystemUsesLightTheme", 0) != 0;
    HIGHCONTRASTW hc{sizeof(hc)};
    t.highContrast = SystemParametersInfoW(SPI_GETHIGHCONTRAST, sizeof(hc), &hc, 0) &&
                     (hc.dwFlags & HCF_HIGHCONTRASTON);

    // Windows accent shades: light 3..1, base, dark 1..3. Fluent uses
    // "light 2" on dark backgrounds and "dark 1" on light ones.
    BYTE palette[32];
    DWORD size = sizeof(palette);
    if (RegGetValueW(HKEY_CURRENT_USER,
                     L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Accent",
                     L"AccentPalette", RRF_RT_REG_BINARY, nullptr, palette,
                     &size) == ERROR_SUCCESS &&
        size >= 32) {
        BYTE* c = palette + (t.dark ? 1 : 4) * 4;
        t.accent = D2D1::ColorF(c[0] / 255.f, c[1] / 255.f, c[2] / 255.f);
    } else {
        t.accent = Rgba(t.dark ? 0x60CDFF : 0x005FB8);
    }

    if (t.highContrast) {
        D2D1_COLOR_F text = FromColorRef(GetSysColor(COLOR_WINDOWTEXT));
        D2D1_COLOR_F back = FromColorRef(GetSysColor(COLOR_WINDOW));
        t.accent = FromColorRef(GetSysColor(COLOR_HIGHLIGHT));
        t.text = t.text2 = t.strong = t.layerStroke = t.controlStroke = t.divider = text;
        t.window = t.layer = t.flyout = t.search = t.control = back;
        t.hover = t.pressed = FromColorRef(GetSysColor(COLOR_HIGHLIGHT), 0.35f);
    } else if (t.dark) {
        t.text = Rgba(0xFFFFFF);
        t.text2 = Rgba(0xFFFFFF, 0.786f);
        t.window = Rgba(0x202020);
        t.layer = Rgba(0x3A3A3A, 0.30f);
        t.layerStroke = Rgba(0x000000, 0.10f);
        t.hover = Rgba(0xFFFFFF, 0.061f);
        t.pressed = Rgba(0xFFFFFF, 0.042f);
        t.control = Rgba(0xFFFFFF, 0.061f);
        t.controlStroke = Rgba(0xFFFFFF, 0.07f);
        t.divider = Rgba(0xFFFFFF, 0.0837f);
        t.flyout = Rgba(0x2C2C2C);
        t.strong = Rgba(0xFFFFFF, 0.545f);
        t.search = Rgba(0x1F1F1F);
    } else {
        t.text = Rgba(0x000000, 0.894f);
        t.text2 = Rgba(0x000000, 0.62f);
        t.window = Rgba(0xF3F3F3);
        t.layer = Rgba(0xFFFFFF, 0.50f);
        t.layerStroke = Rgba(0x000000, 0.0578f);
        t.hover = Rgba(0x000000, 0.037f);
        t.pressed = Rgba(0x000000, 0.024f);
        t.control = Rgba(0xFFFFFF, 0.70f);
        t.controlStroke = Rgba(0x000000, 0.0578f);
        t.divider = Rgba(0x000000, 0.0803f);
        t.flyout = Rgba(0xF9F9F9);
        t.strong = Rgba(0x000000, 0.447f);
        t.search = Rgba(0xFFFFFF);
    }

    // The search field is a native EDIT control: it needs opaque GDI colors
    // that match the field drawn around it.
    t.editBack = ToColorRef(t.search);
    t.editText = t.highContrast ? GetSysColor(COLOR_WINDOWTEXT)
                                : (t.dark ? RGB(255, 255, 255) : RGB(26, 26, 26));
    t.editPlaceholder = t.highContrast ? GetSysColor(COLOR_GRAYTEXT)
                                       : (t.dark ? RGB(200, 200, 200) : RGB(97, 97, 97));
}

void ApplyWindowTheme() {
    BOOL dark = g_theme.dark;
    DwmSetWindowAttribute(g_hwnd, kDwmUseImmersiveDarkMode, &dark, sizeof(dark));
    DWORD corner = kDwmCornerRound;
    DwmSetWindowAttribute(g_hwnd, kDwmWindowCornerPreference, &corner, sizeof(corner));
    DWORD backdrop = g_theme.highContrast ? kDwmBackdropNone : kDwmBackdropAcrylic;
    g_theme.backdrop =
        SUCCEEDED(DwmSetWindowAttribute(g_hwnd, kDwmSystemBackdropType, &backdrop,
                                        sizeof(backdrop))) &&
        !g_theme.highContrast;
    MARGINS margins{-1, -1, -1, -1};
    DwmExtendFrameIntoClientArea(g_hwnd, &margins);
    g_theme.window.a = g_theme.backdrop ? kBackgroundOpacity : 1;

    if (g_editBrush) {
        DeleteObject(g_editBrush);
    }
    g_editBrush = CreateSolidBrush(g_theme.editBack);

    // Native menus (the "…" and tray menus) follow the dark mode too. The
    // functions are undocumented uxtheme exports, available since Windows 10
    // 1903.
    HMODULE uxtheme = GetModuleHandleW(L"uxtheme.dll");
    using SetPreferredAppMode_t = int(WINAPI*)(int);
    using FlushMenuThemes_t = void(WINAPI*)();
    auto setMode = (SetPreferredAppMode_t)(void*)GetProcAddress(uxtheme, MAKEINTRESOURCEA(135));
    auto flush = (FlushMenuThemes_t)(void*)GetProcAddress(uxtheme, MAKEINTRESOURCEA(136));
    if (setMode && flush) {
        setMode(g_theme.dark ? 2 : 3);  // ForceDark : ForceLight.
        flush();
    }
}

////////////////////////////////////////////////////////////////////////////////
// Tray icon

HICON CreateTrayIcon() {
    int size = GetSystemMetricsForDpi(SM_CXSMICON, GetDpiForSystem());
    BITMAPINFO bi{};
    bi.bmiHeader = {sizeof(BITMAPINFOHEADER), size, -size, 1, 32, BI_RGB};
    void* bits = nullptr;
    HDC dc = CreateCompatibleDC(nullptr);
    HBITMAP color = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &bits, nullptr, 0);
    if (!color) {
        DeleteDC(dc);
        return nullptr;
    }
    HGDIOBJ oldBitmap = SelectObject(dc, color);
    HFONT font = CreateFontW(-size, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0,
                             0, ANTIALIASED_QUALITY, 0, g_iconFont);
    HGDIOBJ oldFont = SelectObject(dc, font);
    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, RGB(255, 255, 255));
    RECT rc{0, 0, size, size};
    wchar_t glyph = kIconApp;
    DrawTextW(dc, &glyph, 1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
    GdiFlush();

    // White text on black gives the glyph coverage, used as the alpha channel.
    auto* px = (UINT32*)bits;
    if (g_paused) {
        // Paused: a dimmed clipboard with two bars in its lower right corner.
        int x0 = size / 2, y0 = size / 2, bar = std::max(1, size / 8);
        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                UINT32& p = px[y * size + x];
                BYTE cov = (BYTE)(p & 0xFF) * 2 / 5;
                if (x >= x0 - 1 && y >= y0 - 1) {
                    cov = 0;
                }
                bool inBar = y >= y0 + bar / 2 && y < size - bar / 2 &&
                             ((x >= x0 + bar && x < x0 + 2 * bar) ||
                              (x >= x0 + 3 * bar && x < x0 + 4 * bar));
                p = inBar ? 0xFF : cov;
            }
        }
    }
    UINT32 rgb = g_theme.lightTaskbar ? 0x000000 : 0xFFFFFF;
    for (int i = 0; i < size * size; i++) {
        px[i] = (px[i] & 0xFF) << 24 | rgb;
    }

    std::vector<BYTE> maskBits(((size + 15) / 16 * 2) * size, 0);
    HBITMAP mask = CreateBitmap(size, size, 1, 1, maskBits.data());
    ICONINFO ii{TRUE, 0, 0, mask, color};
    HICON icon = CreateIconIndirect(&ii);
    SelectObject(dc, oldFont);
    SelectObject(dc, oldBitmap);
    DeleteObject(font);
    DeleteObject(mask);
    DeleteObject(color);
    DeleteDC(dc);
    return icon;
}

void UpdateTrayIcon(DWORD message) {
    HICON old = g_trayIcon;
    g_trayIcon = CreateTrayIcon();
    NOTIFYICONDATAW nid{sizeof(nid)};
    nid.hWnd = g_hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_SHOWTIP;
    nid.uCallbackMessage = WM_APP_TRAY;
    nid.hIcon = g_trayIcon;
    wcscpy_s(nid.szTip, g_paused ? L"Clipboard Tweaks: zapisywanie wstrzymane"
                                 : L"Clipboard Tweaks");
    Shell_NotifyIconW(message, &nid);
    if (message == NIM_ADD) {
        nid.uVersion = NOTIFYICON_VERSION_4;
        Shell_NotifyIconW(NIM_SETVERSION, &nid);
    }
    if (old) {
        DestroyIcon(old);
    }
}

void RemoveTrayIcon() {
    NOTIFYICONDATAW nid{sizeof(nid)};
    nid.hWnd = g_hwnd;
    nid.uID = 1;
    Shell_NotifyIconW(NIM_DELETE, &nid);
    if (g_trayIcon) {
        DestroyIcon(g_trayIcon);
        g_trayIcon = nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Fonts and drawing helpers

bool HasFont(const wchar_t* name) {
    IDWriteFontCollection* fonts = nullptr;
    UINT32 index;
    BOOL exists = FALSE;
    if (SUCCEEDED(g_dwrite->GetSystemFontCollection(&fonts))) {
        fonts->FindFamilyName(name, &index, &exists);
        fonts->Release();
    }
    return exists;
}

IDWriteTextFormat* MakeFormat(const wchar_t* family, float size,
                              DWRITE_FONT_WEIGHT weight,
                              DWRITE_TEXT_ALIGNMENT align, bool ellipsis = false) {
    IDWriteTextFormat* f = nullptr;
    if (FAILED(g_dwrite->CreateTextFormat(family, nullptr, weight,
                                          DWRITE_FONT_STYLE_NORMAL,
                                          DWRITE_FONT_STRETCH_NORMAL, size, L"", &f))) {
        return nullptr;
    }
    f->SetTextAlignment(align);
    f->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    f->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
    if (ellipsis) {
        IDWriteInlineObject* sign = nullptr;
        if (SUCCEEDED(g_dwrite->CreateEllipsisTrimmingSign(f, &sign))) {
            DWRITE_TRIMMING trimming{DWRITE_TRIMMING_GRANULARITY_CHARACTER};
            f->SetTrimming(&trimming, sign);
            sign->Release();
        }
    }
    return f;
}

float TextWidth(std::wstring_view s, IDWriteTextFormat* format) {
    IDWriteTextLayout* layout = nullptr;
    DWRITE_TEXT_METRICS m{};
    if (format && SUCCEEDED(g_dwrite->CreateTextLayout(s.data(), (UINT32)s.size(),
                                                       format, 10000, 100, &layout))) {
        layout->GetMetrics(&m);
        layout->Release();
    }
    return m.widthIncludingTrailingWhitespace;
}

void CreateFonts() {
    if (HasFont(L"Segoe UI Variable Text")) {
        g_uiFont = L"Segoe UI Variable Text";
    }
    if (!HasFont(g_iconFont)) {
        g_iconFont = L"Segoe MDL2 Assets";
    }
    auto L = DWRITE_TEXT_ALIGNMENT_LEADING;
    auto C = DWRITE_TEXT_ALIGNMENT_CENTER;
    auto R = DWRITE_TEXT_ALIGNMENT_TRAILING;
    g_fonts.title = MakeFormat(g_uiFont, 12, DWRITE_FONT_WEIGHT_NORMAL, L);
    g_fonts.tab = MakeFormat(g_uiFont, 14, DWRITE_FONT_WEIGHT_SEMI_BOLD, C);
    g_fonts.row = MakeFormat(g_uiFont, 14, DWRITE_FONT_WEIGHT_NORMAL, L, true);
    g_fonts.meta = MakeFormat(g_uiFont, 12, DWRITE_FONT_WEIGHT_NORMAL, R);
    g_fonts.header = MakeFormat(g_uiFont, 12, DWRITE_FONT_WEIGHT_SEMI_BOLD, L);
    g_fonts.small = MakeFormat(g_uiFont, 12, DWRITE_FONT_WEIGHT_NORMAL, L, true);
    g_fonts.smallBold = MakeFormat(g_uiFont, 12, DWRITE_FONT_WEIGHT_SEMI_BOLD, L);
    g_fonts.key = MakeFormat(g_uiFont, 11, DWRITE_FONT_WEIGHT_NORMAL, C);
    g_fonts.empty = MakeFormat(g_uiFont, 14, DWRITE_FONT_WEIGHT_NORMAL, C);
    if (g_fonts.empty) {
        g_fonts.empty->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
    }
    g_fonts.icon16 = MakeFormat(g_iconFont, 16, DWRITE_FONT_WEIGHT_NORMAL, C);
    g_fonts.icon14 = MakeFormat(g_iconFont, 14, DWRITE_FONT_WEIGHT_NORMAL, C);
    g_fonts.icon12 = MakeFormat(g_iconFont, 12, DWRITE_FONT_WEIGHT_NORMAL, C);
    g_tabWidth = TextWidth(L"Historia", g_fonts.tab);
    g_clearWidth = TextWidth(L"Wyczyść historię", g_fonts.small);
    g_undoWidth = TextWidth(L"Cofnij", g_fonts.small);
}

void ReleaseFonts() {
    for (IDWriteTextFormat** f :
         {&g_fonts.title, &g_fonts.tab, &g_fonts.row, &g_fonts.meta, &g_fonts.header,
          &g_fonts.small, &g_fonts.smallBold, &g_fonts.key, &g_fonts.empty,
          &g_fonts.icon16, &g_fonts.icon14, &g_fonts.icon12}) {
        SafeRelease(*f);
    }
}


void Fill(const D2D1_RECT_F& r, D2D1_COLOR_F c, float radius = 0) {
    g_brush->SetColor(c);
    if (radius > 0) {
        g_rt->FillRoundedRectangle(D2D1::RoundedRect(r, radius, radius), g_brush);
    } else {
        g_rt->FillRectangle(r, g_brush);
    }
}

void Stroke(const D2D1_RECT_F& r, D2D1_COLOR_F c, float radius) {
    g_brush->SetColor(c);
    D2D1_RECT_F inner{r.left + 0.5f, r.top + 0.5f, r.right - 0.5f, r.bottom - 0.5f};
    g_rt->DrawRoundedRectangle(D2D1::RoundedRect(inner, radius, radius), g_brush, 1);
}

void Text(std::wstring_view s, IDWriteTextFormat* format, const D2D1_RECT_F& r,
          D2D1_COLOR_F c) {
    if (!format) {
        return;
    }
    g_brush->SetColor(c);
    g_rt->DrawText(s.data(), (UINT32)s.size(), format, r, g_brush);
}

void Glyph(wchar_t glyph, IDWriteTextFormat* format, const D2D1_RECT_F& r,
           D2D1_COLOR_F c) {
    Text({&glyph, 1}, format, r, c);
}

////////////////////////////////////////////////////////////////////////////////
// List model

D2D1_SIZE_F ClientSizeDip() {
    RECT rc;
    GetClientRect(g_hwnd, &rc);
    return {rc.right * 96.f / g_dpi, rc.bottom * 96.f / g_dpi};
}

bool Matches(const Item& it) {
    size_t start = 0;
    while (start < g_query.size()) {
        size_t end = g_query.find(L' ', start);
        if (end == std::wstring::npos) {
            end = g_query.size();
        }
        std::wstring word = g_query.substr(start, end - start);
        start = end + 1;
        if (word.empty()) {
            continue;
        }
        if (!ContainsNoCase(it.text, word) && !ContainsNoCase(it.files, word) &&
            !ContainsNoCase(it.source, word) && !ContainsNoCase(it.preview, word)) {
            return false;
        }
    }
    return true;
}

void RebuildRows() {
    g_rows.clear();
    UINT64 today = LocalDay(NowFileTime());
    UINT64 lastDay = 0;
    float y = 0;
    for (int i = 0; i < (int)g_items.size(); i++) {
        const Item& it = g_items[i];
        if (!g_query.empty() && !Matches(it)) {
            continue;
        }
        UINT64 day = LocalDay(it.time);
        if (day != lastDay) {
            lastDay = day;
            g_rows.push_back({-1, FormatDay(it.time, today), y, kHeaderHeight});
            y += kHeaderHeight;
        }
        g_rows.push_back({i, {}, y, kRowHeight});
        y += kRowHeight + kRowGap;
    }
    g_contentHeight = y + 4;
}

int FirstItemRow() {
    for (int i = 0; i < (int)g_rows.size(); i++) {
        if (g_rows[i].item >= 0) {
            return i;
        }
    }
    return -1;
}

int SelectedRow() {
    for (int i = 0; i < (int)g_rows.size(); i++) {
        if (g_rows[i].item >= 0 && g_items[g_rows[i].item].id == g_selectedId) {
            return i;
        }
    }
    return -1;
}

int RowAt(float y) {
    auto it = std::upper_bound(g_rows.begin(), g_rows.end(), y,
                               [](float v, const Row& r) { return v < r.y; });
    if (it == g_rows.begin()) {
        return -1;
    }
    int i = (int)(it - g_rows.begin()) - 1;
    return y < g_rows[i].y + g_rows[i].height ? i : -1;
}

////////////////////////////////////////////////////////////////////////////////
// Layout and hit testing

Layout GetLayout() {
    D2D1_SIZE_F s = ClientSizeDip();
    float W = s.width, H = s.height;
    Layout l{};
    float x = W - 4 - 40;
    l.close = {x, 4, x + 40, 36};
    x -= 40;
    l.pin = {x, 4, x + 40, 36};
    x -= 40;
    l.menu = {x, 4, x + 40, 36};
    x -= 40;
    l.search = {x, 4, x + 40, 36};
    l.searchBox = {40, 5, l.menu.left - 4, 35};
    l.searchClear = {l.searchBox.right - 28, 9, l.searchBox.right - 4, 31};
    l.edit = {l.searchBox.left + 32, 11, l.searchClear.left - 4, 29};
    l.tab = {12, kTitleHeight, 12 + 24 + g_tabWidth,
             kTitleHeight + kTabsHeight};
    l.card = {8, kTitleHeight + kTabsHeight, W - 8, H - 8};
    l.footer = {l.card.left, l.card.bottom - kFooterHeight, l.card.right, l.card.bottom};
    l.list = {l.card.left + 8, l.card.top + 4, l.card.right - 8, l.footer.top};
    l.info = {l.footer.left + 4, l.footer.top + 4, l.footer.left + 40,
              l.footer.bottom - 4};
    float clearWidth = 12 + 14 + 6 + g_clearWidth + 12;
    l.clear = {l.footer.right - 6 - clearWidth, l.footer.top + 5, l.footer.right - 6,
               l.footer.bottom - 5};
    l.undoBar = {l.list.left + 4, l.list.bottom - 52, l.list.right - 4, l.list.bottom - 8};
    float undoWidth = 24 + g_undoWidth;
    l.undo = {l.undoBar.right - 8 - undoWidth, l.undoBar.top + 7, l.undoBar.right - 8,
              l.undoBar.bottom - 7};
    l.scrollbar = {l.list.right - 10, l.list.top + 2, l.list.right, l.list.bottom - 2};
    return l;
}

float ListHeight() {
    Layout l = GetLayout();
    return l.list.bottom - l.list.top;
}

float ScrollMax() {
    return std::max(0.f, g_contentHeight - ListHeight());
}

bool In(const D2D1_RECT_F& r, float x, float y) {
    return x >= r.left && x < r.right && y >= r.top && y < r.bottom;
}

HitInfo HitTest(float x, float y) {
    Layout l = GetLayout();
    if (y < kTitleHeight) {
        if (In(l.close, x, y)) return {Hit::Close};
        if (In(l.pin, x, y)) return {Hit::Pin};
        if (In(l.menu, x, y)) return {Hit::Menu};
        if (g_searchOpen) {
            if (In(l.searchClear, x, y)) return {Hit::SearchClear};
            if (In(l.searchBox, x, y)) return {Hit::SearchBox};
        } else if (In(l.search, x, y)) {
            return {Hit::Search};
        }
        return {Hit::Caption};
    }
    if (g_undoVisible && In(l.undo, x, y)) return {Hit::Undo};
    if (In(l.info, x, y)) return {Hit::Info};
    if (In(l.clear, x, y) && !g_items.empty()) return {Hit::Clear};
    if (In(l.list, x, y)) {
        if (g_undoVisible && In(l.undoBar, x, y)) return {Hit::None};
        if (ScrollMax() > 0 && x >= l.scrollbar.left) return {Hit::Scrollbar};
        int row = RowAt(y - l.list.top + g_scroll);
        if (row >= 0 && g_rows[row].item >= 0) return {Hit::Row, row};
        return {Hit::List};
    }
    return {};
}

////////////////////////////////////////////////////////////////////////////////
// Rendering

void DiscardGraphics() {
    for (auto& [id, bitmap] : g_thumbs) {
        bitmap->Release();
    }
    g_thumbs.clear();
    SafeRelease(g_brush);
    SafeRelease(g_rt);
}

bool EnsureGraphics() {
    if (g_rt) {
        return true;
    }
    RECT rc;
    GetClientRect(g_hwnd, &rc);
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
        (float)g_dpi, (float)g_dpi);
    if (FAILED(g_d2d->CreateHwndRenderTarget(
            props,
            D2D1::HwndRenderTargetProperties(
                g_hwnd, D2D1::SizeU(std::max(1L, rc.right), std::max(1L, rc.bottom))),
            &g_rt)) ||
        FAILED(g_rt->CreateSolidColorBrush(D2D1::ColorF(0), &g_brush))) {
        DiscardGraphics();
        return false;
    }
    // ClearType needs an opaque background; the window is translucent.
    g_rt->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
    return true;
}

ID2D1Bitmap* Thumbnail(const Item& it) {
    if (it.thumb.empty()) {
        return nullptr;
    }
    if (auto found = g_thumbs.find(it.id); found != g_thumbs.end()) {
        return found->second;
    }
    ID2D1Bitmap* bitmap = nullptr;
    g_rt->CreateBitmap(
        D2D1::SizeU(it.thumbWidth, it.thumbHeight), it.thumb.data(), it.thumbWidth * 4,
        D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
                                                 D2D1_ALPHA_MODE_PREMULTIPLIED)),
        &bitmap);
    if (bitmap) {
        g_thumbs[it.id] = bitmap;
    }
    return bitmap;
}

void DrawButton(const D2D1_RECT_F& r, Hit hit, wchar_t glyph, D2D1_COLOR_F color) {
    if (g_hot.what == hit) {
        Fill(r, g_pressed.what == hit ? g_theme.pressed : g_theme.hover, 4);
    }
    Glyph(glyph, g_fonts.icon16, r, color);
}

void DrawItemIcon(const Item& it, const D2D1_RECT_F& r) {
    const Theme& t = g_theme;
    switch (it.type) {
        case ItemType::Image:
            if (ID2D1Bitmap* bitmap = Thumbnail(it)) {
                float w = it.thumbWidth, h = it.thumbHeight;
                float scale = 16 / std::max(w, h);
                float cx = (r.left + r.right) / 2, cy = (r.top + r.bottom) / 2;
                D2D1_RECT_F dst{cx - w * scale / 2, cy - h * scale / 2,
                                cx + w * scale / 2, cy + h * scale / 2};
                g_rt->DrawBitmap(bitmap, dst, 1, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
                return;
            }
            Glyph(kIconImage, g_fonts.icon16, r, t.text2);
            return;
        case ItemType::Color: {
            D2D1_RECT_F swatch{r.left + 1, r.top + 1, r.right - 1, r.bottom - 1};
            Fill(swatch, Rgba(it.color), 3);
            Stroke(swatch, t.controlStroke, 3);
            return;
        }
        case ItemType::Link:
            Glyph(kIconLink, g_fonts.icon16, r, t.text2);
            return;
        case ItemType::Code:
            Glyph(kIconCode, g_fonts.icon16, r, t.text2);
            return;
        case ItemType::Files:
            Glyph(kIconFiles, g_fonts.icon16, r, t.text2);
            return;
        case ItemType::Text:
            Glyph(kIconText, g_fonts.icon16, r, t.text2);
            return;
    }
}

void DrawList(const Layout& l) {
    const Theme& t = g_theme;
    g_rt->PushAxisAlignedClip(l.list, D2D1_ANTIALIAS_MODE_ALIASED);
    float top = l.list.top - g_scroll;
    int selected = SelectedRow();
    int first = (int)(std::upper_bound(g_rows.begin(), g_rows.end(), g_scroll,
                                       [](float v, const Row& r) { return v < r.y; }) -
                      g_rows.begin()) - 1;
    first = std::max(0, first);
    for (int i = first; i < (int)g_rows.size(); i++) {
        const Row& row = g_rows[i];
        float y = top + row.y;
        if (y > l.list.bottom) {
            break;
        }
        D2D1_RECT_F r{l.list.left, y, l.list.right, y + row.height};
        if (row.item < 0) {
            Text(row.header, g_fonts.header, {r.left + 8, r.top + 8, r.right - 8, r.bottom},
                 t.text2);
            continue;
        }
        const Item& it = g_items[row.item];
        bool hot = g_hot.what == Hit::Row && g_hot.row == i;
        if (hot || i == selected) {
            Fill(r, hot && g_pressed == g_hot ? t.pressed : t.hover, 4);
        }
        if (i == selected) {
            Fill({r.left, r.top + 12, r.left + 3, r.bottom - 12}, t.accent, 1.5f);
        }
        DrawItemIcon(it, {r.left + 12, r.top + 12, r.left + 28, r.bottom - 12});
        std::wstring meta = FormatTime(it.time);
        if (!it.source.empty()) {
            meta = it.source + L" · " + meta;
        }
        float metaWidth = std::min(TextWidth(meta, g_fonts.meta), (r.right - r.left) / 3);
        D2D1_RECT_F metaRect{r.right - 8 - metaWidth, r.top, r.right - 8, r.bottom};
        Text(meta, g_fonts.meta, metaRect, t.text2);
        Text(it.preview, g_fonts.row, {r.left + 40, r.top, metaRect.left - 12, r.bottom},
             t.text);
    }

    if (g_rows.empty()) {
        const wchar_t* message =
            !g_loaded              ? L"Wczytywanie historii…"
            : !g_query.empty()     ? L"Brak wyników. Spróbuj innych słów."
            : g_paused             ? L"Zapisywanie jest wstrzymane. Wznowisz je w menu „…”."
                                   : L"Historia jest pusta. Skopiuj coś, a pojawi się tutaj.";
        Text(message, g_fonts.empty, {l.list.left + 24, l.list.top, l.list.right - 24, l.list.bottom},
             t.text2);
    }

    float max = ScrollMax();
    if (max > 0) {
        float listHeight = l.list.bottom - l.list.top;
        float track = l.scrollbar.bottom - l.scrollbar.top;
        float thumb = std::max(24.f, track * listHeight / g_contentHeight);
        float pos = l.scrollbar.top + (track - thumb) * std::clamp(g_scroll / max, 0.f, 1.f);
        bool wide = g_draggingScrollbar || g_hot.what == Hit::Scrollbar;
        float w = wide ? 6 : 2;
        float x = l.scrollbar.right - 3 - w;
        Fill({x, pos, x + w, pos + thumb}, t.strong, w / 2);
    }
    g_rt->PopAxisAlignedClip();
}

void DrawShortcuts() {
    const Theme& t = g_theme;
    static const std::pair<const wchar_t*, const wchar_t*> kKeys[] = {
        {L"Win+V", L"Otwórz lub zamknij"},  {L"Enter", L"Wklej"},
        {L"Shift+Enter", L"Wklej jako zwykły tekst"},
        {L"Ctrl+1…9", L"Szybkie wklejanie"}, {L"Ctrl+F", L"Szukaj"},
        {L"Esc", L"Zamknij wyszukiwanie lub okno"}};
    D2D1_SIZE_F s = ClientSizeDip();
    float h = 12 + 20 + 8 + ARRAYSIZE(kKeys) * 24 + 12;
    D2D1_RECT_F r{12, s.height - 8 - kFooterHeight - 6 - h, 12 + 330,
                  s.height - 8 - kFooterHeight - 6};
    Fill(r, t.flyout, 8);
    Stroke(r, t.controlStroke, 8);
    Text(L"Skróty klawiszowe", g_fonts.smallBold, {r.left + 14, r.top + 12, r.right, r.top + 32},
         t.text);
    float y = r.top + 40;
    for (const auto& [key, label] : kKeys) {
        float kw = TextWidth(key, g_fonts.key) + 12;
        D2D1_RECT_F kr{r.left + 14, y + 3, r.left + 14 + kw, y + 21};
        Fill(kr, t.control, 4);
        Stroke(kr, t.controlStroke, 4);
        Text(key, g_fonts.key, kr, t.text);
        Text(label, g_fonts.small, {r.left + 124, y, r.right - 12, y + 24}, t.text2);
        y += 24;
    }
}

void Render() {
    if (!EnsureGraphics()) {
        return;
    }
    const Theme& t = g_theme;
    Layout l = GetLayout();
    D2D1_SIZE_F s = ClientSizeDip();
    g_rt->BeginDraw();
    g_rt->Clear(D2D1::ColorF(0, 0));
    Fill({0, 0, s.width, s.height}, t.window);

    // Title bar.
    if (g_searchOpen) {
        Glyph(kIconApp, g_fonts.icon16, {12, 12, 28, 28}, t.text);
        Fill(l.searchBox, t.search, 4);
        Stroke(l.searchBox, t.controlStroke, 4);
        Fill({l.searchBox.left + 1, l.searchBox.bottom - 2, l.searchBox.right - 1,
              l.searchBox.bottom},
             t.accent, 1);
        Glyph(kIconSearch, g_fonts.icon14,
              {l.searchBox.left + 8, l.searchBox.top, l.searchBox.left + 28,
               l.searchBox.bottom},
              t.text2);
        if (g_hot.what == Hit::SearchClear) {
            Fill(l.searchClear, t.hover, 3);
        }
        Glyph(kIconCancel, g_fonts.icon12, l.searchClear, t.text2);
    } else {
        Glyph(kIconApp, g_fonts.icon16, {12, 12, 28, 28}, t.text);
        Text(L"Schowek", g_fonts.title, {36, 0, l.search.left, kTitleHeight}, t.text);
        DrawButton(l.search, Hit::Search, kIconSearch, t.text);
    }
    DrawButton(l.menu, Hit::Menu, kIconMore, t.text);
    DrawButton(l.pin, Hit::Pin, g_pinned ? kIconPinned : kIconPin,
               g_pinned ? t.accent : t.text);
    DrawButton(l.close, Hit::Close, kIconClose, t.text);

    // Tabs: only History in this version.
    Text(L"Historia", g_fonts.tab, l.tab, t.text);
    float cx = (l.tab.left + l.tab.right) / 2;
    Fill({cx - 8, l.tab.bottom - 7, cx + 8, l.tab.bottom - 4}, t.accent, 1.5f);

    // Content card.
    Fill(l.card, t.layer, 8);
    Stroke(l.card, t.layerStroke, 8);
    DrawList(l);

    if (g_undoVisible) {
        Fill(l.undoBar, t.flyout, 6);
        Stroke(l.undoBar, t.controlStroke, 6);
        std::wstring message = L"Wyczyszczono " + std::to_wstring(g_undoCount) + L" " +
                               ElementsWord(g_undoCount) + L" historii.";
        Text(message, g_fonts.small,
             {l.undoBar.left + 14, l.undoBar.top, l.undo.left - 8, l.undoBar.bottom}, t.text);
        Fill(l.undo, g_hot.what == Hit::Undo ? t.hover : t.control, 4);
        Stroke(l.undo, t.controlStroke, 4);
        Text(L"Cofnij", g_fonts.small, {l.undo.left + 12, l.undo.top, l.undo.right, l.undo.bottom},
             t.text);
    }

    // Footer.
    Fill({l.footer.left, l.footer.top, l.footer.right, l.footer.top + 1}, t.divider);
    DrawButton(l.info, Hit::Info, kIconInfo, t.text);
    std::wstring count = std::to_wstring(g_items.size()) + L" " + ElementsWord(g_items.size());
    if (!g_query.empty()) {
        count = L"Wyniki: " + std::to_wstring(std::count_if(
                                  g_rows.begin(), g_rows.end(),
                                  [](const Row& r) { return r.item >= 0; })) +
                L" z " + std::to_wstring(g_items.size());
    }
    if (g_paused) {
        count += L" · zapisywanie wstrzymane";
    }
    Text(count, g_fonts.small, {l.info.right + 8, l.footer.top, l.clear.left - 8, l.footer.bottom},
         t.text2);
    if (!g_items.empty()) {
        Fill(l.clear, g_hot.what == Hit::Clear ? t.hover : t.control, 4);
        Stroke(l.clear, t.controlStroke, 4);
        Glyph(kIconDelete, g_fonts.icon14,
              {l.clear.left + 10, l.clear.top, l.clear.left + 26, l.clear.bottom}, t.text);
        Text(L"Wyczyść historię", g_fonts.small,
             {l.clear.left + 32, l.clear.top, l.clear.right, l.clear.bottom}, t.text);
    }

    if (g_hot.what == Hit::Info) {
        DrawShortcuts();
    }

    if (g_rt->EndDraw() == D2DERR_RECREATE_TARGET) {
        DiscardGraphics();
        InvalidateRect(g_hwnd, nullptr, FALSE);
    }
}

void Redraw() {
    InvalidateRect(g_hwnd, nullptr, FALSE);
}

////////////////////////////////////////////////////////////////////////////////
// Scrolling and selection

void ScrollTo(float target) {
    g_scrollTarget = std::clamp(target, 0.f, ScrollMax());
    g_scrollTick = GetTickCount64();
    SetTimer(g_hwnd, kTimerScroll, 10, nullptr);
}

void OnScrollTimer() {
    ULONGLONG now = GetTickCount64();
    float dt = (now - g_scrollTick) / 1000.f;
    g_scrollTick = now;
    float diff = g_scrollTarget - g_scroll;
    if (std::fabs(diff) < 0.5f) {
        g_scroll = g_scrollTarget;
        KillTimer(g_hwnd, kTimerScroll);
    } else {
        g_scroll += diff * std::min(1.f, dt * 18);
    }
    g_hot = g_mouseInside ? HitTest(g_mouse.x, g_mouse.y) : HitInfo{};
    Redraw();
}

void EnsureVisible(int row) {
    if (row < 0) {
        return;
    }
    float listHeight = ListHeight();
    const Row& r = g_rows[row];
    float top = r.y;
    // Keep the day header above the first item of a day visible too.
    if (row > 0 && g_rows[row - 1].item < 0) {
        top = g_rows[row - 1].y;
    }
    if (top < g_scrollTarget) {
        ScrollTo(top);
    } else if (r.y + r.height + 4 > g_scrollTarget + listHeight) {
        ScrollTo(r.y + r.height + 4 - listHeight);
    }
}

void Select(int row) {
    if (row >= 0 && row < (int)g_rows.size() && g_rows[row].item >= 0) {
        g_selectedId = g_items[g_rows[row].item].id;
        EnsureVisible(row);
        Redraw();
    }
}

void MoveSelection(int step) {
    int row = SelectedRow();
    if (row < 0) {
        Select(FirstItemRow());
        return;
    }
    for (int i = row + step; i >= 0 && i < (int)g_rows.size(); i += step) {
        if (g_rows[i].item >= 0) {
            Select(i);
            return;
        }
    }
}

// Rebuilds the list after a change, keeping the selection when possible.
void Refresh() {
    RebuildRows();
    if (SelectedRow() < 0) {
        int first = FirstItemRow();
        g_selectedId = first >= 0 ? g_items[g_rows[first].item].id : 0;
    }
    g_scroll = std::clamp(g_scroll, 0.f, ScrollMax());
    g_scrollTarget = std::clamp(g_scrollTarget, 0.f, ScrollMax());
    Redraw();
}

////////////////////////////////////////////////////////////////////////////////
// Window actions

void LayoutEdit() {
    Layout l = GetLayout();
    float lineHeight = 18;
    float top = (l.searchBox.top + l.searchBox.bottom - lineHeight) / 2;
    SetWindowPos(g_edit, nullptr, Px(l.edit.left), Px(top),
                 Px(l.edit.right) - Px(l.edit.left), Px(top + lineHeight) - Px(top),
                 SWP_NOZORDER | SWP_NOACTIVATE);
}

void UpdateEditFont() {
    HFONT old = g_editFont;
    g_editFont = CreateFontW(-MulDiv(13, g_dpi, 96), 0, 0, 0, FW_NORMAL, 0, 0, 0,
                             DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");
    SendMessageW(g_edit, WM_SETFONT, (WPARAM)g_editFont, TRUE);
    if (old) {
        DeleteObject(old);
    }
}

void OpenSearch() {
    if (!g_searchOpen) {
        g_searchOpen = true;
        LayoutEdit();
        ShowWindow(g_edit, SW_SHOW);
    }
    SetFocus(g_edit);
    SendMessageW(g_edit, EM_SETSEL, 0, -1);
    Redraw();
}

// Doesn't move the focus: it's also used while the window is deactivated.
void ResetSearch() {
    if (!g_searchOpen) {
        return;
    }
    g_searchOpen = false;
    SetWindowTextW(g_edit, L"");
    ShowWindow(g_edit, SW_HIDE);
    if (!g_query.empty()) {
        g_query.clear();
        Refresh();
    }
    Redraw();
}

void CloseSearch() {
    ResetSearch();
    SetFocus(g_hwnd);
}

void OnSearchChanged() {
    int length = GetWindowTextLengthW(g_edit);
    std::wstring query(length, L'\0');
    GetWindowTextW(g_edit, query.data(), length + 1);
    if (query != g_query) {
        g_query = std::move(query);
        g_selectedId = 0;
        g_scroll = g_scrollTarget = 0;
        Refresh();
    }
}

void HidePanel(bool restoreFocus) {
    if (!IsWindowVisible(g_hwnd)) {
        return;
    }
    ResetSearch();
    g_hiddenTick = GetTickCount64();
    ShowWindow(g_hwnd, SW_HIDE);
    if (restoreFocus && g_prevForeground && IsWindow(g_prevForeground)) {
        SetForegroundWindow(g_prevForeground);
    }
}

void SetPinned(bool pinned) {
    g_pinned = pinned;
    SetWindowPos(g_hwnd, pinned ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    Redraw();
}

// Where the text caret of the app being typed in is, in screen coordinates.
// Falls back to the accessibility caret (used by browsers), then to the mouse.
RECT AnchorRect(HWND foreground) {
    GUITHREADINFO gti{sizeof(gti)};
    DWORD thread = foreground ? GetWindowThreadProcessId(foreground, nullptr) : 0;
    if (thread && GetGUIThreadInfo(thread, &gti)) {
        if (gti.hwndCaret && (gti.rcCaret.bottom > gti.rcCaret.top)) {
            RECT rc = gti.rcCaret;
            MapWindowPoints(gti.hwndCaret, nullptr, (POINT*)&rc, 2);
            return rc;
        }
        IAccessible* acc = nullptr;
        HWND focus = gti.hwndFocus ? gti.hwndFocus : foreground;
        if (SUCCEEDED(AccessibleObjectFromWindow(focus, (DWORD)OBJID_CARET, IID_IAccessible,
                                                 (void**)&acc))) {
            VARIANT self{};
            self.vt = VT_I4;
            self.lVal = CHILDID_SELF;
            long x, y, w, h;
            bool ok = SUCCEEDED(acc->accLocation(&x, &y, &w, &h, self)) && (x || y) && h > 0;
            acc->Release();
            if (ok) {
                return {x, y, x + w, y + h};
            }
        }
    }
    POINT pt;
    GetCursorPos(&pt);
    return {pt.x, pt.y, pt.x, pt.y};
}

// Shows the window below the anchor, or above it when there's no room.
void ShowPanel(RECT anchor, bool aboveAnchor) {
    POINT pt{anchor.left, anchor.bottom};
    HMONITOR monitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi{sizeof(mi)};
    GetMonitorInfoW(monitor, &mi);
    UINT dpiX = 96, dpiY = 96;
    GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
    RECT work = mi.rcWork;
    int w = std::min((int)std::lround(g_sizeDip.width * dpiX / 96),
                     (int)(work.right - work.left));
    int h = std::min((int)std::lround(g_sizeDip.height * dpiX / 96),
                     (int)(work.bottom - work.top));
    int gap = MulDiv(6, dpiX, 96);
    int x = anchor.left;
    int y = anchor.bottom + gap;
    if (aboveAnchor || y + h > work.bottom) {
        y = anchor.top - gap - h;
    }
    x = std::clamp(x, (int)work.left, (int)work.right - w);
    y = std::clamp(y, (int)work.top, (int)work.bottom - h);

    ResetSearch();
    g_selectedId = 0;
    g_scroll = g_scrollTarget = 0;
    Refresh();

    g_placing = true;
    SetWindowPos(g_hwnd, g_pinned ? HWND_TOPMOST : HWND_TOP, x, y, w, h,
                 SWP_SHOWWINDOW);
    g_placing = false;
    SetForegroundWindow(g_hwnd);
    SetFocus(g_hwnd);
}

void ShowNearCaret() {
    HWND foreground = GetForegroundWindow();
    if (foreground && foreground != g_hwnd) {
        g_prevForeground = foreground;
    }
    ShowPanel(AnchorRect(g_prevForeground), false);
}

void ShowNearTray(POINT click) {
    RECT anchor{click.x, click.y, click.x, click.y};
    if (click.x == 0 && click.y == 0) {
        MONITORINFO mi{sizeof(mi)};
        GetMonitorInfoW(MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY), &mi);
        anchor = {mi.rcWork.right, mi.rcWork.bottom, mi.rcWork.right, mi.rcWork.bottom};
    }
    // Centered on the icon, just above the taskbar.
    anchor.left -= (LONG)(g_sizeDip.width * g_dpi / 96 / 2);
    ShowPanel(anchor, true);
}

void TogglePanel() {
    if (IsWindowVisible(g_hwnd) && GetForegroundWindow() == g_hwnd) {
        HidePanel(true);
    } else {
        ShowNearCaret();
    }
}

void PasteItem(const Item& it, bool plain) {
    if (!PutOnClipboard(it, plain && !it.imageWidth)) {
        return;
    }
    HWND target = g_prevForeground;
    if (!g_pinned) {
        HidePanel(false);
    }
    if (!target || !IsWindow(target)) {
        return;  // The item is on the clipboard, ready for Ctrl+V.
    }
    SetForegroundWindow(target);
    // ponytail: short blocking wait for the focus switch; the window is
    // already hidden, so nothing visible stalls.
    for (int i = 0; i < 20 && GetForegroundWindow() != target; i++) {
        Sleep(10);
    }
    SendCtrlV();
}

void PasteRow(int row, bool plain) {
    if (row >= 0 && row < (int)g_rows.size() && g_rows[row].item >= 0) {
        g_selectedId = g_items[g_rows[row].item].id;
        PasteItem(g_items[g_rows[row].item], plain);
    }
}

void PasteNth(int n) {
    for (int i = 0; i < (int)g_rows.size(); i++) {
        if (g_rows[i].item >= 0 && n-- == 0) {
            PasteRow(i, false);
            return;
        }
    }
}

void CommitClear() {
    KillTimer(g_hwnd, kTimerUndo);
    std::vector<UINT64> images;
    for (const Item& it : g_cleared) {
        if (it.imageWidth) {
            images.push_back(it.id);
        }
        if (auto thumb = g_thumbs.find(it.id); thumb != g_thumbs.end()) {
            thumb->second->Release();
            g_thumbs.erase(thumb);
        }
    }
    DeleteImagesLater(std::move(images));
    g_cleared.clear();
    if (g_undoVisible) {
        g_undoVisible = false;
        Redraw();
    }
}

void ClearHistory() {
    if (g_items.empty()) {
        return;
    }
    CommitClear();
    g_undoCount = g_items.size();
    g_cleared = std::move(g_items);
    g_items.clear();
    g_undoVisible = true;
    SetTimer(g_hwnd, kTimerUndo, kUndoMs, nullptr);
    Refresh();
    SaveIndex();
}

void UndoClear() {
    KillTimer(g_hwnd, kTimerUndo);
    // Items copied in the meantime are newer, so they stay on top.
    g_items.insert(g_items.end(), std::make_move_iterator(g_cleared.begin()),
                   std::make_move_iterator(g_cleared.end()));
    g_cleared.clear();
    g_undoVisible = false;
    Refresh();
    SaveIndex();
}

void SetPaused(bool paused) {
    g_paused = paused;
    UpdateTrayIcon(NIM_MODIFY);
    Redraw();
}

void ShowMenu(POINT pt, bool tray) {
    HMENU menu = CreatePopupMenu();
    enum { kOpen = 1, kPause, kClear };
    if (tray) {
        AppendMenuW(menu, MF_STRING, kOpen, L"Otwórz schowek\tWin+V");
        SetMenuDefaultItem(menu, kOpen, FALSE);
    }
    AppendMenuW(menu, MF_STRING | (g_paused ? MF_CHECKED : 0), kPause,
                L"Wstrzymaj zapisywanie");
    if (tray) {
        AppendMenuW(menu, MF_STRING | (g_items.empty() ? MF_GRAYED : 0), kClear,
                    L"Wyczyść historię");
    }
    SetForegroundWindow(g_hwnd);
    int cmd = TrackPopupMenu(menu, TPM_RETURNCMD | TPM_RIGHTBUTTON |
                                       (tray ? TPM_BOTTOMALIGN : 0),
                             pt.x, pt.y, 0, g_hwnd, nullptr);
    DestroyMenu(menu);
    PostMessageW(g_hwnd, WM_NULL, 0, 0);
    switch (cmd) {
        case kOpen:
            ShowNearTray(pt);
            break;
        case kPause:
            SetPaused(!g_paused);
            break;
        case kClear:
            // The window shows the "Undo" button.
            ShowNearTray(pt);
            ClearHistory();
            break;
    }
}

void Activate(HitInfo hit) {
    Layout l = GetLayout();
    switch (hit.what) {
        case Hit::Close:
            HidePanel(true);
            break;
        case Hit::Pin:
            SetPinned(!g_pinned);
            break;
        case Hit::Menu: {
            POINT pt{Px(l.menu.left), Px(l.menu.bottom)};
            ClientToScreen(g_hwnd, &pt);
            ShowMenu(pt, false);
            break;
        }
        case Hit::Search:
        case Hit::SearchBox:
            OpenSearch();
            break;
        case Hit::SearchClear:
            CloseSearch();
            break;
        case Hit::Row:
            PasteRow(hit.row, GetKeyState(VK_SHIFT) < 0);
            break;
        case Hit::Clear:
            ClearHistory();
            break;
        case Hit::Undo:
            UndoClear();
            break;
        default:
            break;
    }
}

// Shared by the window and the search field.
bool OnKey(WPARAM vk) {
    bool ctrl = GetKeyState(VK_CONTROL) < 0;
    bool shift = GetKeyState(VK_SHIFT) < 0;
    switch (vk) {
        case VK_UP:
            MoveSelection(-1);
            return true;
        case VK_DOWN:
            MoveSelection(1);
            return true;
        case VK_PRIOR:
        case VK_NEXT: {
            int rows = std::max(1, (int)(ListHeight() / (kRowHeight + kRowGap)) - 1);
            for (int i = 0; i < rows; i++) {
                MoveSelection(vk == VK_NEXT ? 1 : -1);
            }
            return true;
        }
        case VK_RETURN:
            PasteRow(SelectedRow(), shift);
            return true;
        case VK_ESCAPE:
            if (g_searchOpen) {
                CloseSearch();
            } else {
                HidePanel(true);
            }
            return true;
        case 'F':
            if (ctrl) {
                OpenSearch();
                return true;
            }
            return false;
    }
    if (ctrl && vk >= '1' && vk <= '9') {
        PasteNth((int)(vk - '1'));
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
// Search field: a native EDIT, so that typing, selection and the clipboard work
// as everywhere else. It's drawn through a buffered paint with an opaque alpha,
// because GDI can't draw on the translucent window by itself.

LRESULT CALLBACK EditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_KEYDOWN:
            if (wParam == VK_HOME || wParam == VK_END) {
                break;
            }
            if (OnKey(wParam)) {
                return 0;
            }
            break;
        case WM_CHAR:
            // Handled in WM_KEYDOWN; the default handler would only beep.
            if (wParam == VK_RETURN || wParam == VK_ESCAPE || wParam == 0x7F ||
                wParam == 0x06 /* Ctrl+F */) {
                return 0;
            }
            break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rc;
            GetClientRect(hwnd, &rc);
            HDC mem;
            BP_PAINTPARAMS params{sizeof(params)};
            HPAINTBUFFER buffer =
                BeginBufferedPaint(hdc, &rc, BPBF_TOPDOWNDIB, &params, &mem);
            if (buffer) {
                FillRect(mem, &rc, g_editBrush);
                CallWindowProcW(g_editProc, hwnd, WM_PRINTCLIENT, (WPARAM)mem,
                                PRF_CLIENT);
                if (GetWindowTextLengthW(hwnd) == 0) {
                    HGDIOBJ old = SelectObject(mem, g_editFont);
                    SetBkMode(mem, TRANSPARENT);
                    SetTextColor(mem, g_theme.editPlaceholder);
                    RECT text = rc;
                    text.left += LOWORD(SendMessageW(hwnd, EM_GETMARGINS, 0, 0));
                    DrawTextW(mem, L"Szukaj w historii…", -1, &text,
                              DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
                    SelectObject(mem, old);
                }
                BufferedPaintSetAlpha(buffer, nullptr, 255);
                EndBufferedPaint(buffer, TRUE);
            }
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_ERASEBKGND:
            return 1;
    }
    LRESULT result = CallWindowProcW(g_editProc, hwnd, msg, wParam, lParam);
    // The EDIT draws typed text and the selection directly, outside WM_PAINT;
    // repaint through the buffer so that it stays opaque.
    switch (msg) {
        case WM_CHAR:
        case WM_KEYDOWN:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
        case WM_MOUSEMOVE:
        case WM_LBUTTONUP:
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
        case WM_SETTEXT:
        case WM_PASTE:
        case WM_CUT:
        case WM_CLEAR:
        case WM_UNDO:
        case EM_SETSEL:
            if (msg != WM_MOUSEMOVE || (wParam & MK_LBUTTON)) {
                InvalidateRect(hwnd, nullptr, FALSE);
                UpdateWindow(hwnd);
            }
            break;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// Messages from the worker

// The worker loads before it processes any copy, so this comes first.
void OnLoaded(LoadResult* r) {
    g_items = std::move(r->items);
    g_nextId = std::max(g_nextId, r->nextId);
    g_settings = std::move(r->settings);
    delete r;
    g_loaded = true;
    if (std::abs(g_settings[L"boot"] - BootTime()) < 120 * 10000000LL) {
        g_sizeDip = {(float)g_settings[L"width"], (float)g_settings[L"height"]};
    }
    g_sizeDip.width = std::max(g_sizeDip.width, kMinWidth);
    g_sizeDip.height = std::max(g_sizeDip.height, kMinHeight);
    Refresh();
}

void OnCaptured(Captured* r) {
    Item& incoming = r->item;
    // ponytail: linear scan; fine for tens of thousands of items.
    auto same = std::find_if(g_items.begin(), g_items.end(), [&](const Item& it) {
        return memcmp(it.hash, incoming.hash, sizeof(it.hash)) == 0;
    });
    if (same != g_items.end()) {
        same->time = incoming.time;
        if (!incoming.source.empty()) {
            same->source = incoming.source;
        }
        std::rotate(g_items.begin(), same, same + 1);
    } else {
        incoming.id = g_nextId++;
        if (!r->png.empty()) {
            Enqueue([id = incoming.id, png = std::move(r->png)] {
                WriteEncrypted(ImagePath(id), png);
            });
        }
        g_items.insert(g_items.begin(), std::move(incoming));
    }
    delete r;
    Refresh();
    SaveIndex();
}

////////////////////////////////////////////////////////////////////////////////
// Window procedure

void Shutdown() {
    g_quitting = true;
    RemoveClipboardFormatListener(g_hwnd);
    RemoveTrayIcon();
    if (g_hookThread) {
        PostThreadMessageW(g_hookThreadId, WM_QUIT, 0, 0);
        WaitForSingleObject(g_hookThread, 5000);
        CloseHandle(g_hookThread);
        g_hookThread = nullptr;
    }
    if (g_foregroundHook) {
        UnhookWinEvent(g_foregroundHook);
        g_foregroundHook = nullptr;
    }
    SaveIndexNow();
    StopWorker();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == g_taskbarCreatedMsg && g_taskbarCreatedMsg) {
        UpdateTrayIcon(NIM_ADD);
        return 0;
    }
    switch (msg) {
        case WM_NCCALCSIZE:
            if (wParam) {
                return 0;  // The whole window is client area.
            }
            break;

        case WM_NCHITTEST: {
            POINT pt{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            ScreenToClient(hwnd, &pt);
            RECT rc;
            GetClientRect(hwnd, &rc);
            int b = Px(6);
            bool left = pt.x < b, right = pt.x >= rc.right - b;
            bool top = pt.y < b, bottom = pt.y >= rc.bottom - b;
            if (top && left) return HTTOPLEFT;
            if (top && right) return HTTOPRIGHT;
            if (bottom && left) return HTBOTTOMLEFT;
            if (bottom && right) return HTBOTTOMRIGHT;
            if (left) return HTLEFT;
            if (right) return HTRIGHT;
            if (top) return HTTOP;
            if (bottom) return HTBOTTOM;
            float x = pt.x * 96.f / g_dpi, y = pt.y * 96.f / g_dpi;
            return HitTest(x, y).what == Hit::Caption ? HTCAPTION : HTCLIENT;
        }

        case WM_NCACTIVATE:
            // -1: don't paint a non-client area; the window has none.
            return DefWindowProcW(hwnd, msg, wParam, -1);

        case WM_GETMINMAXINFO: {
            auto* mmi = (MINMAXINFO*)lParam;
            mmi->ptMinTrackSize = {Px(kMinWidth), Px(kMinHeight)};
            return 0;
        }

        case WM_SYSCOMMAND:
            switch (wParam & 0xFFF0) {
                case SC_MAXIMIZE:
                case SC_MINIMIZE:
                    return 0;
            }
            break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            Render();
            return 0;
        }

        case WM_ERASEBKGND:
            return 1;

        case WM_SIZE:
            if (g_rt) {
                g_rt->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam)));
            }
            if (g_searchOpen) {
                LayoutEdit();
            }
            g_scroll = std::clamp(g_scroll, 0.f, ScrollMax());
            g_scrollTarget = std::clamp(g_scrollTarget, 0.f, ScrollMax());
            Redraw();
            return 0;

        case WM_EXITSIZEMOVE: {
            // The size is remembered until the computer restarts.
            g_sizeDip = ClientSizeDip();
            g_settings[L"width"] = std::lround(g_sizeDip.width);
            g_settings[L"height"] = std::lround(g_sizeDip.height);
            g_settings[L"boot"] = BootTime();
            SaveSettings();
            return 0;
        }

        case WM_DPICHANGED: {
            g_dpi = HIWORD(wParam);
            if (!g_placing) {
                auto* r = (RECT*)lParam;
                SetWindowPos(hwnd, nullptr, r->left, r->top, r->right - r->left,
                             r->bottom - r->top, SWP_NOZORDER | SWP_NOACTIVATE);
            }
            if (g_rt) {
                g_rt->SetDpi((float)g_dpi, (float)g_dpi);
            }
            UpdateEditFont();
            LayoutEdit();
            Redraw();
            return 0;
        }

        case WM_ACTIVATE:
            if (LOWORD(wParam) == WA_INACTIVE && !g_pinned) {
                // Clicking outside closes the window, like Win+V.
                HidePanel(false);
            }
            break;

        case WM_CLOSE:
            if (!g_quitting) {
                HidePanel(true);
                return 0;
            }
            break;

        case WM_MOUSEMOVE: {
            g_mouse = {GET_X_LPARAM(lParam) * 96.f / g_dpi,
                       GET_Y_LPARAM(lParam) * 96.f / g_dpi};
            if (!g_mouseInside) {
                g_mouseInside = true;
                TRACKMOUSEEVENT tme{sizeof(tme), TME_LEAVE, hwnd};
                TrackMouseEvent(&tme);
            }
            if (g_draggingScrollbar) {
                Layout l = GetLayout();
                float track = l.scrollbar.bottom - l.scrollbar.top;
                float listHeight = l.list.bottom - l.list.top;
                float thumb = std::max(24.f, track * listHeight / g_contentHeight);
                float ratio = (g_mouse.y - g_dragOffset - l.scrollbar.top) /
                              std::max(1.f, track - thumb);
                g_scroll = g_scrollTarget = std::clamp(ratio, 0.f, 1.f) * ScrollMax();
                Redraw();
                return 0;
            }
            HitInfo hot = HitTest(g_mouse.x, g_mouse.y);
            if (!(hot == g_hot)) {
                g_hot = hot;
                Redraw();
            }
            return 0;
        }

        case WM_MOUSELEAVE:
        case WM_NCMOUSEMOVE:
            g_mouseInside = false;
            if (!(g_hot == HitInfo{})) {
                g_hot = {};
                Redraw();
            }
            break;

        case WM_LBUTTONDOWN: {
            float x = GET_X_LPARAM(lParam) * 96.f / g_dpi;
            float y = GET_Y_LPARAM(lParam) * 96.f / g_dpi;
            g_pressed = HitTest(x, y);
            if (g_pressed.what == Hit::Scrollbar) {
                Layout l = GetLayout();
                float track = l.scrollbar.bottom - l.scrollbar.top;
                float listHeight = l.list.bottom - l.list.top;
                float thumb = std::max(24.f, track * listHeight / g_contentHeight);
                float pos = l.scrollbar.top +
                            (track - thumb) * std::clamp(g_scroll / ScrollMax(), 0.f, 1.f);
                // Grabs the thumb where it was clicked, or centers it on a track click.
                g_dragOffset = (y >= pos && y < pos + thumb) ? y - pos : thumb / 2;
                g_draggingScrollbar = true;
                KillTimer(hwnd, kTimerScroll);
            }
            if (!g_searchOpen) {
                SetFocus(hwnd);
            }
            SetCapture(hwnd);
            Redraw();
            return 0;
        }

        case WM_LBUTTONUP: {
            ReleaseCapture();
            float x = GET_X_LPARAM(lParam) * 96.f / g_dpi;
            float y = GET_Y_LPARAM(lParam) * 96.f / g_dpi;
            HitInfo pressed = g_pressed;
            g_pressed = {};
            if (g_draggingScrollbar) {
                g_draggingScrollbar = false;
            } else if (pressed == HitTest(x, y)) {
                Activate(pressed);
            }
            Redraw();
            return 0;
        }

        case WM_MOUSEWHEEL:
            ScrollTo(g_scrollTarget - GET_WHEEL_DELTA_WPARAM(wParam) / 120.f * 3 *
                                          (kRowHeight + kRowGap));
            return 0;

        case WM_KEYDOWN:
            if (OnKey(wParam)) {
                return 0;
            }
            if (wParam == VK_HOME) {
                Select(FirstItemRow());
                return 0;
            }
            if (wParam == VK_END) {
                for (int i = (int)g_rows.size() - 1; i >= 0; i--) {
                    if (g_rows[i].item >= 0) {
                        Select(i);
                        break;
                    }
                }
                return 0;
            }
            break;

        case WM_COMMAND:
            if ((HWND)lParam == g_edit && HIWORD(wParam) == EN_CHANGE) {
                OnSearchChanged();
            }
            return 0;

        case WM_CTLCOLOREDIT: {
            HDC dc = (HDC)wParam;
            SetTextColor(dc, g_theme.editText);
            SetBkColor(dc, g_theme.editBack);
            return (LRESULT)g_editBrush;
        }

        case WM_TIMER:
            switch (wParam) {
                case kTimerScroll:
                    OnScrollTimer();
                    break;
                case kTimerUndo:
                    CommitClear();
                    break;
                case kTimerClipboardRetry:
                    KillTimer(hwnd, kTimerClipboardRetry);
                    OnClipboardUpdate();
                    break;
            }
            return 0;

        case WM_CLIPBOARDUPDATE:
            g_clipboardRetries = 0;
            OnClipboardUpdate();
            return 0;

        case WM_SETTINGCHANGE:
            if (wParam == SPI_SETHIGHCONTRAST ||
                (lParam && wcscmp((const wchar_t*)lParam, L"ImmersiveColorSet") == 0)) {
                LoadTheme();
                ApplyWindowTheme();
                UpdateTrayIcon(NIM_MODIFY);
                InvalidateRect(g_edit, nullptr, TRUE);
                Redraw();
            }
            break;

        case WM_DWMCOLORIZATIONCOLORCHANGED:
        case WM_THEMECHANGED:
            LoadTheme();
            ApplyWindowTheme();
            InvalidateRect(g_edit, nullptr, TRUE);
            Redraw();
            break;

        case WM_APP_TRAY: {
            POINT pt{GET_X_LPARAM(wParam), GET_Y_LPARAM(wParam)};
            switch (LOWORD(lParam)) {
                case NIN_SELECT:
                case NIN_KEYSELECT:
                    // Clicking the icon deactivates and hides the window
                    // first; that click closes it.
                    if (IsWindowVisible(hwnd)) {
                        HidePanel(false);
                    } else if (GetTickCount64() - g_hiddenTick > 300) {
                        ShowNearTray(pt);
                    }
                    break;
                case WM_CONTEXTMENU:
                    ShowMenu(pt, true);
                    break;
            }
            return 0;
        }

        case WM_APP_TOGGLE:
            TogglePanel();
            return 0;

        case WM_APP_LOADED:
            OnLoaded((LoadResult*)lParam);
            return 0;

        case WM_APP_CAPTURED:
            OnCaptured((Captured*)lParam);
            return 0;

        case WM_APP_SAVE: {
            MSG pending;
            // One save for a burst of changes.
            while (PeekMessageW(&pending, hwnd, WM_APP_SAVE, WM_APP_SAVE, PM_REMOVE)) {
            }
            SaveIndexNow();
            return 0;
        }

        case WM_ENDSESSION:
            if (wParam) {
                // Sign-out or shutdown: write everything before the process ends.
                SaveIndexNow();
                SaveSettings();
                StopWorker();
            }
            return 0;

        case WM_APP_QUIT:
            Shutdown();
            DestroyWindow(hwnd);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////
// Win+V: a low-level keyboard hook on its own thread, so that a busy window
// never delays typing.

bool KeyDown(int vk) {
    return GetAsyncKeyState(vk) < 0;
}

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
    if (code == HC_ACTION) {
        const auto* kb = (const KBDLLHOOKSTRUCT*)lParam;
        if (kb->vkCode == 'V' && kb->dwExtraInfo != kInputTag) {
            bool down = wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN;
            if (down && (g_swallowV || ((KeyDown(VK_LWIN) || KeyDown(VK_RWIN)) &&
                                        !KeyDown(VK_CONTROL) && !KeyDown(VK_MENU) &&
                                        !KeyDown(VK_SHIFT)))) {
                if (!g_swallowV) {
                    g_swallowV = true;  // Auto-repeat doesn't toggle again.
                    INPUT inputs[2] = {{INPUT_KEYBOARD}, {INPUT_KEYBOARD}};
                    for (INPUT& in : inputs) {
                        in.ki.wVk = kDummyVk;
                        in.ki.dwExtraInfo = kInputTag;
                    }
                    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
                    SendInput(2, inputs, sizeof(INPUT));
                    PostMessageW(g_hwnd, WM_APP_TOGGLE, 0, 0);
                }
                return 1;
            }
            if (!down && g_swallowV) {
                g_swallowV = false;
                return 1;
            }
        }
    }
    return CallNextHookEx(nullptr, code, wParam, lParam);
}

DWORD WINAPI HookThread(void*) {
    MSG msg;
    PeekMessageW(&msg, nullptr, WM_USER, WM_USER, PM_NOREMOVE);
    HMODULE module = nullptr;
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                           GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                       (LPCWSTR)&KeyboardProc, &module);
    HHOOK hook = SetWindowsHookExW(WH_KEYBOARD_LL, KeyboardProc, module, 0);
    if (!hook) {
        Wh_Log(L"SetWindowsHookEx failed: %u", GetLastError());
    }
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
    }
    if (hook) {
        UnhookWindowsHookEx(hook);
    }
    return 0;
}

// Remembers the last window of another app that was in the foreground: pasting
// goes there when the window is pinned and was clicked.
void CALLBACK ForegroundProc(HWINEVENTHOOK, DWORD, HWND hwnd, LONG idObject, LONG,
                             DWORD, DWORD) {
    if (hwnd && idObject == OBJID_WINDOW) {
        g_prevForeground = hwnd;
    }
}

////////////////////////////////////////////////////////////////////////////////
// UI thread

DWORD WINAPI UiThread(void*) {
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    BufferedPaintInit();

    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_d2d);
    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
                        (IUnknown**)&g_dwrite);
    CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                     IID_PPV_ARGS(&g_wic));
    if (!g_d2d || !g_dwrite || !g_wic) {
        Wh_Log(L"Graphics initialization failed");
        SetEvent(g_uiReady);
        return 1;
    }
    CreateFonts();
    LoadTheme();

    g_cfHtml = RegisterClipboardFormatW(L"HTML Format");
    g_cfRtf = RegisterClipboardFormatW(L"Rich Text Format");
    g_cfPng = RegisterClipboardFormatW(L"PNG");
    g_cfExclude = RegisterClipboardFormatW(L"ExcludeClipboardContentFromMonitorProcessing");
    g_cfCanInclude = RegisterClipboardFormatW(L"CanIncludeInClipboardHistory");
    g_cfViewerIgnore = RegisterClipboardFormatW(L"Clipboard Viewer Ignore");
    g_taskbarCreatedMsg = RegisterWindowMessageW(L"TaskbarCreated");

    WNDCLASSEXW wc{sizeof(wc)};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.lpszClassName = L"ClipboardTweaks_" WH_MOD_VERSION;
    RegisterClassExW(&wc);
    g_hwnd = CreateWindowExW(WS_EX_TOOLWINDOW, wc.lpszClassName, L"Clipboard Tweaks",
                             WS_POPUP | WS_THICKFRAME | WS_CLIPCHILDREN, 0, 0,
                             (int)kDefaultWidth, (int)kDefaultHeight, nullptr, nullptr,
                             wc.hInstance, nullptr);
    if (!g_hwnd) {
        Wh_Log(L"CreateWindow failed: %u", GetLastError());
        SetEvent(g_uiReady);
        return 1;
    }
    g_dpi = GetDpiForWindow(g_hwnd);
    ApplyWindowTheme();
    SetWindowPos(g_hwnd, nullptr, 0, 0, 0, 0,
                 SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOACTIVATE);

    g_edit = CreateWindowExW(0, L"EDIT", nullptr, WS_CHILD | ES_AUTOHSCROLL, 0, 0, 0, 0,
                             g_hwnd, nullptr, wc.hInstance, nullptr);
    g_editProc = (WNDPROC)SetWindowLongPtrW(g_edit, GWLP_WNDPROC, (LONG_PTR)EditProc);
    UpdateEditFont();
    SendMessageW(g_edit, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(0, 0));

    g_workerThread = CreateThread(nullptr, 0, WorkerProc, nullptr, 0, nullptr);
    Enqueue(LoadJob);
    AddClipboardFormatListener(g_hwnd);
    UpdateTrayIcon(NIM_ADD);
    g_foregroundHook =
        SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, nullptr,
                        ForegroundProc, 0, 0,
                        WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
    g_hookThread = CreateThread(nullptr, 0, HookThread, nullptr, 0, &g_hookThreadId);
    SetEvent(g_uiReady);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    DiscardGraphics();
    ReleaseFonts();
    SafeRelease(g_wic);
    SafeRelease(g_dwrite);
    SafeRelease(g_d2d);
    if (g_editFont) {
        DeleteObject(g_editFont);
    }
    if (g_editBrush) {
        DeleteObject(g_editBrush);
    }
    UnregisterClassW(wc.lpszClassName, wc.hInstance);
    BufferedPaintUnInit();
    CoUninitialize();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Mod callbacks

BOOL WhTool_ModInit() {
    WCHAR dir[MAX_PATH];
    if (!ExpandEnvironmentStringsW(L"%LOCALAPPDATA%\\Clipboard Tweaks", dir,
                                   ARRAYSIZE(dir))) {
        return FALSE;
    }
    g_dataDir = dir;
    ApplyWindowsHistorySetting(Wh_GetIntSetting(L"disableWindowsHistory"));

    g_uiReady = CreateEventW(nullptr, TRUE, FALSE, nullptr);
    g_uiThread = CreateThread(nullptr, 0, UiThread, nullptr, 0, nullptr);
    if (!g_uiThread) {
        return FALSE;
    }
    HANDLE handles[] = {g_uiReady, g_uiThread};
    WaitForMultipleObjects(2, handles, FALSE, 10000);
    return g_hwnd != nullptr;
}

void WhTool_ModSettingsChanged() {
    ApplyWindowsHistorySetting(Wh_GetIntSetting(L"disableWindowsHistory"));
}

void WhTool_ModUninit() {
    if (g_hwnd) {
        PostMessageW(g_hwnd, WM_APP_QUIT, 0, 0);
    }
    if (g_uiThread) {
        WaitForSingleObject(g_uiThread, 30000);
    }
    ApplyWindowsHistorySetting(false);
}

////////////////////////////////////////////////////////////////////////////////
// Windhawk tool mod implementation for mods which don't need to inject to other
// processes or hook other functions. Context:
// https://github.com/ramensoftware/windhawk/wiki/Mods-as-tools:-Running-mods-in-a-dedicated-process
//
// The mod will load and run in a dedicated windhawk.exe process.
//
// Paste the code below as part of the mod code, and use these callbacks:
// * WhTool_ModInit
// * WhTool_ModSettingsChanged
// * WhTool_ModUninit
//
// Currently, other callbacks are not supported.

bool g_isToolModProcessLauncher;
HANDLE g_toolModProcessMutex;

void WINAPI EntryPoint_Hook() {
    Wh_Log(L">");
    ExitThread(0);
}

BOOL Wh_ModInit() {
    DWORD sessionId;
    if (ProcessIdToSessionId(GetCurrentProcessId(), &sessionId) &&
        sessionId == 0) {
        return FALSE;
    }

    bool isExcluded = false;
    bool isToolModProcess = false;
    bool isCurrentToolModProcess = false;
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLine(), &argc);
    if (!argv) {
        Wh_Log(L"CommandLineToArgvW failed");
        return FALSE;
    }

    for (int i = 1; i < argc; i++) {
        if (wcscmp(argv[i], L"-service") == 0 ||
            wcscmp(argv[i], L"-service-start") == 0 ||
            wcscmp(argv[i], L"-service-stop") == 0) {
            isExcluded = true;
            break;
        }
    }

    for (int i = 1; i < argc - 1; i++) {
        if (wcscmp(argv[i], L"-tool-mod") == 0) {
            isToolModProcess = true;
            if (wcscmp(argv[i + 1], WH_MOD_ID) == 0) {
                isCurrentToolModProcess = true;
            }
            break;
        }
    }

    LocalFree(argv);

    if (isExcluded) {
        return FALSE;
    }

    if (isCurrentToolModProcess) {
        g_toolModProcessMutex =
            CreateMutex(nullptr, TRUE, L"windhawk-tool-mod_" WH_MOD_ID);
        if (!g_toolModProcessMutex) {
            Wh_Log(L"CreateMutex failed");
            ExitProcess(1);
        }

        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            Wh_Log(L"Tool mod already running (%s)", WH_MOD_ID);
            ExitProcess(1);
        }

        if (!WhTool_ModInit()) {
            ExitProcess(1);
        }

        IMAGE_DOS_HEADER* dosHeader =
            (IMAGE_DOS_HEADER*)GetModuleHandle(nullptr);
        IMAGE_NT_HEADERS* ntHeaders =
            (IMAGE_NT_HEADERS*)((BYTE*)dosHeader + dosHeader->e_lfanew);

        DWORD entryPointRVA = ntHeaders->OptionalHeader.AddressOfEntryPoint;
        void* entryPoint = (BYTE*)dosHeader + entryPointRVA;

        Wh_SetFunctionHook(entryPoint, (void*)EntryPoint_Hook, nullptr);
        return TRUE;
    }

    if (isToolModProcess) {
        return FALSE;
    }

    g_isToolModProcessLauncher = true;
    return TRUE;
}

void Wh_ModAfterInit() {
    if (!g_isToolModProcessLauncher) {
        return;
    }

    WCHAR currentProcessPath[MAX_PATH];
    switch (GetModuleFileName(nullptr, currentProcessPath,
                              ARRAYSIZE(currentProcessPath))) {
        case 0:
        case ARRAYSIZE(currentProcessPath):
            Wh_Log(L"GetModuleFileName failed");
            return;
    }

    WCHAR
    commandLine[MAX_PATH + 2 +
                (sizeof(L" -tool-mod \"" WH_MOD_ID "\"") / sizeof(WCHAR)) - 1];
    swprintf_s(commandLine, L"\"%s\" -tool-mod \"%s\"", currentProcessPath,
               WH_MOD_ID);

    HMODULE kernelModule = GetModuleHandle(L"kernelbase.dll");
    if (!kernelModule) {
        kernelModule = GetModuleHandle(L"kernel32.dll");
        if (!kernelModule) {
            Wh_Log(L"No kernelbase.dll/kernel32.dll");
            return;
        }
    }

    using CreateProcessInternalW_t = BOOL(WINAPI*)(
        HANDLE hUserToken, LPCWSTR lpApplicationName, LPWSTR lpCommandLine,
        LPSECURITY_ATTRIBUTES lpProcessAttributes,
        LPSECURITY_ATTRIBUTES lpThreadAttributes, WINBOOL bInheritHandles,
        DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory,
        LPSTARTUPINFOW lpStartupInfo,
        LPPROCESS_INFORMATION lpProcessInformation,
        PHANDLE hRestrictedUserToken);
    CreateProcessInternalW_t pCreateProcessInternalW =
        (CreateProcessInternalW_t)GetProcAddress(kernelModule,
                                                 "CreateProcessInternalW");
    if (!pCreateProcessInternalW) {
        Wh_Log(L"No CreateProcessInternalW");
        return;
    }

    STARTUPINFO si{
        .cb = sizeof(STARTUPINFO),
        .dwFlags = STARTF_FORCEOFFFEEDBACK,
    };
    PROCESS_INFORMATION pi;
    if (!pCreateProcessInternalW(nullptr, currentProcessPath, commandLine,
                                 nullptr, nullptr, FALSE, NORMAL_PRIORITY_CLASS,
                                 nullptr, nullptr, &si, &pi, nullptr)) {
        Wh_Log(L"CreateProcess failed");
        return;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void Wh_ModSettingsChanged() {
    if (g_isToolModProcessLauncher) {
        return;
    }

    WhTool_ModSettingsChanged();
}

void Wh_ModUninit() {
    if (g_isToolModProcessLauncher) {
        return;
    }

    WhTool_ModUninit();
    ExitProcess(0);
}
