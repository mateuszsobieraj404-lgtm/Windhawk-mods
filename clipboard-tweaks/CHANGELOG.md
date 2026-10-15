# Changelog

All notable changes to Clipboard Tweaks are documented in this file. The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and versions follow [Semantic Versioning](https://semver.org/).

## [Unreleased]

## [0.7.3] - 2026-10-15

### Fixed

- Win+V still opened the built-in Windows clipboard while an app that runs as administrator, such as Windhawk, was in front. The mod now also loads into Windows Explorer and passes Win+V on from there, which works right away, without signing out.

### Changed

- Turning the mod off gives Win+V back to Windows right away again. The change version 0.7.2 made to Explorer's settings is undone automatically.

## [0.7.2] - 2026-10-14

### Fixed

- Win+V opened the built-in Windows clipboard while an app that runs as administrator, such as Windhawk, was in front. Clipboard Tweaks now takes Win+V over from Windows Explorer, starting with the next sign-in.
- A custom shortcut for opening Clipboard Tweaks didn't work while an app that runs as administrator was in front.

## [0.7.1] - 2026-10-14

### Fixed

- Win+V sometimes opened the built-in Windows clipboard instead of Clipboard Tweaks.
- Three warnings shown in Windhawk's editor when compiling the mod.

## [0.7.0] - 2026-10-14

### Added

- English interface. The mod shows Polish on a Polish Windows and English everywhere else, and **Settings** → **Language** picks one by hand.
- Template fields have English names: `{date}`, `{time}` and `{clipboard}`. The Polish names still work, so existing templates don't change.

## [0.6.0] - 2026-10-14

### Added

- Image editor in its own window: brush, highlighter, eraser, text with size, bold, italic and underline, arrows, lines, circles, squares, rectangles and speech bubbles, blur for hiding data, and crop with fixed aspect ratios.
- Objects stay editable until saving: select, move, restyle or delete them, with undo and redo.
- Zoom from 5% to 1600%, fit to window and 100%, and moving the view with Space or the middle mouse button.
- **Save as new** and **Save and copy** add the result as a new image and keep the original. Closing with unsaved changes asks first.
- Image viewer for a quick look at an image in full size.

## [0.5.0] - 2026-10-14

### Added

- Settings page: theme, background transparency, details panel mode, pinned layout, automatic cleanup of the history, full resolution images, and window size and position.
- Every keyboard shortcut, including Win+V, can be changed or turned off, with conflicts shown before saving.
- Export of pinned items, categories, tags, templates, settings and optionally the history to one file, with an optional password, and import that merges or replaces.

## [0.4.0] - 2026-10-14

### Added

- Templates tab: reusable texts with a name, an emoji and fields filled in when pasting: `{data}`, `{godzina}` and `{schowek}`.
- A template can have its own shortcut that pastes it in any app.

## [0.3.0] - 2026-10-14

### Added

- Item details in a side panel or in place of the list, with the full content, a preview of images in their original resolution, paste options, organizing and information about the item.
- Editing text items in place (F2).
- **Paste as** plain text, **Save as…** to a file and **Open** for links, files and images.

## [0.2.0] - 2026-10-14

### Added

- Pinned tab: pinned items are kept until removed, and can have a category, #tags, an emoji and a color mark.
- Categories with a name, an emoji and a color, in a side column or as collapsible sections, and filtering by tag.
- Search by #tag.
- Trash: deleted items stay for 30 days and can be restored or deleted for good.

## [0.1.0] - 2026-10-14

### Added

- History window that opens with Win+V instead of the built-in panel, near the text cursor, or near the tray icon when opened from there.
- History without a limit that survives restarts: text, HTML, RTF, links, colors, images in their original resolution and copied files. Copying the same thing again moves it to the top.
- Search in the history (Ctrl+F or the magnifier button).
- Paste with a click or Enter, plain text with Shift+Enter, and the n-th item with Ctrl+1…9.
- **Clear history** with **Undo** for 10 seconds.
- Pausing saving from the **…** menu or the tray icon.
- Resizable window with remembered size, an always-on-top pin, light and dark mode, the Windows accent color and an acrylic background.
- History encrypted with a key protected by the Windows account. Content marked as private by apps is skipped.
- **Turn off Windows clipboard history** setting, on by default.
