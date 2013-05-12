#include <iostream>
#include <fstream>
#include <vector>
#include <boost/program_options.hpp>
#include "raytracer.h"
#include "scenegraph.h"

namespace raytracer {

Raytracer::Raytracer(const string &outputfile, int width, int height) : outpath(outputfile)
{
    initImage();

    img = allocateImage(width, height);
}


void Raytracer::trace()
{
    if (!saveImage(img, outpath.c_str()))
    {\
        std::cout << "Error: FreeImage_Save()" << std::endl;
    }
    std::cout << "Saved" << std::endl;
}



Raytracer* RaytraceBuilder::buildRaytracer()
{
    return new Raytracer(outputfile, width, height);
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
static bool validateInput(po::variables_map &vm)
{
    return vm.count(OPTION_OUTPATH);
}

/**
 * open open input file CONFIG_FILEPATH and read it into the store using the provided options_description
 * @param options options to read from the config filename
 * @param vm      the value map to update
 */
static void parseConfig(const po::options_description &options, po::variables_map &vm) 
{
    ifstream config_file(CONFIG_FILEPATH);

    if (config_file.is_open())
    {
        po::store(po::parse_config_file(config_file, options), vm);
    }
    else
    {
        cout << "WARNING: No input file named " << CONFIG_FILEPATH << " found" << endl;
    }

    //TODO: feels like a try/finally is needed
}

/**
 * gather inputs from all sources into vm.
 * @param argc cmd line arg count
 * @param argv cmd line args
 * @param vm   map to insert inputs
 * @return valid inputs
 */
static bool getInput(int argc, char *argv[], po::variables_map &vm)
{
    // commandline configurable only
    po::options_description pd_cmd("Options");
    pd_cmd.add_options()
        ("help", "produce help message")
    ;

    // configurable in the config file and cmdline
    po::options_description pd_config("Configuration");
    pd_config.add_options()
        (OPTION_WIDTH, po::value<int>(), "set image output width")
        (OPTION_HEIGHT, po::value<int>(), "set image output height")
    ;

    // same as pd_config, but will not be displayed in the help text
    po::options_description pd_hidden("Hidden options");
    pd_hidden.add_options()
        (OPTION_OUTPATH, po::value< string >(), "image output filename")
    ;   

    po::positional_options_description pod;
    pod.add(OPTION_OUTPATH, 1);

    // read options, config, and hidden from the commandline
    po::options_description cmdline_options;
    cmdline_options.add(pd_cmd).add(pd_config).add(pd_hidden);

    // read config and hidden options only from the config file
    po::options_description config_file_options;
    config_file_options.add(pd_config).add(pd_hidden);

    po::store(po::command_line_parser(argc, argv).options(cmdline_options).positional(pod).run(), vm);
    parseConfig(config_file_options, vm);
    po::notify(vm); 

    // print help if bad inputs are found or its requested
    if (!validateInput(vm) || vm.count("help"))
    {
        po::options_description visible("Allowed options");
        visible.add(pd_cmd).add(pd_config);

        cout << "Usage: raytracer [options] output-filepath" << endl;
        cout << visible << endl;

        return false;
    }

    return true;
}


static inline void getIntOption(int &option, const string &option_name, const po::variables_map &om)
{

    option = om[option_name].as<int>();
}

/**
 * create and run a {@link Raytracer} with inputs vm.
 * @param vm input to the raytracer
 */
static void runRayTracer(const po::variables_map &vm)
{
    string filename_str;
    RaytraceBuilder builder;
    Raytracer *tracer;

    if (vm.count(OPTION_HEIGHT))
    {
        int i = 0;
        getIntOption(i, OPTION_HEIGHT, vm);
        builder.setHeight(i);
    }

    if (vm.count(OPTION_WIDTH))
    {
        int i = 0;
        getIntOption(i, OPTION_WIDTH, vm);
        builder.setWidth(i);
    }

    filename_str = vm[OPTION_OUTPATH].as< string >(); // already validated
    builder.setOutpath(filename_str);

    tracer = builder.buildRaytracer();
    tracer->trace();
}

int main(int argc, char *argv[])
{
    po::variables_map vm;

    if (!getInput(argc, argv, vm))
    {
        // bad input
        return 1;
    }

    runRayTracer(vm);

	return 0;
}
