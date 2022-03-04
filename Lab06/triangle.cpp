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
    vec3 uvw(0, 0, 0);

    vec3 p0 = mVertices[0];
    vec3 p1 = mVertices[1];
    vec3 p2 = mVertices[2];

    //Lecture 08 - Raytracing
    //Page 18 Ray-Triangle intersection
    vec3 t1 = p1 - p0;
    vec3 t2 = p2 - p0;

    // n=t1xt2
    vec3 n = glm::normalize(glm::cross(t1, t2));

    double lambda = -1;

    // lambda = (p0-pr) x n/(tr x n)
    // page 17. In practice tr x n > epsilon
    if (fabs(dot(tr, n)) > Util::epsilon) {
        lambda = dot(p0 - pr, n) / dot(tr, n);
    } 
    else
        return false;

    if (lambda < 0 || lambda > maxLambda) {
        return false;
    }

    vec3 p = ray.pointOnRay(lambda);

    //check if point inside triangle geometrically
    //https://ppt-online.org/703109 slide 25 (University of Virginia 445)
    vec3 cp1 = cross(p2 - p1, p - p1);
    vec3 cp2 = cross(p2 - p1, p0 - p1);
    vec3 cp3 = cross(p2 - p0, p - p0);
    vec3 cp4 = cross(p2 - p0, p1 - p0);
    vec3 cp5 = cross(p1 - p0, p - p0);
    vec3 cp6 = cross(p1 - p0, p2 - p0);
    if (dot(cp1, cp2) < 0) return false;
    if (dot(cp3, cp4) < 0) return false;
    if (dot(cp5, cp6) < 0) return false;

        
    intersection = RayIntersection(ray, shared_from_this(), lambda, ray.pointOnRay(lambda), uvw);
    return true;

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
