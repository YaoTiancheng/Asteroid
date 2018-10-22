#include "Precompile.h"
#include "Asteroid.h"
#include "ObjectManager.h"
#include "Util/Debug.h"
#include "Object.h"

namespace ASTEROID_NAMESPACE
{
    ObjectManager* ObjectManager::_Singleton = nullptr;

    ObjectManager::ObjectManager()
    {
        ASTEROID_ASSERT(_Singleton == nullptr, "There is already a ObjectManager singleton created.");
        _Singleton = this;
    }

    ObjectManager::~ObjectManager()
    { 
        _Singleton = nullptr;
    }

    Object* ObjectManager::FindObject(ObjectInstanceID instanceId) const
    {
        Object* obj = nullptr;
        auto itor = m_Objects.find(instanceId);
        if (itor != m_Objects.end())
            obj = itor->second;
        return obj;
    }

    ObjectInstanceID ObjectManager::RegisterObject(Object* obj)
    {
        ObjectInstanceID instanceId = m_InstanceIdManager.GetAvailableInstanceId();
        m_Objects.insert(std::make_pair(instanceId, obj));
        return instanceId;
    }

    void ObjectManager::UnregisterObject(Object* obj)
    {
        m_Objects.erase(obj->InstanceId());
        m_InstanceIdManager.ReturnInstanceId(obj->InstanceId());
    }

    ObjectInstanceID ObjectManager::InstanceIDManager::GetAvailableInstanceId()
    {
        return 0;
    }

    void ObjectManager::InstanceIDManager::ReturnInstanceId(ObjectInstanceID id)
    {
    }
}
