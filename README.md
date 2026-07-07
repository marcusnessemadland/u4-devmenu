[u4-devmenu](https://github.com/marcusnessemadland/u4-devmenu) - Uncharted 4 Dev Menu
============================================================================

<p align="center">
    <a href="#what-is-it">What is it?</a> -
    <a href="#installation-prebuilt">Installation</a> -
    <a href="#building">Building</a> -
</p>

[What is it?](https://github.com/marcusnessemadland/u4-devmenu)
-------------------------------------------------------------

A mod for the PC version of Uncharted 4 that enables the game's built-in dev menu / debug menu, giving access to developer and debug tools not normally exposed to players.

[Installation (Prebuilt)](https://github.com/marcusnessemadland/u4-devmenu)
-------------------------------------------------------------

1. Download the prebuilt release files.
2. Drag and drop them into your Uncharted 4 install directory (the same folder as the game's `.exe`).
3. Launch the game.
4. Press **L3 + O/B** in-game to enable the dev menu.

[Building](https://github.com/marcusnessemadland/u4-devmenu)
-------------------------------------------------------------

CMake is used for building IDE project solutions:

```bash
git clone https://github.com/marcusnessemadland/u4-devmenu.git
cd u4-devmenu
mkdir build
cd build
cmake ..
```

If you'd like the build to output directly into your Uncharted 4 install directory, set `RUNTIME_DIR` when configuring:

```bash
cmake .. -DRUNTIME_DIR="C:/Path/To/Uncharted4/Directory"
```

[Thank you](https://github.com/marcusnessemadland/u4-devmenu)
-------------------------------------------------------------

- [Cane556](https://github.com/gg3502)
- [TheMagicalBlob](https://github.com/TheMagicalBlob)
