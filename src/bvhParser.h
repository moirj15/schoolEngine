#ifndef BVHPARSER_H
#define BVHPARSER_H

#include "common.h"
#include "skeleton.h"

#include <memory>

class BVHParser {
  std::unique_ptr<char[]> m_fileData;

public:
  BVHParser() = default;
  ~BVHParser() = default;

  Skeleton Parse(const std::string &filename);
};

#endif // BVHPARSER_H
