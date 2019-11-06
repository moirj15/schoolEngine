#include "bvhParser.h"

#include <cctype>

namespace bvh {

SkeletonNode *Parser::Parse(const std::string &filename) {
  auto [data, size] = ReadFile(filename.c_str());
  if (m_fileData) {
    delete[](m_fileData);
  }
  m_fileData = data;
  m_dataSize = size;
  m_tokens.clear();
  m_dataPos = 0;
  Tokenize();
  auto skeleton = ParseTokens();
  ParseMotions(skeleton);

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
  while (isspace(m_fileData[m_dataPos])) {
    m_dataPos++;
  }
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
  } else if (word == "Frame") {
    auto time = ConsumeNonWhiteSpace();
    m_tokens.emplace_back(TokenType::FrameTime, word + " " + time);
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
  if (!endsite) {
    currNode->allowedMotions = ParseChannels();
  }

  if (m_tokens[m_tokenPos].type == TokenType::Joint) {
    while (m_tokens[m_tokenPos].type != TokenType::EndScope) {
      currNode->children.emplace_back(ParseTokens());
    }
  } else if (m_tokens[m_tokenPos].type == TokenType::EndSite) {
    auto *leafNode = new SkeletonNode{m_tokens[m_tokenPos].value};
    m_tokenPos += 2;
    leafNode->offset = ParseOffset();
    currNode->children.emplace_back(leafNode);
    m_tokenPos++;
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

void Parser::ParseMotions(SkeletonNode *skeleton) {
  m_tokenPos += 2; // skip over the Motion and Frames tokens
  const u32 frameCount = std::stoi(m_tokens[m_tokenPos].value);
  m_tokenPos += 2;
  const f32 frameTime = std::stof(m_tokens[m_tokenPos].value);
  m_tokenPos++;
  auto skeletonNodes = skeleton->ToList();
  for (auto *node : skeletonNodes) {
    printf("allowed motions: 0x%x, name: %s\n", node->allowedMotions,
        node->name.c_str());
  }
  for (u32 i = 0; i < frameCount; i++) {
    for (auto *node : skeletonNodes) {
      // Don't process end sites, since they don't have rotation applied to them
      if (node->children.size() > 0) {
        ParseMotion(node);
      }
    }
  }
}

void Parser::ParseMotion(SkeletonNode *skeleton) {
  const std::vector<u32> masks = {X_POS, Y_POS, Z_POS, Z_ROT, X_ROT, Y_ROT};
  NodeMotion motion;
  for (const auto mask : masks) {
    u32 bit = skeleton->allowedMotions & mask;
    if (bit == X_POS) {
      motion.translations.x = std::stof(m_tokens[m_tokenPos].value);
      m_tokenPos++;
    } else if (bit == Y_POS) {
      motion.translations.y = std::stof(m_tokens[m_tokenPos].value);
      m_tokenPos++;
    } else if (bit == Z_POS) {
      motion.translations.z = std::stof(m_tokens[m_tokenPos].value);
      m_tokenPos++;
    } else if (bit == Z_ROT) {
      motion.rotations.z = std::stof(m_tokens[m_tokenPos].value);
      m_tokenPos++;
    } else if (bit == X_ROT) {
      motion.rotations.x = std::stof(m_tokens[m_tokenPos].value);
      m_tokenPos++;
    } else if (bit == Y_ROT) {
      motion.rotations.y = std::stof(m_tokens[m_tokenPos].value);
      m_tokenPos++;
    }
  }
  skeleton->motions.push_back(motion);
}

} // namespace bvh
