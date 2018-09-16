// Local.
#include "raster2d.h"

void usage_message(){
    std::cout << "Usage: ./raster2d <input_file> <output_file> <x_res> <y_res>" << std::endl;
    std::cout << "{x_res, y_res} resolutions should be given in pixels." << std::endl;
}

int main(int argc, char *argv[]){

    string input_file = "", output_file = "";
    int x_res, y_res;

    // check args
    if (argc != 5){
       usage_message();
       return 0;
    }

    // get input file, output file names
    input_file  = argv[1];
    output_file = argv[2];

    // get x_res, y_res
    x_res = atoi(argv[3]);
    y_res = atoi(argv[4]);

    if(x_res <= 0 || y_res <= 0){
        std::cerr << "ERROR: Unacceptable resolution. Aborting.";
        return 0;
    }

    std::cout<<"Attempting to rasterize " << input_file << " at a resolution of "
            << x_res << " x " << y_res <<" pixels. " << std::endl;

    Raster2d raster_image(x_res, y_res);
    raster_image.load_file(input_file);
    raster_image.draw();
    raster_image.save_out(output_file);
    return 0;
}
