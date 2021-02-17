/*Defines the scene*/

#pragma once

#include <memory>
#include <vector>

#include "Camera.h"
#include "Color.h"
#include "Common.h"
#include "Ray.h"
#include "TransformationStack.h"

#include "lights/Light.h"
#include "shapes/RayShapeIntersector.h"
#include "shapes/Shape.h"

class Scene
{
public:
  Scene();
  void createSceneFromInputFile(const std::string &fileName);
  void render();
  void save() const;

private:
  class SceneSaver
  {
  public:
    SceneSaver() = default;
    SceneSaver(const std::string &inputFileName);

    void setOutputName(const std::string &outputName);
    void save(int width, int height, const std::vector<Color> &colors) const;

  private:
    std::string outputName_;
  };

  friend class Parser;

  void setWindowSize_(int width, int height);
  void setCamera_(const std::array<float, 3> &lookFrom,
                  const std::array<float, 3> &lookAt,
                  const std::array<float, 3> &upVector,
                  float fovYDir);
  void addVertex_(const Point3D &point3D);
  void addTriangle_(const MateriaPropertiesAndAmbient &matProperties,
                    const std::array<int, 3> &cornerNodeIndices);
  void addSphere_(const MateriaPropertiesAndAmbient &matProperties,
                  const Point3D center,
                  float radius);
  void addDirectionalLight_(const Vector3D &direction, const Color &rgb);
  void addPointLight_(const Point3D &point, const Color &rgb);
  
  void addScale_(float sx, float sy, float sz);
  void addTranslation_(float tx, float ty, float tz);
  void addRotation_(const Vector3D &axis, float degrees);
  void popTransformation_();
  void pushTransformation_();

  size_t getNumberOfVertices_() const;
  Color getColorOfPixel_(int pixelWidth, int pixelHeight) const;

  // Private attributes
  int height_;
  int width_;

  Camera camera_;
  bool cameraIsSet_;

  std::vector<std::unique_ptr<const Shape>> shapes_;
  std::vector<std::unique_ptr<const Light>> lights_;
  TransformationStack transformationStack_;
  std::vector<Point3D> vertices_;
  std::vector<Color> colors_;

  SceneSaver saver_;
};