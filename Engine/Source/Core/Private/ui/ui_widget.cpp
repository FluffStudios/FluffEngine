#include <ui/ui_widget.h>

#include <core/debug/debug_log_writer.h>

namespace fluff { namespace ui { 

    UIWidget::~UIWidget() 
    {    
        if (!Destructed) 
        {
            FLUFF_LOG(debug::DebugErrorType::ILLEGAL_STATE, debug::DebugSeverity::WARN, "UI Widget not destructed.  Set destructed flag to true once destructed.");
        }

        if (Components.size() != 0)
        {
            FLUFF_LOG(debug::DebugErrorType::ILLEGAL_STATE, debug::DebugSeverity::WARN, "UI Widget components not removed.  Call fluff::ui::UIWidget::RemoveComponents to default remove components from widget.");
        }
    }

    float UIWidget::GetX() const
    {
        return X;
    }

    float UIWidget::GetY() const
    {
        return Y;
    }

    float UIWidget::GetWidth() const
    {
        return Width;
    }

    float UIWidget::GetHeight() const
    {
        return Height;
    }

    void UIWidget::SetX(const float X) 
    {
        this->X = X;
    }

    void UIWidget::SetY(const float Y) 
    {
        this->Y = Y;
    }

    void UIWidget::SetWidth(const float Width)
    {
        this->Width = Width;
    }

    void UIWidget::SetHeight(const float Height) 
    {
        this->Height = Height;
    }

    void UIWidget::RemoveComponents() 
    {
        for (auto & comp : Components) 
        {
            comp->Destruct();
        }

        Components.clear();
    }

} }