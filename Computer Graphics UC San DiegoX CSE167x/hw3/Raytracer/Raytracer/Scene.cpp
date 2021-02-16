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
  void updateClosestShapeIfNecessary_(const Shape *pShape);

  bool isAnyOfTheShapesIntersectingWithRay_;
  std::unique_ptr<RayShapeIntersector> pClosestShapeIntersector_;
  const Shape *pClosestShape_;
  float closetIntersectionPointRayParameter_ = std::numeric_limits <float>::max();
};
} // End of namespace declaration

Scene::Scene() : height_{0}, width_{0}, cameraIsSet_{false}
{}

void Scene::createSceneFromInputFile(const std::string &fileName)
{
  const auto parser = Parser(fileName, *this);
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
  shapes_.emplace_back(ShapeFactory::createTriangle(matProperties, vertices_, cornerNodeIndices));
}

void Scene::addSphere_(const MateriaPropertiesAndAmbient &matProperties,
                       const Point3D center,
                       float radius)
{
  shapes_.emplace_back(ShapeFactory::createSphere(matProperties, center, radius));
}

void Scene::addDirectionalLight_(const Vector3D &direction, const Color &rgb)
{
  lights_.emplace_back(LightFactory::createDirectionalLight(direction, rgb));
}

void Scene::addPointLight_(const Point3D &point, const Color &rgb)
{
  lights_.emplace_back(LightFactory::createPointLight(point, rgb));
}

void Scene::renderAndSave()
{
  for (auto h = 0; h < height_; ++h)
  {
    for (auto w = 0; w < width_; ++w)
      colors_.emplace_back(getColorOfPixel_(w, h));
  }

  FreeImage::FreeImageWrapper::saveImage(width_, height_, colors_);
}

Color Scene::getColorOfPixel_(int pixelWidth, int pixelHeight) const
{
  auto color = Color{};
  const auto ray = camera_.calculateRayThroughPixel(pixelWidth, pixelHeight);
  const auto closestIntersectedShapeFinder = ClosestIntersectedShapeFinder(shapes_, ray);

  if (closestIntersectedShapeFinder.isAnyOfTheShapesIntersectingWithRay())
  {
    const auto pShape = closestIntersectedShapeFinder.getClosestShape();
    color = pShape->getAmbient();
  }

  return color;
}

void Scene::addScale_(float sx, float sy, float sz)
{
  const auto &top = transformationStack_.top();
  const auto scaledTop = GLMWrapper::GLMWrapper::getScaled(top, sx, sy, sz);
  transformationStack_.push(scaledTop);
}

void Scene::addTranslation_(float tx, float ty, float tz)
{
  const auto &top = transformationStack_.top();
  const auto translatedTop = GLMWrapper::GLMWrapper::getTranslated(top, tx, ty, tz);
  transformationStack_.push(translatedTop);
}

void Scene::addRotation_(const Vector3D &axis, float degrees)
{
  const auto &top = transformationStack_.top();
  const auto rotatedTop = GLMWrapper::GLMWrapper::getRotated(top, axis, degrees);
  transformationStack_.push(rotatedTop);
}

void Scene::popTransformation_()
{
  transformationStack_.pop();
}

void Scene::pushTransformation_()
{
  transformationStack_.push(transformationStack_.top());
}

namespace
{
ClosestIntersectedShapeFinder::ClosestIntersectedShapeFinder(
  const std::vector<std::unique_ptr<const Shape>> &shapes,
  const Ray &ray) : isAnyOfTheShapesIntersectingWithRay_{false}
{
  for (const auto &pShape : shapes)
  {
    pClosestShapeIntersector_ = RayShapeIntersectorFactory::createIntersector(*pShape);
    pClosestShapeIntersector_->calculateIntersectionPointWithRay(ray);

    if (pClosestShapeIntersector_->doesIntersectionPointExist())
      updateClosestShapeIfNecessary_(pShape.get());
  }
}

void ClosestIntersectedShapeFinder::updateClosestShapeIfNecessary_(const Shape *pShape)
{
  const auto intersectionPointRayParameter = pClosestShapeIntersector_->getRayParameter();

  if (intersectionPointRayParameter < closetIntersectionPointRayParameter_)
  {
    closetIntersectionPointRayParameter_ = intersectionPointRayParameter;
    pClosestShape_ = pShape;
    isAnyOfTheShapesIntersectingWithRay_ = true;
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
} // End of namespace definition