#include <ui/ui_component.h>

#include <core/debug/debug_log_writer.h>

namespace fluff { namespace ui {

    UIComponent::~UIComponent() 
    {    
        if (!Destructed) {
            FLUFF_LOG(debug::DebugErrorType::ILLEGAL_STATE, debug::DebugSeverity::WARN, "UI Component not destructed.  Set destructed flag to true once destructed.");
        }
    }

}}