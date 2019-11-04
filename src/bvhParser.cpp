#include "bvhParser.h"

#include <cctype>

namespace bvh {

SkeletonNode *Parser::Parse(const std::string &filename) {
  auto [data, size] = ReadFile(filename.c_str());
  if (m_fileData) { delete[](m_fileData); }
  m_fileData = data;
  m_dataSize = size;
  m_tokens.clear();
  m_dataPos = 0;
  Tokenize();
  auto skeleton = ParseTokens();

  return skeleton;
}

// Private
void Parser::Tokenize() {
  while (m_dataPos < m_dataSize) {
    char currChar = m_fileData[m_dataPos];
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
      m_dataPos += 2;
      m_tokens.emplace_back(TokenType::BeginScope, std::string{"{"});
    } else if (currChar == '}') {
      m_dataPos += 2;
      m_tokens.emplace_back(TokenType::EndScope, std::string{"}"});
    }
  }
}

std::string Parser::ConsumeNonWhiteSpace() {
  std::string ret;
  while (!isspace(m_fileData[m_dataPos])) {
    ret += m_fileData[m_dataPos];
    m_dataPos++;
  }
  m_dataPos++;
  return ret;
}

void Parser::ConsumeWhiteSpace() {
  while (isspace(m_fileData[m_dataPos])) { m_dataPos++; }
}

void Parser::CreateKeyWordToken(std::string &word) {
  if (word == "HIERARCHY") {
    m_tokens.emplace_back(TokenType::Hierarchy, word);
  } else if (word == "ROOT") {
    m_tokens.emplace_back(TokenType::Root, word);
  } else if (word == "OFFSET") {
    m_tokens.emplace_back(TokenType::Offset, word);
  } else if (word == "CHANNELS") {
    m_tokens.emplace_back(TokenType::Channels, word);
  } else if (word == "Xposition") {
    m_tokens.emplace_back(TokenType::XPos, word);
  } else if (word == "Yposition") {
    m_tokens.emplace_back(TokenType::YPos, word);
  } else if (word == "Zposition") {
    m_tokens.emplace_back(TokenType::ZPos, word);
  } else if (word == "Xrotation") {
    m_tokens.emplace_back(TokenType::XRot, word);
  } else if (word == "Yrotation") {
    m_tokens.emplace_back(TokenType::YRot, word);
  } else if (word == "Zrotation") {
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

SkeletonNode *Parser::ParseTokens() {
  while (m_tokenPos < m_tokens.size()) {
    auto currToken = m_tokens[m_tokenPos];
    if (currToken.type == TokenType::Hierarchy
        || currToken.type == TokenType::Root
        || currToken.type == TokenType::Joint) {
      m_tokenPos++;
    } else if (currToken.type == TokenType::Name) {
      m_nodeStack.emplace(new SkeletonNode{currToken.value});
      m_tokenPos++;
    } else if (currToken.type == TokenType::BeginScope) {
      m_tokenPos++;
      ParseSkeletonNode();
    } else if (currToken.type == TokenType::EndScope) {
      m_tokenPos++;
      auto *node = m_nodeStack.top();
      m_nodeStack.pop();
      return node;
    } else if (currToken.type == TokenType::EndSite) {
      m_tokenPos++;
      ParseSkeletonNode(true);
    }
  }
}

void Parser::ParseSkeletonNode(bool endsite) {
  auto *currNode = m_nodeStack.top();
  currNode->offset = ParseOffset();
  if (!endsite) { currNode->allowedMotions = ParseChannels(); }

  if (m_tokens[m_tokenPos].type == TokenType::Joint) {
    currNode->m_children.emplace_back(ParseTokens());
  } else if (m_tokens[m_tokenPos].type == TokenType::EndSite) {
    auto *leafNode = new SkeletonNode{m_tokens[m_tokenPos].value};
    m_tokenPos += 2;
    leafNode->offset = ParseOffset();
    currNode->m_children.emplace_back(leafNode);
  }
}

glm::vec3 Parser::ParseOffset() {
  m_tokenPos++;
  glm::vec3 ret;
  for (s32 i = 0; i < 3; i++) {
    ret[i] = std::stof(m_tokens[m_tokenPos].value);
    m_tokenPos++;
  }
  return ret;
}

u32 Parser::ParseChannels() {
  m_tokenPos++;
  u32 ret = 0;
  s32 count = std::stoi(m_tokens[m_tokenPos].value);
  for (s32 i = 0; i < count; i++) {
    m_tokenPos++;
    auto currType = m_tokens[m_tokenPos].type;
    switch (currType) {
    case TokenType::XPos:
      ret |= X_POS;
      break;
    case TokenType::YPos:
      ret |= Y_POS;
      break;
    case TokenType::ZPos:
      ret |= Z_POS;
      break;
    case TokenType::XRot:
      ret |= X_ROT;
      break;
    case TokenType::YRot:
      ret |= Y_ROT;
      break;
    case TokenType::ZRot:
      ret |= Z_ROT;
      break;
    }
  }
  m_tokenPos++;
  return ret;
}

// void Parser::ParseEndSite() {
//  m_nodeStack.emplace(new SkeletonNode(m_tokens[m_tokenPos].value));
//  m_tokenPos++;
//  m_nodeStack.top()->offset = ParseOffset();
//}

} // namespace bvh
