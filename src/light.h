#ifndef _LIGHT_H_
#define _LIGHT_H_ 

#include "common.h"

namespace raytracer {

class Light
{
public:
	typedef std::shared_ptr<Light> light_ptr;
};


class PointLight : public Light
{

};

class DirectionalLight : public Light
{

};

class SpotLight : public Light
{

};


/**
 * Helper class for visiting Lights
 */
typedef cgutils::Visitor<Light::light_ptr> LightVisitor;

} /* raytracer */

#endif /* _LIGHT_H_ */
