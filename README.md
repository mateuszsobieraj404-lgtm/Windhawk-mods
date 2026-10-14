# Windhawk Mods

Mods for [Windhawk](https://windhawk.net/), the customization platform for Windows programs. Each mod lives in its own folder and is versioned and released on its own.

## Mods

| Mod | Description | Status |
|---|---|---|
| [Clipboard Tweaks](clipboard-tweaks/) | A Fluent 2 clipboard manager window that replaces the Win+V panel | In development |

Released versions of every mod are on the [Releases](https://github.com/mateuszsobieraj404-lgtm/Windhawk-mods/releases) page.

## Installing a mod

1. Install Windhawk from [windhawk.net](https://windhawk.net/).
2. Open the mod's latest release and download its `.wh.cpp` file.
3. In Windhawk, click **Create a new mod**, replace the editor contents with the contents of the file, click **Compile mod**, then **Exit editing mode**.

To update a mod, click **Edit** on its card in Windhawk, replace the code with the new version, and compile again.

## Repository layout

```
<mod-id>/
  <mod-id>.wh.cpp   mod source with its metadata, readme and settings
  README.md         features, screenshots, settings, known issues
  CHANGELOG.md      one section per released version
```

The folder name, the file name and the mod's `@id` are always the same.

## Versioning and releases

- Each mod follows [Semantic Versioning](https://semver.org/) (`MAJOR.MINOR.PATCH`). `0.x` versions are pre-releases.
- The version is the mod's `@version` metadata value. To release a new version, bump `@version` and add a matching `## [x.y.z] - YYYY-MM-DD` section to the mod's `CHANGELOG.md`, following [Keep a Changelog](https://keepachangelog.com/).
- When the change reaches `main`, the [release workflow](.github/workflows/release.yml) tags it `<mod-id>-v<x.y.z>` and publishes a GitHub release with the changelog section as release notes and the `.wh.cpp` file attached. Versions that were already released are skipped.
