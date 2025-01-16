#include "image.h"
#include <iostream>

int main() {
    string input_directory = "input/";
    string output_directory = "output/";
    Image layer1, layer2, pattern1, pattern2, car, circles, text, text2, layer_red, layer_green, layer_blue, output;
    if(!layer1.load(input_directory + "layer1.tga")) {
        cerr << "Error loading file: " << input_directory + "layer1.tga" << endl;
        return 1;
    };
    if(!layer2.load(input_directory + "layer2.tga")) {
        cerr << "Error loading file: " << input_directory + "layer2.tga" << endl;
        return 1;
    };
    if(!pattern1.load(input_directory + "pattern1.tga")) {
        cerr << "Error loading file: " << input_directory + "pattern1.tga" << endl;
        return 1;
    };
    if(!pattern2.load(input_directory + "pattern2.tga")) {
        cerr << "Error loading file: " << input_directory + "pattern2.tga" << endl;
        return 1;
    };
    if(!car.load(input_directory + "car.tga")) {
        cerr << "Error loading file: " << input_directory + "car.tga" << endl;
        return 1;
    };
    if(!circles.load(input_directory + "circles.tga")) {
        cerr << "Error loading file: " << input_directory + "circles.tga" << endl;
        return 1;
    };
    if(!text.load(input_directory + "text.tga")) {
        cerr << "Error loading file: " << input_directory + "text.tga" << endl;
        return 1;
    };
    if(!text2.load(input_directory + "text2.tga")) {
        cerr << "Error loading file: " << input_directory + "text2.tga" << endl;
        return 1;
    };
    if(!layer_red.load(input_directory + "layer_red.tga")) {
        cerr << "Error loading file: " << input_directory + "layer_red.tga" << endl;
        return 1;
    };
    if(!layer_green.load(input_directory + "layer_green.tga")) {
        cerr << "Error loading file: " << input_directory + "layer_green.tga" << endl;
        return 1;
    };
    if(!layer_blue.load(input_directory + "layer_blue.tga")) {
        cerr << "Error loading file: " << input_directory + "layer_blue.tga" << endl;
        return 1;
    };

    //Task 1
    layer1.multiply(pattern1);
    if(!layer1.save(output_directory + "part1.tga")) {
        cerr << "Error saving file :" << output_directory + "part1.tga" << endl;
    }

    //Task 2
    car.subtract(layer2);
    if(!car.save(output_directory + "part2.tga")) {
        cerr << "Error saving file :" << output_directory + "part2.tga" << endl;
    }

    //Task 3
    if(!layer1.load(input_directory + "layer1.tga")) {
        cerr << "Error reloading file: " << input_directory + "layer1.tga" << endl;
        return 1;
    }

    layer1.multiply(pattern2);
    layer1.screen(text);
    if(!layer1.save(output_directory + "part3.tga")) {
        cerr << "Error saving file :" << output_directory + "part3.tga" << endl;
    }

    //Task 4
    layer2;
    layer2.multiply(circles);
    layer2.subtract(pattern2);
    if(!layer2.save(output_directory + "part4.tga")) {
        cerr << "Error saving file :" << output_directory + "part4.tga" << endl;
    }

    //Task 5
    if(!layer1.load(input_directory + "layer1.tga")) {
        cerr << "Error reloading file: " << input_directory + "layer1.tga" << endl;
        return 1;
    }
    if(!pattern1.load(input_directory + "pattern1.tga")) {
        cerr << "Error reloading file: " << input_directory + "pattern1.tga" << endl;
        return 1;
    }
    layer1.overlay(pattern1);
    if(!layer1.save(output_directory + "part5.tga")) {
        cerr << "Error saving file :" << output_directory + "part5.tga" << endl;
    }

    //Task 6
    if(car.load(input_directory + "car.tga")) {
        car.add_green();
        if(!car.save(output_directory + "part6.tga")) {
            cerr << "Error saving file :" << output_directory + "part6.tga" << endl;
        }
    }

    //Task 7
    if(car.load(input_directory + "car.tga")) {
        car.scale_red_blue();
        if(!car.save(output_directory + "part7.tga")) {
            cerr << "Error saving file :" << output_directory + "part7.tga" << endl;
        }
    }

    //Task 8
    if(car.load(input_directory + "car.tga")) {
        car.get_channels(output_directory + "part8_r.tga", output_directory + "part8_g.tga", output_directory + "part8_b.tga");
        Image red_part, green_part, blue_part;
        if(red_part.load(output_directory + "part8_r.tga")) {
            red_part.save(output_directory + "part8_r.tga");
        }
        if(green_part.load(output_directory + "part8_g.tga")) {
            green_part.save(output_directory + "part8_g.tga");
        }
        if(blue_part.load(output_directory + "part8_b.tga")) {
            blue_part.save(output_directory + "part8_b.tga");
        }
    }

    //Task 9
    output.mix_channels(output, layer_red, layer_green, layer_blue);
    if(!output.save(output_directory + "part9.tga")) {
        cerr << "Error saving file :" << output_directory + "part9.tga" << endl;
    }

    //Task 10
    text2.rotate_180();
    if(!text2.save(output_directory + "part10.tga")) {
        cerr << "Error saving file :" << output_directory + "part10.tga" << endl;
    }

    return 0;
}