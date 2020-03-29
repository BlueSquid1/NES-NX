#ifndef NES_COLOUR
#define NES_COLOUR

#include "NES/Memory/Types.h"

typedef byte colourIndex;

union rawColour
{
    qword raw;
    struct
    {
        byte alpha;
        byte red;
        byte green;
        byte blue;
    }channels;
};

class NesColour
{
    private:
    static const rawColour colourLookupIndex [64];

    public:
    static rawColour GetRawColour(colourIndex nesColourIndex = 63);
};

#endif