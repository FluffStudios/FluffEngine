#include <core/ecs/event_manager.h>

namespace luminos { namespace ecs {

	IReceiver::~IReceiver()
	{
		for (auto conn : Connections_)
		{
			auto & ptr = conn.second.first;
			if (!ptr.expired())
			{
				ptr.lock()->disconnect(conn.second.second);
			}
		}
		Connections_.clear();
	}

	const size_t IReceiver::ConnectionSignalCount() const
	{
		size_t count = 0;
		for (auto c : Connections_)
		{
			if (!c.second.first.expired()) count++;
		}
		return count;
	}

	EventManager::EventManager()
	{ }

	EventManager::~EventManager()
	{ }

	std::shared_ptr<EventSignal>& EventManager::SignalFor(size_t SignalID)
	{
		if (SignalID >= Handlers_.size())
		{
			Handlers_.resize(SignalID + 1);
		}
		if (!Handlers_[SignalID])
		{
			Handlers_[SignalID] = std::make_shared<EventSignal>();
		}
		return Handlers_[SignalID];
	}

} }