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
  size m_dataPos = 0;
  size m_dataSize = 0;
  size m_tokenPos = 0;

public:
  Parser() = default;
  /// Manual deletion cause std::unique_ptr wasn't working (at least the way I
  /// expected)
  ~Parser() { delete[](m_fileData); }

  SkeletonNode *Parse(const std::string &filename);

private:
  /**
   * Converts the contents of m_fileData into tokens.
   */
  void Tokenize();

  /**
   * Consumes non-whitespace characters until a whitespace character is reached.
   * @return The non-whitespace characters.
   */
  std::string ConsumeNonWhiteSpace();

  /**
   * Consumes whitespace until a non-whitespace character is reached.
   */
  void ConsumeWhiteSpace();

  /**
   * Creates a token using the string containig the keyword. For this parser a
   * "keyword" is either a keyword specified in the bvh specification or the
   * name of a joint.
   * @param word - The string containing the keyword.
   */
  void CreateKeyWordToken(std::string &word);

  SkeletonNode *ParseTokens();

  void ParseSkeletonNode(bool endsite = false);

  glm::vec3 ParseOffset();

  u32 ParseChannels();
};

} // namespace bvh

#endif // BVHPARSER_H
