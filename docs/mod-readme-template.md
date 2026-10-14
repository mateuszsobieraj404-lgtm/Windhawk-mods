# Mod README template

Use this template for `<mod-id>/README.md` and for the readme block in `<mod-id>.wh.cpp`. It follows the most popular and best-rated mods in the Windhawk catalog: a one-sentence summary, a picture right away, features as short bullets with a bold label, and extra detail only where users need it.

## Rules

- **Two copies, one text.** The readme block in `<mod-id>.wh.cpp` (between `// ==WindhawkModReadme==` and `// ==/WindhawkModReadme==`) is what users see in Windhawk. `README.md` repeats it and adds the sections marked *GitHub only* below. Change both together.
- **Lead with what it does.** The first line under the title is one sentence that a user understands without knowing how Windows works.
- **Show it.** Put a screenshot or a short GIF right under the summary, and describe every image in its alt text.
- **Images.** Keep them in the mod's `images/` folder. In `README.md`, link them with a relative path (`images/main-window.png`). In the `.wh.cpp` readme, link them with a `raw.githubusercontent.com` URL pinned to a commit, so the picture never breaks when files move: `https://raw.githubusercontent.com/mateuszsobieraj404-lgtm/Windhawk-mods/<commit>/<mod-id>/images/main-window.png`.
- **Catalog limits for the `.wh.cpp` readme.** Markdown image syntax only (no HTML `<img>`), images hosted on `raw.githubusercontent.com` or `i.imgur.com`, still images under 2 MB, GIFs under 20 MB.
- **Explain only what isn't obvious.** Windhawk already shows every setting's name and description. The readme covers what they don't: how to open the mod, shortcuts, required Windows settings, and where data is stored.
- **Be honest about limits.** List known issues and the Windows versions the mod was tested on.
- **Plain style.** English, sentence-case headings without emoji, short paragraphs, `**Bold**` for UI labels exactly as Windows or Windhawk shows them.

## Template

~~~markdown
# <Mod name>

<One sentence: what the mod does and what the user gets from it.>

![<What the picture shows>](images/<screenshot>.png)

<!-- GitHub only -->
[Download the latest version](https://github.com/mateuszsobieraj404-lgtm/Windhawk-mods/releases?q=<mod-id>&expanded=true) · [Changelog](CHANGELOG.md) · [Report a bug](https://github.com/mateuszsobieraj404-lgtm/Windhawk-mods/issues/new)

## Features

- **<Feature>**: <what the user gets, in one line>.
- **<Feature>**: <what the user gets, in one line>.

## How to use

<How to open or trigger the mod and its main actions.>

| Shortcut | Action |
|---|---|
| <Keys> | <Action> |

## Settings

<Only the settings that need more than their description in Windhawk. Delete the section otherwise.>

## Data and privacy

<What the mod stores, where, and whether anything leaves the computer. Delete the section if the mod stores nothing.>

## Compatibility

- Tested on <Windows 11 24H2 (build 26100.xxxx)>.
- Windhawk 1.7.3 or newer.

## Known issues

- <Issue>. <Workaround, if any.>

## Related mods

- [<Mod name>](https://windhawk.net/mods/<mod-id>): <why it pairs well with this mod>.

<!-- GitHub only -->
## Installation

See [Installation](../README.md#installation) in the repository readme.
~~~
