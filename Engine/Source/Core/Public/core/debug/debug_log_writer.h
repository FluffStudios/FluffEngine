#pragma once

#include <condition_variable>
#include <mutex>
#include <cereal/external/rapidjson/document.h>
#include <cereal/external/rapidjson/rapidjson.h>

#include <common.h>
#include <core/debug/debug_event.h>

#ifndef LOG_OUTPUT_FILENAME
#define LOG_OUTPUT_FILENAME "logging.json"
#endif

namespace fluff { namespace debug {

	class DebugLogWriter
	{
		class DebugLogWriterImpl
		{
			std::mutex Mutex_;
			std::condition_variable Condition_;
			rapidjson::Document Doc_;
		public:
			DebugLogWriterImpl();
			~DebugLogWriterImpl();
			std::string OutputFile = LOG_OUTPUT_FILENAME;
			void FLUFF_API Log(DebugEvent Msg);
			void FLUFF_API Log(DebugErrorType ErrorType, DebugSeverity Severity, size_t LineNumber, std::string FileName, std::string Description);
		};

		static DebugLogWriterImpl Impl_;
	public:
		static void Log(DebugEvent Msg);
		static void Log(DebugErrorType ErrorType, DebugSeverity Severity, size_t LineNumber, std::string FileName, std::string Description);
	};

} }

#define FLUFF_LOG(ErrorType, Severity, Description) \
	fluff::debug::DebugLogWriter::Log(ErrorType, Severity, __LINE__, __FILE__, Description)