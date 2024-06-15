#include "../debug.h"
#include "entity.h"


bool Entity::ProcessCollide(sf::Vector3f newCoord)
{
    if (scene->GetCell(sf::Vector2i(ToVector2<float>(newCoord))) == nullptr)
        return true;
    else
        return false;
}


Entity::Entity(std::shared_ptr<Scene> scene)
{
	this->scene = scene;
}

sf::Vector3f Entity::GetCoord()
{
    return this->coord;
}

void Entity::MoveTo(sf::Vector3f coord)
{
    this->coord = coord;
}

void Entity::MoveRelative(sf::Vector3f vec) // стоит убрать в будущем отсюда проверку на коллизию и выделить такое движение в третьюи функцию
{
    Transform entityTransform;
    entityTransform.Translate(this->coord);
    entityTransform.Rotate(-this->rotation);

    /*sf::Vector2f forward = sf::Vector2f(std::cos(cameraAngle), std::sinf(cameraAngle));
    sf::Vector2f right = forward*/
    sf::Vector3f transformedVec = entityTransform.TransformPoint(vec);
    transformedVec.z = this->coord.z + vec.z;
    sf::Vector3f newCoord = transformedVec;
    // if (ProcessCollide(newCoord))
    this->coord = newCoord;
}

void Entity::RotateTo(sf::Vector3f rotation)
{
    rotation.x = CycleAngle(rotation.x);
    rotation.y = CycleAngle(rotation.y);
    this->rotation = rotation;
}

void Entity::RotateRelative(sf::Vector3f rotation)
{
    rotation.x = CycleAngle(this->rotation.x + rotation.x);
    //rotation.y = CycleAngle(this->rotation.y + rotation.y);
    this->rotation = rotation;
    Debug::SetMetric("rotation", std::format("{} {}", rotation.x, rotation.y));
}

sf::Vector3f Entity::GetRotation()
{
    return this->rotation;
}
