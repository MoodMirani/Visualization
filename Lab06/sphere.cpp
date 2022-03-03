/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:24:56
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/sphere.h>
#include <labraytracer/util.h>

namespace inviwo {

Sphere::Sphere(const vec3& center, const double& radius) {
    center_ = center;
    radius_ = radius;
}

bool Sphere::closestIntersection(const Ray& ray, double maxLambda,
                                 RayIntersection& intersection) const {
    // Programming TASK 1: implement this method
    // Your code should compute the intersection between a ray and a sphere;

    // If you detect an intersection, the return type should look similar to this:
    // Hints:
    // lambda is the distance form the ray origin an the intersection point.
    // Ray origin p_r : ray.getOrigin()
    // Ray direction t_r : ray.getDirection()
    // If you need the intersection point, use ray.pointOnRay(lambda)
    // You can ignore the uvw (texture coordinates)

    //compute the intersection between a ray and a sphere;
    //define ray
    const vec3 X = ray.getOrigin(); //set ray origin
    const vec3 D = ray.getDirection(); //set get direction
    const vec3 C = center_;
    float r = radius_;

    //define sphere
    float x = dot(D, D); //dot product of the direction of the ray
    float d = 2 * dot(D, X - C); //𝜆2(𝐭𝑟⋅𝐩𝑟)
    float c = dot((X - C), (X - C)) - r * r; //<𝐱(𝜆)−𝐜,𝐱𝜆−𝐜>−𝑟2=0, implicit sphere

    //define equation for the lambda
    float equat = d * d - 4 * x * c;

    if (equat < 0.00) {  // if equation function is not positive, return false
        return false;
    }

    float firstLambda = ((-d + sqrt(equat)) / 2 * x);
    float secondLambda = ((-d - sqrt(equat)) / 2 * x);
    float lambda; //ray parameter(dynamic variable)

    if (firstLambda >= 0.0 && secondLambda <= 0.0) {  // if lambda 1 is positive, take the positive one
        lambda = firstLambda;
    } else if (secondLambda >= 0.0 && firstLambda <= 0.0) {  // if lamb2 is positive and lamb1 negative, take the positive one
        lambda = secondLambda;
    } else if (firstLambda >= 0.0 && secondLambda >= 0.0) {  // if both values are bigger than 0,
        lambda = std::min(firstLambda, secondLambda); // get minimum positive value from the both
    } else {
        return false; //if any option contemplated before, return false
    }

    if (Util::epsilon + lambda > maxLambda) {  // if epsilon is bigger than the manimum lambda value, return false
        return false; 
    }

    const vec3 p = ray.pointOnRay(lambda); // pointing in the location where ray hits sphere
    const vec3 n = p - C; // normal vector
    const vec3 uvw(0, 0, 0); // Texture Coordinate System in 3D Environments (modeling) vector  

    //Reference in the form of a smart pointer to the Object with which the intersection occurred.
    intersection = RayIntersection(ray, shared_from_this(), lambda, n, uvw); //intersect between ray with sphere
    return true;
}

bool Sphere::anyIntersection(const Ray& ray, double maxLambda) const {
    RayIntersection temp;
    return closestIntersection(ray, maxLambda, temp);
}

void Sphere::drawGeometry(std::shared_ptr<BasicMesh> mesh,
                          std::vector<BasicMesh::Vertex>& vertices) const {
    auto indexBuffer = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::None);

    int lat = 8;
    int lon = 10;

    for (int i = 0; i < lat - 1; i++) {
        float theta1 = float(i * M_PI) / (lat - 1);
        float theta2 = float((i + 1) * M_PI) / (lat - 1);

        for (int j = 0; j < lon - 1; j++) {
            float phi1 = float(j * 2 * M_PI) / (lon - 1);
            float phi2 = float((j + 1) * 2 * M_PI) / (lon - 1);

            vec3 v1 = vec3(radius_ * sin(theta1) * cos(phi1), radius_ * sin(theta1) * sin(phi1),
                           radius_ * cos(theta1)) +
                      center_;
            vec3 v2 = vec3(radius_ * sin(theta2) * cos(phi1), radius_ * sin(theta2) * sin(phi1),
                           radius_ * cos(theta2)) +
                      center_;
            vec3 v3 = vec3(radius_ * sin(theta2) * cos(phi2), radius_ * sin(theta2) * sin(phi2),
                           radius_ * cos(theta2)) +
                      center_;

            Util::drawLineSegment(v1, v2, vec4(0.2, 0.2, 0.2, 1), indexBuffer.get(), vertices);
            Util::drawLineSegment(v2, v3, vec4(0.2, 0.2, 0.2, 1), indexBuffer.get(), vertices);
        }
    }
}

}  // namespace inviwo
