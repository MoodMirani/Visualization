/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:24:30
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/triangle.h>
#include <labraytracer/util.h>
#include <memory>

namespace inviwo {

Triangle::Triangle() {}

Triangle::Triangle(const vec3& v0, const vec3& v1, const vec3& v2, const vec3& uvw0,
                   const vec3& uvw1, const vec3& uvw2) {
    mVertices[0] = v0;
    mVertices[1] = v1;
    mVertices[2] = v2;
    mUVW[0] = uvw0;
    mUVW[1] = uvw1;
    mUVW[2] = uvw2;
}

bool Triangle::closestIntersection(const Ray& ray, double maxLambda,
                                   RayIntersection& intersection) const {
    // Programming TASK 1: Implement this method
    // Your code should compute the intersection between a ray and a triangle.
    //
    // If you detect an intersection, the return type should look similar to this:
    // if(rayIntersectsTriangle)
    //{
    //  intersection = RayIntersection(ray,shared_from_this(),lambda,ray.normal,uvw);
    //  return true;
    //}
    //
    // Hints :
    // Ray origin p_r : ray.getOrigin()
    // Ray direction t_r : ray.getDirection()
    // Compute the intersection point using ray.pointOnRay(lambda)

    vec3 tr = ray.getDirection();
    vec3 pr = ray.getOrigin();

    vec3 p0 = mVertices[0];
    vec3 p1 = mVertices[1];
    vec3 p2 = mVertices[2];

    //Lecture 08 - Raytracing
    //Page 18 Ray-Triangle intersection
    vec3 t1 = p1 - p0;
    vec3 t2 = p2 - p0;

    // n=t1xt2
    vec3 normal = glm::normalize(glm::cross(t1, t2));

   double lambda = 0;

    // lambda = (p0-pr) x n/(tr x n)
    // page 17. In practice tr x n > epsilon
    if (fabs(dot(tr, normal)) > Util::epsilon) {
        lambda = dot(p0 - pr, normal) / dot(tr, normal);
    } 
    else
        return false;


    //why?
    if (lambda < 0.0 || lambda + Util::epsilon > maxLambda) {
        return false;
    }

    vec3 p = ray.pointOnRay(lambda) - p0;  // point on ray

    // barycentric coordinates ?

    float denomenator2 = (dot(t1, t2) * dot(t1, t2) - dot(t1, t1) * dot(t2, t2));

    // ??
    float lambda1 = (dot(t1, t2) * dot(p, t2) - dot(t2, t2) * dot(p, t1)) / denomenator2;
    float lambda2 = (dot(t1, t2) * dot(p, t2) - dot(t1, t1) * dot(p, t2)) / denomenator2;

    // lambda 1 and 2 must be 0 or higher, sum cannot be above 1
    // also need to add checks if lambda1 and lambda2 less than 1, page 18
    if (lambda1 >= 0.0 && lambda2 >= 0.0 && (lambda1 + lambda2 <= 1.0)) {
        vec3 uvw(0, 0, 0);
        intersection = RayIntersection(ray, shared_from_this(), lambda, normal, uvw);
        return true;
    }

    return false;
}

bool Triangle::anyIntersection(const Ray& ray, double maxLambda) const {
    RayIntersection temp;
    return closestIntersection(ray, maxLambda, temp);
}

void Triangle::drawGeometry(std::shared_ptr<BasicMesh> mesh,
                            std::vector<BasicMesh::Vertex>& vertices) const {
    auto indexBuffer = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::None);

    Util::drawLineSegment(mVertices[0], mVertices[1], vec4(0.2, 0.2, 0.2, 1), indexBuffer.get(),
                          vertices);
    Util::drawLineSegment(mVertices[1], mVertices[2], vec4(0.2, 0.2, 0.2, 1), indexBuffer.get(),
                          vertices);
    Util::drawLineSegment(mVertices[2], mVertices[0], vec4(0.2, 0.2, 0.2, 1), indexBuffer.get(),
                          vertices);
}

}  // namespace inviwo
