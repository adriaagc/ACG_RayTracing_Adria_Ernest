#include "whittedintegrator.h"
#include "../core/utils.h"
#include "../materials/phong.h"

WhittedIntegrator::WhittedIntegrator() :
    hitColor(Vector3D(1, 0, 0))
{
}

WhittedIntegrator::WhittedIntegrator(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{
}

Vector3D WhittedIntegrator::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{
    Intersection its;
    if (Utils::getClosestIntersection(r, objList, its)) {

        //Vector3D lo = Vector3D(0.0); // Whithout any constant light
        Vector3D lo = Vector3D(0.1) * its.shape->getMaterial().getDiffuseReflectance(); // With a constant light

        for (auto s: lsList) {
            Vector3D n = its.normal;
            Vector3D x = its.itsPoint;                  // intersection point
            Vector3D wi = -(x - s->sampleLightPosition()).normalized(); // vector light source to 
            Vector3D fr = its.shape->getMaterial().getReflectance(n, r.d.normalized(), wi.normalized());
            Ray lightToObject = Ray(s->sampleLightPosition(), -wi);
            lightToObject.maxT = (x - s->sampleLightPosition()).length() - 1.0E-4; // the ray must finish before reaching x.
            int visibility = int(!Utils::hasIntersection(lightToObject, objList));
            lo += s->getIntensity()*fr*dot(wi,n)*visibility;
        }
        return lo;
    }
    return bgColor; // If it does not hit anything
}
