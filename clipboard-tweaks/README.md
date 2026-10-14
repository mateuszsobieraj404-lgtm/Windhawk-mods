# Clipboard Tweaks

A modern clipboard manager that replaces the Windows Win+V panel with a Fluent 2 window. It keeps everything you copy, without a limit and across restarts, and removes items only when you tell it to.

> **Pre-release (0.1.0).** This version brings the history window. Pinned items, templates, item details and the settings page come in the next versions.

[Download the latest version](https://github.com/mateuszsobieraj404-lgtm/Windhawk-mods/releases?q=clipboard-tweaks&expanded=true) · [Changelog](CHANGELOG.md) · [Report a bug](https://github.com/mateuszsobieraj404-lgtm/Windhawk-mods/issues/new)

## Features

- **Replaces Win+V**: Win+V opens Clipboard Tweaks instead of the built-in panel. Turn the mod off in Windhawk and the built-in panel comes back.
- **History without a limit**: text, formatted text (HTML, RTF), links, colors, images in their original resolution and copied files. Nothing disappears on its own, and copying the same thing again moves it to the top instead of adding a duplicate.
- **Search**: click the magnifier in the title bar or press Ctrl+F to filter the history as you type.
- **Paste with one click**: click an item or press Enter to paste it into the window you were working in. Shift+Enter pastes plain text, and Ctrl+1…9 pastes the n-th item.
- **Clear history with undo**: **Clear history** removes the history at once, and **Undo** brings it back within 10 seconds.
- **Pause saving**: pause saving new copies from the **…** menu or the tray icon.
- **Window that fits your workflow**: resize it from any edge, and keep it on top with the pin button. It follows the Windows light or dark mode and accent color, with a translucent acrylic background.
- **Private by design**: items are stored encrypted with your Windows account, and content that apps mark as private, such as passwords from a password manager, is never saved.

## How to use

| Shortcut | Action |
|---|---|
| Win+V | Open or close Clipboard Tweaks |
| Enter | Paste the selected item |
| Shift+Enter | Paste as plain text |
| Ctrl+1…9 | Paste the n-th item |
| Ctrl+F | Search |
| Esc | Close the search or the window |

The tray icon opens the window with a left click, and shows **Pause saving** and **Clear history** with a right click. Hover the **i** icon in the footer to see every shortcut.

## Settings

- **Turn off Windows clipboard history** (on by default): while the mod runs, the built-in Windows clipboard history is turned off so that the two don't overlap. Its previous state comes back when the mod is turned off.

## Data and privacy

The history is stored in `%LOCALAPPDATA%\Clipboard Tweaks`, encrypted with a key protected by your Windows account, so other users of the computer can't read it. Removing the mod doesn't delete it. Nothing leaves the computer.

## Compatibility

- Windows 11 22H2 or newer.
- Windhawk 1.7.3 or newer.

## Known issues

- Pasting into an app that runs as administrator doesn't work, because Windows doesn't let a regular app send keystrokes to it. The item is still put on the clipboard, so you can paste it with Ctrl+V.

## Installation

See [Installation](../README.md#installation) in the repository readme.
