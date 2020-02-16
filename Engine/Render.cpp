#include "Render.h"
#include "Window.h"
#include <assert.h>

Render::Render()
{
    pColorBuffer = reinterpret_cast<Color*>(
        _aligned_malloc(sizeof(Color) * Window::ScreenWidth * Window::ScreenHeight, 16u));
}

Render::~Render()
{
    // free sysbuffer memory (aligned free)
    if (pColorBuffer)
    {
        _aligned_free(pColorBuffer);
        pColorBuffer = nullptr;
    }
}

void Render::DrawIntRect(int x0, int y0, int x1, int y1, uint32 color, game_bitmap_struct* GameBitmapStruct)
{
    x0 = GamePlatform::ClampScreen(0, GameBitmapStruct->BitmapWidth, x0);
    x1 = GamePlatform::ClampScreen(0, GameBitmapStruct->BitmapWidth, x1);
    y0 = GamePlatform::ClampScreen(0, GameBitmapStruct->BitmapHeight, y0);
    y1 = GamePlatform::ClampScreen(0, GameBitmapStruct->BitmapHeight, y1);

    for (int y = y0; y < y1; y++)
    {
        uint32* pixels = (uint32*)GameBitmapStruct->BitmapMemory + x0 + GameBitmapStruct->BitmapWidth * y;
        for (int x = x0; x < x1; x++)
        {
            *pixels++ = color;
        }
    }
}

void Render::DrawFloatRect(game_vector2 p, game_vector2 half_size, uint32 color, game_bitmap_struct* GameBitmapStruct)
{
    float32 aspect_ratio_multiplier = (float32)GameBitmapStruct->BitmapHeight;
    if (GameBitmapStruct->BitmapWidth / GameBitmapStruct->BitmapHeight < 1.77f)
    {
        aspect_ratio_multiplier = (float32)GameBitmapStruct->BitmapWidth / 1.77f;
    }
    float32 scale = 0.001f;
    half_size.x *= aspect_ratio_multiplier * scale;
    half_size.y *= aspect_ratio_multiplier * scale;

    p.x *= aspect_ratio_multiplier * scale;
    p.y *= aspect_ratio_multiplier * scale;

    p.x += GameBitmapStruct->BitmapWidth * 0.5f;
    p.y += GameBitmapStruct->BitmapHeight * 0.5f;

   


    int x0 = (int)(p.x - half_size.x);
    int y0 = (int)(p.y - half_size.y);
    int x1 = (int)(p.x + half_size.x);
    int y1 = (int)(p.y + half_size.y);

    DrawIntRect(x0, y0, x1, y1, color, GameBitmapStruct);
}

void Render::DrawPixel(int dx, int dy, uint8 dR, uint8 dG, uint8 dB, uint8 dA, game_bitmap_struct* GameBitmapStruct)
{
    int Pitch = (dy * GameBitmapStruct->BitmapWidth + dx) * 4;
    uint8* Row = (uint8*)GameBitmapStruct->BitmapMemory;
    Row += Pitch;
    uint32* Pixel = (uint32*)Row;

    *Pixel++ = ((dA << 24) | ((dR << 16) | ((dG << 8) | dB)));
}

void Render::PutPixel(int x, int y, Color c)
{
    assert(x >= 0);
    assert(x < int(Window::ScreenWidth));
    assert(y >= 0);
    assert(y < int(Window::ScreenHeight));
    pColorBuffer[Window::ScreenWidth * y + x] = c;
}

