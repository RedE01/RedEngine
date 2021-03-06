#include "Model.h"
#include <iostream>

Model::Model(const char* modelPath, Texture* texture) : m_modelData(), m_texture(texture), m_specular(nullptr) {
	init(modelPath);
}

Model::Model(const char * modelPath, Texture * texture, Texture * specular) : m_modelData(), m_texture(texture), m_specular(specular) {
	init(modelPath);
}

Model::~Model() {
	delete m_vbo;
	delete m_ibo;
	delete m_vao;
}

void Model::bind() {
	m_vao->bind();
}

void Model::init(const char* modelPath) {
	objReader::parseObjFile(modelPath, &m_modelData);
	m_indicies = m_modelData.indexData.size();

	if (m_indicies != 0) {
		m_vao = new VertexArrayObject();
		m_vbo = new VertexBuffer(&m_modelData.vertexData[0].positions[0], sizeof(objReader::vertex) * m_modelData.vertexData.size());
		m_vbo->addVertexAttribute(0, 3, sizeof(float) * 8, 0);
		m_vbo->addVertexAttribute(1, 3, sizeof(float) * 8, 3 * sizeof(float));
		m_vbo->addVertexAttribute(2, 2, sizeof(float) * 8, 6 * sizeof(float));
		m_ibo = new IndexBuffer(&m_modelData.indexData[0], sizeof(unsigned int) * m_modelData.indexData.size());
	}
	else {
		gameEngineDebug("COULD NOT CREATE MODEL: " + (std::string)modelPath);
	}
}
