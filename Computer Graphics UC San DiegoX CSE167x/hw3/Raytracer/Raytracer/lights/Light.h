#pragma once

#include <array>

#include "../Color.h"
#include "../Common.h"
#include "../Ray.h"
#include "../Vector3D.h"

class Light
{
public:
  Light(const Color &rgb);
  Color getContributionOnObject(
    const MateriaPropertiesAndAmbient &matProp,
    const Vector3D &incomingRayDir,
    const Vector3D &objectUnitNormal,
    const Ray &shadowRay,
    float distanceToLight) const;

  virtual Ray getRayTowardsLightFromPoint(const Point3D &point) const = 0;
  virtual float getDistanceToPoint(const Point3D &point) const = 0;

protected:
  Point3D slighltyMovePointTowardsDirection_(const Point3D &point, const Vector3D &dir) const;
  virtual float getAttenuationFactor_(float distanceToLight) const = 0;

private:
  // Private methods
  Color getDiffuseFactor_(const Color &diffuse,
                          const Vector3D &objectUnitNormal,
                          const Vector3D &lightDirection) const;

  Color getSpecularFactor_(const Color &specular,
                           float shininess,
                           const Vector3D &objectUnitNormal,
                           const Vector3D &incomingRayDirection,
                           const Vector3D &lightDirection) const;

  // Private attributes
  Color color_;
};