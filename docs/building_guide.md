@title Building Guide (How to build the extension from source)

# Building Guide

This guide will provide you the necessary steps for building the extension from source, if you wish to do so.

1. Install Visual Studio 2019 (see [download site](https://visualstudio.microsoft.com/downloads/)).
2. Install GDK (Oct 2021) from Microsoft's GitHub repository ([here](https://github.com/microsoft/GDK/releases/tag/October_2021_Republish)).<br>
![](assets/gdkDownload.jpg) 
![](assets/gdkInstall.jpg)
3. Install CMAKE (see [download site](https://cmake.org/download/)).
4. Clone the GDK Extension repository (see [here](https://github.com/YoYoGames/GDKExtension)).
5. Open the Solution in `DLL/GDKExtension.sln` with Visual Studio 2019.
6. In the Visual Studio IDE, go to `Project Properties → C/C++ → Additional Include Directories` and add the following path: `C:\ProgramData\GameMakerStudio2\Cache\runtimes\<current-runtime>\yyc\include\` (where `<current-runtime>` refers to the current runtime version).<br>
![](assets/visualStudioProperties.jpg)
7. Select either one of these building targets: `Debug|Gaming.Desktop.x64` or `Release|Gaming.Desktop.x64` (other options won't work).<br>
![](assets/visualStudioTarget.jpg)
8. Build the project; the compiled DLL will then be exported to the folder `GDK_Project_GMS2/extensions/GDKExtension/`.
9. You've now finished the building process and can continue [setting up your project](GDK-Extension-Guides#GDK_Extension_Guides).


<br><br>

---