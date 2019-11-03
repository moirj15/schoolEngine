#include "bvhParser.h"

Skeleton BVHParser::Parse(const std::string &filename) {
  m_fileData.reset(ReadFile(filename.c_str()));
  Skeleton skeleton;

  return skeleton;
}
