#include <ui/ui_surface.h>

namespace fluff { namespace ui {

    UISurface::UISurface() 
        : X(0), Y(0), Width(0), Height(0)
    {

    }

    UISurface::~UISurface()
    {

    }

    void UISurface::Construct()
    {

    }

    void UISurface::Destruct()
    {

    }

    void UISurface::Draw()
    {

    }

    float UISurface::GetX() const
    {
        return X;
    }

    float UISurface::GetY() const
    {
        return Y;
    }

    float UISurface::GetWidth() const
    {
        return Width;
    }

    float UISurface::GetHeight() const
    {
        return Height;
    }

    void UISurface::SetX(const float X)
    {
        this->X = X;    
    }

    void UISurface::SetY(const float Y)
    {
        this->Y = Y;
    }

    void UISurface::SetWidth(const float Width)
    {
        this->Width = Width;
    }

    void UISurface::SetHeight(const float Height)
    {
        this->Height = Height;
    }

} }