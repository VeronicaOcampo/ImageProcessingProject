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
    vector<Pixel> pixels;
    short width;
    short height;


    Image();
    bool load(const string& filename);
    bool save(const string& filename) const;
    void clamp(Pixel& px);
    void multiply(const Image& other);
    void subtract(const Image& other);
    void screen(const Image& other);
    void overlay(const Image& other);
    void add_200_to_green();
    void scale_red_by_4();
    void separate_channels(const string& red_filename, const string& green_filename, const string& blue_filename);
    void combine_rgb(const Image& green_layer, const Image& blue_layer);
    void rotate_180degrees();
    void onlyred();
    void onlygreen();
    void onlyblue();
    void addred(int value);
    void addgreen(int value);
    void addblue(int value);
    void scalered(int value);
    void scalegreen(int value);
    void scaleblue(int value);
    bool is_valid_file(const string &filename);
    Header header;

    ~Image();
};