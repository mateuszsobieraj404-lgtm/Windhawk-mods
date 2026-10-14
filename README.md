# Windhawk Mods

Mods for [Windhawk](https://windhawk.net/) that extend and customize Windows. Each mod lives in its own folder with its own documentation and changelog, and is released independently.

## Mods

| Mod | What it does | Status | Downloads |
|---|---|---|---|
| [Clipboard Tweaks](clipboard-tweaks/) | Replaces Win+V with a more powerful, feature-rich clipboard | In development | [Releases](https://github.com/mateuszsobieraj404-lgtm/Windhawk-mods/releases?q=clipboard-tweaks&expanded=true) |

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

## Repository layout

```
<mod-id>/
  <mod-id>.wh.cpp   mod source with its metadata, readme and settings
  README.md         features, screenshots, usage, compatibility, known issues
  CHANGELOG.md      one section per released version
  images/           screenshots and GIFs used by the readmes
docs/
  mod-readme-template.md
```

The folder name, the file name and the mod's `@id` are always the same.

## Versioning and releases

- Each mod follows [Semantic Versioning](https://semver.org/) (`MAJOR.MINOR.PATCH`). `0.x` versions are pre-releases.
- The version is the mod's `@version` metadata value. To release a new version, bump `@version` and add a matching `## [x.y.z] - YYYY-MM-DD` section to the mod's `CHANGELOG.md`, following [Keep a Changelog](https://keepachangelog.com/).
- When the change reaches `main`, the [release workflow](.github/workflows/release.yml) tags it `<mod-id>-v<x.y.z>` and publishes a GitHub release with the changelog section as release notes and the `.wh.cpp` file attached. Versions that were already released are skipped.

## Documenting a mod

Every mod has two readmes with the same core text: the readme block inside `<mod-id>.wh.cpp`, which Windhawk shows on the mod's page, and `<mod-id>/README.md` on GitHub, which adds download and changelog links. Start from the [mod README template](docs/mod-readme-template.md).

## License

All mods in this repository are released under the [MIT License](LICENSE). Each mod also declares it in its metadata with `@license MIT`.
