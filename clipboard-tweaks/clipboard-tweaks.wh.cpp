// ==WindhawkMod==
// @id              clipboard-tweaks
// @name            Clipboard Tweaks
// @description     A modern clipboard manager that replaces Win+V: unlimited, encrypted history, pinned items, templates and an image editor
// @description:pl-PL Nowoczesny schowek zamiast Win+V: historia bez limitu, szyfrowana, przypięte, szablony i edytor grafik
// @version         0.7.1
// @author          Mateusz Sobieraj & Claude
// @github          https://github.com/mateuszsobieraj404-lgtm
// @homepage        https://github.com/mateuszsobieraj404-lgtm/Windhawk-mods/tree/main/clipboard-tweaks
// @include         windhawk.exe
// @compilerOptions -ld2d1 -ldwrite -ldwmapi -lwindowscodecs -lbcrypt -lcrypt32 -lole32 -loleaut32 -loleacc -luuid -luxtheme -lshcore -lversion -lgdi32 -lshell32 -lshlwapi
// @license         MIT
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# Clipboard Tweaks

A modern clipboard manager that replaces the Windows Win+V panel with a Fluent
2 window. It keeps everything you copy, without a limit and across restarts,
and removes items only when you tell it to.

> **Pre-release (0.7.1).** All planned features are in. Version 1.0.0 follows
> after a full round of testing.

## Features

- **Replaces Win+V**: Win+V opens Clipboard Tweaks instead of the built-in
  panel. Turn the mod off in Windhawk and the built-in panel comes back. The
  shortcut can be changed in the settings.
- **History without a limit**: text, formatted text (HTML, RTF), links,
  colors, images in their original resolution and copied files. Copying the
  same thing again moves it to the top instead of adding a duplicate.
- **Paste with one click**: click an item or press Enter to paste it into the
  window you were working in. Shift+Enter pastes plain text, and Ctrl+1…9
  pastes the n-th item.
- **Pinned items, organized**: pin anything you use often (Ctrl+P). Pinned
  items are kept until you remove them, and can have a category, #tags, an
  emoji and a color mark. Categories show in a side column or as collapsible
  sections.
- **Templates**: reusable texts with fields filled in when you paste them:
  `{date}` (today's date), `{time}` (the time) and `{clipboard}` (what's on
  the clipboard). The Polish names `{data}`, `{godzina}` and `{schowek}` work
  too. A template can have its own shortcut that pastes it in any app.
- **Item details**: press Space or click the panel icon to see the full
  content, an image preview, paste options, organizing and information about
  the item, in a side panel or in place of the list.
- **Search**: Ctrl+F filters the current tab as you type. Start with `#` to
  search tags.
- **Edit in place**: F2 edits a text item, or opens an image in the editor.
- **Image editor**: brush, highlighter, eraser, text, arrows, lines, circles,
  squares, rectangles, speech bubbles, blur for hiding data, and crop with
  fixed aspect ratios. Undo and redo, zoom from 5% to 1600%, and an image
  viewer. Saving adds a new image and keeps the original as it was.
- **Trash**: deleted items stay in the trash for 30 days and can be restored.
  **Clear history** can be undone for 10 seconds.
- **Settings page**: language, theme, background transparency, details panel
  and pinned layout, automatic cleanup of the history, full resolution images,
  window size and position, and every keyboard shortcut.
- **Move to another computer**: export pinned items, categories, tags,
  templates, settings and optionally the history to one file, protected with
  a password, and import it on the other computer.
- **Window that fits your workflow**: resize it from any edge, and keep it on
  top with the pin button. It follows the Windows light or dark mode and
  accent color, with a translucent acrylic background.
- **English and Polish**: the interface is in Polish on a Polish Windows and
  in English everywhere else. The language can be changed in the settings.
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
| Space | Show or hide the item's details |
| Ctrl+F | Search, also by #tag |
| Ctrl+P | Pin to a category |
| F2 | Edit the item |
| Delete | Move the item to the trash |
| Ctrl+Tab | Next tab |
| Esc | Close the details, the search or the window |

In the image editor, single letters pick a tool: V select, B brush, H
highlighter, E eraser, T text, S shapes, M blur, C crop. Ctrl+Z and Ctrl+Y
undo and redo, Ctrl+S saves, Ctrl+plus and Ctrl+minus zoom, Ctrl+0 fits the
image to the window, and Space with a drag moves the view.

Right-click an item for more actions. The tray icon opens the window with a
left click, and shows **Pause saving**, **Clear history** and **Settings**
with a right click.

## Data and privacy

Everything is stored in `%LOCALAPPDATA%\Clipboard Tweaks`, encrypted with a
key protected by your Windows account, so other users of the computer can't
read it. Removing the mod doesn't delete it. Nothing leaves the computer
unless you export it.

## Settings

- **Turn off Windows clipboard history** (on by default): while the mod runs,
  the built-in Windows clipboard history is turned off so that the two don't
  overlap. Its previous state comes back when the mod is turned off.

Everything else is set on the mod's own settings page: **…** → **Settings**.

## Compatibility

- Windows 11 22H2 or newer.
- Windhawk 1.7.3 or newer.

## Known issues

- Pasting into an app that runs as administrator doesn't work, because Windows
  doesn't let a regular app send keystrokes to it. The item is still put on
  the clipboard, so you can paste it with Ctrl+V.

## Credits

Designed by Mateusz Sobieraj and built together with Claude, an AI assistant
by Anthropic.
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
#include <d2d1_1.h>
#include <dpapi.h>
#include <dwmapi.h>
#include <dwrite.h>
#include <oleacc.h>
#include <shellapi.h>
#include <shellscalingapi.h>
#include <shlobj.h>
#include <shobjidl.h>
#include <shlwapi.h>
#include <uxtheme.h>
#include <wincodec.h>

#include <algorithm>
#include <cmath>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <set>
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
constexpr DWORD kDwmBackdropMica = 2;
constexpr DWORD kDwmBackdropAcrylic = 3;

// Marks keyboard input sent by this mod, so that the hook ignores it.
constexpr ULONG_PTR kInputTag = 0x43547770;
// Unassigned virtual key. Sent after a swallowed Win shortcut, it keeps the
// Start menu closed when Win is released and lets this process take the
// foreground.
constexpr WORD kDummyVk = 0xE8;

enum : UINT {
    WM_APP_TOGGLE = WM_APP + 1,  // The open shortcut, from the hook thread.
    WM_APP_CALL,                 // lParam: std::function<void()>*, from any thread.
    WM_APP_TRAY,                 // Tray icon callback.
    WM_APP_SAVE,                 // Coalesced index save.
    WM_APP_QUIT,                 // Mod unload.
    WM_APP_RECORD,               // A recorded shortcut: wParam vk, lParam modifiers.
};

enum : UINT_PTR {
    kTimerScroll = 1,
    kTimerToast,
    kTimerClipboardRetry,
    kTimerTooltip,
    kTimerHourly,
    kTimerPassword,
    kTimerEditorCaret,
};

// Layout, in DIPs.
constexpr float kTitleHeight = 40;
constexpr float kTabsHeight = 40;
constexpr float kFooterHeight = 40;
constexpr float kRowHeight = 40;
constexpr float kRowGap = 2;
constexpr float kHeaderHeight = 30;
constexpr float kDefaultWidth = 600;
constexpr float kDefaultHeight = 580;
constexpr float kMinWidth = 440;
constexpr float kMinHeight = 360;
constexpr float kPanelWidth = 400;  // Details panel beside the list.
constexpr UINT kToastMs = 10000;
constexpr int kThumbSize = 32;
constexpr UINT64 kDay = 864000000000ULL;  // In FILETIME units.
constexpr UINT64 kTrashDays = 30;
constexpr UINT kMaxImageSide = 1920;  // "Full resolution" off.
constexpr ULONGLONG kPbkdf2Iterations = 600000;

// Segoe Fluent Icons / Segoe MDL2 Assets glyphs.
constexpr wchar_t kIconApp = 0xE77F;
constexpr wchar_t kIconSearch = 0xE721;
constexpr wchar_t kIconMore = 0xE712;
constexpr wchar_t kIconPin = 0xE718;
constexpr wchar_t kIconPinned = 0xE840;
constexpr wchar_t kIconUnpin = 0xE77A;
constexpr wchar_t kIconClose = 0xE8BB;
constexpr wchar_t kIconCancel = 0xE711;
constexpr wchar_t kIconInfo = 0xE946;
constexpr wchar_t kIconDelete = 0xE74D;
constexpr wchar_t kIconText = 0xE8E4;
constexpr wchar_t kIconLink = 0xE71B;
constexpr wchar_t kIconCode = 0xE943;
constexpr wchar_t kIconImage = 0xEB9F;
constexpr wchar_t kIconFiles = 0xE8B7;
constexpr wchar_t kIconFolder = 0xE8B7;
constexpr wchar_t kIconAll = 0xE71D;
constexpr wchar_t kIconNoCategory = 0xE7B8;
constexpr wchar_t kIconSettings = 0xE713;
constexpr wchar_t kIconBack = 0xE72B;
constexpr wchar_t kIconEdit = 0xE70F;
constexpr wchar_t kIconSave = 0xE74E;
constexpr wchar_t kIconOpen = 0xE8A7;
constexpr wchar_t kIconPane = 0xE8A0;
constexpr wchar_t kIconPaste = 0xE77F;
constexpr wchar_t kIconAdd = 0xE710;
constexpr wchar_t kIconChevronDown = 0xE70D;
constexpr wchar_t kIconChevronRight = 0xE76C;
constexpr wchar_t kIconCheck = 0xE73E;
constexpr wchar_t kIconRestore = 0xE7A7;
constexpr wchar_t kIconUndo = 0xE7A7;
constexpr wchar_t kIconRedo = 0xE7A6;
constexpr wchar_t kIconPause = 0xE769;
constexpr wchar_t kIconExport = 0xEDE1;
constexpr wchar_t kIconImport = 0xE8B5;
constexpr wchar_t kIconTemplate = 0xE8A5;
constexpr wchar_t kIconTheme = 0xE790;
constexpr wchar_t kIconLanguage = 0xE774;
constexpr wchar_t kIconTransparency = 0xE706;
constexpr wchar_t kIconLayout = 0xE80A;
constexpr wchar_t kIconClock = 0xE823;
constexpr wchar_t kIconSize = 0xE740;
constexpr wchar_t kIconPosition = 0xE707;
constexpr wchar_t kIconLock = 0xE72E;
constexpr wchar_t kIconMove = 0xE8AB;
constexpr wchar_t kIconKeyboard = 0xE765;
constexpr wchar_t kIconEye = 0xE890;
constexpr wchar_t kIconWarning = 0xE7BA;
constexpr wchar_t kIconEmoji = 0xE76E;
constexpr wchar_t kIconPointer = 0xE7C9;
constexpr wchar_t kIconBrush = 0xE771;
constexpr wchar_t kIconHighlight = 0xE7E6;
constexpr wchar_t kIconEraser = 0xE75C;
constexpr wchar_t kIconFont = 0xE8D2;
constexpr wchar_t kIconCrop = 0xE7A8;
constexpr wchar_t kIconZoomIn = 0xE8A3;
constexpr wchar_t kIconZoomOut = 0xE71F;
constexpr wchar_t kIconBold = 0xE8DD;
constexpr wchar_t kIconItalic = 0xE8DB;
constexpr wchar_t kIconUnderline = 0xE8DC;
constexpr wchar_t kIconMinimize = 0xE921;
constexpr wchar_t kIconMaximize = 0xE922;
constexpr wchar_t kIconRestoreWindow = 0xE923;

////////////////////////////////////////////////////////////////////////////////
// Data model

enum class ItemType : BYTE { Text, Link, Code, Color, Image, Files };
enum class Kind : BYTE { History, Pinned, Template };

struct Item {
    UINT64 id = 0;
    UINT64 time = 0;  // FILETIME, UTC: when copied, or when a template was made.
    Kind kind = Kind::History;
    ItemType type = ItemType::Text;
    std::wstring source;  // Name of the app the item was copied from.
    std::wstring text;    // CF_UNICODETEXT, or a template's content.
    std::string html;     // "HTML Format".
    std::string rtf;      // "Rich Text Format".
    std::wstring files;   // CF_HDROP paths, one per line.
    UINT32 imageWidth = 0;  // 0 when the item has no image.
    UINT32 imageHeight = 0;
    UINT16 thumbWidth = 0;
    UINT16 thumbHeight = 0;
    std::string thumb;  // Premultiplied BGRA pixels.
    BYTE hash[32] = {};
    // Pinned items.
    UINT64 category = 0;  // 0: no category.
    UINT64 pinnedTime = 0;
    std::vector<std::wstring> tags;  // Without "#".
    std::wstring emoji;
    BYTE mark = 0;  // Color mark: 0 none, 1..6 kCategoryColors.
    // Templates.
    std::wstring name;
    UINT32 hotkey = 0;  // Shortcut encoding, see Shortcut keys below.
    // Usage.
    UINT32 pasteCount = 0;
    UINT64 lastPasted = 0;
    // In the trash since this time, or 0.
    UINT64 deletedTime = 0;
    // Derived, not saved.
    std::wstring preview;
    UINT32 color = 0;  // 0xRRGGBB, for ItemType::Color.
};

struct Category {
    UINT64 id = 0;
    std::wstring name;
    std::wstring emoji;
    BYTE color = 0;  // 0 none, 1..6 kCategoryColors.
    bool collapsed = false;  // "Collapsible sections" layout.
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
    UINT maxSide = 0;  // Scale larger images down to this, 0 keeps them.
};

struct LoadResult {
    std::vector<Item> items;
    std::vector<Category> categories;
    std::map<std::wstring, INT64> settings;
    UINT64 nextId = 1;
};

// The interface is Polish on a Polish Windows and English everywhere else, unless the
// "language" setting picks one (see ApplyLook).
bool g_english = PRIMARYLANGID(GetUserDefaultUILanguage()) != LANG_POLISH;

const wchar_t* Tr(const wchar_t* pl, const wchar_t* en) {
    return g_english ? en : pl;
}

// A text in both languages, for tables.
struct Bilingual {
    const wchar_t* pl;
    const wchar_t* en;
    operator const wchar_t*() const { return Tr(pl, en); }
};

// Template fields, filled in when pasting. Both names of a field work in any language.
constexpr Bilingual kTemplateFields[] = {
    {L"{data}", L"{date}"}, {L"{godzina}", L"{time}"}, {L"{schowek}", L"{clipboard}"}};

// Category colors: dark mode, light mode, name.
struct ColorDef {
    UINT32 dark, light;
    Bilingual name;
};
constexpr ColorDef kCategoryColors[] = {
    {0, 0, {L"Bez koloru", L"No color"}},
    {0x62ABF5, 0x0F6CBD, {L"Niebieski", L"Blue"}},
    {0x6CCB5F, 0x107C10, {L"Zielony", L"Green"}},
    {0xF7A35C, 0xCA5010, {L"Pomarańczowy", L"Orange"}},
    {0xC19AF5, 0x8764B8, {L"Fioletowy", L"Purple"}},
    {0xEE88C0, 0xC239B3, {L"Różowy", L"Pink"}},
    {0x9A9A9A, 0x7A7A7A, {L"Szary", L"Gray"}},
};
constexpr int kColorCount = 7;

const wchar_t* const kEmojis[] = {
    L"💼", L"🏠", L"🧩", L"📦", L"🧾", L"📧", L"🔑", L"📌", L"⭐", L"❤️", L"🛒",
    L"🎮", L"📚", L"✈️", L"💡", L"✍️", L"✅", L"📅", L"📝", L"🐞", L"🔥", L"❗",
    L"🎨", L"🔗", L"💬", L"📞", L"💰", L"🎵", L"🍕", L"🚗", L"🌍", L"😀",
};

////////////////////////////////////////////////////////////////////////////////
// Shortcut keys. A key is the virtual key in the low word and MOD_* flags in
// the high word; kKeyOff marks a turned off shortcut.

constexpr UINT kKeyOff = 0x80000000;

constexpr UINT Key(UINT mods, UINT vk) {
    return mods << 16 | vk;
}

enum Shortcut {
    ScOpen,
    ScPaste,
    ScPastePlain,
    ScDetails,
    ScSearch,
    ScPin,
    ScNth,
    ScDelete,
    ScEdit,
    ScNextTab,
    ScClose,
    ScEdSelect,
    ScEdBrush,
    ScEdHighlighter,
    ScEdEraser,
    ScEdText,
    ScEdShape,
    ScEdBlur,
    ScEdCrop,
    ScEdUndo,
    ScEdRedo,
    ScEdSave,
    ScEdZoomIn,
    ScEdZoomOut,
    ScEdFit,
    ScEdActual,
    ScEdBold,
    ScEdItalic,
    ScEdUnderline,
    ScEdDelete,
    ScCount,
};

enum ShortcutScope : BYTE { kScopeGlobal, kScopeWindow, kScopeEditor };

struct ShortcutDef {
    const wchar_t* id;
    Bilingual label;
    UINT def;
    ShortcutScope scope;
};

constexpr ShortcutDef kShortcuts[ScCount] = {
    {L"open",
     {L"Otwórz lub zamknij okno", L"Open or close the window"},
     Key(MOD_WIN, 'V'),
     kScopeGlobal},
    {L"paste", {L"Wklej", L"Paste"}, Key(0, VK_RETURN), kScopeWindow},
    {L"pastePlain",
     {L"Wklej jako zwykły tekst", L"Paste as plain text"},
     Key(MOD_SHIFT, VK_RETURN),
     kScopeWindow},
    {L"details", {L"Szczegóły elementu", L"Item details"}, Key(0, VK_SPACE), kScopeWindow},
    {L"search",
     {L"Szukaj, także po #tagu", L"Search, also by #tag"},
     Key(MOD_CONTROL, 'F'),
     kScopeWindow},
    {L"pin", {L"Przypnij do kategorii", L"Pin to a category"}, Key(MOD_CONTROL, 'P'), kScopeWindow},
    {L"nth", {L"Wklej n-ty element", L"Paste the n-th item"}, Key(MOD_CONTROL, '1'), kScopeWindow},
    {L"delete",
     {L"Usuń element do kosza", L"Move the item to the trash"},
     Key(0, VK_DELETE),
     kScopeWindow},
    {L"edit", {L"Edytuj element", L"Edit the item"}, Key(0, VK_F2), kScopeWindow},
    {L"nextTab", {L"Następna zakładka", L"Next tab"}, Key(MOD_CONTROL, VK_TAB), kScopeWindow},
    {L"close",
     {L"Zamknij szczegóły, wyszukiwanie lub okno", L"Close the details, the search or the window"},
     Key(0, VK_ESCAPE),
     kScopeWindow},
    {L"edSelect", {L"Zaznacz i przesuń", L"Select and move"}, Key(0, 'V'), kScopeEditor},
    {L"edBrush", {L"Pędzel", L"Brush"}, Key(0, 'B'), kScopeEditor},
    {L"edHighlighter", {L"Zakreślacz", L"Highlighter"}, Key(0, 'H'), kScopeEditor},
    {L"edEraser", {L"Gumka", L"Eraser"}, Key(0, 'E'), kScopeEditor},
    {L"edText", {L"Tekst", L"Text"}, Key(0, 'T'), kScopeEditor},
    {L"edShape", {L"Kształty", L"Shapes"}, Key(0, 'S'), kScopeEditor},
    {L"edBlur", {L"Zamazywanie", L"Blur"}, Key(0, 'M'), kScopeEditor},
    {L"edCrop", {L"Kadrowanie", L"Crop"}, Key(0, 'C'), kScopeEditor},
    {L"edUndo", {L"Cofnij", L"Undo"}, Key(MOD_CONTROL, 'Z'), kScopeEditor},
    {L"edRedo", {L"Ponów", L"Redo"}, Key(MOD_CONTROL, 'Y'), kScopeEditor},
    {L"edSave", {L"Zapisz jako nowy", L"Save as new"}, Key(MOD_CONTROL, 'S'), kScopeEditor},
    {L"edZoomIn", {L"Powiększ", L"Zoom in"}, Key(MOD_CONTROL, VK_OEM_PLUS), kScopeEditor},
    {L"edZoomOut", {L"Pomniejsz", L"Zoom out"}, Key(MOD_CONTROL, VK_OEM_MINUS), kScopeEditor},
    {L"edFit", {L"Dopasuj do okna", L"Fit to window"}, Key(MOD_CONTROL, '0'), kScopeEditor},
    {L"edActual", {L"Rozmiar 100%", L"Actual size"}, Key(MOD_CONTROL, '1'), kScopeEditor},
    {L"edBold", {L"Pogrubienie tekstu", L"Bold text"}, Key(MOD_CONTROL, 'B'), kScopeEditor},
    {L"edItalic", {L"Kursywa tekstu", L"Italic text"}, Key(MOD_CONTROL, 'I'), kScopeEditor},
    {L"edUnderline",
     {L"Podkreślenie tekstu", L"Underline text"},
     Key(MOD_CONTROL, 'U'),
     kScopeEditor},
    {L"edDelete",
     {L"Usuń zaznaczony obiekt", L"Delete the selected object"},
     Key(0, VK_DELETE),
     kScopeEditor},
};

////////////////////////////////////////////////////////////////////////////////
// Forward declarations

void FinishItem(Item& it);
void Redraw();
void RedrawEditor();
void SaveIndex();
void SaveSettings();
void Refresh();
INT64 Setting(const wchar_t* key, INT64 def);
void SetSetting(const wchar_t* key, INT64 value);

////////////////////////////////////////////////////////////////////////////////
// Globals

std::wstring g_dataDir;
std::string g_masterKey;  // Set by the worker before the data loads.

HANDLE g_uiThread;
HANDLE g_uiReady;
HANDLE g_hookThread;
DWORD g_hookThreadId;
HANDLE g_workerThread;
HWINEVENTHOOK g_foregroundHook;

HWND g_hwnd;
UINT g_taskbarCreatedMsg;
bool g_quitting;

// Data, UI thread only.
std::vector<Item> g_items;  // All items, in no particular order; views sort them.
std::vector<Category> g_categories;
std::map<std::wstring, INT64> g_settings;
UINT64 g_nextId = 1;
bool g_loaded;
bool g_paused;
DWORD g_ownClipboardSeq;
int g_clipboardRetries;
UINT g_cfHtml, g_cfRtf, g_cfPng, g_cfExclude, g_cfCanInclude, g_cfViewerIgnore;
HWND g_prevForeground;
UINT64 g_storageBytes;  // Size of the data folder, for the settings page.

// Shared with the hook thread.
volatile UINT g_openKey = Key(MOD_WIN, 'V');
volatile bool g_recordingKey;

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

std::string ToUtf8(const std::wstring& s) {
    if (s.empty()) {
        return {};
    }
    int n = WideCharToMultiByte(CP_UTF8, 0, s.data(), (int)s.size(), nullptr, 0,
                                nullptr, nullptr);
    std::string out(n, '\0');
    WideCharToMultiByte(CP_UTF8, 0, s.data(), (int)s.size(), out.data(), n, nullptr,
                        nullptr);
    return out;
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

bool SameBoot(INT64 saved) {
    return std::abs(saved - BootTime()) < 120 * 10000000LL;
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
    return ((UINT64)local.dwHighDateTime << 32 | local.dwLowDateTime) / kDay;
}

std::wstring FormatTime(UINT64 fileTime) {
    SYSTEMTIME st = LocalTime(fileTime);
    WCHAR buf[64];
    if (!GetTimeFormatEx(LOCALE_NAME_USER_DEFAULT, TIME_NOSECONDS, &st, nullptr, buf,
                         ARRAYSIZE(buf))) {
        return {};
    }
    return buf;
}

std::wstring FormatDatePattern(UINT64 fileTime, const wchar_t* pattern) {
    SYSTEMTIME st = LocalTime(fileTime);
    WCHAR buf[64];
    // Month names in the interface language, not the system one.
    if (!GetDateFormatEx(Tr(L"pl-PL", L"en-US"), 0, &st, pattern, buf, ARRAYSIZE(buf), nullptr)) {
        return {};
    }
    return buf;
}

// "Dzisiaj", "Wczoraj", "2 września" or "2 września 2025".
std::wstring FormatDay(UINT64 fileTime) {
    UINT64 day = LocalDay(fileTime), today = LocalDay(NowFileTime());
    if (day == today) {
        return Tr(L"Dzisiaj", L"Today");
    }
    if (day + 1 == today) {
        return Tr(L"Wczoraj", L"Yesterday");
    }
    bool thisYear = LocalTime(fileTime).wYear == LocalTime(NowFileTime()).wYear;
    return FormatDatePattern(
        fileTime, thisYear ? Tr(L"d MMMM", L"MMMM d") : Tr(L"d MMMM yyyy", L"MMMM d, yyyy"));
}

// "dziś 09:12", "wczoraj 21:30" or "2 wrz 2026, 10:14".
std::wstring FormatWhen(UINT64 fileTime) {
    UINT64 day = LocalDay(fileTime), today = LocalDay(NowFileTime());
    if (day == today) {
        return Tr(L"dziś ", L"today ") + FormatTime(fileTime);
    }
    if (day + 1 == today) {
        return Tr(L"wczoraj ", L"yesterday ") + FormatTime(fileTime);
    }
    return FormatDatePattern(fileTime, Tr(L"d MMMM yyyy", L"MMMM d, yyyy")) + L", " +
           FormatTime(fileTime);
}

// "2 wrz", or the time for today.
std::wstring FormatShort(UINT64 fileTime) {
    if (LocalDay(fileTime) == LocalDay(NowFileTime())) {
        return FormatTime(fileTime);
    }
    bool thisYear = LocalTime(fileTime).wYear == LocalTime(NowFileTime()).wYear;
    return FormatDatePattern(fileTime,
                             thisYear ? Tr(L"d MMM", L"MMM d") : Tr(L"d MMM yyyy", L"MMM d, yyyy"));
}

// Polish plural: 1 one, 2-4 few (but not 12-14), others many.
const wchar_t* Plural(size_t n, const wchar_t* one, const wchar_t* few,
                      const wchar_t* many) {
    size_t ones = n % 10, tens = n % 100;
    if (n == 1) {
        return one;
    }
    if (ones >= 2 && ones <= 4 && !(tens >= 12 && tens <= 14)) {
        return few;
    }
    return many;
}

std::wstring Count(size_t n, const wchar_t* one, const wchar_t* few, const wchar_t* many) {
    return std::to_wstring(n) + L" " + Plural(n, one, few, many);
}

std::wstring FormatSize(UINT64 bytes) {
    WCHAR buf[64];
    if (bytes < 1024) {
        swprintf_s(buf, L"%llu B", bytes);
    } else if (bytes < 1024 * 1024) {
        swprintf_s(buf, L"%.0f KB", bytes / 1024.0);
    } else if (bytes < 1024ULL * 1024 * 1024) {
        swprintf_s(buf, L"%.1f MB", bytes / (1024.0 * 1024));
    } else {
        swprintf_s(buf, L"%.1f GB", bytes / (1024.0 * 1024 * 1024));
    }
    std::wstring s = buf;
    std::replace(s.begin(), s.end(), L'.', L',');
    return s;
}

bool ContainsNoCase(const std::wstring& text, const std::wstring& word) {
    return !text.empty() &&
           FindNLSStringEx(LOCALE_NAME_USER_DEFAULT,
                           FIND_FROMSTART | LINGUISTIC_IGNORECASE |
                               LINGUISTIC_IGNOREDIACRITIC,
                           text.c_str(), (int)text.size(), word.c_str(),
                           (int)word.size(), nullptr, nullptr, nullptr, 0) >= 0;
}

bool EqualsNoCase(const std::wstring& a, const std::wstring& b) {
    return CompareStringEx(LOCALE_NAME_USER_DEFAULT, LINGUISTIC_IGNORECASE, a.c_str(),
                           (int)a.size(), b.c_str(), (int)b.size(), nullptr, nullptr,
                           0) == CSTR_EQUAL;
}

bool LessNoCase(const std::wstring& a, const std::wstring& b) {
    return CompareStringEx(LOCALE_NAME_USER_DEFAULT, LINGUISTIC_IGNORECASE, a.c_str(),
                           (int)a.size(), b.c_str(), (int)b.size(), nullptr, nullptr,
                           0) == CSTR_LESS_THAN;
}

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

DWORD RegDword(HKEY root, const wchar_t* key, const wchar_t* name, DWORD def) {
    DWORD value, size = sizeof(value);
    return RegGetValueW(root, key, name, RRF_RT_REG_DWORD, nullptr, &value, &size) ==
                   ERROR_SUCCESS
               ? value
               : def;
}

template <class T>
void SafeRelease(T*& p) {
    if (p) {
        p->Release();
        p = nullptr;
    }
}

INT64 Setting(const wchar_t* key, INT64 def) {
    auto found = g_settings.find(key);
    return found == g_settings.end() ? def : found->second;
}

void SetSetting(const wchar_t* key, INT64 value) {
    g_settings[key] = value;
    SaveSettings();
}

Item* FindItem(UINT64 id) {
    for (Item& it : g_items) {
        if (it.id == id) {
            return &it;
        }
    }
    return nullptr;
}

Category* FindCategory(UINT64 id) {
    for (Category& c : g_categories) {
        if (c.id == id) {
            return &c;
        }
    }
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
// Encryption: a random master key protected with DPAPI (the Windows account),
// and AES-256-GCM for every data file. Export files use a key derived from a
// password with PBKDF2-SHA256.

constexpr char kFileMagic[4] = {'C', 'T', 'W', '1'};
constexpr size_t kNonceSize = 12;
constexpr size_t kTagSize = 16;
constexpr size_t kHeaderSize = sizeof(kFileMagic) + kNonceSize + kTagSize;

bool AesGcm(const std::string& keyBytes, bool encrypt, BYTE* nonce, BYTE* tag,
            const BYTE* in, BYTE* out, ULONG size) {
    BCRYPT_KEY_HANDLE key;
    if (BCryptGenerateSymmetricKey(BCRYPT_AES_GCM_ALG_HANDLE, &key, nullptr, 0,
                                   (PUCHAR)keyBytes.data(), (ULONG)keyBytes.size(),
                                   0) < 0) {
        return false;
    }
    BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO info;
    BCRYPT_INIT_AUTH_MODE_INFO(info);
    info.pbNonce = nonce;
    info.cbNonce = kNonceSize;
    info.pbTag = tag;
    info.cbTag = kTagSize;
    ULONG done = 0;
    NTSTATUS status = encrypt ? BCryptEncrypt(key, (PUCHAR)in, size, &info, nullptr, 0,
                                              out, size, &done, 0)
                              : BCryptDecrypt(key, (PUCHAR)in, size, &info, nullptr, 0,
                                              out, size, &done, 0);
    BCryptDestroyKey(key);
    return status >= 0;
}

// Magic, nonce, tag, then the ciphertext.
std::string EncryptWith(const std::string& key, std::string_view plain) {
    std::string out(kHeaderSize + plain.size(), '\0');
    BYTE* p = (BYTE*)out.data();
    memcpy(p, kFileMagic, sizeof(kFileMagic));
    BYTE* nonce = p + sizeof(kFileMagic);
    if (BCryptGenRandom(nullptr, nonce, kNonceSize, BCRYPT_USE_SYSTEM_PREFERRED_RNG) < 0 ||
        !AesGcm(key, true, nonce, nonce + kNonceSize, (const BYTE*)plain.data(),
                p + kHeaderSize, (ULONG)plain.size())) {
        return {};
    }
    return out;
}

bool DecryptWith(const std::string& key, std::string_view data, std::string* plain) {
    if (data.size() < kHeaderSize || memcmp(data.data(), kFileMagic, sizeof(kFileMagic)) != 0) {
        return false;
    }
    BYTE* p = (BYTE*)data.data();
    plain->resize(data.size() - kHeaderSize);
    return AesGcm(key, false, p + sizeof(kFileMagic), p + sizeof(kFileMagic) + kNonceSize,
                  p + kHeaderSize, (BYTE*)plain->data(), (ULONG)plain->size());
}

std::string Encrypt(std::string_view plain) {
    return EncryptWith(g_masterKey, plain);
}

bool Decrypt(const std::string& data, std::string* plain) {
    return DecryptWith(g_masterKey, data, plain);
}

std::string DeriveKey(const std::wstring& password, const std::string& salt,
                      ULONGLONG iterations) {
    BCRYPT_ALG_HANDLE alg = nullptr;
    std::string key(32, '\0');
    std::string pw = ToUtf8(password);
    bool ok = BCryptOpenAlgorithmProvider(&alg, BCRYPT_SHA256_ALGORITHM, nullptr,
                                          BCRYPT_ALG_HANDLE_HMAC_FLAG) >= 0 &&
              BCryptDeriveKeyPBKDF2(alg, (PUCHAR)pw.data(), (ULONG)pw.size(),
                                    (PUCHAR)salt.data(), (ULONG)salt.size(), iterations,
                                    (PUCHAR)key.data(), (ULONG)key.size(), 0) >= 0;
    if (alg) {
        BCryptCloseAlgorithmProvider(alg, 0);
    }
    SecureZeroMemory(pw.data(), pw.size());
    return ok ? key : std::string();
}

void Sha256(std::initializer_list<std::string_view> parts, BYTE out[32]) {
    BCRYPT_HASH_HANDLE hash;
    if (BCryptCreateHash(BCRYPT_SHA256_ALG_HANDLE, &hash, nullptr, 0, nullptr, 0, 0) < 0) {
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

void HashItem(Item& it, std::string_view png) {
    Sha256({Bytes(it.text), it.html, it.rtf, Bytes(it.files), png}, it.hash);
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
    HANDLE file = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
                              OPEN_EXISTING, 0, nullptr);
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

UINT64 FileSize(const std::wstring& path) {
    WIN32_FILE_ATTRIBUTE_DATA fad;
    if (!GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &fad)) {
        return 0;
    }
    return (UINT64)fad.nFileSizeHigh << 32 | fad.nFileSizeLow;
}

// Writes a temporary file, then swaps it in. The previous version stays as
// "<path>.bak", so a power cut never leaves a half-written file behind.
bool WriteFileAtomic(const std::wstring& path, const std::string& data) {
    std::wstring temp = path + L".tmp";
    HANDLE file = CreateFileW(temp.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) {
        return false;
    }
    DWORD written = 0;
    bool ok = WriteFile(file, data.data(), (DWORD)data.size(), &written, nullptr) &&
              written == data.size() && FlushFileBuffers(file);
    CloseHandle(file);
    if (ok) {
        ok = GetFileAttributesW(path.c_str()) != INVALID_FILE_ATTRIBUTES
                 ? ReplaceFileW(path.c_str(), temp.c_str(), (path + L".bak").c_str(),
                                REPLACEFILE_IGNORE_MERGE_ERRORS, nullptr, nullptr)
                 : MoveFileExW(temp.c_str(), path.c_str(), MOVEFILE_WRITE_THROUGH);
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
        std::wstring target = candidate + L".unreadable-" + std::to_wstring(NowFileTime());
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

// Copies the data files to a dated folder, before an import replaces them.
void BackupData() {
    std::wstring dir = g_dataDir + Tr(L"\\Kopia ", L"\\Backup ") +
                       FormatDatePattern(NowFileTime(), L"yyyy-MM-dd") + L" " +
                       std::to_wstring(NowFileTime() / 10000000 % 100000);
    CreateDirectoryW(dir.c_str(), nullptr);
    CreateDirectoryW((dir + L"\\images").c_str(), nullptr);
    for (const wchar_t* name : {L"index.dat", L"settings.dat"}) {
        CopyFileW(DataPath(name).c_str(), (dir + L"\\" + name).c_str(), FALSE);
    }
    WIN32_FIND_DATAW fd;
    HANDLE find = FindFirstFileW((g_dataDir + L"\\images\\*.bin").c_str(), &fd);
    if (find != INVALID_HANDLE_VALUE) {
        do {
            CopyFileW((g_dataDir + L"\\images\\" + fd.cFileName).c_str(),
                      (dir + L"\\images\\" + fd.cFileName).c_str(), FALSE);
        } while (FindNextFileW(find, &fd));
        FindClose(find);
    }
}

UINT64 FolderSize(const std::wstring& dir) {
    UINT64 total = 0;
    WIN32_FIND_DATAW fd;
    HANDLE find = FindFirstFileW((dir + L"\\*").c_str(), &fd);
    if (find == INVALID_HANDLE_VALUE) {
        return 0;
    }
    do {
        if (wcscmp(fd.cFileName, L".") == 0 || wcscmp(fd.cFileName, L"..") == 0) {
            continue;
        }
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            total += FolderSize(dir + L"\\" + fd.cFileName);
        } else {
            total += (UINT64)fd.nFileSizeHigh << 32 | fd.nFileSizeLow;
        }
    } while (FindNextFileW(find, &fd));
    FindClose(find);
    return total;
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
    explicit Reader(std::string_view s) : p(s.data()), end(s.data() + s.size()) {}
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
constexpr UINT32 kIndexVersion = 2;
constexpr char kSettingsMagic[4] = {'C', 'T', 'S', 'T'};

void WriteItem(Writer& w, const Item& it) {
    w.Num<UINT64>(it.id);
    w.Num<UINT64>(it.time);
    w.Num<BYTE>((BYTE)it.kind);
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
    w.Num<UINT64>(it.category);
    w.Num<UINT64>(it.pinnedTime);
    w.Num<UINT32>((UINT32)it.tags.size());
    for (const std::wstring& tag : it.tags) {
        w.WStr(tag);
    }
    w.WStr(it.emoji);
    w.Num<BYTE>(it.mark);
    w.WStr(it.name);
    w.Num<UINT32>(it.hotkey);
    w.Num<UINT32>(it.pasteCount);
    w.Num<UINT64>(it.lastPasted);
    w.Num<UINT64>(it.deletedTime);
}

// Version 1 (0.1.0) had history items only, without the fields after the hash.
bool ReadItem(Reader& rd, Item& it, UINT32 version) {
    it.id = rd.Num<UINT64>();
    it.time = rd.Num<UINT64>();
    if (version >= 2) {
        BYTE kind = rd.Num<BYTE>();
        it.kind = kind <= (BYTE)Kind::Template ? (Kind)kind : Kind::History;
    }
    BYTE type = rd.Num<BYTE>();
    it.type = type <= (BYTE)ItemType::Files ? (ItemType)type : ItemType::Text;
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
    if (version >= 2) {
        it.category = rd.Num<UINT64>();
        it.pinnedTime = rd.Num<UINT64>();
        UINT32 tags = rd.Num<UINT32>();
        for (UINT32 i = 0; i < tags && rd.ok; i++) {
            it.tags.push_back(rd.WStr());
        }
        it.emoji = rd.WStr();
        it.mark = std::min<BYTE>(rd.Num<BYTE>(), kColorCount - 1);
        it.name = rd.WStr();
        it.hotkey = rd.Num<UINT32>();
        it.pasteCount = rd.Num<UINT32>();
        it.lastPasted = rd.Num<UINT64>();
        it.deletedTime = rd.Num<UINT64>();
    }
    if (it.thumb.size() != (size_t)it.thumbWidth * it.thumbHeight * 4) {
        it.thumb.clear();
        it.thumbWidth = it.thumbHeight = 0;
    }
    if (rd.ok) {
        FinishItem(it);
    }
    return rd.ok;
}

void WriteCategory(Writer& w, const Category& c) {
    w.Num<UINT64>(c.id);
    w.WStr(c.name);
    w.WStr(c.emoji);
    w.Num<BYTE>(c.color);
    w.Num<BYTE>(c.collapsed);
}

bool ReadCategory(Reader& rd, Category& c) {
    c.id = rd.Num<UINT64>();
    c.name = rd.WStr();
    c.emoji = rd.WStr();
    c.color = std::min<BYTE>(rd.Num<BYTE>(), kColorCount - 1);
    c.collapsed = rd.Num<BYTE>() != 0;
    return rd.ok;
}

std::string SerializeIndex() {
    Writer w;
    w.Raw(kIndexMagic, 4);
    w.Num<UINT32>(kIndexVersion);
    w.Num<UINT64>(g_nextId);
    w.Num<UINT32>((UINT32)g_categories.size());
    for (const Category& c : g_categories) {
        WriteCategory(w, c);
    }
    w.Num<UINT32>((UINT32)g_items.size());
    for (const Item& it : g_items) {
        WriteItem(w, it);
    }
    return std::move(w.s);
}

bool ParseIndex(const std::string& data, LoadResult* r) {
    Reader rd(data);
    char magic[4];
    if (!rd.Raw(magic, 4) || memcmp(magic, kIndexMagic, 4) != 0) {
        return false;
    }
    UINT32 version = rd.Num<UINT32>();
    if (version < 1 || version > kIndexVersion) {
        return false;
    }
    r->nextId = rd.Num<UINT64>();
    std::vector<Category> categories;
    if (version >= 2) {
        UINT32 count = rd.Num<UINT32>();
        for (UINT32 i = 0; i < count && rd.ok; i++) {
            Category c;
            if (ReadCategory(rd, c)) {
                categories.push_back(std::move(c));
            }
        }
    }
    UINT32 count = rd.Num<UINT32>();
    std::vector<Item> items;
    for (UINT32 i = 0; i < count && rd.ok; i++) {
        Item it;
        if (ReadItem(rd, it, version)) {
            items.push_back(std::move(it));
        }
    }
    if (!rd.ok) {
        return false;
    }
    r->items = std::move(items);
    r->categories = std::move(categories);
    return true;
}

void WriteSettingsMap(Writer& w, const std::map<std::wstring, INT64>& settings) {
    w.Num<UINT32>((UINT32)settings.size());
    for (const auto& [key, value] : settings) {
        w.WStr(key);
        w.Num<INT64>(value);
    }
}

bool ReadSettingsMap(Reader& rd, std::map<std::wstring, INT64>* settings) {
    UINT32 count = rd.Num<UINT32>();
    for (UINT32 i = 0; i < count && rd.ok; i++) {
        std::wstring key = rd.WStr();
        (*settings)[key] = rd.Num<INT64>();
    }
    return rd.ok;
}

std::string SerializeSettings() {
    Writer w;
    w.Raw(kSettingsMagic, 4);
    WriteSettingsMap(w, g_settings);
    return std::move(w.s);
}

bool ParseSettings(const std::string& data, LoadResult* r) {
    Reader rd(data);
    char magic[4];
    if (!rd.Raw(magic, 4) || memcmp(magic, kSettingsMagic, 4) != 0) {
        return false;
    }
    return ReadSettingsMap(rd, &r->settings);
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

// Runs a function on the UI thread.
void PostUi(std::function<void()> fn) {
    auto* call = new std::function<void()>(std::move(fn));
    if (!g_hwnd || !PostMessageW(g_hwnd, WM_APP_CALL, 0, (LPARAM)call)) {
        delete call;
    }
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
    WaitForSingleObject(g_workerThread, 60000);
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
    if (g_loaded && !g_masterKey.empty()) {
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
    if (g_loaded && !g_masterKey.empty()) {
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

void WriteImageLater(UINT64 id, std::string png) {
    Enqueue([id, png = std::move(png)] { WriteEncrypted(ImagePath(id), png); });
}

bool LoadImagePng(UINT64 id, std::string* png) {
    std::string data;
    return ReadFileBytes(ImagePath(id), &data) && Decrypt(data, png);
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
    HANDLE find = FindFirstFileW((g_dataDir + L"\\images\\*.bin").c_str(), &fd);
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

void OnLoaded(LoadResult* r);

void LoadJob() {
    auto* r = new LoadResult;
    CreateDirectoryW(g_dataDir.c_str(), nullptr);
    CreateDirectoryW((g_dataDir + L"\\images").c_str(), nullptr);
    if (LoadMasterKey()) {
        int index = ReadEncrypted(DataPath(L"index.dat"),
                                  [r](const std::string& s) { return ParseIndex(s, r); });
        if (index < 0) {
            SetAsideData();
        } else {
            DeleteOrphanImages(r->items);
        }
        ReadEncrypted(DataPath(L"settings.dat"),
                      [r](const std::string& s) { return ParseSettings(s, r); });
    } else {
        Wh_Log(L"Can't create the encryption key, history won't be saved");
    }
    PostUi([r] { OnLoaded(r); });
}

////////////////////////////////////////////////////////////////////////////////
// Colors and theme

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

D2D1_COLOR_F WithAlpha(D2D1_COLOR_F c, float alpha) {
    c.a = alpha;
    return c;
}

struct Theme {
    bool dark = false;
    bool highContrast = false;
    bool lightTaskbar = false;
    bool backdrop = false;
    D2D1_COLOR_F accent, accentText, onAccent, text, text2, text3, window, layer,
        layerStroke, hover, pressed, control, controlStroke, divider, flyout, strong,
        search, smoke, danger, warning, card;
    COLORREF editText, editBack, editPlaceholder;
};

Theme g_theme;
HBRUSH g_editBrush;

// Category and mark colors for the current mode.
D2D1_COLOR_F CategoryColor(BYTE color, float alpha = 1) {
    const ColorDef& c = kCategoryColors[std::min<BYTE>(color, kColorCount - 1)];
    return Rgba(g_theme.dark ? c.dark : c.light, alpha);
}

void LoadTheme() {
    Theme& t = g_theme;
    const wchar_t* personalize =
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
    INT64 mode = Setting(L"theme", 0);  // 0 system, 1 light, 2 dark.
    t.dark = mode == 2 || (mode == 0 && RegDword(HKEY_CURRENT_USER, personalize,
                                                 L"AppsUseLightTheme", 1) == 0);
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
        c = palette + (t.dark ? 0 : 5) * 4;
        t.accentText = D2D1::ColorF(c[0] / 255.f, c[1] / 255.f, c[2] / 255.f);
    } else {
        t.accent = Rgba(t.dark ? 0x60CDFF : 0x005FB8);
        t.accentText = Rgba(t.dark ? 0x99EBFF : 0x003E92);
    }
    t.onAccent = Rgba(t.dark ? 0x000000 : 0xFFFFFF);
    t.danger = Rgba(t.dark ? 0xFF99A4 : 0xC42B1C);
    t.warning = Rgba(t.dark ? 0xFCE100 : 0x9D5D00);

    if (t.highContrast) {
        D2D1_COLOR_F text = FromColorRef(GetSysColor(COLOR_WINDOWTEXT));
        D2D1_COLOR_F back = FromColorRef(GetSysColor(COLOR_WINDOW));
        t.accent = t.accentText = FromColorRef(GetSysColor(COLOR_HIGHLIGHT));
        t.onAccent = FromColorRef(GetSysColor(COLOR_HIGHLIGHTTEXT));
        t.text = t.text2 = t.strong = t.layerStroke = t.controlStroke = t.divider = text;
        t.text3 = FromColorRef(GetSysColor(COLOR_GRAYTEXT));
        t.window = t.layer = t.flyout = t.search = t.control = t.card = back;
        t.hover = t.pressed = FromColorRef(GetSysColor(COLOR_HIGHLIGHT), 0.35f);
        t.smoke = Rgba(0, 0.5f);
        t.danger = t.warning = text;
    } else if (t.dark) {
        t.text = Rgba(0xFFFFFF);
        t.text2 = Rgba(0xFFFFFF, 0.786f);
        t.text3 = Rgba(0xFFFFFF, 0.362f);
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
        t.smoke = Rgba(0x000000, 0.30f);
        t.card = Rgba(0xFFFFFF, 0.051f);
    } else {
        t.text = Rgba(0x000000, 0.894f);
        t.text2 = Rgba(0x000000, 0.62f);
        t.text3 = Rgba(0x000000, 0.361f);
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
        t.smoke = Rgba(0x000000, 0.30f);
        t.card = Rgba(0xFFFFFF, 0.70f);
    }

    // Text fields are native EDIT controls: they need opaque GDI colors that
    // match the field drawn around them.
    t.editBack = ToColorRef(t.search);
    t.editText = t.highContrast ? GetSysColor(COLOR_WINDOWTEXT)
                                : (t.dark ? RGB(255, 255, 255) : RGB(26, 26, 26));
    t.editPlaceholder = t.highContrast ? GetSysColor(COLOR_GRAYTEXT)
                                       : (t.dark ? RGB(200, 200, 200) : RGB(97, 97, 97));
    if (g_editBrush) {
        DeleteObject(g_editBrush);
    }
    g_editBrush = CreateSolidBrush(t.editBack);
}

// The window's own color layer over the acrylic; the transparency setting
// controls how much of the blurred desktop shows through.
float WindowAlpha() {
    if (!g_theme.backdrop) {
        return 1;
    }
    return 1 - std::clamp<INT64>(Setting(L"transparency", 30), 0, 90) / 100.f;
}

void ApplyWindowTheme(HWND hwnd, DWORD backdropType) {
    BOOL dark = g_theme.dark;
    DwmSetWindowAttribute(hwnd, kDwmUseImmersiveDarkMode, &dark, sizeof(dark));
    DWORD corner = kDwmCornerRound;
    DwmSetWindowAttribute(hwnd, kDwmWindowCornerPreference, &corner, sizeof(corner));
    DWORD backdrop = g_theme.highContrast ? kDwmBackdropNone : backdropType;
    g_theme.backdrop =
        SUCCEEDED(DwmSetWindowAttribute(hwnd, kDwmSystemBackdropType, &backdrop,
                                        sizeof(backdrop))) &&
        !g_theme.highContrast;
    MARGINS margins{-1, -1, -1, -1};
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    // Native menus (the tray menu) follow the dark mode too. The functions
    // are undocumented uxtheme exports, available since Windows 10 1903.
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
// Images (WIC)

IWICBitmapSource* DecodeImage(IWICImagingFactory* wic, const std::string& data) {
    IWICStream* stream = nullptr;
    IWICBitmapDecoder* decoder = nullptr;
    IWICBitmapFrameDecode* frame = nullptr;
    if (SUCCEEDED(wic->CreateStream(&stream)) &&
        SUCCEEDED(stream->InitializeFromMemory((BYTE*)data.data(),
                                               (DWORD)data.size())) &&
        SUCCEEDED(wic->CreateDecoderFromStream(
            stream, nullptr, WICDecodeMetadataCacheOnLoad, &decoder))) {
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

// Converts any image to the given 32 bpp format, optionally scaled.
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

// Size that fits in a box of maxSide, keeping the aspect ratio.
void FitSize(UINT w, UINT h, UINT maxSide, UINT* outW, UINT* outH) {
    float scale = std::min(1.f, (float)maxSide / std::max(w, h));
    *outW = std::max(1u, (UINT)std::lround(w * scale));
    *outH = std::max(1u, (UINT)std::lround(h * scale));
}

bool EncodePixelsPng(IWICImagingFactory* wic, UINT w, UINT h, const std::string& pixels,
                     WICPixelFormatGUID format, std::string* png) {
    IWICBitmap* bitmap = nullptr;
    bool ok = SUCCEEDED(wic->CreateBitmapFromMemory(w, h, format, w * 4,
                                                    (UINT)pixels.size(),
                                                    (BYTE*)pixels.data(), &bitmap)) &&
              EncodePng(wic, bitmap, png);
    SafeRelease(bitmap);
    return ok;
}

// Fills the image fields of an item. The PNG keeps the original resolution
// and is lossless (a PNG from the clipboard is kept byte for byte), unless
// maxSide asks for a smaller copy.
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
    bool ok = w && h;
    if (ok && c->maxSide && std::max(w, h) > c->maxSide) {
        UINT sw, sh;
        FitSize(w, h, c->maxSide, &sw, &sh);
        std::string pixels;
        ok = ToPixels(wic, source, sw, sh, GUID_WICPixelFormat32bppBGRA, &pixels) &&
             EncodePixelsPng(wic, sw, sh, pixels, GUID_WICPixelFormat32bppBGRA, png);
        w = sw;
        h = sh;
    } else if (ok) {
        if (c->png.empty()) {
            ok = EncodePng(wic, source, png);
        } else {
            *png = std::move(c->png);
        }
    }
    if (ok) {
        UINT tw, th;
        FitSize(w, h, kThumbSize, &tw, &th);
        IWICBitmapSource* thumbSource = source;
        IWICBitmapSource* scaled = nullptr;
        if (c->maxSide && png->size()) {
            // Thumbnail from the stored copy, so both match.
            scaled = DecodeImage(wic, *png);
            if (scaled) {
                thumbSource = scaled;
            }
        }
        if (ToPixels(wic, thumbSource, tw, th, GUID_WICPixelFormat32bppPBGRA, &it->thumb)) {
            it->thumbWidth = (UINT16)tw;
            it->thumbHeight = (UINT16)th;
        }
        SafeRelease(scaled);
        it->imageWidth = w;
        it->imageHeight = h;
    }
    source->Release();
    return ok;
}

// A 32 bpp bottom-up CF_DIB, which every app that takes images understands.
bool PngToDib(IWICImagingFactory* wic, const std::string& png, std::string* dib) {
    IWICBitmapSource* source = DecodeImage(wic, png);
    if (!source) {
        return false;
    }
    UINT w = 0, h = 0;
    source->GetSize(&w, &h);
    std::string pixels;
    bool ok = ToPixels(wic, source, w, h, GUID_WICPixelFormat32bppBGRA, &pixels);
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

// One line, whitespace collapsed.
std::wstring OneLine(const std::wstring& src, size_t max = 300) {
    std::wstring out;
    bool space = false;
    for (wchar_t c : src) {
        if (out.size() >= max) {
            break;
        }
        if (iswspace(c)) {
            space = !out.empty();
            continue;
        }
        if (space) {
            out += L' ';
            space = false;
        }
        out += c;
    }
    return out;
}

// Fills the fields that are derived from the content and not saved.
void FinishItem(Item& it) {
    if (it.imageWidth) {
        it.preview = Tr(L"Obraz · ", L"Image · ") + std::to_wstring(it.imageWidth) + L" × " +
                     std::to_wstring(it.imageHeight);
        return;
    }
    it.preview = OneLine(it.text.empty() ? it.files : it.text);
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
        if (name.size() > 4 && _wcsicmp(name.c_str() + name.size() - 4, L".exe") == 0) {
            name.resize(name.size() - 4);
        }
    }
    return cache[path] = name;
}

struct Captured {
    Item item;
    std::string png;
    // Set for an image saved by the editor.
    Kind kind = Kind::History;
    UINT64 category = 0;
    bool copy = false;
};

void OnCaptured(Captured* r);

void ProcessCapture(Capture* c, Captured* r) {
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
    HashItem(it, r->png);
    PostUi([r] { OnCaptured(r); });
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

std::wstring ClipboardText() {
    std::string text = ClipboardBytes(CF_UNICODETEXT);
    std::wstring s((const wchar_t*)text.data(), text.size() / 2);
    s.resize(wcsnlen(s.c_str(), s.size()));
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
        c->text = ClipboardText();
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
    c->maxSide = Setting(L"fullRes", 1) ? 0 : kMaxImageSide;
    Enqueue([c] { ProcessCapture(c, new Captured); });
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

void PutText(const std::wstring& text) {
    PutGlobal(CF_UNICODETEXT, text.c_str(), (text.size() + 1) * sizeof(wchar_t));
}

void PutFiles(const std::wstring& files) {
    std::wstring list = files;
    std::replace(list.begin(), list.end(), L'\n', L'\0');
    list.append(2, L'\0');
    std::string drop(sizeof(DROPFILES), '\0');
    auto* df = (DROPFILES*)drop.data();
    df->pFiles = sizeof(DROPFILES);
    df->fWide = TRUE;
    drop += Bytes(list);
    PutGlobal(CF_HDROP, drop.data(), drop.size());
}

bool OpenClipboardRetry() {
    for (int i = 0; i < 10; i++) {
        if (OpenClipboard(g_hwnd)) {
            return true;
        }
        Sleep(10);
    }
    return false;
}

enum class PasteMode { Normal, Plain, PngFile, PngPath };

IWICImagingFactory* g_wic;  // UI thread.

// A PNG copy of an image item in the temporary folder, for "Paste as file".
std::wstring TempPngPath(const Item& it) {
    WCHAR temp[MAX_PATH];
    GetTempPathW(ARRAYSIZE(temp), temp);
    std::wstring dir = std::wstring(temp) + L"Clipboard Tweaks";
    CreateDirectoryW(dir.c_str(), nullptr);
    std::wstring name = Tr(L"Obraz ", L"Image ") + FormatDatePattern(it.time, L"yyyy-MM-dd") +
                        L" " + std::to_wstring(it.id) + L".png";
    return dir + L"\\" + name;
}

void DeleteTempFiles() {
    WCHAR temp[MAX_PATH];
    GetTempPathW(ARRAYSIZE(temp), temp);
    std::wstring dir = std::wstring(temp) + L"Clipboard Tweaks";
    WIN32_FIND_DATAW fd;
    HANDLE find = FindFirstFileW((dir + L"\\*.png").c_str(), &fd);
    if (find != INVALID_HANDLE_VALUE) {
        do {
            DeleteFileW((dir + L"\\" + fd.cFileName).c_str());
        } while (FindNextFileW(find, &fd));
        FindClose(find);
    }
}

bool PutOnClipboard(const Item& it, PasteMode mode, const std::wstring* textOverride = nullptr) {
    std::string png;
    if (it.imageWidth && !LoadImagePng(it.id, &png)) {
        Wh_Log(L"Can't read image %llu", it.id);
        return false;
    }
    std::wstring pngPath;
    if (!png.empty() && (mode == PasteMode::PngFile || mode == PasteMode::PngPath)) {
        pngPath = TempPngPath(it);
        HANDLE file = CreateFileW(pngPath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
                                  FILE_ATTRIBUTE_NORMAL, nullptr);
        DWORD written = 0;
        bool ok = file != INVALID_HANDLE_VALUE &&
                  WriteFile(file, png.data(), (DWORD)png.size(), &written, nullptr);
        if (file != INVALID_HANDLE_VALUE) {
            CloseHandle(file);
        }
        if (!ok) {
            return false;
        }
    }
    if (!OpenClipboardRetry()) {
        return false;
    }
    EmptyClipboard();
    if (textOverride) {
        PutText(*textOverride);
    } else if (!pngPath.empty()) {
        if (mode == PasteMode::PngFile) {
            PutFiles(pngPath);
        } else {
            PutText(pngPath);
        }
    } else {
        bool plain = mode == PasteMode::Plain;
        std::wstring text = it.text.empty() && plain ? it.files : it.text;
        if (!text.empty()) {
            PutText(text);
        }
        if (!plain) {
            if (!it.html.empty()) {
                PutGlobal(g_cfHtml, it.html.c_str(), it.html.size() + 1);
            }
            if (!it.rtf.empty()) {
                PutGlobal(g_cfRtf, it.rtf.c_str(), it.rtf.size() + 1);
            }
            if (!it.files.empty()) {
                PutFiles(it.files);
            }
        }
        if (!png.empty()) {
            PutGlobal(g_cfPng, png.data(), png.size());
            std::string dib;
            if (PngToDib(g_wic, png, &dib)) {
                PutGlobal(CF_DIB, dib.data(), dib.size());
            }
        }
    }
    CloseClipboard();
    g_ownClipboardSeq = GetClipboardSequenceNumber();
    return true;
}

// Fills the fields of a template.
std::wstring ExpandTemplate(const std::wstring& text) {
    std::wstring out;
    UINT64 now = NowFileTime();
    SYSTEMTIME st = LocalTime(now);
    WCHAR date[64] = L"";
    GetDateFormatEx(LOCALE_NAME_USER_DEFAULT, DATE_SHORTDATE, &st, nullptr, date,
                    ARRAYSIZE(date), nullptr);
    std::wstring clip;
    bool clipRead = false;
    for (size_t i = 0; i < text.size();) {
        auto field = [&](const wchar_t* name) {
            size_t n = wcslen(name);
            return text.compare(i, n, name) == 0 ? n : 0;
        };
        if (size_t n = std::max(field(L"{data}"), field(L"{date}"))) {
            out += date;
            i += n;
        } else if (size_t n = std::max(field(L"{godzina}"), field(L"{time}"))) {
            out += FormatTime(now);
            i += n;
        } else if (size_t n = std::max(field(L"{schowek}"), field(L"{clipboard}"))) {
            if (!clipRead && OpenClipboardRetry()) {
                clip = ClipboardText();
                CloseClipboard();
                clipRead = true;
            }
            out += clip;
            i += n;
        } else {
            out += text[i++];
        }
    }
    return out;
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
    // Releases held modifiers, so that Shift+Enter, Alt or a template's
    // shortcut don't turn Ctrl+V into another shortcut. The dummy key keeps
    // an Alt or Win release from opening a menu.
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
// Fonts

ID2D1Factory* g_d2d;
IDWriteFactory* g_dwrite;
const wchar_t* g_uiFont = L"Segoe UI";
const wchar_t* g_iconFont = L"Segoe Fluent Icons";
const wchar_t* g_monoFont = L"Consolas";

struct {
    IDWriteTextFormat *title, *tab, *row, *rowBold, *meta, *header, *small, *smallBold,
        *smallCenter, *key, *empty, *body, *mono, *subtitle, *pageTitle, *emoji16,
        *emoji20, *emoji28, *icon16, *icon14, *icon12, *icon10, *icon20;
} g_fonts;

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
                              DWRITE_TEXT_ALIGNMENT align, bool ellipsis = false,
                              bool wrap = false) {
    IDWriteTextFormat* f = nullptr;
    if (FAILED(g_dwrite->CreateTextFormat(family, nullptr, weight,
                                          DWRITE_FONT_STYLE_NORMAL,
                                          DWRITE_FONT_STRETCH_NORMAL, size, L"", &f))) {
        return nullptr;
    }
    f->SetTextAlignment(align);
    f->SetParagraphAlignment(wrap ? DWRITE_PARAGRAPH_ALIGNMENT_NEAR
                                  : DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    f->SetWordWrapping(wrap ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP);
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

IDWriteTextLayout* MakeLayout(std::wstring_view s, IDWriteTextFormat* format, float width,
                              float height = 10000) {
    IDWriteTextLayout* layout = nullptr;
    if (format) {
        g_dwrite->CreateTextLayout(s.data(), (UINT32)s.size(), format, std::max(1.f, width),
                                   std::max(1.f, height), &layout);
    }
    return layout;
}

float TextWidth(std::wstring_view s, IDWriteTextFormat* format) {
    DWRITE_TEXT_METRICS m{};
    if (IDWriteTextLayout* layout = MakeLayout(s, format, 10000, 100)) {
        layout->GetMetrics(&m);
        layout->Release();
    }
    return m.widthIncludingTrailingWhitespace;
}

float TextHeight(std::wstring_view s, IDWriteTextFormat* format, float width) {
    DWRITE_TEXT_METRICS m{};
    if (IDWriteTextLayout* layout = MakeLayout(s, format, width)) {
        layout->GetMetrics(&m);
        layout->Release();
    }
    return m.height;
}

void CreateFonts() {
    if (HasFont(L"Segoe UI Variable Text")) {
        g_uiFont = L"Segoe UI Variable Text";
    }
    if (!HasFont(g_iconFont)) {
        g_iconFont = L"Segoe MDL2 Assets";
    }
    if (HasFont(L"Cascadia Mono")) {
        g_monoFont = L"Cascadia Mono";
    }
    auto L = DWRITE_TEXT_ALIGNMENT_LEADING;
    auto C = DWRITE_TEXT_ALIGNMENT_CENTER;
    auto R = DWRITE_TEXT_ALIGNMENT_TRAILING;
    auto N = DWRITE_FONT_WEIGHT_NORMAL;
    auto S = DWRITE_FONT_WEIGHT_SEMI_BOLD;
    g_fonts.title = MakeFormat(g_uiFont, 12, N, L, true);
    g_fonts.tab = MakeFormat(g_uiFont, 14, S, C);
    g_fonts.row = MakeFormat(g_uiFont, 14, N, L, true);
    g_fonts.rowBold = MakeFormat(g_uiFont, 14, S, L, true);
    g_fonts.meta = MakeFormat(g_uiFont, 12, N, R, true);
    g_fonts.header = MakeFormat(g_uiFont, 12, S, L, true);
    g_fonts.small = MakeFormat(g_uiFont, 12, N, L, true);
    g_fonts.smallBold = MakeFormat(g_uiFont, 12, S, L, true);
    g_fonts.smallCenter = MakeFormat(g_uiFont, 12, N, C, true);
    g_fonts.key = MakeFormat(g_uiFont, 11, N, C);
    g_fonts.empty = MakeFormat(g_uiFont, 14, N, C);
    if (g_fonts.empty) {
        g_fonts.empty->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
    }
    g_fonts.body = MakeFormat(g_uiFont, 14, N, L, false, true);
    g_fonts.mono = MakeFormat(g_monoFont, 13, N, L, false, true);
    g_fonts.subtitle = MakeFormat(g_uiFont, 14, S, L, true);
    g_fonts.pageTitle = MakeFormat(g_uiFont, 20, S, L, true);
    g_fonts.emoji16 = MakeFormat(g_uiFont, 14, N, C);
    g_fonts.emoji20 = MakeFormat(g_uiFont, 18, N, C);
    g_fonts.emoji28 = MakeFormat(g_uiFont, 24, N, C);
    g_fonts.icon16 = MakeFormat(g_iconFont, 16, N, C);
    g_fonts.icon14 = MakeFormat(g_iconFont, 14, N, C);
    g_fonts.icon12 = MakeFormat(g_iconFont, 12, N, C);
    g_fonts.icon10 = MakeFormat(g_iconFont, 10, N, C);
    g_fonts.icon20 = MakeFormat(g_iconFont, 20, N, C);
}

void ReleaseFonts() {
    for (IDWriteTextFormat** f :
         {&g_fonts.title, &g_fonts.tab, &g_fonts.row, &g_fonts.rowBold, &g_fonts.meta,
          &g_fonts.header, &g_fonts.small, &g_fonts.smallBold, &g_fonts.smallCenter,
          &g_fonts.key, &g_fonts.empty, &g_fonts.body, &g_fonts.mono, &g_fonts.subtitle,
          &g_fonts.pageTitle, &g_fonts.emoji16, &g_fonts.emoji20, &g_fonts.emoji28,
          &g_fonts.icon16, &g_fonts.icon14, &g_fonts.icon12, &g_fonts.icon10,
          &g_fonts.icon20}) {
        SafeRelease(*f);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Tray icon

HICON g_trayIcon;

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
    wcscpy_s(nid.szTip, g_paused ? Tr(L"Clipboard Tweaks: zapisywanie wstrzymane",
                                      L"Clipboard Tweaks: saving paused")
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
// Immediate-mode UI. Every frame draws a window from the current state and
// registers the clickable regions it drew; the mouse is matched against the
// regions of the last frame. A Hit names a region: the source line that drew
// it plus an index, so it stays the same from frame to frame.

struct Hit {
    int kind = 0;
    INT64 index = 0;
    bool operator==(const Hit&) const = default;
    explicit operator bool() const { return kind != 0; }
};
#define HIT(i) Hit{__LINE__, (INT64)(i)}

enum class Drag { Start, Move, End };

struct Scroller {
    float pos = 0, target = 0, content = 0, view = 0;
    float grab = 0;  // Where the scrollbar thumb was grabbed.
    float Max() const { return std::max(0.f, content - view); }
    void Clamp() {
        pos = std::clamp(pos, 0.f, Max());
        target = std::clamp(target, 0.f, Max());
    }
    void To(float t);
    void By(float d) { To(target + d); }
    void Reset() { pos = target = 0; }
};

struct Region {
    D2D1_RECT_F r{};
    Hit hit;
    std::function<void()> click, rclick, dblclick;
    std::function<void(Drag, D2D1_POINT_2F)> drag;
    Scroller* scroll = nullptr;
    std::wstring tip;
    LPCWSTR cursor = nullptr;
    bool caption = false;
};

struct MenuItem {
    std::wstring label;
    wchar_t glyph = 0;
    std::wstring emoji;
    int dot = -1;  // Category color index, drawn as a dot.
    std::wstring hint;
    bool checked = false;
    bool separator = false;
    bool disabled = false;
    bool danger = false;
    std::function<void()> action;
};

struct Flyout {
    bool open = false;
    D2D1_RECT_F anchor{};
    bool above = false;
    std::vector<MenuItem> items;
    float width = 0;
    // Custom content instead of menu items.
    D2D1_SIZE_F size{};
    std::function<void(D2D1_RECT_F)> draw;
    Hit owner;
    int focus = -1;
};

struct Dialog {
    bool open = false;
    std::wstring title;
    float width = 440;
    float height = 200;
    // Draws the content from the given top-left; returns the bottom.
    std::function<float(D2D1_RECT_F)> draw;
    std::function<void()> onEnter, onEscape;
};

struct Surface {
    HWND hwnd = nullptr;
    ID2D1HwndRenderTarget* rt = nullptr;
    ID2D1SolidColorBrush* brush = nullptr;
    UINT dpi = 96;
    void (*render)() = nullptr;
    std::vector<Region> regions;
    std::vector<D2D1_RECT_F> clips;
    Hit hot, pressed;
    bool dragging = false;
    std::function<void(Drag, D2D1_POINT_2F)> dragFn;
    D2D1_POINT_2F mouse{-100, -100};
    bool mouseInside = false;
    bool again = false;
    bool tipShown = false;
    std::unordered_map<UINT64, ID2D1Bitmap*> bitmaps;
    Flyout flyout;
    Dialog dialog;
};

Surface g_main;    // The clipboard window.
Surface g_edView;  // The image editor.
Surface* g_s = &g_main;
ID2D1RenderTarget* g_rt;
ID2D1SolidColorBrush* g_brush;

int Px(float dip) {
    return (int)std::lround(dip * g_s->dpi / 96.f);
}

D2D1_SIZE_F SizeDip(Surface& s) {
    RECT rc;
    GetClientRect(s.hwnd, &rc);
    return {rc.right * 96.f / s.dpi, rc.bottom * 96.f / s.dpi};
}

void Invalidate(Surface& s) {
    if (s.hwnd) {
        InvalidateRect(s.hwnd, nullptr, FALSE);
    }
}

void Redraw() {
    Invalidate(g_main);
}

void RedrawEditor() {
    Invalidate(g_edView);
}

bool In(const D2D1_RECT_F& r, D2D1_POINT_2F p) {
    return p.x >= r.left && p.x < r.right && p.y >= r.top && p.y < r.bottom;
}

D2D1_RECT_F Intersect(const D2D1_RECT_F& a, const D2D1_RECT_F& b) {
    return {std::max(a.left, b.left), std::max(a.top, b.top), std::min(a.right, b.right),
            std::min(a.bottom, b.bottom)};
}

bool IsEmpty(const D2D1_RECT_F& r) {
    return r.right <= r.left || r.bottom <= r.top;
}

D2D1_RECT_F Inset(const D2D1_RECT_F& r, float dx, float dy) {
    return {r.left + dx, r.top + dy, r.right - dx, r.bottom - dy};
}

// Registers a clickable region, clipped to the current scroll area.
Region& Add(const D2D1_RECT_F& r, Hit hit) {
    static Region dummy;
    D2D1_RECT_F clipped = g_s->clips.empty() ? r : Intersect(r, g_s->clips.back());
    if (IsEmpty(clipped)) {
        dummy = {};
        return dummy;
    }
    g_s->regions.push_back({clipped, hit});
    return g_s->regions.back();
}

bool Hot(Hit h) {
    return g_s->hot == h;
}

bool Pressed(Hit h) {
    return g_s->pressed == h && g_s->hot == h;
}

void PushClip(const D2D1_RECT_F& r) {
    D2D1_RECT_F clip = g_s->clips.empty() ? r : Intersect(r, g_s->clips.back());
    g_s->clips.push_back(clip);
    g_rt->PushAxisAlignedClip(clip, D2D1_ANTIALIAS_MODE_ALIASED);
}

void PopClip() {
    g_rt->PopAxisAlignedClip();
    g_s->clips.pop_back();
}

////////////////////////////////////////////////////////////////////////////////
// Drawing helpers

void Fill(const D2D1_RECT_F& r, D2D1_COLOR_F c, float radius = 0) {
    g_brush->SetColor(c);
    if (radius > 0) {
        g_rt->FillRoundedRectangle(D2D1::RoundedRect(r, radius, radius), g_brush);
    } else {
        g_rt->FillRectangle(r, g_brush);
    }
}

void Stroke(const D2D1_RECT_F& r, D2D1_COLOR_F c, float radius, float width = 1,
            ID2D1StrokeStyle* style = nullptr) {
    g_brush->SetColor(c);
    float h = width / 2;
    D2D1_RECT_F inner{r.left + h, r.top + h, r.right - h, r.bottom - h};
    g_rt->DrawRoundedRectangle(D2D1::RoundedRect(inner, radius, radius), g_brush, width,
                               style);
}

void Line(float x1, float y1, float x2, float y2, D2D1_COLOR_F c, float width = 1) {
    g_brush->SetColor(c);
    g_rt->DrawLine({x1, y1}, {x2, y2}, g_brush, width);
}

void Circle(float cx, float cy, float radius, D2D1_COLOR_F c) {
    g_brush->SetColor(c);
    g_rt->FillEllipse(D2D1::Ellipse({cx, cy}, radius, radius), g_brush);
}

void CircleStroke(float cx, float cy, float radius, D2D1_COLOR_F c, float width = 1) {
    g_brush->SetColor(c);
    g_rt->DrawEllipse(D2D1::Ellipse({cx, cy}, radius - width / 2, radius - width / 2),
                      g_brush, width);
}

void Text(std::wstring_view s, IDWriteTextFormat* format, const D2D1_RECT_F& r,
          D2D1_COLOR_F c) {
    if (!format || s.empty()) {
        return;
    }
    g_brush->SetColor(c);
    g_rt->DrawText(s.data(), (UINT32)s.size(), format, r, g_brush,
                   D2D1_DRAW_TEXT_OPTIONS_CLIP);
}

void Glyph(wchar_t glyph, IDWriteTextFormat* format, const D2D1_RECT_F& r,
           D2D1_COLOR_F c) {
    if (!format) {
        return;
    }
    g_brush->SetColor(c);
    g_rt->DrawText(&glyph, 1, format, r, g_brush);
}

// Emoji in color; DirectWrite falls back to Segoe UI Emoji by itself.
void Emoji(std::wstring_view s, IDWriteTextFormat* format, const D2D1_RECT_F& r) {
    if (!format || s.empty()) {
        return;
    }
    g_brush->SetColor(g_theme.text);
    g_rt->DrawText(s.data(), (UINT32)s.size(), format, r, g_brush,
                   D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
}

void Layout(IDWriteTextLayout* layout, D2D1_POINT_2F at, D2D1_COLOR_F c) {
    g_brush->SetColor(c);
    g_rt->DrawTextLayout(at, layout, g_brush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
}

ID2D1StrokeStyle* g_dashStyle;

ID2D1StrokeStyle* DashStyle() {
    if (!g_dashStyle) {
        float dashes[] = {3, 3};
        g_d2d->CreateStrokeStyle(
            D2D1::StrokeStyleProperties(D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT,
                                        D2D1_CAP_STYLE_FLAT, D2D1_LINE_JOIN_MITER, 10,
                                        D2D1_DASH_STYLE_CUSTOM, 0),
            dashes, 2, &g_dashStyle);
    }
    return g_dashStyle;
}

ID2D1Bitmap* PixelBitmap(UINT w, UINT h, const void* pixels) {
    ID2D1Bitmap* bitmap = nullptr;
    g_rt->CreateBitmap(D2D1::SizeU(w, h), pixels, w * 4,
                       D2D1::BitmapProperties(D2D1::PixelFormat(
                           DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
                       &bitmap);
    return bitmap;
}

ID2D1Bitmap* Thumbnail(const Item& it) {
    if (it.thumb.empty()) {
        return nullptr;
    }
    auto& cache = g_s->bitmaps;
    if (auto found = cache.find(it.id); found != cache.end()) {
        return found->second;
    }
    ID2D1Bitmap* bitmap = PixelBitmap(it.thumbWidth, it.thumbHeight, it.thumb.data());
    if (bitmap) {
        cache[it.id] = bitmap;
    }
    return bitmap;
}

void ForgetThumbnail(UINT64 id) {
    if (auto found = g_main.bitmaps.find(id); found != g_main.bitmaps.end()) {
        found->second->Release();
        g_main.bitmaps.erase(found);
    }
}

// Draws a bitmap centered in a box, scaled to fit.
void DrawFitted(ID2D1Bitmap* bitmap, const D2D1_RECT_F& box, float maxScale = 1e9f,
                D2D1_RECT_F* drawn = nullptr) {
    D2D1_SIZE_F s = bitmap->GetSize();
    float scale = std::min({(box.right - box.left) / s.width,
                            (box.bottom - box.top) / s.height, maxScale});
    float cx = (box.left + box.right) / 2, cy = (box.top + box.bottom) / 2;
    D2D1_RECT_F dst{cx - s.width * scale / 2, cy - s.height * scale / 2,
                    cx + s.width * scale / 2, cy + s.height * scale / 2};
    g_rt->DrawBitmap(bitmap, dst, 1, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
    if (drawn) {
        *drawn = dst;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Controls

D2D1_COLOR_F SubtleFill(Hit h) {
    if (Pressed(h)) {
        return g_theme.pressed;
    }
    return Hot(h) ? g_theme.hover : D2D1::ColorF(0, 0);
}

Region& IconButton(const D2D1_RECT_F& r, Hit h, wchar_t glyph, const wchar_t* tip,
                   D2D1_COLOR_F color, IDWriteTextFormat* font = nullptr) {
    Fill(r, SubtleFill(h), 4);
    Glyph(glyph, font ? font : g_fonts.icon16, r, color);
    Region& region = Add(r, h);
    if (tip) {
        region.tip = tip;
    }
    return region;
}

Region& IconButton(const D2D1_RECT_F& r, Hit h, wchar_t glyph, const wchar_t* tip) {
    return IconButton(r, h, glyph, tip, g_theme.text);
}

enum ButtonStyle { kStandard, kAccent, kSubtle, kDanger };

float ButtonWidth(const std::wstring& label, wchar_t glyph = 0) {
    return 24 + TextWidth(label, g_fonts.row) + (glyph ? 24 : 0);
}

Region& Button(const D2D1_RECT_F& r, Hit h, const std::wstring& label, wchar_t glyph = 0,
               ButtonStyle style = kStandard, bool disabled = false) {
    const Theme& t = g_theme;
    D2D1_COLOR_F fg = t.text;
    if (style == kAccent && !disabled) {
        D2D1_COLOR_F back = t.accent;
        if (Pressed(h)) {
            back.a = 0.8f;
        } else if (Hot(h)) {
            back.a = 0.9f;
        }
        Fill(r, back, 4);
        fg = t.onAccent;
    } else if (style == kSubtle) {
        Fill(r, disabled ? D2D1::ColorF(0, 0) : SubtleFill(h), 4);
    } else {
        Fill(r, t.control, 4);
        if (!disabled) {
            Fill(r, SubtleFill(h), 4);
        }
        Stroke(r, t.controlStroke, 4);
        if (style == kDanger) {
            fg = t.danger;
        }
    }
    if (disabled) {
        fg = t.text3;
    }
    float w = TextWidth(label, g_fonts.row) + (glyph ? 22 : 0);
    float x = (r.left + r.right - w) / 2;
    if (glyph) {
        Glyph(glyph, g_fonts.icon14, {x, r.top, x + 16, r.bottom}, fg);
        x += 22;
    }
    Text(label, g_fonts.row, {x, r.top, r.right, r.bottom}, fg);
    Region& region = Add(r, disabled ? Hit{} : h);
    return region;
}

Region& Toggle(float x, float cy, Hit h, bool on, bool disabled = false) {
    const Theme& t = g_theme;
    D2D1_RECT_F r{x, cy - 10, x + 40, cy + 10};
    if (on) {
        D2D1_COLOR_F back = disabled ? t.text3 : t.accent;
        if (Hot(h) && !disabled) {
            back.a = 0.9f;
        }
        Fill(r, back, 10);
        Circle(r.right - 10, cy, Hot(h) ? 7 : 6, t.onAccent);
    } else {
        Fill(r, Hot(h) ? t.hover : t.control, 10);
        Stroke(r, disabled ? t.text3 : t.strong, 10);
        Circle(r.left + 10, cy, Hot(h) ? 6 : 5, disabled ? t.text3 : t.text2);
    }
    return Add(Inset(r, -4, -6), disabled ? Hit{} : h);
}

Region& Checkbox(const D2D1_RECT_F& r, Hit h, const std::wstring& label, bool checked,
                 bool disabled = false) {
    const Theme& t = g_theme;
    float cy = (r.top + r.bottom) / 2;
    D2D1_RECT_F box{r.left, cy - 10, r.left + 20, cy + 10};
    if (checked) {
        Fill(box, disabled ? t.text3 : t.accent, 4);
        Glyph(kIconCheck, g_fonts.icon12, box, t.onAccent);
    } else {
        Fill(box, Hot(h) ? t.hover : t.control, 4);
        Stroke(box, t.strong, 4);
    }
    Text(label, g_fonts.row, {box.right + 10, r.top, r.right, r.bottom},
         disabled ? t.text3 : t.text);
    return Add(r, disabled ? Hit{} : h);
}

Region& Radio(const D2D1_RECT_F& r, Hit h, bool selected) {
    const Theme& t = g_theme;
    float cy = (r.top + r.bottom) / 2, cx = r.left + 10;
    if (selected) {
        Circle(cx, cy, 10, t.accent);
        Circle(cx, cy, Hot(h) ? 5 : 4, t.onAccent);
    } else {
        Circle(cx, cy, 10, Hot(h) ? t.hover : t.control);
        CircleStroke(cx, cy, 10, t.strong);
    }
    return Add(r, h);
}

// A keyboard key chip; returns its width.
float Kbd(float x, float cy, const std::wstring& key, bool accent = false) {
    const Theme& t = g_theme;
    float w = TextWidth(key, g_fonts.key) + 12;
    D2D1_RECT_F r{x, cy - 10, x + w, cy + 10};
    Fill(r, accent ? t.accent : t.control, 4);
    if (!accent) {
        Stroke(r, t.controlStroke, 4);
    }
    Text(key, g_fonts.key, r, accent ? t.onAccent : t.text);
    return w;
}

// A rounded chip; returns its width.
float ChipWidth(const std::wstring& label, bool close = false) {
    return TextWidth(label, g_fonts.small) + 20 + (close ? 18 : 0);
}

Region& Chip(float x, float y, Hit h, const std::wstring& label, bool selected,
             D2D1_COLOR_F color, bool close = false) {
    const Theme& t = g_theme;
    float w = ChipWidth(label, close);
    D2D1_RECT_F r{x, y, x + w, y + 24};
    if (selected) {
        Fill(r, t.accent, 12);
        color = t.onAccent;
    } else {
        Fill(r, t.control, 12);
        Fill(r, SubtleFill(h), 12);
        Stroke(r, t.controlStroke, 12);
    }
    Text(label, g_fonts.small, {r.left + 10, r.top, r.right, r.bottom}, color);
    if (close) {
        Glyph(kIconCancel, g_fonts.icon10, {r.right - 24, r.top, r.right - 6, r.bottom},
              color);
    }
    return Add(r, h);
}

// A small horizontal slider; value in 0..1.
Region& Slider(const D2D1_RECT_F& r, Hit h, float value,
               std::function<void(float)> onChange) {
    const Theme& t = g_theme;
    float cy = (r.top + r.bottom) / 2;
    float x0 = r.left + 10, x1 = r.right - 10;
    float x = x0 + (x1 - x0) * std::clamp(value, 0.f, 1.f);
    Fill({x0, cy - 2, x1, cy + 2}, t.strong, 2);
    Fill({x0, cy - 2, x, cy + 2}, t.accent, 2);
    Circle(x, cy, 10, t.flyout);
    CircleStroke(x, cy, 10, t.controlStroke);
    Circle(x, cy, Pressed(h) ? 4 : Hot(h) ? 7 : 5, t.accent);
    Region& region = Add(r, h);
    region.drag = [x0, x1, onChange](Drag, D2D1_POINT_2F p) {
        onChange(std::clamp((p.x - x0) / std::max(1.f, x1 - x0), 0.f, 1.f));
    };
    return region;
}

////////////////////////////////////////////////////////////////////////////////
// Flyouts: menus and small panels drawn inside the window.

void CloseFlyout() {
    if (g_s->flyout.open) {
        g_s->flyout = {};
        Invalidate(*g_s);
    }
}

// Closes the flyout when it belongs to the same button; returns true then.
bool ToggleFlyout(Hit owner) {
    if (g_s->flyout.open && g_s->flyout.owner == owner) {
        CloseFlyout();
        return true;
    }
    return false;
}

void OpenMenu(const D2D1_RECT_F& anchor, Hit owner, std::vector<MenuItem> items,
              float width = 0, bool above = false) {
    Flyout& f = g_s->flyout;
    f = {};
    f.open = true;
    f.anchor = anchor;
    f.owner = owner;
    f.items = std::move(items);
    f.width = width;
    f.above = above;
    Invalidate(*g_s);
}

void OpenFlyout(const D2D1_RECT_F& anchor, Hit owner, D2D1_SIZE_F size,
                std::function<void(D2D1_RECT_F)> draw, bool above = false) {
    Flyout& f = g_s->flyout;
    f = {};
    f.open = true;
    f.anchor = anchor;
    f.owner = owner;
    f.size = size;
    f.draw = std::move(draw);
    f.above = above;
    Invalidate(*g_s);
}

constexpr float kMenuItemHeight = 32;

D2D1_SIZE_F MenuSize(const Flyout& f, bool* checks) {
    float w = f.width, h = 8;
    *checks = false;
    for (const MenuItem& item : f.items) {
        if (item.checked) {
            *checks = true;
        }
    }
    for (const MenuItem& item : f.items) {
        if (item.separator) {
            h += 9;
            continue;
        }
        h += kMenuItemHeight;
        float iw = 24 + TextWidth(item.label, g_fonts.row) + (*checks ? 24 : 0) +
                   (item.glyph || !item.emoji.empty() || item.dot >= 0 ? 28 : 0) +
                   (item.hint.empty() ? 0 : 24 + TextWidth(item.hint, g_fonts.small));
        w = std::max(w, iw + 8);
    }
    return {std::max(w, 160.f), h};
}

void ActivateMenuItem(int index) {
    Flyout& f = g_s->flyout;
    if (index < 0 || index >= (int)f.items.size() || f.items[index].separator ||
        f.items[index].disabled) {
        return;
    }
    std::function<void()> action = f.items[index].action;
    CloseFlyout();
    if (action) {
        action();
    }
}

void DrawFlyout() {
    Flyout& f = g_s->flyout;
    if (!f.open) {
        return;
    }
    const Theme& t = g_theme;
    D2D1_SIZE_F win = SizeDip(*g_s);
    bool checks = false;
    D2D1_SIZE_F size = f.draw ? f.size : MenuSize(f, &checks);
    size.width = std::min(size.width, win.width - 16);
    float x = f.anchor.left;
    if (x + size.width > win.width - 8) {
        x = f.anchor.right - size.width;
    }
    x = std::clamp(x, 8.f, std::max(8.f, win.width - 8 - size.width));
    float below = f.anchor.bottom + 4, above = f.anchor.top - 4 - size.height;
    bool fitsBelow = below + size.height <= win.height - 8;
    bool fitsAbove = above >= 8;
    float y = (f.above ? (fitsAbove || !fitsBelow) : !fitsBelow && fitsAbove) ? above : below;
    y = std::clamp(y, 8.f, std::max(8.f, win.height - 8 - size.height));
    D2D1_RECT_F r{x, y, x + size.width, y + size.height};

    // Clicking outside closes the flyout and does nothing else.
    Region& outside = Add({0, 0, win.width, win.height}, HIT(0));
    outside.click = outside.rclick = [] { CloseFlyout(); };
    // A soft shadow.
    for (int i = 1; i <= 4; i++) {
        Fill({r.left - i, r.top - i + 2, r.right + i, r.bottom + i + 2},
             Rgba(0, t.dark ? 0.06f : 0.025f), 8 + i);
    }
    Fill(r, t.flyout, 8);
    Stroke(r, t.dark ? Rgba(0, 0.4f) : Rgba(0, 0.1f), 8);
    Add(r, HIT(1));  // Clicks between items do nothing.
    if (f.draw) {
        f.draw(r);
        return;
    }
    float iy = r.top + 4;
    for (int i = 0; i < (int)f.items.size(); i++) {
        const MenuItem& item = f.items[i];
        if (item.separator) {
            Fill({r.left, iy + 4, r.right, iy + 5}, t.divider);
            iy += 9;
            continue;
        }
        D2D1_RECT_F ir{r.left + 4, iy, r.right - 4, iy + kMenuItemHeight};
        Hit h = HIT(i);
        if (!item.disabled && (Hot(h) || f.focus == i)) {
            Fill(ir, Pressed(h) ? t.pressed : t.hover, 4);
        }
        D2D1_COLOR_F fg = item.disabled ? t.text3 : item.danger ? t.danger : t.text;
        float lx = ir.left + 12;
        if (checks) {
            if (item.checked) {
                Glyph(kIconCheck, g_fonts.icon12, {lx, ir.top, lx + 16, ir.bottom}, fg);
            }
            lx += 24;
        }
        if (item.glyph) {
            Glyph(item.glyph, g_fonts.icon16, {lx, ir.top, lx + 16, ir.bottom}, fg);
            lx += 28;
        } else if (!item.emoji.empty()) {
            Emoji(item.emoji, g_fonts.emoji16, {lx - 2, ir.top, lx + 18, ir.bottom});
            lx += 28;
        } else if (item.dot >= 0) {
            float cy = (ir.top + ir.bottom) / 2;
            if (item.dot == 0) {
                CircleStroke(lx + 8, cy, 6, t.strong);
            } else {
                Circle(lx + 8, cy, 6, CategoryColor((BYTE)item.dot));
            }
            lx += 28;
        }
        float hintWidth = item.hint.empty() ? 0 : TextWidth(item.hint, g_fonts.small);
        Text(item.label, g_fonts.row, {lx, ir.top, ir.right - 12 - hintWidth - 12, ir.bottom},
             fg);
        if (hintWidth) {
            Text(item.hint, g_fonts.small,
                 {ir.right - 12 - hintWidth, ir.top, ir.right - 8, ir.bottom}, t.text2);
        }
        if (!item.disabled) {
            Add(ir, h).click = [i] { ActivateMenuItem(i); };
        }
        iy += kMenuItemHeight;
    }
}

// Arrow keys and Enter in an open menu.
bool FlyoutKey(UINT vk) {
    Flyout& f = g_s->flyout;
    if (!f.open) {
        return false;
    }
    if (vk == VK_ESCAPE) {
        CloseFlyout();
        return true;
    }
    if (f.draw) {
        return false;
    }
    int n = (int)f.items.size();
    if (vk == VK_UP || vk == VK_DOWN) {
        int step = vk == VK_DOWN ? 1 : -1;
        for (int k = 0, i = f.focus; k < n; k++) {
            i = ((i < 0 && step < 0 ? n : i) + step + n) % n;
            if (!f.items[i].separator && !f.items[i].disabled) {
                f.focus = i;
                break;
            }
        }
        Invalidate(*g_s);
        return true;
    }
    if (vk == VK_RETURN || vk == VK_SPACE) {
        ActivateMenuItem(f.focus);
        return true;
    }
    return false;
}

// A small question with a confirming button, such as "Delete this template?".
void OpenConfirm(const D2D1_RECT_F& anchor, Hit owner, const std::wstring& question,
                 const std::wstring& confirm, std::function<void()> yes) {
    float w = 300;
    float textHeight = TextHeight(question, g_fonts.body, w - 32);
    OpenFlyout(anchor, owner, {w, 16 + textHeight + 16 + 32 + 16},
               [=](D2D1_RECT_F r) {
                   Text(question, g_fonts.body,
                        {r.left + 16, r.top + 16, r.right - 16, r.top + 16 + textHeight},
                        g_theme.text);
                   float by = r.bottom - 16 - 32;
                   float bw = (r.right - r.left - 32 - 8) / 2;
                   Button({r.left + 16, by, r.left + 16 + bw, by + 32}, HIT(0), confirm,
                          0, kAccent)
                       .click = [yes] {
                       CloseFlyout();
                       yes();
                   };
                   Button({r.right - 16 - bw, by, r.right - 16, by + 32}, HIT(0),
                          Tr(L"Anuluj", L"Cancel"))
                       .click = [] { CloseFlyout(); };
               });
}

// A grid of emoji; "none" clears it.
void OpenEmojiPicker(const D2D1_RECT_F& anchor, Hit owner,
                     std::function<void(std::wstring)> pick) {
    constexpr int kColumns = 8;
    constexpr float kCell = 36;
    int rows = (ARRAYSIZE(kEmojis) + kColumns - 1) / kColumns;
    D2D1_SIZE_F size{kColumns * kCell + 16, rows * kCell + 16 + 36};
    OpenFlyout(anchor, owner, size, [pick](D2D1_RECT_F r) {
        for (int i = 0; i < (int)ARRAYSIZE(kEmojis); i++) {
            float x = r.left + 8 + (i % kColumns) * kCell;
            float y = r.top + 8 + (i / kColumns) * kCell;
            D2D1_RECT_F cell{x, y, x + kCell, y + kCell};
            Hit h = HIT(i);
            Fill(Inset(cell, 2, 2), SubtleFill(h), 4);
            Emoji(kEmojis[i], g_fonts.emoji20, cell);
            Add(cell, h).click = [pick, i] {
                CloseFlyout();
                pick(kEmojis[i]);
            };
        }
        D2D1_RECT_F none{r.left + 8, r.bottom - 8 - 32, r.right - 8, r.bottom - 8};
        Button(none, HIT(0), Tr(L"Bez emoji", L"No emoji"), kIconCancel, kSubtle).click = [pick] {
            CloseFlyout();
            pick(L"");
        };
    });
}

// A drop-down value picker.
Region& Combo(const D2D1_RECT_F& r, Hit h, const std::vector<std::wstring>& options,
              int selected, std::function<void(int)> onPick) {
    const Theme& t = g_theme;
    Fill(r, t.control, 4);
    Fill(r, SubtleFill(h), 4);
    Stroke(r, t.controlStroke, 4);
    std::wstring value = selected >= 0 && selected < (int)options.size() ? options[selected]
                                                                         : L"";
    Text(value, g_fonts.row, {r.left + 12, r.top, r.right - 32, r.bottom}, t.text);
    Glyph(kIconChevronDown, g_fonts.icon10, {r.right - 30, r.top, r.right - 10, r.bottom},
          t.text2);
    Region& region = Add(r, h);
    region.click = [r, h, options, selected, onPick] {
        if (ToggleFlyout(h)) {
            return;
        }
        std::vector<MenuItem> items;
        for (int i = 0; i < (int)options.size(); i++) {
            MenuItem item;
            item.label = options[i];
            item.checked = i == selected;
            item.action = [onPick, i] { onPick(i); };
            items.push_back(std::move(item));
        }
        OpenMenu(r, h, std::move(items), r.right - r.left);
    };
    return region;
}

////////////////////////////////////////////////////////////////////////////////
// Dialogs: a modal card over a dimmed window.

void ShowDialog(const std::wstring& title, float width, std::function<float(D2D1_RECT_F)> draw,
                std::function<void()> onEnter = nullptr,
                std::function<void()> onEscape = nullptr) {
    Dialog& d = g_s->dialog;
    d.open = true;
    d.title = title;
    d.width = width;
    d.draw = std::move(draw);
    d.onEnter = std::move(onEnter);
    d.onEscape = std::move(onEscape);
    g_s->flyout = {};
    Invalidate(*g_s);
}

void CloseDialog() {
    Dialog& d = g_s->dialog;
    if (d.open) {
        d.open = false;
        d.draw = nullptr;
        d.onEnter = d.onEscape = nullptr;
        Invalidate(*g_s);
        SetFocus(g_s->hwnd);
    }
}

void DrawDialog() {
    Dialog& d = g_s->dialog;
    if (!d.open) {
        return;
    }
    const Theme& t = g_theme;
    D2D1_SIZE_F win = SizeDip(*g_s);
    Fill({0, 0, win.width, win.height}, t.smoke);
    Add({0, 0, win.width, win.height}, HIT(0));  // Blocks the window below.
    float w = std::min(d.width, win.width - 24);
    float h = std::min(d.height, win.height - 24);
    float x = (win.width - w) / 2, y = std::max(12.f, (win.height - h) / 2);
    D2D1_RECT_F r{x, y, x + w, y + d.height};
    for (int i = 1; i <= 6; i++) {
        Fill({r.left - i, r.top - i + 4, r.right + i, r.bottom + i + 4},
             Rgba(0, t.dark ? 0.06f : 0.025f), 8 + i);
    }
    Fill(r, t.flyout, 8);
    Stroke(r, t.dark ? Rgba(0, 0.4f) : Rgba(0, 0.1f), 8);
    Add(r, HIT(1));
    Text(d.title, g_fonts.pageTitle, {r.left + 24, r.top + 18, r.right - 24, r.top + 50},
         t.text);
    float bottom = d.draw ? d.draw({r.left + 24, r.top + 62, r.right - 24, r.bottom}) : r.top;
    float needed = bottom - r.top + 24;
    if (std::fabs(needed - d.height) > 0.5f) {
        d.height = needed;
        g_s->again = true;
    }
}

// The usual button row at the bottom of a dialog; returns its bottom.
float DialogButtons(const D2D1_RECT_F& body, float y, const std::wstring& ok, bool okEnabled,
                    std::function<void()> onOk,
                    const std::wstring& cancel = Tr(L"Anuluj", L"Cancel")) {
    float bw = (body.right - body.left - 8) / 2;
    Button({body.left, y, body.left + bw, y + 32}, HIT(0), ok, 0, kAccent, !okEnabled)
        .click = onOk;
    Button({body.right - bw, y, body.right, y + 32}, HIT(0), cancel).click = [] {
        CloseDialog();
    };
    return y + 32;
}

////////////////////////////////////////////////////////////////////////////////
// Toast: a short message at the bottom of the list, with an optional action.

struct Toast {
    bool visible = false;
    std::wstring text, action;
    std::function<void()> onAction, onExpire;
} g_toast;

void ExpireToast() {
    KillTimer(g_hwnd, kTimerToast);
    if (g_toast.visible) {
        std::function<void()> expire = std::move(g_toast.onExpire);
        g_toast = {};
        if (expire) {
            expire();
        }
        Redraw();
    }
}

void ShowToast(const std::wstring& text, const std::wstring& action = L"",
               std::function<void()> onAction = nullptr,
               std::function<void()> onExpire = nullptr) {
    ExpireToast();
    g_toast.visible = true;
    g_toast.text = text;
    g_toast.action = action;
    g_toast.onAction = std::move(onAction);
    g_toast.onExpire = std::move(onExpire);
    SetTimer(g_hwnd, kTimerToast, kToastMs, nullptr);
    Redraw();
}

void DrawToast(const D2D1_RECT_F& area) {
    if (!g_toast.visible) {
        return;
    }
    const Theme& t = g_theme;
    float actionWidth = g_toast.action.empty() ? 0 : ButtonWidth(g_toast.action);
    float textWidth = area.right - area.left - 28 - (actionWidth ? actionWidth + 16 : 0) - 32;
    float textHeight = std::max(20.f, TextHeight(g_toast.text, g_fonts.body, textWidth));
    float h = std::max(48.f, textHeight + 24);
    D2D1_RECT_F r{area.left + 4, area.bottom - 8 - h, area.right - 4, area.bottom - 8};
    for (int i = 1; i <= 3; i++) {
        Fill({r.left - i, r.top - i + 2, r.right + i, r.bottom + i + 2},
             Rgba(0, t.dark ? 0.06f : 0.03f), 6 + i);
    }
    Fill(r, t.flyout, 6);
    Stroke(r, t.controlStroke, 6);
    Add(r, HIT(0));
    Glyph(kIconInfo, g_fonts.icon16, {r.left + 12, r.top, r.left + 32, r.bottom}, t.accent);
    Text(g_toast.text, g_fonts.body,
         {r.left + 40, (r.top + r.bottom - textHeight) / 2, r.left + 40 + textWidth,
          (r.top + r.bottom + textHeight) / 2},
         t.text);
    if (actionWidth) {
        D2D1_RECT_F b{r.right - 12 - actionWidth, (r.top + r.bottom) / 2 - 16, r.right - 12,
                      (r.top + r.bottom) / 2 + 16};
        Button(b, HIT(0), g_toast.action).click = [] {
            KillTimer(g_hwnd, kTimerToast);
            std::function<void()> action = std::move(g_toast.onAction);
            g_toast = {};
            if (action) {
                action();
            }
            Redraw();
        };
    }
}

////////////////////////////////////////////////////////////////////////////////
// Tooltips

void DrawTooltip() {
    Surface& s = *g_s;
    if (!s.tipShown || !s.hot) {
        return;
    }
    const Region* found = nullptr;
    for (const Region& r : s.regions) {
        if (r.hit == s.hot && !r.tip.empty()) {
            found = &r;
        }
    }
    if (!found) {
        return;
    }
    const Theme& t = g_theme;
    D2D1_SIZE_F win = SizeDip(s);
    float w = std::min(TextWidth(found->tip, g_fonts.small) + 18, win.width - 16);
    float h = 26;
    float x = std::clamp((found->r.left + found->r.right - w) / 2, 8.f, win.width - 8 - w);
    float y = found->r.bottom + 6;
    if (y + h > win.height - 4) {
        y = found->r.top - 6 - h;
    }
    D2D1_RECT_F r{x, y, x + w, y + h};
    Fill(r, t.flyout, 4);
    Stroke(r, t.controlStroke, 4);
    Text(found->tip, g_fonts.small, {r.left + 9, r.top, r.right - 6, r.bottom}, t.text);
}

////////////////////////////////////////////////////////////////////////////////
// Scrolling

std::vector<Scroller*> g_animating;
ULONGLONG g_scrollTick;

void Scroller::To(float t) {
    target = std::clamp(t, 0.f, Max());
    if (std::find(g_animating.begin(), g_animating.end(), this) == g_animating.end()) {
        g_animating.push_back(this);
    }
    g_scrollTick = GetTickCount64();
    SetTimer(g_hwnd, kTimerScroll, 10, nullptr);
}

void OnScrollTimer() {
    ULONGLONG now = GetTickCount64();
    float dt = (now - g_scrollTick) / 1000.f;
    g_scrollTick = now;
    for (size_t i = 0; i < g_animating.size();) {
        Scroller* s = g_animating[i];
        float diff = s->target - s->pos;
        if (std::fabs(diff) < 0.5f) {
            s->pos = s->target;
            g_animating.erase(g_animating.begin() + i);
        } else {
            s->pos += diff * std::min(1.f, dt * 18);
            i++;
        }
    }
    if (g_animating.empty()) {
        KillTimer(g_hwnd, kTimerScroll);
    }
    Redraw();
}

// Starts a scrolling area; returns the y of the content's top.
float BeginScroll(Scroller& s, const D2D1_RECT_F& view) {
    s.view = view.bottom - view.top;
    Add(view, HIT(&s)).scroll = &s;
    PushClip(view);
    return view.top - s.pos;
}

void EndScroll(Scroller& s, const D2D1_RECT_F& view, float contentHeight) {
    PopClip();
    s.content = contentHeight;
    s.Clamp();
    float max = s.Max();
    if (max <= 0) {
        return;
    }
    const Theme& t = g_theme;
    D2D1_RECT_F bar{view.right - 10, view.top + 2, view.right, view.bottom - 2};
    float track = bar.bottom - bar.top;
    float thumb = std::max(24.f, track * s.view / contentHeight);
    float pos = bar.top + (track - thumb) * std::clamp(s.pos / max, 0.f, 1.f);
    Hit h = HIT(&s);
    bool wide = Hot(h) || (g_s->dragging && g_s->pressed == h);
    float w = wide ? 6 : 2;
    float x = bar.right - 3 - w;
    Fill({x, pos, x + w, pos + thumb}, t.strong, w / 2);
    Scroller* sp = &s;
    Add(bar, h).drag = [sp, bar, track, thumb, pos](Drag phase, D2D1_POINT_2F p) {
        if (phase == Drag::Start) {
            // Grabs the thumb where it was clicked, or centers it on a track click.
            sp->grab = (p.y >= pos && p.y < pos + thumb) ? p.y - pos : thumb / 2;
            auto found = std::find(g_animating.begin(), g_animating.end(), sp);
            if (found != g_animating.end()) {
                g_animating.erase(found);
            }
        }
        float ratio = (p.y - sp->grab - bar.top) / std::max(1.f, track - thumb);
        sp->pos = sp->target = std::clamp(ratio, 0.f, 1.f) * sp->Max();
        Invalidate(*g_s);
    };
}

void EnsureVisible(Scroller& s, float top, float bottom) {
    if (top < s.target) {
        s.To(top);
    } else if (bottom > s.target + s.view) {
        s.To(bottom - s.view);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Text fields: native EDIT controls, so that typing, Polish characters,
// selection and the clipboard work as everywhere else. They're placed by the
// frame that draws them and hidden when a frame doesn't. GDI can't draw on
// the translucent window, so they paint through an opaque buffer.

enum FieldId {
    FSearch,
    FCategoryName,
    FTagAdd,
    FEditText,
    FTemplateName,
    FTemplateBody,
    FPassword,
    FCount,
};

struct Field {
    HWND hwnd = nullptr;
    bool multiline = false;
    bool placed = false;
    bool swallowChar = false;
    RECT rc{};
    std::wstring placeholder;
    std::function<void()> onChange;
    std::function<bool(UINT)> onKey;  // Returns true when it handled the key.
};

Field g_fields[FCount];
WNDPROC g_editProc;
HFONT g_editFont;
bool g_settingText;  // Ignores EN_CHANGE caused by the code itself.

bool OnFieldKey(Field& f, UINT vk);

LRESULT CALLBACK EditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Field* f = (Field*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
    switch (msg) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if (f && OnFieldKey(*f, (UINT)wParam)) {
                f->swallowChar = true;
                return 0;
            }
            break;
        case WM_CHAR:
        case WM_SYSCHAR:
            if (f && f->swallowChar) {
                f->swallowChar = false;
                return 0;
            }
            // Ctrl+Backspace would insert a box character.
            if (wParam == 0x7F || (f && !f->multiline && (wParam == VK_RETURN ||
                                                      wParam == VK_ESCAPE ||
                                                      wParam == VK_TAB))) {
                return 0;
            }
            break;
        case WM_KEYUP:
            if (f) {
                f->swallowChar = false;
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
                if (f && GetWindowTextLengthW(hwnd) == 0 && !f->placeholder.empty()) {
                    HGDIOBJ old = SelectObject(mem, g_editFont);
                    SetBkMode(mem, TRANSPARENT);
                    SetTextColor(mem, g_theme.editPlaceholder);
                    RECT text = rc;
                    text.left += LOWORD(SendMessageW(hwnd, EM_GETMARGINS, 0, 0));
                    DrawTextW(mem, f->placeholder.c_str(), -1, &text,
                              (f->multiline ? DT_WORDBREAK : DT_SINGLELINE | DT_VCENTER) |
                                  DT_NOPREFIX);
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
        case WM_MOUSEWHEEL:
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
        case WM_SETTEXT:
        case WM_PASTE:
        case WM_CUT:
        case WM_CLEAR:
        case WM_UNDO:
        case EM_SETSEL:
        case EM_REPLACESEL:
            if (msg != WM_MOUSEMOVE || (wParam & MK_LBUTTON)) {
                InvalidateRect(hwnd, nullptr, FALSE);
                UpdateWindow(hwnd);
            }
            if (msg == WM_SETFOCUS || msg == WM_KILLFOCUS || (f && f->multiline)) {
                Redraw();  // Focus line and scroll indicator.
            }
            break;
    }
    return result;
}

void UpdateEditFont() {
    HFONT old = g_editFont;
    g_editFont = CreateFontW(-MulDiv(14, g_main.dpi, 96), 0, 0, 0, FW_NORMAL, 0, 0, 0,
                             DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");
    for (Field& f : g_fields) {
        if (f.hwnd) {
            SendMessageW(f.hwnd, WM_SETFONT, (WPARAM)g_editFont, TRUE);
        }
    }
    if (old) {
        DeleteObject(old);
    }
}

std::wstring FieldText(int id) {
    HWND hwnd = g_fields[id].hwnd;
    if (!hwnd) {
        return {};
    }
    int length = GetWindowTextLengthW(hwnd);
    std::wstring s(length, L'\0');
    GetWindowTextW(hwnd, s.data(), length + 1);
    return s;
}

HWND EnsureField(int id, bool multiline, bool password) {
    Field& f = g_fields[id];
    if (!f.hwnd) {
        DWORD style = WS_CHILD | (multiline ? ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN
                                            : ES_AUTOHSCROLL) |
                      (password ? ES_PASSWORD : 0);
        f.hwnd = CreateWindowExW(0, L"EDIT", nullptr, style, 0, 0, 0, 0, g_hwnd, nullptr,
                                 GetModuleHandleW(nullptr), nullptr);
        f.multiline = multiline;
        SetWindowLongPtrW(f.hwnd, GWLP_USERDATA, (LONG_PTR)&f);
        g_editProc = (WNDPROC)SetWindowLongPtrW(f.hwnd, GWLP_WNDPROC, (LONG_PTR)EditProc);
        SendMessageW(f.hwnd, WM_SETFONT, (WPARAM)g_editFont, FALSE);
        SendMessageW(f.hwnd, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(0, 0));
        if (password) {
            SendMessageW(f.hwnd, EM_SETPASSWORDCHAR, 0x25CF, 0);
        }
    }
    return f.hwnd;
}

void SetFieldText(int id, const std::wstring& s) {
    HWND hwnd = EnsureField(id, id == FEditText || id == FTemplateBody, id == FPassword);
    g_settingText = true;
    SetWindowTextW(hwnd, s.c_str());
    g_settingText = false;
    SendMessageW(hwnd, EM_SETSEL, s.size(), s.size());
}

void FocusField(int id) {
    if (g_fields[id].hwnd) {
        SetFocus(g_fields[id].hwnd);
    }
}

bool FieldFocused(int id) {
    return g_fields[id].hwnd && GetFocus() == g_fields[id].hwnd;
}

// Draws a text field and places its EDIT inside. trailing and leading reserve
// room for a button on the right and an icon on the left.
Field& PlaceField(int id, const D2D1_RECT_F& r, const std::wstring& placeholder,
                  float trailing = 0, float leading = 0) {
    const Theme& t = g_theme;
    bool multiline = id == FEditText || id == FTemplateBody;
    Field& f = g_fields[id];
    HWND hwnd = EnsureField(id, multiline, id == FPassword);
    f.placeholder = placeholder;
    bool focused = GetFocus() == hwnd;
    Fill(r, t.search, 4);
    Stroke(r, t.controlStroke, 4);
    if (focused) {
        Fill({r.left + 1, r.bottom - 2, r.right - 1, r.bottom}, t.accent, 1);
    } else {
        Fill({r.left + 4, r.bottom - 1, r.right - 4, r.bottom}, t.strong);
    }
    D2D1_RECT_F inner = multiline ? D2D1_RECT_F{r.left + 10, r.top + 7, r.right - 10,
                                                r.bottom - 7}
                                  : D2D1_RECT_F{r.left + 10 + leading, (r.top + r.bottom) / 2 - 10,
                                                r.right - 10 - trailing,
                                                (r.top + r.bottom) / 2 + 10};
    Region& region = Add(r, HIT(id));
    region.cursor = IDC_IBEAM;
    region.click = [hwnd] { SetFocus(hwnd); };
    // A field that is partly scrolled out of view stays hidden.
    if (!g_s->clips.empty() && !IsEmpty(inner)) {
        D2D1_RECT_F clip = g_s->clips.back();
        if (inner.top < clip.top || inner.bottom > clip.bottom) {
            return f;
        }
    }
    RECT rc{Px(inner.left), Px(inner.top), Px(inner.right), Px(inner.bottom)};
    f.placed = true;
    if (!EqualRect(&rc, &f.rc) || !IsWindowVisible(hwnd)) {
        f.rc = rc;
        SetWindowPos(hwnd, nullptr, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
                     SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    }
    if (multiline) {
        // A thin scroll indicator instead of a scrollbar.
        int first = (int)SendMessageW(hwnd, EM_GETFIRSTVISIBLELINE, 0, 0);
        int lines = (int)SendMessageW(hwnd, EM_GETLINECOUNT, 0, 0);
        float lineHeight = 19;
        int visible = std::max(1, (int)((inner.bottom - inner.top) / lineHeight));
        if (lines > visible) {
            float track = r.bottom - r.top - 8;
            float thumb = std::max(16.f, track * visible / lines);
            float y = r.top + 4 + (track - thumb) * first / std::max(1, lines - visible);
            Fill({r.right - 5, y, r.right - 3, y + thumb}, t.strong, 1);
        }
    }
    return f;
}

void BeginFields() {
    for (Field& f : g_fields) {
        f.placed = false;
    }
}

void EndFields() {
    for (Field& f : g_fields) {
        if (f.hwnd && !f.placed && IsWindowVisible(f.hwnd)) {
            if (GetFocus() == f.hwnd) {
                SetFocus(g_hwnd);
            }
            ShowWindow(f.hwnd, SW_HIDE);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Painting and mouse input, shared by the clipboard window and the editor.

void DiscardGraphics(Surface& s) {
    for (auto& [id, bitmap] : s.bitmaps) {
        bitmap->Release();
    }
    s.bitmaps.clear();
    SafeRelease(s.brush);
    SafeRelease(s.rt);
}

bool EnsureGraphics(Surface& s) {
    if (s.rt) {
        return true;
    }
    RECT rc;
    GetClientRect(s.hwnd, &rc);
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
        (float)s.dpi, (float)s.dpi);
    if (FAILED(g_d2d->CreateHwndRenderTarget(
            props,
            D2D1::HwndRenderTargetProperties(
                s.hwnd, D2D1::SizeU(std::max(1L, rc.right), std::max(1L, rc.bottom))),
            &s.rt)) ||
        FAILED(s.rt->CreateSolidColorBrush(D2D1::ColorF(0), &s.brush))) {
        DiscardGraphics(s);
        return false;
    }
    // ClearType needs an opaque background; the window is translucent.
    s.rt->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
    return true;
}

Region* RegionAt(Surface& s, D2D1_POINT_2F p) {
    for (size_t i = s.regions.size(); i-- > 0;) {
        if (In(s.regions[i].r, p)) {
            return &s.regions[i];
        }
    }
    return nullptr;
}

void UpdateHot(Surface& s) {
    Region* r = s.mouseInside ? RegionAt(s, s.mouse) : nullptr;
    Hit hot = r ? r->hit : Hit{};
    if (!(hot == s.hot)) {
        s.hot = hot;
        if (s.tipShown) {
            s.tipShown = false;
        }
        KillTimer(s.hwnd, kTimerTooltip);
        if (r && !r->tip.empty()) {
            SetTimer(s.hwnd, kTimerTooltip, 600, nullptr);
        }
        Invalidate(s);
    }
}

void Paint(Surface& s) {
    Surface* prev = g_s;
    ID2D1RenderTarget* prevRt = g_rt;
    ID2D1SolidColorBrush* prevBrush = g_brush;
    g_s = &s;
    if (EnsureGraphics(s)) {
        g_rt = s.rt;
        g_brush = s.brush;
        bool main = &s == &g_main;
        s.rt->BeginDraw();
        for (int pass = 0; pass < 3; pass++) {
            s.again = false;
            s.regions.clear();
            s.clips.clear();
            if (main) {
                BeginFields();
            }
            s.rt->SetTransform(D2D1::Matrix3x2F::Identity());
            s.rt->Clear(D2D1::ColorF(0, 0));
            s.render();
            if (!s.again) {
                break;
            }
        }
        if (main) {
            EndFields();
        }
        if (s.rt->EndDraw() == D2DERR_RECREATE_TARGET) {
            DiscardGraphics(s);
            Invalidate(s);
        }
    }
    g_s = prev;
    g_rt = prevRt;
    g_brush = prevBrush;
    UpdateHot(s);
}

D2D1_POINT_2F MousePoint(Surface& s, LPARAM lParam) {
    return {GET_X_LPARAM(lParam) * 96.f / s.dpi, GET_Y_LPARAM(lParam) * 96.f / s.dpi};
}

// Handles the mouse for a surface; returns true when the message was used.
bool HandleMouse(Surface& s, UINT msg, WPARAM wParam, LPARAM lParam) {
    Surface* prev = g_s;
    g_s = &s;
    bool handled = true;
    switch (msg) {
        case WM_MOUSEMOVE:
            s.mouse = MousePoint(s, lParam);
            if (!s.mouseInside) {
                s.mouseInside = true;
                TRACKMOUSEEVENT tme{sizeof(tme), TME_LEAVE, s.hwnd};
                TrackMouseEvent(&tme);
            }
            if (s.dragging && s.dragFn) {
                auto fn = s.dragFn;
                fn(Drag::Move, s.mouse);
            } else {
                UpdateHot(s);
            }
            break;
        case WM_MOUSELEAVE:
            s.mouseInside = false;
            if (!s.dragging) {
                UpdateHot(s);
            }
            break;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK: {
            UpdateWindow(s.hwnd);  // Regions of the current state.
            s.mouse = MousePoint(s, lParam);
            Region* r = RegionAt(s, s.mouse);
            if (msg == WM_LBUTTONDBLCLK && r && r->dblclick) {
                auto fn = r->dblclick;
                fn();
                break;
            }
            s.pressed = r ? r->hit : Hit{};
            if (!r || r->cursor != IDC_IBEAM) {
                SetFocus(s.hwnd);
            }
            SetCapture(s.hwnd);
            if (r && r->drag) {
                s.dragging = true;
                s.dragFn = r->drag;
                auto fn = s.dragFn;
                fn(Drag::Start, s.mouse);
            }
            Invalidate(s);
            break;
        }
        case WM_LBUTTONUP: {
            ReleaseCapture();
            s.mouse = MousePoint(s, lParam);
            Hit pressed = s.pressed;
            s.pressed = {};
            if (s.dragging) {
                s.dragging = false;
                auto fn = std::move(s.dragFn);
                s.dragFn = nullptr;
                if (fn) {
                    fn(Drag::End, s.mouse);
                }
            } else if (Region* r = RegionAt(s, s.mouse); r && r->hit == pressed && r->click) {
                auto fn = r->click;
                fn();
            }
            Invalidate(s);
            UpdateHot(s);
            break;
        }
        case WM_RBUTTONUP: {
            UpdateWindow(s.hwnd);
            Region* r = RegionAt(s, MousePoint(s, lParam));
            if (r && r->rclick) {
                auto fn = r->rclick;
                fn();
            }
            break;
        }
        case WM_MOUSEWHEEL: {
            POINT pt{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            ScreenToClient(s.hwnd, &pt);
            D2D1_POINT_2F p{pt.x * 96.f / s.dpi, pt.y * 96.f / s.dpi};
            for (size_t i = s.regions.size(); i-- > 0;) {
                if (s.regions[i].scroll && In(s.regions[i].r, p)) {
                    s.regions[i].scroll->By(-GET_WHEEL_DELTA_WPARAM(wParam) / 120.f * 3 * 42);
                    break;
                }
            }
            break;
        }
        case WM_SETCURSOR:
            if (LOWORD(lParam) == HTCLIENT) {
                Region* r = s.mouseInside ? RegionAt(s, s.mouse) : nullptr;
                SetCursor(LoadCursorW(nullptr, r && r->cursor ? r->cursor : IDC_ARROW));
            } else {
                handled = false;
            }
            break;
        case WM_TIMER:
            if (wParam == kTimerTooltip) {
                KillTimer(s.hwnd, kTimerTooltip);
                s.tipShown = true;
                Invalidate(s);
            } else {
                handled = false;
            }
            break;
        default:
            handled = false;
    }
    g_s = prev;
    return handled;
}

////////////////////////////////////////////////////////////////////////////////
// Window state

enum class Tab { History, Pinned, Templates };
enum class Page { Main, Settings, Trash };

constexpr UINT64 kAllCategories = ~0ULL;

Tab g_tab = Tab::History;
Page g_page = Page::Main;
bool g_onTop;  // The pin in the title bar: stays open and on top.
bool g_searchOpen;
std::wstring g_query;
UINT64 g_selectedId;
UINT64 g_detailsId;  // The item shown in the details panel, or 0.
bool g_editingText;
UINT64 g_pinFilter = kAllCategories;
std::wstring g_tagFilter;
std::vector<Item> g_cleared;  // Kept while "Undo" is offered.
D2D1_SIZE_F g_sizeDip{kDefaultWidth, kDefaultHeight};  // Without the details panel.
bool g_placing;  // Set while the window is moved by code: see WM_DPICHANGED.
bool g_sizing;
ULONGLONG g_hiddenTick;
int g_modal;  // Open system dialogs; the window stays while they're up.
UINT g_keys[ScCount];
int g_recordTarget = -1;  // Shortcut being recorded, or kRecordTemplate.
constexpr int kRecordTemplate = 1000;
std::function<void(UINT)> g_recordDone;

Scroller g_listScroll, g_catScroll, g_detailScroll, g_pageScroll;

struct Row {
    enum Type : BYTE { Header, Entry, Section, NewCategory };
    Type type = Entry;
    UINT64 id = 0;  // Item, or category for a section (0: no category).
    std::wstring header;
    float y = 0;
    float h = 0;
};

std::vector<Row> g_rows;
float g_rowsHeight;
std::unordered_set<std::string> g_pinnedHashes;
std::vector<std::pair<std::wstring, int>> g_tagCounts;

void HidePanel(bool restoreFocus);
void ResizeForDetails(bool open);
void OpenEditor(UINT64 id, bool viewOnly);
void ApplyLook();

bool Live(const Item& it) {
    return it.deletedTime == 0;
}

std::string HashKey(const Item& it) {
    return std::string((const char*)it.hash, sizeof(it.hash));
}

size_t CountItems(Kind kind) {
    return std::count_if(g_items.begin(), g_items.end(),
                         [kind](const Item& it) { return it.kind == kind && Live(it); });
}

size_t CountTrash() {
    return std::count_if(g_items.begin(), g_items.end(),
                         [](const Item& it) { return !Live(it); });
}

bool SideDetails() {
    return Setting(L"detailsMode", 0) == 0;
}

bool SectionsLayout() {
    return Setting(L"pinnedLayout", 0) == 1;
}

// A short name for messages: the template's name or the start of the text.
std::wstring ItemTitle(const Item& it) {
    std::wstring s = it.kind == Kind::Template && !it.name.empty() ? it.name : it.preview;
    if (s.size() > 40) {
        s = s.substr(0, 39) + L"…";
    }
    return s;
}

std::wstring CategoryName(UINT64 id) {
    const Category* c = FindCategory(id);
    return c ? c->name : Tr(L"Bez kategorii", L"Uncategorized");
}

bool HasTag(const Item& it, const std::wstring& tag) {
    return std::any_of(it.tags.begin(), it.tags.end(),
                       [&](const std::wstring& t) { return EqualsNoCase(t, tag); });
}

// Every word of the search must match; "#word" looks at tags only.
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
        if (word[0] == L'#') {
            word.erase(0, 1);
            if (!word.empty() &&
                !std::any_of(it.tags.begin(), it.tags.end(),
                             [&](const std::wstring& t) { return ContainsNoCase(t, word); })) {
                return false;
            }
            continue;
        }
        bool found = ContainsNoCase(it.text, word) || ContainsNoCase(it.files, word) ||
                     ContainsNoCase(it.source, word) || ContainsNoCase(it.preview, word) ||
                     ContainsNoCase(it.name, word) ||
                     std::any_of(it.tags.begin(), it.tags.end(),
                                 [&](const std::wstring& t) { return ContainsNoCase(t, word); });
        if (!found && it.kind == Kind::Pinned) {
            const Category* c = FindCategory(it.category);
            found = c && ContainsNoCase(c->name, word);
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

bool MatchesPinnedFilters(const Item& it) {
    return (g_tagFilter.empty() || HasTag(it, g_tagFilter)) && Matches(it);
}

////////////////////////////////////////////////////////////////////////////////
// List rows

constexpr float kTwoLineHeight = 56;
constexpr float kSectionHeight = 40;

void RebuildRows() {
    g_rows.clear();
    g_pinnedHashes.clear();
    std::map<std::wstring, int, bool (*)(const std::wstring&, const std::wstring&)> tags(
        LessNoCase);
    for (const Item& it : g_items) {
        if (it.kind == Kind::Pinned && Live(it)) {
            g_pinnedHashes.insert(HashKey(it));
            for (const std::wstring& tag : it.tags) {
                tags[tag]++;
            }
        }
    }
    g_tagCounts.assign(tags.begin(), tags.end());
    if (!g_tagFilter.empty() && !tags.contains(g_tagFilter)) {
        g_tagFilter.clear();
    }

    std::vector<const Item*> list;
    float y = 0;
    auto entry = [&](const Item* it, float h) {
        g_rows.push_back({Row::Entry, it->id, {}, y, h});
        y += h + kRowGap;
    };

    if (g_page == Page::Trash) {
        for (const Item& it : g_items) {
            if (!Live(it) && Matches(it)) {
                list.push_back(&it);
            }
        }
        std::stable_sort(list.begin(), list.end(), [](const Item* a, const Item* b) {
            return a->deletedTime > b->deletedTime;
        });
        for (const Item* it : list) {
            entry(it, kTwoLineHeight);
        }
    } else if (g_tab == Tab::History) {
        for (const Item& it : g_items) {
            if (it.kind == Kind::History && Live(it) && Matches(it)) {
                list.push_back(&it);
            }
        }
        std::stable_sort(list.begin(), list.end(),
                         [](const Item* a, const Item* b) { return a->time > b->time; });
        UINT64 lastDay = 0;
        for (const Item* it : list) {
            UINT64 day = LocalDay(it->time);
            if (day != lastDay) {
                lastDay = day;
                g_rows.push_back({Row::Header, 0, FormatDay(it->time), y, kHeaderHeight});
                y += kHeaderHeight;
            }
            entry(it, kRowHeight);
        }
    } else if (g_tab == Tab::Pinned) {
        for (const Item& it : g_items) {
            if (it.kind == Kind::Pinned && Live(it) && MatchesPinnedFilters(it)) {
                list.push_back(&it);
            }
        }
        std::stable_sort(list.begin(), list.end(), [](const Item* a, const Item* b) {
            return a->pinnedTime > b->pinnedTime;
        });
        if (SectionsLayout()) {
            bool filtering = !g_query.empty() || !g_tagFilter.empty();
            std::vector<UINT64> sections;
            for (const Category& c : g_categories) {
                sections.push_back(c.id);
            }
            sections.push_back(0);
            for (UINT64 id : sections) {
                size_t count = std::count_if(list.begin(), list.end(), [&](const Item* it) {
                    return (FindCategory(it->category) ? it->category : 0) == id;
                });
                if (filtering && !count) {
                    continue;
                }
                if (id == 0 && !count && !g_categories.empty()) {
                    continue;
                }
                g_rows.push_back({Row::Section, id, {}, y, kSectionHeight});
                y += kSectionHeight + kRowGap;
                const Category* c = FindCategory(id);
                bool collapsed = c ? c->collapsed : Setting(L"noCategoryCollapsed", 0) != 0;
                if (collapsed && !filtering) {
                    continue;
                }
                for (const Item* it : list) {
                    if ((FindCategory(it->category) ? it->category : 0) == id) {
                        entry(it, kTwoLineHeight);
                    }
                }
            }
            if (!filtering) {
                g_rows.push_back({Row::NewCategory, 0, {}, y, kSectionHeight});
                y += kSectionHeight + kRowGap;
            }
        } else {
            for (const Item* it : list) {
                UINT64 category = FindCategory(it->category) ? it->category : 0;
                if (g_pinFilter == kAllCategories || g_pinFilter == category) {
                    entry(it, kTwoLineHeight);
                }
            }
        }
    } else {
        for (const Item& it : g_items) {
            if (it.kind == Kind::Template && Live(it) && Matches(it)) {
                list.push_back(&it);
            }
        }
        std::stable_sort(list.begin(), list.end(),
                         [](const Item* a, const Item* b) { return a->time < b->time; });
        for (const Item* it : list) {
            entry(it, kTwoLineHeight);
        }
    }
    g_rowsHeight = y + 4;
}

int SelectedRow() {
    for (int i = 0; i < (int)g_rows.size(); i++) {
        if (g_rows[i].type == Row::Entry && g_rows[i].id == g_selectedId) {
            return i;
        }
    }
    return -1;
}

int FirstEntryRow() {
    for (int i = 0; i < (int)g_rows.size(); i++) {
        if (g_rows[i].type == Row::Entry) {
            return i;
        }
    }
    return -1;
}

int EntryCount() {
    return (int)std::count_if(g_rows.begin(), g_rows.end(),
                              [](const Row& r) { return r.type == Row::Entry; });
}

void EnsureRowVisible(int row) {
    if (row < 0) {
        return;
    }
    const Row& r = g_rows[row];
    float top = r.y;
    // Keep the day header above the first item of a day visible too.
    if (row > 0 && g_rows[row - 1].type == Row::Header) {
        top = g_rows[row - 1].y;
    }
    EnsureVisible(g_listScroll, top, r.y + r.h + 4);
}

void Select(int row) {
    if (row >= 0 && row < (int)g_rows.size() && g_rows[row].type == Row::Entry) {
        g_selectedId = g_rows[row].id;
        EnsureRowVisible(row);
        if (g_detailsId && g_detailsId != g_selectedId && !g_editingText) {
            g_detailsId = g_selectedId;
            g_detailScroll.Reset();
        }
        Redraw();
    }
}

void MoveSelection(int step) {
    int row = SelectedRow();
    if (row < 0) {
        Select(FirstEntryRow());
        return;
    }
    for (int i = row + step; i >= 0 && i < (int)g_rows.size(); i += step) {
        if (g_rows[i].type == Row::Entry) {
            Select(i);
            return;
        }
    }
}

void CloseDetails();

// Rebuilds the list after a change, keeping the selection when possible.
void Refresh() {
    RebuildRows();
    if (SelectedRow() < 0) {
        int first = FirstEntryRow();
        g_selectedId = first >= 0 ? g_rows[first].id : 0;
    }
    if (g_detailsId) {
        Item* it = FindItem(g_detailsId);
        if (!it || !Live(*it)) {
            CloseDetails();
        }
    }
    g_listScroll.content = g_rowsHeight;
    g_listScroll.Clamp();
    Redraw();
}

////////////////////////////////////////////////////////////////////////////////
// Shortcut keys

UINT CurrentMods() {
    UINT mods = 0;
    if (GetKeyState(VK_CONTROL) < 0) {
        mods |= MOD_CONTROL;
    }
    if (GetKeyState(VK_MENU) < 0) {
        mods |= MOD_ALT;
    }
    if (GetKeyState(VK_SHIFT) < 0) {
        mods |= MOD_SHIFT;
    }
    if (GetKeyState(VK_LWIN) < 0 || GetKeyState(VK_RWIN) < 0) {
        mods |= MOD_WIN;
    }
    return mods;
}

std::wstring VkName(UINT vk) {
    switch (vk) {
        case VK_RETURN: return L"Enter";
        case VK_SPACE:
            return Tr(L"Spacja", L"Space");
        case VK_ESCAPE: return L"Esc";
        case VK_DELETE: return L"Del";
        case VK_TAB: return L"Tab";
        case VK_BACK: return L"Backspace";
        case VK_INSERT: return L"Insert";
        case VK_HOME: return L"Home";
        case VK_END: return L"End";
        case VK_PRIOR: return L"PgUp";
        case VK_NEXT: return L"PgDn";
        case VK_UP: return L"↑";
        case VK_DOWN: return L"↓";
        case VK_LEFT: return L"←";
        case VK_RIGHT: return L"→";
        case VK_OEM_PLUS: return L"Plus";
        case VK_OEM_MINUS: return L"Minus";
        case VK_ADD: return L"Num +";
        case VK_SUBTRACT: return L"Num −";
        case VK_MULTIPLY: return L"Num *";
        case VK_DIVIDE: return L"Num /";
        case VK_SNAPSHOT: return L"PrtScn";
        case VK_PAUSE: return L"Pause";
    }
    if (vk >= VK_F1 && vk <= VK_F24) {
        return L"F" + std::to_wstring(vk - VK_F1 + 1);
    }
    if (vk >= VK_NUMPAD0 && vk <= VK_NUMPAD9) {
        return L"Num " + std::to_wstring(vk - VK_NUMPAD0);
    }
    if ((vk >= '0' && vk <= '9') || (vk >= 'A' && vk <= 'Z')) {
        return std::wstring(1, (wchar_t)vk);
    }
    WCHAR buf[64];
    UINT scan = MapVirtualKeyW(vk, MAPVK_VK_TO_VSC);
    if (scan && GetKeyNameTextW((LONG)(scan << 16), buf, ARRAYSIZE(buf))) {
        return buf;
    }
    return L"?";
}

std::wstring KeyName(UINT key) {
    if (!key || (key & kKeyOff)) {
        return Tr(L"Wyłączony", L"Off");
    }
    UINT mods = key >> 16;
    std::wstring s;
    if (mods & MOD_WIN) s += L"Win+";
    if (mods & MOD_CONTROL) s += L"Ctrl+";
    if (mods & MOD_ALT) s += L"Alt+";
    if (mods & MOD_SHIFT) s += L"Shift+";
    return s + VkName(key & 0xFFFF);
}

std::wstring ShortcutName(int i) {
    UINT key = g_keys[i];
    if (i == ScNth && !(key & kKeyOff) && (key & 0xFFFF) == '1') {
        return KeyName(key) + L"…9";
    }
    return KeyName(key);
}

std::wstring SettingKey(int i) {
    return std::wstring(L"key.") + kShortcuts[i].id;
}

void LoadShortcuts() {
    for (int i = 0; i < ScCount; i++) {
        g_keys[i] = (UINT)Setting(SettingKey(i).c_str(), kShortcuts[i].def);
    }
    g_openKey = g_keys[ScOpen];
}

void SetShortcut(int i, UINT key) {
    if (key == kShortcuts[i].def) {
        g_settings.erase(SettingKey(i));
    } else {
        g_settings[SettingKey(i)] = key;
    }
    SaveSettings();
    LoadShortcuts();
    Redraw();
}

// The shortcut pressed, for one scope. For "paste nth" with a digit, also
// returns which item.
int MatchShortcut(UINT vk, ShortcutScope scope, int* nth = nullptr) {
    UINT mods = CurrentMods();
    UINT key = Key(mods, vk);
    for (int i = 0; i < ScCount; i++) {
        if (kShortcuts[i].scope != scope || (g_keys[i] & kKeyOff) || !g_keys[i]) {
            continue;
        }
        if (i == ScNth && (g_keys[i] & 0xFFFF) == '1' && (g_keys[i] >> 16) == mods &&
            vk >= '1' && vk <= '9') {
            if (nth) {
                *nth = (int)(vk - '1');
            }
            return i;
        }
        if (g_keys[i] == key) {
            if (nth) {
                *nth = 0;
            }
            return i;
        }
    }
    return -1;
}

// Another shortcut that uses the same keys, as a name to show, or empty.
std::wstring ShortcutConflict(UINT key, int self, UINT64 selfTemplate = 0) {
    if (!key || (key & kKeyOff)) {
        return {};
    }
    ShortcutScope scope = self >= 0 ? kShortcuts[self].scope : kScopeGlobal;
    for (int j = 0; j < ScCount; j++) {
        if (j == self || (g_keys[j] & kKeyOff)) {
            continue;
        }
        bool sameScope = kShortcuts[j].scope == scope || kShortcuts[j].scope == kScopeGlobal ||
                         scope == kScopeGlobal;
        if (sameScope && kShortcuts[j].scope != kScopeEditor) {
            sameScope = scope != kScopeEditor;
        } else if (sameScope) {
            sameScope = scope == kScopeEditor;
        }
        if (sameScope && g_keys[j] == key) {
            return std::wstring(kShortcuts[j].label);
        }
    }
    if (scope == kScopeGlobal) {
        for (const Item& it : g_items) {
            if (it.kind == Kind::Template && Live(it) && it.id != selfTemplate &&
                it.hotkey == key) {
                return Tr(L"szablon „", L"template “") + it.name + L"”";
            }
        }
    }
    return {};
}

void StopRecording() {
    g_recordingKey = false;
    g_recordTarget = -1;
    g_recordDone = nullptr;
    Redraw();
}

void StartRecording(int target, std::function<void(UINT)> done) {
    g_recordTarget = target;
    g_recordDone = std::move(done);
    g_recordingKey = true;
    Redraw();
}

void OnRecorded(UINT vk, UINT mods) {
    auto done = std::move(g_recordDone);
    StopRecording();
    if (vk == VK_ESCAPE && !mods) {
        return;  // Cancelled.
    }
    if (done) {
        done(Key(mods, vk));
    }
}

////////////////////////////////////////////////////////////////////////////////
// Template shortcuts: registered with Windows, so they work in every app.

constexpr int kHotkeyBase = 0x100;
std::vector<UINT64> g_hotkeyItems;  // By hotkey id - kHotkeyBase.
std::unordered_set<UINT64> g_hotkeyFailed;  // Templates whose shortcut is taken.

void RegisterTemplateHotkeys() {
    for (size_t i = 0; i < g_hotkeyItems.size(); i++) {
        UnregisterHotKey(g_hwnd, kHotkeyBase + (int)i);
    }
    g_hotkeyItems.clear();
    g_hotkeyFailed.clear();
    for (const Item& it : g_items) {
        if (it.kind != Kind::Template || !Live(it) || !it.hotkey || (it.hotkey & kKeyOff)) {
            continue;
        }
        int id = kHotkeyBase + (int)g_hotkeyItems.size();
        if (RegisterHotKey(g_hwnd, id, (it.hotkey >> 16) | MOD_NOREPEAT, it.hotkey & 0xFFFF)) {
            g_hotkeyItems.push_back(it.id);
        } else {
            g_hotkeyItems.push_back(0);
            g_hotkeyFailed.insert(it.id);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Actions

void PasteItem(Item& it, PasteMode mode) {
    std::wstring expanded;
    bool ok;
    if (it.kind == Kind::Template) {
        expanded = ExpandTemplate(it.text);
        ok = PutOnClipboard(it, mode, &expanded);
    } else {
        ok = PutOnClipboard(it, it.imageWidth && mode == PasteMode::Plain ? PasteMode::Normal
                                                                          : mode);
    }
    if (!ok) {
        ShowToast(Tr(L"Nie udało się wstawić elementu do schowka.",
                     L"Couldn't put the item on the clipboard."));
        return;
    }
    it.pasteCount++;
    it.lastPasted = NowFileTime();
    SaveIndex();
    HWND target = g_prevForeground;
    if (!g_onTop) {
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

void PasteId(UINT64 id, PasteMode mode) {
    if (Item* it = FindItem(id)) {
        g_selectedId = id;
        PasteItem(*it, mode);
    }
}

void PasteNth(int n) {
    for (const Row& r : g_rows) {
        if (r.type == Row::Entry && n-- == 0) {
            PasteId(r.id, PasteMode::Normal);
            return;
        }
    }
}

// A template's shortcut, pressed in another app: pastes there directly.
void PasteTemplateHotkey(UINT64 id) {
    Item* it = FindItem(id);
    if (!it || !Live(*it)) {
        return;
    }
    std::wstring text = ExpandTemplate(it->text);
    if (PutOnClipboard(*it, PasteMode::Normal, &text)) {
        it->pasteCount++;
        it->lastPasted = NowFileTime();
        SaveIndex();
        SendCtrlV();
    }
}

void PurgeItems(const std::function<bool(const Item&)>& pred) {
    std::vector<UINT64> images;
    std::erase_if(g_items, [&](const Item& it) {
        if (!pred(it)) {
            return false;
        }
        if (it.imageWidth) {
            images.push_back(it.id);
        }
        ForgetThumbnail(it.id);
        return true;
    });
    DeleteImagesLater(std::move(images));
}

void PinItem(UINT64 id, UINT64 category);

void UnpinItem(UINT64 id) {
    Item* it = FindItem(id);
    if (!it || it->kind != Kind::Pinned) {
        return;
    }
    UINT64 category = it->category, pinnedTime = it->pinnedTime, time = it->time;
    std::string hash = HashKey(*it);
    it->kind = Kind::History;
    it->time = NowFileTime();
    // An identical copy in the history would now show twice.
    PurgeItems([&](const Item& other) {
        return other.id != id && other.kind == Kind::History && Live(other) &&
               HashKey(other) == hash;
    });
    ShowToast(Tr(L"Odpięto „", L"Unpinned “") + ItemTitle(*FindItem(id)) +
                  Tr(L"”. Wrócił na górę historii.", L"”. It's back at the top of the history."),
              Tr(L"Cofnij", L"Undo"), [id, category, pinnedTime, time] {
                  if (Item* it = FindItem(id)) {
                      it->kind = Kind::Pinned;
                      it->category = category;
                      it->pinnedTime = pinnedTime;
                      it->time = time;
                      Refresh();
                      SaveIndex();
                  }
              });
    Refresh();
    SaveIndex();
}

void PinItem(UINT64 id, UINT64 category) {
    Item* it = FindItem(id);
    if (!it || it->kind == Kind::Template) {
        return;
    }
    if (category && !FindCategory(category)) {
        category = 0;
    }
    if (it->kind == Kind::Pinned) {
        // Already pinned: moves it to another category.
        it->category = category;
    } else {
        it->kind = Kind::Pinned;
        it->category = category;
        it->pinnedTime = NowFileTime();
        ShowToast(Tr(L"Przypięto do „", L"Pinned to “") + CategoryName(category) + L"”.",
                  Tr(L"Cofnij", L"Undo"), [id] {
                      if (Item* it = FindItem(id); it && it->kind == Kind::Pinned) {
                          it->kind = Kind::History;
                          Refresh();
                          SaveIndex();
                      }
                  });
    }
    g_settings[L"lastCategory"] = (INT64)category;
    SaveSettings();
    Refresh();
    SaveIndex();
}

void RestoreItem(UINT64 id, bool announce) {
    Item* it = FindItem(id);
    if (!it) {
        return;
    }
    it->deletedTime = 0;
    if (it->kind == Kind::Pinned && it->category && !FindCategory(it->category)) {
        it->category = 0;
    }
    if (it->kind == Kind::Template) {
        RegisterTemplateHotkeys();
    }
    if (announce) {
        const wchar_t* where = it->kind == Kind::Pinned     ? Tr(L"przypiętych", L"Pinned")
                               : it->kind == Kind::Template ? Tr(L"szablonów", L"Templates")
                                                            : Tr(L"historii", L"History");
        ShowToast(Tr(L"Przywrócono „", L"Restored “") + ItemTitle(*it) + Tr(L"” do ", L"” to ") +
                  where + L".");
    }
    Refresh();
    SaveIndex();
}

void TrashItem(UINT64 id) {
    Item* it = FindItem(id);
    if (!it || !Live(*it)) {
        return;
    }
    it->deletedTime = NowFileTime();
    if (it->kind == Kind::Template) {
        RegisterTemplateHotkeys();
    }
    if (g_detailsId == id) {
        CloseDetails();
    }
    ShowToast(Tr(L"„", L"“") + ItemTitle(*it) +
                  Tr(L"” jest w koszu przez 30 dni.", L"” is in the trash for 30 days."),
              Tr(L"Cofnij", L"Undo"), [id] { RestoreItem(id, false); });
    Refresh();
    SaveIndex();
}

void PurgeItem(UINT64 id) {
    PurgeItems([id](const Item& it) { return it.id == id; });
    Refresh();
    SaveIndex();
}

void EmptyTrash() {
    PurgeItems([](const Item& it) { return !Live(it); });
    ShowToast(Tr(L"Kosz jest pusty.", L"The trash is empty."));
    Refresh();
    SaveIndex();
}

void PurgeExpiredTrash() {
    UINT64 limit = NowFileTime() - kTrashDays * kDay;
    size_t before = g_items.size();
    PurgeItems([limit](const Item& it) { return !Live(it) && it.deletedTime < limit; });
    if (g_items.size() != before) {
        Refresh();
        SaveIndex();
    }
}

void CommitClear() {
    std::vector<UINT64> images;
    for (const Item& it : g_cleared) {
        if (it.imageWidth) {
            images.push_back(it.id);
        }
        ForgetThumbnail(it.id);
    }
    DeleteImagesLater(std::move(images));
    g_cleared.clear();
}

void UndoClear() {
    g_items.insert(g_items.end(), std::make_move_iterator(g_cleared.begin()),
                   std::make_move_iterator(g_cleared.end()));
    g_cleared.clear();
    Refresh();
    SaveIndex();
}

void ClearHistory() {
    ExpireToast();
    std::vector<Item> kept;
    for (Item& it : g_items) {
        (it.kind == Kind::History && Live(it) ? g_cleared : kept).push_back(std::move(it));
    }
    g_items = std::move(kept);
    if (g_cleared.empty()) {
        return;
    }
    size_t n = g_cleared.size();
    ShowToast(Tr(L"Wyczyszczono ", L"Cleared ") +
                  Count(n, Tr(L"element", L"item"), Tr(L"elementy", L"items"),
                        Tr(L"elementów", L"items")) +
                  Tr(L" historii. Przypięte i szablony bez zmian.",
                     L" from the history. Pinned items and templates are unchanged."),
              Tr(L"Cofnij", L"Undo"), UndoClear, CommitClear);
    Refresh();
    SaveIndex();
}

// "Automatic cleanup": only plain history, never pinned items or templates.
void AutoClean() {
    INT64 mode = Setting(L"autoClean", 0);
    size_t before = g_items.size();
    if (mode == 1) {
        if (!SameBoot(Setting(L"cleanBoot", 0))) {
            PurgeItems([](const Item& it) { return it.kind == Kind::History && Live(it); });
            g_settings[L"cleanBoot"] = BootTime();
            SaveSettings();
        }
    } else if (mode >= 2 && mode <= 4) {
        static const UINT64 kDays[] = {1, 7, 30};
        UINT64 limit = NowFileTime() - kDays[mode - 2] * kDay;
        PurgeItems([limit](const Item& it) {
            return it.kind == Kind::History && Live(it) && it.time < limit;
        });
    }
    if (g_items.size() != before) {
        Refresh();
        SaveIndex();
    }
    PurgeExpiredTrash();
}

void SaveEditedText(UINT64 id, const std::wstring& text) {
    Item* it = FindItem(id);
    if (!it || it->imageWidth || Trim(text).empty()) {
        return;
    }
    it->text = text;
    it->files.clear();
    it->html.clear();
    it->rtf.clear();
    it->type = DetectType(*it);
    FinishItem(*it);
    HashItem(*it, {});
    Refresh();
    SaveIndex();
}

UINT64 AddCategory(const std::wstring& name, const std::wstring& emoji, BYTE color) {
    Category c;
    c.id = g_nextId++;
    c.name = name;
    c.emoji = emoji;
    c.color = color;
    g_categories.push_back(c);
    SaveIndex();
    Refresh();
    return c.id;
}

void DeleteCategory(UINT64 id) {
    Category* c = FindCategory(id);
    if (!c) {
        return;
    }
    std::wstring name = c->name;
    for (Item& it : g_items) {
        if (it.category == id) {
            it.category = 0;
        }
    }
    std::erase_if(g_categories, [id](const Category& c) { return c.id == id; });
    if (g_pinFilter == id) {
        g_pinFilter = kAllCategories;
    }
    ShowToast(
        Tr(L"Usunięto kategorię „", L"Deleted the category “") + name +
        Tr(L"”. Jej elementy są w „Bez kategorii”.", L"”. Its items are in “Uncategorized”."));
    Refresh();
    SaveIndex();
}

std::wstring CleanTag(std::wstring s) {
    s = Trim(s);
    while (!s.empty() && s[0] == L'#') {
        s.erase(0, 1);
    }
    for (wchar_t& c : s) {
        if (iswspace(c)) {
            c = L'-';
        }
    }
    return s.substr(0, 40);
}

void AddTag(UINT64 id, const std::wstring& raw) {
    Item* it = FindItem(id);
    std::wstring tag = CleanTag(raw);
    if (!it || tag.empty() || HasTag(*it, tag)) {
        return;
    }
    it->tags.push_back(tag);
    Refresh();
    SaveIndex();
}

void RemoveTag(UINT64 id, const std::wstring& tag) {
    if (Item* it = FindItem(id)) {
        std::erase_if(it->tags, [&](const std::wstring& t) { return EqualsNoCase(t, tag); });
        Refresh();
        SaveIndex();
    }
}

UINT64 SaveTemplate(UINT64 id, const std::wstring& name, const std::wstring& body,
                    const std::wstring& emoji, UINT hotkey) {
    Item* it = id ? FindItem(id) : nullptr;
    if (!it) {
        Item fresh;
        fresh.id = g_nextId++;
        fresh.kind = Kind::Template;
        fresh.time = NowFileTime();
        g_items.push_back(std::move(fresh));
        it = &g_items.back();
    }
    it->name = name;
    it->text = body;
    it->emoji = emoji;
    it->hotkey = hotkey;
    it->type = ItemType::Text;
    FinishItem(*it);
    HashItem(*it, {});
    RegisterTemplateHotkeys();
    Refresh();
    SaveIndex();
    return it->id;
}

////////////////////////////////////////////////////////////////////////////////
// Messages from the worker

void ApplyWindowSizeSetting();

// The worker loads before it processes any copy, so this comes first.
void OnLoaded(LoadResult* r) {
    g_items = std::move(r->items);
    g_categories = std::move(r->categories);
    g_nextId = std::max(g_nextId, r->nextId);
    g_settings = std::move(r->settings);
    delete r;
    g_loaded = true;
    for (const Item& it : g_items) {
        g_nextId = std::max(g_nextId, it.id + 1);
    }
    for (const Category& c : g_categories) {
        g_nextId = std::max(g_nextId, c.id + 1);
    }
    LoadShortcuts();
    ApplyLook();
    ApplyWindowSizeSetting();
    AutoClean();
    RegisterTemplateHotkeys();
    Refresh();
}

void OnCaptured(Captured* r) {
    Item& incoming = r->item;
    if (r->kind == Kind::History) {
        // ponytail: linear scan; fine for tens of thousands of items.
        for (Item& it : g_items) {
            if (it.kind == Kind::History && Live(it) &&
                memcmp(it.hash, incoming.hash, sizeof(it.hash)) == 0) {
                it.time = incoming.time;
                if (!incoming.source.empty()) {
                    it.source = incoming.source;
                }
                delete r;
                Refresh();
                SaveIndex();
                return;
            }
        }
    }
    incoming.id = g_nextId++;
    incoming.kind = r->kind;
    if (r->kind == Kind::Pinned) {
        incoming.category = FindCategory(r->category) ? r->category : 0;
        incoming.pinnedTime = incoming.time;
    }
    UINT64 id = incoming.id;
    if (!r->png.empty()) {
        WriteImageLater(id, std::move(r->png));
    }
    g_items.push_back(std::move(incoming));
    if (r->copy) {
        // After the worker wrote the image.
        Enqueue([id] {
            PostUi([id] {
                if (Item* it = FindItem(id)) {
                    PutOnClipboard(*it, PasteMode::Normal);
                }
            });
        });
    }
    delete r;
    Refresh();
    SaveIndex();
}

////////////////////////////////////////////////////////////////////////////////
// Image previews for the details panel, decoded by the worker.

struct Preview {
    UINT64 id = 0;
    UINT w = 0, h = 0;
    std::string pixels;
    bool loading = false;
} g_preview;

constexpr UINT64 kPreviewKey = 1ULL << 63;  // Bitmap cache key flag.

void LoadPreview(UINT64 id) {
    Item* it = FindItem(id);
    if (!it || !it->imageWidth || g_preview.id == id) {
        return;
    }
    if (auto found = g_main.bitmaps.find(g_preview.id | kPreviewKey);
        found != g_main.bitmaps.end()) {
        found->second->Release();
        g_main.bitmaps.erase(found);
    }
    g_preview = {};
    g_preview.id = id;
    g_preview.loading = true;
    Enqueue([id] {
        auto* p = new Preview;
        p->id = id;
        std::string png;
        if (g_workerWic && LoadImagePng(id, &png)) {
            if (IWICBitmapSource* source = DecodeImage(g_workerWic, png)) {
                UINT w, h;
                source->GetSize(&w, &h);
                FitSize(w, h, 1200, &p->w, &p->h);
                if (!ToPixels(g_workerWic, source, p->w, p->h, GUID_WICPixelFormat32bppPBGRA,
                              &p->pixels)) {
                    p->pixels.clear();
                }
                source->Release();
            }
        }
        PostUi([p] {
            if (g_preview.id == p->id) {
                g_preview = std::move(*p);
                g_preview.loading = false;
                Redraw();
            }
            delete p;
        });
    });
}

ID2D1Bitmap* PreviewBitmap(UINT64 id) {
    if (g_preview.id != id || g_preview.pixels.empty()) {
        return nullptr;
    }
    auto& cache = g_main.bitmaps;
    if (auto found = cache.find(id | kPreviewKey); found != cache.end()) {
        return found->second;
    }
    ID2D1Bitmap* bitmap = PixelBitmap(g_preview.w, g_preview.h, g_preview.pixels.data());
    if (bitmap) {
        cache[id | kPreviewKey] = bitmap;
    }
    return bitmap;
}

void OpenDetails(UINT64 id) {
    Item* it = FindItem(id);
    if (!it) {
        return;
    }
    bool wasOpen = g_detailsId != 0;
    g_detailsId = id;
    g_selectedId = id;
    g_editingText = false;
    g_detailScroll.Reset();
    LoadPreview(id);
    if (!wasOpen && SideDetails()) {
        ResizeForDetails(true);
    }
    Redraw();
}

void CloseDetails() {
    if (!g_detailsId) {
        return;
    }
    g_detailsId = 0;
    g_editingText = false;
    if (SideDetails()) {
        ResizeForDetails(false);
    }
    Redraw();
}

////////////////////////////////////////////////////////////////////////////////
// Files: "Save as…", "Open"

// Runs a system dialog; the window stays open meanwhile.
template <class T>
bool FileDialog(const CLSID& clsid, const wchar_t* title, const wchar_t* fileName,
                const COMDLG_FILTERSPEC* filters, UINT filterCount, const wchar_t* ext,
                std::wstring* path) {
    T* dialog = nullptr;
    if (FAILED(CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog)))) {
        return false;
    }
    dialog->SetTitle(title);
    if (fileName) {
        dialog->SetFileName(fileName);
    }
    dialog->SetFileTypes(filterCount, filters);
    dialog->SetDefaultExtension(ext);
    g_modal++;
    bool ok = SUCCEEDED(dialog->Show(g_hwnd));
    g_modal--;
    IShellItem* item = nullptr;
    if (ok && SUCCEEDED(dialog->GetResult(&item))) {
        PWSTR name = nullptr;
        if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &name))) {
            *path = name;
            CoTaskMemFree(name);
        } else {
            ok = false;
        }
        item->Release();
    } else {
        ok = false;
    }
    dialog->Release();
    return ok;
}

bool WriteWholeFile(const std::wstring& path, std::string_view data) {
    HANDLE file = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) {
        return false;
    }
    DWORD written = 0;
    bool ok = WriteFile(file, data.data(), (DWORD)data.size(), &written, nullptr) &&
              written == data.size();
    CloseHandle(file);
    return ok;
}

std::wstring FileName(const std::wstring& path) {
    size_t slash = path.find_last_of(L"\\/");
    return slash == std::wstring::npos ? path : path.substr(slash + 1);
}

void SaveItemAs(UINT64 id) {
    Item* it = FindItem(id);
    if (!it || (!it->imageWidth && it->text.empty())) {
        return;
    }
    bool image = it->imageWidth != 0;
    std::wstring name = (image ? Tr(L"Obraz ", L"Image ") : Tr(L"Tekst ", L"Text ")) +
                        FormatDatePattern(it->time, L"yyyy-MM-dd") + (image ? L".png" : L".txt");
    COMDLG_FILTERSPEC png[] = {{Tr(L"Obraz PNG", L"PNG image"), L"*.png"}};
    COMDLG_FILTERSPEC txt[] = {{Tr(L"Plik tekstowy", L"Text file"), L"*.txt"},
                               {Tr(L"Wszystkie pliki", L"All files"), L"*.*"}};
    std::wstring path;
    if (!FileDialog<IFileSaveDialog>(CLSID_FileSaveDialog, Tr(L"Zapisz jako", L"Save as"),
                                     name.c_str(), image ? png : txt, image ? 1 : 2,
                                     image ? L"png" : L"txt", &path)) {
        return;
    }
    it = FindItem(id);
    if (!it) {
        return;
    }
    if (image) {
        Enqueue([id, path] {
            std::string data;
            bool ok = LoadImagePng(id, &data) && WriteWholeFile(path, data);
            PostUi([ok, path] {
                ShowToast(ok ? Tr(L"Zapisano „", L"Saved “") + FileName(path) + L"”."
                             : Tr(L"Nie udało się zapisać pliku.", L"Couldn't save the file."));
            });
        });
    } else {
        std::string data = "\xEF\xBB\xBF" + ToUtf8(it->text);
        bool ok = WriteWholeFile(path, data);
        ShowToast(ok ? Tr(L"Zapisano „", L"Saved “") + FileName(path) + L"”."
                     : Tr(L"Nie udało się zapisać pliku.", L"Couldn't save the file."));
    }
}

bool CanOpen(const Item& it) {
    return it.imageWidth || it.type == ItemType::Link || it.type == ItemType::Files;
}

void OpenItem(UINT64 id) {
    Item* it = FindItem(id);
    if (!it) {
        return;
    }
    if (it->imageWidth) {
        OpenEditor(id, true);
    } else if (it->type == ItemType::Link) {
        std::wstring url = Trim(it->text);
        if (StartsWithNoCase(url, L"www.")) {
            url = L"https://" + url;
        }
        ShellExecuteW(nullptr, L"open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
        if (!g_onTop) {
            HidePanel(false);
        }
    } else if (it->type == ItemType::Files) {
        std::wstring first = it->files.substr(0, it->files.find(L'\n'));
        std::wstring args = L"/select,\"" + first + L"\"";
        ShellExecuteW(nullptr, nullptr, L"explorer.exe", args.c_str(), nullptr, SW_SHOWNORMAL);
        if (!g_onTop) {
            HidePanel(false);
        }
    }
}

UINT64 ImageFileSize(UINT64 id) {
    static std::unordered_map<UINT64, UINT64> cache;
    if (auto found = cache.find(id); found != cache.end()) {
        return found->second;
    }
    UINT64 size = FileSize(ImagePath(id));
    size = size > kHeaderSize ? size - kHeaderSize : 0;
    if (size) {
        cache[id] = size;
    }
    return size;
}

////////////////////////////////////////////////////////////////////////////////
// Export and import: one ".schowek" file. With a password it's encrypted with
// a key derived by PBKDF2-SHA256; without one it's plain, so anyone can read it.

constexpr char kExportMagic[4] = {'C', 'T', 'E', 'X'};
constexpr char kPayloadMagic[4] = {'C', 'T', 'E', 'P'};
constexpr UINT32 kExportVersion = 1;
enum : UINT32 { kExportPinned = 1, kExportTemplates = 2, kExportSettings = 4, kExportHistory = 8 };

// Settings that only make sense on this computer.
bool LocalOnlySetting(const std::wstring& key) {
    return key == L"boot" || key == L"posBoot" || key == L"cleanBoot";
}

struct ImportData {
    UINT32 flags = 0;
    std::vector<Category> categories;
    std::vector<Item> items;
    std::vector<std::string> pngs;
    std::map<std::wstring, INT64> settings;
};

void ExportData(UINT32 flags, const std::wstring& path, std::wstring password,
                std::function<void(bool)> done) {
    struct Job {
        UINT32 flags;
        std::vector<Category> categories;
        std::vector<Item> items;
        std::map<std::wstring, INT64> settings;
        std::wstring path, password;
    };
    auto job = std::make_shared<Job>();
    job->flags = flags;
    job->path = path;
    job->password = std::move(password);
    if (flags & kExportPinned) {
        job->categories = g_categories;
    }
    for (const Item& it : g_items) {
        bool take = Live(it) && ((it.kind == Kind::Pinned && (flags & kExportPinned)) ||
                                 (it.kind == Kind::Template && (flags & kExportTemplates)) ||
                                 (it.kind == Kind::History && (flags & kExportHistory)));
        if (take) {
            job->items.push_back(it);
        }
    }
    if (flags & kExportSettings) {
        for (const auto& [key, value] : g_settings) {
            if (!LocalOnlySetting(key)) {
                job->settings[key] = value;
            }
        }
    }
    Enqueue([job, done] {
        Writer w;
        w.Raw(kPayloadMagic, 4);
        w.Num<UINT32>(job->flags);
        w.Num<UINT32>((UINT32)job->categories.size());
        for (const Category& c : job->categories) {
            WriteCategory(w, c);
        }
        w.Num<UINT32>((UINT32)job->items.size());
        bool ok = true;
        for (const Item& it : job->items) {
            WriteItem(w, it);
            std::string png;
            if (it.imageWidth && !LoadImagePng(it.id, &png)) {
                ok = false;
            }
            w.Str(png);
        }
        WriteSettingsMap(w, job->settings);
        Writer out;
        out.Raw(kExportMagic, 4);
        out.Num<UINT32>(kExportVersion);
        bool encrypt = !job->password.empty();
        out.Num<BYTE>(encrypt);
        if (encrypt) {
            std::string salt(16, '\0');
            BCryptGenRandom(nullptr, (PUCHAR)salt.data(), 16, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
            std::string key = DeriveKey(job->password, salt, kPbkdf2Iterations);
            SecureZeroMemory(job->password.data(), job->password.size() * sizeof(wchar_t));
            std::string sealed = key.empty() ? std::string() : EncryptWith(key, w.s);
            SecureZeroMemory(key.data(), key.size());
            ok = ok && !sealed.empty();
            out.Raw(salt.data(), salt.size());
            out.Num<UINT64>(kPbkdf2Iterations);
            out.s += sealed;
        } else {
            out.s += w.s;
        }
        ok = ok && WriteWholeFile(job->path, out.s);
        PostUi([done, ok] { done(ok); });
    });
}

bool ParsePayload(std::string_view payload, ImportData* d) {
    Reader rd(payload);
    char magic[4];
    if (!rd.Raw(magic, 4) || memcmp(magic, kPayloadMagic, 4) != 0) {
        return false;
    }
    d->flags = rd.Num<UINT32>();
    UINT32 categories = rd.Num<UINT32>();
    for (UINT32 i = 0; i < categories && rd.ok; i++) {
        Category c;
        if (ReadCategory(rd, c)) {
            d->categories.push_back(std::move(c));
        }
    }
    UINT32 items = rd.Num<UINT32>();
    for (UINT32 i = 0; i < items && rd.ok; i++) {
        Item it;
        if (!ReadItem(rd, it, kIndexVersion)) {
            break;
        }
        std::string png = rd.Str();
        d->items.push_back(std::move(it));
        d->pngs.push_back(std::move(png));
    }
    return ReadSettingsMap(rd, &d->settings) && rd.ok;
}

// The file's header: whether it's encrypted, and with which salt.
bool ReadExportHeader(const std::string& file, bool* encrypted, std::string* salt,
                      UINT64* iterations, std::string_view* body) {
    Reader rd(file);
    char magic[4];
    if (!rd.Raw(magic, 4) || memcmp(magic, kExportMagic, 4) != 0 ||
        rd.Num<UINT32>() != kExportVersion) {
        return false;
    }
    *encrypted = rd.Num<BYTE>() != 0;
    if (*encrypted) {
        salt->resize(16);
        rd.Raw(salt->data(), 16);
        *iterations = rd.Num<UINT64>();
        if (*iterations < 1000 || *iterations > 100000000) {
            return false;
        }
    }
    if (!rd.ok) {
        return false;
    }
    *body = std::string_view(rd.p, rd.end - rd.p);
    return true;
}

struct ImportState {
    std::wstring path;
    std::shared_ptr<std::string> file;
    enum Status { None, Reading, NeedPassword, Checking, Ready, WrongPassword, Invalid };
    Status status = None;
    bool encrypted = false;
    std::shared_ptr<ImportData> data;
    bool replace = false;
    bool withSettings = true;
    UINT64 generation = 0;
} g_import;

// Decrypts and reads the chosen file on the worker.
void CheckImport(const std::wstring& password) {
    UINT64 generation = ++g_import.generation;
    auto file = g_import.file;
    if (!file) {
        return;
    }
    g_import.status = password.empty() && g_import.encrypted ? ImportState::NeedPassword
                                                             : ImportState::Checking;
    if (g_import.status == ImportState::NeedPassword) {
        Redraw();
        return;
    }
    Enqueue([generation, file, password = std::wstring(password)]() mutable {
        bool encrypted = false;
        std::string salt;
        UINT64 iterations = 0;
        std::string_view body;
        auto data = std::make_shared<ImportData>();
        ImportState::Status status = ImportState::Invalid;
        if (ReadExportHeader(*file, &encrypted, &salt, &iterations, &body)) {
            if (encrypted) {
                std::string key = DeriveKey(password, salt, iterations);
                std::string plain;
                if (!key.empty() && DecryptWith(key, body, &plain)) {
                    status = ParsePayload(plain, data.get()) ? ImportState::Ready
                                                             : ImportState::Invalid;
                } else {
                    status = ImportState::WrongPassword;
                }
                SecureZeroMemory(key.data(), key.size());
            } else {
                status = ParsePayload(body, data.get()) ? ImportState::Ready
                                                        : ImportState::Invalid;
            }
        }
        SecureZeroMemory(password.data(), password.size() * sizeof(wchar_t));
        PostUi([generation, status, data] {
            if (generation == g_import.generation) {
                g_import.status = status;
                g_import.data = status == ImportState::Ready ? data : nullptr;
                Redraw();
            }
        });
    });
}

void LoadImportFile(const std::wstring& path) {
    g_import.path = path;
    g_import.status = ImportState::Reading;
    g_import.data = nullptr;
    g_import.file = nullptr;
    UINT64 generation = ++g_import.generation;
    Enqueue([generation, path] {
        auto file = std::make_shared<std::string>();
        bool ok = ReadFileBytes(path, file.get());
        bool encrypted = false;
        std::string salt;
        UINT64 iterations;
        std::string_view body;
        ok = ok && ReadExportHeader(*file, &encrypted, &salt, &iterations, &body);
        PostUi([generation, file, ok, encrypted] {
            if (generation != g_import.generation) {
                return;
            }
            if (!ok) {
                g_import.status = ImportState::Invalid;
                Redraw();
                return;
            }
            g_import.file = file;
            g_import.encrypted = encrypted;
            g_import.status = ImportState::NeedPassword;
            if (encrypted) {
                SetFieldText(FPassword, L"");
                FocusField(FPassword);
                Redraw();
            } else {
                CheckImport(L"");
            }
        });
    });
}

std::wstring ImportSummary(const ImportData& d) {
    size_t pinned = 0, templates = 0, history = 0, images = 0;
    std::set<std::wstring> tags;
    for (const Item& it : d.items) {
        if (it.kind == Kind::Pinned) {
            pinned++;
            images += it.imageWidth != 0;
            tags.insert(it.tags.begin(), it.tags.end());
        } else if (it.kind == Kind::Template) {
            templates++;
        } else {
            history++;
        }
    }
    std::vector<std::wstring> parts;
    if (pinned) {
        parts.push_back(
            Count(pinned, Tr(L"przypięty", L"pinned item"), Tr(L"przypięte", L"pinned items"),
                  Tr(L"przypiętych", L"pinned items")) +
            (images ? Tr(L" (z obrazami w oryginale)", L" (with original images)") : L""));
    }
    if (!d.categories.empty()) {
        parts.push_back(Count(d.categories.size(), Tr(L"kategoria", L"category"),
                              Tr(L"kategorie", L"categories"), Tr(L"kategorii", L"categories")));
    }
    if (!tags.empty()) {
        parts.push_back(
            Count(tags.size(), Tr(L"tag", L"tag"), Tr(L"tagi", L"tags"), Tr(L"tagów", L"tags")));
    }
    if (templates) {
        parts.push_back(Count(templates, Tr(L"szablon", L"template"), Tr(L"szablony", L"templates"),
                              Tr(L"szablonów", L"templates")));
    }
    if (history) {
        parts.push_back(Count(history, Tr(L"element historii", L"history item"),
                              Tr(L"elementy historii", L"history items"),
                              Tr(L"elementów historii", L"history items")));
    }
    if (!d.settings.empty()) {
        parts.push_back(Tr(L"ustawienia", L"settings"));
    }
    if (parts.empty()) {
        return Tr(L"Plik jest pusty.", L"The file is empty.");
    }
    std::wstring s = Tr(L"W pliku: ", L"In the file: ");
    for (size_t i = 0; i < parts.size(); i++) {
        s += parts[i];
        if (i + 2 < parts.size()) {
            s += L", ";
        } else if (i + 2 == parts.size()) {
            s += Tr(L" i ", L" and ");
        }
    }
    return s + L".";
}

void ApplyImport() {
    auto d = g_import.data;
    if (!d) {
        return;
    }
    bool replace = g_import.replace, withSettings = g_import.withSettings;
    auto apply = [d, replace, withSettings] {
        if (replace) {
            UINT32 f = d->flags;
            PurgeItems([f](const Item& it) {
                return (it.kind == Kind::Pinned && (f & kExportPinned)) ||
                       (it.kind == Kind::Template && (f & kExportTemplates)) ||
                       (it.kind == Kind::History && (f & kExportHistory));
            });
            if (f & kExportPinned) {
                g_categories.clear();
                g_pinFilter = kAllCategories;
            }
        }
        std::map<UINT64, UINT64> categoryIds;
        for (const Category& c : d->categories) {
            auto same = std::find_if(g_categories.begin(), g_categories.end(),
                                     [&](const Category& o) { return EqualsNoCase(o.name, c.name); });
            if (same != g_categories.end()) {
                categoryIds[c.id] = same->id;
            } else {
                Category copy = c;
                copy.id = g_nextId++;
                categoryIds[c.id] = copy.id;
                g_categories.push_back(copy);
            }
        }
        std::unordered_set<std::string> existing;
        for (const Item& it : g_items) {
            if (Live(it)) {
                existing.insert((char)it.kind + HashKey(it));
            }
        }
        size_t added = 0, skipped = 0;
        for (size_t i = 0; i < d->items.size(); i++) {
            Item it = d->items[i];
            std::string key = (char)it.kind + HashKey(it);
            if (existing.contains(key)) {
                skipped++;
                continue;
            }
            existing.insert(key);
            it.id = g_nextId++;
            it.deletedTime = 0;
            it.category = categoryIds.contains(it.category) ? categoryIds[it.category] : 0;
            if (it.imageWidth && !d->pngs[i].empty()) {
                WriteImageLater(it.id, d->pngs[i]);
            }
            g_items.push_back(std::move(it));
            added++;
        }
        if (withSettings && !d->settings.empty()) {
            for (const auto& [key, value] : d->settings) {
                if (!LocalOnlySetting(key)) {
                    g_settings[key] = value;
                }
            }
            SaveSettings();
            LoadShortcuts();
            ApplyLook();
            ApplyWindowSizeSetting();
        }
        RegisterTemplateHotkeys();
        Refresh();
        SaveIndex();
        std::wstring message = Tr(L"Zaimportowano ", L"Imported ") +
                               Count(added, Tr(L"element", L"item"), Tr(L"elementy", L"items"),
                                     Tr(L"elementów", L"items"));
        if (skipped) {
            message += Tr(L", pominięto ", L", skipped ") +
                       Count(skipped, Tr(L"duplikat", L"duplicate"),
                             Tr(L"duplikaty", L"duplicates"), Tr(L"duplikatów", L"duplicates"));
        }
        if (replace) {
            message += Tr(L". Poprzednie dane są w kopii w folderze danych",
                          L". A backup of the previous data is in the data folder");
        }
        ShowToast(message + L".");
    };
    if (replace) {
        // The copy comes after any pending save, the worker runs jobs in order.
        SaveIndexNow();
        Enqueue([apply] {
            BackupData();
            PostUi(apply);
        });
    } else {
        apply();
    }
}

////////////////////////////////////////////////////////////////////////////////
// Views of the clipboard window

void OpenSearch();
void CloseSearch();
void ShowPage(Page page);
void SwitchTab(Tab tab);
void ShowCategoryDialog(UINT64 id, std::function<void(UINT64)> created);
void ShowTemplateDialog(UINT64 id);
void ShowExportDialog();
void ShowImportDialog();
void StartTextEdit(UINT64 id);
void SetOnTop(bool onTop);
void SetPaused(bool paused);

// The mouse is over this rectangle, and nothing covers it.
bool MouseOver(const D2D1_RECT_F& r) {
    Surface& s = *g_s;
    return s.mouseInside && !s.flyout.open && !s.dialog.open && In(r, s.mouse) &&
           (s.clips.empty() || In(s.clips.back(), s.mouse));
}

void EmptyMessage(const std::wstring& message, const D2D1_RECT_F& area) {
    Text(message, g_fonts.empty, {area.left + 24, area.top, area.right - 24, area.bottom},
         g_theme.text2);
}

std::wstring TypeLabel(const Item& it) {
    switch (it.type) {
        case ItemType::Image:
            return Tr(L"Obraz · PNG", L"Image · PNG");
        case ItemType::Files:
            return std::count(it.files.begin(), it.files.end(), L'\n') ? Tr(L"Pliki", L"Files")
                                                                       : Tr(L"Plik", L"File");
        case ItemType::Link:
            return L"Link";
        case ItemType::Code:
            return Tr(L"Kod", L"Code");
        case ItemType::Color:
            return Tr(L"Kolor", L"Color");
        case ItemType::Text:
            break;
    }
    if (it.kind == Kind::Template) {
        return Tr(L"Szablon", L"Template");
    }
    return it.html.empty() && it.rtf.empty() ? Tr(L"Tekst", L"Text")
                                             : Tr(L"Tekst z formatowaniem", L"Formatted text");
}

wchar_t TypeGlyph(const Item& it) {
    if (it.kind == Kind::Template) {
        return kIconTemplate;
    }
    switch (it.type) {
        case ItemType::Image: return kIconImage;
        case ItemType::Files: return kIconFiles;
        case ItemType::Link: return kIconLink;
        case ItemType::Code: return kIconCode;
        case ItemType::Color: return kIconTheme;
        case ItemType::Text: break;
    }
    return kIconText;
}

// The small icon of an item: its emoji, a thumbnail, a color swatch or the
// icon of its type.
void DrawItemIcon(const Item& it, const D2D1_RECT_F& r) {
    const Theme& t = g_theme;
    if (!it.emoji.empty() && it.kind != Kind::History) {
        Emoji(it.emoji, g_fonts.emoji16, Inset(r, -4, -4));
        return;
    }
    if (it.type == ItemType::Image) {
        if (ID2D1Bitmap* bitmap = Thumbnail(it)) {
            DrawFitted(bitmap, Inset(r, -2, -2));
            return;
        }
    }
    if (it.type == ItemType::Color && it.kind != Kind::Template) {
        D2D1_RECT_F swatch = Inset(r, 1, 1);
        Fill(swatch, Rgba(it.color), 3);
        Stroke(swatch, t.controlStroke, 3);
        return;
    }
    Glyph(TypeGlyph(it), g_fonts.icon16, r, t.text2);
}

// A 32 DIP tile with the item's icon and its color mark.
void DrawTile(const Item& it, const D2D1_RECT_F& tile) {
    const Theme& t = g_theme;
    Fill(tile, it.mark ? CategoryColor(it.mark, 0.22f) : t.control, 6);
    Stroke(tile, t.controlStroke, 6);
    if (it.type == ItemType::Image && it.emoji.empty()) {
        if (ID2D1Bitmap* bitmap = Thumbnail(it)) {
            DrawFitted(bitmap, Inset(tile, 3, 3));
        }
    } else {
        DrawItemIcon(it, Inset(tile, 8, 8));
    }
    if (it.mark) {
        Circle(tile.right - 3, tile.bottom - 3, 4.5f, t.flyout);
        Circle(tile.right - 3, tile.bottom - 3, 3.5f, CategoryColor(it.mark));
    }
}

void DrawCategoryIcon(UINT64 id, const D2D1_RECT_F& r) {
    const Theme& t = g_theme;
    if (id == kAllCategories) {
        Glyph(kIconAll, g_fonts.icon16, r, t.text2);
        return;
    }
    const Category* c = FindCategory(id);
    if (!c) {
        Glyph(kIconNoCategory, g_fonts.icon16, r, t.text2);
    } else if (!c->emoji.empty()) {
        Emoji(c->emoji, g_fonts.emoji16, Inset(r, -4, -4));
    } else {
        Glyph(kIconFolder, g_fonts.icon16, r, c->color ? CategoryColor(c->color) : t.text2);
    }
}

// Highlights the fields of a template in its preview.
void DrawTemplateText(const std::wstring& text, const D2D1_RECT_F& r, D2D1_COLOR_F color) {
    std::wstring line = OneLine(text, 300);
    IDWriteTextLayout* layout = MakeLayout(line, g_fonts.small, r.right - r.left, r.bottom - r.top);
    if (!layout) {
        return;
    }
    ID2D1SolidColorBrush* accent = nullptr;
    g_rt->CreateSolidColorBrush(g_theme.accentText, &accent);
    for (const wchar_t* field :
         {L"{data}", L"{godzina}", L"{schowek}", L"{date}", L"{time}", L"{clipboard}"}) {
        size_t n = wcslen(field);
        for (size_t pos = line.find(field); pos != std::wstring::npos;
             pos = line.find(field, pos + n)) {
            layout->SetDrawingEffect(accent, {(UINT32)pos, (UINT32)n});
        }
    }
    Layout(layout, {r.left, r.top}, color);
    SafeRelease(accent);
    layout->Release();
}

////////////////////////////////////////////////////////////////////////////////
// Menus for items

void OpenPinMenu(const D2D1_RECT_F& anchor, Hit owner, UINT64 id) {
    if (ToggleFlyout(owner)) {
        return;
    }
    Item* it = FindItem(id);
    if (!it) {
        return;
    }
    bool pinned = it->kind == Kind::Pinned;
    UINT64 last = pinned ? it->category : (UINT64)Setting(L"lastCategory", 0);
    std::vector<MenuItem> items;
    int focus = -1;
    auto add = [&](UINT64 category, const std::wstring& label, const std::wstring& emoji,
                   wchar_t glyph) {
        MenuItem m;
        m.label = label;
        m.emoji = emoji;
        m.glyph = emoji.empty() ? glyph : 0;
        m.checked = pinned && it->category == category;
        m.action = [id, category] { PinItem(id, category); };
        if (category == last) {
            focus = (int)items.size();
        }
        items.push_back(std::move(m));
    };
    for (const Category& c : g_categories) {
        add(c.id, c.name, c.emoji, kIconFolder);
    }
    add(0, Tr(L"Bez kategorii", L"Uncategorized"), L"", kIconNoCategory);
    items.push_back({.separator = true});
    MenuItem create;
    create.label = Tr(L"Nowa kategoria…", L"New category…");
    create.glyph = kIconAdd;
    create.action = [id] {
        ShowCategoryDialog(0, [id](UINT64 category) { PinItem(id, category); });
    };
    items.push_back(std::move(create));
    OpenMenu(anchor, owner, std::move(items), 220);
    g_s->flyout.focus = focus;
}

void ConfirmDeleteTemplate(const D2D1_RECT_F& anchor, Hit owner, UINT64 id) {
    Item* it = FindItem(id);
    if (!it || ToggleFlyout(owner)) {
        return;
    }
    OpenConfirm(anchor, owner,
                Tr(L"Usunąć szablon „", L"Delete the template “") + it->name +
                    Tr(L"”? Trafi do kosza.", L"”? It goes to the trash."),
                Tr(L"Usuń", L"Delete"), [id] { TrashItem(id); });
}

bool CanEditText(const Item& it) {
    return !it.imageWidth && it.files.empty() && it.kind != Kind::Template;
}

void EditItem(UINT64 id) {
    Item* it = FindItem(id);
    if (!it) {
        return;
    }
    if (it->kind == Kind::Template) {
        ShowTemplateDialog(id);
    } else if (it->imageWidth) {
        OpenEditor(id, false);
    } else if (CanEditText(*it)) {
        StartTextEdit(id);
    }
}

void OpenItemMenu(const D2D1_RECT_F& anchor, UINT64 id) {
    Item* it = FindItem(id);
    if (!it) {
        return;
    }
    g_selectedId = id;
    Hit owner = HIT(id);
    std::vector<MenuItem> items;
    auto add = [&](const std::wstring& label, wchar_t glyph, std::function<void()> action,
                   const std::wstring& hint = L"", bool danger = false) {
        MenuItem m;
        m.label = label;
        m.glyph = glyph;
        m.hint = hint;
        m.danger = danger;
        m.action = std::move(action);
        items.push_back(std::move(m));
    };
    add(
        Tr(L"Wklej", L"Paste"), kIconPaste, [id] { PasteId(id, PasteMode::Normal); },
        KeyName(g_keys[ScPaste]));
    if (!it->imageWidth && it->kind != Kind::Template) {
        add(
            Tr(L"Wklej jako zwykły tekst", L"Paste as plain text"), kIconText,
            [id] { PasteId(id, PasteMode::Plain); }, KeyName(g_keys[ScPastePlain]));
    }
    items.push_back({.separator = true});
    add(
        Tr(L"Szczegóły", L"Details"), kIconPane, [id] { OpenDetails(id); },
        KeyName(g_keys[ScDetails]));
    if (it->kind == Kind::History) {
        add(
            Tr(L"Przypnij…", L"Pin…"), kIconPin,
            [anchor, owner, id] { OpenPinMenu(anchor, owner, id); }, KeyName(g_keys[ScPin]));
    } else if (it->kind == Kind::Pinned) {
        add(
            Tr(L"Zmień kategorię…", L"Change category…"), kIconFolder,
            [anchor, owner, id] { OpenPinMenu(anchor, owner, id); }, KeyName(g_keys[ScPin]));
        add(Tr(L"Odepnij", L"Unpin"), kIconUnpin, [id] { UnpinItem(id); });
    }
    if (it->imageWidth || CanEditText(*it) || it->kind == Kind::Template) {
        add(Tr(L"Edytuj", L"Edit"), kIconEdit, [id] { EditItem(id); }, KeyName(g_keys[ScEdit]));
    }
    if (it->imageWidth || !it->text.empty()) {
        add(Tr(L"Zapisz jako…", L"Save as…"), kIconSave, [id] { SaveItemAs(id); });
    }
    if (CanOpen(*it)) {
        add(Tr(L"Otwórz", L"Open"), kIconOpen, [id] { OpenItem(id); });
    }
    items.push_back({.separator = true});
    if (it->kind == Kind::Template) {
        add(
            Tr(L"Usuń", L"Delete"), kIconDelete,
            [anchor, owner, id] { ConfirmDeleteTemplate(anchor, owner, id); },
            KeyName(g_keys[ScDelete]), true);
    } else {
        add(
            Tr(L"Usuń", L"Delete"), kIconDelete, [id] { TrashItem(id); }, KeyName(g_keys[ScDelete]),
            true);
    }
    OpenMenu(anchor, owner, std::move(items), 240);
}

// The clickable part of a list entry, shared by every list.
Region& EntryRegion(const D2D1_RECT_F& r, Hit h, UINT64 id) {
    Region& region = Add(r, h);
    region.click = [id] {
        PasteId(id, GetKeyState(VK_SHIFT) < 0 ? PasteMode::Plain : PasteMode::Normal);
    };
    region.rclick = [id] {
        D2D1_POINT_2F p = g_s->mouse;
        OpenItemMenu({p.x, p.y, p.x, p.y}, id);
    };
    return region;
}

void DrawEntryBackground(const D2D1_RECT_F& r, Hit h, UINT64 id) {
    const Theme& t = g_theme;
    bool selected = id == g_selectedId;
    bool hover = MouseOver(r);
    if (hover || selected || id == g_detailsId) {
        Fill(r, Pressed(h) ? t.pressed : t.hover, 4);
    }
    if (selected) {
        float cy = (r.top + r.bottom) / 2;
        Fill({r.left, cy - 8, r.left + 3, cy + 8}, t.accent, 1.5f);
    }
}

// Buttons shown on the right of a hovered entry; returns their left edge.
float HoverButtons(const D2D1_RECT_F& r, const Item& it, bool trash = false) {
    UINT64 id = it.id;
    float x = r.right - 4;
    float cy = (r.top + r.bottom) / 2;
    auto button = [&](Hit h, wchar_t glyph, const wchar_t* tip) -> Region& {
        x -= 32;
        return IconButton({x, cy - 16, x + 32, cy + 16}, h, glyph, tip, g_theme.text,
                          g_fonts.icon14);
    };
    if (trash) {
        button(HIT(id), kIconDelete, Tr(L"Usuń na stałe", L"Delete permanently")).click = [id] {
            PurgeItem(id);
        };
        button(HIT(id), kIconRestore, Tr(L"Przywróć", L"Restore")).click = [id] {
            RestoreItem(id, true);
        };
        return x;
    }
    if (it.kind == Kind::Template) {
        Hit del = HIT(id);
        D2D1_RECT_F anchor{x - 32, cy - 16, x, cy + 16};
        button(del, kIconDelete, Tr(L"Usuń", L"Delete")).click = [anchor, del, id] {
            ConfirmDeleteTemplate(anchor, del, id);
        };
        button(HIT(id), kIconEdit, Tr(L"Edytuj", L"Edit")).click = [id] { ShowTemplateDialog(id); };
        return x;
    }
    button(HIT(id), kIconDelete, Tr(L"Usuń do kosza", L"Move to trash")).click = [id] {
        TrashItem(id);
    };
    if (it.kind == Kind::Pinned) {
        button(HIT(id), kIconUnpin, Tr(L"Odepnij", L"Unpin")).click = [id] { UnpinItem(id); };
    }
    button(HIT(id), kIconPane, Tr(L"Szczegóły", L"Details")).click = [id] {
        g_detailsId == id ? CloseDetails() : OpenDetails(id);
    };
    if (it.kind == Kind::History) {
        Hit pin = HIT(id);
        D2D1_RECT_F anchor{x - 32, cy - 16, x, cy + 16};
        button(pin, kIconPin, Tr(L"Przypnij", L"Pin")).click = [anchor, pin, id] {
            OpenPinMenu(anchor, pin, id);
        };
    }
    if (it.imageWidth) {
        float w = ButtonWidth(Tr(L"Edytuj", L"Edit"), kIconEdit) - 4;
        x -= w + 4;
        Button({x, cy - 14, x + w, cy + 14}, HIT(id), Tr(L"Edytuj", L"Edit"), kIconEdit).click =
            [id] { OpenEditor(id, false); };
    }
    return x;
}

////////////////////////////////////////////////////////////////////////////////
// History

void DrawHistoryEntry(const Item& it, const D2D1_RECT_F& r) {
    const Theme& t = g_theme;
    Hit h = HIT(it.id);
    DrawEntryBackground(r, h, it.id);
    EntryRegion(r, h, it.id);
    DrawItemIcon(it, {r.left + 12, r.top + 12, r.left + 28, r.bottom - 12});
    float right = r.right - 8;
    if (MouseOver(r)) {
        right = HoverButtons(r, it) - 4;
    } else {
        std::wstring meta = FormatTime(it.time);
        if (!it.source.empty()) {
            meta = it.source + L" · " + meta;
        }
        float metaWidth = std::min(TextWidth(meta, g_fonts.meta), (r.right - r.left) / 3);
        Text(meta, g_fonts.meta, {r.right - 8 - metaWidth, r.top, r.right - 8, r.bottom},
             t.text2);
        right = r.right - 8 - metaWidth - 12;
        if (g_pinnedHashes.contains(HashKey(it))) {
            Glyph(kIconPinned, g_fonts.icon12, {right - 14, r.top, right, r.bottom}, t.text2);
            right -= 20;
        }
    }
    Text(it.preview, g_fonts.row, {r.left + 40, r.top, right, r.bottom}, t.text);
}

void DrawList(const D2D1_RECT_F& area, const std::function<void(const Row&, D2D1_RECT_F)>& draw) {
    float top = BeginScroll(g_listScroll, area);
    float clipTop = area.top, clipBottom = area.bottom;
    for (const Row& row : g_rows) {
        float y = top + row.y;
        if (y + row.h < clipTop) {
            continue;
        }
        if (y > clipBottom) {
            break;
        }
        draw(row, {area.left + 4, y, area.right - 4, y + row.h});
    }
    EndScroll(g_listScroll, area, g_rowsHeight);
}

void DrawHistory(const D2D1_RECT_F& area) {
    if (g_rows.empty()) {
        EmptyMessage(!g_loaded          ? Tr(L"Wczytywanie historii…", L"Loading the history…")
                     : !g_query.empty() ? Tr(L"Brak wyników. Spróbuj innych słów.",
                                             L"No results. Try other words.")
                     : g_paused ? Tr(L"Zapisywanie jest wstrzymane. Wznowisz je w menu „…”.",
                                     L"Saving is paused. You can resume it in the “…” menu.")
                                : Tr(L"Historia jest pusta. Skopiuj coś, a pojawi się tutaj.",
                                     L"The history is empty. Copy something and it shows up here."),
                     area);
        return;
    }
    DrawList(area, [](const Row& row, D2D1_RECT_F r) {
        if (row.type == Row::Header) {
            Text(row.header, g_fonts.header, {r.left + 8, r.top + 8, r.right - 8, r.bottom},
                 g_theme.text2);
        } else if (const Item* it = FindItem(row.id)) {
            DrawHistoryEntry(*it, r);
        }
    });
}

////////////////////////////////////////////////////////////////////////////////
// Pinned

// Second line of a pinned entry: tags, source and date.
void DrawPinnedMeta(const Item& it, const D2D1_RECT_F& r) {
    const Theme& t = g_theme;
    float x = r.left;
    for (const std::wstring& tag : it.tags) {
        std::wstring s = L"#" + tag;
        float w = TextWidth(s, g_fonts.small);
        if (x + w > r.right) {
            break;
        }
        Text(s, g_fonts.small, {x, r.top, x + w + 1, r.bottom}, t.accentText);
        x += w + 6;
    }
    std::wstring meta;
    if (!it.source.empty()) {
        meta = it.source + L" · ";
    }
    meta += FormatShort(it.pinnedTime ? it.pinnedTime : it.time);
    if (x > r.left) {
        meta = L"· " + meta;
    }
    Text(meta, g_fonts.small, {x, r.top, r.right, r.bottom}, t.text2);
}

void DrawPinnedEntry(const Item& it, const D2D1_RECT_F& r) {
    const Theme& t = g_theme;
    Hit h = HIT(it.id);
    DrawEntryBackground(r, h, it.id);
    EntryRegion(r, h, it.id);
    DrawTile(it, {r.left + 10, r.top + 12, r.left + 42, r.top + 44});
    float right = MouseOver(r) ? HoverButtons(r, it) - 4 : r.right - 10;
    Text(it.preview, g_fonts.row, {r.left + 54, r.top + 8, right, r.top + 30}, t.text);
    DrawPinnedMeta(it, {r.left + 54, r.top + 30, right, r.top + 48});
}

void CategoryMenu(const D2D1_RECT_F& anchor, Hit owner, UINT64 id) {
    if (!FindCategory(id) || ToggleFlyout(owner)) {
        return;
    }
    std::vector<MenuItem> items;
    MenuItem edit;
    edit.label = Tr(L"Edytuj kategorię…", L"Edit category…");
    edit.glyph = kIconEdit;
    edit.action = [id] { ShowCategoryDialog(id, nullptr); };
    items.push_back(edit);
    MenuItem del;
    del.label = Tr(L"Usuń kategorię", L"Delete category");
    del.glyph = kIconDelete;
    del.danger = true;
    del.action = [anchor, owner, id] {
        const Category* c = FindCategory(id);
        if (!c) {
            return;
        }
        OpenConfirm(anchor, owner,
                    Tr(L"Usunąć kategorię „", L"Delete the category “") + c->name +
                        Tr(L"”? Jej elementy zostaną w przypiętych, w „Bez kategorii”.",
                           L"”? Its items stay pinned, in “Uncategorized”."),
                    Tr(L"Usuń", L"Delete"), [id] { DeleteCategory(id); });
    };
    items.push_back(del);
    OpenMenu(anchor, owner, std::move(items));
}

size_t PinnedCount(UINT64 category) {
    return std::count_if(g_items.begin(), g_items.end(), [category](const Item& it) {
        if (it.kind != Kind::Pinned || !Live(it)) {
            return false;
        }
        UINT64 c = FindCategory(it.category) ? it.category : 0;
        return category == kAllCategories || c == category;
    });
}

// Tag chips, wrapped; returns the bottom.
float DrawTagChips(float x0, float y, float right, float maxRows = 1000) {
    float x = x0;
    int rows = 1;
    for (int i = 0; i < (int)g_tagCounts.size(); i++) {
        const auto& [tag, count] = g_tagCounts[i];
        std::wstring label = L"#" + tag + L"  " + std::to_wstring(count);
        float w = ChipWidth(label);
        if (x + w > right && x > x0) {
            if (++rows > maxRows) {
                break;
            }
            x = x0;
            y += 30;
        }
        bool selected = EqualsNoCase(g_tagFilter, tag);
        std::wstring name = tag;
        Chip(x, y, HIT(i), label, selected, g_theme.text).click = [name] {
            g_tagFilter = EqualsNoCase(g_tagFilter, name) ? L"" : name;
            g_listScroll.Reset();
            Refresh();
        };
        x += w + 6;
    }
    return y + 24;
}

void DrawCategoryColumn(const D2D1_RECT_F& area) {
    const Theme& t = g_theme;
    float y = BeginScroll(g_catScroll, area);
    float top = y;
    auto entry = [&](UINT64 id, const std::wstring& name) {
        D2D1_RECT_F r{area.left + 4, y, area.right - 6, y + 36};
        Hit h = HIT(id);
        bool selected = g_pinFilter == id;
        if (selected || MouseOver(r)) {
            Fill(r, Pressed(h) ? t.pressed : t.hover, 4);
        }
        if (selected) {
            Fill({r.left, r.top + 10, r.left + 3, r.bottom - 10}, t.accent, 1.5f);
        }
        DrawCategoryIcon(id, {r.left + 10, r.top + 10, r.left + 26, r.bottom - 10});
        std::wstring count = std::to_wstring(PinnedCount(id));
        float cw = TextWidth(count, g_fonts.meta);
        Text(count, g_fonts.meta, {r.right - 8 - cw, r.top, r.right - 8, r.bottom}, t.text2);
        float nameRight = r.right - 16 - cw;
        const Category* c = FindCategory(id);
        if (c && c->color && !c->emoji.empty()) {
            nameRight -= 12;
            Circle(nameRight + 6, (r.top + r.bottom) / 2, 3.5f, CategoryColor(c->color));
        }
        Text(name, g_fonts.row, {r.left + 34, r.top, nameRight, r.bottom},
             selected ? t.text : t.text);
        Region& region = Add(r, h);
        region.click = [id] {
            g_pinFilter = id;
            g_listScroll.Reset();
            Refresh();
        };
        if (c) {
            region.rclick = [r, h, id] { CategoryMenu(r, h, id); };
            region.dblclick = [id] { ShowCategoryDialog(id, nullptr); };
        }
        y += 38;
    };
    entry(kAllCategories, Tr(L"Wszystkie", L"All"));
    for (const Category& c : g_categories) {
        entry(c.id, c.name);
    }
    entry(0, Tr(L"Bez kategorii", L"Uncategorized"));
    D2D1_RECT_F add{area.left + 4, y + 2, area.right - 6, y + 34};
    Hit addHit = HIT(0);
    Fill(add, SubtleFill(addHit), 4);
    Stroke(add, t.strong, 4, 1, DashStyle());
    Glyph(kIconAdd, g_fonts.icon12, {add.left + 10, add.top, add.left + 26, add.bottom}, t.text2);
    Text(Tr(L"Nowa kategoria", L"New category"), g_fonts.small,
         {add.left + 34, add.top, add.right - 4, add.bottom}, t.text2);
    Add(add, addHit).click = [] {
        ShowCategoryDialog(0, [](UINT64 id) {
            g_pinFilter = id;
            Refresh();
        });
    };
    y += 48;
    if (!g_tagCounts.empty()) {
        Text(Tr(L"Tagi", L"Tags"), g_fonts.header, {area.left + 8, y, area.right, y + 20}, t.text2);
        y = DrawTagChips(area.left + 8, y + 26, area.right - 8) + 8;
    }
    EndScroll(g_catScroll, area, y - top + g_catScroll.pos + 8);
}

void DrawPinnedHeader(const D2D1_RECT_F& r) {
    const Theme& t = g_theme;
    UINT64 id = g_pinFilter;
    const Category* c = FindCategory(id);
    if (id != kAllCategories && id != 0 && !c) {
        g_pinFilter = id = kAllCategories;
    }
    D2D1_RECT_F tile{r.left + 10, r.top + 8, r.left + 42, r.top + 40};
    Fill(tile, c && c->color ? CategoryColor(c->color, 0.22f) : t.control, 6);
    Stroke(tile, t.controlStroke, 6);
    DrawCategoryIcon(id, Inset(tile, 8, 8));
    std::wstring name = id == kAllCategories ? Tr(L"Wszystkie", L"All")
                        : c                  ? c->name
                                             : Tr(L"Bez kategorii", L"Uncategorized");
    if (!g_tagFilter.empty()) {
        name += L" · #" + g_tagFilter;
    }
    float right = r.right - 8;
    if (c) {
        Hit h = HIT(id);
        D2D1_RECT_F b{r.right - 40, r.top + 8, r.right - 8, r.top + 40};
        IconButton(b, h, kIconEdit, Tr(L"Edytuj kategorię", L"Edit category")).click = [id] {
            ShowCategoryDialog(id, nullptr);
        };
        right = b.left - 4;
    }
    Text(name, g_fonts.subtitle, {r.left + 54, r.top + 6, right, r.top + 26}, t.text);
    size_t count = EntryCount();
    Text(Count(count, Tr(L"element", L"item"), Tr(L"elementy", L"items"),
               Tr(L"elementów", L"items")),
         g_fonts.small, {r.left + 54, r.top + 25, right, r.top + 42}, t.text2);
}

void DrawSectionRow(const Row& row, const D2D1_RECT_F& r) {
    const Theme& t = g_theme;
    UINT64 id = row.id;
    const Category* c = FindCategory(id);
    bool collapsed = c ? c->collapsed : Setting(L"noCategoryCollapsed", 0) != 0;
    Hit h = HIT(id);
    if (MouseOver(r)) {
        Fill(r, Pressed(h) ? t.pressed : t.hover, 4);
    }
    Glyph(collapsed ? kIconChevronRight : kIconChevronDown, g_fonts.icon10,
          {r.left + 6, r.top, r.left + 22, r.bottom}, t.text2);
    DrawCategoryIcon(id, {r.left + 28, r.top + 12, r.left + 44, r.bottom - 12});
    std::wstring name = c ? c->name : Tr(L"Bez kategorii", L"Uncategorized");
    float nameWidth = std::min(TextWidth(name, g_fonts.rowBold), r.right - r.left - 160);
    Text(name, g_fonts.rowBold, {r.left + 52, r.top, r.left + 52 + nameWidth, r.bottom}, t.text);
    float x = r.left + 52 + nameWidth + 8;
    if (c && c->color) {
        Circle(x + 4, (r.top + r.bottom) / 2, 3.5f, CategoryColor(c->color));
        x += 14;
    }
    Text(std::to_wstring(PinnedCount(id)), g_fonts.small, {x, r.top, x + 60, r.bottom}, t.text2);
    Region& region = Add(r, h);
    region.click = [id] {
        if (Category* c = FindCategory(id)) {
            c->collapsed = !c->collapsed;
            SaveIndex();
        } else {
            SetSetting(L"noCategoryCollapsed", !Setting(L"noCategoryCollapsed", 0));
        }
        Refresh();
    };
    if (c) {
        region.rclick = [r, h, id] { CategoryMenu(r, h, id); };
        if (MouseOver(r)) {
            IconButton({r.right - 36, r.top + 4, r.right - 4, r.bottom - 4}, HIT(id), kIconEdit,
                       Tr(L"Edytuj kategorię", L"Edit category"), t.text, g_fonts.icon14)
                .click = [id] { ShowCategoryDialog(id, nullptr); };
        }
    }
}

void DrawPinned(const D2D1_RECT_F& area) {
    const Theme& t = g_theme;
    if (SectionsLayout()) {
        D2D1_RECT_F list = area;
        if (!g_tagCounts.empty()) {
            float bottom = DrawTagChips(area.left + 8, area.top + 6, area.right - 8, 2);
            list.top = bottom + 6;
        }
        if (g_rows.empty()) {
            EmptyMessage(Tr(L"Brak wyników. Spróbuj innych słów.", L"No results. Try other words."),
                         list);
            return;
        }
        DrawList(list, [](const Row& row, D2D1_RECT_F r) {
            if (row.type == Row::Section) {
                DrawSectionRow(row, r);
            } else if (row.type == Row::NewCategory) {
                const Theme& t = g_theme;
                D2D1_RECT_F add = Inset(r, 0, 4);
                Hit h = HIT(0);
                Fill(add, SubtleFill(h), 4);
                Stroke(add, t.strong, 4, 1, DashStyle());
                Glyph(kIconAdd, g_fonts.icon12, {add.left + 12, add.top, add.left + 28, add.bottom},
                      t.text2);
                Text(Tr(L"Nowa kategoria", L"New category"), g_fonts.small,
                     {add.left + 36, add.top, add.right, add.bottom}, t.text2);
                Add(add, h).click = [] { ShowCategoryDialog(0, nullptr); };
            } else if (const Item* it = FindItem(row.id)) {
                DrawPinnedEntry(*it, {r.left + 12, r.top, r.right, r.bottom});
            }
        });
        return;
    }
    float split = area.left + 178;
    DrawCategoryColumn({area.left, area.top, split, area.bottom});
    Fill({split, area.top + 4, split + 1, area.bottom - 4}, t.divider);
    D2D1_RECT_F right{split + 1, area.top, area.right, area.bottom};
    DrawPinnedHeader(right);
    D2D1_RECT_F list{right.left, right.top + 48, right.right, right.bottom};
    if (g_rows.empty()) {
        bool none = CountItems(Kind::Pinned) == 0;
        EmptyMessage(
            none ? Tr(L"Nic tu jeszcze nie ma. W historii najedź na element i kliknij "
                      L"pinezkę albo naciśnij ",
                      L"Nothing here yet. In the history, hover over an item and click the pin, or "
                      L"press ") +
                       KeyName(g_keys[ScPin]) + L"."
            : !g_query.empty()
                ? Tr(L"Brak wyników. Spróbuj innych słów.", L"No results. Try other words.")
                : Tr(L"W tej kategorii nie ma elementów.", L"This category has no items."),
            list);
        return;
    }
    DrawList(list, [](const Row& row, D2D1_RECT_F r) {
        if (const Item* it = FindItem(row.id)) {
            DrawPinnedEntry(*it, r);
        }
    });
}

////////////////////////////////////////////////////////////////////////////////
// Templates

void DrawTemplateEntry(const Item& it, const D2D1_RECT_F& r) {
    const Theme& t = g_theme;
    Hit h = HIT(it.id);
    DrawEntryBackground(r, h, it.id);
    EntryRegion(r, h, it.id).dblclick = [id = it.id] { ShowTemplateDialog(id); };
    D2D1_RECT_F tile{r.left + 10, r.top + 12, r.left + 42, r.top + 44};
    Fill(tile, t.control, 6);
    Stroke(tile, t.controlStroke, 6);
    if (!it.emoji.empty()) {
        Emoji(it.emoji, g_fonts.emoji20, tile);
    } else {
        Glyph(kIconTemplate, g_fonts.icon16, tile, t.text2);
    }
    float right = r.right - 10;
    if (MouseOver(r)) {
        right = HoverButtons(r, it) - 4;
    } else if (it.hotkey && !(it.hotkey & kKeyOff)) {
        std::wstring key = KeyName(it.hotkey);
        float w = TextWidth(key, g_fonts.key) + 12;
        float cy = (r.top + r.bottom) / 2;
        Kbd(r.right - 10 - w, cy, key);
        right = r.right - 10 - w - 8;
        if (g_hotkeyFailed.contains(it.id)) {
            D2D1_RECT_F warn{right - 20, cy - 10, right, cy + 10};
            Glyph(kIconWarning, g_fonts.icon14, warn, t.warning);
            Add(warn, HIT(it.id)).tip =
                Tr(L"Ten skrót zajął już inny program", L"Another app already uses this shortcut");
            right -= 24;
        }
    }
    Text(it.name, g_fonts.rowBold, {r.left + 54, r.top + 8, right, r.top + 30}, t.text);
    DrawTemplateText(it.text, {r.left + 54, r.top + 30, right, r.top + 48}, t.text2);
}

void DrawTemplates(const D2D1_RECT_F& area) {
    const Theme& t = g_theme;
    D2D1_RECT_F head{area.left + 8, area.top + 4, area.right - 8, area.top + 44};
    float bw = ButtonWidth(Tr(L"Nowy szablon", L"New template"), kIconAdd);
    Button({head.right - bw, head.top + 4, head.right, head.bottom - 4}, HIT(0),
           Tr(L"Nowy szablon", L"New template"), kIconAdd, kAccent)
        .click = [] { ShowTemplateDialog(0); };
    float x = head.left + 4;
    float labelWidth = TextWidth(
        Tr(L"Pola wypełniane przy wklejaniu:", L"Fields filled in when pasting:"), g_fonts.small);
    if (x + labelWidth + 220 < head.right - bw - 8) {
        Text(Tr(L"Pola wypełniane przy wklejaniu:", L"Fields filled in when pasting:"),
             g_fonts.small, {x, head.top, x + labelWidth + 1, head.bottom}, t.text2);
        x += labelWidth + 8;
    }
    for (int i = 0; i < 3; i++) {
        const wchar_t* field = kTemplateFields[i];
        float w = ChipWidth(field);
        if (x + w > head.right - bw - 8) {
            break;
        }
        D2D1_RECT_F chip{x, head.top + 8, x + w, head.top + 32};
        Fill(chip, t.control, 12);
        Stroke(chip, t.controlStroke, 12);
        Text(field, g_fonts.small, {chip.left + 10, chip.top, chip.right, chip.bottom},
             t.accentText);
        Add(chip, HIT(i)).tip =
            i == 0   ? Tr(L"Dzisiejsza data", L"Today's date")
            : i == 1 ? Tr(L"Bieżąca godzina", L"Current time")
                     : Tr(L"To, co jest teraz w schowku", L"What's on the clipboard now");
        x += w + 6;
    }
    D2D1_RECT_F list{area.left, head.bottom + 4, area.right, area.bottom};
    Fill({area.left + 8, head.bottom + 2, area.right - 8, head.bottom + 3}, t.divider);
    if (g_rows.empty()) {
        EmptyMessage(
            CountItems(Kind::Template)
                ? Tr(L"Brak wyników. Spróbuj innych słów.", L"No results. Try other words.")
                : Tr(L"Brak szablonów. Kliknij „Nowy szablon”, żeby zapisać gotowy tekst "
                     L"z polami {data}, {godzina} i {schowek}.",
                     L"No templates yet. Click “New template” to save a ready text with the fields "
                     L"{date}, {time} and {clipboard}."),
            list);
        return;
    }
    DrawList(list, [](const Row& row, D2D1_RECT_F r) {
        if (const Item* it = FindItem(row.id)) {
            DrawTemplateEntry(*it, r);
        }
    });
}

////////////////////////////////////////////////////////////////////////////////
// Details

struct InfoRow {
    std::wstring label, value;
};

std::vector<InfoRow> ItemInfo(const Item& it) {
    std::vector<InfoRow> rows;
    if (it.imageWidth) {
        rows.push_back(
            {Tr(L"Rozdzielczość", L"Resolution"),
             std::to_wstring(it.imageWidth) + L" × " + std::to_wstring(it.imageHeight) + L" px"});
        rows.push_back(
            {Tr(L"Plik", L"File"), Tr(L"PNG bez kompresji stratnej · ", L"PNG, lossless · ") +
                                       FormatSize(ImageFileSize(it.id))});
    } else if (!it.files.empty()) {
        size_t n = std::count(it.files.begin(), it.files.end(), L'\n') + 1;
        rows.push_back(
            {Tr(L"Pliki", L"Files"), Count(n, Tr(L"ścieżka", L"path"), Tr(L"ścieżki", L"paths"),
                                           Tr(L"ścieżek", L"paths"))});
    } else {
        size_t lines = std::count(it.text.begin(), it.text.end(), L'\n') + 1;
        rows.push_back({Tr(L"Długość", L"Length"),
                        Count(it.text.size(), Tr(L"znak", L"character"),
                              Tr(L"znaki", L"characters"), Tr(L"znaków", L"characters")) +
                            L", " +
                            Count(lines, Tr(L"wiersz", L"line"), Tr(L"wiersze", L"lines"),
                                  Tr(L"wierszy", L"lines"))});
        if (it.type == ItemType::Color) {
            WCHAR rgb[64];
            swprintf_s(rgb, L"RGB %u, %u, %u", (it.color >> 16) & 0xFF, (it.color >> 8) & 0xFF,
                       it.color & 0xFF);
            rows.push_back({Tr(L"Kolor", L"Color"), rgb});
        }
    }
    if (!it.source.empty()) {
        rows.push_back({Tr(L"Źródło", L"Source"), it.source});
    }
    if (it.kind == Kind::Template) {
        rows.push_back({Tr(L"Utworzono", L"Created"), FormatWhen(it.time)});
        rows.push_back(
            {Tr(L"Skrót", L"Shortcut"), it.hotkey ? KeyName(it.hotkey) : Tr(L"brak", L"none")});
    } else {
        rows.push_back({Tr(L"Skopiowano", L"Copied"), FormatWhen(it.time)});
    }
    if (it.kind == Kind::Pinned && it.pinnedTime) {
        rows.push_back({Tr(L"Przypięto", L"Pinned"), FormatWhen(it.pinnedTime)});
    }
    rows.push_back({Tr(L"Wklejano", L"Pasted"),
                    it.pasteCount ? Count(it.pasteCount, Tr(L"raz", L"time"), Tr(L"razy", L"times"),
                                          Tr(L"razy", L"times")) +
                                        Tr(L" · ostatnio ", L" · last ") + FormatWhen(it.lastPasted)
                                  : Tr(L"jeszcze nie", L"not yet")});
    return rows;
}

float SectionTitle(const std::wstring& title, float x, float y, float right) {
    Text(title, g_fonts.header, {x, y, right, y + 20}, g_theme.text2);
    return y + 26;
}

// The preview block; returns its bottom.
float DrawPreview(const Item& it, float x, float y, float right) {
    const Theme& t = g_theme;
    float w = right - x;
    if (it.imageWidth) {
        float h = std::min(260.f, std::max(120.f, w * it.imageHeight / std::max(1u, it.imageWidth)));
        D2D1_RECT_F box{x, y, right, y + h};
        Fill(box, t.control, 6);
        Stroke(box, t.controlStroke, 6);
        if (ID2D1Bitmap* bitmap = PreviewBitmap(it.id)) {
            D2D1_RECT_F drawn;
            DrawFitted(bitmap, Inset(box, 6, 6), 1e9f, &drawn);
            Add(drawn, HIT(it.id)).dblclick = [id = it.id] { OpenEditor(id, true); };
        } else {
            LoadPreview(it.id);
            Text(Tr(L"Wczytywanie podglądu…", L"Loading the preview…"), g_fonts.smallCenter, box,
                 t.text2);
        }
        Text(Tr(L"Podgląd · 100% w „Otwórz”", L"Preview · 100% in “Open”"), g_fonts.small,
             {x, box.bottom + 4, right, box.bottom + 22}, t.text2);
        return box.bottom + 26;
    }
    if (it.type == ItemType::Color && it.kind != Kind::Template) {
        D2D1_RECT_F swatch{x, y, right, y + 72};
        Fill(swatch, Rgba(it.color), 6);
        Stroke(swatch, t.controlStroke, 6);
        Text(Trim(it.text), g_fonts.subtitle, {x, swatch.bottom + 6, right, swatch.bottom + 28},
             t.text);
        return swatch.bottom + 32;
    }
    if (!it.files.empty()) {
        float fy = y;
        size_t start = 0;
        int shown = 0;
        while (start < it.files.size() && shown < 8) {
            size_t end = it.files.find(L'\n', start);
            std::wstring path = it.files.substr(start, end == std::wstring::npos ? std::wstring::npos
                                                                                 : end - start);
            start = end == std::wstring::npos ? it.files.size() : end + 1;
            std::wstring name = FileName(path);
            std::wstring folder = path.substr(0, path.size() - name.size());
            Glyph(kIconFiles, g_fonts.icon14, {x, fy, x + 16, fy + 36}, t.text2);
            Text(name, g_fonts.row, {x + 26, fy, right, fy + 20}, t.text);
            Text(folder, g_fonts.small, {x + 26, fy + 18, right, fy + 34}, t.text2);
            fy += 40;
            shown++;
        }
        return fy + 4;
    }
    if (g_editingText && g_detailsId == it.id) {
        D2D1_RECT_F field{x, y, right, y + 220};
        PlaceField(FEditText, field, Tr(L"Treść elementu", L"Item content"));
        float by = field.bottom + 8;
        float bw = (w - 8) / 2;
        Button({x, by, x + bw, by + 32}, HIT(0), Tr(L"Zapisz", L"Save"), kIconSave, kAccent).click =
            [id = it.id] {
                SaveEditedText(id, FieldText(FEditText));
                g_editingText = false;
                SetFocus(g_hwnd);
                Redraw();
            };
        Button({right - bw, by, right, by + 32}, HIT(0), Tr(L"Anuluj", L"Cancel")).click = [] {
            g_editingText = false;
            SetFocus(g_hwnd);
            Redraw();
        };
        Text(it.html.empty() && it.rtf.empty()
                 ? Tr(L"Ctrl+Enter zapisuje, Esc anuluje.", L"Ctrl+Enter saves, Esc cancels.")
                 : Tr(L"Po zapisie formatowanie zniknie i zostanie zwykły tekst.",
                      L"Saving removes the formatting and keeps plain text."),
             g_fonts.small, {x, by + 38, right, by + 56}, t.text2);
        return by + 60;
    }
    bool code = it.type == ItemType::Code;
    IDWriteTextFormat* format = code ? g_fonts.mono : g_fonts.body;
    std::wstring text = it.text.size() > 4000 ? it.text.substr(0, 4000) + L"…" : it.text;
    float th = std::min(240.f, TextHeight(text, format, w - 24) + 2);
    D2D1_RECT_F box{x, y, right, y + th + 20};
    Fill(box, t.control, 6);
    Stroke(box, t.controlStroke, 6);
    PushClip(Inset(box, 1, 1));
    if (it.kind == Kind::Template) {
        IDWriteTextLayout* layout = MakeLayout(text, format, w - 24, th);
        if (layout) {
            ID2D1SolidColorBrush* accent = nullptr;
            g_rt->CreateSolidColorBrush(t.accentText, &accent);
            for (const wchar_t* field :
                 {L"{data}", L"{godzina}", L"{schowek}", L"{date}", L"{time}", L"{clipboard}"}) {
                size_t n = wcslen(field);
                for (size_t pos = text.find(field); pos != std::wstring::npos;
                     pos = text.find(field, pos + n)) {
                    layout->SetDrawingEffect(accent, {(UINT32)pos, (UINT32)n});
                }
            }
            Layout(layout, {x + 12, y + 10}, t.text);
            SafeRelease(accent);
            layout->Release();
        }
    } else {
        Text(text, format, {x + 12, y + 10, right - 12, y + 10 + th},
             it.type == ItemType::Link ? t.accentText : t.text);
    }
    PopClip();
    return box.bottom + 8;
}

// "Paste as" options.
float DrawPasteAs(const Item& it, float x, float y, float right) {
    const Theme& t = g_theme;
    struct Option {
        const wchar_t* label;
        wchar_t glyph;
        PasteMode mode;
    };
    std::vector<Option> options;
    if (it.imageWidth) {
        options = {{Tr(L"Obraz w oryginale", L"Original image"), kIconImage, PasteMode::Normal},
                   {Tr(L"Plik PNG", L"PNG file"), kIconFiles, PasteMode::PngFile},
                   {Tr(L"Ścieżka do pliku", L"File path"), kIconLink, PasteMode::PngPath}};
    } else if (it.files.empty() && it.kind != Kind::Template) {
        options.push_back({Tr(L"Zwykły tekst", L"Plain text"), kIconText, PasteMode::Plain});
        if (!it.html.empty() || !it.rtf.empty()) {
            options.push_back(
                {Tr(L"Z formatowaniem", L"With formatting"), kIconFont, PasteMode::Normal});
        }
    }
    if (options.empty()) {
        return y;
    }
    y = SectionTitle(Tr(L"Wklej jako", L"Paste as"), x, y, right);
    for (const Option& o : options) {
        D2D1_RECT_F r{x, y, right, y + 36};
        Hit h = HIT(o.glyph);
        Fill(r, t.control, 4);
        Fill(r, SubtleFill(h), 4);
        Stroke(r, t.controlStroke, 4);
        Glyph(o.glyph, g_fonts.icon14, {r.left + 10, r.top, r.left + 28, r.bottom}, t.text2);
        Text(o.label, g_fonts.row, {r.left + 38, r.top, r.right - 8, r.bottom}, t.text);
        PasteMode mode = o.mode;
        Add(r, h).click = [id = it.id, mode] { PasteId(id, mode); };
        y += 40;
    }
    return y + 8;
}

// Category, tags, emoji and color of a pinned item.
float DrawOrganize(const Item& it, float x, float y, float right) {
    const Theme& t = g_theme;
    UINT64 id = it.id;
    y = SectionTitle(Tr(L"Porządkowanie", L"Organize"), x, y, right);
    float labelWidth = 76;
    auto label = [&](const wchar_t* s, float h = 32) {
        Text(s, g_fonts.small, {x, y, x + labelWidth, y + h}, t.text2);
    };

    label(Tr(L"Kategoria", L"Category"));
    {
        D2D1_RECT_F r{x + labelWidth, y, right, y + 32};
        Hit h = HIT(id);
        Fill(r, t.control, 4);
        Fill(r, SubtleFill(h), 4);
        Stroke(r, t.controlStroke, 4);
        UINT64 category = FindCategory(it.category) ? it.category : 0;
        DrawCategoryIcon(category, {r.left + 10, r.top + 8, r.left + 26, r.bottom - 8});
        Text(CategoryName(category), g_fonts.row, {r.left + 34, r.top, r.right - 30, r.bottom}, t.text);
        Glyph(kIconChevronDown, g_fonts.icon10, {r.right - 28, r.top, r.right - 10, r.bottom}, t.text2);
        Add(r, h).click = [r, h, id] { OpenPinMenu(r, h, id); };
        y += 40;
    }

    label(Tr(L"Tagi", L"Tags"));
    {
        float tx = x + labelWidth;
        float ty = y + 4;
        for (int i = 0; i < (int)it.tags.size(); i++) {
            std::wstring tag = it.tags[i];
            std::wstring s = L"#" + tag;
            float w = ChipWidth(s, true);
            if (tx + w > right && tx > x + labelWidth) {
                tx = x + labelWidth;
                ty += 30;
            }
            Chip(tx, ty, HIT(i), s, false, t.accentText, true).click = [id, tag] {
                RemoveTag(id, tag);
            };
            tx += w + 6;
        }
        if (tx > x + labelWidth) {
            ty += 32;
        }
        D2D1_RECT_F field{x + labelWidth, ty - 4, right, ty + 28};
        Field& f = PlaceField(FTagAdd, field,
                              Tr(L"Dodaj tag i naciśnij Enter", L"Add a tag and press Enter"));
        f.onKey = [id](UINT vk) {
            if (vk == VK_RETURN) {
                AddTag(id, FieldText(FTagAdd));
                SetFieldText(FTagAdd, L"");
                return true;
            }
            return false;
        };
        y = field.bottom + 8;
    }

    label(L"Emoji");
    {
        Hit h = HIT(id);
        D2D1_RECT_F r{x + labelWidth, y, x + labelWidth + 120, y + 32};
        Fill(r, t.control, 4);
        Fill(r, SubtleFill(h), 4);
        Stroke(r, t.controlStroke, 4);
        if (it.emoji.empty()) {
            Glyph(kIconEmoji, g_fonts.icon14, {r.left + 10, r.top, r.left + 28, r.bottom}, t.text2);
            Text(Tr(L"Brak", L"None"), g_fonts.row, {r.left + 36, r.top, r.right, r.bottom},
                 t.text2);
        } else {
            Emoji(it.emoji, g_fonts.emoji20, {r.left + 6, r.top, r.left + 34, r.bottom});
            Text(Tr(L"Zmień", L"Change"), g_fonts.row, {r.left + 40, r.top, r.right, r.bottom},
                 t.text);
        }
        Add(r, h).click = [r, h, id] {
            if (!ToggleFlyout(h)) {
                OpenEmojiPicker(r, h, [id](std::wstring emoji) {
                    if (Item* it = FindItem(id)) {
                        it->emoji = emoji;
                        SaveIndex();
                        Redraw();
                    }
                });
            }
        };
        y += 40;
    }

    label(Tr(L"Kolor", L"Color"));
    {
        float cx = x + labelWidth + 14;
        for (BYTE c = 0; c < kColorCount; c++) {
            Hit h = HIT(c);
            D2D1_RECT_F r{cx - 14, y + 2, cx + 14, y + 30};
            float cy = y + 16;
            if (it.mark == c) {
                CircleStroke(cx, cy, 13, t.text, 2);
            } else if (Hot(h)) {
                CircleStroke(cx, cy, 13, t.strong, 1);
            }
            if (c == 0) {
                CircleStroke(cx, cy, 9, t.strong);
                Line(cx - 6, cy + 6, cx + 6, cy - 6, t.strong);
            } else {
                Circle(cx, cy, 9, CategoryColor(c));
            }
            Region& region = Add(r, h);
            region.tip = kCategoryColors[c].name;
            region.click = [id, c] {
                if (Item* it = FindItem(id)) {
                    it->mark = c;
                    SaveIndex();
                    Redraw();
                }
            };
            cx += 34;
        }
        y += 40;
    }
    return y + 4;
}

float DrawInfo(const Item& it, float x, float y, float right) {
    const Theme& t = g_theme;
    y = SectionTitle(Tr(L"Informacje", L"Information"), x, y, right);
    for (const InfoRow& row : ItemInfo(it)) {
        Text(row.label, g_fonts.small, {x, y, x + 100, y + 22}, t.text2);
        Text(row.value, g_fonts.small, {x + 104, y, right, y + 22}, t.text);
        y += 24;
    }
    return y + 8;
}

void DrawDetails(const D2D1_RECT_F& card, bool inPlace) {
    const Theme& t = g_theme;
    Item* found = FindItem(g_detailsId);
    if (!found) {
        return;
    }
    const Item& it = *found;
    UINT64 id = it.id;
    float x = card.left + 16, right = card.right - 16;

    // Header.
    float y = card.top + 8;
    if (inPlace) {
        IconButton({card.left + 4, y, card.left + 40, y + 32}, HIT(0), kIconBack,
                   Tr(L"Wróć do listy", L"Back to the list"))
            .click = [] { CloseDetails(); };
        std::wstring crumb = g_tab == Tab::History ? Tr(L"Historia", L"History")
                             : g_tab == Tab::Pinned
                                 ? Tr(L"Przypięte › ", L"Pinned › ") + CategoryName(it.category)
                                 : Tr(L"Szablony", L"Templates");
        Text(crumb, g_fonts.small, {card.left + 46, y, right, y + 32}, t.text2);
        y += 36;
    }
    D2D1_RECT_F chip{x, y + 4, x + 36, y + 40};
    Fill(chip, t.control, 6);
    Stroke(chip, t.controlStroke, 6);
    if (!it.emoji.empty() && it.kind != Kind::History) {
        Emoji(it.emoji, g_fonts.emoji20, chip);
    } else {
        Glyph(TypeGlyph(it), g_fonts.icon16, chip, t.text);
    }
    float headRight = inPlace ? right : right - 36;
    std::wstring title = it.kind == Kind::Template ? it.name : TypeLabel(it);
    Text(title, g_fonts.subtitle, {x + 46, y + 2, headRight, y + 22}, t.text);
    std::wstring meta = it.kind == Kind::Template ? Tr(L"Szablon", L"Template") : TypeLabel(it);
    meta = it.kind == Kind::Template ? meta : (it.source.empty() ? L"" : it.source + L" · ");
    meta += FormatWhen(it.kind == Kind::Pinned && it.pinnedTime ? it.pinnedTime : it.time);
    Text(meta, g_fonts.small, {x + 46, y + 22, headRight, y + 40}, t.text2);
    if (!inPlace) {
        IconButton({right - 32, y + 4, right, y + 36}, HIT(0), kIconClose,
                   Tr(L"Zamknij szczegóły", L"Close the details"))
            .click = [] { CloseDetails(); };
    }
    y += 50;

    // Commands.
    {
        float bx = x;
        float pw = ButtonWidth(Tr(L"Wklej", L"Paste"), kIconPaste);
        Button({bx, y, bx + pw, y + 32}, HIT(0), Tr(L"Wklej", L"Paste"), kIconPaste, kAccent)
            .click = [id] { PasteId(id, PasteMode::Normal); };
        bx += pw + 6;
        bool editable = it.imageWidth || CanEditText(it) || it.kind == Kind::Template;
        if (editable && !(g_editingText && g_detailsId == id)) {
            float ew = ButtonWidth(Tr(L"Edytuj", L"Edit"), kIconEdit);
            Button({bx, y, bx + ew, y + 32}, HIT(0), Tr(L"Edytuj", L"Edit"), kIconEdit).click =
                [id] { EditItem(id); };
            bx += ew + 6;
        }
        float ix = right;
        auto icon = [&](Hit h, wchar_t glyph, const wchar_t* tip) -> Region& {
            ix -= 34;
            return IconButton({ix, y, ix + 32, y + 32}, h, glyph, tip);
        };
        if (it.kind == Kind::Template) {
            Hit del = HIT(0);
            D2D1_RECT_F anchor{ix - 34, y, ix - 2, y + 32};
            icon(del, kIconDelete, Tr(L"Usuń", L"Delete")).click = [anchor, del, id] {
                ConfirmDeleteTemplate(anchor, del, id);
            };
        } else {
            icon(HIT(0), kIconDelete, Tr(L"Usuń do kosza", L"Move to trash")).click = [id] {
                TrashItem(id);
            };
        }
        if (CanOpen(it)) {
            icon(HIT(0), kIconOpen,
                 it.imageWidth ? Tr(L"Otwórz w 100%", L"Open at 100%") : Tr(L"Otwórz", L"Open"))
                .click = [id] { OpenItem(id); };
        }
        if (it.kind == Kind::History) {
            Hit pin = HIT(0);
            D2D1_RECT_F anchor{ix - 34, y, ix - 2, y + 32};
            icon(pin, kIconPin, Tr(L"Przypnij", L"Pin")).click = [anchor, pin, id] {
                OpenPinMenu(anchor, pin, id);
            };
        } else if (it.kind == Kind::Pinned) {
            icon(HIT(0), kIconUnpin, Tr(L"Odepnij", L"Unpin")).click = [id] { UnpinItem(id); };
        }
        if (it.imageWidth || !it.text.empty()) {
            icon(HIT(0), kIconSave, Tr(L"Zapisz jako…", L"Save as…")).click = [id] {
                SaveItemAs(id);
            };
        }
        y += 44;
    }
    Fill({card.left + 1, y - 4, card.right - 1, y - 3}, t.divider);

    // Body.
    D2D1_RECT_F view{card.left, y, card.right, card.bottom - 4};
    float top = BeginScroll(g_detailScroll, view);
    float by = top + 8;
    by = DrawPreview(it, x, by, right - 4);
    by = DrawPasteAs(it, x, by, right - 4);
    if (it.kind == Kind::Pinned) {
        by = DrawOrganize(it, x, by, right - 4);
    }
    by = DrawInfo(it, x, by, right - 4);
    EndScroll(g_detailScroll, view, by - top + 8);
}

void StartTextEdit(UINT64 id) {
    Item* it = FindItem(id);
    if (!it || !CanEditText(*it)) {
        return;
    }
    if (g_detailsId != id) {
        OpenDetails(id);
    }
    g_editingText = true;
    g_detailScroll.Reset();
    std::wstring text = it->text;
    // The EDIT control wants Windows line ends.
    std::wstring crlf;
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == L'\n' && (i == 0 || text[i - 1] != L'\r')) {
            crlf += L'\r';
        }
        crlf += text[i];
    }
    SetFieldText(FEditText, crlf);
    g_fields[FEditText].onKey = [id](UINT vk) {
        if (vk == VK_ESCAPE) {
            g_editingText = false;
            SetFocus(g_hwnd);
            Redraw();
            return true;
        }
        if (vk == VK_RETURN && GetKeyState(VK_CONTROL) < 0) {
            SaveEditedText(id, FieldText(FEditText));
            g_editingText = false;
            SetFocus(g_hwnd);
            Redraw();
            return true;
        }
        return false;
    };
    Redraw();
    UpdateWindow(g_hwnd);
    FocusField(FEditText);
}

////////////////////////////////////////////////////////////////////////////////
// Window chrome: title bar, tabs and footer

D2D1_RECT_F g_listArea;  // Where the current list was drawn, for anchoring menus.

void OpenMainMenu(const D2D1_RECT_F& anchor, Hit owner) {
    if (ToggleFlyout(owner)) {
        return;
    }
    std::vector<MenuItem> items;
    MenuItem pause;
    pause.label = Tr(L"Wstrzymaj zapisywanie", L"Pause saving");
    pause.glyph = kIconPause;
    pause.checked = g_paused;
    pause.action = [] { SetPaused(!g_paused); };
    items.push_back(pause);
    MenuItem trash;
    trash.label = Tr(L"Kosz", L"Trash");
    trash.glyph = kIconDelete;
    size_t inTrash = CountTrash();
    trash.hint = inTrash ? std::to_wstring(inTrash) : Tr(L"pusty", L"empty");
    trash.action = [] { ShowPage(Page::Trash); };
    items.push_back(trash);
    items.push_back({.separator = true});
    MenuItem exportItem;
    exportItem.label = Tr(L"Eksportuj dane…", L"Export data…");
    exportItem.glyph = kIconExport;
    exportItem.action = [] { ShowExportDialog(); };
    items.push_back(exportItem);
    MenuItem importItem;
    importItem.label = Tr(L"Importuj dane…", L"Import data…");
    importItem.glyph = kIconImport;
    importItem.action = [] { ShowImportDialog(); };
    items.push_back(importItem);
    items.push_back({.separator = true});
    MenuItem settings;
    settings.label = Tr(L"Ustawienia", L"Settings");
    settings.glyph = kIconSettings;
    settings.action = [] { ShowPage(Page::Settings); };
    items.push_back(settings);
    OpenMenu(anchor, owner, std::move(items), 240);
}

// Title bar of the main view and of the pages. Returns the room left for a
// title or the search field.
float DrawTitleButtons(float width, bool withMenu) {
    const Theme& t = g_theme;
    Add({0, 0, width, kTitleHeight}, HIT(0)).caption = true;
    float x = width - 4;
    x -= 40;
    IconButton({x, 4, x + 40, 36}, HIT(0), kIconClose, Tr(L"Zamknij", L"Close")).click = [] {
        HidePanel(true);
    };
    x -= 40;
    IconButton({x, 4, x + 40, 36}, HIT(0), g_onTop ? kIconPinned : kIconPin,
               g_onTop ? Tr(L"Odepnij okno", L"Don't keep on top")
                       : Tr(L"Przypnij okno na wierzchu", L"Keep on top"),
               g_onTop ? t.accent : t.text)
        .click = [] { SetOnTop(!g_onTop); };
    if (withMenu) {
        x -= 40;
        D2D1_RECT_F menu{x, 4, x + 40, 36};
        Hit h = HIT(0);
        IconButton(menu, h, kIconMore, Tr(L"Więcej", L"More")).click = [menu, h] {
            OpenMainMenu(menu, h);
        };
    }
    return x;
}

void DrawSearchOrTitle(float left, float right, const wchar_t* title) {
    const Theme& t = g_theme;
    if (g_searchOpen) {
        D2D1_RECT_F box{left, 5, right - 4, 35};
        PlaceField(FSearch, box,
                   g_page == Page::Trash ? Tr(L"Szukaj w koszu…", L"Search the trash…")
                   : g_tab == Tab::Pinned
                       ? Tr(L"Szukaj w przypiętych, #tag…", L"Search pinned, #tag…")
                   : g_tab == Tab::Templates
                       ? Tr(L"Szukaj w szablonach…", L"Search templates…")
                       : Tr(L"Szukaj w historii, #tag…", L"Search the history, #tag…"),
                   28, 28);
        Glyph(kIconSearch, g_fonts.icon14, {box.left + 8, box.top, box.left + 28, box.bottom},
              t.text2);
        D2D1_RECT_F clear{box.right - 28, box.top + 4, box.right - 4, box.bottom - 4};
        IconButton(clear, HIT(0), kIconCancel, Tr(L"Zamknij wyszukiwanie", L"Close the search"),
                   t.text2, g_fonts.icon12)
            .click = [] { CloseSearch(); };
        return;
    }
    Text(title, g_fonts.title, {left, 0, right - 44, kTitleHeight}, t.text);
    IconButton({right - 40, 4, right, 36}, HIT(0), kIconSearch,
               (Tr(L"Szukaj (", L"Search (") + KeyName(g_keys[ScSearch]) + L")").c_str())
        .click = [] { OpenSearch(); };
}

void DrawMainTitle(float width) {
    const Theme& t = g_theme;
    float right = DrawTitleButtons(width, true);
    Glyph(kIconApp, g_fonts.icon16, {12, 12, 28, 28}, t.text);
    DrawSearchOrTitle(g_searchOpen ? 40.f : 36.f, right, Tr(L"Schowek", L"Clipboard"));
}

void DrawPageTitle(float width, const wchar_t* title, bool search) {
    const Theme& t = g_theme;
    float right = DrawTitleButtons(width, false);
    IconButton({4, 4, 44, 36}, HIT(0), kIconBack, Tr(L"Wróć", L"Back")).click = [] {
        ShowPage(Page::Main);
    };
    if (search) {
        DrawSearchOrTitle(50, right, title);
    } else {
        Text(title, g_fonts.title, {52, 0, right, kTitleHeight}, t.text);
    }
}

void DrawTabs(float width) {
    const Theme& t = g_theme;
    const std::pair<Tab, const wchar_t*> kTabs[] = {
        {Tab::History, Tr(L"Historia", L"History")},
        {Tab::Pinned, Tr(L"Przypięte", L"Pinned")},
        {Tab::Templates, Tr(L"Szablony", L"Templates")}};
    float x = 12;
    for (const auto& [tab, label] : kTabs) {
        float w = TextWidth(label, g_fonts.tab) + 24;
        D2D1_RECT_F r{x, kTitleHeight, x + w, kTitleHeight + kTabsHeight};
        Hit h = HIT((int)tab);
        bool selected = g_tab == tab;
        D2D1_RECT_F inner = Inset(r, 0, 4);
        Fill(inner, SubtleFill(h), 4);
        Text(label, g_fonts.tab, r, selected ? t.text : t.text2);
        if (selected) {
            float cx = (r.left + r.right) / 2;
            Fill({cx - 8, r.bottom - 7, cx + 8, r.bottom - 4}, t.accent, 1.5f);
        }
        Tab target = tab;
        Add(r, h).click = [target] { SwitchTab(target); };
        x += w + 4;
        if (x > width) {
            break;
        }
    }
}

void DrawShortcutsPopup(const D2D1_RECT_F& footer) {
    const Theme& t = g_theme;
    static const Shortcut kShown[] = {ScOpen,   ScPaste, ScPastePlain, ScDetails, ScSearch,
                                      ScPin,    ScNth,   ScDelete,     ScEdit,    ScNextTab,
                                      ScClose};
    std::vector<std::pair<std::wstring, const wchar_t*>> rows;
    float keyWidth = 0;
    for (Shortcut s : kShown) {
        if (g_keys[s] & kKeyOff) {
            continue;
        }
        rows.push_back({ShortcutName(s), kShortcuts[s].label});
        keyWidth = std::max(keyWidth, TextWidth(rows.back().first, g_fonts.key) + 12);
    }
    float h = 12 + 20 + 8 + rows.size() * 26 + 10;
    float w = std::min(keyWidth + 300, SizeDip(g_main).width - 24);
    D2D1_RECT_F r{12, footer.top - 6 - h, 12 + w, footer.top - 6};
    Fill(r, t.flyout, 8);
    Stroke(r, t.controlStroke, 8);
    Text(Tr(L"Skróty klawiszowe", L"Keyboard shortcuts"), g_fonts.smallBold,
         {r.left + 14, r.top + 12, r.right, r.top + 32}, t.text);
    float y = r.top + 40;
    for (const auto& [key, label] : rows) {
        Kbd(r.left + 14, y + 12, key);
        Text(label, g_fonts.small, {r.left + 26 + keyWidth, y, r.right - 12, y + 24}, t.text2);
        y += 26;
    }
}

void DrawFooter(const D2D1_RECT_F& footer) {
    const Theme& t = g_theme;
    Fill({footer.left, footer.top, footer.right, footer.top + 1}, t.divider);
    float cy = (footer.top + footer.bottom) / 2;
    IconButton({footer.left + 4, cy - 16, footer.left + 36, cy + 16}, HIT(0), kIconSettings,
               Tr(L"Ustawienia", L"Settings"))
        .click = [] { ShowPage(Page::Settings); };
    Hit info = HIT(0);
    IconButton({footer.left + 38, cy - 16, footer.left + 70, cy + 16}, info, kIconInfo, nullptr);
    float clearLeft = footer.right;
    if (g_tab == Tab::History && CountItems(Kind::History)) {
        float w = ButtonWidth(Tr(L"Wyczyść historię", L"Clear history"), kIconDelete);
        D2D1_RECT_F clear{footer.right - 6 - w, cy - 15, footer.right - 6, cy + 15};
        Button(clear, HIT(0), Tr(L"Wyczyść historię", L"Clear history"), kIconDelete).click = [] {
            ClearHistory();
        };
        clearLeft = clear.left;
    }
    std::wstring count;
    size_t history = CountItems(Kind::History), pinned = CountItems(Kind::Pinned),
           templates = CountItems(Kind::Template);
    if (!g_query.empty() || (g_tab == Tab::Pinned && !g_tagFilter.empty())) {
        count = Tr(L"Wyniki: ", L"Results: ") + std::to_wstring(EntryCount());
    } else if (g_tab == Tab::History) {
        count = Count(history, Tr(L"element", L"item"), Tr(L"elementy", L"items"),
                      Tr(L"elementów", L"items")) +
                L" · " +
                Count(pinned, Tr(L"przypięty", L"pinned"), Tr(L"przypięte", L"pinned"),
                      Tr(L"przypiętych", L"pinned"));
    } else if (g_tab == Tab::Pinned) {
        count = Count(pinned, Tr(L"przypięty", L"pinned"), Tr(L"przypięte", L"pinned"),
                      Tr(L"przypiętych", L"pinned")) +
                Tr(L" · zapisane na stałe, szyfrowane", L" · saved permanently, encrypted");
    } else {
        count = Count(templates, Tr(L"szablon", L"template"), Tr(L"szablony", L"templates"),
                      Tr(L"szablonów", L"templates")) +
                Tr(L" · zapisane na dysku", L" · saved on disk");
    }
    if (g_paused) {
        count += Tr(L" · zapisywanie wstrzymane", L" · saving paused");
    }
    Text(count, g_fonts.small, {footer.left + 78, footer.top, clearLeft - 8, footer.bottom}, t.text2);
    if (Hot(info)) {
        DrawShortcutsPopup(footer);
    }
}

void DrawMainPage(D2D1_SIZE_F s) {
    const Theme& t = g_theme;
    bool side = g_detailsId && SideDetails();
    float listWidth = side ? s.width - (kPanelWidth + 8) : s.width;
    DrawMainTitle(s.width);
    DrawTabs(listWidth);
    D2D1_RECT_F card{8, kTitleHeight + kTabsHeight, listWidth - (side ? 4 : 8), s.height - 8};
    Fill(card, t.layer, 8);
    Stroke(card, t.layerStroke, 8);
    D2D1_RECT_F footer{card.left, card.bottom - kFooterHeight, card.right, card.bottom};
    D2D1_RECT_F area{card.left + 4, card.top + 4, card.right - 4, footer.top};
    g_listArea = area;
    if (g_detailsId && !side) {
        DrawDetails({card.left, card.top, card.right, footer.top}, true);
    } else if (g_tab == Tab::History) {
        DrawHistory(area);
    } else if (g_tab == Tab::Pinned) {
        DrawPinned(area);
    } else {
        DrawTemplates(area);
    }
    DrawToast(area);
    DrawFooter(footer);
    if (side) {
        D2D1_RECT_F panel{listWidth + 4, kTitleHeight, s.width - 8, s.height - 8};
        Fill(panel, t.layer, 8);
        Stroke(panel, t.layerStroke, 8);
        DrawDetails(panel, false);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Trash

void DrawTrashEntry(const Item& it, const D2D1_RECT_F& r) {
    const Theme& t = g_theme;
    Hit h = HIT(it.id);
    DrawEntryBackground(r, h, it.id);
    UINT64 id = it.id;
    Region& region = Add(r, h);
    region.click = [id] {
        g_selectedId = id;
        Redraw();
    };
    region.rclick = [id] {
        g_selectedId = id;
        D2D1_POINT_2F p = g_s->mouse;
        MenuItem restore;
        restore.label = Tr(L"Przywróć", L"Restore");
        restore.glyph = kIconRestore;
        restore.action = [id] { RestoreItem(id, true); };
        MenuItem purge;
        purge.label = Tr(L"Usuń na stałe", L"Delete permanently");
        purge.glyph = kIconDelete;
        purge.danger = true;
        purge.action = [id] { PurgeItem(id); };
        OpenMenu({p.x, p.y, p.x, p.y}, HIT(id), {restore, purge});
    };
    DrawTile(it, {r.left + 10, r.top + 12, r.left + 42, r.top + 44});
    float right = MouseOver(r) ? HoverButtons(r, it, true) - 4 : r.right - 10;
    Text(ItemTitle(it), g_fonts.row, {r.left + 54, r.top + 8, right, r.top + 30}, t.text);
    UINT64 now = NowFileTime();
    UINT64 end = it.deletedTime + kTrashDays * kDay;
    UINT64 daysLeft = end > now ? (end - now + kDay - 1) / kDay : 0;
    std::wstring where = it.kind == Kind::Pinned     ? Tr(L"Przypięte", L"Pinned")
                         : it.kind == Kind::Template ? Tr(L"Szablony", L"Templates")
                                                     : Tr(L"Historia", L"History");
    std::wstring left = daysLeft <= 1 ? Tr(L"zniknie w ciągu doby", L"goes away within a day")
                        : g_english   ? Count(daysLeft, L"day", L"days", L"days") + L" left"
                                      : L"zostało " + Count(daysLeft, L"dzień", L"dni", L"dni");
    Text(where + Tr(L" · usunięto ", L" · deleted ") + FormatWhen(it.deletedTime) + L" · " + left,
         g_fonts.small, {r.left + 54, r.top + 30, right, r.top + 48}, t.text2);
}

void DrawTrashPage(D2D1_SIZE_F s) {
    const Theme& t = g_theme;
    DrawPageTitle(s.width, Tr(L"Kosz", L"Trash"), true);
    D2D1_RECT_F card{8, kTitleHeight, s.width - 8, s.height - 8};
    Fill(card, t.layer, 8);
    Stroke(card, t.layerStroke, 8);
    size_t count = CountTrash();
    D2D1_RECT_F head{card.left + 12, card.top + 6, card.right - 12, card.top + 50};
    float bw = 0;
    if (count) {
        bw = ButtonWidth(Tr(L"Opróżnij kosz", L"Empty trash"), kIconDelete);
        D2D1_RECT_F b{head.right - bw, head.top + 6, head.right, head.bottom - 6};
        Hit h = HIT(0);
        Button(b, h, Tr(L"Opróżnij kosz", L"Empty trash"), kIconDelete, kDanger).click = [b, h,
                                                                                          count] {
            if (!ToggleFlyout(h)) {
                OpenConfirm(b, h,
                            Tr(L"Usunąć na stałe ", L"Permanently delete ") +
                                Count(count, Tr(L"element", L"item"), Tr(L"elementy", L"items"),
                                      Tr(L"elementów", L"items")) +
                                Tr(L" z kosza? Tego nie da się cofnąć.",
                                   L" from the trash? This can't be undone."),
                            Tr(L"Opróżnij", L"Empty"), [] { EmptyTrash(); });
            }
        };
    }
    Text(count ? Count(count, Tr(L"element", L"item"), Tr(L"elementy", L"items"),
                       Tr(L"elementów", L"items")) +
                     Tr(L" · każdy znika na stałe 30 dni po usunięciu",
                        L" · each is gone for good 30 days after deletion")
               : Tr(L"Usunięte elementy czekają tu 30 dni, potem znikają na stałe.",
                    L"Deleted items wait here for 30 days, then they're gone for good."),
         g_fonts.small, {head.left, head.top, head.right - bw - 12, head.bottom}, t.text2);
    Fill({card.left + 8, head.bottom, card.right - 8, head.bottom + 1}, t.divider);
    D2D1_RECT_F area{card.left + 4, head.bottom + 4, card.right - 4, card.bottom - 4};
    g_listArea = area;
    if (g_rows.empty()) {
        EmptyMessage(
            count ? Tr(L"Brak wyników. Spróbuj innych słów.", L"No results. Try other words.")
                  : Tr(L"Kosz jest pusty.", L"The trash is empty."),
            area);
    } else {
        DrawList(area, [](const Row& row, D2D1_RECT_F r) {
            if (const Item* it = FindItem(row.id)) {
                DrawTrashEntry(*it, r);
            }
        });
    }
    DrawToast(area);
}

////////////////////////////////////////////////////////////////////////////////
// Settings

D2D1_RECT_F SettingCard(float x, float& y, float right, wchar_t glyph, const std::wstring& title,
                        const std::wstring& description, float controlWidth) {
    const Theme& t = g_theme;
    D2D1_RECT_F r{x, y, right, y + 64};
    Fill(r, t.card, 6);
    Stroke(r, t.layerStroke, 6);
    Glyph(glyph, g_fonts.icon16, {x + 16, y, x + 36, y + 64}, t.text);
    float textRight = right - 16 - controlWidth - 12;
    Text(title, g_fonts.row, {x + 52, y + 12, textRight, y + 32}, t.text);
    Text(description, g_fonts.small, {x + 52, y + 32, textRight, y + 50}, t.text2);
    y += 68;
    return {right - 16 - controlWidth, r.top + 16, right - 16, r.bottom - 16};
}

float SettingSection(const wchar_t* title, float x, float y, float right) {
    Text(title, g_fonts.subtitle, {x + 2, y + 10, right, y + 34}, g_theme.text);
    return y + 40;
}

void ShortcutMenu(const D2D1_RECT_F& anchor, Hit owner, int i) {
    if (ToggleFlyout(owner)) {
        return;
    }
    std::vector<MenuItem> items;
    MenuItem change;
    change.label = Tr(L"Zmień skrót…", L"Change shortcut…");
    change.glyph = kIconKeyboard;
    change.action = [i] {
        StartRecording(i, [i](UINT key) {
            if (i == ScNth && (key & 0xFFFF) >= '1' && (key & 0xFFFF) <= '9') {
                key = (key & 0xFFFF0000) | '1';  // One modifier for the whole 1…9 row.
            }
            SetShortcut(i, key);
        });
    };
    items.push_back(change);
    MenuItem reset;
    reset.label =
        Tr(L"Przywróć domyślny (", L"Restore default (") + KeyName(kShortcuts[i].def) + L")";
    reset.glyph = kIconRestore;
    reset.disabled = g_keys[i] == kShortcuts[i].def;
    reset.action = [i] { SetShortcut(i, kShortcuts[i].def); };
    items.push_back(reset);
    MenuItem off;
    off.label = Tr(L"Wyłącz", L"Turn off");
    off.glyph = kIconCancel;
    off.disabled = (g_keys[i] & kKeyOff) != 0;
    off.action = [i] { SetShortcut(i, kKeyOff); };
    items.push_back(off);
    OpenMenu(anchor, owner, std::move(items), 260);
}

float DrawShortcutRows(float x, float y, float right, ShortcutScope a, ShortcutScope b,
                       const wchar_t* title) {
    const Theme& t = g_theme;
    std::vector<int> list;
    for (int i = 0; i < ScCount; i++) {
        if (kShortcuts[i].scope == a || kShortcuts[i].scope == b) {
            list.push_back(i);
        }
    }
    float height = 36;
    std::vector<std::wstring> conflicts;
    for (int i : list) {
        conflicts.push_back(ShortcutConflict(g_keys[i], i));
        height += conflicts.back().empty() ? 40 : 56;
    }
    D2D1_RECT_F card{x, y, right, y + height};
    Fill(card, t.card, 6);
    Stroke(card, t.layerStroke, 6);
    Text(title, g_fonts.smallBold, {x + 16, y + 8, right, y + 28}, t.text2);
    float ry = y + 36;
    for (size_t k = 0; k < list.size(); k++) {
        int i = list[k];
        float h = conflicts[k].empty() ? 40 : 56;
        D2D1_RECT_F row{x + 4, ry, right - 4, ry + h};
        Fill({x + 16, ry, right - 16, ry + 1}, t.divider);
        Hit h1 = HIT(i);
        if (MouseOver(row)) {
            Fill(Inset(row, 0, 2), SubtleFill(h1), 4);
        }
        Text(std::wstring(kShortcuts[i].label), g_fonts.row, {x + 16, ry, right - 220, ry + 40},
             t.text);
        if (!conflicts[k].empty()) {
            Glyph(kIconWarning, g_fonts.icon12, {x + 16, ry + 34, x + 30, ry + 50}, t.warning);
            Text(Tr(L"Ten sam skrót ma: ", L"Also used by: ") + conflicts[k], g_fonts.small,
                 {x + 36, ry + 32, right - 16, ry + 52}, t.warning);
        }
        D2D1_RECT_F more{right - 44, ry + 4, right - 12, ry + 36};
        Hit moreHit = HIT(i);
        float kx = more.left - 8;
        float cy = ry + 20;
        if (g_recordTarget == i) {
            std::wstring s =
                Tr(L"Naciśnij skrót… (Esc anuluje)", L"Press a shortcut… (Esc cancels)");
            float w = TextWidth(s, g_fonts.key) + 12;
            Kbd(kx - w, cy, s, true);
        } else if (g_keys[i] & kKeyOff) {
            Text(Tr(L"Wyłączony", L"Off"), g_fonts.meta, {kx - 120, ry, kx, ry + 40}, t.text3);
        } else {
            std::wstring s = ShortcutName(i);
            float w = TextWidth(s, g_fonts.key) + 12;
            Kbd(kx - w, cy, s);
        }
        Region& region = Add(row, h1);
        region.click = [i] {
            StartRecording(i, [i](UINT key) {
                if (i == ScNth && (key & 0xFFFF) >= '1' && (key & 0xFFFF) <= '9') {
                    key = (key & 0xFFFF0000) | '1';
                }
                SetShortcut(i, key);
            });
        };
        region.tip = Tr(L"Kliknij i naciśnij nowy skrót", L"Click and press a new shortcut");
        IconButton(more, moreHit, kIconMore, Tr(L"Więcej opcji", L"More options")).click =
            [more, moreHit, i] { ShortcutMenu(more, moreHit, i); };
        ry += h;
    }
    return card.bottom + 8;
}

void DrawSettingsPage(D2D1_SIZE_F s) {
    const Theme& t = g_theme;
    DrawPageTitle(s.width, Tr(L"Ustawienia", L"Settings"), false);
    D2D1_RECT_F view{0, kTitleHeight, s.width, s.height};
    float top = BeginScroll(g_pageScroll, view);
    float x = 16, right = s.width - 16;
    float y = top;
    Text(Tr(L"Każda zmiana zapisuje się od razu, na stałe",
            L"Every change is saved right away, permanently"),
         g_fonts.small, {x + 2, y, right, y + 20}, t.text2);
    y += 20;
    float combo = std::min(220.f, (right - x) * 0.42f);

    y = SettingSection(Tr(L"Wygląd", L"Appearance"), x, y, right);
    {
        D2D1_RECT_F c = SettingCard(x, y, right, kIconLanguage, Tr(L"Język", L"Language"),
                                    Tr(L"Polski w polskim Windows, w innym angielski",
                                       L"Polish on a Polish Windows, else English"),
                                    combo);
        Combo(c, HIT(0), {Tr(L"Jak w Windows", L"Same as Windows"), L"Polski", L"English"},
              (int)Setting(L"language", 0), [](int v) {
                  SetSetting(L"language", v);
                  ApplyLook();
              });
    }
    {
        D2D1_RECT_F c = SettingCard(x, y, right, kIconTheme, Tr(L"Motyw", L"Theme"),
                                    Tr(L"Jasny lub ciemny i kolor akcentu jak w Windows",
                                       L"Light or dark, with the accent color of Windows"),
                                    combo);
        Combo(c, HIT(0),
              {Tr(L"Zgodny z systemem", L"Same as Windows"), Tr(L"Jasny", L"Light"),
               Tr(L"Ciemny", L"Dark")},
              (int)Setting(L"theme", 0), [](int v) {
                  SetSetting(L"theme", v);
                  ApplyLook();
              });
    }
    {
        INT64 value = std::clamp<INT64>(Setting(L"transparency", 30), 0, 90);
        D2D1_RECT_F c = SettingCard(
            x, y, right, kIconTransparency, Tr(L"Przezroczystość tła", L"Background transparency"),
            Tr(L"Tekst i ikony zostają wyraźne", L"Text and icons stay sharp"), combo);
        std::wstring label = std::to_wstring(value) + L" %";
        Text(label, g_fonts.meta, {c.right - 44, c.top, c.right, c.bottom}, t.text2);
        Slider({c.left, c.top, c.right - 50, c.bottom}, HIT(0), value / 90.f, [](float v) {
            g_settings[L"transparency"] = std::lround(v * 90);
            SaveSettings();
            Redraw();
        });
    }
    {
        D2D1_RECT_F c = SettingCard(
            x, y, right, kIconPane, Tr(L"Szczegóły elementu", L"Item details"),
            Tr(L"Panel z boku albo w miejscu listy", L"A side panel or in place of the list"),
            combo);
        Combo(c, HIT(0),
              {Tr(L"Panel z boku", L"Side panel"), Tr(L"W miejscu listy", L"In place of the list")},
              (int)Setting(L"detailsMode", 0), [](int v) {
                  CloseDetails();
                  SetSetting(L"detailsMode", v);
              });
    }
    {
        D2D1_RECT_F c =
            SettingCard(x, y, right, kIconLayout, Tr(L"Układ przypiętych", L"Pinned layout"),
                        Tr(L"Kategorie z boku albo zwijane sekcje",
                           L"Categories at the side or collapsible sections"),
                        combo);
        Combo(c, HIT(0),
              {Tr(L"Kategorie z boku", L"Categories at the side"),
               Tr(L"Zwijane sekcje", L"Collapsible sections")},
              (int)Setting(L"pinnedLayout", 0), [](int v) {
                  SetSetting(L"pinnedLayout", v);
                  Refresh();
              });
    }

    y = SettingSection(Tr(L"Historia", L"History"), x, y, right);
    {
        D2D1_RECT_F c = SettingCard(
            x, y, right, kIconClock, Tr(L"Automatyczne czyszczenie", L"Automatic cleanup"),
            Tr(L"Tylko zwykła historia, nigdy przypięte i szablony",
               L"Only the regular history, never pinned items or templates"),
            combo);
        Combo(c, HIT(0),
              {Tr(L"Nigdy", L"Never"), Tr(L"Przy każdym restarcie", L"On every restart"),
               Tr(L"Codziennie", L"Daily"), Tr(L"Co tydzień", L"Weekly"),
               Tr(L"Co miesiąc", L"Monthly")},
              (int)Setting(L"autoClean", 0), [](int v) {
                  if (v == 1) {
                      g_settings[L"cleanBoot"] = BootTime();
                  }
                  SetSetting(L"autoClean", v);
                  AutoClean();
              });
    }
    {
        bool on = Setting(L"fullRes", 1) != 0;
        D2D1_RECT_F c =
            SettingCard(x, y, right, kIconImage,
                        Tr(L"Obrazy w pełnej rozdzielczości", L"Full resolution images"),
                        on ? Tr(L"Bez pomniejszania i kompresji stratnej",
                                L"No downscaling or lossy compression")
                           : Tr(L"Nowe obrazy pomniejszam do 1920 px dłuższego boku",
                                L"New images are scaled down to 1920 px on the longer side"),
                        80);
        Text(on ? Tr(L"Wł.", L"On") : Tr(L"Wył.", L"Off"), g_fonts.meta,
             {c.left, c.top, c.right - 50, c.bottom}, t.text);
        Toggle(c.right - 40, (c.top + c.bottom) / 2, HIT(0), on).click = [on] {
            SetSetting(L"fullRes", !on);
            Redraw();
        };
    }

    y = SettingSection(Tr(L"Okno", L"Window"), x, y, right);
    {
        WCHAR now[64];
        swprintf_s(now,
                   Tr(L"Teraz %ld × %ld, krawędzią lub rogiem",
                      L"Now %ld × %ld, resize it from an edge or corner"),
                   std::lround(g_sizeDip.width), std::lround(g_sizeDip.height));
        D2D1_RECT_F c =
            SettingCard(x, y, right, kIconSize, Tr(L"Rozmiar okna", L"Window size"), now, combo);
        Combo(c, HIT(0),
              {Tr(L"Do restartu komputera", L"Until restart"), Tr(L"Na stałe", L"Permanently"),
               Tr(L"Zawsze domyślny", L"Always default")},
              (int)Setting(L"sizeMode", 0), [](int v) {
                  SetSetting(L"sizeMode", v);
                  g_settings[L"boot"] = BootTime();
                  SaveSettings();
              });
    }
    {
        D2D1_RECT_F c =
            SettingCard(x, y, right, kIconPosition, Tr(L"Pozycja okna", L"Window position"),
                        Tr(L"Miejsce otwarcia po ", L"Where it opens with ") +
                            KeyName(g_keys[ScOpen]) + Tr(L" i z zasobnika", L" and from the tray"),
                        combo);
        Combo(c, HIT(0),
              {Tr(L"Przy kursorze tekstowym", L"At the text cursor"),
               Tr(L"Przy myszy", L"At the mouse"),
               Tr(L"Zapamiętana do restartu", L"Remembered until restart"),
               Tr(L"Zapamiętana na stałe", L"Remembered permanently")},
              (int)Setting(L"posMode", 0), [](int v) {
                  SetSetting(L"posMode", v);
                  if (v >= 2) {
                      RECT rc;
                      GetWindowRect(g_hwnd, &rc);
                      g_settings[L"posX"] = rc.left;
                      g_settings[L"posY"] = rc.top;
                      g_settings[L"posBoot"] = BootTime();
                      SaveSettings();
                  }
              });
    }

    y = SettingSection(Tr(L"Skróty klawiszowe", L"Keyboard shortcuts"), x, y, right);
    y = DrawShortcutRows(x, y, right, kScopeGlobal, kScopeWindow,
                         Tr(L"Okno schowka", L"Clipboard window"));
    y = DrawShortcutRows(x, y, right, kScopeEditor, kScopeEditor,
                         Tr(L"Edytor grafik", L"Image editor"));
    Text(Tr(L"Skróty szablonów ustawisz w zakładce Szablony.",
            L"Template shortcuts are set in the Templates tab."),
         g_fonts.small, {x + 2, y, right, y + 20}, t.text2);
    y += 24;

    y = SettingSection(Tr(L"Przechowywanie", L"Storage"), x, y, right);
    {
        std::wstring used = g_storageBytes ? FormatSize(g_storageBytes) : L"…";
        D2D1_RECT_F c = SettingCard(
            x, y, right, kIconLock, Tr(L"Zapis na tym komputerze", L"Stored on this computer"),
            Tr(L"Szyfrowany kluczem konta · zajęte ", L"Encrypted with your account key · uses ") +
                used,
            90);
        Text(Tr(L"Bez limitu", L"No limit"), g_fonts.meta, c, t.text2);
    }
    {
        float w1 = ButtonWidth(Tr(L"Eksportuj…", L"Export…")),
              w2 = ButtonWidth(Tr(L"Importuj…", L"Import…"));
        D2D1_RECT_F c = SettingCard(x, y, right, kIconMove,
                                    Tr(L"Przenieś na inny komputer", L"Move to another computer"),
                                    Tr(L"Przypięte, kategorie, tagi, szablony, ustawienia",
                                       L"Pinned items, categories, tags, templates, settings"),
                                    w1 + w2 + 8);
        Button({c.left, c.top - 2, c.left + w1, c.bottom + 2}, HIT(0),
               Tr(L"Eksportuj…", L"Export…"))
            .click = [] { ShowExportDialog(); };
        Button({c.right - w2, c.top - 2, c.right, c.bottom + 2}, HIT(0),
               Tr(L"Importuj…", L"Import…"))
            .click = [] { ShowImportDialog(); };
    }
    y += 8;
    EndScroll(g_pageScroll, view, y - top);
    DrawToast({8, kTitleHeight, s.width - 8, s.height});
}

////////////////////////////////////////////////////////////////////////////////
// Dialogs

float Label(const wchar_t* text, float x, float y, float right) {
    Text(text, g_fonts.small, {x, y, right, y + 18}, g_theme.text2);
    return y + 22;
}

struct CategoryDraft {
    UINT64 id = 0;
    std::wstring emoji;
    BYTE color = 0;
    std::function<void(UINT64)> created;
} g_categoryDraft;

void SaveCategoryDraft() {
    std::wstring name = Trim(FieldText(FCategoryName));
    if (name.empty()) {
        return;
    }
    CategoryDraft draft = g_categoryDraft;
    CloseDialog();
    if (Category* c = FindCategory(draft.id)) {
        c->name = name;
        c->emoji = draft.emoji;
        c->color = draft.color;
        SaveIndex();
        Refresh();
        return;
    }
    UINT64 id = AddCategory(name, draft.emoji, draft.color);
    if (draft.created) {
        draft.created(id);
    }
}

void ShowCategoryDialog(UINT64 id, std::function<void(UINT64)> created) {
    const Category* existing = FindCategory(id);
    g_categoryDraft = {};
    g_categoryDraft.id = existing ? id : 0;
    g_categoryDraft.created = std::move(created);
    if (existing) {
        g_categoryDraft.emoji = existing->emoji;
        g_categoryDraft.color = existing->color;
    }
    SetFieldText(FCategoryName, existing ? existing->name : L"");
    g_fields[FCategoryName].onKey = nullptr;
    ShowDialog(
        existing ? Tr(L"Edytuj kategorię", L"Edit category")
                 : Tr(L"Nowa kategoria", L"New category"),
        440,
        [](D2D1_RECT_F b) {
            const Theme& t = g_theme;
            CategoryDraft& d = g_categoryDraft;
            float x = b.left, right = b.right, y = b.top;
            y = Label(Tr(L"Nazwa", L"Name"), x, y, right);
            PlaceField(FCategoryName, {x, y, right, y + 32},
                       Tr(L"Np. Praca, Dom, Faktury", L"E.g. Work, Home, Invoices"));
            y += 44;
            y = Label(Tr(L"Emoji (opcjonalnie)", L"Emoji (optional)"), x, y, right);
            int columns = 11;
            float cell = (right - x) / columns;
            for (int i = -1; i < (int)ARRAYSIZE(kEmojis); i++) {
                int k = i + 1;
                float cx = x + (k % columns) * cell, cy = y + (k / columns) * cell;
                D2D1_RECT_F r{cx, cy, cx + cell, cy + cell};
                Hit h = HIT(i);
                bool selected = i < 0 ? d.emoji.empty() : d.emoji == kEmojis[i];
                if (selected) {
                    Fill(Inset(r, 2, 2), t.hover, 4);
                    Stroke(Inset(r, 2, 2), t.accent, 4, 2);
                } else {
                    Fill(Inset(r, 2, 2), SubtleFill(h), 4);
                }
                if (i < 0) {
                    Glyph(kIconCancel, g_fonts.icon12, r, t.text2);
                } else {
                    Emoji(kEmojis[i], g_fonts.emoji16, r);
                }
                Region& region = Add(r, h);
                region.click = [i] {
                    g_categoryDraft.emoji = i < 0 ? L"" : kEmojis[i];
                    Redraw();
                };
                if (i < 0) {
                    region.tip = Tr(L"Bez emoji: ikona folderu", L"No emoji: folder icon");
                }
            }
            y += ((ARRAYSIZE(kEmojis) + 1 + columns - 1) / columns) * cell + 10;
            y = Label(Tr(L"Kolor (opcjonalnie)", L"Color (optional)"), x, y, right);
            float cx = x + 14;
            for (BYTE c = 0; c < kColorCount; c++) {
                Hit h = HIT(c);
                float cy = y + 14;
                if (d.color == c) {
                    CircleStroke(cx, cy, 14, t.text, 2);
                } else if (Hot(h)) {
                    CircleStroke(cx, cy, 14, t.strong);
                }
                if (c == 0) {
                    CircleStroke(cx, cy, 10, t.strong);
                    Line(cx - 7, cy + 7, cx + 7, cy - 7, t.strong);
                } else {
                    Circle(cx, cy, 10, CategoryColor(c));
                }
                Region& region = Add({cx - 15, y, cx + 15, y + 28}, h);
                region.tip = kCategoryColors[c].name;
                region.click = [c] {
                    g_categoryDraft.color = c;
                    Redraw();
                };
                cx += 38;
            }
            y += 40;
            y = Label(Tr(L"Podgląd", L"Preview"), x, y, right);
            D2D1_RECT_F preview{x, y, right, y + 40};
            Fill(preview, t.control, 6);
            Stroke(preview, t.controlStroke, 6);
            D2D1_RECT_F icon{x + 12, y + 12, x + 28, y + 28};
            if (!d.emoji.empty()) {
                Emoji(d.emoji, g_fonts.emoji16, Inset(icon, -4, -4));
            } else {
                Glyph(kIconFolder, g_fonts.icon16, icon,
                      d.color ? CategoryColor(d.color) : t.text2);
            }
            std::wstring name = Trim(FieldText(FCategoryName));
            float nameWidth =
                std::min(TextWidth(name.empty() ? Tr(L"Nazwa", L"Name") : name, g_fonts.row),
                         right - x - 80);
            Text(name.empty() ? Tr(L"Nazwa", L"Name") : name, g_fonts.row,
                 {x + 38, y, x + 38 + nameWidth, y + 40}, name.empty() ? t.text3 : t.text);
            if (d.color && !d.emoji.empty()) {
                Circle(x + 46 + nameWidth, y + 20, 3.5f, CategoryColor(d.color));
            }
            y += 56;
            if (d.id) {
                float dw = ButtonWidth(Tr(L"Usuń kategorię", L"Delete category"), kIconDelete);
                D2D1_RECT_F del{x, y, x + dw, y + 32};
                Hit h = HIT(0);
                UINT64 id = d.id;
                Button(del, h, Tr(L"Usuń kategorię", L"Delete category"), kIconDelete, kDanger)
                    .click = [del, h, id] {
                    const Category* c = FindCategory(id);
                    if (c && !ToggleFlyout(h)) {
                        OpenConfirm(
                            del, h,
                            Tr(L"Usunąć kategorię „", L"Delete the category “") + c->name +
                                Tr(L"”? Jej elementy zostaną w przypiętych, w „Bez kategorii”.",
                                   L"”? Its items stay pinned, in “Uncategorized”."),
                            Tr(L"Usuń", L"Delete"), [id] {
                                CloseDialog();
                                DeleteCategory(id);
                            });
                    }
                };
                float bw = (right - del.right - 16) / 2;
                Button({del.right + 8, y, del.right + 8 + bw, y + 32}, HIT(0),
                       Tr(L"Zapisz", L"Save"), 0, kAccent, name.empty())
                    .click = SaveCategoryDraft;
                Button({right - bw, y, right, y + 32}, HIT(0), Tr(L"Anuluj", L"Cancel")).click =
                    [] { CloseDialog(); };
                return y + 32;
            }
            return DialogButtons(b, y, Tr(L"Zapisz", L"Save"), !name.empty(), SaveCategoryDraft);
        },
        SaveCategoryDraft);
    g_fields[FCategoryName].onChange = [] { Redraw(); };
    UpdateWindow(g_hwnd);
    FocusField(FCategoryName);
}

struct TemplateDraft {
    UINT64 id = 0;
    std::wstring emoji;
    UINT hotkey = 0;
    std::wstring warning;
} g_templateDraft;

void SaveTemplateDraft() {
    std::wstring name = Trim(FieldText(FTemplateName));
    std::wstring body = FieldText(FTemplateBody);
    if (name.empty() || body.empty()) {
        return;
    }
    TemplateDraft d = g_templateDraft;
    if (g_recordTarget == kRecordTemplate) {
        StopRecording();
    }
    CloseDialog();
    g_selectedId = SaveTemplate(d.id, name, body, d.emoji, d.hotkey);
    if (g_tab != Tab::Templates) {
        SwitchTab(Tab::Templates);
    }
    Refresh();
    if (g_hotkeyFailed.contains(g_selectedId)) {
        ShowToast(Tr(L"Zapisano szablon, ale skrót ", L"Saved the template, but the shortcut ") +
                  KeyName(d.hotkey) +
                  Tr(L" zajął już inny program. Wybierz inny.",
                     L" is already used by another app. Choose another one."));
    }
}

void InsertIntoBody(const wchar_t* field) {
    HWND hwnd = g_fields[FTemplateBody].hwnd;
    SetFocus(hwnd);
    SendMessageW(hwnd, EM_REPLACESEL, TRUE, (LPARAM)field);
}

void ShowTemplateDialog(UINT64 id) {
    const Item* existing = FindItem(id);
    if (existing && existing->kind != Kind::Template) {
        existing = nullptr;
    }
    g_templateDraft = {};
    g_templateDraft.id = existing ? id : 0;
    if (existing) {
        g_templateDraft.emoji = existing->emoji;
        g_templateDraft.hotkey = existing->hotkey;
    }
    SetFieldText(FTemplateName, existing ? existing->name : L"");
    std::wstring body;
    if (existing) {
        for (size_t i = 0; i < existing->text.size(); i++) {
            if (existing->text[i] == L'\n' && (i == 0 || existing->text[i - 1] != L'\r')) {
                body += L'\r';
            }
            body += existing->text[i];
        }
    }
    SetFieldText(FTemplateBody, body);
    g_fields[FTemplateName].onKey = nullptr;
    g_fields[FTemplateBody].onKey = [](UINT vk) {
        if (vk == VK_RETURN && GetKeyState(VK_CONTROL) < 0) {
            SaveTemplateDraft();
            return true;
        }
        return false;
    };
    g_fields[FTemplateName].onChange = g_fields[FTemplateBody].onChange = [] { Redraw(); };
    ShowDialog(
        existing ? Tr(L"Edytuj szablon", L"Edit template") : Tr(L"Nowy szablon", L"New template"),
        480,
        [](D2D1_RECT_F b) {
            const Theme& t = g_theme;
            TemplateDraft& d = g_templateDraft;
            float x = b.left, right = b.right, y = b.top;
            y = Label(Tr(L"Nazwa", L"Name"), x, y, right);
            D2D1_RECT_F emoji{x, y, x + 44, y + 32};
            Hit eh = HIT(0);
            Fill(emoji, t.control, 4);
            Fill(emoji, SubtleFill(eh), 4);
            Stroke(emoji, t.controlStroke, 4);
            if (d.emoji.empty()) {
                Glyph(kIconEmoji, g_fonts.icon16, emoji, t.text2);
            } else {
                Emoji(d.emoji, g_fonts.emoji20, emoji);
            }
            Region& er = Add(emoji, eh);
            er.tip = Tr(L"Emoji szablonu", L"Template emoji");
            er.click = [emoji, eh] {
                if (!ToggleFlyout(eh)) {
                    OpenEmojiPicker(emoji, eh, [](std::wstring e) {
                        g_templateDraft.emoji = e;
                        Redraw();
                    });
                }
            };
            PlaceField(FTemplateName, {x + 52, y, right, y + 32},
                       Tr(L"Np. Podpis e-mail", L"E.g. Email signature"));
            y += 44;
            y = Label(Tr(L"Treść", L"Content"), x, y, right);
            PlaceField(
                FTemplateBody, {x, y, right, y + 150},
                Tr(L"Tekst, który wkleisz jednym kliknięciem", L"Text you paste with one click"));
            y += 158;
            Text(Tr(L"Wstaw pole:", L"Insert field:"), g_fonts.small, {x, y, x + 80, y + 24},
                 t.text2);
            float cx = x + 80;
            for (int i = 0; i < 3; i++) {
                const wchar_t* field = kTemplateFields[i];
                float w = ChipWidth(field);
                Chip(cx, y, HIT(i), field, false, t.accentText).click = [field] {
                    InsertIntoBody(field);
                };
                cx += w + 6;
            }
            y += 36;
            y = Label(Tr(L"Skrót (opcjonalnie)", L"Shortcut (optional)"), x, y, right);
            float kx = x;
            if (g_recordTarget == kRecordTemplate) {
                kx += Kbd(kx, y + 16,
                          Tr(L"Naciśnij skrót… (Esc anuluje)", L"Press a shortcut… (Esc cancels)"),
                          true) +
                      8;
            } else if (d.hotkey && !(d.hotkey & kKeyOff)) {
                kx += Kbd(kx, y + 16, KeyName(d.hotkey)) + 8;
            } else {
                Text(Tr(L"Brak", L"None"), g_fonts.row, {kx, y, kx + 50, y + 32}, t.text3);
                kx += 50;
            }
            float sw = ButtonWidth(Tr(L"Ustaw skrót…", L"Set shortcut…"));
            Button({kx, y, kx + sw, y + 32}, HIT(0), Tr(L"Ustaw skrót…", L"Set shortcut…")).click =
                [] {
                    g_templateDraft.warning.clear();
                    StartRecording(kRecordTemplate, [](UINT key) {
                        UINT mods = key >> 16;
                        if (!(mods & (MOD_CONTROL | MOD_ALT | MOD_WIN))) {
                            g_templateDraft.warning =
                                Tr(L"Skrót szablonu musi mieć Ctrl, Alt albo Win, "
                                   L"bo działa w każdym programie.",
                                   L"A template shortcut needs Ctrl, Alt or Win, because it works "
                                   L"in every app.");
                        } else {
                            g_templateDraft.hotkey = key;
                        }
                        Redraw();
                    });
                };
            kx += sw + 6;
            if (d.hotkey) {
                float rw = ButtonWidth(Tr(L"Usuń skrót", L"Remove shortcut"));
                Button({kx, y, kx + rw, y + 32}, HIT(0), Tr(L"Usuń skrót", L"Remove shortcut"), 0,
                       kSubtle)
                    .click = [] {
                    g_templateDraft.hotkey = 0;
                    Redraw();
                };
            }
            y += 38;
            std::wstring conflict = ShortcutConflict(d.hotkey, -1, d.id);
            std::wstring warning = !d.warning.empty() ? d.warning
                                   : !conflict.empty()
                                       ? Tr(L"Ten sam skrót ma: ", L"Also used by: ") + conflict
                                       : L"";
            if (!warning.empty()) {
                Glyph(kIconWarning, g_fonts.icon12, {x, y, x + 14, y + 20}, t.warning);
                Text(warning, g_fonts.small, {x + 20, y, right, y + 20}, t.warning);
            } else {
                Text(Tr(L"Skrót działa w każdym programie i od razu wkleja szablon.",
                        L"The shortcut works in every app and pastes the template right away."),
                     g_fonts.small, {x, y, right, y + 20}, t.text2);
            }
            y += 34;
            bool ok = !Trim(FieldText(FTemplateName)).empty() && !FieldText(FTemplateBody).empty();
            return DialogButtons(b, y, Tr(L"Zapisz", L"Save"), ok, SaveTemplateDraft);
        },
        SaveTemplateDraft,
        [] {
            if (g_recordTarget == kRecordTemplate) {
                StopRecording();
            }
            CloseDialog();
        });
    UpdateWindow(g_hwnd);
    FocusField(existing ? FTemplateBody : FTemplateName);
}

struct ExportDraft {
    bool pinned = true, templates = true, settings = true, history = false;
    bool protect = true, showPassword = false, busy = false;
} g_exportDraft;

void SetPasswordVisible(bool visible) {
    HWND hwnd = EnsureField(FPassword, false, true);
    SendMessageW(hwnd, EM_SETPASSWORDCHAR, visible ? 0 : 0x25CF, 0);
    InvalidateRect(hwnd, nullptr, TRUE);
}

// The password field with its show/hide button; returns its bottom.
float PasswordField(float x, float y, float right, bool* show) {
    D2D1_RECT_F field{x, y, right, y + 32};
    PlaceField(FPassword, field, Tr(L"Hasło", L"Password"), 32);
    D2D1_RECT_F eye{right - 34, y + 3, right - 4, y + 29};
    IconButton(eye, HIT(0), kIconEye,
               *show ? Tr(L"Ukryj hasło", L"Hide password") : Tr(L"Pokaż hasło", L"Show password"),
               g_theme.text2, g_fonts.icon14)
        .click = [show] {
        *show = !*show;
        SetPasswordVisible(*show);
        Redraw();
    };
    return field.bottom;
}

void StartExport() {
    ExportDraft& d = g_exportDraft;
    UINT32 flags = (d.pinned ? kExportPinned : 0) | (d.templates ? kExportTemplates : 0) |
                   (d.settings ? kExportSettings : 0) | (d.history ? kExportHistory : 0);
    std::wstring password = d.protect ? FieldText(FPassword) : L"";
    if (!flags || d.busy || (d.protect && password.empty())) {
        return;
    }
    std::wstring name = Tr(L"Schowek-kopia-", L"Clipboard-backup-") +
                        FormatDatePattern(NowFileTime(), L"yyyy-MM-dd") + L".schowek";
    COMDLG_FILTERSPEC filters[] = {{Tr(L"Kopia schowka", L"Clipboard backup"), L"*.schowek"}};
    std::wstring path;
    if (!FileDialog<IFileSaveDialog>(CLSID_FileSaveDialog, Tr(L"Eksportuj dane", L"Export data"),
                                     name.c_str(), filters, 1, L"schowek", &path)) {
        return;
    }
    d.busy = true;
    Redraw();
    ExportData(flags, path, std::move(password), [path](bool ok) {
        g_exportDraft.busy = false;
        CloseDialog();
        ShowToast(ok ? Tr(L"Zapisano kopię „", L"Saved the backup “") + FileName(path) + L"”."
                     : Tr(L"Nie udało się zapisać kopii. Sprawdź, czy folder jest dostępny.",
                          L"Couldn't save the backup. Check that the folder is available."));
    });
}

void ShowExportDialog() {
    g_exportDraft.busy = false;
    g_exportDraft.showPassword = false;
    SetFieldText(FPassword, L"");
    SetPasswordVisible(false);
    g_fields[FPassword].onChange = [] { Redraw(); };
    g_fields[FPassword].onKey = nullptr;
    ShowDialog(
        Tr(L"Eksportuj dane", L"Export data"), 460,
        [](D2D1_RECT_F b) {
            const Theme& t = g_theme;
            ExportDraft& d = g_exportDraft;
            float x = b.left, right = b.right, y = b.top;
            const wchar_t* intro =
                Tr(L"Jeden plik do przeniesienia na inny komputer. Tam otwierasz go w Ustawienia › "
                   L"Importuj…",
                   L"One file to move to another computer. Open it there in Settings › Import…");
            float ih = TextHeight(intro, g_fonts.body, right - x);
            Text(intro, g_fonts.body, {x, y, right, y + ih}, t.text2);
            y += ih + 12;
            struct Option {
                bool* value;
                std::wstring label;
            };
            Option options[] = {
                {&d.pinned, Tr(L"Przypięte z kategoriami i tagami (",
                               L"Pinned items with categories and tags (") +
                                std::to_wstring(CountItems(Kind::Pinned)) + L")"},
                {&d.templates, Tr(L"Szablony (", L"Templates (") +
                                   std::to_wstring(CountItems(Kind::Template)) + L")"},
                {&d.settings, Tr(L"Ustawienia okna i wyglądu", L"Window and appearance settings")},
                {&d.history, Tr(L"Zwykła historia z obrazami (", L"Regular history with images (") +
                                 std::to_wstring(CountItems(Kind::History)) + L")"},
            };
            for (Option& o : options) {
                bool* value = o.value;
                Checkbox({x, y, right, y + 32}, HIT(value), o.label, *value).click = [value] {
                    *value = !*value;
                    Redraw();
                };
                y += 34;
            }
            y += 6;
            Fill({x, y, right, y + 1}, t.divider);
            y += 10;
            Text(Tr(L"Chroń plik hasłem", L"Protect the file with a password"), g_fonts.row,
                 {x, y, right - 50, y + 32}, t.text);
            Toggle(right - 40, y + 16, HIT(0), d.protect).click = [] {
                g_exportDraft.protect = !g_exportDraft.protect;
                Redraw();
            };
            y += 40;
            if (d.protect) {
                y = PasswordField(x, y, right, &d.showPassword) + 6;
                const wchar_t* hint =
                    Tr(L"Bez tego hasła pliku nie otworzy nikt, także Ty, więc je zapisz.",
                       L"Without this password nobody can open the file, you included, so write it "
                       L"down.");
                float hh = TextHeight(hint, g_fonts.body, right - x);
                Text(hint, g_fonts.body, {x, y, right, y + hh}, t.text2);
                y += hh + 16;
            } else {
                const wchar_t* warn = Tr(
                    L"Plik bez hasła nie jest szyfrowany: każdy, kto go dostanie, "
                    L"przeczyta zawartość.",
                    L"A file without a password isn't encrypted: anyone who gets it can read it.");
                float wh = TextHeight(warn, g_fonts.body, right - x - 24);
                Glyph(kIconWarning, g_fonts.icon16, {x, y, x + 18, y + 20}, t.warning);
                Text(warn, g_fonts.body, {x + 24, y, right, y + wh}, t.warning);
                y += wh + 16;
            }
            bool any = d.pinned || d.templates || d.settings || d.history;
            bool ok = any && !d.busy &&
                      (!d.protect || GetWindowTextLengthW(g_fields[FPassword].hwnd) > 0);
            return DialogButtons(
                b, y, d.busy ? Tr(L"Zapisywanie…", L"Saving…") : Tr(L"Zapisz plik…", L"Save file…"),
                ok, StartExport);
        },
        StartExport);
}

void ChooseImportFile() {
    COMDLG_FILTERSPEC filters[] = {{Tr(L"Kopia schowka", L"Clipboard backup"), L"*.schowek"},
                                   {Tr(L"Wszystkie pliki", L"All files"), L"*.*"}};
    std::wstring path;
    if (FileDialog<IFileOpenDialog>(CLSID_FileOpenDialog, Tr(L"Importuj dane", L"Import data"),
                                    nullptr, filters, 2, L"schowek", &path)) {
        LoadImportFile(path);
        Redraw();
    }
}

bool g_showImportPassword;

void ShowImportDialog() {
    g_import = {};
    g_showImportPassword = false;
    SetFieldText(FPassword, L"");
    SetPasswordVisible(false);
    g_fields[FPassword].onChange = [] {
        g_import.status = ImportState::NeedPassword;
        g_import.data = nullptr;
        SetTimer(g_hwnd, kTimerPassword, 500, nullptr);
        Redraw();
    };
    g_fields[FPassword].onKey = nullptr;
    ShowDialog(
        Tr(L"Importuj dane", L"Import data"), 480,
        [](D2D1_RECT_F b) {
            const Theme& t = g_theme;
            ImportState& s = g_import;
            float x = b.left, right = b.right, y = b.top;
            y = Label(Tr(L"Plik", L"File"), x, y, right);
            float cw = ButtonWidth(Tr(L"Wybierz…", L"Choose…"));
            D2D1_RECT_F file{x, y, right - cw - 8, y + 32};
            Fill(file, t.control, 4);
            Stroke(file, t.controlStroke, 4);
            Glyph(kIconFiles, g_fonts.icon14,
                  {file.left + 8, file.top, file.left + 26, file.bottom}, t.text2);
            Text(s.path.empty() ? Tr(L"Nie wybrano pliku", L"No file chosen") : FileName(s.path),
                 g_fonts.row, {file.left + 34, file.top, file.right - 8, file.bottom},
                 s.path.empty() ? t.text3 : t.text);
            Button({right - cw, y, right, y + 32}, HIT(0), Tr(L"Wybierz…", L"Choose…")).click =
                ChooseImportFile;
            y += 44;
            if (s.encrypted && s.status != ImportState::Invalid &&
                s.status != ImportState::Reading) {
                y = Label(Tr(L"Hasło", L"Password"), x, y, right);
                y = PasswordField(x, y, right, &g_showImportPassword) + 10;
            }
            std::wstring status;
            D2D1_COLOR_F color = t.text2;
            wchar_t glyph = kIconInfo;
            switch (s.status) {
                case ImportState::None:
                    status = Tr(L"Wybierz plik .schowek zapisany przez „Eksportuj dane”.",
                                L"Choose a .schowek file saved with “Export data”.");
                    break;
                case ImportState::Reading:
                    status = Tr(L"Wczytywanie pliku…", L"Loading the file…");
                    break;
                case ImportState::NeedPassword:
                    status = Tr(
                        L"Plik jest chroniony hasłem. Wpisz je, żeby zobaczyć, co zawiera.",
                        L"The file is protected with a password. Enter it to see what's inside.");
                    break;
                case ImportState::Checking:
                    status = Tr(L"Sprawdzam hasło…", L"Checking the password…");
                    break;
                case ImportState::WrongPassword:
                    status = Tr(L"To hasło nie pasuje do pliku.",
                                L"This password doesn't match the file.");
                    color = t.danger;
                    glyph = kIconWarning;
                    break;
                case ImportState::Invalid:
                    status = Tr(L"To nie jest plik kopii schowka albo jest uszkodzony.",
                                L"This isn't a clipboard backup file, or it's damaged.");
                    color = t.danger;
                    glyph = kIconWarning;
                    break;
                case ImportState::Ready:
                    status = (s.encrypted ? Tr(L"Hasło poprawne. ", L"Password correct. ") : L"") +
                             ImportSummary(*s.data);
                    color = t.text;
                    glyph = kIconCheck;
                    break;
            }
            float sh = TextHeight(status, g_fonts.body, right - x - 26);
            Glyph(glyph, g_fonts.icon14, {x, y, x + 18, y + 20},
                  s.status == ImportState::Ready ? t.accent : color);
            Text(status, g_fonts.body, {x + 26, y, right, y + sh}, color);
            y += sh + 16;
            if (s.status == ImportState::Ready) {
                Text(Tr(L"Co zrobić z danymi na tym komputerze?",
                        L"What should happen to the data on this computer?"),
                     g_fonts.row, {x, y, right, y + 22}, t.text);
                y += 28;
                struct Mode {
                    bool replace;
                    const wchar_t* label;
                    const wchar_t* description;
                };
                const Mode kModes[] = {
                    {false, Tr(L"Połącz z obecnymi", L"Merge with current data"),
                     Tr(L"Dodam brakujące, duplikaty pominę",
                        L"Adds what's missing, skips duplicates")},
                    {true, Tr(L"Zastąp obecne", L"Replace current data"),
                     Tr(L"Najpierw zrobię kopię tego, co jest teraz",
                        L"Backs up the current data first")},
                };
                for (const Mode& m : kModes) {
                    D2D1_RECT_F r{x, y, right, y + 44};
                    Hit h = HIT(m.replace);
                    if (MouseOver(r)) {
                        Fill(r, SubtleFill(h), 4);
                    }
                    Radio({x + 4, y + 2, x + 28, y + 24}, h, s.replace == m.replace);
                    Text(m.label, g_fonts.row, {x + 34, y, right, y + 24}, t.text);
                    Text(m.description, g_fonts.small, {x + 34, y + 22, right, y + 40}, t.text2);
                    bool replace = m.replace;
                    Add(r, h).click = [replace] {
                        g_import.replace = replace;
                        Redraw();
                    };
                    y += 48;
                }
                if (!s.data->settings.empty()) {
                    Checkbox({x, y, right, y + 32}, HIT(0),
                             Tr(L"Przenieś też ustawienia okna i wyglądu",
                                L"Also bring the window and appearance settings"),
                             s.withSettings)
                        .click = [] {
                        g_import.withSettings = !g_import.withSettings;
                        Redraw();
                    };
                    y += 40;
                }
                y += 4;
            }
            return DialogButtons(b, y, Tr(L"Importuj", L"Import"), s.status == ImportState::Ready,
                                 [] {
                                     CloseDialog();
                                     ApplyImport();
                                 });
        },
        [] {
            if (g_import.status == ImportState::Ready) {
                CloseDialog();
                ApplyImport();
            }
        });
}

////////////////////////////////////////////////////////////////////////////////
// Rendering and keyboard

void DrawSizeHint(D2D1_SIZE_F s) {
    const Theme& t = g_theme;
    const wchar_t* const kModes[] = {
        Tr(L"zapamiętam do restartu", L"remembered until restart"),
        Tr(L"zapamiętam na stałe", L"remembered permanently"),
        Tr(L"po zamknięciu wróci domyślny", L"back to default after closing")};
    WCHAR text[128];
    swprintf_s(text, L"%ld × %ld · %s", std::lround(g_sizeDip.width), std::lround(g_sizeDip.height),
               kModes[std::clamp<INT64>(Setting(L"sizeMode", 0), 0, 2)]);
    float w = TextWidth(text, g_fonts.small) + 24;
    D2D1_RECT_F r{(s.width - w) / 2, s.height - 60, (s.width + w) / 2, s.height - 32};
    Fill(r, t.flyout, 14);
    Stroke(r, t.controlStroke, 14);
    Text(text, g_fonts.smallCenter, r, t.text);
}

void RenderMain() {
    const Theme& t = g_theme;
    D2D1_SIZE_F s = SizeDip(g_main);
    Fill({0, 0, s.width, s.height}, WithAlpha(t.window, WindowAlpha()));
    if (g_page == Page::Settings) {
        DrawSettingsPage(s);
    } else if (g_page == Page::Trash) {
        DrawTrashPage(s);
    } else {
        DrawMainPage(s);
    }
    if (g_sizing) {
        DrawSizeHint(s);
    }
    DrawDialog();
    DrawFlyout();
    DrawTooltip();
}

void SwitchTab(Tab tab) {
    CloseDetails();
    g_page = Page::Main;
    g_tab = tab;
    g_selectedId = 0;
    g_listScroll.Reset();
    Refresh();
}

void ShowPage(Page page) {
    CloseDetails();
    CloseFlyout();
    if (g_searchOpen) {
        CloseSearch();
    }
    if (page == Page::Settings) {
        Enqueue([] {
            UINT64 size = FolderSize(g_dataDir);
            PostUi([size] {
                g_storageBytes = size;
                Redraw();
            });
        });
    }
    g_page = page;
    g_selectedId = 0;
    g_pageScroll.Reset();
    g_listScroll.Reset();
    Refresh();
}

void OpenSearch() {
    if (g_page == Page::Settings) {
        return;
    }
    g_searchOpen = true;
    g_fields[FSearch].onChange = [] {
        g_query = FieldText(FSearch);
        g_selectedId = 0;
        g_listScroll.Reset();
        Refresh();
    };
    g_fields[FSearch].onKey = nullptr;
    Redraw();
    UpdateWindow(g_hwnd);
    FocusField(FSearch);
    SendMessageW(g_fields[FSearch].hwnd, EM_SETSEL, 0, -1);
}

// Doesn't move the focus: it's also used while the window is deactivated.
void ResetSearch() {
    if (!g_searchOpen) {
        return;
    }
    g_searchOpen = false;
    SetFieldText(FSearch, L"");
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

void SetPaused(bool paused) {
    g_paused = paused;
    UpdateTrayIcon(NIM_MODIFY);
    Redraw();
}

// The rectangle of the selected row, for a menu opened from the keyboard.
D2D1_RECT_F SelectedRowRect() {
    int row = SelectedRow();
    if (row < 0) {
        return {g_listArea.left, g_listArea.top, g_listArea.right, g_listArea.top + 40};
    }
    float top = g_listArea.top - g_listScroll.pos + g_rows[row].y;
    top = std::clamp(top, g_listArea.top, g_listArea.bottom - g_rows[row].h);
    float left = g_listArea.left + 40;
    return {left, top, left + 1, top + g_rows[row].h};
}

bool OnKey(UINT vk) {
    if (g_recordingKey) {
        return true;
    }
    Surface* prev = g_s;
    g_s = &g_main;
    struct Restore {
        Surface* p;
        ~Restore() { g_s = p; }
    } restore{prev};
    if (FlyoutKey(vk)) {
        return true;
    }
    Dialog& d = g_main.dialog;
    if (d.open) {
        if (vk == VK_RETURN && d.onEnter) {
            auto fn = d.onEnter;
            fn();
            return true;
        }
        if (vk == VK_ESCAPE) {
            if (d.onEscape) {
                auto fn = d.onEscape;
                fn();
            } else {
                CloseDialog();
            }
            return true;
        }
        return false;
    }
    UINT mods = CurrentMods();
    if (g_page == Page::Settings) {
        switch (vk) {
            case VK_ESCAPE:
            case VK_BACK:
                ShowPage(Page::Main);
                return true;
            case VK_UP:
            case VK_DOWN:
                g_pageScroll.By(vk == VK_UP ? -60 : 60);
                return true;
            case VK_PRIOR:
            case VK_NEXT:
                g_pageScroll.By((vk == VK_PRIOR ? -1 : 1) * (g_pageScroll.view - 60));
                return true;
            case VK_HOME:
                g_pageScroll.To(0);
                return true;
            case VK_END:
                g_pageScroll.To(g_pageScroll.Max());
                return true;
        }
        return false;
    }
    int nth = 0;
    int shortcut = MatchShortcut(vk, kScopeWindow, &nth);
    UINT64 sel = g_selectedId;
    Item* it = FindItem(sel);
    if (g_page == Page::Trash) {
        if (shortcut == ScClose) {
            if (g_searchOpen) {
                CloseSearch();
            } else {
                ShowPage(Page::Main);
            }
            return true;
        }
        if (shortcut == ScSearch) {
            OpenSearch();
            return true;
        }
        if (it && (shortcut == ScPaste || vk == VK_RETURN)) {
            RestoreItem(sel, true);
            return true;
        }
        if (it && shortcut == ScDelete) {
            PurgeItem(sel);
            return true;
        }
    } else {
        switch (shortcut) {
            case ScPaste:
            case ScPastePlain:
                if (it) {
                    PasteItem(*it, shortcut == ScPastePlain ? PasteMode::Plain : PasteMode::Normal);
                }
                return true;
            case ScDetails:
                if (it) {
                    g_detailsId == sel ? CloseDetails() : OpenDetails(sel);
                }
                return true;
            case ScSearch:
                OpenSearch();
                return true;
            case ScPin:
                if (it && it->kind != Kind::Template) {
                    OpenPinMenu(SelectedRowRect(), HIT(0), sel);
                }
                return true;
            case ScNth:
                PasteNth(nth);
                return true;
            case ScDelete:
                if (it) {
                    if (it->kind == Kind::Template) {
                        ConfirmDeleteTemplate(SelectedRowRect(), HIT(0), sel);
                    } else {
                        TrashItem(sel);
                    }
                }
                return true;
            case ScEdit:
                if (it) {
                    EditItem(sel);
                }
                return true;
            case ScNextTab:
                SwitchTab((Tab)(((int)g_tab + 1) % 3));
                return true;
            case ScClose:
                if (g_editingText) {
                    g_editingText = false;
                    Redraw();
                } else if (g_detailsId) {
                    CloseDetails();
                } else if (g_searchOpen) {
                    CloseSearch();
                } else {
                    HidePanel(true);
                }
                return true;
        }
        if (vk == VK_TAB && mods == (MOD_CONTROL | MOD_SHIFT) &&
            g_keys[ScNextTab] == Key(MOD_CONTROL, VK_TAB)) {
            SwitchTab((Tab)(((int)g_tab + 2) % 3));
            return true;
        }
    }
    switch (vk) {
        case VK_UP:
        case VK_DOWN:
            MoveSelection(vk == VK_DOWN ? 1 : -1);
            return true;
        case VK_PRIOR:
        case VK_NEXT: {
            int rows = std::max(1, (int)(g_listScroll.view / (kRowHeight + kRowGap)) - 1);
            for (int i = 0; i < rows; i++) {
                MoveSelection(vk == VK_NEXT ? 1 : -1);
            }
            return true;
        }
        case VK_HOME:
            Select(FirstEntryRow());
            return true;
        case VK_END:
            for (int i = (int)g_rows.size() - 1; i >= 0; i--) {
                if (g_rows[i].type == Row::Entry) {
                    Select(i);
                    break;
                }
            }
            return true;
    }
    return false;
}

bool IsEditingKey(UINT vk, UINT mods) {
    if (mods & (MOD_ALT | MOD_WIN)) {
        return false;
    }
    if (mods & MOD_CONTROL) {
        return wcschr(L"ACVXZY", (wchar_t)vk) || vk == VK_LEFT || vk == VK_RIGHT ||
               vk == VK_BACK || vk == VK_DELETE || vk == VK_HOME || vk == VK_END ||
               vk == VK_INSERT;
    }
    switch (vk) {
        case VK_RETURN:
        case VK_ESCAPE:
        case VK_UP:
        case VK_DOWN:
        case VK_PRIOR:
        case VK_NEXT:
            return false;
    }
    return !(vk >= VK_F1 && vk <= VK_F24);
}

bool OnFieldKey(Field& f, UINT vk) {
    if (f.onKey && f.onKey(vk)) {
        return true;
    }
    UINT mods = CurrentMods();
    if (vk == VK_TAB && !(mods & (MOD_CONTROL | MOD_ALT))) {
        // Moves between the visible fields.
        std::vector<HWND> visible;
        for (Field& other : g_fields) {
            if (other.hwnd && IsWindowVisible(other.hwnd)) {
                visible.push_back(other.hwnd);
            }
        }
        auto self = std::find(visible.begin(), visible.end(), f.hwnd);
        if (self != visible.end() && visible.size() > 1) {
            int i = (int)(self - visible.begin());
            int n = (int)visible.size();
            SetFocus(visible[(i + ((mods & MOD_SHIFT) ? n - 1 : 1)) % n]);
        }
        return true;
    }
    if (f.multiline) {
        return vk == VK_ESCAPE ? OnKey(vk) : false;
    }
    if (IsEditingKey(vk, mods)) {
        return false;
    }
    return OnKey(vk);
}

////////////////////////////////////////////////////////////////////////////////
// Image editor: a separate window on the UI thread. Strokes, shapes, texts and
// blurred areas are a list of objects over the original bitmap, in image
// pixels. Saving renders them at the original resolution into a new item, so
// the original stays as it was.

enum class Tool : BYTE { Select, Brush, Highlighter, Eraser, Text, Shape, Blur, Crop };
enum class ShapeKind : BYTE { Arrow, Line, Circle, Square, Rect, Bubble };

struct EdObject {
    enum Type : BYTE { Stroke, Shape, Text, Blur };
    Type type = Stroke;
    UINT32 color = 0xE81123;
    float width = 6;  // Line thickness; for a blurred area, the block size.
    bool highlighter = false;
    std::vector<D2D1_POINT_2F> points;  // A stroke.
    ShapeKind shape = ShapeKind::Arrow;
    D2D1_POINT_2F a{}, b{};  // Shape ends, blurred area corners, a text's top left.
    std::wstring text;
    float size = 32;
    bool bold = false, italic = false, underline = false;
};

struct EdDoc {
    std::vector<EdObject> objects;
    D2D1_RECT_F crop{};
    UINT64 version = 0;  // Changes with every edit; tells the saved state.
};

constexpr UINT32 kEdColors[] = {0xFFFFFF, 0x1A1A1A, 0xE81123, 0xF7630C,
                                0xFFB900, 0x16C60C, 0x0078D4, 0x8764B8};
const Bilingual kEdColorNames[] = {{L"Biały", L"White"},    {L"Czarny", L"Black"},
                                   {L"Czerwony", L"Red"},   {L"Pomarańczowy", L"Orange"},
                                   {L"Żółty", L"Yellow"},   {L"Zielony", L"Green"},
                                   {L"Niebieski", L"Blue"}, {L"Fioletowy", L"Purple"}};
const Bilingual kShapeNames[] = {{L"Strzałka", L"Arrow"},
                                 {L"Linia", L"Line"},
                                 {L"Okrąg (z Shift: koło)", L"Ellipse (with Shift: circle)"},
                                 {L"Kwadrat", L"Square"},
                                 {L"Prostokąt", L"Rectangle"},
                                 {L"Dymek", L"Speech bubble"}};
constexpr float kEdTextSizes[] = {12, 16, 20, 24, 32, 40, 48, 64, 80, 96, 128};
const Bilingual kAspectNames[] = {{L"Dowolne", L"Free"}, {L"Jak oryginał", L"Original"},
                                  {L"1:1", L"1:1"},      {L"4:3", L"4:3"},
                                  {L"3:2", L"3:2"},      {L"16:9", L"16:9"}};
constexpr float kEdTitleHeight = 40;
constexpr float kEdToolbarHeight = 48;
constexpr float kEdOptionsHeight = 44;
constexpr float kEdFooterHeight = 56;
constexpr float kEdMinWidth = 760;
constexpr float kEdMinHeight = 520;
constexpr UINT64 kBlurKey = 1ULL << 62;  // Bitmap cache key flag.
constexpr float kQuarterTurn = 0.785398163f;  // 45°, in radians.

enum class EdDrag : BYTE { None, Pan, Draw, Erase, NewShape, Move, Handle, Crop };

struct Editor {
    HWND hwnd = nullptr;
    UINT64 itemId = 0;
    bool viewOnly = false;
    Kind kind = Kind::History;
    UINT64 category = 0;
    std::wstring source;
    // The original image, premultiplied BGRA; empty while it loads.
    UINT w = 0, h = 0;
    std::string pixels;
    bool loading = false;
    UINT64 generation = 0;
    std::wstring error, message;
    EdDoc doc;
    std::vector<EdDoc> undo, redo;
    UINT64 versions = 0, savedVersion = 0;
    bool changeOpen = false;  // A drag or typing that already saved its undo step.
    // Tools; their settings stay for the next image.
    Tool tool = Tool::Brush, beforeCrop = Tool::Brush;
    UINT32 color = 0xE81123;
    float brushWidth = 6, highlighterWidth = 24, eraserWidth = 24, shapeWidth = 6;
    float blurBlock = 16, textSize = 32;
    bool bold = false, italic = false, underline = false;
    ShapeKind shape = ShapeKind::Arrow;
    int selected = -1;
    int editing = -1;  // The text being typed.
    size_t caret = 0;
    bool caretOn = true;
    D2D1_RECT_F cropDraft{};
    int aspect = 0;
    // The view: DIPs per image pixel, and the offset of the image center from
    // the stage center.
    float zoom = 1;
    D2D1_POINT_2F pan{};
    bool fitted = true;
    bool space = false, middlePan = false;
    Hit stageHit;
    // The drag in progress.
    EdDrag drag = EdDrag::None;
    int handle = -1;
    bool created = false;
    D2D1_POINT_2F dragStart{}, dragScreen{}, panStart{}, lastErase{};
    EdObject dragOrigin;
    D2D1_RECT_F cropOrigin{};
} g_ed;

constexpr wchar_t kEditorClass[] = L"ClipboardTweaksEditor_" WH_MOD_VERSION;
ATOM g_editorClass;

// Points g_s at a surface for the code that runs outside its paint and mouse
// handling, such as keys.
struct UseSurface {
    Surface* prev;
    explicit UseSurface(Surface& s) : prev(g_s) { g_s = &s; }
    ~UseSurface() { g_s = prev; }
};

void CloseEditor(std::function<void()> after = nullptr);
bool SaveEditor(bool copy);

////////////////////////////////////////////////////////////////////////////////
// Geometry

D2D1_RECT_F Norm(D2D1_POINT_2F a, D2D1_POINT_2F b) {
    return {std::min(a.x, b.x), std::min(a.y, b.y), std::max(a.x, b.x), std::max(a.y, b.y)};
}

float Dist(D2D1_POINT_2F a, D2D1_POINT_2F b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}

float SegmentDistance(D2D1_POINT_2F p, D2D1_POINT_2F a, D2D1_POINT_2F b) {
    float dx = b.x - a.x, dy = b.y - a.y;
    float len = dx * dx + dy * dy;
    float t = len > 0 ? std::clamp(((p.x - a.x) * dx + (p.y - a.y) * dy) / len, 0.f, 1.f) : 0;
    return Dist(p, {a.x + t * dx, a.y + t * dy});
}

bool Shift() {
    return GetKeyState(VK_SHIFT) < 0;
}

float EdActual() {
    return 96.f / g_edView.dpi;  // Zoom of 100%: one image pixel per screen pixel.
}

D2D1_RECT_F EdStageRect() {
    D2D1_SIZE_F s = SizeDip(g_edView);
    float top = kEdTitleHeight + (g_ed.viewOnly ? 0 : kEdToolbarHeight + kEdOptionsHeight);
    return {0, top, s.width, std::max(top + 1, s.height - kEdFooterHeight)};
}

D2D1_RECT_F FullImage() {
    return {0, 0, (float)g_ed.w, (float)g_ed.h};
}

// The part of the image the view shows: all of it while cropping.
D2D1_RECT_F EdRef() {
    return g_ed.tool == Tool::Crop && !g_ed.viewOnly ? FullImage() : g_ed.doc.crop;
}

// Where the image's (0, 0) is on the screen.
D2D1_POINT_2F EdOrigin() {
    D2D1_RECT_F ref = EdRef(), s = EdStageRect();
    return {(s.left + s.right) / 2 + g_ed.pan.x - (ref.left + ref.right) / 2 * g_ed.zoom,
            (s.top + s.bottom) / 2 + g_ed.pan.y - (ref.top + ref.bottom) / 2 * g_ed.zoom};
}

D2D1_POINT_2F ToImage(D2D1_POINT_2F p) {
    D2D1_POINT_2F o = EdOrigin();
    return {(p.x - o.x) / g_ed.zoom, (p.y - o.y) / g_ed.zoom};
}

D2D1_POINT_2F ToScreen(D2D1_POINT_2F p) {
    D2D1_POINT_2F o = EdOrigin();
    return {o.x + p.x * g_ed.zoom, o.y + p.y * g_ed.zoom};
}

D2D1_RECT_F ToScreenRect(const D2D1_RECT_F& r) {
    D2D1_POINT_2F a = ToScreen({r.left, r.top}), b = ToScreen({r.right, r.bottom});
    return {a.x, a.y, b.x, b.y};
}

D2D1_POINT_2F StageCenter() {
    D2D1_RECT_F s = EdStageRect();
    return {(s.left + s.right) / 2, (s.top + s.bottom) / 2};
}

// Keeps a part of the image in view.
void ClampPan() {
    D2D1_RECT_F ref = EdRef(), s = EdStageRect();
    float mx = std::max(0.f, ((ref.right - ref.left) * g_ed.zoom + (s.right - s.left)) / 2 - 64);
    float my = std::max(0.f, ((ref.bottom - ref.top) * g_ed.zoom + (s.bottom - s.top)) / 2 - 64);
    g_ed.pan.x = std::clamp(g_ed.pan.x, -mx, mx);
    g_ed.pan.y = std::clamp(g_ed.pan.y, -my, my);
}

void EdFit() {
    D2D1_RECT_F ref = EdRef(), s = EdStageRect();
    float rw = std::max(1.f, ref.right - ref.left), rh = std::max(1.f, ref.bottom - ref.top);
    float z = std::min({(s.right - s.left - 48) / rw, (s.bottom - s.top - 48) / rh, EdActual()});
    g_ed.zoom = std::max(z, 0.01f * EdActual());
    g_ed.pan = {0, 0};
    g_ed.fitted = true;
    RedrawEditor();
}

void EdSetZoom(float zoom, D2D1_POINT_2F at) {
    zoom = std::clamp(zoom, 0.05f * EdActual(), 16 * EdActual());
    D2D1_POINT_2F ip = ToImage(at);
    g_ed.zoom = zoom;
    D2D1_POINT_2F now = ToScreen(ip);
    g_ed.pan.x += at.x - now.x;
    g_ed.pan.y += at.y - now.y;
    g_ed.fitted = false;
    ClampPan();
    RedrawEditor();
}

int ZoomPercent() {
    return (int)std::lround(g_ed.zoom / EdActual() * 100);
}

// The buttons and shortcuts step through round percentages.
void EdZoomStep(int dir) {
    static const float kSteps[] = {5,   10,  15,  20,  25,  33,  50,  67,   75,  100,
                                   125, 150, 200, 300, 400, 600, 800, 1200, 1600};
    float pct = g_ed.zoom / EdActual() * 100;
    float target = dir > 0 ? kSteps[ARRAYSIZE(kSteps) - 1] : kSteps[0];
    for (float step : kSteps) {
        if (dir > 0 && step > pct * 1.01f) {
            target = step;
            break;
        }
        if (dir < 0 && step < pct * 0.99f) {
            target = step;
        }
    }
    EdSetZoom(target / 100 * EdActual(), StageCenter());
}

////////////////////////////////////////////////////////////////////////////////
// Objects

IDWriteTextLayout* TextLayout(const EdObject& o) {
    IDWriteTextFormat* format = nullptr;
    if (FAILED(g_dwrite->CreateTextFormat(
            L"Segoe UI", nullptr, o.bold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
            o.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL, o.size, L"", &format))) {
        return nullptr;
    }
    format->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
    IDWriteTextLayout* layout = nullptr;
    g_dwrite->CreateTextLayout(o.text.data(), (UINT32)o.text.size(), format, 100000, 100000,
                               &layout);
    format->Release();
    if (layout && o.underline && !o.text.empty()) {
        layout->SetUnderline(TRUE, {0, (UINT32)o.text.size()});
    }
    return layout;
}

D2D1_RECT_F ObjectBounds(const EdObject& o) {
    switch (o.type) {
        case EdObject::Text: {
            DWRITE_TEXT_METRICS m{};
            if (IDWriteTextLayout* layout = TextLayout(o)) {
                layout->GetMetrics(&m);
                layout->Release();
            }
            float w = std::max(m.widthIncludingTrailingWhitespace, o.size * 0.5f);
            float h = std::max(m.height, o.size * 1.3f);
            return {o.a.x, o.a.y, o.a.x + w, o.a.y + h};
        }
        case EdObject::Stroke: {
            D2D1_RECT_F r{1e9f, 1e9f, -1e9f, -1e9f};
            for (D2D1_POINT_2F p : o.points) {
                r = {std::min(r.left, p.x), std::min(r.top, p.y), std::max(r.right, p.x),
                     std::max(r.bottom, p.y)};
            }
            return Inset(r, -o.width / 2, -o.width / 2);
        }
        case EdObject::Shape: {
            D2D1_RECT_F r = Norm(o.a, o.b);
            if (o.shape == ShapeKind::Bubble) {
                r.bottom += (r.bottom - r.top) * 0.35f;
            }
            return Inset(r, -o.width / 2, -o.width / 2);
        }
        case EdObject::Blur:
            return Norm(o.a, o.b);
    }
    return {};
}

bool IsLine(const EdObject& o) {
    return o.type == EdObject::Shape &&
           (o.shape == ShapeKind::Line || o.shape == ShapeKind::Arrow);
}

enum class HitFilter : BYTE { All, Text, NoStrokes };

// The topmost object under a point.
int HitObject(D2D1_POINT_2F ip, HitFilter filter = HitFilter::All) {
    float tolerance = 6 / g_ed.zoom;
    const auto& objects = g_ed.doc.objects;
    for (int i = (int)objects.size() - 1; i >= 0; i--) {
        const EdObject& o = objects[i];
        if ((filter == HitFilter::Text && o.type != EdObject::Text) ||
            (filter == HitFilter::NoStrokes && o.type == EdObject::Stroke)) {
            continue;
        }
        bool hit = false;
        if (o.type == EdObject::Stroke) {
            for (D2D1_POINT_2F p : o.points) {
                hit = hit || Dist(p, ip) <= o.width / 2 + tolerance;
            }
        } else if (IsLine(o)) {
            hit = SegmentDistance(ip, o.a, o.b) <= o.width / 2 + tolerance;
        } else {
            hit = In(Inset(ObjectBounds(o), -tolerance, -tolerance), ip);
        }
        if (hit) {
            return i;
        }
    }
    return -1;
}

void Translate(EdObject& o, D2D1_POINT_2F d) {
    o.a = {o.a.x + d.x, o.a.y + d.y};
    o.b = {o.b.x + d.x, o.b.y + d.y};
    for (D2D1_POINT_2F& p : o.points) {
        p = {p.x + d.x, p.y + d.y};
    }
}

// Handles of a shape or a blurred area, in image pixels.
std::vector<D2D1_POINT_2F> Handles(const EdObject& o) {
    if (IsLine(o)) {
        return {o.a, o.b};
    }
    if (o.type == EdObject::Shape || o.type == EdObject::Blur) {
        D2D1_RECT_F r = Norm(o.a, o.b);
        return {{r.left, r.top}, {r.right, r.top}, {r.right, r.bottom}, {r.left, r.bottom}};
    }
    return {};
}

// Square shapes, Shift for a perfect circle or square, and Shift for lines at
// multiples of 45°.
D2D1_POINT_2F Constrain(const EdObject& o, D2D1_POINT_2F a, D2D1_POINT_2F b) {
    if (o.type != EdObject::Shape) {
        return b;
    }
    if (IsLine(o)) {
        if (!Shift()) {
            return b;
        }
        float len = Dist(a, b);
        float angle = std::round(std::atan2(b.y - a.y, b.x - a.x) / kQuarterTurn) *
                      kQuarterTurn;
        return {a.x + len * std::cos(angle), a.y + len * std::sin(angle)};
    }
    if (o.shape == ShapeKind::Square || Shift()) {
        float dx = b.x - a.x, dy = b.y - a.y;
        float side = std::max(std::fabs(dx), std::fabs(dy));
        return {a.x + std::copysign(side, dx), a.y + std::copysign(side, dy)};
    }
    return b;
}

ID2D1StrokeStyle* g_roundStyle;

ID2D1StrokeStyle* RoundStyle() {
    if (!g_roundStyle) {
        g_d2d->CreateStrokeStyle(
            D2D1::StrokeStyleProperties(D2D1_CAP_STYLE_ROUND, D2D1_CAP_STYLE_ROUND,
                                        D2D1_CAP_STYLE_ROUND, D2D1_LINE_JOIN_ROUND),
            nullptr, 0, &g_roundStyle);
    }
    return g_roundStyle;
}

ID2D1PathGeometry* Polygon(std::initializer_list<D2D1_POINT_2F> points, bool closed = true) {
    ID2D1PathGeometry* geometry = nullptr;
    ID2D1GeometrySink* sink = nullptr;
    if (FAILED(g_d2d->CreatePathGeometry(&geometry)) || FAILED(geometry->Open(&sink))) {
        SafeRelease(geometry);
        return nullptr;
    }
    auto it = points.begin();
    sink->BeginFigure(*it, closed ? D2D1_FIGURE_BEGIN_FILLED : D2D1_FIGURE_BEGIN_HOLLOW);
    for (++it; it != points.end(); ++it) {
        sink->AddLine(*it);
    }
    sink->EndFigure(closed ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN);
    sink->Close();
    sink->Release();
    return geometry;
}

// A speech bubble: a rounded box with a tail, as one outline.
ID2D1PathGeometry* BubbleGeometry(const EdObject& o) {
    D2D1_RECT_F r = Norm(o.a, o.b);
    float w = r.right - r.left, h = r.bottom - r.top;
    float radius = std::min(w, h) * 0.25f;
    ID2D1RoundedRectangleGeometry* box = nullptr;
    ID2D1PathGeometry* tail = Polygon({{r.left + w * 0.2f, r.bottom - radius / 2},
                                       {r.left + w * 0.1f, r.bottom + h * 0.35f},
                                       {r.left + w * 0.42f, r.bottom - radius / 2}});
    ID2D1PathGeometry* result = nullptr;
    ID2D1GeometrySink* sink = nullptr;
    if (tail &&
        SUCCEEDED(g_d2d->CreateRoundedRectangleGeometry(D2D1::RoundedRect(r, radius, radius),
                                                        &box)) &&
        SUCCEEDED(g_d2d->CreatePathGeometry(&result)) && SUCCEEDED(result->Open(&sink))) {
        box->CombineWithGeometry(tail, D2D1_COMBINE_MODE_UNION, nullptr, sink);
        sink->Close();
        sink->Release();
    }
    SafeRelease(box);
    SafeRelease(tail);
    return result;
}

void DrawEdShape(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush, const EdObject& o) {
    float w = o.width;
    brush->SetColor(Rgba(o.color));
    switch (o.shape) {
        case ShapeKind::Line:
            rt->DrawLine(o.a, o.b, brush, w, RoundStyle());
            break;
        case ShapeKind::Arrow: {
            float dx = o.b.x - o.a.x, dy = o.b.y - o.a.y, len = std::hypot(dx, dy);
            if (len < 0.5f) {
                break;
            }
            float ux = dx / len, uy = dy / len;
            float head = std::min(len, w * 3 + 10), half = w * 1.5f + 5;
            D2D1_POINT_2F base{o.b.x - ux * head, o.b.y - uy * head};
            rt->DrawLine(o.a, {o.b.x - ux * head * 0.8f, o.b.y - uy * head * 0.8f}, brush, w,
                         RoundStyle());
            if (ID2D1PathGeometry* tip = Polygon(
                    {o.b, {base.x - uy * half, base.y + ux * half},
                     {base.x + uy * half, base.y - ux * half}})) {
                rt->FillGeometry(tip, brush);
                rt->DrawGeometry(tip, brush, std::min(w, 2.f), RoundStyle());
                tip->Release();
            }
            break;
        }
        case ShapeKind::Circle: {
            D2D1_RECT_F r = Norm(o.a, o.b);
            rt->DrawEllipse(D2D1::Ellipse({(r.left + r.right) / 2, (r.top + r.bottom) / 2},
                                          (r.right - r.left) / 2, (r.bottom - r.top) / 2),
                            brush, w);
            break;
        }
        case ShapeKind::Square:
        case ShapeKind::Rect:
            rt->DrawRectangle(Norm(o.a, o.b), brush, w);
            break;
        case ShapeKind::Bubble:
            if (ID2D1PathGeometry* bubble = BubbleGeometry(o)) {
                brush->SetColor(Rgba(o.color == 0xFFFFFF ? 0x1A1A1A : 0xFFFFFF));
                rt->FillGeometry(bubble, brush);
                brush->SetColor(Rgba(o.color));
                rt->DrawGeometry(bubble, brush, w, RoundStyle());
                bubble->Release();
            }
            break;
    }
}

// The pixelated look of a blurred area: each block is the average of the
// original pixels under it.
struct BlurGrid {
    int x0 = 0, y0 = 0, x1 = 0, y1 = 0, block = 1;
    UINT cols = 0, rows = 0;
};

bool GetBlurGrid(const EdObject& o, BlurGrid* g) {
    D2D1_RECT_F r = Norm(o.a, o.b);
    g->x0 = std::clamp((int)std::floor(r.left), 0, (int)g_ed.w);
    g->y0 = std::clamp((int)std::floor(r.top), 0, (int)g_ed.h);
    g->x1 = std::clamp((int)std::ceil(r.right), 0, (int)g_ed.w);
    g->y1 = std::clamp((int)std::ceil(r.bottom), 0, (int)g_ed.h);
    g->block = std::max(2, (int)std::lround(o.width));
    g->cols = (g->x1 - g->x0 + g->block - 1) / g->block;
    g->rows = (g->y1 - g->y0 + g->block - 1) / g->block;
    return g->x1 > g->x0 && g->y1 > g->y0 && !g_ed.pixels.empty();
}

ID2D1Bitmap* BlurBitmap(ID2D1RenderTarget* rt, const BlurGrid& g) {
    std::string out((size_t)g.cols * g.rows * 4, '\0');
    const BYTE* src = (const BYTE*)g_ed.pixels.data();
    for (UINT row = 0; row < g.rows; row++) {
        for (UINT col = 0; col < g.cols; col++) {
            int bx0 = g.x0 + col * g.block, by0 = g.y0 + row * g.block;
            int bx1 = std::min(bx0 + g.block, g.x1), by1 = std::min(by0 + g.block, g.y1);
            UINT64 sum[4] = {};
            for (int y = by0; y < by1; y++) {
                const BYTE* p = src + ((size_t)y * g_ed.w + bx0) * 4;
                for (int x = bx0; x < bx1; x++, p += 4) {
                    sum[0] += p[0], sum[1] += p[1], sum[2] += p[2], sum[3] += p[3];
                }
            }
            UINT64 n = std::max(1, (bx1 - bx0) * (by1 - by0));
            BYTE* d = (BYTE*)out.data() + ((size_t)row * g.cols + col) * 4;
            for (int k = 0; k < 4; k++) {
                d[k] = (BYTE)(sum[k] / n);
            }
        }
    }
    ID2D1Bitmap* bitmap = nullptr;
    rt->CreateBitmap(D2D1::SizeU(g.cols, g.rows), out.data(), g.cols * 4,
                     D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
                                                              D2D1_ALPHA_MODE_PREMULTIPLIED)),
                     &bitmap);
    return bitmap;
}

std::unordered_set<UINT64> g_blurUsed;

// On the screen, a blurred area keeps its bitmap while it doesn't change.
ID2D1Bitmap* CachedBlur(const BlurGrid& g) {
    UINT64 key = kBlurKey | (((UINT64)g.x0 * 73856093) ^ ((UINT64)g.y0 * 19349663) ^
                             ((UINT64)g.x1 * 83492791) ^ ((UINT64)g.y1 * 2654435761ULL) ^
                             ((UINT64)g.block << 40)) >> 2;
    g_blurUsed.insert(key);
    auto& cache = g_edView.bitmaps;
    if (auto found = cache.find(key); found != cache.end()) {
        return found->second;
    }
    ID2D1Bitmap* bitmap = BlurBitmap(g_rt, g);
    if (bitmap) {
        cache[key] = bitmap;
    }
    return bitmap;
}

void DrawEdObject(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush, const EdObject& o,
                  bool screen) {
    switch (o.type) {
        case EdObject::Stroke: {
            brush->SetColor(Rgba(o.color, o.highlighter ? 0.4f : 1));
            if (o.points.size() == 1) {
                rt->FillEllipse(D2D1::Ellipse(o.points[0], o.width / 2, o.width / 2), brush);
                break;
            }
            ID2D1PathGeometry* geometry = nullptr;
            ID2D1GeometrySink* sink = nullptr;
            if (o.points.size() > 1 && SUCCEEDED(g_d2d->CreatePathGeometry(&geometry)) &&
                SUCCEEDED(geometry->Open(&sink))) {
                sink->BeginFigure(o.points[0], D2D1_FIGURE_BEGIN_HOLLOW);
                sink->AddLines(o.points.data() + 1, (UINT32)o.points.size() - 1);
                sink->EndFigure(D2D1_FIGURE_END_OPEN);
                sink->Close();
                sink->Release();
                // One geometry, so that a highlighter doesn't darken where it
                // crosses itself.
                rt->DrawGeometry(geometry, brush, o.width, RoundStyle());
            }
            SafeRelease(geometry);
            break;
        }
        case EdObject::Shape:
            DrawEdShape(rt, brush, o);
            break;
        case EdObject::Text:
            if (IDWriteTextLayout* layout = o.text.empty() ? nullptr : TextLayout(o)) {
                brush->SetColor(Rgba(o.color));
                rt->DrawTextLayout(o.a, layout, brush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
                layout->Release();
            }
            break;
        case EdObject::Blur: {
            BlurGrid g;
            if (!GetBlurGrid(o, &g)) {
                break;
            }
            ID2D1Bitmap* bitmap = screen ? CachedBlur(g) : BlurBitmap(rt, g);
            if (bitmap) {
                rt->PushAxisAlignedClip({(float)g.x0, (float)g.y0, (float)g.x1, (float)g.y1},
                                        D2D1_ANTIALIAS_MODE_ALIASED);
                rt->DrawBitmap(bitmap,
                               {(float)g.x0, (float)g.y0, (float)(g.x0 + g.cols * g.block),
                                (float)(g.y0 + g.rows * g.block)},
                               1, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
                rt->PopAxisAlignedClip();
                if (!screen) {
                    bitmap->Release();
                }
            }
            break;
        }
    }
}

void DrawEdObjects(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush, bool screen) {
    for (const EdObject& o : g_ed.doc.objects) {
        DrawEdObject(rt, brush, o, screen);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Changes and undo

bool Unsaved() {
    return g_ed.doc.version != g_ed.savedVersion;
}

// Saves an undo step before a change. A drag or a typing session merges into
// one step.
void Change(bool merge = false) {
    Editor& e = g_ed;
    e.message.clear();
    if (merge && e.changeOpen) {
        return;
    }
    EdDoc snapshot = e.doc;
    std::erase_if(snapshot.objects, [](const EdObject& o) {
        return o.type == EdObject::Text && o.text.empty();
    });
    e.undo.push_back(std::move(snapshot));
    if (e.undo.size() > 300) {
        e.undo.erase(e.undo.begin());
    }
    e.redo.clear();
    e.doc.version = ++e.versions;
    e.changeOpen = merge;
}

EdObject* Target() {
    Editor& e = g_ed;
    int i = e.editing >= 0 ? e.editing : e.tool == Tool::Select ? e.selected : -1;
    return i >= 0 && i < (int)e.doc.objects.size() ? &e.doc.objects[i] : nullptr;
}

void FinishText() {
    Editor& e = g_ed;
    if (e.editing < 0) {
        return;
    }
    int i = e.editing;
    e.editing = -1;
    e.changeOpen = false;
    KillTimer(e.hwnd, kTimerEditorCaret);
    auto& objects = e.doc.objects;
    if (i < (int)objects.size() && objects[i].type == EdObject::Text &&
        Trim(objects[i].text).empty()) {
        objects.erase(objects.begin() + i);
        if (e.selected == i) {
            e.selected = -1;
        } else if (e.selected > i) {
            e.selected--;
        }
    }
    RedrawEditor();
}

void StartText(int index, size_t caret) {
    Editor& e = g_ed;
    e.editing = index;
    e.caret = std::min(caret, e.doc.objects[index].text.size());
    e.caretOn = true;
    e.changeOpen = false;
    SetTimer(e.hwnd, kTimerEditorCaret, GetCaretBlinkTime(), nullptr);
    RedrawEditor();
}

void Undo(bool redo) {
    Editor& e = g_ed;
    FinishText();
    auto& from = redo ? e.redo : e.undo;
    auto& to = redo ? e.undo : e.redo;
    if (from.empty()) {
        return;
    }
    bool cropChanged = memcmp(&from.back().crop, &e.doc.crop, sizeof(D2D1_RECT_F)) != 0;
    to.push_back(std::move(e.doc));
    e.doc = std::move(from.back());
    from.pop_back();
    e.selected = -1;
    e.changeOpen = false;
    e.drag = EdDrag::None;
    e.message.clear();
    if (cropChanged) {
        EdFit();
    }
    RedrawEditor();
}

void SetTool(Tool tool) {
    Editor& e = g_ed;
    if (e.viewOnly) {
        return;
    }
    FinishText();
    CloseFlyout();
    e.drag = EdDrag::None;
    bool wasCrop = e.tool == Tool::Crop;
    if (tool == Tool::Crop && !wasCrop) {
        e.beforeCrop = e.tool;
        e.cropDraft = e.doc.crop;
    }
    e.tool = tool;
    if (tool != Tool::Select) {
        e.selected = -1;
    }
    if (wasCrop != (tool == Tool::Crop)) {
        EdFit();
    }
    RedrawEditor();
}

void ApplyCrop() {
    Editor& e = g_ed;
    D2D1_RECT_F r{std::round(e.cropDraft.left), std::round(e.cropDraft.top),
                  std::round(e.cropDraft.right), std::round(e.cropDraft.bottom)};
    if (r.right - r.left >= 1 && r.bottom - r.top >= 1 &&
        memcmp(&r, &e.doc.crop, sizeof(r)) != 0) {
        Change();
        e.doc.crop = r;
    }
    e.tool = e.beforeCrop;
    EdFit();
}

void CancelCrop() {
    g_ed.tool = g_ed.beforeCrop;
    EdFit();
}

float AspectRatio() {
    static const float kRatios[] = {0, 0, 1, 4.f / 3, 3.f / 2, 16.f / 9};
    Editor& e = g_ed;
    if (e.aspect == 1) {
        return (float)e.w / std::max(1u, e.h);
    }
    float ratio = kRatios[std::clamp(e.aspect, 0, 5)];
    D2D1_RECT_F d = e.cropDraft;
    // Follows the crop's orientation.
    return ratio && d.bottom - d.top > d.right - d.left ? 1 / ratio : ratio;
}

void SetAspect(int aspect) {
    Editor& e = g_ed;
    e.aspect = aspect;
    float ratio = AspectRatio();
    if (ratio > 0) {
        // The largest crop of that shape, centered in the current one.
        D2D1_RECT_F d = e.cropDraft;
        float w = d.right - d.left, h = d.bottom - d.top;
        if (w / h > ratio) {
            w = h * ratio;
        } else {
            h = w / ratio;
        }
        float cx = (d.left + d.right) / 2, cy = (d.top + d.bottom) / 2;
        e.cropDraft = {cx - w / 2, cy - h / 2, cx + w / 2, cy + h / 2};
    }
    RedrawEditor();
}

D2D1_POINT_2F CropHandle(const D2D1_RECT_F& r, int i) {
    float cx = (r.left + r.right) / 2, cy = (r.top + r.bottom) / 2;
    switch (i) {
        case 0: return {r.left, r.top};
        case 1: return {cx, r.top};
        case 2: return {r.right, r.top};
        case 3: return {r.right, cy};
        case 4: return {r.right, r.bottom};
        case 5: return {cx, r.bottom};
        case 6: return {r.left, r.bottom};
        default: return {r.left, cy};
    }
}

// The crop after dragging a handle (0..7 clockwise from the top left corner,
// -2 moves it) by d image pixels.
D2D1_RECT_F CropFrom(const D2D1_RECT_F& o, int handle, D2D1_POINT_2F d) {
    float W = (float)g_ed.w, H = (float)g_ed.h;
    D2D1_RECT_F r = o;
    if (handle == -2) {
        float w = o.right - o.left, h = o.bottom - o.top;
        r.left = std::clamp(o.left + d.x, 0.f, W - w);
        r.top = std::clamp(o.top + d.y, 0.f, H - h);
        return {r.left, r.top, r.left + w, r.top + h};
    }
    bool left = handle == 0 || handle == 6 || handle == 7;
    bool right = handle >= 2 && handle <= 4;
    bool top = handle <= 2;
    bool bottom = handle >= 4 && handle <= 6;
    constexpr float kMin = 8;
    if (left) r.left = std::clamp(o.left + d.x, 0.f, o.right - kMin);
    if (right) r.right = std::clamp(o.right + d.x, o.left + kMin, W);
    if (top) r.top = std::clamp(o.top + d.y, 0.f, o.bottom - kMin);
    if (bottom) r.bottom = std::clamp(o.bottom + d.y, o.top + kMin, H);
    float ratio = AspectRatio();
    if (ratio > 0 && (left || right) && (top || bottom)) {
        float w = r.right - r.left, h = r.bottom - r.top;
        if (w / h > ratio) {
            w = h * ratio;
        } else {
            h = w / ratio;
        }
        if (left) r.left = r.right - w; else r.right = r.left + w;
        if (top) r.top = r.bottom - h; else r.bottom = r.top + h;
    }
    return r;
}

// A new crop dragged from a point.
D2D1_RECT_F CropBetween(D2D1_POINT_2F a, D2D1_POINT_2F b) {
    a = {std::clamp(a.x, 0.f, (float)g_ed.w), std::clamp(a.y, 0.f, (float)g_ed.h)};
    b = {std::clamp(b.x, 0.f, (float)g_ed.w), std::clamp(b.y, 0.f, (float)g_ed.h)};
    float ratio = AspectRatio();
    if (ratio > 0) {
        float w = std::fabs(b.x - a.x), h = std::fabs(b.y - a.y);
        if (w / std::max(h, 1.f) > ratio) {
            w = h * ratio;
        } else {
            h = w / ratio;
        }
        b = {a.x + std::copysign(w, b.x - a.x), a.y + std::copysign(h, b.y - a.y)};
    }
    return Norm(a, b);
}

////////////////////////////////////////////////////////////////////////////////
// Tool settings: they change the selected object too.

void SetEdColor(UINT32 color) {
    g_ed.color = color;
    if (EdObject* t = Target(); t && t->type != EdObject::Blur && t->color != color) {
        Change();
        Target()->color = color;
    }
    RedrawEditor();
}

void SetEdWidth(float v) {
    Editor& e = g_ed;
    EdObject* t = Target();
    if (t && t->type != EdObject::Text) {
        (t->type == EdObject::Shape  ? e.shapeWidth
         : t->type == EdObject::Blur ? e.blurBlock
         : t->highlighter            ? e.highlighterWidth
                                     : e.brushWidth) = v;
        if (t->width != v) {
            Change(true);
            Target()->width = v;
        }
    } else {
        switch (e.tool) {
            case Tool::Brush: e.brushWidth = v; break;
            case Tool::Highlighter: e.highlighterWidth = v; break;
            case Tool::Eraser: e.eraserWidth = v; break;
            case Tool::Shape: e.shapeWidth = v; break;
            case Tool::Blur: e.blurBlock = v; break;
            default: break;
        }
    }
    RedrawEditor();
}

void SetTextSize(float size) {
    g_ed.textSize = size;
    if (EdObject* t = Target(); t && t->type == EdObject::Text && t->size != size) {
        Change();
        Target()->size = size;
    }
    RedrawEditor();
}

void ToggleStyle(int which) {
    Editor& e = g_ed;
    bool* def = which == 0 ? &e.bold : which == 1 ? &e.italic : &e.underline;
    EdObject* t = Target();
    if (t && t->type == EdObject::Text) {
        Change();
        t = Target();
        bool* v = which == 0 ? &t->bold : which == 1 ? &t->italic : &t->underline;
        *v = !*v;
        *def = *v;
    } else {
        *def = !*def;
    }
    RedrawEditor();
}

void SetShapeKind(ShapeKind shape) {
    g_ed.shape = shape;
    if (EdObject* t = Target(); t && t->type == EdObject::Shape && t->shape != shape) {
        Change();
        t = Target();
        t->shape = shape;
        t->b = Constrain(*t, t->a, t->b);
    }
    RedrawEditor();
}

void DeleteSelected() {
    Editor& e = g_ed;
    if (e.tool != Tool::Select || e.selected < 0 || e.selected >= (int)e.doc.objects.size()) {
        return;
    }
    Change();
    e.doc.objects.erase(e.doc.objects.begin() + e.selected);
    e.selected = -1;
    e.drag = EdDrag::None;
    RedrawEditor();
}

////////////////////////////////////////////////////////////////////////////////
// Drawing and erasing on the canvas

void AddStrokePoint(EdObject& o, D2D1_POINT_2F p) {
    D2D1_POINT_2F last = o.points.back();
    float d = Dist(last, p);
    if (d < 0.5f) {
        return;
    }
    // Dense points, so that the eraser cuts strokes where it passes.
    float step = std::max(1.f, o.width / 3);
    for (float t = step; t < d; t += step) {
        o.points.push_back({last.x + (p.x - last.x) * t / d, last.y + (p.y - last.y) * t / d});
    }
    o.points.push_back(p);
}

bool StrokeUnder(D2D1_POINT_2F ip) {
    float r = g_ed.eraserWidth / 2;
    for (const EdObject& o : g_ed.doc.objects) {
        if (o.type == EdObject::Stroke) {
            for (D2D1_POINT_2F p : o.points) {
                if (Dist(p, ip) <= r + o.width / 2) {
                    return true;
                }
            }
        }
    }
    return false;
}

// Cuts the strokes under the eraser into the parts that remain.
void EraseAt(D2D1_POINT_2F ip) {
    if (!StrokeUnder(ip)) {
        return;
    }
    Change(true);
    float r = g_ed.eraserWidth / 2;
    std::vector<EdObject> result;
    for (EdObject& o : g_ed.doc.objects) {
        if (o.type != EdObject::Stroke) {
            result.push_back(std::move(o));
            continue;
        }
        EdObject part = o;
        part.points.clear();
        for (D2D1_POINT_2F p : o.points) {
            if (Dist(p, ip) > r + o.width / 2) {
                part.points.push_back(p);
            } else if (!part.points.empty()) {
                result.push_back(part);
                part.points.clear();
            }
        }
        if (!part.points.empty()) {
            result.push_back(std::move(part));
        }
    }
    g_ed.doc.objects = std::move(result);
}

size_t CaretAt(const EdObject& o, D2D1_POINT_2F ip) {
    IDWriteTextLayout* layout = TextLayout(o);
    if (!layout) {
        return o.text.size();
    }
    BOOL trailing = FALSE, inside = FALSE;
    DWRITE_HIT_TEST_METRICS m{};
    layout->HitTestPoint(ip.x - o.a.x, ip.y - o.a.y, &trailing, &inside, &m);
    layout->Release();
    return std::min(o.text.size(), (size_t)m.textPosition + (trailing ? m.length : 0));
}

void StageDrag(Drag phase, D2D1_POINT_2F p) {
    Editor& e = g_ed;
    D2D1_POINT_2F ip = ToImage(p);
    auto& objects = e.doc.objects;
    if (phase == Drag::Start) {
        e.changeOpen = false;
        e.dragStart = ip;
        e.dragScreen = p;
        e.drag = EdDrag::None;
        CloseFlyout();
        if (e.space || e.viewOnly || e.pixels.empty()) {
            e.drag = EdDrag::Pan;
            e.panStart = e.pan;
            return;
        }
        // A click in the text being typed moves the caret.
        if (e.editing >= 0) {
            const EdObject& o = objects[e.editing];
            if (In(Inset(ObjectBounds(o), -4 / e.zoom, -4 / e.zoom), ip)) {
                e.caret = CaretAt(o, ip);
                e.caretOn = true;
                RedrawEditor();
                return;
            }
            FinishText();
        }
        switch (e.tool) {
            case Tool::Brush:
            case Tool::Highlighter: {
                Change(true);
                EdObject o;
                o.type = EdObject::Stroke;
                o.color = e.color;
                o.highlighter = e.tool == Tool::Highlighter;
                o.width = o.highlighter ? e.highlighterWidth : e.brushWidth;
                o.points = {ip};
                objects.push_back(std::move(o));
                e.drag = EdDrag::Draw;
                break;
            }
            case Tool::Eraser:
                e.drag = EdDrag::Erase;
                e.lastErase = ip;
                if (StrokeUnder(ip)) {
                    EraseAt(ip);
                } else if (int i = HitObject(ip, HitFilter::NoStrokes); i >= 0) {
                    Change(true);
                    objects.erase(objects.begin() + i);
                }
                break;
            case Tool::Text: {
                if (int i = HitObject(ip, HitFilter::Text); i >= 0) {
                    StartText(i, CaretAt(objects[i], ip));
                    break;
                }
                // Made empty and without an undo step: the first letter adds
                // one, and an empty text disappears.
                EdObject o;
                o.type = EdObject::Text;
                o.color = e.color;
                o.size = e.textSize;
                o.bold = e.bold;
                o.italic = e.italic;
                o.underline = e.underline;
                o.a = {ip.x, ip.y - e.textSize * 0.65f};
                objects.push_back(std::move(o));
                StartText((int)objects.size() - 1, 0);
                break;
            }
            case Tool::Shape:
            case Tool::Blur:
                e.drag = EdDrag::NewShape;
                e.created = false;
                break;
            case Tool::Select: {
                if (e.selected >= 0 && e.selected < (int)objects.size()) {
                    auto handles = Handles(objects[e.selected]);
                    for (int i = 0; i < (int)handles.size(); i++) {
                        if (Dist(ToScreen(handles[i]), p) <= 8) {
                            e.drag = EdDrag::Handle;
                            e.handle = i;
                            e.dragOrigin = objects[e.selected];
                            return;
                        }
                    }
                }
                e.selected = HitObject(ip);
                if (e.selected >= 0) {
                    e.drag = EdDrag::Move;
                    e.dragOrigin = objects[e.selected];
                } else {
                    e.drag = EdDrag::Pan;
                    e.panStart = e.pan;
                }
                break;
            }
            case Tool::Crop: {
                e.drag = EdDrag::Crop;
                e.cropOrigin = e.cropDraft;
                e.handle = -3;  // A new crop.
                bool corners = AspectRatio() > 0;
                for (int i = 0; i < 8; i++) {
                    if ((!corners || i % 2 == 0) &&
                        Dist(ToScreen(CropHandle(e.cropDraft, i)), p) <= 10) {
                        e.handle = i;
                        break;
                    }
                }
                if (e.handle == -3 && In(e.cropDraft, ip)) {
                    e.handle = -2;
                }
                break;
            }
        }
        RedrawEditor();
        return;
    }

    if (phase == Drag::Move) {
        switch (e.drag) {
            case EdDrag::Pan:
                e.pan = {e.panStart.x + p.x - e.dragScreen.x, e.panStart.y + p.y - e.dragScreen.y};
                e.fitted = false;
                ClampPan();
                break;
            case EdDrag::Draw:
                AddStrokePoint(objects.back(), ip);
                break;
            case EdDrag::Erase: {
                // Also between mouse samples, so that a quick move erases a line.
                float d = Dist(e.lastErase, ip);
                float step = std::max(1.f, e.eraserWidth / 4);
                for (float t = step; t < d; t += step) {
                    EraseAt({e.lastErase.x + (ip.x - e.lastErase.x) * t / d,
                             e.lastErase.y + (ip.y - e.lastErase.y) * t / d});
                }
                EraseAt(ip);
                e.lastErase = ip;
                break;
            }
            case EdDrag::NewShape: {
                if (!e.created) {
                    if (Dist(p, e.dragScreen) < 4) {
                        return;
                    }
                    Change(true);
                    EdObject o;
                    o.type = e.tool == Tool::Blur ? EdObject::Blur : EdObject::Shape;
                    o.color = e.color;
                    o.shape = e.shape;
                    o.width = e.tool == Tool::Blur ? e.blurBlock : e.shapeWidth;
                    o.a = e.dragStart;
                    objects.push_back(std::move(o));
                    e.created = true;
                }
                EdObject& o = objects.back();
                o.b = Constrain(o, o.a, ip);
                break;
            }
            case EdDrag::Move: {
                if (e.selected < 0 || e.selected >= (int)objects.size()) {
                    return;
                }
                D2D1_POINT_2F d{ip.x - e.dragStart.x, ip.y - e.dragStart.y};
                if (d.x == 0 && d.y == 0) {
                    return;
                }
                Change(true);
                objects[e.selected] = e.dragOrigin;
                Translate(objects[e.selected], d);
                break;
            }
            case EdDrag::Handle: {
                if (e.selected < 0 || e.selected >= (int)objects.size()) {
                    return;
                }
                Change(true);
                EdObject& o = objects[e.selected];
                o = e.dragOrigin;
                if (IsLine(o)) {
                    (e.handle == 0 ? o.a : o.b) = Constrain(o, e.handle == 0 ? o.b : o.a, ip);
                } else {
                    D2D1_POINT_2F fixed = Handles(e.dragOrigin)[(e.handle + 2) % 4];
                    o.a = fixed;
                    o.b = Constrain(o, fixed, ip);
                }
                break;
            }
            case EdDrag::Crop:
                e.cropDraft = e.handle == -3
                                  ? CropBetween(e.dragStart, ip)
                                  : CropFrom(e.cropOrigin, e.handle,
                                             {ip.x - e.dragStart.x, ip.y - e.dragStart.y});
                break;
            case EdDrag::None:
                return;
        }
        RedrawEditor();
        return;
    }

    // Drag::End
    if (e.drag == EdDrag::Crop && e.handle == -3 &&
        (e.cropDraft.right - e.cropDraft.left < 2 || e.cropDraft.bottom - e.cropDraft.top < 2)) {
        e.cropDraft = e.cropOrigin;  // A click, not a new crop.
    }
    e.drag = EdDrag::None;
    e.changeOpen = e.editing >= 0 && e.changeOpen;
    RedrawEditor();
}

void StageDoubleClick() {
    Editor& e = g_ed;
    if (e.viewOnly || e.tool != Tool::Select) {
        return;
    }
    D2D1_POINT_2F ip = ToImage(g_edView.mouse);
    if (int i = HitObject(ip, HitFilter::Text); i >= 0) {
        e.selected = i;
        StartText(i, CaretAt(e.doc.objects[i], ip));
    }
}

LPCWSTR StageCursor(D2D1_POINT_2F p) {
    Editor& e = g_ed;
    if (e.space || e.middlePan || e.drag == EdDrag::Pan || e.viewOnly) {
        return e.drag == EdDrag::Pan || e.middlePan ? IDC_SIZEALL : IDC_HAND;
    }
    D2D1_POINT_2F ip = ToImage(p);
    switch (e.tool) {
        case Tool::Text:
            return IDC_IBEAM;
        case Tool::Select:
            if (e.selected >= 0 && e.selected < (int)e.doc.objects.size()) {
                const EdObject& o = e.doc.objects[e.selected];
                auto handles = Handles(o);
                for (int i = 0; i < (int)handles.size(); i++) {
                    if (Dist(ToScreen(handles[i]), p) <= 8) {
                        return IsLine(o) ? IDC_SIZEALL : i % 2 ? IDC_SIZENESW : IDC_SIZENWSE;
                    }
                }
            }
            return HitObject(ip) >= 0 ? IDC_SIZEALL : IDC_ARROW;
        case Tool::Crop: {
            bool corners = AspectRatio() > 0;
            for (int i = 0; i < 8; i++) {
                if ((!corners || i % 2 == 0) &&
                    Dist(ToScreen(CropHandle(e.cropDraft, i)), p) <= 10) {
                    static const LPCWSTR kCursors[] = {IDC_SIZENWSE, IDC_SIZENS, IDC_SIZENESW,
                                                       IDC_SIZEWE,   IDC_SIZENWSE, IDC_SIZENS,
                                                       IDC_SIZENESW, IDC_SIZEWE};
                    return kCursors[i];
                }
            }
            return In(e.cropDraft, ip) ? IDC_SIZEALL : IDC_CROSS;
        }
        default:
            return IDC_CROSS;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Rendering

ID2D1Bitmap* EditorImage() {
    auto& cache = g_edView.bitmaps;
    if (auto found = cache.find(0); found != cache.end()) {
        return found->second;
    }
    if (g_ed.pixels.empty()) {
        return nullptr;
    }
    ID2D1Bitmap* bitmap = PixelBitmap(g_ed.w, g_ed.h, g_ed.pixels.data());
    if (bitmap) {
        cache[0] = bitmap;
    }
    return bitmap;
}

std::wstring Tip(const wchar_t* label, int shortcut) {
    UINT key = g_keys[shortcut];
    if (!key || (key & kKeyOff)) {
        return label;
    }
    return std::wstring(label) + L" (" + KeyName(key) + L")";
}

void DrawShapeIcon(ShapeKind kind, const D2D1_RECT_F& r, D2D1_COLOR_F c) {
    float cx = (r.left + r.right) / 2, cy = (r.top + r.bottom) / 2;
    switch (kind) {
        case ShapeKind::Arrow:
            Line(cx - 6, cy + 6, cx + 5, cy - 5, c, 1.5f);
            Line(cx + 6, cy - 6, cx + 6, cy, c, 1.5f);
            Line(cx + 6, cy - 6, cx, cy - 6, c, 1.5f);
            break;
        case ShapeKind::Line:
            Line(cx - 6, cy + 6, cx + 6, cy - 6, c, 1.5f);
            break;
        case ShapeKind::Circle:
            CircleStroke(cx, cy, 7, c, 1.5f);
            break;
        case ShapeKind::Square:
            Stroke({cx - 6.5f, cy - 6.5f, cx + 6.5f, cy + 6.5f}, c, 0, 1.5f);
            break;
        case ShapeKind::Rect:
            Stroke({cx - 8, cy - 5, cx + 8, cy + 5}, c, 0, 1.5f);
            break;
        case ShapeKind::Bubble:
            Stroke({cx - 8, cy - 7, cx + 8, cy + 4}, c, 3, 1.5f);
            Line(cx - 4, cy + 4, cx - 6, cy + 8, c, 1.5f);
            Line(cx - 6, cy + 8, cx, cy + 4, c, 1.5f);
            break;
    }
}

void DrawBlurIcon(const D2D1_RECT_F& r, D2D1_COLOR_F c) {
    float x0 = (r.left + r.right) / 2 - 7, y0 = (r.top + r.bottom) / 2 - 7;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Fill({x0 + i * 5, y0 + j * 5, x0 + i * 5 + 4, y0 + j * 5 + 4},
                 WithAlpha(c, (i + j) % 2 ? 0.35f : 1));
        }
    }
}

void DrawEditorTitle(float width) {
    const Theme& t = g_theme;
    Editor& e = g_ed;
    Add({0, 0, width, kEdTitleHeight}, HIT(0)).caption = true;
    Glyph(kIconImage, g_fonts.icon14, {14, 0, 32, kEdTitleHeight}, t.text);
    std::wstring title = e.viewOnly ? Tr(L"Podgląd obrazu", L"Image viewer")
                                    : Tr(L"Edytor grafiki", L"Image editor");
    WCHAR size[64];
    swprintf_s(size, L" · %u × %u", e.w, e.h);
    std::wstring detail = (e.source.empty() ? L"" : L" · " + e.source) + size;
    float x = 42, right = width - 3 * 46 - 8;
    float tw = TextWidth(title, g_fonts.title);
    Text(title, g_fonts.title, {x, 0, std::min(right, x + tw + 1), kEdTitleHeight}, t.text);
    Text(detail, g_fonts.title, {x + tw, 0, right, kEdTitleHeight}, t.text2);

    bool zoomed = IsZoomed(e.hwnd);
    float bx = width - 46;
    Hit close = HIT(0);
    D2D1_RECT_F cr{bx, 0, bx + 46, kEdTitleHeight};
    if (Hot(close)) {
        Fill(cr, Rgba(0xC42B1C, Pressed(close) ? 0.9f : 1));
    }
    Glyph(kIconClose, g_fonts.icon10, cr, Hot(close) ? Rgba(0xFFFFFF) : t.text);
    Add(cr, close).click = [] { CloseEditor(); };
    bx -= 46;
    IconButton({bx, 0, bx + 46, kEdTitleHeight}, HIT(0),
               zoomed ? kIconRestoreWindow : kIconMaximize,
               zoomed ? Tr(L"Przywróć w dół", L"Restore down") : Tr(L"Maksymalizuj", L"Maximize"),
               t.text, g_fonts.icon10)
        .click = [zoomed] { ShowWindow(g_ed.hwnd, zoomed ? SW_RESTORE : SW_MAXIMIZE); };
    bx -= 46;
    IconButton({bx, 0, bx + 46, kEdTitleHeight}, HIT(0), kIconMinimize,
               Tr(L"Minimalizuj", L"Minimize"), t.text, g_fonts.icon10)
        .click = [] { ShowWindow(g_ed.hwnd, SW_MINIMIZE); };
}

// A tool button with the active tool's accent bar; returns its right edge.
float ToolButton(float x, float y, Tool tool, int shortcut, wchar_t glyph) {
    const Theme& t = g_theme;
    D2D1_RECT_F r{x, y, x + 40, y + 36};
    Hit h = HIT((int)tool);
    bool on = g_ed.tool == tool;
    Fill(r, on ? (Hot(h) ? t.pressed : t.hover) : SubtleFill(h), 4);
    if (tool == Tool::Blur) {
        DrawBlurIcon(r, t.text);
    } else {
        Glyph(glyph, g_fonts.icon16, r, t.text);
    }
    if (on) {
        float cx = (r.left + r.right) / 2;
        Fill({cx - 7, r.bottom - 5, cx + 7, r.bottom - 2}, t.accent, 1.5f);
    }
    Region& region = Add(r, h);
    region.tip = Tip(kShortcuts[shortcut].label, shortcut);
    region.click = [tool] { SetTool(tool); };
    return r.right + 4;
}

float ToolDivider(float x, float cy) {
    Fill({x + 2, cy - 12, x + 3, cy + 12}, g_theme.divider);
    return x + 9;
}

void DrawToolbar(const D2D1_RECT_F& bar) {
    const Theme& t = g_theme;
    Editor& e = g_ed;
    float x = 12, y = bar.top + 6, cy = y + 18;
    x = ToolButton(x, y, Tool::Select, ScEdSelect, kIconPointer);
    x = ToolButton(x, y, Tool::Brush, ScEdBrush, kIconBrush);
    x = ToolButton(x, y, Tool::Highlighter, ScEdHighlighter, kIconHighlight);
    x = ToolButton(x, y, Tool::Eraser, ScEdEraser, kIconEraser);
    x = ToolDivider(x, cy);
    x = ToolButton(x, y, Tool::Text, ScEdText, kIconFont);
    {
        // Shapes: the current shape with a menu of the others.
        D2D1_RECT_F r{x, y, x + 52, y + 36};
        Hit h = HIT(0);
        bool on = e.tool == Tool::Shape;
        Fill(r, on ? (Hot(h) ? t.pressed : t.hover) : SubtleFill(h), 4);
        DrawShapeIcon(e.shape, {r.left + 6, r.top, r.left + 32, r.bottom}, t.text);
        Glyph(kIconChevronDown, g_fonts.icon10, {r.left + 30, r.top, r.right - 4, r.bottom},
              t.text2);
        if (on) {
            float cx = (r.left + r.right) / 2;
            Fill({cx - 7, r.bottom - 5, cx + 7, r.bottom - 2}, t.accent, 1.5f);
        }
        Region& region = Add(r, h);
        region.tip = Tip(Tr(L"Kształty", L"Shapes"), ScEdShape);
        region.click = [r, h] {
            if (g_ed.tool != Tool::Shape) {
                SetTool(Tool::Shape);
                return;
            }
            if (ToggleFlyout(h)) {
                return;
            }
            std::vector<MenuItem> items;
            for (int i = 0; i < 6; i++) {
                MenuItem item;
                item.label = kShapeNames[i];
                item.checked = (int)g_ed.shape == i;
                item.action = [i] { SetShapeKind((ShapeKind)i); };
                items.push_back(std::move(item));
            }
            OpenMenu(r, h, std::move(items));
        };
        x = r.right + 4;
    }
    x = ToolDivider(x, cy);
    x = ToolButton(x, y, Tool::Blur, ScEdBlur, 0);
    x = ToolButton(x, y, Tool::Crop, ScEdCrop, kIconCrop);
    x = ToolDivider(x, cy);
    bool canUndo = !e.undo.empty() || e.editing >= 0, canRedo = !e.redo.empty();
    Region& undo = IconButton({x, y, x + 40, y + 36}, HIT(0), kIconUndo, nullptr,
                              canUndo ? t.text : t.text3);
    undo.tip = Tip(Tr(L"Cofnij", L"Undo"), ScEdUndo);
    if (canUndo) {
        undo.click = [] { Undo(false); };
    }
    x += 44;
    Region& redo = IconButton({x, y, x + 40, y + 36}, HIT(0), kIconRedo, nullptr,
                              canRedo ? t.text : t.text3);
    redo.tip = Tip(Tr(L"Ponów", L"Redo"), ScEdRedo);
    if (canRedo) {
        redo.click = [] { Undo(true); };
    }
}

float OptLabel(float x, float cy, const std::wstring& s) {
    float w = TextWidth(s, g_fonts.row);
    Text(s, g_fonts.row, {x, cy - 15, x + w + 1, cy + 15}, g_theme.text2);
    return x + w + 10;
}

float OptDivider(float x, float cy) {
    Fill({x, cy - 10, x + 1, cy + 10}, g_theme.divider);
    return x + 13;
}

float ColorSwatches(float x, float cy, UINT32 current) {
    const Theme& t = g_theme;
    x = OptLabel(x, cy, Tr(L"Kolor", L"Color"));
    for (int i = 0; i < (int)ARRAYSIZE(kEdColors); i++) {
        float sx = x + 12 + i * 28;
        Hit h = HIT(i);
        if (current == kEdColors[i]) {
            CircleStroke(sx, cy, 14, t.accent, 2);
        } else if (Hot(h)) {
            CircleStroke(sx, cy, 13, t.strong);
        }
        Circle(sx, cy, 10, Rgba(kEdColors[i]));
        CircleStroke(sx, cy, 10, t.dark ? Rgba(0xFFFFFF, 0.3f) : Rgba(0, 0.2f));
        Region& region = Add({sx - 14, cy - 14, sx + 14, cy + 14}, h);
        region.tip = kEdColorNames[i];
        region.click = [c = kEdColors[i]] { SetEdColor(c); };
    }
    return x + ARRAYSIZE(kEdColors) * 28 + 6;
}

float WidthSlider(float x, float cy, const wchar_t* label, float value, float lo, float hi) {
    const Theme& t = g_theme;
    x = OptLabel(x, cy, label);
    D2D1_RECT_F r{x - 10, cy - 12, x + 110, cy + 12};
    Region& region = Slider(r, HIT(0), (value - lo) / (hi - lo), [lo, hi](float v) {
        SetEdWidth(std::round(lo + v * (hi - lo)));
    });
    // One undo step for the whole drag.
    auto inner = region.drag;
    if (inner) {
        region.drag = [inner](Drag d, D2D1_POINT_2F p) {
            if (d == Drag::Start) {
                g_ed.changeOpen = false;
            }
            inner(d, p);
            if (d == Drag::End) {
                g_ed.changeOpen = false;
            }
        };
    }
    x += 108;
    WCHAR text[16];
    swprintf_s(text, L"%d px", (int)value);
    Text(text, g_fonts.row, {x, cy - 15, x + 56, cy + 15}, t.text);
    return x + 56;
}

void OptHint(float x, float cy, float right, const wchar_t* text) {
    if (right - x > 40) {
        Text(text, g_fonts.small, {x, cy - 15, right, cy + 15}, g_theme.text2);
    }
}

void DrawOptions(const D2D1_RECT_F& bar) {
    const Theme& t = g_theme;
    Editor& e = g_ed;
    Fill({bar.left, bar.top, bar.right, bar.top + 1}, t.divider);
    Fill({bar.left, bar.bottom - 1, bar.right, bar.bottom}, t.divider);
    float cy = (bar.top + bar.bottom) / 2, x = 16, right = bar.right - 16;
    EdObject* target = Target();
    Tool kind = e.tool;
    if (target) {
        kind = target->type == EdObject::Text    ? Tool::Text
               : target->type == EdObject::Shape ? Tool::Shape
               : target->type == EdObject::Blur  ? Tool::Blur
               : target->highlighter             ? Tool::Highlighter
                                                 : Tool::Brush;
    }
    if (target && e.editing < 0) {
        // The selected object can go away from here too.
        float w = ButtonWidth(Tr(L"Usuń", L"Delete"), kIconDelete);
        Button({right - w, cy - 15, right, cy + 15}, HIT(0), Tr(L"Usuń", L"Delete"), kIconDelete)
            .click = [] { DeleteSelected(); };
        right -= w + 12;
    }
    switch (kind) {
        case Tool::Select:
            OptHint(x, cy, right,
                    Tr(L"Kliknij rysunek, tekst, kształt lub zamazany obszar, aby go "
                       L"przesunąć. Dwuklik na tekście go edytuje.",
                       L"Click a drawing, text, shape or blurred area to move it. Double-click "
                       L"text to edit it."));
            break;
        case Tool::Brush:
            x = ColorSwatches(x, cy, target ? target->color : e.color);
            x = OptDivider(x, cy);
            WidthSlider(x, cy, Tr(L"Grubość", L"Width"), target ? target->width : e.brushWidth, 1,
                        40);
            break;
        case Tool::Highlighter:
            x = ColorSwatches(x, cy, target ? target->color : e.color);
            x = OptDivider(x, cy);
            WidthSlider(x, cy, Tr(L"Grubość", L"Width"),
                        target ? target->width : e.highlighterWidth, 4, 80);
            break;
        case Tool::Eraser:
            x = WidthSlider(x, cy, Tr(L"Rozmiar", L"Size"), e.eraserWidth, 4, 120);
            OptHint(
                x + 8, cy, right,
                Tr(L"Wymazuje pędzel i zakreślacz. Kliknięty tekst lub kształt usuwa w całości.",
                   L"Erases brush and highlighter strokes. Click text or a shape to delete it "
                   L"whole."));
            break;
        case Tool::Text: {
            float size = target ? target->size : e.textSize;
            bool styles[3] = {target ? target->bold : e.bold, target ? target->italic : e.italic,
                              target ? target->underline : e.underline};
            x = OptLabel(x, cy, Tr(L"Rozmiar", L"Size"));
            std::vector<std::wstring> sizes;
            int selected = 0;
            for (int i = 0; i < (int)ARRAYSIZE(kEdTextSizes); i++) {
                sizes.push_back(std::to_wstring((int)kEdTextSizes[i]));
                if (std::fabs(kEdTextSizes[i] - size) <
                    std::fabs(kEdTextSizes[selected] - size)) {
                    selected = i;
                }
            }
            Combo({x, cy - 15, x + 72, cy + 15}, HIT(0), sizes, selected,
                  [](int i) { SetTextSize(kEdTextSizes[i]); });
            x = OptDivider(x + 84, cy);
            static const wchar_t kGlyphs[] = {kIconBold, kIconItalic, kIconUnderline};
            static const int kShortcutIds[] = {ScEdBold, ScEdItalic, ScEdUnderline};
            for (int k = 0; k < 3; k++) {
                D2D1_RECT_F b{x, cy - 15, x + 32, cy + 15};
                Hit h = HIT(k);
                Fill(b, styles[k] ? WithAlpha(t.accent, Hot(h) ? 0.28f : 0.2f) : SubtleFill(h), 4);
                Glyph(kGlyphs[k], g_fonts.icon14, b, t.text);
                Region& region = Add(b, h);
                region.tip = Tip(kShortcuts[kShortcutIds[k]].label, kShortcutIds[k]);
                region.click = [k] { ToggleStyle(k); };
                x += 34;
            }
            x = OptDivider(x + 6, cy);
            ColorSwatches(x, cy, target ? target->color : e.color);
            break;
        }
        case Tool::Shape: {
            ShapeKind current = target ? target->shape : e.shape;
            for (int i = 0; i < 6; i++) {
                D2D1_RECT_F b{x, cy - 15, x + 32, cy + 15};
                Hit h = HIT(i);
                bool on = (int)current == i;
                Fill(b, on ? WithAlpha(t.accent, Hot(h) ? 0.28f : 0.2f) : SubtleFill(h), 4);
                DrawShapeIcon((ShapeKind)i, b, t.text);
                Region& region = Add(b, h);
                region.tip = kShapeNames[i];
                region.click = [i] { SetShapeKind((ShapeKind)i); };
                x += 34;
            }
            x = OptDivider(x + 6, cy);
            x = ColorSwatches(x, cy, target ? target->color : e.color);
            x = OptDivider(x, cy);
            WidthSlider(x, cy, Tr(L"Grubość", L"Width"), target ? target->width : e.shapeWidth, 1,
                        40);
            break;
        }
        case Tool::Blur:
            x = WidthSlider(x, cy, Tr(L"Siła", L"Strength"), target ? target->width : e.blurBlock,
                            4, 64);
            if (!target) {
                OptHint(x + 8, cy, right,
                        Tr(L"Zaznacz prostokątem dane, które chcesz ukryć.",
                           L"Drag a rectangle over the data you want to hide."));
            }
            break;
        case Tool::Crop: {
            x = OptLabel(x, cy, Tr(L"Proporcje", L"Aspect ratio"));
            Combo({x, cy - 15, x + 140, cy + 15}, HIT(0),
                  std::vector<std::wstring>(std::begin(kAspectNames), std::end(kAspectNames)),
                  e.aspect, [](int i) { SetAspect(i); });
            x += 152;
            x = OptLabel(x, cy, Tr(L"Wycinek:", L"Selection:"));
            WCHAR size[64];
            swprintf_s(size, L"%ld × %ld px", std::lround(e.cropDraft.right - e.cropDraft.left),
                       std::lround(e.cropDraft.bottom - e.cropDraft.top));
            Text(size, g_fonts.row, {x - 6, cy - 15, x + 140, cy + 15}, t.text);
            float applyWidth = ButtonWidth(Tr(L"Zastosuj (Enter)", L"Apply (Enter)"));
            float cancelWidth = ButtonWidth(Tr(L"Anuluj (Esc)", L"Cancel (Esc)"));
            float bx = right - applyWidth;
            Button({bx, cy - 15, right, cy + 15}, HIT(0), Tr(L"Zastosuj (Enter)", L"Apply (Enter)"),
                   0, kAccent)
                .click = [] { ApplyCrop(); };
            bx -= 8 + cancelWidth;
            Button({bx, cy - 15, bx + cancelWidth, cy + 15}, HIT(0),
                   Tr(L"Anuluj (Esc)", L"Cancel (Esc)"))
                .click = [] { CancelCrop(); };
            break;
        }
    }
}

void DrawSelection(const EdObject& o, bool handles) {
    const Theme& t = g_theme;
    D2D1_RECT_F r = Inset(ToScreenRect(ObjectBounds(o)), -3, -3);
    if (!IsLine(o)) {
        g_brush->SetColor(t.accent);
        g_rt->DrawRectangle(r, g_brush, 1, DashStyle());
    }
    if (handles) {
        for (D2D1_POINT_2F h : Handles(o)) {
            D2D1_POINT_2F s = ToScreen(h);
            D2D1_RECT_F box{s.x - 5, s.y - 5, s.x + 5, s.y + 5};
            Fill(box, Rgba(0xFFFFFF), 2);
            Stroke(box, t.accent, 2, 1.5f);
        }
    }
}

void DrawCaret(const EdObject& o) {
    IDWriteTextLayout* layout = TextLayout(o);
    if (!layout) {
        return;
    }
    FLOAT x = 0, y = 0;
    DWRITE_HIT_TEST_METRICS m{};
    layout->HitTestTextPosition((UINT32)g_ed.caret, FALSE, &x, &y, &m);
    layout->Release();
    D2D1_POINT_2F top = ToScreen({o.a.x + x, o.a.y + m.top});
    float height = std::max(m.height, o.size * 1.2f) * g_ed.zoom;
    Fill({top.x - 0.75f, top.y, top.x + 0.75f, top.y + height}, Rgba(o.color));
    Fill({top.x + 0.75f, top.y, top.x + 1.75f, top.y + height},
         Rgba(o.color == 0x1A1A1A ? 0xFFFFFF : 0x000000, 0.5f));
}

void DrawCropOverlay(const D2D1_RECT_F& image) {
    Editor& e = g_ed;
    D2D1_RECT_F c = ToScreenRect(e.cropDraft);
    D2D1_COLOR_F shade = Rgba(0, 0.55f);
    Fill({image.left, image.top, image.right, c.top}, shade);
    Fill({image.left, c.bottom, image.right, image.bottom}, shade);
    Fill({image.left, c.top, c.left, c.bottom}, shade);
    Fill({c.right, c.top, image.right, c.bottom}, shade);
    for (int i = 1; i <= 2; i++) {
        float x = c.left + (c.right - c.left) * i / 3, y = c.top + (c.bottom - c.top) * i / 3;
        Line(x, c.top, x, c.bottom, Rgba(0xFFFFFF, 0.35f));
        Line(c.left, y, c.right, y, Rgba(0xFFFFFF, 0.35f));
    }
    Stroke(Inset(c, -1, -1), Rgba(0xFFFFFF), 0, 1);
    bool corners = AspectRatio() > 0;
    for (int i = 0; i < 8; i++) {
        if (corners && i % 2) {
            continue;
        }
        D2D1_POINT_2F p = ToScreen(CropHandle(e.cropDraft, i));
        D2D1_RECT_F box{p.x - 5, p.y - 5, p.x + 5, p.y + 5};
        Fill(box, Rgba(0xFFFFFF), 2);
        Stroke(box, Rgba(0, 0.5f), 2);
    }
}

void DrawStage(const D2D1_RECT_F& stage) {
    const Theme& t = g_theme;
    Editor& e = g_ed;
    Fill(stage, t.dark ? Rgba(0x1C1C1C) : Rgba(0xE6E6E6));
    PushClip(stage);
    Hit h = HIT(0);
    e.stageHit = h;
    Region& region = Add(stage, h);
    region.drag = StageDrag;
    region.dblclick = StageDoubleClick;
    ID2D1Bitmap* image = EditorImage();
    if (!image) {
        const wchar_t* text = e.loading ? Tr(L"Wczytuję obraz…", L"Loading the image…")
                              : !e.error.empty() ? e.error.c_str()
                                                 : Tr(L"Ten obraz jest za duży, by go tu pokazać.",
                                                      L"This image is too large to show here.");
        Text(text, g_fonts.empty, stage, t.text2);
        PopClip();
        return;
    }
    D2D1_RECT_F view = EdRef();
    D2D1_RECT_F shown = ToScreenRect(view);
    for (int i = 1; i <= 4; i++) {
        Fill({shown.left - i, shown.top - i + 2, shown.right + i, shown.bottom + i + 2},
             Rgba(0, 0.06f), (float)i);
    }
    D2D1_POINT_2F o = EdOrigin();
    g_rt->SetTransform(D2D1::Matrix3x2F::Scale(e.zoom, e.zoom) *
                       D2D1::Matrix3x2F::Translation(o.x, o.y));
    g_rt->PushAxisAlignedClip(view, D2D1_ANTIALIAS_MODE_ALIASED);
    // Smooth when scaled down, sharp pixels when zoomed in.
    D2D1_RECT_F full = FullImage();
    ID2D1DeviceContext* dc = nullptr;
    if (e.zoom / EdActual() >= 2) {
        g_rt->DrawBitmap(image, full, 1, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
    } else if (SUCCEEDED(g_rt->QueryInterface(IID_PPV_ARGS(&dc)))) {
        dc->DrawBitmap(image, &full, 1, D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC, nullptr);
        dc->Release();
    } else {
        g_rt->DrawBitmap(image, full, 1, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
    }
    DrawEdObjects(g_rt, g_brush, true);
    g_rt->PopAxisAlignedClip();
    g_rt->SetTransform(D2D1::Matrix3x2F::Identity());

    if (!e.viewOnly) {
        if (e.tool == Tool::Crop) {
            DrawCropOverlay(shown);
        } else if (e.editing >= 0 && e.editing < (int)e.doc.objects.size()) {
            const EdObject& text = e.doc.objects[e.editing];
            DrawSelection(text, false);
            if (e.caretOn) {
                DrawCaret(text);
            }
        } else if (EdObject* target = Target()) {
            DrawSelection(*target, true);
        }
        // The size of the brush or eraser at the mouse.
        bool sized = e.tool == Tool::Brush || e.tool == Tool::Highlighter ||
                     e.tool == Tool::Eraser;
        if (sized && g_edView.mouseInside && In(stage, g_edView.mouse) && !e.space) {
            float w = e.tool == Tool::Brush         ? e.brushWidth
                      : e.tool == Tool::Highlighter ? e.highlighterWidth
                                                    : e.eraserWidth;
            float radius = std::max(2.f, w * e.zoom / 2);
            CircleStroke(g_edView.mouse.x, g_edView.mouse.y, radius + 1, Rgba(0, 0.6f));
            CircleStroke(g_edView.mouse.x, g_edView.mouse.y, radius, Rgba(0xFFFFFF, 0.9f));
        }
    }
    PopClip();
}

void DrawEditorFooter(const D2D1_RECT_F& r) {
    const Theme& t = g_theme;
    Editor& e = g_ed;
    Fill({r.left, r.top, r.right, r.top + 1}, t.divider);
    float cy = (r.top + r.bottom) / 2, x = 12;
    Region& out = IconButton({x, cy - 16, x + 32, cy + 16}, HIT(0), kIconZoomOut, nullptr);
    out.tip = Tip(Tr(L"Pomniejsz", L"Zoom out"), ScEdZoomOut);
    out.click = [] { EdZoomStep(-1); };
    x += 32;
    Text(std::to_wstring(ZoomPercent()) + L" %", g_fonts.smallCenter, {x, cy - 16, x + 52, cy + 16},
         t.text);
    x += 52;
    Region& in = IconButton({x, cy - 16, x + 32, cy + 16}, HIT(0), kIconZoomIn, nullptr);
    in.tip = Tip(Tr(L"Powiększ", L"Zoom in"), ScEdZoomIn);
    in.click = [] { EdZoomStep(1); };
    x += 40;
    float fw = ButtonWidth(Tr(L"Dopasuj", L"Fit"));
    if (e.fitted) {
        Fill({x, cy - 16, x + fw, cy + 16}, t.hover, 4);
    }
    Region& fit = Button({x, cy - 16, x + fw, cy + 16}, HIT(0), Tr(L"Dopasuj", L"Fit"), 0, kSubtle);
    fit.tip = Tip(Tr(L"Dopasuj do okna", L"Fit to window"), ScEdFit);
    fit.click = [] { EdFit(); };
    x += fw + 4;
    float aw = ButtonWidth(L"100 %");
    if (ZoomPercent() == 100) {
        Fill({x, cy - 16, x + aw, cy + 16}, t.hover, 4);
    }
    Region& actual = Button({x, cy - 16, x + aw, cy + 16}, HIT(0), L"100 %", 0, kSubtle);
    actual.tip = Tip(Tr(L"Rozmiar 100%", L"Actual size"), ScEdActual);
    actual.click = [] { EdSetZoom(EdActual(), StageCenter()); };
    x += aw + 16;

    float right = r.right - 16;
    bool ready = !e.pixels.empty();
    if (e.viewOnly) {
        float cw = ButtonWidth(Tr(L"Zamknij", L"Close"));
        Button({right - cw, cy - 16, right, cy + 16}, HIT(0), Tr(L"Zamknij", L"Close")).click = [] {
            CloseEditor();
        };
        right -= cw + 8;
        float ew = ButtonWidth(Tr(L"Edytuj", L"Edit"), kIconEdit);
        Button({right - ew, cy - 16, right, cy + 16}, HIT(0), Tr(L"Edytuj", L"Edit"), kIconEdit,
               kAccent, !ready)
            .click = [] {
            g_ed.viewOnly = false;
            EdFit();
        };
        right -= ew + 16;
    } else {
        bool canSave = ready && Unsaved();
        const wchar_t* save = Tr(L"Zapisz jako nowy", L"Save as new");
        float sw = ButtonWidth(save);
        Region& saveButton = Button({right - sw, cy - 16, right, cy + 16}, HIT(0), save, 0,
                                    kAccent, !canSave);
        saveButton.tip = Tip(Tr(L"Zapisz jako nowy obraz", L"Save as a new image"), ScEdSave);
        saveButton.click = [] { SaveEditor(false); };
        right -= sw + 8;
        float cw = ButtonWidth(Tr(L"Zapisz i kopiuj", L"Save and copy"));
        Button({right - cw, cy - 16, right, cy + 16}, HIT(0),
               Tr(L"Zapisz i kopiuj", L"Save and copy"), 0, kStandard, !canSave)
            .click = [] { SaveEditor(true); };
        right -= cw + 8;
        float xw = ButtonWidth(Tr(L"Anuluj", L"Cancel"));
        Button({right - xw, cy - 16, right, cy + 16}, HIT(0), Tr(L"Anuluj", L"Cancel")).click = [] {
            CloseEditor();
        };
        right -= xw + 16;
    }
    if (right - x > 40) {
        std::wstring info;
        D2D1_COLOR_F color = t.text2;
        if (!e.message.empty()) {
            info = e.message;
            color = t.accentText;
        } else if (ready) {
            D2D1_RECT_F c = e.tool == Tool::Crop && !e.viewOnly ? e.cropDraft : e.doc.crop;
            WCHAR size[64];
            swprintf_s(size, L"%ld × %ld px", std::lround(c.right - c.left),
                       std::lround(c.bottom - c.top));
            info = size;
            if (!e.viewOnly) {
                info += Tr(L" · zapis jako nowy obraz, oryginał bez zmian",
                           L" · saves as a new image, the original stays as it is");
            }
        }
        Text(info, g_fonts.small, {x, cy - 16, right, cy + 16}, color);
    }
}

void RenderEditor() {
    const Theme& t = g_theme;
    Editor& e = g_ed;
    g_blurUsed.clear();
    D2D1_SIZE_F s = SizeDip(g_edView);
    if (!t.backdrop) {
        Fill({0, 0, s.width, s.height}, t.window);
    }
    DrawEditorTitle(s.width);
    float y = kEdTitleHeight;
    if (!e.viewOnly) {
        DrawToolbar({0, y, s.width, y + kEdToolbarHeight});
        y += kEdToolbarHeight;
        DrawOptions({0, y, s.width, y + kEdOptionsHeight});
    }
    DrawStage(EdStageRect());
    DrawEditorFooter({0, s.height - kEdFooterHeight, s.width, s.height});
    DrawDialog();
    DrawFlyout();
    DrawTooltip();
    // Blurred areas that moved or went away.
    auto& cache = g_edView.bitmaps;
    for (auto it = cache.begin(); it != cache.end();) {
        if ((it->first & kBlurKey) && !g_blurUsed.count(it->first)) {
            it->second->Release();
            it = cache.erase(it);
        } else {
            ++it;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Saving: everything rendered at the original resolution, as a new item.

bool RenderPng(std::string* png) {
    Editor& e = g_ed;
    D2D1_RECT_F c = e.doc.crop;
    UINT cw = (UINT)std::max(1L, std::lround(c.right - c.left));
    UINT ch = (UINT)std::max(1L, std::lround(c.bottom - c.top));
    IWICBitmap* target = nullptr;
    ID2D1RenderTarget* rt = nullptr;
    ID2D1SolidColorBrush* brush = nullptr;
    ID2D1Bitmap* image = nullptr;
    auto format = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
    bool ok = SUCCEEDED(g_wic->CreateBitmap(cw, ch, GUID_WICPixelFormat32bppPBGRA,
                                            WICBitmapCacheOnLoad, &target)) &&
              SUCCEEDED(g_d2d->CreateWicBitmapRenderTarget(
                  target,
                  D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, format, 96, 96),
                  &rt)) &&
              SUCCEEDED(rt->CreateSolidColorBrush(D2D1::ColorF(0), &brush)) &&
              SUCCEEDED(rt->CreateBitmap(D2D1::SizeU(e.w, e.h), e.pixels.data(), e.w * 4,
                                         D2D1::BitmapProperties(format), &image));
    if (ok) {
        rt->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
        rt->BeginDraw();
        rt->Clear(D2D1::ColorF(0, 0));
        rt->SetTransform(D2D1::Matrix3x2F::Translation(-c.left, -c.top));
        rt->DrawBitmap(image, FullImage(), 1, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
        DrawEdObjects(rt, brush, false);
        ok = SUCCEEDED(rt->EndDraw());
    }
    SafeRelease(image);
    SafeRelease(brush);
    SafeRelease(rt);
    std::string pixels;
    ok = ok && ToPixels(g_wic, target, cw, ch, GUID_WICPixelFormat32bppBGRA, &pixels) &&
         EncodePixelsPng(g_wic, cw, ch, pixels, GUID_WICPixelFormat32bppBGRA, png);
    SafeRelease(target);
    return ok;
}

bool SaveEditor(bool copy) {
    Editor& e = g_ed;
    FinishText();
    if (e.pixels.empty()) {
        return false;
    }
    std::string png;
    if (!RenderPng(&png)) {
        e.message = Tr(L"Nie udało się zapisać obrazu.", L"Couldn't save the image.");
        RedrawEditor();
        return false;
    }
    auto* c = new Capture;
    c->time = NowFileTime();
    c->png = std::move(png);
    auto* r = new Captured;
    r->item.source = Tr(L"Edytor grafik", L"Image editor");
    r->kind = e.kind == Kind::Pinned ? Kind::Pinned : Kind::History;
    r->category = e.category;
    r->copy = copy;
    Enqueue([c, r] { ProcessCapture(c, r); });
    e.savedVersion = e.doc.version;
    std::wstring where =
        r->kind == Kind::Pinned
            ? Tr(L"Zapisano nowy obraz w przypiętych.", L"Saved a new image to Pinned.")
            : Tr(L"Zapisano nowy obraz na górze historii.",
                 L"Saved a new image at the top of the history.");
    e.message = copy ? where + Tr(L" Jest też w schowku.", L" It's also on the clipboard.") : where;
    RedrawEditor();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Keyboard

void InsertText(std::wstring_view s) {
    Editor& e = g_ed;
    if (s.empty() || e.editing < 0) {
        return;
    }
    Change(true);
    std::wstring& text = e.doc.objects[e.editing].text;
    text.insert(e.caret, s);
    e.caret += s.size();
}

void PasteText() {
    if (!OpenClipboardRetry()) {
        return;
    }
    std::wstring text;
    if (HANDLE h = GetClipboardData(CF_UNICODETEXT)) {
        if (auto* p = (const wchar_t*)GlobalLock(h)) {
            text = p;
            GlobalUnlock(h);
        }
    }
    CloseClipboard();
    std::erase(text, L'\r');
    InsertText(text);
}

bool TextKey(UINT vk) {
    Editor& e = g_ed;
    std::wstring& s = e.doc.objects[e.editing].text;
    bool ctrl = GetKeyState(VK_CONTROL) < 0;
    auto prev = [&](size_t i) {
        if (i > 0) {
            i--;
            if (i > 0 && IS_LOW_SURROGATE(s[i]) && IS_HIGH_SURROGATE(s[i - 1])) {
                i--;
            }
        }
        return i;
    };
    auto next = [&](size_t i) {
        if (i < s.size()) {
            i++;
            if (i < s.size() && IS_LOW_SURROGATE(s[i])) {
                i++;
            }
        }
        return i;
    };
    switch (vk) {
        case VK_LEFT:
            e.caret = prev(e.caret);
            break;
        case VK_RIGHT:
            e.caret = next(e.caret);
            break;
        case VK_HOME: {
            size_t nl = e.caret ? s.rfind(L'\n', e.caret - 1) : std::wstring::npos;
            e.caret = nl == std::wstring::npos ? 0 : nl + 1;
            break;
        }
        case VK_END: {
            size_t nl = s.find(L'\n', e.caret);
            e.caret = nl == std::wstring::npos ? s.size() : nl;
            break;
        }
        case VK_BACK:
            if (e.caret) {
                size_t p = prev(e.caret);
                Change(true);
                s.erase(p, e.caret - p);
                e.caret = p;
            }
            break;
        case VK_DELETE:
            if (e.caret < s.size()) {
                Change(true);
                s.erase(e.caret, next(e.caret) - e.caret);
            }
            break;
        case VK_ESCAPE:
            FinishText();
            return true;
        case VK_UP:
        case VK_DOWN:
        case VK_TAB:
            break;
        case 'V':
            if (!ctrl) {
                return false;
            }
            PasteText();
            break;
        default:
            return false;
    }
    e.caretOn = true;
    SetTimer(e.hwnd, kTimerEditorCaret, GetCaretBlinkTime(), nullptr);
    RedrawEditor();
    return true;
}

void EditorChar(wchar_t ch) {
    Editor& e = g_ed;
    if (e.editing < 0) {
        return;
    }
    if (ch == L'\n') {
        FinishText();  // Ctrl+Enter.
        return;
    }
    if (ch == L'\r') {
        ch = L'\n';
    } else if (ch < 32 || ch == 0x7F) {
        return;
    }
    InsertText(std::wstring_view(&ch, 1));
    e.caretOn = true;
    SetTimer(e.hwnd, kTimerEditorCaret, GetCaretBlinkTime(), nullptr);
    RedrawEditor();
}

bool EditorKey(UINT vk) {
    Editor& e = g_ed;
    UseSurface use(g_edView);
    if (FlyoutKey(vk)) {
        return true;
    }
    Dialog& d = g_edView.dialog;
    if (d.open) {
        if (vk == VK_RETURN && d.onEnter) {
            auto fn = d.onEnter;
            fn();
            return true;
        }
        if (vk == VK_ESCAPE) {
            auto fn = d.onEscape;
            fn ? fn() : CloseDialog();
            return true;
        }
        return false;
    }
    UINT mods = CurrentMods();
    if (e.editing >= 0) {
        if (TextKey(vk)) {
            return true;
        }
        if (!(mods & (MOD_CONTROL | MOD_ALT))) {
            return false;  // Letters type, through WM_CHAR.
        }
    }
    if (!e.viewOnly && e.tool == Tool::Crop && !mods) {
        if (vk == VK_RETURN) {
            ApplyCrop();
            return true;
        }
        if (vk == VK_ESCAPE) {
            CancelCrop();
            return true;
        }
    }
    if (vk == VK_SPACE && !mods && e.editing < 0) {
        e.space = true;
        return true;
    }
    if ((mods & MOD_CONTROL) && (vk == VK_ADD || vk == VK_SUBTRACT)) {
        EdZoomStep(vk == VK_ADD ? 1 : -1);
        return true;
    }
    int shortcut = MatchShortcut(vk, kScopeEditor);
    switch (shortcut) {
        case ScEdZoomIn:
            EdZoomStep(1);
            return true;
        case ScEdZoomOut:
            EdZoomStep(-1);
            return true;
        case ScEdFit:
            EdFit();
            return true;
        case ScEdActual:
            EdSetZoom(EdActual(), StageCenter());
            return true;
    }
    if (vk == VK_ESCAPE && !mods) {
        if (e.selected >= 0 && !e.viewOnly) {
            e.selected = -1;
            RedrawEditor();
        } else {
            CloseEditor();
        }
        return true;
    }
    if (e.viewOnly || e.pixels.empty()) {
        return false;
    }
    switch (shortcut) {
        case ScEdSelect: SetTool(Tool::Select); return true;
        case ScEdBrush: SetTool(Tool::Brush); return true;
        case ScEdHighlighter: SetTool(Tool::Highlighter); return true;
        case ScEdEraser: SetTool(Tool::Eraser); return true;
        case ScEdText: SetTool(Tool::Text); return true;
        case ScEdShape: SetTool(Tool::Shape); return true;
        case ScEdBlur: SetTool(Tool::Blur); return true;
        case ScEdCrop: SetTool(Tool::Crop); return true;
        case ScEdUndo: Undo(false); return true;
        case ScEdRedo: Undo(true); return true;
        case ScEdSave:
            if (Unsaved()) {
                SaveEditor(false);
            }
            return true;
        case ScEdBold: ToggleStyle(0); return true;
        case ScEdItalic: ToggleStyle(1); return true;
        case ScEdUnderline: ToggleStyle(2); return true;
        case ScEdDelete: DeleteSelected(); return true;
    }
    // Arrows nudge the selected object; Shift moves it further.
    if (e.tool == Tool::Select && e.selected >= 0 &&
        (vk == VK_LEFT || vk == VK_RIGHT || vk == VK_UP || vk == VK_DOWN)) {
        float step = (mods & MOD_SHIFT) ? 10 : 1;
        float dx = vk == VK_LEFT ? -step : vk == VK_RIGHT ? step : 0;
        float dy = vk == VK_UP ? -step : vk == VK_DOWN ? step : 0;
        Change();
        Translate(e.doc.objects[e.selected], {dx, dy});
        RedrawEditor();
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
// Window

void DestroyEditorWindow() {
    if (g_ed.hwnd) {
        DestroyWindow(g_ed.hwnd);
    }
}

void AskToSave(std::function<void()> after) {
    UseSurface use(g_edView);
    auto save = [after] {
        CloseDialog();
        if (SaveEditor(false)) {
            DestroyEditorWindow();
            if (after) {
                after();
            }
        }
    };
    ShowDialog(
        Tr(L"Zapisać zmiany?", L"Save changes?"), 460,
        [save, after](D2D1_RECT_F body) -> float {
            const wchar_t* text =
                Tr(L"Obraz ma niezapisane zmiany. Zapisany trafi do schowka jako nowy obraz, a "
                   L"oryginał zostanie bez zmian.",
                   L"The image has unsaved changes. If you save them, it goes to the clipboard as "
                   L"a new image, and the original stays as it is.");
            float th = TextHeight(text, g_fonts.body, body.right - body.left);
            Text(text, g_fonts.body, {body.left, body.top, body.right, body.top + th},
                 g_theme.text);
            float y = body.top + th + 24;
            float bw = (body.right - body.left - 16) / 3;
            Button({body.left, y, body.left + bw, y + 32}, HIT(0), Tr(L"Zapisz", L"Save"), 0,
                   kAccent)
                .click = save;
            Button({body.left + bw + 8, y, body.left + 2 * bw + 8, y + 32}, HIT(0),
                   Tr(L"Nie zapisuj", L"Don't save"))
                .click = [after] {
                CloseDialog();
                DestroyEditorWindow();
                if (after) {
                    after();
                }
            };
            Button({body.right - bw, y, body.right, y + 32}, HIT(0), Tr(L"Anuluj", L"Cancel"))
                .click = [] { CloseDialog(); };
            return y + 32;
        },
        save);
}

void CloseEditor(std::function<void()> after) {
    Editor& e = g_ed;
    if (!e.hwnd) {
        if (after) {
            after();
        }
        return;
    }
    FinishText();
    if (Unsaved() && !e.pixels.empty()) {
        if (IsIconic(e.hwnd)) {
            ShowWindow(e.hwnd, SW_RESTORE);
        }
        SetForegroundWindow(e.hwnd);
        AskToSave(std::move(after));
        return;
    }
    DestroyEditorWindow();
    if (after) {
        after();
    }
}

LRESULT CALLBACK EditorProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Editor& e = g_ed;
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
            if (!IsZoomed(hwnd)) {
                int b = MulDiv(6, g_edView.dpi, 96);
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
            }
            Region* r = RegionAt(g_edView, {pt.x * 96.f / g_edView.dpi, pt.y * 96.f / g_edView.dpi});
            return r && r->caption ? HTCAPTION : HTCLIENT;
        }

        case WM_NCACTIVATE:
            return DefWindowProcW(hwnd, msg, wParam, -1);

        case WM_GETMINMAXINFO: {
            auto* mmi = (MINMAXINFO*)lParam;
            UINT dpi = GetDpiForWindow(hwnd);
            mmi->ptMinTrackSize = {MulDiv((int)kEdMinWidth, dpi, 96),
                                   MulDiv((int)kEdMinHeight, dpi, 96)};
            // Maximized, the window fills the work area, not the whole screen.
            MONITORINFO mi{sizeof(mi)};
            if (GetMonitorInfoW(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi)) {
                mmi->ptMaxPosition = {mi.rcWork.left - mi.rcMonitor.left,
                                      mi.rcWork.top - mi.rcMonitor.top};
                mmi->ptMaxSize = {mi.rcWork.right - mi.rcWork.left,
                                  mi.rcWork.bottom - mi.rcWork.top};
            }
            return 0;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            if (g_edView.hwnd) {
                Paint(g_edView);
            }
            return 0;
        }

        case WM_ERASEBKGND:
            return 1;

        case WM_SIZE:
            if (g_edView.rt) {
                g_edView.rt->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam)));
            }
            if (g_edView.hwnd) {
                if (e.fitted) {
                    EdFit();
                } else {
                    ClampPan();
                }
                RedrawEditor();
            }
            return 0;

        case WM_DPICHANGED: {
            g_edView.dpi = HIWORD(wParam);
            auto* r = (RECT*)lParam;
            SetWindowPos(hwnd, nullptr, r->left, r->top, r->right - r->left, r->bottom - r->top,
                         SWP_NOZORDER | SWP_NOACTIVATE);
            if (g_edView.rt) {
                g_edView.rt->SetDpi((float)g_edView.dpi, (float)g_edView.dpi);
            }
            if (e.fitted) {
                EdFit();
            }
            RedrawEditor();
            return 0;
        }

        case WM_SETCURSOR:
            if (LOWORD(lParam) == HTCLIENT && g_edView.mouseInside) {
                Region* r = RegionAt(g_edView, g_edView.mouse);
                if (r && r->hit == e.stageHit) {
                    SetCursor(LoadCursorW(nullptr, StageCursor(g_edView.mouse)));
                    return TRUE;
                }
            }
            if (HandleMouse(g_edView, msg, wParam, lParam)) {
                return TRUE;
            }
            break;

        case WM_MOUSEMOVE: {
            if (e.middlePan) {
                D2D1_POINT_2F p = MousePoint(g_edView, lParam);
                e.pan = {e.panStart.x + p.x - e.dragScreen.x, e.panStart.y + p.y - e.dragScreen.y};
                e.fitted = false;
                ClampPan();
                RedrawEditor();
                return 0;
            }
            HandleMouse(g_edView, msg, wParam, lParam);
            if (e.tool == Tool::Brush || e.tool == Tool::Highlighter || e.tool == Tool::Eraser) {
                RedrawEditor();  // The brush size circle follows the mouse.
            }
            return 0;
        }

        case WM_MBUTTONDOWN:
            e.middlePan = true;
            e.dragScreen = MousePoint(g_edView, lParam);
            e.panStart = e.pan;
            SetCapture(hwnd);
            SetCursor(LoadCursorW(nullptr, IDC_SIZEALL));
            return 0;

        case WM_MBUTTONUP:
            if (e.middlePan) {
                e.middlePan = false;
                ReleaseCapture();
            }
            return 0;

        case WM_MOUSEWHEEL:
        case WM_MOUSEHWHEEL: {
            POINT pt{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            ScreenToClient(hwnd, &pt);
            D2D1_POINT_2F p{pt.x * 96.f / g_edView.dpi, pt.y * 96.f / g_edView.dpi};
            float steps = GET_WHEEL_DELTA_WPARAM(wParam) / 120.f;
            WORD keys = GET_KEYSTATE_WPARAM(wParam);
            if (msg == WM_MOUSEWHEEL && (keys & MK_CONTROL)) {
                EdSetZoom(e.zoom * std::pow(1.25f, steps), p);
                return 0;
            }
            if (msg == WM_MOUSEHWHEEL || (keys & MK_SHIFT)) {
                e.pan.x += (msg == WM_MOUSEHWHEEL ? -steps : steps) * 60;
            } else {
                e.pan.y += steps * 60;
            }
            e.fitted = false;
            ClampPan();
            RedrawEditor();
            return 0;
        }

        case WM_MOUSELEAVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
            HandleMouse(g_edView, msg, wParam, lParam);
            return 0;

        case WM_KEYDOWN:
            if (EditorKey((UINT)wParam)) {
                return 0;
            }
            break;

        case WM_KEYUP:
            if (wParam == VK_SPACE && e.space) {
                e.space = false;
                RedrawEditor();
            }
            break;

        case WM_CHAR:
            EditorChar((wchar_t)wParam);
            return 0;

        case WM_KILLFOCUS:
            e.space = false;
            break;

        case WM_TIMER:
            if (wParam == kTimerEditorCaret) {
                e.caretOn = !e.caretOn;
                RedrawEditor();
                return 0;
            }
            HandleMouse(g_edView, msg, wParam, lParam);
            return 0;

        case WM_CLOSE:
            CloseEditor();
            return 0;

        case WM_DESTROY:
            KillTimer(hwnd, kTimerEditorCaret);
            DiscardGraphics(g_edView);
            g_edView = {};
            e.hwnd = nullptr;
            e.generation++;
            e.pixels = {};
            e.doc = {};
            e.undo = {};
            e.redo = {};
            e.selected = e.editing = -1;
            e.drag = EdDrag::None;
            e.middlePan = e.space = false;
            if (e.tool == Tool::Crop) {
                e.tool = e.beforeCrop;
            }
            return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void LoadEditorImage(UINT64 id) {
    UINT64 generation = g_ed.generation;
    Enqueue([id, generation] {
        std::string png, pixels;
        UINT w = 0, h = 0;
        if (g_workerWic && LoadImagePng(id, &png)) {
            if (IWICBitmapSource* source = DecodeImage(g_workerWic, png)) {
                source->GetSize(&w, &h);
                if (!ToPixels(g_workerWic, source, w, h, GUID_WICPixelFormat32bppPBGRA,
                              &pixels)) {
                    pixels.clear();
                }
                source->Release();
            }
        }
        PostUi([generation, w, h, pixels = std::move(pixels)]() mutable {
            Editor& e = g_ed;
            if (generation != e.generation || !e.hwnd) {
                return;
            }
            e.loading = false;
            if (pixels.empty()) {
                e.error = Tr(L"Nie udało się wczytać obrazu.", L"Couldn't load the image.");
            } else {
                e.w = w;
                e.h = h;
                e.pixels = std::move(pixels);
                e.doc.crop = FullImage();
                e.cropDraft = e.doc.crop;
                if (e.fitted) {
                    EdFit();
                }
            }
            RedrawEditor();
        });
    });
}

void ShowEditorWindow() {
    if (IsIconic(g_ed.hwnd)) {
        ShowWindow(g_ed.hwnd, SW_RESTORE);
    }
    SetForegroundWindow(g_ed.hwnd);
}

void OpenEditor(UINT64 id, bool viewOnly) {
    Editor& e = g_ed;
    Item* it = FindItem(id);
    if (!it || !it->imageWidth) {
        return;
    }
    if (e.hwnd) {
        if (e.itemId == id) {
            if (!viewOnly && e.viewOnly) {
                e.viewOnly = false;
                EdFit();
            }
            ShowEditorWindow();
            return;
        }
        // Another image is open: it closes first, asking about its changes.
        ShowEditorWindow();
        CloseEditor([id, viewOnly] { OpenEditor(id, viewOnly); });
        return;
    }
    e.itemId = id;
    e.viewOnly = viewOnly;
    e.kind = it->kind;
    e.category = it->category;
    e.source = it->source;
    e.w = it->imageWidth;
    e.h = it->imageHeight;
    e.pixels.clear();
    e.loading = true;
    e.error.clear();
    e.message.clear();
    e.doc = {};
    e.doc.crop = FullImage();
    e.cropDraft = e.doc.crop;
    e.undo.clear();
    e.redo.clear();
    e.versions = e.savedVersion = 0;
    e.selected = e.editing = -1;
    e.changeOpen = false;
    e.aspect = 0;

    HINSTANCE instance = GetModuleHandleW(nullptr);
    if (!g_editorClass) {
        WNDCLASSEXW wc{sizeof(wc)};
        wc.style = CS_DBLCLKS;
        wc.lpfnWndProc = EditorProc;
        wc.hInstance = instance;
        wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wc.lpszClassName = kEditorClass;
        g_editorClass = RegisterClassExW(&wc);
    }
    // Centered on the screen with the mouse; the viewer fits the image at 100%.
    POINT cursor;
    GetCursorPos(&cursor);
    HMONITOR monitor = MonitorFromPoint(cursor, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi{sizeof(mi)};
    GetMonitorInfoW(monitor, &mi);
    UINT dpi = 96, dpiY = 96;
    GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpi, &dpiY);
    float scale = dpi / 96.f;
    RECT work = mi.rcWork;
    int workW = work.right - work.left, workH = work.bottom - work.top;
    int w, h;
    if (viewOnly) {
        w = (int)e.w + (int)(48 * scale);
        h = (int)e.h + (int)((kEdTitleHeight + kEdFooterHeight + 48) * scale);
    } else {
        w = (int)(1120 * scale);
        h = (int)(780 * scale);
    }
    w = std::min(std::max(w, (int)(kEdMinWidth * scale)), workW * 9 / 10);
    h = std::min(std::max(h, (int)(kEdMinHeight * scale)), workH * 9 / 10);
    g_edView = {};
    g_edView.render = RenderEditor;
    g_edView.dpi = dpi;
    e.hwnd =
        CreateWindowExW(0, kEditorClass, Tr(L"Edytor grafiki", L"Image editor"),
                        WS_POPUP | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
                        work.left + (workW - w) / 2, work.top + (workH - h) / 2, w, h, nullptr,
                        nullptr, instance, nullptr);
    if (!e.hwnd) {
        Wh_Log(L"Can't open the editor: %u", GetLastError());
        return;
    }
    g_edView.hwnd = e.hwnd;
    g_edView.dpi = GetDpiForWindow(e.hwnd);
    ApplyWindowTheme(e.hwnd, kDwmBackdropMica);
    SetWindowPos(e.hwnd, nullptr, 0, 0, 0, 0,
                 SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    if (viewOnly) {
        e.zoom = EdActual();
        e.pan = {0, 0};
        e.fitted = false;
    } else {
        EdFit();
    }
    ShowWindow(e.hwnd, SW_SHOW);
    SetForegroundWindow(e.hwnd);
    LoadEditorImage(id);
}

void ShutdownEditor() {
    DestroyEditorWindow();
    SafeRelease(g_roundStyle);
    if (g_editorClass) {
        UnregisterClassW(kEditorClass, GetModuleHandleW(nullptr));
        g_editorClass = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Showing, hiding and placing the window

int MainPx(float dip) {
    return MulDiv((int)std::lround(dip * 100), g_main.dpi, 9600);
}

// The details panel beside the list widens the window by this much.
int PanelPx() {
    return g_detailsId && SideDetails() ? MainPx(kPanelWidth + 8) : 0;
}

void ApplyWindowSizeSetting() {
    INT64 mode = Setting(L"sizeMode", 0);
    bool keep = mode == 1 || (mode == 0 && SameBoot(Setting(L"boot", 0)));
    if (keep && Setting(L"width", 0) > 0 && Setting(L"height", 0) > 0) {
        g_sizeDip = {(float)Setting(L"width", 0), (float)Setting(L"height", 0)};
    } else {
        g_sizeDip = {kDefaultWidth, kDefaultHeight};
    }
    g_sizeDip.width = std::max(g_sizeDip.width, kMinWidth);
    g_sizeDip.height = std::max(g_sizeDip.height, kMinHeight);
}

void ApplyLook() {
    INT64 language = Setting(L"language", 0);  // 0 follows Windows, 1 Polish, 2 English.
    bool english =
        language ? language == 2 : PRIMARYLANGID(GetUserDefaultUILanguage()) != LANG_POLISH;
    if (english != g_english) {
        g_english = english;
        for (Item& it : g_items) {
            if (it.imageWidth) {
                FinishItem(it);  // The preview of an image names its type.
            }
        }
    }
    LoadTheme();
    if (g_ed.hwnd) {
        SetWindowTextW(g_ed.hwnd, Tr(L"Edytor grafiki", L"Image editor"));
        ApplyWindowTheme(g_ed.hwnd, kDwmBackdropMica);
        RedrawEditor();
    }
    ApplyWindowTheme(g_hwnd, kDwmBackdropAcrylic);
    UpdateTrayIcon(NIM_MODIFY);
    for (Field& f : g_fields) {
        if (f.hwnd) {
            InvalidateRect(f.hwnd, nullptr, TRUE);
        }
    }
    Redraw();
}

void HidePanel(bool restoreFocus) {
    if (!IsWindowVisible(g_hwnd)) {
        return;
    }
    if (g_recordTarget >= 0) {
        StopRecording();
    }
    // Without moving the focus: the window may be losing it right now.
    g_main.flyout = {};
    g_main.dialog = {};
    ResetSearch();
    g_hiddenTick = GetTickCount64();
    ShowWindow(g_hwnd, SW_HIDE);
    CloseDetails();
    if (restoreFocus && g_prevForeground && IsWindow(g_prevForeground)) {
        SetForegroundWindow(g_prevForeground);
    }
}

void SetOnTop(bool onTop) {
    g_onTop = onTop;
    SetWindowPos(g_hwnd, onTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    Redraw();
}

int g_detailsShift;  // How far the window moved left to make room for the panel.

void ResizeForDetails(bool open) {
    RECT rc;
    GetWindowRect(g_hwnd, &rc);
    int delta = MainPx(kPanelWidth + 8);
    int w = rc.right - rc.left, x = rc.left;
    if (open) {
        w += delta;
        MONITORINFO mi{sizeof(mi)};
        GetMonitorInfoW(MonitorFromWindow(g_hwnd, MONITOR_DEFAULTTONEAREST), &mi);
        g_detailsShift = 0;
        if (x + w > mi.rcWork.right) {
            int nx = std::max((int)mi.rcWork.left, (int)mi.rcWork.right - w);
            g_detailsShift = x - nx;
            x = nx;
        }
    } else {
        w = std::max(w - delta, MainPx(kMinWidth));
        x += g_detailsShift;
        g_detailsShift = 0;
    }
    g_placing = true;
    SetWindowPos(g_hwnd, nullptr, x, rc.top, w, rc.bottom - rc.top,
                 SWP_NOZORDER | SWP_NOACTIVATE);
    g_placing = false;
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
        if (SUCCEEDED(AccessibleObjectFromWindow(focus, (DWORD)OBJID_CARET, __uuidof(IAccessible),
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

RECT MouseRect() {
    POINT pt;
    GetCursorPos(&pt);
    return {pt.x, pt.y, pt.x, pt.y};
}

// The remembered position, when the setting asks for one and it's still valid.
bool SavedPosition(POINT* pt) {
    INT64 mode = Setting(L"posMode", 0);
    if (mode < 2 || !g_settings.contains(L"posX") ||
        (mode == 2 && !SameBoot(Setting(L"posBoot", 0)))) {
        return false;
    }
    *pt = {(LONG)Setting(L"posX", 0), (LONG)Setting(L"posY", 0)};
    return true;
}

// Shows the window below the anchor, or above it when there's no room; at a
// remembered position when the settings ask for it.
void ShowPanel(RECT anchor, bool aboveAnchor) {
    if (Setting(L"sizeMode", 0) == 2) {
        g_sizeDip = {kDefaultWidth, kDefaultHeight};
    }
    POINT saved;
    bool atSaved = SavedPosition(&saved);
    POINT pt = atSaved ? saved : POINT{anchor.left, anchor.bottom};
    HMONITOR monitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi{sizeof(mi)};
    GetMonitorInfoW(monitor, &mi);
    UINT dpiX = 96, dpiY = 96;
    GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
    RECT work = mi.rcWork;
    int w = std::min((int)std::lround(g_sizeDip.width * dpiX / 96), (int)(work.right - work.left));
    int h = std::min((int)std::lround(g_sizeDip.height * dpiX / 96),
                     (int)(work.bottom - work.top));
    int x, y;
    if (atSaved) {
        x = saved.x;
        y = saved.y;
    } else {
        int gap = MulDiv(6, dpiX, 96);
        x = anchor.left;
        y = anchor.bottom + gap;
        if (aboveAnchor || y + h > work.bottom) {
            y = anchor.top - gap - h;
        }
    }
    x = std::clamp(x, (int)work.left, (int)work.right - w);
    y = std::clamp(y, (int)work.top, (int)work.bottom - h);

    ResetSearch();
    g_page = Page::Main;
    g_selectedId = 0;
    g_listScroll.Reset();
    Refresh();

    g_placing = true;
    SetWindowPos(g_hwnd, g_onTop ? HWND_TOPMOST : HWND_TOP, x, y, w, h, SWP_SHOWWINDOW);
    g_placing = false;
    SetForegroundWindow(g_hwnd);
    SetFocus(g_hwnd);
}

void RememberForeground() {
    HWND foreground = GetForegroundWindow();
    if (foreground && foreground != g_hwnd && foreground != g_ed.hwnd) {
        g_prevForeground = foreground;
    }
}

void ShowNearTray(POINT click) {
    RememberForeground();
    RECT anchor{click.x, click.y, click.x, click.y};
    if (click.x == 0 && click.y == 0) {
        MONITORINFO mi{sizeof(mi)};
        GetMonitorInfoW(MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY), &mi);
        anchor = {mi.rcWork.right, mi.rcWork.bottom, mi.rcWork.right, mi.rcWork.bottom};
    }
    // Centered on the icon, just above the taskbar.
    anchor.left -= (LONG)(g_sizeDip.width * g_main.dpi / 96 / 2);
    ShowPanel(anchor, true);
}

void TogglePanel() {
    if (IsWindowVisible(g_hwnd) && GetForegroundWindow() == g_hwnd) {
        HidePanel(true);
        return;
    }
    RememberForeground();
    ShowPanel(Setting(L"posMode", 0) == 1 ? MouseRect() : AnchorRect(g_prevForeground), false);
}

void ShowTrayMenu(POINT pt) {
    HMENU menu = CreatePopupMenu();
    enum { kOpen = 1, kPause, kClear, kSettings };
    std::wstring open = Tr(L"Otwórz schowek", L"Open clipboard");
    if (!(g_keys[ScOpen] & kKeyOff) && g_keys[ScOpen]) {
        open += L"\t" + KeyName(g_keys[ScOpen]);
    }
    AppendMenuW(menu, MF_STRING, kOpen, open.c_str());
    SetMenuDefaultItem(menu, kOpen, FALSE);
    AppendMenuW(menu, MF_STRING | (g_paused ? MF_CHECKED : 0), kPause,
                Tr(L"Wstrzymaj zapisywanie", L"Pause saving"));
    AppendMenuW(menu, MF_STRING | (CountItems(Kind::History) ? 0 : MF_GRAYED), kClear,
                Tr(L"Wyczyść historię", L"Clear history"));
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(menu, MF_STRING, kSettings, Tr(L"Ustawienia", L"Settings"));
    SetForegroundWindow(g_hwnd);
    int cmd = TrackPopupMenu(menu, TPM_RETURNCMD | TPM_RIGHTBUTTON | TPM_BOTTOMALIGN, pt.x, pt.y,
                             0, g_hwnd, nullptr);
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
            SwitchTab(Tab::History);
            ClearHistory();
            break;
        case kSettings:
            ShowNearTray(pt);
            ShowPage(Page::Settings);
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Window procedure

void Shutdown() {
    g_quitting = true;
    ShutdownEditor();
    ExpireToast();  // Commits a cleared history that is still offered back.
    for (size_t i = 0; i < g_hotkeyItems.size(); i++) {
        UnregisterHotKey(g_hwnd, kHotkeyBase + (int)i);
    }
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

RECT g_moveStart;

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
            int b = MainPx(6);
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
            Region* r = RegionAt(g_main, {pt.x * 96.f / g_main.dpi, pt.y * 96.f / g_main.dpi});
            return r && r->caption ? HTCAPTION : HTCLIENT;
        }

        case WM_NCACTIVATE:
            // -1: don't paint a non-client area; the window has none.
            return DefWindowProcW(hwnd, msg, wParam, -1);

        case WM_GETMINMAXINFO: {
            auto* mmi = (MINMAXINFO*)lParam;
            mmi->ptMinTrackSize = {MainPx(kMinWidth) + PanelPx(), MainPx(kMinHeight)};
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
            Paint(g_main);
            return 0;
        }

        case WM_ERASEBKGND:
            return 1;

        case WM_SIZE:
            if (g_main.rt) {
                g_main.rt->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam)));
            }
            if (g_sizing) {
                D2D1_SIZE_F s = SizeDip(g_main);
                g_sizeDip = {s.width - (PanelPx() ? kPanelWidth + 8 : 0), s.height};
            }
            for (Scroller* s : {&g_listScroll, &g_catScroll, &g_detailScroll, &g_pageScroll}) {
                s->Clamp();
            }
            Redraw();
            return 0;

        case WM_ENTERSIZEMOVE:
            GetWindowRect(hwnd, &g_moveStart);
            break;

        case WM_SIZING:
            g_sizing = true;
            break;

        case WM_EXITSIZEMOVE: {
            RECT rc;
            GetWindowRect(hwnd, &rc);
            if (g_sizing) {
                // The size stays as the setting says: until a restart, for good,
                // or only while the window is open.
                g_sizing = false;
                if (Setting(L"sizeMode", 0) != 2) {
                    g_settings[L"width"] = std::lround(g_sizeDip.width);
                    g_settings[L"height"] = std::lround(g_sizeDip.height);
                    g_settings[L"boot"] = BootTime();
                    SaveSettings();
                }
                Redraw();
            }
            if ((rc.left != g_moveStart.left || rc.top != g_moveStart.top) &&
                Setting(L"posMode", 0) >= 2) {
                g_settings[L"posX"] = rc.left + g_detailsShift;
                g_settings[L"posY"] = rc.top;
                g_settings[L"posBoot"] = BootTime();
                SaveSettings();
            }
            return 0;
        }

        case WM_DPICHANGED: {
            g_main.dpi = HIWORD(wParam);
            if (!g_placing) {
                auto* r = (RECT*)lParam;
                SetWindowPos(hwnd, nullptr, r->left, r->top, r->right - r->left,
                             r->bottom - r->top, SWP_NOZORDER | SWP_NOACTIVATE);
            }
            if (g_main.rt) {
                g_main.rt->SetDpi((float)g_main.dpi, (float)g_main.dpi);
            }
            UpdateEditFont();
            Redraw();
            return 0;
        }

        case WM_ACTIVATE:
            // Clicking outside closes the window, like Win+V, unless it's
            // pinned on top or a system dialog of this window is open.
            if (LOWORD(wParam) == WA_INACTIVE && !g_onTop && g_modal == 0) {
                HidePanel(false);
            }
            break;

        case WM_CLOSE:
            if (!g_quitting) {
                HidePanel(true);
                return 0;
            }
            break;

        case WM_MOUSEMOVE:
        case WM_MOUSELEAVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MOUSEWHEEL:
            HandleMouse(g_main, msg, wParam, lParam);
            return 0;

        case WM_SETCURSOR:
            if (HandleMouse(g_main, msg, wParam, lParam)) {
                return TRUE;
            }
            break;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if (OnKey((UINT)wParam)) {
                return 0;
            }
            break;

        case WM_COMMAND:
            if (HIWORD(wParam) == EN_CHANGE && !g_settingText) {
                for (Field& f : g_fields) {
                    if (f.hwnd && f.hwnd == (HWND)lParam && f.onChange) {
                        auto fn = f.onChange;
                        fn();
                    }
                }
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
                case kTimerToast:
                    ExpireToast();
                    break;
                case kTimerClipboardRetry:
                    KillTimer(hwnd, kTimerClipboardRetry);
                    OnClipboardUpdate();
                    break;
                case kTimerPassword:
                    KillTimer(hwnd, kTimerPassword);
                    CheckImport(FieldText(FPassword));
                    break;
                case kTimerHourly:
                    AutoClean();
                    break;
                default:
                    HandleMouse(g_main, msg, wParam, lParam);
            }
            return 0;

        case WM_CLIPBOARDUPDATE:
            g_clipboardRetries = 0;
            OnClipboardUpdate();
            return 0;

        case WM_SETTINGCHANGE:
            if (wParam == SPI_SETHIGHCONTRAST ||
                (lParam && wcscmp((const wchar_t*)lParam, L"ImmersiveColorSet") == 0)) {
                ApplyLook();
            }
            break;

        case WM_DWMCOLORIZATIONCOLORCHANGED:
        case WM_THEMECHANGED:
            ApplyLook();
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
                    ShowTrayMenu(pt);
                    break;
            }
            return 0;
        }

        case WM_APP_TOGGLE:
            TogglePanel();
            return 0;

        case WM_APP_CALL: {
            auto* fn = (std::function<void()>*)lParam;
            (*fn)();
            delete fn;
            return 0;
        }

        case WM_APP_SAVE: {
            MSG pending;
            // One save for a burst of changes.
            while (PeekMessageW(&pending, hwnd, WM_APP_SAVE, WM_APP_SAVE, PM_REMOVE)) {
            }
            SaveIndexNow();
            return 0;
        }

        case WM_APP_RECORD:
            OnRecorded((UINT)wParam, (UINT)lParam);
            return 0;

        case WM_HOTKEY: {
            int i = (int)wParam - kHotkeyBase;
            if (i >= 0 && i < (int)g_hotkeyItems.size() && g_hotkeyItems[i]) {
                PasteTemplateHotkey(g_hotkeyItems[i]);
            }
            return 0;
        }

        case WM_ENDSESSION:
            if (wParam) {
                // Sign-out or shutdown: write everything before the process ends.
                ExpireToast();
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
// The open shortcut (Win+V by default): a low-level keyboard hook on its own
// thread, so that a busy window never delays typing. It also records new
// shortcuts in the settings, Win combinations included.

volatile DWORD g_swallowVk;  // Its key up is swallowed too.

bool KeyDown(int vk) {
    return GetAsyncKeyState(vk) < 0;
}

UINT HookMods() {
    UINT mods = 0;
    if (KeyDown(VK_LWIN) || KeyDown(VK_RWIN)) mods |= MOD_WIN;
    if (KeyDown(VK_CONTROL)) mods |= MOD_CONTROL;
    if (KeyDown(VK_MENU)) mods |= MOD_ALT;
    if (KeyDown(VK_SHIFT)) mods |= MOD_SHIFT;
    return mods;
}

bool IsModifierKey(DWORD vk) {
    switch (vk) {
        case VK_LWIN:
        case VK_RWIN:
        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL:
        case VK_MENU:
        case VK_LMENU:
        case VK_RMENU:
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
            return true;
    }
    return false;
}

// Keeps the Start menu closed when Win is released after a swallowed key, and
// lets this process take the foreground.
void SendDummyKey() {
    INPUT inputs[2] = {{INPUT_KEYBOARD}, {INPUT_KEYBOARD}};
    for (INPUT& in : inputs) {
        in.ki.wVk = kDummyVk;
        in.ki.dwExtraInfo = kInputTag;
    }
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(2, inputs, sizeof(INPUT));
}

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
    if (code == HC_ACTION) {
        const auto* kb = (const KBDLLHOOKSTRUCT*)lParam;
        DWORD vk = kb->vkCode;
        bool down = wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN;
        if (kb->dwExtraInfo != kInputTag && !IsModifierKey(vk)) {
            if (g_recordingKey && down) {
                UINT mods = HookMods();
                if (mods & MOD_WIN) {
                    SendDummyKey();
                }
                g_recordingKey = false;
                g_swallowVk = vk;
                PostMessageW(g_hwnd, WM_APP_RECORD, vk, mods);
                return 1;
            }
            UINT key = g_openKey;
            if (down && key && !(key & kKeyOff) && vk == (key & 0xFFFF) &&
                (g_swallowVk == vk || HookMods() == key >> 16)) {
                if (g_swallowVk != vk) {  // Auto-repeat doesn't toggle again.
                    g_swallowVk = vk;
                    if ((key >> 16) & MOD_WIN) {
                        SendDummyKey();
                    }
                    PostMessageW(g_hwnd, WM_APP_TOGGLE, 0, 0);
                }
                return 1;
            }
            if (!down && g_swallowVk == vk) {
                g_swallowVk = 0;
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
void CALLBACK ForegroundProc(HWINEVENTHOOK, DWORD, HWND hwnd, LONG idObject, LONG, DWORD,
                             DWORD) {
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
    for (int i = 0; i < ScCount; i++) {
        g_keys[i] = kShortcuts[i].def;
    }

    g_cfHtml = RegisterClipboardFormatW(L"HTML Format");
    g_cfRtf = RegisterClipboardFormatW(L"Rich Text Format");
    g_cfPng = RegisterClipboardFormatW(L"PNG");
    g_cfExclude = RegisterClipboardFormatW(L"ExcludeClipboardContentFromMonitorProcessing");
    g_cfCanInclude = RegisterClipboardFormatW(L"CanIncludeInClipboardHistory");
    g_cfViewerIgnore = RegisterClipboardFormatW(L"Clipboard Viewer Ignore");
    g_taskbarCreatedMsg = RegisterWindowMessageW(L"TaskbarCreated");

    WNDCLASSEXW wc{sizeof(wc)};
    wc.style = CS_DBLCLKS;
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
    g_main.hwnd = g_hwnd;
    g_main.dpi = GetDpiForWindow(g_hwnd);
    g_main.render = RenderMain;
    ApplyWindowTheme(g_hwnd, kDwmBackdropAcrylic);
    SetWindowPos(g_hwnd, nullptr, 0, 0, 0, 0,
                 SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    UpdateEditFont();

    g_workerThread = CreateThread(nullptr, 0, WorkerProc, nullptr, 0, nullptr);
    Enqueue(LoadJob);
    Enqueue(DeleteTempFiles);
    AddClipboardFormatListener(g_hwnd);
    UpdateTrayIcon(NIM_ADD);
    SetTimer(g_hwnd, kTimerHourly, 60 * 60 * 1000, nullptr);
    g_foregroundHook =
        SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, nullptr,
                        ForegroundProc, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
    g_hookThread = CreateThread(nullptr, 0, HookThread, nullptr, 0, &g_hookThreadId);
    SetEvent(g_uiReady);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    DiscardGraphics(g_main);
    SafeRelease(g_dashStyle);
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
