# Windhawk Mods

Mods for [Windhawk](https://windhawk.net/) that extend and customize Windows. Each mod lives in its own folder with its own documentation and changelog, and is released independently.

## Mods

| Mod | What it does | Status | Downloads |
|---|---|---|---|
| [Clipboard Tweaks](clipboard-tweaks/) | Replaces Win+V with a more powerful, feature-rich clipboard | Pre-release (0.7.0) | [Releases](https://github.com/mateuszsobieraj404-lgtm/Windhawk-mods/releases?q=clipboard-tweaks&expanded=true) |

## Requirements

- Windows 10 or Windows 11. Each mod lists the versions it supports.
- [Windhawk](https://windhawk.net/) 1.7.3 or newer.

## Installation

1. Install Windhawk.
2. Open the mod's page from the table above and download the `.wh.cpp` file from its latest release.
3. In Windhawk, click **Create a new mod**. An editor opens with sample code.
4. Replace all of the editor's contents with the contents of the downloaded file.
5. Click **Compile mod**, then **Exit editing mode**. The mod is now installed and enabled.

**Updating:** click **Edit** on the mod's card in Windhawk, replace the code with the new version, and compile it again. The mod's settings are kept.

**Uninstalling:** open the mod's **Details** in Windhawk and click **Remove**.

## Support

Found a bug or have an idea? [Open an issue](https://github.com/mateuszsobieraj404-lgtm/Windhawk-mods/issues/new) and include:

- the mod's name and version,
- your Windows version (press Win+R and run `winver`) and Windhawk version,
- the steps that lead to the problem,
- the mod's log: in the mod's **Details**, open the **Advanced** tab, set **Debug logging** to **Mod logs**, click **Show log output**, and reproduce the problem.

## License

All mods in this repository are released under the [MIT License](LICENSE). Each mod also declares it in its metadata with `@license MIT`.
