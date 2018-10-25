#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <boost/program_options.hpp>

#include "raytracer.h"
#include "scenegraph.h"
#include "resources.h"
#include "camera.h"

namespace po = boost::program_options;
using namespace raytracer;

/**
 * ensure valid inputs after parsing
 * @param vm filled input map
 * @return everything validated
 */
static bool validateInput(po::variables_map &vm)
{
    return vm.count(OPTION_ASSETPATH) > 0;
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
        ("help", "produce help message");

    // configurable in the config file and cmdline
    po::options_description pd_config("Configuration");
    pd_config.add_options()
        (OPTION_OUTPATH, po::value< std::string >(), "image output filename")
        (OPTION_WIDTH, po::value<int>(), "set image output width")
        (OPTION_HEIGHT, po::value<int>(), "set image output height")
		(OPTION_THREADS, po::value<int>(), "number of threads.");

    // same as pd_config, but will not be displayed in the help text
    po::options_description pd_hidden("Hidden options");
    pd_hidden.add_options()
        (OPTION_ASSETPATH, po::value<std::vector<std::string>>(), "input asset file filename");

    po::positional_options_description pod;
    pod.add(OPTION_ASSETPATH, -1);

    // read options, config, and hidden from the commandline
    po::options_description cmdline_options;
    cmdline_options.add(pd_cmd).add(pd_config).add(pd_hidden);

    // read config and hidden options only from the config file
    po::options_description config_file_options;
    config_file_options.add(pd_config).add(pd_hidden);

	// actual parse and store
	bool ret = false;
	try
	{
		po::store(po::command_line_parser(argc, argv).options(cmdline_options).positional(pod).run(), vm);
		po::store(po::parse_config_file<char>(CONFIG_FILEPATH, config_file_options), vm);
		po::notify(vm);
		ret = validateInput( vm );
	}
	catch (boost::program_options::invalid_option_value& v)
	{
		std::cout << "error:" << v.what() << std::endl;
	}

    // print help if bad inputs are found or its requested
    if (!ret || vm.count( "help" ) )
    {
        po::options_description visible;
        visible.add(pd_cmd).add(pd_config);

        std::cout << "usage: raytracer [options] <filepath>" << std::endl
			<< visible << std::endl;
    }

    return ret;
}


/**
 * create a {@link Raytracer} from input map vm.
 * @param vm input to the raytracer
 * @return fully configured {@link Raytracer}
 */
static Raytracer* buildRayTracer(const po::variables_map &vm)
{
    // option height
	int height = DEFAULT_IMG_HEIGHT;
    if (vm.count(OPTION_HEIGHT))
    {
		height = vm[OPTION_HEIGHT].as<int>();
    }

    // option width
	int width = DEFAULT_IMG_WIDTH;
    if (vm.count(OPTION_WIDTH))
    {
		width = vm[OPTION_WIDTH].as<int>();
    }

    // option outpath
	std::string filename = DEFAULT_OUT_PATH;
    if (vm.count(OPTION_OUTPATH))
    {
		filename = vm[OPTION_OUTPATH].as<std::string>();
    }

	// option threads
	int threadNum = DEFAULT_THREAD_COUNT;
	if ( vm.count(OPTION_THREADS) )
	{
		threadNum = vm[OPTION_THREADS].as<int>();
	}

	return new Raytracer(filename, width, height, threadNum);
}

/**
 * Add everything in path to the scene as entities, performing all loading
 * @param sb   sceneinjector for the scene we are adding too
 * @param path filename to load
 */
static void addToScene(const SceneGraphInjector &sb, std::string &path)
{
	std::unique_ptr<AssetReader> reader = ResourceLoaderFactory::getReaderForFiletype(path);

    if(reader)
    {
        if(reader->open(path))
        {
            // add everything into the Scenegraph
            reader->accept((EntityVisitor&)sb);
            reader->accept((LightVisitor&)sb);
        }
        else
        {
			std::cout << "Warning: problem opening asset file '" << path << "'" << std::endl;
        }
    }
    else
    {
		std::cout << "Warning: unsupported file type '" << path << "'" << std::endl;
    }
}

/**
 * create a {@link Scenegraph} from input map vm
 * @param vm [description]
 * @return fully configured {@link SceneGraph}
 */
static SceneGraph* buildScene(const po::variables_map &vm)
{
    SceneGraph *scene;
	std::vector<std::string> assetpaths;

    RAYTRACER_ASSERT(vm.count(OPTION_ASSETPATH)); // bomb if we somehow got here without assets

    assetpaths = vm[OPTION_ASSETPATH].as< std::vector<std::string> >();
    scene = SceneGraphFactory::getSceneGraph();
    SceneGraphInjector injector(scene);

    for (std::string filename : assetpaths)
    {
		std::cout << "Processing assetfile " << filename << "... " << std::flush;
        addToScene(injector, filename);
		std::cout << "done" << std::endl;
    }

	std::cout << "Building scene... " << std::flush;
    scene->build();
	std::cout << "done" << std::endl;

    return scene;
}

static Camera* buildCamera(const po::variables_map &vm)
{
    // option height
    int x_res = DEFAULT_IMG_HEIGHT;
    if (vm.count(OPTION_WIDTH))
    {
		x_res = vm[OPTION_WIDTH].as<int>();
    }

    // option width
    int y_res = DEFAULT_IMG_WIDTH;
    if (vm.count(OPTION_HEIGHT))
    {
		y_res = vm[OPTION_HEIGHT].as<int>();
    }
    return new Camera(x_res, y_res);
}

int main(int argc, char *argv[])
{
    po::variables_map vm;
    if (!getInput(argc, argv, vm))
    {
        // bad input
        return 1;
    }

	Raytracer* tracer = buildRayTracer(vm);

	SceneGraph* scene = buildScene(vm);
	tracer->setScene(scene);

	Camera* camera = buildCamera(vm);
    tracer->setCamera(camera);

	Light* light = new PointLight("custom");
    light->setLocation(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0));
    light->setColor(RGB(0), RGB(1.0f, 1.0f, 1.0f), RGB(0));
    light->setAttenuation( 1.0f, 0.01f, 0.0003f );
    scene->addLight(Light::light_ptr(light));

	std::cout << "tracing scene... " << std::flush;
    tracer->run();
	std::cout << "done" << std::endl;

	std::cout << "stats:" << std::endl;
	std::cout << tracer->getStats() << std::endl;

	delete camera;
    delete tracer;
    delete scene;

	return 0;
}
