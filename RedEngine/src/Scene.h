#pragma once
#include "GameObject.h"
#include "Renderer.h"
#include "Components/Collider.h"
#include "Components/Rigidbody.h"
#include "Debug.h"

class Scene {
public:
	std::vector<Collider*> m_colliders;
    std::vector<Light*> m_lights;
    std::vector<ObjectRenderer*> m_objectRenderers;
    std::vector<Rigidbody*> m_rigidbodies;
private:
    std::vector<GameObject*> m_gameObjects;
    Camera* m_camera = nullptr;
public:
    Scene();
    ~Scene();

    void updateObjects();
    void renderScene(Renderer* renderer);

    GameObject* createGameObject(glm::vec3 position);
    GameObject* createGameObject(glm::vec3 position, glm::vec3 rotation);
    GameObject* createGameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    GameObject* createGameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Model* model);

    Light* createLight(glm::vec3 pos, float strength);
	Camera* createCamera(glm::vec3 pos, glm::vec3 rot, CameraType cameraType, glm::vec2 windowSize);

	inline const std::vector<GameObject*>* getGameObjectList() const { return &m_gameObjects; }
	inline const std::vector<ObjectRenderer*>* getObjectRendererList() const { return &m_objectRenderers; }
	inline const std::vector<Light*>* getLightList() const { return &m_lights; }
    inline Camera* const getCamera() const { 
        if(m_camera == nullptr) {
            gameEngineDebug("Error in Scene.h: getCamera(): Camera does not exist");
        }
        return m_camera; 
    }
};