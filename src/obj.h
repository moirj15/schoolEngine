#ifndef OBJ_H
#define OBJ_H

#include "VertexBuffer.h"
#include "common.h"

#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

struct Mesh {
  std::vector<f32> vertecies = {};
  std::vector<u32> connections = {};
  std::vector<f32> normals = {};
  std::vector<f32> textureCoords = {};
  u32 vertexSize = 3;

  Mesh() = default;
  Mesh(Mesh &&m) :
      vertecies(std::move(m.vertecies)), connections(std::move(m.connections)),
      normals(std::move(m.normals)), vertexSize(m.vertexSize) {}
};

class ObjReader {
  std::string m_filename;
  std::unique_ptr<char[]> m_data;
  std::vector<glm::vec3> m_vertecies;
  std::vector<glm::vec3> m_normals;
  std::vector<glm::vec2> m_textureCoords;
  std::vector<s32> m_faceVertIndecies;

  struct Face {
    s32 index;
    glm::vec3 vert;
    glm::vec3 normal;
    glm::vec2 textureCoord;
  };

  std::vector<Face> m_faces;

  size_t m_dataLen;

  size_t m_pos;

  Mesh m_mesh;

  enum class DataType {
    Vertex,
    TextureCoord,
    VertexNormal,
    ParameterSpaceVertex,
    PolygonFace,
    LineElement,
    Object,
    Group,
    Comment,
    Unknown
  };

public:
  ObjReader();

  Mesh *Parse(const std::string &filename);

  void Clear();

private:
  void ReadFile(const std::string &filename);

  DataType ParseType();

  inline char Token() { return m_data[m_pos]; }

  void ParseVertex();

  void ParseTextureCoord();

  void ParseNormal();

  void ParseFace();

  std::string ReadLine();

  void SkipLine();

  void ReplaceChars(std::string *str, char toReplace, char replacement);

  Mesh *CreateMeshFromFaces();
};

#endif // OBJ_H