void Render::DrawImage(int x0, int y0, int x1, int y1, game_bitmap_struct* GameBitmapStruct, uint32* Pixels, uint16 BytesPerPixel)
{
    uint32* pixels = nullptr;
    //If BMP is 32 bit aligned
    if (BytesPerPixel == 32)
    {
        for (sint32 y = y1; y > y0; --y)
        {
            pixels = (uint32*)GameBitmapStruct->BitmapMemory + x0 + GameBitmapStruct->BitmapWidth * y;
            for (sint32 x = x0; x < x1; ++x)
            {
                *pixels++ = *Pixels++;
            }
        }
    }
    //If BMP is 24 bit aligned
    else if(BytesPerPixel == 24)
    {

        //TODO: need to revise the code below for performance optimization
        unsigned char* buf = reinterpret_cast<unsigned char*>(Pixels);
        int i;
        int numBmpBytes;
        size_t numImgBytes;
        int ind = 0;
        int line;
        int temp;
        int ih, iw; /* Номера строки и столбца для отражения  */
        int new_ind; /* Новый индекс */
        int y = y1;
        //uint32* pixels = (uint32*)GameBitmapStruct->BitmapMemory + x0 + GameBitmapStruct->BitmapWidth * y;
        temp = x1 * 3;
        line = temp + x1 % 4; /* Длина строки с учетом выравнивания */
        numImgBytes = (4 * (x1 * y1));
        //pixels = (uint32*)malloc(numImgBytes);
        if (pixels)
        {
            //std::free((uint32*)pixels);
            VirtualFree(pixels, 0, MEM_RELEASE);
        }
        pixels = (uint32*)VirtualAlloc(0, numImgBytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

        /* Размер, отведенный на BMP в файле с учетом выравнивания */
        numBmpBytes = line * y1;
        for (i = 0; i < numBmpBytes; i++) 
        {
            unsigned int r, g, b;

            /* Пропускаем padding */
            if (i >= temp && (i % line) >= temp)
                continue;

            b = buf[i];
            i++;
            g = buf[i];
            i++;
            r = buf[i];


            /* Вычисляем новый индекс для отражения по вертикали */
            iw = ind % x1;
            ih = ind / x1;
            new_ind = iw + (y1 - ih - 1) * x1;

            pixels[new_ind] = (b | g << 8 | r << 16); // Corrected formular
            ind++;
        }
        for (int y = y0; y < y1; y++)
        {
            uint32* dest = (uint32*)GameBitmapStruct->BitmapMemory + x0 + GameBitmapStruct->BitmapWidth * y;
            for (int x = x0; x < x1; x++)
            {
                *dest++ = *pixels++;
            }
        }
        if (pixels)
        {
            //std::free((uint32*)pixels);
            VirtualFree(pixels, 0, MEM_RELEASE);
        }
    }
    else
    {
        //error
    }
}

void Render::DrawSpriteNonChroma(int x, int y, const Surface& s)
{
    DrawSpriteNonChroma(x, y, s.GetRect(), s);
}

void Render::DrawSpriteNonChroma(int x, int y, const RectI& srcRect, const Surface& s)
{
    assert(srcRect.left >= 0);
    assert(srcRect.right < s.GetWidth());
    assert(srcRect.top >= 0);
    assert(srcRect.bottom < s.GetHeight());
    for (int sy = srcRect.top; sy < srcRect.bottom; sy++)
    {
        for (int sx = srcRect.left; sx < srcRect.right; sx++)
        {
            PutPixel(x + sx - srcRect.left, y + sy - srcRect.top, s.GetPixel(sx, sy));
        }
    }
}

void Render::DrawSpriteNonChroma(int x, int y, RectI srcRect, const RectI& clip, const Surface& s)
{
    assert(srcRect.left >= 0);
    assert(srcRect.right < s.GetWidth());
    assert(srcRect.top >= 0);
    assert(srcRect.bottom < s.GetHeight());
    if (x < clip.left)
    {
        srcRect.left += clip.left - x;
        x = clip.left;
    }
    if (y < clip.top)
    {
        srcRect.top += clip.top - y;
        y = clip.top;
    }
    if (x + srcRect.GetWidth() > clip.right)
    {
        srcRect.right -= x + srcRect.GetWidth() - clip.right;
    }
    if (y + srcRect.GetHeight() > clip.bottom)
    {
        srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
    }
    for (int sy = srcRect.top; sy < srcRect.bottom; sy++)
    {
        for (int sx = srcRect.left; sx < srcRect.right; sx++)
        {
            PutPixel(x + sx - srcRect.left, y + sy - srcRect.top, s.GetPixel(sx, sy));
        }
    }
}

void Render::DrawSprite(int x, int y, RectI srcRect, const RectI& clip, const Surface& s, Color chroma)
{
    assert(srcRect.left >= 0);
    assert(srcRect.right < s.GetWidth());
    assert(srcRect.top >= 0);
    assert(srcRect.bottom < s.GetHeight());
    if (x < clip.left)
    {
        srcRect.left += clip.left - x;
        x = clip.left;
    }
    if (y < clip.top)
    {
        srcRect.top += clip.top - y;
        y = clip.top;
    }
    if (x + srcRect.GetWidth() > clip.right)
    {
        srcRect.right -= x + srcRect.GetWidth() - clip.right;
    }
    if (y + srcRect.GetHeight() > clip.bottom)
    {
        srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
    }
    for (int sy = srcRect.top; sy < srcRect.bottom; sy++)
    {
        for (int sx = srcRect.left; sx < srcRect.right; sx++)
        {
            const Color srcPixel = s.GetPixel(sx, sy);
            if (srcPixel != chroma)
            {
                PutPixel(x + sx - srcRect.left, y + sy - srcRect.top, srcPixel);
            }
        }
    }
}

void Render::DrawSprite(int x, int y, const Surface& s, Color chroma)
{
    DrawSprite(x, y, s.GetRect(), s, chroma);
}

void Render::DrawSprite(int x, int y, RectI srcRect, const Surface& s, Color chroma)
{
    DrawSprite(x, y, srcRect, s.GetRect(), s, chroma);
}

void Render::ClearScreen(Color c)
{
    for (sint32 y = 0; y < Window::ScreenHeight; ++y)
    {
        for (sint32 x = 0; x < Window::ScreenWidth; ++x)
        {
            PutPixel(x, y, c);
        }
    }
}

void Render::BeginFrame()
{
    // clear the sysbuffer
    memset(pColorBuffer, 0u, sizeof(Color) * Window::ScreenHeight * Window::ScreenWidth);
}

void Render::EndFrame(game_bitmap_struct* GameBitmapStruct)
{
    // setup parameters for copy operation
    Color* pDst = reinterpret_cast<Color*>(GameBitmapStruct->BitmapMemory);
    const size_t dstPitch = GameBitmapStruct->BitmapWidth * 4 / sizeof(Color);
    const size_t srcPitch = Window::ScreenWidth;
    const size_t rowBytes = srcPitch * sizeof(Color);
    // perform the copy line-by-line
    for (size_t y = 0u; y < Window::ScreenHeight; y++)
    {
        memcpy(&pDst[y * dstPitch], &pColorBuffer[y * srcPitch], rowBytes);
    }
}
