[![image](https://github.com/user-attachments/assets/9fb86cc4-99a3-44cb-b7cc-ca05259e9dad)](https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b)

# Minimal D3D11 sprite renderer NEO
**Ultra-compact sprite rendering code with example frame animation logic.** This release contains tech bits from the upcoming SuperNeo™ 2D game engine and includes rotation, anchor/pivot point, color filtering, alpha blending and built-in antialiased point sampling. As usual: complete, runnable single-function app. ~150 LOC. No modern C++, OOP or (other) obscuring cruft.

[![317289895-7ed6c416-7608-44f6-88d9-fbd6641ddfe2](https://github.com/user-attachments/assets/32a09661-6d80-4552-b852-18755c267e74)](https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b#file-cpu-cpp-L201-L221)

Sprites are rendered back-to-front (AKA "painter's algorithm") in the order they are submitted, as one draw call. The provided setup employs a single texture atlas containing all the sprite graphics.

The renderer is "immediate mode" in the sense that a fresh batch of sprite data (an array of `spritedesc` entries specifying what textures to use, their position, scale, rotation etc) is uploaded every frame. The texture data itself is retained on the GPU.

ARGB filtering & alpha blending:

[![color tinting](https://gist.github.com/assets/5618797/3a8db764-8d3e-425a-8a86-b7ad44a1809d)](https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b#file-cpu-cpp-L217)[![alpha blending](https://gist.github.com/assets/5618797/f140a10d-78bf-4626-8c49-dfec244fab56)](https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b#file-cpu-cpp-L217)

Custom antialiased point sampling that works for smooth sub-pixel movement, non-integer scaling and arbitrary rotation:

[![spacer281](https://gist.github.com/assets/5618797/d6bbf37a-512b-4049-b380-f744a0e6d50d)](#)[![move double](https://gist.github.com/assets/5618797/a35d720d-db29-4652-bc9e-f8facc0c4759)](https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b#file-cpu-cpp-L214)

[![spacer281](https://gist.github.com/assets/5618797/d6bbf37a-512b-4049-b380-f744a0e6d50d)](#)[![scale double](https://gist.github.com/assets/5618797/7ba85821-e2c5-4f5f-8b61-ba3a6e4d9eb6)](https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b#file-cpu-cpp-L215)

[![spacer281](https://gist.github.com/assets/5618797/d6bbf37a-512b-4049-b380-f744a0e6d50d)](#)[![rotate double](https://gist.github.com/assets/5618797/f6e069f7-418b-4b4b-96f1-0140e13ae2da)](https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b#file-cpu-cpp-L216)

Sprite textures must be separated by at least a 1-pixel wide, fully transparent gap (0x00000000, but indicated in green here): 

[![spacer212](https://gist.github.com/assets/5618797/fc52aae1-7ad1-4966-ad03-466ce697bbbc)](#)[![sprite_sheet_padding](https://gist.github.com/assets/5618797/bbfaff36-c2e1-4b01-8110-1070077a9179)](https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b#file-spritesheet-h-L15-L74)

Anchor/pivot point: `(0.0, 0.0)` is top-left, `(1.0, 1.0)` is bottom-right, but can be set to anywhere [inside or outside](https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b#file-cpu-cpp-L213) sprite:

[![spacer354](https://gist.github.com/assets/5618797/1753adee-d051-4fb7-8d8d-0e57d78a54cd)](#)[![anchor topleft](https://gist.github.com/assets/5618797/711aa4b0-f2e0-4c5f-88c6-fd64de5a6a0b)](https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b#file-cpu-cpp-L213) [![anchor bottomright](https://gist.github.com/assets/5618797/cb609934-b1ec-461c-ba86-b37a7744021f)](https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b#file-cpu-cpp-L213)

The sprite's `position` corresponds to the position of the anchor point on screen, with the sprite texture aligned accordingly. Similarly, `rotation` and `scale` (including horizontal and vertical mirroring) are done around the anchor (pivot) point.

The [antialiasing technique](https://gist.github.com/d7samurai/9f17966ba6130a75d1bfb0f1894ed377) used in this renderer requires premultiplied alpha textures. [TexPrep](https://gist.github.com/d7samurai/9f17966ba6130a75d1bfb0f1894ed377#texprep) is a small (~17 KB) commandline tool that can read most image formats, convert to premultiplied alpha and output as PNG, BMP, BIN or [TXT](https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b#file-spritesheet-h-L15-L74).

Also see [Minimal D3D11 sprite renderer](https://gist.github.com/d7samurai/8f91f0343c411286373161202c199b5c) and the original [Minimal D3D11](https://gist.github.com/d7samurai/261c69490cce0620d0bfc93003cd1052) reference for adjacent / elaborated example code.

#### Sprite sheet uses characters from [Tiny Heroes @ CraftPix.net](https://craftpix.net/freebies/free-pixel-art-tiny-hero-sprites/).

###### Follow on [Twitter](https://twitter.com/d7samurai) for future announcements. Soon™:
#
[![sponsored by SuperNeo copy 4](https://gist.github.com/assets/5618797/9fc0cf54-cdf7-4944-9fbc-3063716282e0)](https://twitter.com/d7samurai)


