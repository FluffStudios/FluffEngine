#pragma once
#include <core/ecs/system.h>

namespace fluff { namespace render { namespace ui {

    class UIRenderer : public ecs::System<UIRenderer>, public ecs::Receiver<UIRenderer>
    {

    };

} } }
