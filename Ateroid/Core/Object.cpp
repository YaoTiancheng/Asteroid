#include "Precompile.h"
#include "Asteroid.h"
#include "Object.h"
#include "ObjectManager.h"

namespace ASTEROID_NAMESPACE
{
    Object::Object()
    {
        m_InstanceId = ObjectManager::Singleton()->RegisterObject(this);
    }

    Object::~Object()
    {
        ObjectManager::Singleton()->UnregisterObject(this);
    }
}
