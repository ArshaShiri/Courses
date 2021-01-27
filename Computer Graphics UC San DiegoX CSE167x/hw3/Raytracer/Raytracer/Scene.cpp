#include <array>
#include <stdexcept>
#include <string>

#include "lights/LightFactory.h"
#include "Scene.h"
#include "shapes/ShapeFactory.h"

Scene::Scene() : height_{0}, width_{0}, cameraIsSet_{false}
{}

void Scene::setWindowSize(int height, int width)
{
  if ((height < 0) || (width < 0))
    throw std::runtime_error(
      "Invalid height: " + std::to_string(height) + "or width: " + std::to_string(width));

  height_ = height;
  width_ = width;
}

void Scene::setCamera(const std::array<float, 3> &lookFrom,
                      const std::array<float, 3> &lookAt,
                      const std::array<float, 3> &upVector,
                      float fovYDir)
{
  if (cameraIsSet_)
    throw std::runtime_error("Camera is already set!");

  camera_ = Camera{lookFrom, lookAt, upVector, fovYDir};
  cameraIsSet_ = true;
}

void Scene::addVertex(const Point3D &point3D)
{
  Vertices_.emplace_back(point3D);
}

size_t Scene::getNumberOfVertices()
{
  return Vertices_.size();
}

void Scene::addTriangle(const MateriaPropertiesAndAmbient &matProperties,
                        const std::array<int, 3> &cornerNodeIndices)
{
  shapes_.emplace_back(ShapeFactory::createTriangle(matProperties, Vertices_, cornerNodeIndices));
}

void Scene::addDirectionalLight(const Vector3D &direction, const Color &rgb)
{
  lights_.emplace_back(LightFactory::createDirectionalLight(direction, rgb));
}

void Scene::addPointLight(const Point3D &point, const Color &rgb)
{
  lights_.emplace_back(LightFactory::createPointLight(point, rgb));
}