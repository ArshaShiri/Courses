#include <array>
#include <stdexcept>
#include <string>

#include "GLMWrapper.h"
#include "FreeImageWrapper.h"
#include "Parser.h"
#include "Scene.h"

#include "lights/LightFactory.h"
#include "shapes/ShapeFactory.h"

namespace
{
class LightCalculator
{
public:
  LightCalculator(const std::vector<std::unique_ptr<const Light>> &lights,
                  const Ray &incomingRay,
                  ClosestIntersectedShapeFinder &closestIntersectedShapeFinder);

  Color getLightsContributions() const;

private:
  const std::vector<std::unique_ptr<const Light>> &lights_;
  const Ray &incomingRay_;
  ClosestIntersectedShapeFinder &closestIntersectedShapeFinder_;
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
  closestIntersectedShapeFinder_.buildIntersectorMap(shapes_);

  for (auto h = 0; h < height_; ++h)
  {
    for (auto w = 0; w < width_; ++w)
    {
      addTheColorOfPixel_(w, h);
    }
  }
}

void Scene::save() const
{
  saver_.save(width_, height_, colors_);
}

void Scene::addTheColorOfPixel_(int width, int height)
{
  auto color = Color{};

  const auto ray = camera_.calculateRayThroughPixel(width, height);
  closestIntersectedShapeFinder_.intersectRay(ray);

  // We first see if the ray is intersecting any of the shapes.
  if (closestIntersectedShapeFinder_.isAnyOfTheShapesIntersectingWithRay())
  {
    const auto pShape = closestIntersectedShapeFinder_.getClosestShape();
    const auto &closestIntersection = closestIntersectedShapeFinder_.getClosestIntersectionPoint();

    // Base color for all the shapes
    color = pShape->getAmbient() + pShape->getEmission();

    auto lightCalculator = LightCalculator(lights_, ray, closestIntersectedShapeFinder_);
    color += lightCalculator.getLightsContributions();
  }

  colors_.emplace_back(std::move(color));
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
LightCalculator::LightCalculator(const std::vector<std::unique_ptr<const Light>> &lights,
                                 const Ray &incomingRay,
                                 ClosestIntersectedShapeFinder &closestIntersectedShapeFinder) :
  lights_{lights},
  incomingRay_{incomingRay},
  closestIntersectedShapeFinder_{closestIntersectedShapeFinder}
{}

Color LightCalculator::getLightsContributions() const
{
  auto color = Color{};

  const auto pCurrentShape = closestIntersectedShapeFinder_.getClosestShape();
  const auto &currentIntersection = closestIntersectedShapeFinder_.getClosestIntersectionPoint();
  const auto &shapeMatProperties = pCurrentShape->getMateriaPropertiesAndAmbient();

  // All the light contributions:
  for (const auto &pLight : lights_)
  {
    const auto shadowRay = pLight->getRayTowardsLightFromPoint(currentIntersection);
    closestIntersectedShapeFinder_.intersectRay(shadowRay);
    const auto distanceToLight = pLight->getDistanceToPoint(currentIntersection);
    const auto &unitNormalAtIntersectionPoint = closestIntersectedShapeFinder_.getUnitNormalAtIntersectionPoint();

    if (closestIntersectedShapeFinder_.isAnyOfTheShapesIntersectingWithRay())
    {
      const auto &closestIntersection = closestIntersectedShapeFinder_.getClosestIntersectionPoint();
      const auto distanceToIntersectionPoint = closestIntersection.distance(closestIntersection);

      if (distanceToLight < distanceToIntersectionPoint)
        color += pLight->getContributionOnObject(
          shapeMatProperties,
          incomingRay_.getUnitDirection(),
          unitNormalAtIntersectionPoint,
          shadowRay,
          distanceToLight);
    }
    else
      color += pLight->getContributionOnObject(
        shapeMatProperties,
        incomingRay_.getUnitDirection(),
        unitNormalAtIntersectionPoint,
        shadowRay,
        distanceToLight);
  }

  return color;
}
} // End of namespace definition