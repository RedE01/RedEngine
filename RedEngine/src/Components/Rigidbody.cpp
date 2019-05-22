#include "Rigidbody.h"
#include "Time.h"
#include "Physics/PhysicsManager.h"

void Rigidbody::start() {
	m_object = getParentGameObject();
	velocity = glm::vec3(0.0f);
}

void Rigidbody::update() {
	velocity.y -= Time::getDeltaTime() * PhysicsManager::gravity;

	m_object->move(velocity * float(Time::getDeltaTime()));
}