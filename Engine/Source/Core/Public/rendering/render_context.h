#pragma once

#include <rendering/render_resource.h>
#include <rendering/framebuffer_resource.h>

namespace fluff { namespace render {

    class RenderContext
    {
    public:
        FramebufferResource2D * CreateFramebufferResource2D() const;
    };

} }