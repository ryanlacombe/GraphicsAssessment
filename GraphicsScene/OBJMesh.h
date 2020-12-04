#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <vector>
#include "Texture.h"

namespace aie
{

	//Simple Triangle Mesh Wrapper
	class OBJMesh
	{

	public:

		//Vertex structure for OBJ Files

		struct Vertex
		{
			glm::vec4 position;
			glm::vec4 normal;
			glm::vec2 texCoord;

			glm::vec4 tangent;
		};

		//Base Material
		class Material
		{

		public:

			Material() : ambient(1), diffuse(1), specular(0), emissive(0), specularPower(1), opacity(1) {}
			~Material() {}

			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			glm::vec3 emissive;

			float specularPower;
			float opacity;

			Texture diffuseTexture;
			Texture alphaTexture;
			Texture ambientTexture;
			Texture specularTexture;
			Texture specularHighlightTexture;
			Texture normalTexture;
			Texture displacementTexture;
		};

		OBJMesh() {}
		~OBJMesh();

		//Fails if Mesh has already been loaded
		bool load(const char* filename, bool loadTextures = true, bool flipTextureV = false);

		//Allows option to draw patches for Tessellation
		void draw(bool usePatches = false);

		//Access the Filename that was loaded
		const std::string& getFilename() const { return m_filename; }

		//Material Access
		size_t getMaterialCount() const { return m_materials.size(); }
		Material& getMaterial(size_t index) { return m_materials[index]; }

	private:

		void calculateTangents(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

		struct MeshChunk
		{
			unsigned int vao, vbo, ibo;
			unsigned int indexCount;
			int materialId;
		};

		std::string m_filename;
		std::vector<MeshChunk> m_meshChunks;
		std::vector<Material> m_materials;
	};

}