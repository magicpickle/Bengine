#include "Bullet.h"

#include <Bengine\ResourceManager.h>
#include <Bengine\GLTexture.h>


Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}

void Bullet::init(glm::vec2 pos, glm::vec2 dir, float speed)
{
	_position = pos;
	_direction = dir;
	_speed = speed;
}
void Bullet::draw(Bengine::SpriteBatch& spriteBatch)
{
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	Bengine::Color color;
	color.r = 255;
	color.b = 255;
	color.g = 255;
	color.a = 255;
	
	glm::vec4 posAndSize = glm::vec4(_position.x, _position.y, 30, 30);

	spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);
}