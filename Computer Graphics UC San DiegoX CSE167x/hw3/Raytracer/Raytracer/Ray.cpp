#include "Ray.h"

Ray::Ray(const Point &viewPoint, const Vector3D &direction) : 
  viewPoint_{viewPoint}, unitDirection_{direction.normalize()}
{}