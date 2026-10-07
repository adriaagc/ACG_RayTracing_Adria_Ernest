#define _USE_MATH_DEFINES //so we can use M_PI

#include "phong.h"

#include <iostream>

#include "../core/utils.h" //for computeReflectionDirection



Phong::Phong()
{ }

Phong::Phong(Vector3D Kd_, Vector3D Ks_, float alpha_):
rho_d(Kd_), Ks(Ks_), alpha(alpha_){}


Vector3D Phong::getReflectance(const Vector3D& n, const Vector3D& wo,
    const Vector3D& wi) const {

    //FILL(...)
    Vector3D wr = Utils::computeReflectionDirection(wi, n);
    Vector3D f_r = (rho_d / M_PI) + Ks * pow(dot(wo, wr), alpha); // std::max(dot(wo, wr), 0.0) ???

    return f_r;
};

double Phong::getIndexOfRefraction() const
{
    std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
              << std::endl;

    return -1;
}


Vector3D Phong::getEmissiveRadiance() const
{
    return Vector3D(0.0);
}


Vector3D Phong::getDiffuseReflectance() const
{
    return rho_d;
}

