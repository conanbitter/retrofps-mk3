# Texture pack file format

Texture pack file contains a bunch of textures in paletted format and palette for them. Palette indexes may be shifted if you want to use several texture packs simultaneously.

## Future functionality

- ability to force palette calculator to use some predefined colors.

## Binary format

Binary format may be changed in future versions of RGF. Current file format allows parsing in one step without seeking.

```
// PALETTE SECTION

u8 palette_size
u8 palette_offset
// palette_size+palette_offset must be no bigger than 256
struct {
    u8 r
    u8 g
    u8 b
} palette[palette_size]

// TEXTURE SECTION
u32 textures_count
struct {
    char name[16]
    // names must be unique, names can have spaces
    u32 width
    u32 height
    i16 transparent_color // -1 if there is no transparency
    u8 data[width*height]
} textures[textures_count]
```

## Project format

Files in this format intended to use with texture packer app.

```
#colors <number_of_colors>
#offset <palette_offset>
<texture_name> <file_name> [<tranparent_pixel_x> <tranparent_pixel_y>]
```

Texture name and file name can be putted in double quotes if they contain spaces