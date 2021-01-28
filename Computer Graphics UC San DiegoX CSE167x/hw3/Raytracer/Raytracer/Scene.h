/*Defines the scene*/

#pragma once

#include <memory>
#include <vector>

#include "Camera.h"
#include "Common.h"

#include "lights/Light.h"
#include "shapes/Shape.h"

class Scene
{
public:
  Scene();
  void createSceneFromInputFile(const std::string &fileName);

private:
  friend class Parser;

  void setWindowSize_(int height, int width);
  void setCamera_(const std::array<float, 3> &lookFrom,
                  const std::array<float, 3> &lookAt,
                  const std::array<float, 3> &upVector,
                  float fovYDir);
  void addVertex_(const Point3D &point3D);
  void addTriangle_(const MateriaPropertiesAndAmbient &matProperties,
                    const std::array<int, 3> &cornerNodeIndices);
  void addDirectionalLight_(const Vector3D &direction, const Color &rgb);
  void addPointLight_(const Point3D &point, const Color &rgb);

  size_t getNumberOfVertices();

  // Private attributes
  int height_;
  int width_;

  Camera camera_;
  bool cameraIsSet_;

  std::vector<std::unique_ptr<Shape>> shapes_;
  std::vector<std::unique_ptr<Light>> lights_;
  std::vector<Point3D> Vertices_;
};