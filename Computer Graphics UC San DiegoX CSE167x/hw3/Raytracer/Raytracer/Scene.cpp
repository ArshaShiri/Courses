#include <array>
#include <stdexcept>
#include <string>

#include "GLMWrapper.h"
#include "FreeImageWrapper.h"
#include "Parser.h"
#include "Scene.h"

#include "lights/LightFactory.h"
#include "shapes/ShapeFactory.h"
#include "shapes/RayShapeIntersectorFactory.h"

namespace
{
class ClosestIntersectedShapeFinder
{
public:
  ClosestIntersectedShapeFinder(
    const std::vector<std::unique_ptr<const Shape>> &shapes, const Ray &ray);

  bool isAnyOfTheShapesIntersectingWithRay() const;
  const Shape *getClosestShape() const;
  const RayShapeIntersector *getClosestShapeIntersector() const;

private:
  bool isAnyOfTheShapesIntersectingWithRay_;
  std::unique_ptr<RayShapeIntersector> pClosestShapeIntersector_;
  const Shape *pClosestShape_;
  float closestIntersectionPointDistance_ = std::numeric_limits <float>::max();
};

class LightCalculator
{
public:
  LightCalculator(const Light *pLight, 
                  const RayShapeIntersector *pRayShapeIntersector);

  const Ray &getShadowRay() const;
private:
  const Light *pLight_;
  Ray shadowRay_;
};
} // End of namespace declaration

Scene::Scene() : height_{0}, width_{0}, cameraIsSet_{false}
{}

void Scene::createSceneFromInputFile(const std::string &fileName)
{
  saver_ = Scene::SceneSaver{fileName};
  const auto parser = Parser(fileName, *this);
}

void Scene::render()
{
  // buildIntersectorMap_();
  for (auto h = 0; h < height_; ++h)
  {
    for (auto w = 0; w < width_; ++w)
      colors_.emplace_back(getColorOfPixel_(w, h));
  }
}

void Scene::save() const
{
  saver_.save(width_, height_, colors_);
}

void Scene::buildIntersectorMap_()
{
  for (const auto &pShape : shapes_)
  {
    intersectorMap_.try_emplace(pShape.get(), RayShapeIntersectorFactory::createIntersector(*pShape));
  }
}

Color Scene::getColorOfPixel_(int pixelWidth, int pixelHeight) const
{
  auto color = Color{};
  const auto ray = camera_.calculateRayThroughPixel(pixelWidth, pixelHeight);
  const auto closestIntersectedShapeFinder = ClosestIntersectedShapeFinder(shapes_, ray);

  if (closestIntersectedShapeFinder.isAnyOfTheShapesIntersectingWithRay())
  {
    const auto pShape = closestIntersectedShapeFinder.getClosestShape();
    const auto pClosestShapeIntersector = closestIntersectedShapeFinder.getClosestShapeIntersector();
    color = pShape->getAmbient() + pShape->getEmission();

    // All the light contributions:
    for (const auto &pLight : lights_)
    {
      const auto lightCalculator = LightCalculator(pLight.get(), pClosestShapeIntersector);
      const auto &shadowRay = lightCalculator.getShadowRay();
      const auto closestIntersectedShapeFinderForShadow = ClosestIntersectedShapeFinder(shapes_, shadowRay);

      const auto &matProperties = pShape->getMateriaPropertiesAndAmbient();
      const auto &intersectionPoint = pClosestShapeIntersector->getIntersectionPoint();
      const auto distanceToLight = pLight->getDistanceToPoint(intersectionPoint);

      if (closestIntersectedShapeFinderForShadow.isAnyOfTheShapesIntersectingWithRay())
      {
        const auto pClosestShapeToShadowRay = closestIntersectedShapeFinderForShadow.getClosestShapeIntersector();
        const auto &closestIntersection = pClosestShapeToShadowRay->getIntersectionPoint();
        const auto distanceToIntersectionPoint = intersectionPoint.distance(closestIntersection);

        if (distanceToLight < distanceToIntersectionPoint)
          color += pLight->getContributionOnObject(
            matProperties,
            ray.getUnitDirection(),
            pClosestShapeIntersector->getUnitNormalOfShapeAtIntersectionPoint(),
            shadowRay,
            distanceToLight);
      }
      else
        color += pLight->getContributionOnObject(
          matProperties,
          ray.getUnitDirection(),
          pClosestShapeIntersector->getUnitNormalOfShapeAtIntersectionPoint(),
          shadowRay,
          distanceToLight);
    }
  }

  return color;
}

void Scene::setWindowSize_(int width, int height)
{
  if ((width < 0) || (height < 0))
    throw std::runtime_error(
      "Invalid height: " + std::to_string(height) + "or width: " + std::to_string(width));

  width_ = width;
  height_ = height;
  camera_.setHeightAndWidthOfImagePlane(width, height);
}

void Scene::setCamera_(const std::array<float, 3> &lookFrom,
                       const std::array<float, 3> &lookAt,
                       const std::array<float, 3> &upVector,
                       float fovYDir)
{
  if (cameraIsSet_)
    throw std::runtime_error("Camera is already set!");

  camera_.setCamerProperties(lookFrom, lookAt, upVector, fovYDir);
  cameraIsSet_ = true;
}

