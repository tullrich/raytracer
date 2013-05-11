#include <iostream>
#include <boost/program_options.hpp>
#include "raytracer.h"

namespace raytracer {

Raytracer::Raytracer() : img(NULL), hx(DEFAULT_IMG_HEIGHT), wx(DEFAULT_IMG_WIDTH), outpath(DEFAULT_OUT_PATH)
{
    initImage();
    img = allocateImage(wx, hx);
}


void Raytracer::trace()
{
    if (saveImage(img, outpath.c_str()))
    {
        std::cout << "Error: FreeImage_Save()" << std::endl;
    }
    std::cout << "Saved" << std::endl;
}

} /* namespace raytracer */

namespace po = boost::program_options;
using namespace raytracer;
using namespace std;

int main(int argc, char *argv[])
{

    po::options_description pd;
    pd.add_options()
        ("help", "produce help message")
        ("w", po::value<int>(), "set image output width")
        ("h", po::value<int>(), "set image output height")
    ;

    po::positional_options_description pod;
    pod.add("input-file", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(pd).positional(pod).run(), vm);
    po::notify(vm); 

    if (vm.count("help")) 
    {
        cout << pd << "\n";
        return 1;
    }

    if (vm.count("compression")) 
    {
        cout << "Compression level was set to " 
        << vm["compression"].as<int>() << ".\n";
    } else {
        cout << "Compression level was not set.\n";
    }

/*    char *filename_o = argv[1];
    Raytracer tracer;
    std::string filename_str(filename_o);


    if(argc > 1)
    {
        tracer.setOutpath(filename_str);
    }
    tracer.trace();*/

	return 0;
}
