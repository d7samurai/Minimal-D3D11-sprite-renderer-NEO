[![image](https://github.com/user-attachments/assets/9fb86cc4-99a3-44cb-b7cc-ca05259e9dad)](https://twitter.com/d7samurai)

# Minimal D3D11 sprite renderer NEO
**Ultra-compact sprite rendering code with example frame animation logic.** This release contains tech bits from the upcoming SuperNeo™ 2D game engine and includes anchor/pivot point, rotation, color filtering, alpha blending and built-in antialiased point sampling. As usual: complete, runnable single-function app. ~150 LOC. No modern C++, OOP or (other) obscuring cruft. The program produces this output:

[![317289895-7ed6c416-7608-44f6-88d9-fbd6641ddfe2](https://github.com/user-attachments/assets/32a09661-6d80-4552-b852-18755c267e74)](https://github.com/d7samurai/Minimal-D3D11-sprite-renderer-NEO/blob/main/cpu.cpp#L201-L221)

Sprites are rendered back-to-front (AKA "painter's algorithm") in the order they are submitted, as one draw call. The provided setup employs a single texture atlas containing all the sprite graphics.

The renderer is "immediate mode" in the sense that a fresh batch of sprite data (an array of `spritedesc` entries specifying what textures to use, their position, scale, rotation etc) is uploaded every frame. The texture data itself is retained on the GPU.

ARGB filtering & alpha blending:

[![315867464-3a8db764-8d3e-425a-8a86-b7ad44a1809d](https://github.com/user-attachments/assets/5ac4ffa2-0b8b-4ea4-97bf-00872770fba5)](https://github.com/d7samurai/Minimal-D3D11-sprite-renderer-NEO/blob/main/cpu.cpp#L217)[![315875615-f140a10d-78bf-4626-8c49-dfec244fab56 (1)](https://github.com/user-attachments/assets/d69f6a0c-15a3-49d4-8f73-712a33ede275)](https://github.com/d7samurai/Minimal-D3D11-sprite-renderer-NEO/blob/main/cpu.cpp#L217)

Custom antialiased point sampling that works for smooth sub-pixel movement, non-integer scaling and arbitrary rotation:

[![image](https://github.com/user-attachments/assets/9cacb366-6eb1-48f2-af2a-231b58393948)](#)[![316345019-a35d720d-db29-4652-bc9e-f8facc0c4759 (1)](https://github.com/user-attachments/assets/d6390a0e-f2e6-4bb6-ab6b-ad8ff318f478)](https://github.com/d7samurai/Minimal-D3D11-sprite-renderer-NEO/blob/main/cpu.cpp#L214)

[![image](https://github.com/user-attachments/assets/9cacb366-6eb1-48f2-af2a-231b58393948)](#)[![316345024-7ba85821-e2c5-4f5f-8b61-ba3a6e4d9eb6 (1)](https://github.com/user-attachments/assets/2161ce55-c77b-4ed0-abde-7893863f1f58)](https://github.com/d7samurai/Minimal-D3D11-sprite-renderer-NEO/blob/main/cpu.cpp#L215)

[![image](https://github.com/user-attachments/assets/9cacb366-6eb1-48f2-af2a-231b58393948)](#)[![316345026-f6e069f7-418b-4b4b-96f1-0140e13ae2da (1)](https://github.com/user-attachments/assets/bda1e233-7cf4-4ea1-bce4-2493a13a6721)](https://github.com/d7samurai/Minimal-D3D11-sprite-renderer-NEO/blob/main/cpu.cpp#L216)

Sprite textures must be separated by at least a 1-pixel wide, fully transparent gap (0x00000000, but indicated in green here): 

[![image](https://github.com/user-attachments/assets/04124b9b-8838-4467-92eb-2ea03957da93)](#)[![image](https://github.com/user-attachments/assets/d852547c-01f1-4e03-9456-8ff65a3fb707)](https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b#file-spritesheet-h-L15-L74)

Anchor/pivot point: `(0.0, 0.0)` is top-left, `(1.0, 1.0)` is bottom-right, but can be set to anywhere [inside or outside](https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b#file-cpu-cpp-L213) sprite:

[![image](https://github.com/user-attachments/assets/bdc9f4e5-e97f-4437-a495-c419874bfd6a)](#)[![image](https://github.com/user-attachments/assets/4a262b4a-287e-4f07-bcd9-822bfca3e77f)](https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b#file-cpu-cpp-L213) [![image](https://github.com/user-attachments/assets/4d283496-dba7-4d6c-995b-62b7dd948b7e)
](https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b#file-cpu-cpp-L213)

The sprite's `position` corresponds to the position of the anchor point on screen, with the sprite texture aligned accordingly. Similarly, `rotation` and `scale` (including horizontal and vertical mirroring) are done around the anchor (pivot) point.

The [antialiasing technique](https://gist.github.com/d7samurai/9f17966ba6130a75d1bfb0f1894ed377) used in this renderer requires premultiplied alpha textures. [TexPrep](https://gist.github.com/d7samurai/9f17966ba6130a75d1bfb0f1894ed377#texprep) is a small (~17 KB) commandline tool that can read most image formats, convert to premultiplied alpha and output as PNG, BMP, BIN or [TXT](https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b#file-spritesheet-h-L15-L74).
#
Also see [Minimal D3D11 sprite renderer](https://gist.github.com/d7samurai/8f91f0343c411286373161202c199b5c) and the original [Minimal D3D11](https://gist.github.com/d7samurai/261c69490cce0620d0bfc93003cd1052) gist reference for adjacent / elaborated example code.

#### Sprite sheet uses characters from [Tiny Heroes @ CraftPix.net](https://craftpix.net/freebies/free-pixel-art-tiny-hero-sprites/).

###### Follow on [Twitter](https://twitter.com/d7samurai) for future announcements. Soon™:
#
[![image](https://github.com/user-attachments/assets/9fb86cc4-99a3-44cb-b7cc-ca05259e9dad)](https://twitter.com/d7samurai)












