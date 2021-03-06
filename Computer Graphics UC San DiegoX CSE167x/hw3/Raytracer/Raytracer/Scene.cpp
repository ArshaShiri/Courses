#include <array>
#include <iostream>
#include <stdexcept>
#include <string>

#include "GLMWrapper.h"
#include "FreeImageWrapper.h"
#include "Parser.h"
#include "Scene.h"

#include "lights/LightCalculator.h"
#include "lights/LightFactory.h"
#include "shapes/ShapeFactory.h"

namespace
{
class ProgressPrinter
{
public:
  ProgressPrinter(int width, int height);
  void updateCurrentPixelNumber(int width, int height, int totalWidth);
  void print();

private:
  const int totalNumberOfPixels_;
  int previousProgress_;
  int currentPixelNumber_;
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
      calculateAndStoreTheColorOfPixel_(w, h);
      printProgress(w, h);
    }
  }
}

void Scene::save() const
{
  saver_.save(width_, height_, colors_);
}

void Scene::calculateAndStoreTheColorOfPixel_(int width, int height)
{
  const auto ray = camera_.calculateRayThroughPixel(width, height);
  colors_.emplace_back(retrieveColorForRay_(ray, maxDepth_));
}

Color Scene::retrieveColorForRay_(const Ray &ray, int maxDepth)
{
  if (maxDepth == 0)
    return{};

  auto color = Color{};

  const auto intersectionInfo = closestIntersectedShapeFinder_.
    getIntersectionInfoAtCloesestIntersectionPoint(ray);

  // We first see if the ray is intersecting any of the shapes.
  if (intersectionInfo.doesIntersectionPointExist())
  {
    const auto pShape = intersectionInfo.getUnderlyingShape();
    const auto &closestIntersection = intersectionInfo.getIntersectionPoint();

    // Base color for all the shapes
    color = pShape->getAmbient() + pShape->getEmission();

    auto lightCalculator = LightCalculator(lights_, closestIntersectedShapeFinder_);
    color += lightCalculator.getLightsContribution(ray.getUnitDirection(), intersectionInfo);


    const auto &specular = pShape->getSpecular();
    const auto specularIsNonZero = specular.getRed() > 0.0f ||
                                   specular.getGreen() > 0.0f ||
                                   specular.getBlue() > 0.0f;
    if (specularIsNonZero)
    {
      const auto &comingRayOrigin = ray.getViewPoint();
      const auto &comingRayDir = ray.getUnitDirection();
      const auto &normalAtIntersectionPoint = intersectionInfo.getUnitNormalOfShapeAtIntersectionPoint();
      const auto reflectedRayDir = comingRayDir + normalAtIntersectionPoint * (-comingRayDir).dot(normalAtIntersectionPoint) * 2.0f;
      const auto reflectedRayDirNormalized = reflectedRayDir.normalize();
      const auto reflectedRayOrigin = closestIntersection + reflectedRayDirNormalized * 0.001f;

      const auto reflectedRay = Ray(reflectedRayOrigin, reflectedRayDir);
      color += specular * retrieveColorForRay_(reflectedRay, maxDepth - 1);
    }
  }
  
  return color;
}

void Scene::printProgress(int width, int height) const
{
  static auto printer = ProgressPrinter(width_, height_);
  printer.updateCurrentPixelNumber(width, height, width_);
  printer.print();
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

void Scene::setMaxDepth_(int depth)
{
  maxDepth_ = depth;
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
ProgressPrinter::ProgressPrinter(int width, int height) : 
  totalNumberOfPixels_{width * height},
  currentPixelNumber_{0},
  previousProgress_{0}
{}

void ProgressPrinter::updateCurrentPixelNumber(int width, int height, int totalWidth)
{
  currentPixelNumber_ = (height * totalWidth) + width;
}

void ProgressPrinter::print()
{
  const auto progress = (int)((float)currentPixelNumber_ / (float)totalNumberOfPixels_ * 100.0f);

  if ((progress - previousProgress_) > 0)
    std::cout << std::to_string(progress) + "%\n";

  previousProgress_ = progress;
}
} // End of namespace definition