#include "Renderer.h"

Renderer::Renderer(Camera * camera, GameObjectManager * gameObjectManager, Shader * shader) : m_camera(camera), m_gameObjectManager(gameObjectManager), m_shader(shader) {
}

Renderer::~Renderer() {
}

void Renderer::renderFrame() {
	updateLightPositions();
	m_shader->setUniformMat4f(m_shader->getProjectionUniformLocation(), &(*m_camera->getProjectionMat())[0][0]);
	m_shader->setUniformMat4f(m_shader->getViewUniformLocation(), &(*m_camera->getViewMat())[0][0]);

	for (ObjectRenderer* objRend : *m_gameObjectManager->getObjectRendererList()) {
		renderGameObject(objRend);
	}
}

void Renderer::renderGameObject(ObjectRenderer* objectRenderer) {
	objectRenderer->getModel()->bind();
	objectRenderer->getModel()->getTexture()->bind(0);
	m_shader->setUniformMat4f(m_shader->getModelUniformLocation(), &(*objectRenderer->getParentGameObject()->getModelMat())[0][0]);

	glDebug(glDrawElements(GL_TRIANGLES, objectRenderer->getModel()->getIndexCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::updateLightPositions() {
	std::vector<glm::vec3> tempList;
	auto lList = m_gameObjectManager->getLightList();
	for (int i = 0; i < lList->size(); i++) {
		glm::vec3* test = (*lList)[0]->getPosition();
		tempList.push_back(*test);
	}
	if (tempList.size() == 0) tempList.push_back(glm::vec3(0.0f));

	m_shader->setUniform1i(m_shader->getLightCountUniformLocation(), lList->size());
	m_shader->setUniformVec3f(m_shader->getLightPosUniformLocation(), &((tempList[0])[0]));
}
