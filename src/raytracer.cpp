#include <iostream>
#include <vector>
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


/**
 * ensure valid inputs after parsing
 * @param vm filled input map
 * @return everything validated
 */
bool validateInput(po::variables_map &vm)
{
    if (!vm.count("output-file")) 
    {
        cout << "ERROR: no output-file specified" << endl;
        return false;
    }

    return true;
}

/**
 * gather inputs from all sources into vm.
 * @param argc cmd line arg count
 * @param argv cmd line args
 * @param vm   map to insert inputs
 * @return valid inputs
 */
bool parseInput(int argc, char *argv[], po::variables_map &vm)
{
    po::options_description pd("Options");
    pd.add_options()
        ("help", "produce help message")
        ("w", po::value<int>(), "set image output width")
        ("h", po::value<int>(), "set image output height")
        ("output-file", po::value< string >(), "image output filename")
    ;

    po::positional_options_description pod;
    pod.add("output-file", 1);

    po::store(po::command_line_parser(argc, argv).options(pd).positional(pod).run(), vm);
    po::notify(vm); 

    if (!validateInput(vm) || vm.count("help"))
    {
        cout << pd << endl;
        return false;
    }

    return true;
}

/**
 * create and run a {@link Raytracer} with inputs vm.
 * @param vm input to the raytracer
 */
void runRayTracer(po::variables_map &vm)
{
    Raytracer tracer;
    string filename_str;

    filename_str = vm["output-file"].as< string >();
    cout << filename_str << endl;

    tracer.setOutpath(filename_str);
    tracer.trace();
}

int main(int argc, char *argv[])
{
    po::variables_map vm;

    if (!parseInput(argc, argv, vm))
    {
        // bad input
        return 1;
    }

    runRayTracer(vm);

	return 0;
}
