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
  EndScope,
  FrameTime,
};

struct Token {
  TokenType type;
  std::string value;

  Token(TokenType t, std::string &value) : type{t}, value{std::move(value)} {}
  Token(TokenType t, const std::string &value) : type{t}, value{value} {}
};

class Parser {
  char *m_fileData = nullptr;
  std::vector<Token> m_tokens = {};
  std::stack<SkeletonNode *> m_nodeStack = {};
  Size m_dataPos = 0;
  Size m_dataSize = 0;
  Size m_tokenPos = 0;

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

  /**
   * Parses the list of tokens into a SkeletonNode.
   * @return The skeletonNode.
   */
  SkeletonNode *ParseTokens();

  /**
   * Parses the tokens starting at the current token position and stores them in
   * the skeletonNode that is at the top of the nodeStack.
   * @param endsite: Bool denoting if the node being parsed is an endsite.
   */
  void ParseSkeletonNode(bool endsite = false);

  /**
   * Parses the offset values into a vec3.
   * @return: The vec3 containing the offset values.
   */
  glm::vec3 ParseOffset();

  /**
   * Parses the allowed channels into a bitmap.
   * @return: The bitmap containing the allowed channels.
   */
  u32 ParseChannels();

  void ParseMotions(SkeletonNode *skeleton);
  void ParseMotion(SkeletonNode *skeleton);
};

} // namespace bvh

#endif // BVHPARSER_H
