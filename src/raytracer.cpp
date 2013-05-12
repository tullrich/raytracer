#include <iostream>
#include <fstream>
#include <vector>
#include <boost/program_options.hpp>

#include "raytracer.h"
#include "scenegraph.h"
#include "cgutils/cgutils.hpp"
    
using namespace cgutils;

namespace raytracer {

Raytracer::Raytracer(const string &outputfile, int width, int height) : outpath(outputfile)
{
    scene = NULL;

    initImage();
    img   = allocateImage(width, height);
}


void Raytracer::trace()
{
    CGUTILS_ASSERT(scene);

    if (!saveImage(img, outpath.c_str()))
    {
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
    return vm.count(OPTION_ASSETPATH);
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
        (OPTION_OUTPATH, po::value< string >(), "image output filename")
        (OPTION_WIDTH, po::value<int>(), "set image output width")
        (OPTION_HEIGHT, po::value<int>(), "set image output height")
    ;

    // same as pd_config, but will not be displayed in the help text
    po::options_description pd_hidden("Hidden options");
    pd_hidden.add_options()
        (OPTION_ASSETPATH, po::value< vector<string> >(), "input asset file filename")
    ;   

    po::positional_options_description pod;
    pod.add(OPTION_ASSETPATH, -1);

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

        cout << "Usage: raytracer [options] asset-filepath..." << endl;
        cout << visible << endl;

        return false;
    }

    return true;
}

/**
 * set var option from om[option_name] as an int
 * @param option      var to set
 * @param option_name string name of the option
 * @param om          map of options
 */
static inline void getIntOption(int &option, const string &option_name, const po::variables_map &om)
{

    option = om[option_name].as<int>();
}

/**
 * create a {@link Raytracer} from input map vm.
 * @param vm input to the raytracer
 * @return fully configured {@link Raytracer}
 */
static Raytracer* buildRayTracer(const po::variables_map &vm)
{
    int i;
    string filename_str;
    RaytraceBuilder builder;
    Raytracer *tracer;

    // option height
    if (vm.count(OPTION_HEIGHT))
    {
        i = 0;
        getIntOption(i, OPTION_HEIGHT, vm);
        builder.setHeight(i);
    }

    // option width
    if (vm.count(OPTION_WIDTH))
    {
        i = 0;
        getIntOption(i, OPTION_WIDTH, vm);
        builder.setWidth(i);
    }

    // option outpath
    if (vm.count(OPTION_OUTPATH))
    {
        filename_str = vm[OPTION_OUTPATH].as< string >();
        builder.setOutpath(filename_str);
    }

    return builder.buildRaytracer();
}

/**
 * create a {@link Scenegraph} from input map vm
 * @param vm [description]
 * @return fully configured {@link SceneGraph}
 */
static SceneGraph* buildScene(const po::variables_map &vm)
{
    CGUTILS_ASSERT(vm.count(OPTION_ASSETPATH));

    SceneGraph *scene;
    vector<string> assetpaths;

    scene = SceneGraphFactory::getSceneGraph();
    assetpaths = vm[OPTION_ASSETPATH].as< vector<string> >();


    for (string filename : assetpaths)
    {
        cout << "\t Processing assetfile: " << filename << endl;
        
    }

    return scene;
}

int main(int argc, char *argv[])
{
    Raytracer *tracer;
    po::variables_map vm;

    if (!getInput(argc, argv, vm))
    {
        // bad input
        return 1;
    }

    tracer = buildRayTracer(vm);
    tracer->setScene(buildScene(vm));

    tracer->trace();

	return 0;
}
