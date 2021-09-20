# Building under Windows

## Development Environment

These steps has only been tested for MSVC 2019 and Qt 5.15.2.

### Installing Git Bash

1. Download Git Bash [here](https://git-scm.com/download/win).
2. When installing be sure to add it to `PATH`, if not do it later.
    * By default, it should be `C:\Program Files\Git\cmd\`.

### Installing Meson / Ninja

1. Go to Meson's releases [page](https://github.com/mesonbuild/meson/releases).
2. Choose the latest Windows build, an `.msi` file.
3. When installing, be sure to both install Meson and Ninja.
4. Add `meson.exe` and `ninja.exe` directory to `PATH`.
    * By default, it should be `C:\Program Files\Meson\`.

### Installing Qt5

1. Downloads the Qt installer [here](https://www.qt.io/download-thank-you?os=windows).
2. When choosing components, you can unselect everything except `Qt\Qt 5.15.2\MSVC 2019 64-bit`.
3. Add Qt5 binary directory to `PATH`.
    * By default, it should be `C:\Qt\5.15.2\msvc2019_64\bin\`.

### Installing Toolchain

1. Go to Visual Studio download [page](https://visualstudio.microsoft.com/downloads/).
2. Under "Tools for Visual Studio 2019", download "Build Tools for Visual Studio 2019".
3. When selecting components, select "Desktop development with C++".
4. Add `vcvars64.bat` directory to `PATH`.
    * By default, it should be `C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\`.

## Building

1. Open a command line prompt (`cmd.exe`).
2. Execute `vcvars64.bat`.
3. Go to the root directory of this project.
4. Create a `build` directory and go into it.
5. Execute `meson ..`, then `ninja`.
6. Create a `deploy` directory, copy `canary.exe` and go into it.
7. Execute `windeployqt canary.exe --qml C:\Qt\5.15.2\msvc2019_64\qml\`.
