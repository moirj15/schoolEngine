#include "obj.h"

#include "VertexBuffer.h"

#include <algorithm>
#include <cstdio>
#include <sstream>

ObjReader::ObjReader() : m_filename(""), m_dataLen(0), m_pos(0) {
}

Mesh *ObjReader::Parse(const std::string &filename) {
  Clear();
  ReadFile(filename);
  while (m_pos < m_dataLen) {
    auto dataType = ParseType();
    switch (dataType) {
    case DataType::Vertex:
      ParseVertex();
      break;
    case DataType::TextureCoord:
      ParseTextureCoord();
      break;
    case DataType::VertexNormal:
      ParseNormal();
      break;
    case DataType::ParameterSpaceVertex:
      fprintf(stderr, "NOT IMPLEMENTED\n");
      break;
    case DataType::PolygonFace:
      ParseFace();
      break;
    case DataType::LineElement:
      fprintf(stderr, "NOT IMPLEMENTED\n");
      break;
    case DataType::Object:
      fprintf(stderr, "NOT IMPLEMENTED\n");
      SkipLine();
      break;
    case DataType::Group:
      fprintf(stderr, "NOT IMPLEMENTED\n");
      break;
    case DataType::Comment:
      SkipLine();
      break;
    case DataType::Unknown:
      fprintf(stderr, "NOT IMPLEMENTED\n");
      SkipLine();
      break;
    }
  }
  // This should clear out m_mesh
  return CreateMeshFromFaces();
}

void ObjReader::Clear() {
  m_data.reset();
  m_vertecies.clear();
  m_normals.clear();
  m_textureCoords.clear();
  m_faces.clear();
  m_filename.clear();
  m_dataLen = 0;
  m_pos = 0;
}

// Private

void ObjReader::ReadFile(const std::string &filename) {
  m_filename = filename;
  FILE *fp = OpenFile(m_filename.c_str(), "r");
  char *data = nullptr;
  size_t len = 0;

  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  rewind(fp);

  if (len == 0) { fprintf(stderr, "failed to get file size"); }

  // m_data.reserve(len + 1);
  data = new char[len + 1]();
  m_dataLen = len + 1;
  len = fread(data, sizeof(char), len, fp);
  // len = fread(&m_data[0], sizeof(char), len, fp);
  m_data.reset(data);

  fclose(fp);
}

ObjReader::DataType ObjReader::ParseType() {
  if (Token() == 'f') {
    m_pos += 2;
    return DataType::PolygonFace;
  } else if (Token() == 'l') {
    m_pos += 2;
    return DataType::LineElement;
  } else if (Token() == 'o') {
    m_pos += 2;
    return DataType::Object;
  } else if (Token() == 'v') {
    m_pos++;
    if (Token() == ' ') {
      m_pos++;
      return DataType::Vertex;
    } else if (Token() == 't') {
      m_pos += 2;
      return DataType::TextureCoord;
    } else if (Token() == 'p') {
      m_pos += 2;
      return DataType::ParameterSpaceVertex;
    } else if (Token() == 'n') {
      m_pos += 2;
      return DataType::VertexNormal;
    }
  } else if (Token() == '#') {
    return DataType::Comment;
  }
  fprintf(stderr, "Unrecognized OBJ datatype\n");
  return DataType::Unknown;
}

// TODO: Refractor into a single function
void ObjReader::ParseVertex() {
  std::stringstream line{ReadLine()};
  f32 val = 0.0f;
  glm::vec3 vertex;
  for (s32 i = 0; i < 3; i++) {
    line >> val;
    vertex[i] = val;
  }
  m_vertecies.emplace_back(vertex);
}

void ObjReader::ParseTextureCoord() {
  std::stringstream line{ReadLine()};
  f32 val = 0.0f;
  glm::vec2 textureCoord;
  for (s32 i = 0; i < 2; i++) {
    line >> val;
    textureCoord[i] = val;
  }
  m_textureCoords.emplace_back(textureCoord);
}

void ObjReader::ParseNormal() {
  std::stringstream line{ReadLine()};
  f32 val = 0.0f;
  glm::vec3 normal;
  for (s32 i = 0; i < 3; i++) {
    line >> val;
    normal[i] = val;
  }
  m_normals.emplace_back(normal);
}
// END TODO

void ObjReader::ParseFace() {
  std::string lineStr{ReadLine()};
  ReplaceChars(&lineStr, '/', ' ');
  std::stringstream line{lineStr};
  //  if (m_normals.size() > 0 && m_textureCoords.size() > 0
  //      && m_vertecies.size() > 0) {
  for (s32 i = 0; i < 3; i++) {
    Face face;
    u32 vertConnection = 0.0f;
    u32 textureConnection = 0.0f;
    u32 normalConnection = 0.0f;
    line >> vertConnection >> textureConnection >> normalConnection;
    m_faceVertIndecies.emplace_back(vertConnection - 1);
    face.index = vertConnection - 1;
    face.vert = m_vertecies[vertConnection - 1];
    face.normal = m_normals[normalConnection - 1];
    face.textureCoord = m_textureCoords[textureConnection - 1];
    m_faces.emplace_back(face);
  }
  //  } else {
  //    for (s32 i = 0; i < 3; i++) {
  //      u32 val = 0.0f;
  //      u32 junk = 0.0f;
  //      line >> val >> junk;
  //      m_mesh.connections.emplace_back(val - 1);
  //    }
  //  }
}

std::string ObjReader::ReadLine() {
  std::string line = "";
  while (Token() != '\n' && m_pos < m_dataLen) {
    line += Token();
    m_pos++;
  }
  m_pos++;
  return line;
}

void ObjReader::SkipLine() {
  while (Token() != '\n' && m_pos < m_dataLen) { m_pos++; }
  m_pos++;
}

void ObjReader::ReplaceChars(
    std::string *str, char toReplace, char replacement) {
  for (size_t i = 0; i < str->length(); i++) {
    if ((*str)[i] == toReplace) { (*str)[i] = replacement; }
  }
}

Mesh *ObjReader::CreateMeshFromFaces() {
  Mesh *mesh = new Mesh();

  std::sort(m_faces.begin(), m_faces.end(),
      [](const Face &a, const Face &b) { return a.index < b.index; });
  m_faces.erase(
      std::unique(m_faces.begin(), m_faces.end(),
          [](const Face &a, const Face &b) { return a.index == b.index; }),
      m_faces.end());
  for (const auto &face : m_faces) {
    mesh->normals.emplace_back(face.normal[0]);
    mesh->normals.emplace_back(face.normal[1]);
    mesh->normals.emplace_back(face.normal[2]);
    mesh->vertecies.emplace_back(face.vert[0]);
    mesh->vertecies.emplace_back(face.vert[1]);
    mesh->vertecies.emplace_back(face.vert[2]);
    mesh->textureCoords.emplace_back(face.textureCoord[0]);
    mesh->textureCoords.emplace_back(face.textureCoord[1]);
  }
  mesh->connections.assign(
      m_faceVertIndecies.begin(), m_faceVertIndecies.end());
  return mesh;
}
