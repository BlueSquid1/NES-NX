#include "NesColour.h"

// colours came from:
// https://wiki.nesdev.com/w/index.php/PPU_palettes

const rawColour NesColour::colourLookupIndex [64] = {
    {0x666666FF}, {0x002A88FF}, {0x1412A7FF}, {0x3B00A4FF}, {0x5C007EFF}, {0x6E0040FF}, {0x6C0600FF}, {0x561D00FF},
    {0x333500FF}, {0x0B4800FF}, {0x005200FF}, {0x004F08FF}, {0x00404DFF}, {0x000000FF}, {0x000000FF}, {0x000000FF},
    {0xADADADFF}, {0x155FD9FF}, {0x4240FFFF}, {0x7527FEFF}, {0xA01ACCFF}, {0xB71E7BFF}, {0xB53120FF}, {0x994E00FF},
    {0x6B6D00FF}, {0x388700FF}, {0x0C9300FF}, {0x008F32FF}, {0x007C8DFF}, {0x000000FF}, {0x000000FF}, {0x000000FF},
    {0xFFFEFFFF}, {0x64B0FFFF}, {0x9290FFFF}, {0xC676FFFF}, {0xF36AFFFF}, {0xFE6ECCFF}, {0xFE8170FF}, {0xEA9E22FF},
    {0xBCBE00FF}, {0x88D800FF}, {0x5CE430FF}, {0x45E082FF}, {0x48CDDEFF}, {0x4F4F4FFF}, {0x000000FF}, {0x000000FF},
    {0xFFFEFFFF}, {0xC0DFFFFF}, {0xD3D2FFFF}, {0xE8C8FFFF}, {0xFBC2FFFF}, {0xFEC4EAFF}, {0xFECCC5FF}, {0xF7D8A5FF},
    {0xE4E594FF}, {0xCFEF96FF}, {0xBDF4ABFF}, {0xB3F3CCFF}, {0xB5EBF2FF}, {0xB8B8B8FF}, {0x000000FF}, {0x000000FF}
};


rawColour NesColour::GetRawColour(colourIndex nesColourIndex)
{
    return colourLookupIndex[nesColourIndex];
}