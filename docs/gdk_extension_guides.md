@title Quick Start Guide (Project setup, sandboxes & configuration files)

# Quick Start Guide

You will initially need to set up your project for the GDK Extension to work properly. This guide covers all aspects of your project that you will need to configure.

## Project Setup

In order to build your project using the extension you need to import it and make some changes to your project:

1. Import the local package ( **.yymps** ) provided with the public release version.
2. Use the 64-bit Windows runtime. To change this go into **Game Options → Windows → General** and enable " **Use x64 Windows Runtime** ".<br>
![](assets/Setup1.jpg) 
3. Also make sure you enable the **"Copy exe to output folder"** setting: <br>
![](assets/gdkCopyToDestination.png)

> [!IMPORTANT]
> These steps are required only for older versions of the GameMaker:
>
> * **For GameMaker 2022.3 (or previous)** : Also make sure you disable the subst from within the **Preferences → General Settings → Compiling** menu, otherwise this extension will not work:
>
> ![](assets/gdkDisableSubst.png)
>
> * **For GameMaker 2022.2 (or previous)** : The extension comes with two **.bat** files: `post_package_step.bat` and `post_run_step.bat` (placed inside the extension's folder). These files need to be moved into your project's root folder, which is the same directory as your project's **.yyp** file:
> 
> ![](assets/Setup2.jpg)

After finishing this we'll set up the config file required for the Microsoft Store.

## Sandbox Get/Set

  When using Xbox Live features, you might need to change your PC sandboxes (see [official documentation](https://docs.microsoft.com/en-us/gaming/gdk/_content/gc/live/test-release/tools/live-pc-sandbox-switcher)) so you can test those same features. For this purpose, you can run the GDK Command Line (under `Start → All Apps → Microsoft GDK → Desktop VS 2018 Gaming Command Prompt`) and use one of the following commands:

* `XblPCSandbox /get` (returns the current sandbox, default is RETAIL)
* `XblPCSandbox <sandbox>` (changes the sandbox, where *`<sandbox>`* refers to your sandbox name)
* `XblPCSandbox RETAIL` (sets the sandbox back to RETAIL)

> [!NOTE]
> Sandbox names are case sensitive.

## Config File

To build and run your project using the GDK Extension, it's necessary to set up and include a `MicrosoftGame.Config` file:

1. This config file can be created using Microsoft's MicrosoftGame Editor tool (for more information refer to its [documentation page](https://docs.microsoft.com/en-us/gaming/gdk/_content/gc/system/overviews/microsoft-game-config/microsoftgameconfig-editor)) or can be copied from the [demo project](https://github.com/YoYoGames/GDKExtension/tree/main/GDK_Project_GMS2) and edited manually.<br>

![](assets/Setup3.jpg)

> [!NOTE]
> The `ExecutableList/Executable/Name` property should be set to the value in `GameOptions → Windows → Executable Name`.

2. Add the `MicrosoftGame.Config` file to the Included Files of your project.
3. Depending on the image file names specified in the `ShellVisuals` tag you will need to add those to the Included Files as well.<br>

![](assets/Setup4.jpg)

  Upon finishing this setup and following the [Project Setup](#project-setup) section above, you should be ready to run and test your project.

## Manifest File

  This step is not required unless you are using the event-based functions from the Xbox Live module (this requires enabling of **Event-Based Stats** on the Partner Center).
  If you are using the event-based system from Xbox Live module you need to provide GameMaker with some extra information about the **statistics**, **events** and **leaderboards** created on you Partner Center's dashboard. For this you need to do the following:

1. Log in to your **Partner Center** account, open your game's configuration page and go into the **Xbox Services → Gameplay Settings** where you can define your stats.<br>

![](assets/Setup5.jpg)

2. From here you will need to select **Player stats → Stat rules** the drop-down menu at the top.<br>

![](assets/Setup6.jpg)

3. On the page that opens, click on the " **Download published events manifest** " button.<br>

![](assets/Setup7.jpg)

4. Add the downloaded file to your project's **Included Files (/datafiles)** folder.

  After setting up this manifest file, you will be able to use event-based stats and leaderboards in your project.

> [!IMPORTANT]
> If you make any changes to your events on the Partner Center dashboard, you'll need to republish those changes and download a new **manifest** file.


<br><br>

---

## Shell Localization Guide

  This feature allows you to define the title's Shell presence. For example, Images and Names. Used during registration to surface the title in the Shell. In order to use this the user needs to make some modifications to both their `MicrosoftGame.Config` file and their project:

  - In their `MicrosoftGame.Config` file, they need to:
    - Change the value of `DefaultDisplayName` in `ShellVisuals` to `"ms-resource:ApplicationDisplayName"`
    - Change the value of `Description` in `ShellVisuals` to `"ms-resource:ApplicationDescription"`
    - Add a section which declares which languages they want to support using standard language\region codes (this should be in the `Game` section), i.e.:

```
<Resources>
    <Resource Language="en-us" />
    <Resource Language="en-gb" />
    <Resource Language="de-de" />
</Resources>
```

  - In their project developers need to add some folders to included files:
  
    1. In the root of included files (i.e.: `datafiles`) they should add a `GDKExtensionStrings` folder
    2. Inside the `GDKExtensionStrings` folder they should add one for each supported language, i.e. `en-us` for American English and `de-de` for German, so for the above list of languages the directory structure would be:

    <br>

    ```js
    GDKExtensionStrings\en-us
    GDKExtensionStrings\en-gb
    GDKExtensionStrings\de-de
    ```

    <br>

    3. Inside the base `GDKExtensionStrings` folder they should add an XML file named `resources.resw` which will contains the fallback language info and should look like the following (where the values should be replaced with the required defaults):

    ```xml
    <?xml version="1.0" encoding="utf-8"?>
    <root>
        <data name="ApplicationDescription">
            <value>Default App Description Here</value>
        </data>
        <data name="ApplicationDisplayName">
            <value>Default Display Name Here</value>
        </data>
    </root>
    ```

    4. Inside each of the language-specific folders they should add another `resources.resw` file with the appropriate values for that language.