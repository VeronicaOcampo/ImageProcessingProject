#pragma once
#include <vector>
#include <string>
using namespace std;

struct Pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;

    Pixel(unsigned char b = 0, unsigned char g = 0, unsigned char r = 0) {
        blue = b;
        green = g;
        red = r;
    }
};
#pragma pack(push, 1)
struct Header {
    char idLength = 0;
    char colorMapType = 0;
    char dataTypeCode = 2;
    short colorMapOrigin = 0;
    short colorMapLength = 0;
    char colorMapDepth = 0;
    short xOrigin = 0;
    short yOrigin = 0;
    short width = 0;
    short height = 0;
    char bitsPerPixel = 24;
    char imageDescriptor = 0;
};
#pragma pack(pop)

class Image {
public:
    Image();
    Image(const Image& other);
    Image& operator=(const Image& other);
    ~Image();

    vector<Pixel> pixels;
    short width;
    short height;
    Header header;

    bool load(const string& filename);
    bool save(const string& filename) const;
    void clamp(Pixel& pix);
    void multiply(const Image& other);
    void subtract(const Image& other);
    void screen(const Image& other);
    void overlay(const Image& other);
    void add_green();
    void scale_red_blue();
    void get_channels(const string &red_file, const string &green_file, const string &blue_file);
    void mix_channels(Image& out_img, const Image& red_img, const Image& green_img, const Image& blue_img);
    void rotate_180();

};