#pragma once

#include <common.h>
#include <ui/ui_component.h>

#include <memory>
#include <vector>

namespace fluff { namespace ui {

    class UIWidget
    {
    protected:
        std::vector<std::unique_ptr<UIComponent>> Components;
        bool Destructed;

        float X, Y;
        float Width, Height;
        unsigned int Layer;
    public:
        virtual FLUFF_API ~UIWidget();
        virtual void FLUFF_API Construct() = 0;
        virtual void FLUFF_API Destruct() = 0;
        virtual void FLUFF_API Draw() = 0;

        float FLUFF_API GetX() const;
        float FLUFF_API GetY() const;
        float FLUFF_API GetWidth() const;
        float FLUFF_API GetHeight() const;
        
        void FLUFF_API SetX(const float X);
        void FLUFF_API SetY(const float Y);
        void FLUFF_API SetWidth(const float Width);
        void FLUFF_API SetHeight(const float Height);
    protected:
        virtual void RemoveComponents();
    };

} }