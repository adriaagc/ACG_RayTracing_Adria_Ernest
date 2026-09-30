#include "intersectionshader.h"
#include "../core/utils.h"

IntersectionShader::IntersectionShader() :
    hitColor(Vector3D(1, 0, 0))
{ }

IntersectionShader::IntersectionShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{ }

Vector3D IntersectionShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<LightSource*> &lsList) const
{
    //(FILL..)
   
    //Example 1
    if (Utils::hasIntersection(r, objList)) {
        return hitColor;
    }
    return bgColor;

    //Example 2 
    Intersection its;
    if (Utils::getClosestIntersection(r, objList, its)) {
        return Vector3D(0.0, 1.0, 0.0);
    }
    return bgColor;

    //Example 3
    Intersection its;
    if (Utils::getClosestIntersection(r, objList, its)) {
        //origin point
        Vector3D ray_position = r.o;
        //intersection point
        Vector3D its_pos = its.itsPoint;
        //Distance between this two points
        float distance = (its_pos - ray_position).length();
        return Vector3D(0.0, 1.0 - distance / 8.0, 0.0);
    }
    return bgColor;

}
