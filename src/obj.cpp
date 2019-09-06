#include "obj.h"

#include <cstdio>

#include "VertexBuffer.h"

ObjReader::ObjReader(std::string &filename) :
        m_filename(std::move(filename)) {
    FILE *fp = nullptr;
//    char *data = nullptr;
    size_t len = 0;

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    rewind(fp);

    if (len == 0) {
        fprintf(stderr, "failed to get file size");
    }

    m_filename.reserve(len + 1);
//    data = new char[len + 1];
//    len = fread(data, sizeof(char), len, fp);
    len = fread(m_filename.data(), sizeof(char), len, fp);
//    m_data.reset(data);

    fclose(fp);
}

Mesh *ObjReader::Parse() {
    Mesh *mesh = new Mesh();
    while (m_pos < m_data.size()) {
        auto dataType = ParseType();
        switch (dataType) {
        case DataType::Vertex:
            break;
        case DataType::TextureCoord:
            break;
        case DataType::VertexNormal:
            break;
        case DataType::ParameterSpaceVertex:
            break;
        case DataType::PolygonFace:
            break;
        case DataType::LineElement:
            break;
        case DataType::Object:
            break;
        case DataType::Group:
            break;
        case DataType::Unknown:
            break;
        }
    }
    return mesh;
}


// Private
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
    }
    fprintf(stderr, "Unrecognized OBJ datatype\n");
    return DataType::Unknown;
}
