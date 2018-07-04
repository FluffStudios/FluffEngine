#pragma once

#include <core/ecs/event.h>
#include <non_copyable.h>
#include <common.h>
#include <core/ecs/signal.h>

#include <memory>
#include <functional>
#include <unordered_map>

namespace fluff { namespace ecs {

	typedef Simple::Signal<void(const void *)> EventSignal;

	class IReceiver
	{
		friend class EventManager;
		std::unordered_map<size_t, std::pair<std::weak_ptr<EventSignal>, size_t>> Connections_;
	public:
		/*
			Destructor
		*/
		virtual FLUFF_API ~IReceiver();

		/*
			Gets count of connected signals

			Returns number of connected signals
		*/
		const size_t FLUFF_API ConnectionSignalCount() const;
	};

	template <typename T>
	class Receiver : public IReceiver
	{
	public:
		/*
			Destructor
		*/
		~Receiver();
	};

	template<typename T>
	inline Receiver<T>::~Receiver()
	{ }

	class EventManager : NonCopyable
	{
	protected:
		std::vector<std::shared_ptr<EventSignal>> Handlers_;
		
		template<typename T>
		struct EventCallback
		{
			explicit EventCallback(std::function<void(const T &)> Callback)
				: CallbackFunction(Callback) { }
			void operator()(const void * event)
			{
				CallbackFunction(*static_cast<const T*>(event));
			}
			std::function<void(const T &)> CallbackFunction;
		};
	public:
		/*
			Creates new event manager
		*/
		FLUFF_API EventManager();
		
		/*
			Destructor
		*/
		virtual FLUFF_API ~EventManager();

		/*
			Subscribes receiver to event

			EventType - event to subscribe to
			R - Receiver to subscribe
		*/
		template<typename EventType, typename Receiver>
		void SubscribeToEvent(Receiver & R)
		{
			void (Receiver::*Receive)(const EventType &) = &Receiver::Receive;
			auto signal = SignalFor(Event<EventType>::EventFamilyID());
			EventCallback<EventType> wrapper(std::bind(Receive, &R, std::placeholders::_1));
			auto conn = signal->connect(wrapper);
			IReceiver & receiver = R;
			receiver.Connections_.insert(std::pair<size_t, std::pair<std::weak_ptr<EventSignal>, size_t>>(Event<EventType>::EventFamilyID(), std::make_pair(std::weak_ptr<EventSignal>(signal), conn)));
		}

		/*
			Unsubscribes receiver to event

			EventType - event to unsubscribe to
			R - receiver to unsubscribe
		*/
		template<typename EventType, typename Receiver>
		void UnsubscribeFromEvent(Receiver & R)
		{
			IReceiver & receiver = R;
			FLUFF_ASSERT(receiver.Connections_.find(Event<EventType>::EventFamilyID()) != receiver.Connections_.end())
			auto pair = receiver.Connections_[Event<EventType>::EventFamilyID()];
			auto conn = pair.second;
			auto &ptr = pair.first;

			if (!ptr.expired())
			{
				ptr.lock()->disconnect(conn);
			}
			receiver.Connections_.erase(Event<EventType>::EventFamilyID());
		}

		/*
			Emits an event
			
			EventType - type of event
			E - pointer to event
		*/
		template<typename EventType>
		void EmitEvent(std::unique_ptr<EventType> E)
		{
			auto sig = SignalFor(Event<EventType>::EventFamilyID());
			sig->emit(E.get());
		}

		/*
			Emits an event
			
			EventType - type of event
			Arguments - type of parameters
			Args - Arguments for event construction
		*/
		template<typename EventType, typename ... Arguments>
		void EmitEvent(Arguments ... Args)
		{
			EventType e = EventType(std::forward<Arguments>(Args) ...);
			auto signal = SignalFor(static_cast<size_t>(Event<EventType>::EventFamilyID()));
			signal->emit(&e);
		}

		/*
			Calculates the number of connected receivers

			Returns connected receiver count
		*/
		const size_t FLUFF_API ConnectedReceiverCount() const
		{
			size_t Count = 0;
			for (auto handler : Handlers_)
			{
				if (handler) Count += handler->size();
			}
			return Count;
		}
	protected:
		std::shared_ptr<EventSignal> FLUFF_API & SignalFor(size_t SignalID);
	};

} }