#ifndef BVHPARSER_H
#define BVHPARSER_H

#include "common.h"
#include "skeleton.h"

#include <memory>
#include <stack>
#include <string>

namespace bvh {

enum class TokenType {
  Hierarchy,
  Root,
  Name,
  Offset,
  Value,
  EndSite,
  XPos,
  YPos,
  ZPos,
  XRot,
  YRot,
  ZRot,
  Motion,
  Float,
  Int,
  Channels,
  Joint,
  BeginScope,
  EndScope
};

struct Token {
  TokenType type;
  std::string value;

  Token(TokenType t, std::string &value) : type{t}, value{std::move(value)} {}
};

class Parser {
  char *m_fileData = nullptr;
  std::vector<Token> m_tokens = {};
  std::stack<SkeletonNode *> m_nodeStack = {};
  size m_pos = 0;
  size m_dataSize = 0;

public:
  Parser() = default;
  ~Parser() { delete[](m_fileData); }

  Skeleton Parse(const std::string &filename);

private:
  void Tokenize();
  std::string ConsumeNonWhiteSpace();
  void ConsumeWhiteSpace();
  void CreateKeyWordToken(std::string &word);
};

} // namespace bvh

#endif // BVHPARSER_H
