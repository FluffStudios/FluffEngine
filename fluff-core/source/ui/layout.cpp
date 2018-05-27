#include <ui/layout.h>

namespace luminos { namespace ui {

	void RowComponent::Draw(nk_context * ctx)
	{
		if (LComponent)
		{
			LComponent->Draw(ctx);
		}
		else if (RComponent)
		{
			RComponent->Draw(ctx);
		}
	}

	void Row::Draw(nk_context * ctx)
	{
		switch (State)
		{
			case STATIC:
			{
				nk_layout_row_begin(ctx, NK_STATIC, ComponentHeight, Components.size());
				for (auto elem : Components)
				{
					nk_layout_row_push(ctx, elem.ElementWidth);
					elem.Draw(ctx);
				}
				nk_layout_row_end(ctx);
				break;
			}
			case DYNAMIC:
			{
				nk_layout_row_dynamic(ctx, ComponentHeight, Components.size());
				for (auto elem : Components)
				{
					elem.Draw(ctx);
				}
				break;
			}
		}
	}

	void Group::Draw(nk_context * ctx)
	{
		if (nk_group_begin(ctx, GroupName, Flags))
		{
			for (auto comp : Components)
			{
				comp->Draw(ctx);
			}
			nk_group_end(ctx);
		}
	}

	void Tree::Draw(nk_context * ctx)
	{
		if (nk_tree_push(ctx, NK_TREE_NODE, GroupName, NK_MAXIMIZED))
		{
			for (auto comp : Components) comp->Draw(ctx);
			nk_tree_pop(ctx);
		}
	}

	uint32_t Tree::IDTracker_ = 0;

} }