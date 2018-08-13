#include <core/debug/debug_log_writer.h>

#include <cereal/external/rapidjson/filewritestream.h>
#include <cereal/external/rapidjson/prettywriter.h>

#include <ctime>
#include <sstream>
#include <stdio.h>

#ifndef MIN_DEBUG_LVL
#define MIN_DEBUG_LVL fluff::debug::DebugSeverity::ERROR
#endif

namespace fluff { namespace debug {

	DebugLogWriter::DebugLogWriterImpl::DebugLogWriterImpl()
	{
		using namespace rapidjson;
		Doc_.SetObject();

		Value arr;
		arr.SetArray();
		Doc_.AddMember("errors", arr, Doc_.GetAllocator());
	}

	DebugLogWriter::DebugLogWriterImpl::~DebugLogWriterImpl()
	{
		FILE* fp = fopen(OutputFile.c_str(), "wb"); // non-Windows use "w"
		char writeBuffer[65536];
		
		using namespace rapidjson;
		
		FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
		PrettyWriter<FileWriteStream> writer(os);
		Doc_.Accept(writer);
		fclose(fp);
	}

	void DebugLogWriter::DebugLogWriterImpl::Log(DebugEvent Msg)
	{
		Log(Msg.ErrorType, Msg.Severity, Msg.LineNumber, Msg.FileName, Msg.Description);
	}

	void DebugLogWriter::DebugLogWriterImpl::Log(DebugErrorType ErrorType, DebugSeverity Severity, size_t LineNumber, std::string FileName, std::string Description)
	{
		if (Severity < MIN_DEBUG_LVL)
		{
			return;
		}
		std::lock_guard<std::mutex> lock(Mutex_);

		using namespace rapidjson;

		Value error_msg;
		error_msg.SetObject();

		std::time_t t = std::time(0);
		std::tm * now = std::localtime(&t);

		std::stringstream str;
		str << 1900 + now->tm_year << "/" << now->tm_mon << "/" << now->tm_mday << " ";
		str << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec;
		std::string * time = new std::string(str.str());

		Value timestamp;
		timestamp.SetString(time->c_str(), time->length(), Doc_.GetAllocator());
		error_msg.AddMember("timestamp", timestamp, Doc_.GetAllocator());

		Value error_type;
		error_type.SetUint(static_cast<uint32_t>(ErrorType));
		error_msg.AddMember("error_type", error_type, Doc_.GetAllocator());

		Value severity;
		severity.SetUint(static_cast<uint32_t>(Severity));
		error_msg.AddMember("severity", severity, Doc_.GetAllocator());

		Value line;
		line.SetUint(static_cast<uint32_t>(LineNumber));
		error_msg.AddMember("line", line, Doc_.GetAllocator());

		Value file;
		file.SetString(FileName.c_str(), FileName.length(), Doc_.GetAllocator());
		error_msg.AddMember("file", file, Doc_.GetAllocator());

		Value description;
		description.SetString(Description.c_str(), Description.length(), Doc_.GetAllocator());
		error_msg.AddMember("description", description, Doc_.GetAllocator());

		Doc_["errors"].PushBack(error_msg, Doc_.GetAllocator());

		delete time;
		Condition_.notify_one();
	}

	DebugLogWriter::DebugLogWriterImpl DebugLogWriter::Impl_;

	void DebugLogWriter::Log(DebugEvent Msg)
	{
		Impl_.Log(Msg);
	}

	void DebugLogWriter::Log(DebugErrorType ErrorType, DebugSeverity Severity, size_t LineNumber, std::string FileName, std::string Description)
	{
		Impl_.Log(ErrorType, Severity, LineNumber, FileName, Description);
	}

} }