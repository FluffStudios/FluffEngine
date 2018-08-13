#pragma once

#include <common.h>
#include <rendering/render_resource.h>

#define MAX_TEXTURE_ATTACHMENTS

namespace fluff { namespace render {

    struct FramebufferAttachment2DDesc
    {
        uint32_t Width;
        uint32_t Height;
    };

    class FramebufferResource2D : public IRenderResource
    {
        uint32_t mFramebufferID;
        uint32_t mAttachedTextures[MAX_TEXTURE_ATTACHMENTS];
        uint32_t mAttachedTextureCount;
    public:
        void Construct() override;
        void Destruct() override;

        void AttachTexture(FramebufferAttachment2D & Desc);
    };

    struct FramebufferBufferAttachment3DDesc
    {
        uint32_t Width;
        uint32_t Height;
        uint32_t Depth;
    };

    class FramebufferResource3D : public IRenderResource
    {
        uint32_t mFramebufferID;
        uint32_t mAttachedTextures[MAX_TEXTURE_ATTACHMENTS];
        uint32_t mAttachedTextureCount;
    public:
        void Construct() override;
        void Destruct() override;

        void AttachTexture(FramebufferAttachment2DDesc & Desc);
    };

} }