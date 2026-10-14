# Clipboard Tweaks

A modern clipboard manager that replaces the Windows Win+V panel with a Fluent 2 window. It keeps everything you copy, without a limit and across restarts, and removes items only when you tell it to.

> **Pre-release (0.7.1).** All planned features are in. Version 1.0.0 follows after a full round of testing.

[Download the latest version](https://github.com/mateuszsobieraj404-lgtm/Windhawk-mods/releases?q=clipboard-tweaks&expanded=true) · [Changelog](CHANGELOG.md) · [Report a bug](https://github.com/mateuszsobieraj404-lgtm/Windhawk-mods/issues/new)

## Features

- **Replaces Win+V**: Win+V opens Clipboard Tweaks instead of the built-in panel. Turn the mod off in Windhawk and the built-in panel comes back. The shortcut can be changed in the settings.
- **History without a limit**: text, formatted text (HTML, RTF), links, colors, images in their original resolution and copied files. Copying the same thing again moves it to the top instead of adding a duplicate.
- **Paste with one click**: click an item or press Enter to paste it into the window you were working in. Shift+Enter pastes plain text, and Ctrl+1…9 pastes the n-th item.
- **Pinned items, organized**: pin anything you use often (Ctrl+P). Pinned items are kept until you remove them, and can have a category, #tags, an emoji and a color mark. Categories show in a side column or as collapsible sections.
- **Templates**: reusable texts with fields filled in when you paste them: `{date}` (today's date), `{time}` (the time) and `{clipboard}` (what's on the clipboard). The Polish names `{data}`, `{godzina}` and `{schowek}` work too. A template can have its own shortcut that pastes it in any app.
- **Item details**: press Space or click the panel icon to see the full content, an image preview, paste options, organizing and information about the item, in a side panel or in place of the list.
- **Search**: Ctrl+F filters the current tab as you type. Start with `#` to search tags.
- **Edit in place**: F2 edits a text item, or opens an image in the editor.
- **Image editor**: brush, highlighter, eraser, text, arrows, lines, circles, squares, rectangles, speech bubbles, blur for hiding data, and crop with fixed aspect ratios. Undo and redo, zoom from 5% to 1600%, and an image viewer. Saving adds a new image and keeps the original as it was.
- **Trash**: deleted items stay in the trash for 30 days and can be restored. **Clear history** can be undone for 10 seconds.
- **Settings page**: language, theme, background transparency, details panel and pinned layout, automatic cleanup of the history, full resolution images, window size and position, and every keyboard shortcut.
- **Move to another computer**: export pinned items, categories, tags, templates, settings and optionally the history to one file, protected with a password, and import it on the other computer.
- **Window that fits your workflow**: resize it from any edge, and keep it on top with the pin button. It follows the Windows light or dark mode and accent color, with a translucent acrylic background.
- **English and Polish**: the interface is in Polish on a Polish Windows and in English everywhere else. The language can be changed in the settings.
- **Private by design**: items are stored encrypted with your Windows account, and content that apps mark as private, such as passwords from a password manager, is never saved.

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

In the image editor, single letters pick a tool: V select, B brush, H highlighter, E eraser, T text, S shapes, M blur, C crop. Ctrl+Z and Ctrl+Y undo and redo, Ctrl+S saves, Ctrl+plus and Ctrl+minus zoom, Ctrl+0 fits the image to the window, and Space with a drag moves the view.

Right-click an item for more actions. The tray icon opens the window with a left click, and shows **Pause saving**, **Clear history** and **Settings** with a right click.

## Data and privacy

Everything is stored in `%LOCALAPPDATA%\Clipboard Tweaks`, encrypted with a key protected by your Windows account, so other users of the computer can't read it. Removing the mod doesn't delete it. Nothing leaves the computer unless you export it.

## Settings

- **Turn off Windows clipboard history** (on by default): while the mod runs, the built-in Windows clipboard history is turned off so that the two don't overlap. Its previous state comes back when the mod is turned off.

Everything else is set on the mod's own settings page: **…** → **Settings**.

## Compatibility

- Windows 11 22H2 or newer.
- Windhawk 1.7.3 or newer.

## Known issues

- Pasting into an app that runs as administrator doesn't work, because Windows doesn't let a regular app send keystrokes to it. The item is still put on the clipboard, so you can paste it with Ctrl+V.

## Credits

Designed by Mateusz Sobieraj and built together with Claude, an AI assistant by Anthropic.

## Installation

See [Installation](../README.md#installation) in the repository readme.
