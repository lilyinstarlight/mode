# .:mode:.

> I hope you enjoy an early preview of .:mode:., a side-scrolling puzzle platformer where you write the rules!
> 
> Fund our Kickstarter today and we will totally ship the game and not just take your money!

A 2D game engine based on SDL2, sol3, and Lua in C++ originally written for my CPSC 4160 class at Clemson University.


## Requirements

* SDL2{,\_image,\_mixer,\_ttf,\_gfx}
* yaml-cpp


## Issues

* width/height aren't calculated right for rotated sprites (I think this is an SDL2\_gfx issue)
* death dialog that shows engine-level commands should be shown on player death


## Extras

* scripting
* console
* animation states
* world (re)loading
* sprite indexing
* window icon
* sound
* background tiling
* dialog abstraction
* code injection
* physics raycasting
* platforms
* rigid body physics (partially implemented)
* menu (can already be implemented at runtime but no reference has been created)
* more that have not been put here yet because I am lazy


## Credits

* the creators of Lua - https://www.lua.org/authors.html
* the creators of sol3 - https://github.com/ThePhD/sol2/graphs/contributors
* sprites by Ben Golden taken from our game PyLink - https://github.com/ProjectPyLink/PyLink
* the fabulous Dr. Brian Malloy


## License

.:mode:. is licensed under the terms of the MIT License. Libraries and assets are licensed as follows:

* SDL2{,\_image,\_mixer,\_ttf,\_gfx} (dynamic linking) - Zlib
* yaml-cpp (dynamic linking) - MIT
* Lua (static linking) - MIT
* sol3 (header-only) - MIT
* PyLink (asset) - MIT
* Shoot\_01.wav (LittleRobotSoundFactory) (asset) - CC-BY-3.0
* PressStart2P.ttf (asset) - OFL-1.1
