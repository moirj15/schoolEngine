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
    VertexArray vertexArray;
};

class ObjReader
{
    std::string m_filename;
    std::vector<char> m_data;

    size_t m_pos;

    enum class DataType {
        Vertex,
        TextureCoord,
        VertexNormal,
        ParameterSpaceVertex,
        PolygonFace,
        LineElement,
        Object,
        Group,
        Unknown
    };

public:
    ObjReader(std::string &filename);

    Mesh *Parse();


private:
    DataType ParseType();

    inline char Token() { return m_data[m_pos]; }

};

#endif // OBJ_H
