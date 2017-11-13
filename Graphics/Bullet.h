#pragma once

#include <glm\glm.hpp>
#include <Bengine\SpriteBatch.h>

class Bullet
{
public:
	Bullet();
	~Bullet();

	void init(glm::vec2 pos, glm::vec2 dir, float speed);
	void draw(Bengine::SpriteBatch& spriteBatch);

private:
	float _speed;
	glm::vec2 _direction;
	glm::vec2 _position;
};

