#include <array>
#include <stdexcept>
#include <string>

#include "Parser.h"
#include "Scene.h"

#include "lights/LightFactory.h"
#include "shapes/ShapeFactory.h"

Scene::Scene() : height_{0}, width_{0}, cameraIsSet_{false}
{}

void Scene::createSceneFromInputFile(const std::string &fileName)
{
  const auto parser = Parser(fileName, *this);
}

void Scene::setWindowSize_(int height, int width)
{
  if ((height < 0) || (width < 0))
    throw std::runtime_error(
      "Invalid height: " + std::to_string(height) + "or width: " + std::to_string(width));

  height_ = height;
  width_ = width;
}

void Scene::setCamera_(const std::array<float, 3> &lookFrom,
                      const std::array<float, 3> &lookAt,
                      const std::array<float, 3> &upVector,
                      float fovYDir)
{
  if (cameraIsSet_)
    throw std::runtime_error("Camera is already set!");

  camera_ = Camera{lookFrom, lookAt, upVector, fovYDir};
  cameraIsSet_ = true;
}

void Scene::addVertex_(const Point3D &point3D)
{
  Vertices_.emplace_back(point3D);
}

size_t Scene::getNumberOfVertices()
{
  return Vertices_.size();
}

void Scene::addTriangle_(const MateriaPropertiesAndAmbient &matProperties,
                        const std::array<int, 3> &cornerNodeIndices)
{
  shapes_.emplace_back(ShapeFactory::createTriangle(matProperties, Vertices_, cornerNodeIndices));
}

void Scene::addDirectionalLight_(const Vector3D &direction, const Color &rgb)
{
  lights_.emplace_back(LightFactory::createDirectionalLight(direction, rgb));
}

void Scene::addPointLight_(const Point3D &point, const Color &rgb)
{
  lights_.emplace_back(LightFactory::createPointLight(point, rgb));
}

void Scene::render() const
{

}