void Scene::addVertex_(const Point3D &point3D)
{
  vertices_.emplace_back(point3D);
}

size_t Scene::getNumberOfVertices_() const
{
  return vertices_.size();
}

void Scene::addTriangle_(const MateriaPropertiesAndAmbient &matProperties,
                         const std::array<int, 3> &cornerNodeIndices)
{
  auto getTriangleVertices = 
    [this](const std::array<int, 3> &cornerNodeIndices)
    {     
      auto p1 = vertices_.at(cornerNodeIndices.at(0));
      auto p2 = vertices_.at(cornerNodeIndices.at(1));
      auto p3 = vertices_.at(cornerNodeIndices.at(2));

      return std::array<Point3D, 3>{p1, p2, p3};
    };


  shapes_.emplace_back(ShapeFactory::createTriangle(
    matProperties, transformationStack_.top(), getTriangleVertices(cornerNodeIndices)));
}

void Scene::addSphere_(const MateriaPropertiesAndAmbient &matProperties,
                       const Point3D center,
                       float radius)
{
  auto pSphere = ShapeFactory::createSphere(
    matProperties, transformationStack_.top(), center, radius);
  shapes_.emplace_back(std::move(pSphere));
}

void Scene::addDirectionalLight_(const Vector3D &direction, const Color &rgb)
{
  lights_.emplace_back(LightFactory::createDirectionalLight(direction, rgb));
}

void Scene::addPointLight_(const Point3D &point, const Color &rgb, const Attenuation &attenuation)
{
  lights_.emplace_back(LightFactory::createPointLight(point, rgb, attenuation));
}

void Scene::addScale_(float sx, float sy, float sz)
{
  auto &top = transformationStack_.top();
  top = GLMWrapper::GLM::getScaled(top, sx, sy, sz);
}

void Scene::addTranslation_(float tx, float ty, float tz)
{
  auto &top = transformationStack_.top();
  top = GLMWrapper::GLM::getTranslated(top, tx, ty, tz);
}

void Scene::addRotation_(const Vector3D &axis, float degrees)
{
  auto &top = transformationStack_.top();
  top = GLMWrapper::GLM::getRotated(top, axis, degrees);
}

void Scene::popTransformation_()
{
  transformationStack_.pop();
}

void Scene::pushTransformation_()
{
  transformationStack_.push(transformationStack_.top());
}

void Scene::setOutputName(const std::string &outputName)
{
  saver_.setOutputName(outputName);
}

Scene::SceneSaver::SceneSaver(const std::string &inputFileName)
{
  setOutputName(inputFileName);
}

void Scene::SceneSaver::setOutputName(const std::string &outputName)
{
  const auto extensionLocation = outputName.find('.');

  if (extensionLocation == outputName.npos)
    outputName_ = outputName;
  else
    outputName_ = outputName.substr(0, extensionLocation);
}

void Scene::SceneSaver::save(int width, int height, const std::vector<Color> &colors) const
{
  FreeImage::FreeImageWrapper::saveImage(outputName_, width, height, colors);
}

namespace
{
ClosestIntersectedShapeFinder::ClosestIntersectedShapeFinder(
  const std::vector<std::unique_ptr<const Shape>> &shapes,
  const Ray &ray) : isAnyOfTheShapesIntersectingWithRay_{false}
{
  for (const auto &pShape : shapes)
  {
    auto pShapeIntersector = RayShapeIntersectorFactory::createIntersector(*pShape);
    pShapeIntersector->calculateIntersectionPointWithRay(ray);

    if (pShapeIntersector->doesIntersectionPointExist())
    {
      const auto distance = pShapeIntersector->getIntersectionPointDistanceToOrigin();
      if (distance < closestIntersectionPointDistance_)
      {
        closestIntersectionPointDistance_ = distance;
        pClosestShape_ = pShape.get();
        pClosestShapeIntersector_ = std::move(pShapeIntersector);
        isAnyOfTheShapesIntersectingWithRay_ = true;
      }
    }
  }
}

bool ClosestIntersectedShapeFinder::isAnyOfTheShapesIntersectingWithRay() const
{
  return isAnyOfTheShapesIntersectingWithRay_;
}

const Shape *ClosestIntersectedShapeFinder::getClosestShape() const
{
  if (!isAnyOfTheShapesIntersectingWithRay())
    throw std::runtime_error("Intersection does not exist!");

  return pClosestShape_;
}

const RayShapeIntersector *ClosestIntersectedShapeFinder::getClosestShapeIntersector() const
{
  if (!isAnyOfTheShapesIntersectingWithRay())
    throw std::runtime_error("Intersection does not exist!");

  return pClosestShapeIntersector_.get();
}

/* LightCalculator */

LightCalculator::LightCalculator(const Light *pLight,
                                 const RayShapeIntersector *pRayShapeIntersector) :
  pLight_{pLight}
{
  const auto &intersectionPoint = pRayShapeIntersector->getIntersectionPoint();
  shadowRay_ = pLight_->getRayTowardsLightFromPoint(intersectionPoint);
}

const Ray &LightCalculator::getShadowRay() const
{
  return shadowRay_;
}
} // End of namespace definition