#ifndef BVHPARSER_H
#define BVHPARSER_H

#include "common.h"

#include <memory>

class BVHParser {




  std::unique_ptr<char[]> m_fileData;
public:
  BVHParser();
};

#endif // BVHPARSER_H
