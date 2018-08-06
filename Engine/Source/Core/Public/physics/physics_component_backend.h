#pragma once

namespace fluff { namespace physics {

    enum PhysicsObjectType {
        STATIC,
        DYNAMIC
    };

    enum PhysicsColliderType {
        SPHERE,
        PLANE,
        CUBE,
        HEIGHTFIELD
    };

    struct PhysicsComponentBackend {
        PhysicsColliderType ColliderType;
        PhysicsObjectType ObjectType;
        size_t ID;
        void * Actor;
    };

}}