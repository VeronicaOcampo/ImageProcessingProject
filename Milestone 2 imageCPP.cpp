#include "image.h"
#include <iostream>
#include <fstream>

Image::Image() {
    width = 0;
    height = 0;
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
    for(int i = 0; i < width * height; ++i) {
        pixels[i].red = static_cast<unsigned char>((pixels[i].red * other.pixels[i].red) / 255.0f + 0.5f);
        pixels[i].green = static_cast<unsigned char>((pixels[i].green * other.pixels[i].green) / 255.0f + 0.5f);
        pixels[i].blue = static_cast<unsigned char>((pixels[i].blue * other.pixels[i].blue) / 255.0f + 0.5f);
        clamp(pixels[i]);
    }
}

void Image::subtract(const Image& other) {
    if(width != other.width || height != other.height) {
        cerr << "Image dimensions do not match for subtraction." << endl;
        return;
    }
    for(int i = 0; i < width * height; ++i) {
        int red_b4_clamp = static_cast<int>(pixels[i].red) - static_cast<int>(other.pixels[i].red);
        int green_b4_clamp = static_cast<int>(pixels[i].green) - static_cast<int>(other.pixels[i].green);
        int blue_b4_clamp = static_cast<int>(pixels[i].blue) - static_cast<int>(other.pixels[i].blue);

        if(red_b4_clamp < 0) {
            pixels[i].red = 0;
        }
        else if(red_b4_clamp > 255) {
            pixels[i].red = 255;
        }
        else {
            pixels[i].red = red_b4_clamp;
        }

        if(green_b4_clamp < 0) {
            pixels[i].green = 0;
        }
        else if(green_b4_clamp > 255) {
            pixels[i].green = 255;
        }
        else {
            pixels[i].green = green_b4_clamp;
        }

        if(blue_b4_clamp < 0) {
            pixels[i].blue = 0;
        }
        else if(blue_b4_clamp > 255) {
            pixels[i].blue = 255;
        }
        else {
            pixels[i].blue = blue_b4_clamp;
        }
    }
}

void Image::screen(const Image& other) {
    for(int i = 0; i < width * height; ++i) {
        pixels[i].red = static_cast<unsigned char>(255-((255-pixels[i].red) * (255-other.pixels[i].red) / 255.0f) + 0.5f);
        pixels[i].green = static_cast<unsigned char>(255-((255-pixels[i].green) * (255-other.pixels[i].green) / 255.0f) + 0.5f);
        pixels[i].blue = static_cast<unsigned char>(255-((255-pixels[i].blue) * (255-other.pixels[i].blue) / 255.0f) + 0.5f);
        clamp(pixels[i]);
    }
}

void Image::overlay(const Image& other) {
    for(int i = 0; i < width * height; ++i) {
        float normal1_red = pixels[i].red / 255.0f;
        float normal2_red = other.pixels[i].red / 255.0f;
        if(normal2_red <= 0.5f) {
            pixels[i].red = static_cast<unsigned char>(2 * normal1_red * normal2_red * 255.0f + 0.5f);
        }
        else {
            pixels[i].red = static_cast<unsigned char>((1 - 2 * (1-normal1_red) * (1 - normal2_red)) * 255.0f + 0.5f);
        }
        clamp(pixels[i]);
        float normal1_green = pixels[i].green / 255.0f;
        float normal2_green = other.pixels[i].green / 255.0f;
        if(normal2_green <= 0.5f) {
            pixels[i].green = static_cast<unsigned char>(2 * normal1_green * normal2_green * 255.0f + 0.5f);
        }
        else {
            pixels[i].green = static_cast<unsigned char>((1 - 2 * (1-normal1_green) * (1 - normal2_green)) * 255.0f + 0.5f);
        }
        clamp(pixels[i]);
        float normal1_blue = pixels[i].blue / 255.0f;
        float normal2_blue = other.pixels[i].blue / 255.0f;
        if(normal2_blue <= 0.5f) {
            pixels[i].blue = static_cast<unsigned char>(2 * normal1_blue * normal2_blue * 255.0f + 0.5f);
        }
        else {
            pixels[i].blue = static_cast<unsigned char>((1 - 2 * (1 - normal1_blue) * (1 - normal2_blue)) * 255.0f + 0.5f);
        }
        clamp(pixels[i]);
    }
}

