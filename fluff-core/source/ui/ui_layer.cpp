#include <ui/ui_layer.h>

namespace luminos { namespace ui {

		size_t UILayer::ID = 1;

		void UILayer::Draw(nk_context * ctx) const
		{
			for (auto wid : Widgets) wid->Draw(ctx);
		}

		void UILayer::Enable(std::shared_ptr<ECSManager> Manager) const
		{
			Manager->GetEventManager()->EmitEvent<UILayerEnabledEvent>(*this);
		}

} }