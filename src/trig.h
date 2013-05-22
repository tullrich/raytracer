#ifndef _TRIG_H_
#define _TRIG_H_ 

#include "common.h"

namespace raytracer {

/**
 * generate a uniformally random direction vector on a hemisphere around
 * the input normal
 * @param  normal normal vector around which the direction vector will be
 * @return        random direction vector
 */
glm::vec3 uniformDirectionOnHemisphere(const glm::vec3 normal);

} /* raytracer */

#endif /* _TRIG_H_ */
