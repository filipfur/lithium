#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust trinagulation. Requires C++11
//#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tinyobjloader.h"
#include "tiny_obj_loader.h"

#include <iostream>
#include <vector>

struct MeshVertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 tex;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

// Computes tangents using Lengyel's method for an indexed triangle list.
// Tangents are computed as a 4d vector where w stores the sign need to reconstruct a bitangent in the shader.
void compute_tangents_lengyel(MeshVertex* pVertices, GLuint kVertices, const GLuint* pIndices, GLuint kIndices)
{
	const GLuint kTris = kIndices / 3;

	// Tangents are accumulated so we need some space to work in.
	glm::vec3* buffer = new glm::vec3[kVertices * 2];
	memset(buffer, 0, sizeof(glm::vec3) * kVertices * 2);
	
	// offsets into the buffer;
	glm::vec3* tan1 = buffer;
	glm::vec3* tan2 = buffer + kVertices;

	// Step through each triangle.
	for (GLuint iTri = 0; iTri < kTris; ++iTri)
	{
		GLuint i1 = pIndices[0];
		GLuint i2 = pIndices[1];
		GLuint i3 = pIndices[2];

		glm::vec3 p1 = pVertices[i1].pos;
		glm::vec3 p2 = pVertices[i2].pos;
		glm::vec3 p3 = pVertices[i3].pos;

		glm::vec2 w1 = pVertices[i1].tex;
		glm::vec2 w2 = pVertices[i2].tex;
		glm::vec2 w3 = pVertices[i3].tex;

		GLfloat x1 = p2.x - p1.x;
		GLfloat x2 = p3.x - p1.x;
		GLfloat y1 = p2.y - p1.y;
		GLfloat y2 = p3.y - p1.y;
		GLfloat z1 = p2.z - p1.z;
		GLfloat z2 = p3.z - p1.z;

		GLfloat s1 = w2.x - w1.x;
		GLfloat s2 = w3.x - w1.x;
		GLfloat t1 = w2.y - w1.y;
		GLfloat t2 = w3.y - w1.y;

		GLfloat r = 1.f / (s1 * t2 - s2 * t1);
		glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
		glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

		// accumulate the tangents
		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;

		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;

		pIndices += 3;
	}

	// Step through each vertex.
	for (GLuint i = 0; i < kVertices; ++i)
	{
		glm::vec3 n = pVertices[i].normal;
		glm::vec3 t1 = tan1[i];
		glm::vec3 t2 = tan2[i];
		
		// Gram-Schmidt Orthogonalization
		glm::vec3 tangent = glm::normalize(t1 - n * glm::dot(n, t1));
		glm::vec3 bitangent = glm::cross(n, t1);

		//XMStoreFloat4(pVertices[i].tangent, tangent);
        pVertices[i].tangent = tangent;
		//pVertices[i].tangent.w = bitangent.x < 0.f ? -1.0f : 1.0f; // sign
        pVertices[i].bitangent = bitangent;
	}

	// cleanup the temp buffer
	delete[] buffer;
}

mygl::Mesh* tinyobjloader_load(const char* inputfile, glm::vec2 uvScale, bool print)
{
    //std::string inputfile = "cornell_box.obj";
    tinyobj::ObjReaderConfig reader_config;
    std::string dir = inputfile;
    reader_config.mtl_search_path = dir.substr(0, dir.find_last_of("/")); // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    std::vector<MeshVertex> meshVertices;
    std::vector<GLuint> indices;

    bool first = true;

    for(const auto& shape : shapes)
    {
        for(const auto& idx : shape.mesh.indices)
        {
            tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
            tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
            tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

            tinyobj::real_t nx = 0.0f;
            tinyobj::real_t ny = 0.0f;
            tinyobj::real_t nz = 0.0f;
            // Check if `normal_index` is zero or positive. negative = no normal data
            if (idx.normal_index >= 0) {
                nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
            }

            tinyobj::real_t tx = 0.0f;
            tinyobj::real_t ty = 0.0f;
            // Check if `texcoord_index` is zero or positive. negative = no texcoord data
            if (idx.texcoord_index >= 0) {
                tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0] * uvScale.x;
                ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1] * uvScale.y;
            }

            //vertices.insert(vertices.end(), {vx, vy, vz, nx, ny, nz, tx, ty});
            meshVertices.push_back(MeshVertex{glm::vec3{vx, vy, vz}, glm::vec3{nx, ny, nz}, glm::vec2{tx, ty}, glm::vec3{0.0f}, glm::vec3{0.0f}});
            
            indices.push_back(indices.size());
        }
    }

    compute_tangents_lengyel(&meshVertices[0], meshVertices.size(), &indices[0], indices.size());

    std::vector<GLfloat> vertices;
    vertices.reserve(meshVertices.size() * sizeof(MeshVertex));
    for(MeshVertex meshVertex : meshVertices)
    {
        vertices.insert(vertices.end(), {meshVertex.pos.x, meshVertex.pos.y, meshVertex.pos.z, meshVertex.normal.x, meshVertex.normal.y, meshVertex.normal.z,
            meshVertex.tex.x, meshVertex.tex.y, meshVertex.tangent.x, meshVertex.tangent.y, meshVertex.tangent.z,
            meshVertex.bitangent.x, meshVertex.bitangent.y, meshVertex.bitangent.z});
            if(print)
            {
                std::cout << meshVertex.normal.x << " " << meshVertex.normal.y << " " << meshVertex.normal.z << std::endl;
            }
    }

    mygl::Mesh* mesh = new mygl::Mesh(vertices, indices, mygl::Mesh::State::POS_NORMAL_UV_TANGENTS);
    return mesh;
}