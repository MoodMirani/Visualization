/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 24, 2017 - 17:17:44
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/phongmaterial.h>
#include <labraytracer/util.h>

namespace inviwo {

PhongMaterial::PhongMaterial(const vec3& color, const double reflectance, const double shininess,
    const vec3& ambientMaterialColor, const vec3& diffuseMaterialColor, const vec3& specularMaterialColor) 
    : Material(color, reflectance) {

    constexpr double LightIntensity = 100.0;

    shininess_ = shininess;
    ambientMaterialColor_   = Util::scalarMult(LightIntensity, ambientMaterialColor);
    diffuseMaterialColor_   = Util::scalarMult(LightIntensity, diffuseMaterialColor);
    specularMaterialColor_  = Util::scalarMult(LightIntensity, specularMaterialColor);
}

vec4 PhongMaterial::shade(const RayIntersection& intersection, const Light& light) const {
    // get normal and light direction
    vec3 N = intersection.getNormal(); //get normal vector, i.e surface normal orthogonal to surface
    vec3 L = Util::normalize(light.getPosition() - intersection.getPosition()); //normalized light direction

    double cosNL = std::max(double(dot(N, L)), double(0));

    // Programming Task 2: Extend this method.
    // This method currently implements a Lambert's material with ideal
    // diffuse reflection.
    // Your task is to implement a Phong shading model.
    // 
    // Hints:
    //
    // 1. This function should return the sum of diffuse and specular parts (no ambient part)
    // 2. The used light color for each type (diffuse/specular) from the light source 
    //    is the light color divided by the quadratic distance of the light source from 
    //    the point of intersection. (quadratic falloff)
    // 3. The view vector V is the direction of the ray which intersects the object.
    // 4. The rest of the terms as per the slides are as follows
    // 5. You have to take into account shininess_ (p), material colors, light colors 
    //    light, view, reflection and normal vector.

    /* vec3 V = glm::normalize(
        -intersection.getRay().getDirection());  // V represents the direction of the ray
                                                 // intersection with the obj, i.e. the view vector
    vec3 R = glm::normalize(2 * cosNL * N - L); //reflection vector
    double cosRV = glm::max(double(dot(V, R)), double(0)); 

    //calculation of distance between position of the light and position of the intersection point
    vec3 distanceVector = light.getPosition() - intersection.getPosition();
    double distance = length(distanceVector); 
    double attentuation = (1 / pow(distance, 2)); // 1/(dist^2), these are the point lights

    vec3 diffuseLightColor = light.getDiffuseColor() * cosNL * attentuation; //multiply the diffuse color for the attenuation to get the diffuse light color 
    vec3 specularLightColor = light.getSpecularColor() * pow(distance, 2) * attentuation;  // multiply the diffuse color for the attenuation to get the specular light color

    //vec3 diffC = diffuseLightColor * cosNL * diffuseMaterialColor_; // get the result of the diffuse C component after the diffuse lights hits the material
    //vec3 specC = specularLightColor * pow(cosRV, shininess_) * specularMaterialColor_;  // get the result of the specular C component after the soecular light hits the material
    */

    // distance
    vec3 distanceVec = light.getPosition() - intersection.getPosition(); //this is pos_light - pos_object
    double distance = length(distanceVec);
    double attentuation = glm::pow(distance, 2);
    float ambientStrength = 0.005;

    
    vec3 normN = Util::normalize(N);
    vec3 N2 = Util::scalarMult(2, normN);  // scalar multiplication of normalized N

    double T1 = std::max(double(dot(normN, L)), double(0.0)); //get maximum value from the dot product between normalized N and L vectors
    vec3 R0 = Util::scalarMult(T1, N2); //scalar multiplication between t1 and N2

    vec3 R = - (R0 - L);
    vec3 normR = Util::normalize(R);

    vec3 V = intersection.getRay().getDirection();
    vec3 normV = Util::normalize(V);

    double T2 = std::max(double(dot(normR, normV)), double(0.0));

    //ambient part
    vec3 ambiC = vec3(ambientStrength * ambientMaterialColor_[0] * light.getAmbientColor()[0], //we dive and multiply three times as we use an RGB model
                      ambientStrength * ambientMaterialColor_[1] * light.getAmbientColor()[1],
                      ambientStrength * ambientMaterialColor_[2] * light.getAmbientColor()[2]);

    // Specular (glossy) part
    vec3 specC = vec3(specularMaterialColor_[0] * light.getSpecularColor()[0] * glm::pow(T2, shininess_) / attentuation, //we dive and multiply three times as we use an RGB model
                      specularMaterialColor_[1] * light.getSpecularColor()[1] * glm::pow(T2, shininess_) / attentuation,  
                      specularMaterialColor_[2] * light.getSpecularColor()[2] * glm::pow(T2, shininess_) / attentuation);

    // Diffuse part
    vec3 diffC = vec3((diffuseMaterialColor_[0] * light.getDiffuseColor()[0] * cosNL) / attentuation, //we dive and multiply three times as we use an RGB model
                      (diffuseMaterialColor_[1] * light.getDiffuseColor()[1] * cosNL) / attentuation,
                      (diffuseMaterialColor_[2] * light.getDiffuseColor()[2] * cosNL) / attentuation);


    vec3 sumC = (ambiC + specC + diffC); // sum both diffuse and specular color components //color

    return vec4(sumC, 1.0);
    //return vec4(Util::scalarMult(cosNL, this->color()), 1.0);
}

} // namespace
