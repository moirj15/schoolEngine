#include "boundingbox.h"

bool BoundingBox::IntersectRay(const Ray &ray) {
  f32 txMin = (m_bounds[ray.sign[0]].x - ray.origin.x) * ray.inverseDir.x;
  f32 txMax = (m_bounds[1 - ray.sign[0]].x - ray.origin.x) * ray.inverseDir.x;
  f32 tyMin = (m_bounds[ray.sign[1]].y - ray.origin.y) * ray.inverseDir.y;
  f32 tyMax = (m_bounds[1 - ray.sign[1]].y - ray.origin.y) * ray.inverseDir.y;
  if ((txMin > tyMax) || (tyMin > txMax)) { return false; }
  if (tyMin > txMin) { txMin = tyMin; }
  if (tyMax < txMax) { txMax = tyMax; }
  f32 tzMin = (m_bounds[ray.sign[2]].z - ray.origin.z) * ray.inverseDir.z;
  f32 tzMax = (m_bounds[1 - ray.sign[2]].z - ray.origin.z) * ray.inverseDir.z;
  if ((txMin > tzMax) || (tzMin > txMax)) { return false; }
  if (tzMin > txMin) { txMin = tzMin; }
  if (tzMax < txMax) { txMax = tzMax; }

  return true;
}
