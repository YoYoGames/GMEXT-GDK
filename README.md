# GameMaker Studio 2 - GDK Extension

An Extension for GameMaker Studio 2 (GMS2) that gives GMS2 Windows Target support for the GDK allowing them to be released on the Microsoft Store and use XBox Live functionality (for those developers that have access through id@xbox, see [this link](https://www.xbox.com/developers/id) for more information on id@xbox).

For more information on how to use the GDK Extension check our [tech blog](https://www.yoyogames.com/en/blog/gdk-extension) and for extra details on configuring the Partner Center check out our [zendesk article](https://help.yoyogames.com/hc/en-us/articles/4411044955793).

> [!WARNING]
> Only Windows x64 Target is supported by the GDK. Also make sure the - ✅Copy exe to output folder - is selected from the Game Options -> Windows tab.

--- 

## Contents of this repository

This repository contains the source code for the DLL that implements the GDK functionality that is exposed to GameMaker games, it is written in C++. It also contains an example GMS2 project that contains the extension definition and illustrates how to use the extension.

---

## Building this Extension


1. Install [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/) 
2. Install [GDK Apr25](https://github.com/microsoft/GDK/releases/tag/April-2025-v2504.0.4020)
3. Install [CMAKE](https://cmake.org/download/)
4. Clone this repository or download the package

> [!NOTE]
> The repository has submodules that you should initialise. If you opt to use the release package the **modules are already included**.

5. Open the Visual Studio 2022
6. Open the Solution in `<project_root>/extensions/GDKExtension/gdkextension_windows/GDKExtension.sln`
7. Select solution platform - **Gaming.Desktop.x64**
9. Select solution configuration - **Release** or **Debug**
10. Build

> [!TIP]
> After building the output `.dll` will be copied into the extension folder.

> [!IMPORTANT]
> Version 3.X.X requires GDK version 250400 to work properly.

---

## Documentation

* Check [the documentation](../../wiki)

The online documentation is regularly updated to ensure it contains the most current information. For those who prefer a different format, we also offer a HTML version. This HTML is directly converted from the GitHub Wiki content, ensuring consistency, although it may follow slightly behind in updates.

We encourage users to refer primarily to the GitHub Wiki for the latest information and updates. The HTML version, included with the extension and within the demo project's data files, serves as a secondary, static reference.

Additionally, if you're contributing new features through PR (Pull Requests), we kindly ask that you also provide accompanying documentation for these features, to maintain the comprehensiveness and usefulness of our resources.

---
