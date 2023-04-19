#include <cstdint>

struct Pixel {
    uint8_t r, b, g;
};

class PixelTable {
    public:
        PixelTable();
        PixelTable(int32_t width, int32_t height);
        void setPixel(int32_t x, int32_t y, Pixel pixel);
        Pixel getPixel(int32_t x, int32_t y);
    private:
        Pixel* pixels;
        int32_t width, height;
};