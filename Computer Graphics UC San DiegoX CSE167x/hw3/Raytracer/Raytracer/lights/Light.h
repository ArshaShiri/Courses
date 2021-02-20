#pragma once

#include <array>

#include "../Color.h"
#include "../Common.h"
#include "../Ray.h"
#include "../Vector3D.h"

struct ShapeProperties
{
  ShapeProperties(const MateriaPropertiesAndAmbient &matProp, 
                  const Vector3D &objectUnitNormal,
                  float distanceToLight) : 
    matProp{matProp},
    objectUnitNormal{objectUnitNormal},
    distanceToLight{distanceToLight}
  {}

  const MateriaPropertiesAndAmbient &matProp;
  const Vector3D &objectUnitNormal;
  float distanceToLight;
};

class Light
{
public:
  Light(const Color &rgb);
  Color getContributionOnObject(const ShapeProperties &shapeProperties,
                                const Vector3D &incomingRayUnitDir,
                                const Vector3D &shadowRayUnitDir) const;

  virtual Ray getRayTowardsLightFromPoint(const Point3D &point) const = 0;
  virtual float getDistanceToPoint(const Point3D &point) const = 0;

protected:
  Point3D slighltyMovePointTowardsDirection_(const Point3D &point, const Vector3D &dir) const;
  virtual float getAttenuationFactor_(float distanceToLight) const = 0;

private:
  // Private methods
  Color getDiffuseFactor_(const ShapeProperties &shapeProperties,
                          const Vector3D &shadowRayUnitDir) const;

  Color getSpecularFactor_(const ShapeProperties &shapeProperties,
                           const Vector3D &incomingRayDirection,
                           const Vector3D &shadowRayUnitDir) const;

  // Private attributes
  Color color_;
};