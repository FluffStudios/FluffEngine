#pragma once

#include <common.h>
#include <ui/ui_widget.h>

#include <utility>
#include <vector>

namespace fluff { namespace ui {

    class UISurface 
    {
    protected:
        float X, Y;
        float Width, Height;
        bool Destructed;

        std::vector<std::pair<unsigned int, std::vector<UIWidget>>> Widgets;
    public:
        FLUFF_API UISurface();
        virtual FLUFF_API ~UISurface();
        virtual void FLUFF_API Construct();
        virtual void FLUFF_API Destruct();
        virtual void FLUFF_API Draw();

        float FLUFF_API GetX() const;
        float FLUFF_API GetY() const;
        float FLUFF_API GetWidth() const;
        float FLUFF_API GetHeight() const;
        
        void FLUFF_API SetX(const float X);
        void FLUFF_API SetY(const float Y);
        void FLUFF_API SetWidth(const float Width);
        void FLUFF_API SetHeight(const float Height);
    };

} }