void Image::add_200_to_green() {
    for(int i = 0; i < width * height; ++i) {
        int green = pixels[i].green + 200;
        if(green > 255) {
            pixels[i].green = 255;
        }
        else {
            pixels[i].green = green;
        }
    }
}


void Image::scale_red_by_4() {
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

void Image::separate_channels(const string &red_filename, const string &green_filename, const string &blue_filename) {
    Image red_image = *this, green_image = *this, blue_image = *this;
    for(int i = 0; i < width * height; ++i) {
        red_image.pixels[i].green = red_image.pixels[i].blue = red_image.pixels[i].red;
        green_image.pixels[i].red = green_image.pixels[i].blue = green_image.pixels[i].green;
        blue_image.pixels[i].red = blue_image.pixels[i].green = blue_image.pixels[i].blue;
    }
    red_image.save(red_filename);
    green_image.save(green_filename);
    blue_image.save(blue_filename);
}


void Image::combine_rgb(const Image& green_layer, const Image& blue_layer) {
    if(width != green_layer.width || height != green_layer.height || width != blue_layer.width || height != blue_layer.height) {
        cerr << "Image dimensions do not match for combining RGB layers." << endl;
    }

    for(int i = 0; i < width * height; ++i) {
        pixels[i].green = green_layer.pixels[i].green;
        pixels[i].blue = blue_layer.pixels[i].blue;
    }
}

void Image::rotate_180degrees() {
    for(int i = 0; i < pixels.size() / 2; ++i) {
        Pixel temp = pixels[i];
        pixels[i] = pixels[pixels.size() -1 - i];
        pixels[pixels.size() -1 -i] = temp;
    }
}

void Image::onlyred() {
    for(int i = 0; i < width * height; i++) {
        pixels[i].green = pixels[i].red;
        pixels[i].blue = pixels[i].red;
    }
}

void Image::onlygreen() {
    for(int i = 0; i < width * height; i++) {
        pixels[i].red = pixels[i].green;
        pixels[i].blue = pixels[i].green;
    }
}

void Image::onlyblue() {
    for(int i = 0; i < width * height; i++) {
        pixels[i].red = pixels[i].blue;
        pixels[i].green = pixels[i].blue;
    }
}

void Image::addred(int value) {
    for(int i = 0; i < width * height; i++) {
        int new_red = pixels[i].red + value;
        if(new_red < 0) {
            new_red = 0;
        }
        if(new_red > 255) {
            new_red = 255;
        }
        pixels[i].red = new_red;
        clamp(pixels[i]);
    }
}

void Image::addgreen(int value) {
    for(int i = 0; i < width * height; i++) {
        int new_green = pixels[i].green + value;
        if(new_green < 0) {
            new_green = 0;
        }
        if(new_green > 255) {
            new_green = 255;
        }
        pixels[i].green = new_green;
        clamp(pixels[i]);
    }
}

void Image::addblue(int value) {
    for(int i = 0; i < width * height; i++) {
        int new_blue = pixels[i].blue + value;
        if(new_blue < 0) {
            new_blue = 0;
        }
        if(new_blue > 255) {
            new_blue = 255;
        }
        pixels[i].blue = new_blue;
        clamp(pixels[i]);
    }
}

void Image::scalered(int value) {
    for(int i = 0; i < width * height; i++) {
        int new_red = pixels[i].red * value;
        if(new_red < 0) {
            new_red = 0;
        }
        if(new_red > 255) {
            new_red = 255;
        }
        pixels[i].red = new_red;
        clamp(pixels[i]);
    }
}

void Image::scalegreen(int value) {
    for(int i = 0; i < width * height; i++) {
        int new_green = pixels[i].green * value;
        if(new_green < 0) {
            new_green = 0;
        }
        if(new_green > 255) {
            new_green = 255;
        }
        pixels[i].green = new_green;
        clamp(pixels[i]);
    }
}

void Image::scaleblue(int value) {
    for(int i = 0; i < width * height; i++) {
        int new_blue = pixels[i].blue * value;
        if(new_blue < 0) {
            new_blue = 0;
        }
        if(new_blue > 255) {
            new_blue = 255;
        }
        pixels[i].blue = new_blue;
        clamp(pixels[i]);
    }
}

Image::~Image() {

}