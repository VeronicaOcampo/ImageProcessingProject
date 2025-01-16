#include "image.h"
#include <iostream>
#include <fstream>

Image::Image() {
    width = 0;
    height = 0;
}

Image::Image(const Image& other) {
    width = other.width;
    height = other.height;
    pixels = other.pixels;
    header = other.header;
}

Image& Image::operator=(const Image& other) {
    if(this != &other) {
        width = other.width;
        height = other.height;
        pixels = other.pixels;
        header = other.header;
    }
    return *this;
}

Image::~Image() {

}

bool Image::load(const string& filename) {
    ifstream file(filename, ios::binary);
    if(!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return false;
    }

    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    width = header.width;
    height = header.height;
    pixels.resize(width * height);

    file.read(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(Pixel));

    file.close();
    return true;
}

bool Image::save(const string& filename) const{
    ofstream file(filename, ios::binary);
    if(!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return false;
    }

    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(Pixel));
    file.close();
    return true;
}

void Image::clamp(Pixel& pix) {
    if(pix.red < 0){
        pix.red = 0;
    }
    else if(pix.red > 255) {
        pix.red = 255;
    }
    if(pix.green < 0){
        pix.green = 0;
    }
    else if(pix.green > 255) {
        pix.green = 255;
    }
    if(pix.blue < 0){
        pix.blue = 0;
    }
    else if(pix.blue > 255) {
        pix.blue = 255;
    }
}

void Image::multiply(const Image& other) {
    if(width != other.width || height != other.height) {
        cerr <<"Image dimensions do not match for multiplication." << endl;
        return;
    }
    for(int i = 0; i < width * height; ++i) {
        pixels[i].red = (pixels[i].red * other.pixels[i].red) / 255.0f + 0.5f;
        pixels[i].green = (pixels[i].green * other.pixels[i].green) / 255.0f + 0.5f;
        pixels[i].blue = (pixels[i].blue * other.pixels[i].blue) / 255.0f + 0.5f;

    }
}

void Image::subtract(const Image& other) {
    for(int i = 0; i < width * height; ++i) {
        int red = pixels[i].red - other.pixels[i].red;
        int green = pixels[i].green - other.pixels[i].green;
        int blue = pixels[i].blue - other.pixels[i].blue;
        if(red < 0) {
            pixels[i].red = 0;
        }
        else {
            pixels[i].red = red;
        }
        if(green < 0) {
            pixels[i].green = 0;
        }
        else {
            pixels[i].green = green;
        }
        if(blue < 0) {
            pixels[i].blue = 0;
        }
        else {
            pixels[i].blue = blue;
        }
    }
}

void Image::screen(const Image& other) {
    for(int i = 0; i < width * height; ++i) {
        pixels[i].red = static_cast<unsigned char>(255 - ((255 - pixels[i].red) * (255 - other.pixels[i].red) / 255.0f) + 0.5f);
        pixels[i].green = static_cast<unsigned char>(255 - ((255 - pixels[i].green) * (255 - other.pixels[i].green) / 255.0f) + 0.5f);
        pixels[i].blue = static_cast<unsigned char>(255 - ((255 - pixels[i].blue) * (255 - other.pixels[i].blue) / 255.0f) + 0.5f);
        
    }
}

void Image::overlay(const Image& other) {
    for(int i = 0; i < width * height; ++i) {
        float norm1_red = pixels[i].red / 255.0f;
        float norm2_red = other.pixels[i].red / 255.0f;
        if(norm2_red <= 0.5f) {
            pixels[i].red = static_cast<unsigned char>(2 * norm1_red * norm2_red * 255.0f + 0.5f);
        }
        else {
            pixels[i].red = static_cast<unsigned char>((1 - 2 * (1-norm1_red) * (1 - norm2_red)) * 255.0f + 0.5f);
        }

        float norm1_green = pixels[i].green / 255.0f;
        float norm2_green = other.pixels[i].green / 255.0f;
        if(norm2_green <= 0.5f) {
            pixels[i].green = static_cast<unsigned char>(2 * norm1_green * norm2_green * 255.0f + 0.5f);
        }
        else {
            pixels[i].green = static_cast<unsigned char>((1 - 2 * (1-norm1_green) * (1 - norm2_green)) * 255.0f + 0.5f);
        }

        float norm1_blue = pixels[i].blue / 255.0f;
        float norm2_blue = other.pixels[i].blue / 255.0f;
        if(norm2_blue <= 0.5f) {
            pixels[i].blue = static_cast<unsigned char>(2 * norm1_blue * norm2_blue * 255.0f + 0.5f);
        }
        else {
            pixels[i].blue = static_cast<unsigned char>((1 - 2 * (1 - norm1_blue) * (1 - norm2_blue)) * 255.0f + 0.5f);
        }
    }
}

void Image::add_green() {
    for(int i = 0; i < width * height; ++i){
        int green = pixels[i].green + 200;
        if(green > 255) {
            pixels[i].green = 255;
        }
        else {
            pixels[i].green = green;
        }
    }
}

void Image::scale_red_blue() {
    for(int i = 0; i < width * height; ++i) {
        int new_red = static_cast<int>(pixels[i].red) * 4;
        if(new_red > 255) {
            pixels[i].red = 255;
        }
        else {
            pixels[i].red = new_red;
        }
        pixels[i].blue = 0;
    }
}

void Image::get_channels(const string &red_file, const string &green_file, const string &blue_file) {
    Image red_image = *this, green_image = *this, blue_image = *this;
    for(int i = 0; i < width * height; ++i) {
        red_image.pixels[i].green = red_image.pixels[i].blue = red_image.pixels[i].red;
        green_image.pixels[i].red = green_image.pixels[i].blue = green_image.pixels[i].green;
        blue_image.pixels[i].red = blue_image.pixels[i].green = blue_image.pixels[i].blue;
    }
    red_image.save(red_file);
    green_image.save(green_file);
    blue_image.save(blue_file);
}


void Image::mix_channels(Image &out_img, const Image &red_img, const Image &green_img, const Image &blue_img) {
    out_img.header = red_img.header;
    out_img.width = red_img.width;
    out_img.height = red_img.height;
    out_img.pixels.resize(out_img.width * out_img.height);

    for(int i = 0; i < out_img.width * out_img.height; ++i) {
        out_img.pixels[i].red = red_img.pixels[i].red;
        out_img.pixels[i].green = green_img.pixels[i].green;
        out_img.pixels[i].blue = blue_img.pixels[i].blue;
    }
}

void Image::rotate_180() {
    int n = pixels.size();
    for(int i = 0; i < n / 2; ++i) {
        Pixel temp = pixels[i];
        pixels[i] = pixels[n -1 -i];
        pixels[n -1 -i] = temp;
    }
}




