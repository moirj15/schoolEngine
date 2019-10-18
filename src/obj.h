#ifndef OBJ_H
#define OBJ_H

#include "VertexBuffer.h"
#include "common.h"

#include <memory>
#include <string>
#include <vector>

struct Mesh {
  std::vector<f32> vertecies = {};
  std::vector<u32> connections = {};
  std::vector<f32> normals = {};
  u32 vertexSize = 3;

  Mesh() = default;
  Mesh(Mesh &&m) :
      vertecies(std::move(m.vertecies)), connections(std::move(m.connections)),
      normals(std::move(m.normals)), vertexSize(m.vertexSize) {}
};

class ObjReader {
  std::string m_filename;
  // std::vector<char> m_data;
  std::unique_ptr<char[]> m_data;
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
  ObjReader(char *filename);

  Mesh *Parse();

  void Clear();

private:
  DataType ParseType();

  inline char Token() { return m_data[m_pos]; }

  void ParseVertex();

  void ParseNormal();

  void ParseFace();

  std::string ReadLine();

  void SkipLine();

  void ReplaceChars(std::string *str, char toReplace, char replacement);
};

#endif // OBJ_H
