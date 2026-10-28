#ifndef NEXTEVENTINTEGRATOR_H
#define NEXTEVENTINTEGRATOR_H

#include "shader.h"

class NextEventIntegrator : public Shader
{
public:
    NextEventIntegrator();
    NextEventIntegrator(Vector3D hitColor, Vector3D bgColor_);

    Vector3D directRadiance(const Ray& r,
        const Intersection& its,
        const std::vector<Shape*>& objList,
        const std::vector<LightSource*>& lsList) const;

    Vector3D indirectRadiance(const Ray& r,
        const Intersection& its,
        const std::vector<Shape*>& objList,
        const std::vector<LightSource*>& lsList) const;

    Vector3D reflectedRadinace(const Ray& r,
        const Intersection& its,
        const std::vector<Shape*>& objList,
        const std::vector<LightSource*>& lsList) const;

    Vector3D computeRadiance(const Ray& r,
        const std::vector<Shape*>& objList, 
        const std::vector<LightSource*>& lsList) const;

    virtual Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<LightSource*>& lsList) const;

    Vector3D hitColor;
    int max_depth;
    int spp;
};

#endif
