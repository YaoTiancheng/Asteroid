#pragma once

#include "ObjectInstanceID.h"

namespace ASTEROID_NAMESPACE
{
    class Object;

    class ObjectManager
    {
    private:
        class InstanceIDManager
        {
        public:

            ObjectInstanceID GetAvailableInstanceId();
            void ReturnInstanceId(ObjectInstanceID id);
        };

    public:
        static ObjectManager* Singleton() { return _Singleton; }

        ObjectManager();
        ~ObjectManager();

        ASTEROID_NON_COPYABLE(ObjectManager)

        Object* FindObject(ObjectInstanceID instanceId) const;

        size_t ObjectCount() const { return m_Objects.size(); }

        ObjectInstanceID RegisterObject(Object* obj);
        void UnregisterObject(Object* obj);

    public:
        static ObjectManager* _Singleton;

    private:
        std::unordered_map<ObjectInstanceID, Object*> m_Objects;
        InstanceIDManager m_InstanceIdManager;
    };
}

