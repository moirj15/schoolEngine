#include "bvhParser.h"

#include <cctype>

namespace bvh {

Skeleton Parser::Parse(const std::string &filename) {
  m_fileData.reset(ReadFile(filename.c_str()));
  m_dataSize = filename.size();
  m_tokens.clear();
  Skeleton skeleton;
  Tokenize();

  return skeleton;
}

// Private
void Parser::Tokenize() {
  while (m_pos < m_dataSize) {
    char currChar = m_fileData[m_pos];
    if (isalpha(currChar)) {
      auto word = ConsumeNonWhiteSpace();
      CreateKeyWordToken(word);
    } else if (isspace(currChar)) {
      ConsumeWhiteSpace();
    } else if (isdigit(currChar)) {
      auto lastType = m_tokens.back().type;
      if (lastType == TokenType::Float || lastType == TokenType::Offset) {
        auto number = ConsumeNonWhiteSpace();
        m_tokens.emplace_back(TokenType::Float, number);
      } else {
        auto number = ConsumeNonWhiteSpace();
        m_tokens.emplace_back(TokenType::Int, number);
      }
    }
  }
}

std::string Parser::ConsumeNonWhiteSpace() {
  std::string ret;
  while (!isspace(m_fileData[m_pos])) {
    ret += m_fileData[m_pos];
    m_pos++;
  }
  m_pos++;
  return ret;
}

void Parser::ConsumeWhiteSpace() {
  while (isspace(m_fileData[m_pos])) { m_pos++; }
}

void Parser::CreateKeyWordToken(std::string &word) {
}

} // namespace bvh
