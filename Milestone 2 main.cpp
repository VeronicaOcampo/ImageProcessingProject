#include "image.h"
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>

void print_help() {
    cout << "Project 2: Image Processing, Summer 2024" << endl;
    cout << endl;
    cout << "Usage:" << endl;
    cout << "\t./project2.out [output] [firstImage] [method] [...]" << endl;
}

bool is_valid_file(const string &filename) {
    return filename.size() > 4 && filename.substr(filename.size() - 4) == ".tga";
}

bool file_exists(const string &filename) {
    ifstream file(filename);
    return static_cast<bool>(file);
}

int main(int argc, char *argv[]) {
    if(argc == 1 || (argc == 2 && strcmp(argv[1], "--help") == 0)) {
        print_help();
        return 0;
    }
    if(argc < 3 || !is_valid_file(argv[1]) || !is_valid_file(argv[2])) {
        cout << "Invalid file name." << endl;
        return 1;
    }
    string output_filename = argv[1];
    string input_filename = argv[2];
    Image tracking_image;
    if(!tracking_image.load(input_filename)) {
        cout << "File does not exist." << endl;
        cerr << "Error opening file: " << input_filename << endl;
        return 1;
    }
    for(int i = 3; i < argc;) {
        string method = argv[i++];
        if(method == "multiply") {
            if(i >= argc) {
                cout << "Missing argument." << endl;
                return 1;
            }
            if(!is_valid_file(argv[i])) {
                cout << "Invalid argument, invalid file name." << endl;
                return 1;
            }
            Image other_image;
            if(!other_image.load(argv[i++])) {
                cout << "Invalid argument, file does not exist." << endl;
                return 1;
            }
            tracking_image.multiply(other_image);
        }
        else if(method == "subtract") {
            if(i >= argc) {
                cout << "Missing argument." << endl;
                return 1;
            }
            if(!is_valid_file(argv[i])) {
                cout << "Invalid argument, invalid file name." << endl;
                return 1;
            }
            Image other_image;
            if(!other_image.load(argv[i++])) {
                cout << "Invalid argument, file does not exist." << endl;
                return 1;
            }
            tracking_image.subtract(other_image);
        }
        else if(method == "overlay") {
            if(i >= argc) {
                cout << "Missing argument." << endl;
                return 1;
            }
            if(!is_valid_file(argv[i])) {
                cout << "Invalid argument, invalid file name." << endl;
                return 1;
            }
            Image other_image;
            if(!other_image.load(argv[i++])) {
                cout << "Invalid argument, file does not exist." << endl;
                return 1;
            }
            tracking_image.overlay(other_image);
        }
        else if(method == "screen") {
            if(i >= argc) {
                cout << "Missing argument." << endl;
                return 1;
            }
            if(!is_valid_file(argv[i])) {
                cout << "Invalid argument, invalid file name." << endl;
                return 1;
            }
            Image other_image;
            if(!other_image.load(argv[i++])) {
                cout << "Invalid argument, file does not exist." << endl;
                return 1;
            }
            tracking_image.screen(other_image);
        }
        else if(method == "combine" && i + 1 < argc) {
            if(i >= argc) {
                cout << "Missing argument." << endl;
                return 1;
            }
            if(!is_valid_file(argv[i])) {
                cout << "Invalid argument, invalid file name." << endl;
                return 1;
            }
            Image green_layer, blue_layer;
            if(!green_layer.load(argv[i++]) || !blue_layer.load(argv[i++])) {
                cout << "Invalid argument, file does not exist." << endl;
                return 1;
            }
            tracking_image.combine_rgb(green_layer, blue_layer);
        }
        else if(method == "flip") {
            tracking_image.rotate_180degrees();
        }
        else if(method == "onlyred") {
            tracking_image.onlyred();
        }
        else if(method == "onlygreen") {
            tracking_image.onlygreen();
        }
        else if(method == "onlyblue") {
            tracking_image.onlyblue();
        }
        else if(method == "addred" || method == "addgreen" || method == "addblue") {
            if(i >= argc) {
                cerr << "Missing argument." << endl;
                return 1;
            }
            int value;
            try {
                value = stoi(argv[i++]);
            }
            catch(const invalid_argument &) {
                cout << "Invalid argument, expected number." << endl;
                return 1;
            }
            if(method == "addred") {
                tracking_image.addred(value);
            }
            if(method == "addgreen") {
                tracking_image.addgreen(value);
            }
            if(method == "addblue") {
                tracking_image.addblue(value);
            }
        }
        else if(method == "scalered" || method == "scalegreen" || method == "scaleblue") {
            if(i >= argc) {
                cerr << "Missing argument." << endl;
                return 1;
            }
            int value;
            try {
                value = stoi(argv[i++]);
            }
            catch(const invalid_argument &) {
                cout << "Invalid argument, expected number." << endl;
                return 1;
            }
            if(method == "scalered") {
                tracking_image.scalered(value);
            }
            if(method == "scalegreen") {
                tracking_image.scalegreen(value);
            }
            if(method == "scaleblue") {
                tracking_image.scaleblue(value);
            }
        }
        else {
            cout << "Invalid method name." << endl;
            return 1;
        }
    }
    if(!tracking_image.save(output_filename)) {
        cerr << "Failed to save output image: " << output_filename << endl;
        return 1;
    }
    return 0;
}