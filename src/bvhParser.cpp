#include "bvhParser.h"

#include <cctype>

namespace bvh {

Skeleton Parser::Parse(const std::string &filename) {
  auto [data, size] = ReadFile(filename.c_str());
  if (m_fileData) { delete[](m_fileData); }
  m_fileData = data;
  m_dataSize = size;
  m_tokens.clear();
  m_pos = 0;
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
    } else if (isdigit(currChar) || currChar == '-') {
      auto lastType = m_tokens.back().type;
      if (lastType == TokenType::Float || lastType == TokenType::Offset) {
        auto number = ConsumeNonWhiteSpace();
        m_tokens.emplace_back(TokenType::Float, number);
      } else {
        auto number = ConsumeNonWhiteSpace();
        m_tokens.emplace_back(TokenType::Int, number);
      }
    } else if (currChar == '{') {
      m_pos += 2;
      m_tokens.emplace_back(TokenType::BeginScope, std::string{"{"});
    } else if (currChar == '}') {
      m_pos += 2;
      m_tokens.emplace_back(TokenType::EndScope, std::string{"}"});
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
  if (word == "HIERARCHY") {
    m_tokens.emplace_back(TokenType::Root, word);
  } else if (word == "ROOT") {
    m_tokens.emplace_back(TokenType::Root, word);
  } else if (word == "OFFSET") {
    m_tokens.emplace_back(TokenType::Offset, word);
  } else if (word == "CHANNELS") {
    m_tokens.emplace_back(TokenType::Channels, word);
  } else if (word == "XPosition") {
    m_tokens.emplace_back(TokenType::XPos, word);
  } else if (word == "YPosition") {
    m_tokens.emplace_back(TokenType::YPos, word);
  } else if (word == "ZPosition") {
    m_tokens.emplace_back(TokenType::ZPos, word);
  } else if (word == "XRotation") {
    m_tokens.emplace_back(TokenType::XRot, word);
  } else if (word == "YRotation") {
    m_tokens.emplace_back(TokenType::YRot, word);
  } else if (word == "ZRotation") {
    m_tokens.emplace_back(TokenType::ZRot, word);
  } else if (word == "JOINT") {
    m_tokens.emplace_back(TokenType::Joint, word);
  } else if (word == "End") {
    auto site = ConsumeNonWhiteSpace();
    m_tokens.emplace_back(TokenType::EndSite, word + " " + site);
  } else if (word == "MOTION") {
    m_tokens.emplace_back(TokenType::Motion, word);
  } else { // Name
    m_tokens.emplace_back(TokenType::Name, word);
  }
}

} // namespace bvh
