#ifndef _TRIG_H_
#define _TRIG_H_ 

#include "common.h"

namespace raytracer {

glm::vec3 uniformDirectionOnHemisphere(const glm::vec3 normal, float &pdf_theta);

} /* raytracer */

#endif /* _TRIG_H_ */
