///////////////////////////////////////////
// Include this in order to get the core //
// framework of the engine. Does not     //
// include any rendering or other		 //
// components to the Luminos Engine      //
///////////////////////////////////////////

#pragma once

#include <core/debug_event.h>
#include <core/debug_message.h>
#include <core/debug_system.h>
#include <core/timer.h>
#include <core/transformation.h>
#include <core/transformation_component.h>
#include <core/window.h>

#include <core/inputs/cursor.h>
#include <core/inputs/keyboard.h>
#include <core/inputs/mouse.h>

#include <core/ecs/dependency.h>
#include <core/ecs/entity.h>
#include <core/ecs/event.h>
#include <core/ecs/event_manager.h>
#include <core/ecs/ievent.h>
#include <core/ecs/isystem.h>
#include <core/ecs/pool.h>
#include <core/ecs/signal.h>
#include <core/ecs/system.h>
#include <core/ecs/ecs_manager.h>