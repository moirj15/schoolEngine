#ifndef OBJ_H
#define OBJ_H

#include <vector>
#include <string>
#include <memory>

#include "common.h"

#include "VertexBuffer.h"

struct Mesh {
    std::vector<f32> vertecies;
    std::vector<u32> connections;
    u32 vertexSize;

    Mesh() = default;
    Mesh(Mesh &&m) :
        vertecies(std::move(m.vertecies)),
        connections(std::move(m.connections)),
        vertexSize(m.vertexSize) {}
};

class ObjReader
{
    std::string m_filename;
    std::vector<char> m_data;

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
    ObjReader(std::string &filename);

    Mesh *Parse();


private:
    DataType ParseType();

    inline char Token() { return m_data[m_pos]; }

    void ParseVertex();

    void ParseFace();

    std::string ReadLine();

    void SkipLine();

    void ReplaceChars(std::string *str, char toReplace, char replacement);

};

#endif // OBJ_H
