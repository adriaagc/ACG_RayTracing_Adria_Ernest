#define _USE_MATH_DEFINES //so we can use M_PI

#include "nexteventintegrator.h"
#include "../core/utils.h"
#include "../materials/phong.h"

#include "../core/hemisphericalsampler.h" // we need to get random light vectors


NextEventIntegrator::NextEventIntegrator() :
    hitColor(Vector3D(1, 0, 0)), max_depth(4), spp(256)
{
}

NextEventIntegrator::NextEventIntegrator(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{
}

Vector3D NextEventIntegrator::computeRadiance(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{
    Intersection its;
    Vector3D Le;
    Vector3D Lr;
    if (Utils::getClosestIntersection(r, objList, its)) {
        if (its.shape->getMaterial().isEmissive()) {
            Le = its.shape->getMaterial().getEmissiveRadiance();
        }
        Lr = reflectedRadinace(r, its, objList, lsList);
    }
    return Le + Lr;
}

Vector3D NextEventIntegrator::reflectedRadinace(const Ray& r, const Intersection& its, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{
    Vector3D Ldir = directRadiance(r, its, objList, lsList);
    Vector3D Lind = indirectRadiance(r, its, objList, lsList);
    return Ldir + Lind;
}

Vector3D NextEventIntegrator::directRadiance(const Ray& r, const Intersection& its, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{
    Vector3D x = its.itsPoint;
    Vector3D nx = its.normal;
    Vector3D wo = -r.d;
    Vector3D Ldir = Vector3D(0.0);
    for (auto s : lsList) {
        float lsArea = s->getArea();
        float P_wi = 1 / lsArea; // probability distribution = 1/A
        Vector3D ny = s->getNormal();
        Vector3D y = s->sampleLightPosition();   // generates a random point inside the square ls
        Vector3D wi = (y - x).normalized();      // unit vector that goes from x towards a random point from the ls (y)

        Ray ls_ray = Ray(x, wi);                        // ray towards the light source
        Intersection its2;

        Vector3D L_i_dir = Vector3D(0.0);

        if (Utils::getClosestIntersection(ls_ray, objList, its2)) {   // check for occlusion !!

            if (its2.shape->getMaterial().isEmissive()) {
                L_i_dir = its2.shape->getMaterial().getEmissiveRadiance();   // Le only contributes if its a ls (direct illumination)
            }
            else {
                // replaces the visibility term:
                //    if the object the ray hits 1st is not a ls (emissive mat) then it is occluded.
                continue;
            }
        }
        else {
            // if there is no intersection!
            continue;
        }

        Vector3D G_xy = (std::max(0.0, dot(nx, wi)) * std::max(0.0, dot(-wi, ny))) / (y - x).lengthSq();   // geometric term

        Vector3D fr = its.shape->getMaterial().getReflectance(nx, wo, wi);

        Ldir += L_i_dir * fr * G_xy / P_wi;
    }
    return Ldir;
}

Vector3D NextEventIntegrator::indirectRadiance(const Ray& r, const Intersection& its, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{
    Vector3D n = its.normal;
    Vector3D x = its.itsPoint;
    Vector3D wo = -r.d;
    HemisphericalSampler sampler;
    Vector3D wi = sampler.getSample(n);
    float pdf = 1.0 / (2 * M_PI);
    Vector3D Lind = Vector3D(0.0);
    Ray newR = Ray(x, wi, r.depth + 1);
    if (newR.depth < max_depth) {
        Intersection its2;
        if (Utils::getClosestIntersection(newR, objList, its2)) {
            Vector3D y = its2.itsPoint;
            Lind = reflectedRadinace(newR, its2, objList, lsList) * its.shape->getMaterial().getReflectance(n, wo, wi) * dot(n, wi) / pdf;
        }
        return Lind;
    }

}

Vector3D NextEventIntegrator::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{
    Vector3D Lo = Vector3D(0.0);

    // Monte Carlo over the pixel (spp)
    for (int i = 0; i < spp; i++) {
        Lo += computeRadiance(r, objList, lsList);
    }
    return Lo / float(spp);
}
