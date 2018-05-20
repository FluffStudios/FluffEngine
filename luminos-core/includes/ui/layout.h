#pragma once

#include <ui/iui_component.h>
#include <ui/nk_ui.h>
#include <vector>

namespace luminos { namespace ui {

	enum Alignment
	{
		ALIGN_LEFT,
		ALIGN_RIGHT,
		ALIGN_CENTER
	};

	enum GroupFlags
	{
		GROUP_BORDER = 1,
		GROUP_MOVABLE = 2,
		GROUP_SCALABLE = 4,
		GROUP_CLOSABLE = 8,
		GROUP_MINIMIZABLE = 16,
		GROUP_NO_SCROLLBAR = 32,
		GROUP_TITLE = 64,
		GROUP_SCROLL_AUTO_HIDE = 128,
		GROUP_BACKGROUND = 256,
		GROUP_SCALE_LEFT = 512,
		GROUP_NO_INPUT = 1024
	};

	enum RowState
	{
		STATIC = 1,
		DYNAMIC = 0
	};

	struct LayoutComponent
	{
		uint32_t Flags = 0;
		uint32_t ComponentHeight = 30;

		/*
			Draws layout component

			ctx - Layout component
		*/
		virtual void Draw(nk_context * ctx) = 0;
	};

	struct RowComponent : public LayoutComponent
	{
		LayoutComponent *	LComponent = nullptr;
		IUIComponent	*	RComponent = nullptr;
		float32_t			ElementWidth = 0.0f;

		/*
			Draws layout component

			ctx - Layout component
		*/
		void Draw(nk_context * ctx) override;
	};

	class Row : public LayoutComponent
	{
	public:
		/*
			Creates empty row
		*/
		Row() { }
		std::vector<RowComponent> Components;
		RowState State = DYNAMIC;

		/*
			Draws layout component

			ctx - Layout component
		*/
		void Draw(nk_context * ctx) override;
	};

	class Group : public LayoutComponent
	{
	public:
		/*
			Creates empty group
		*/
		Group() { }
		std::vector<LayoutComponent *> Components;
		const char * GroupName;

		/*
			Draws layout component

			ctx - Layout component
		*/
		void Draw(nk_context * ctx) override;
	};

	struct Tree : public LayoutComponent
	{
		uint32_t ID_ = 0;
		static uint32_t IDTracker_;
	public:
		/*
			Creates empty tree
		*/
		Tree() { ID_ = IDTracker_++; }
		std::vector<LayoutComponent *> Components;
		const char * GroupName;

		/*
			Draws layout component

			ctx - Layout component
		*/
		void Draw(nk_context * ctx) override;
	};

} }