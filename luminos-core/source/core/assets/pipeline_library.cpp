#include <core/assets/pipeline_library.h>
#include <set>

namespace luminos {

	std::vector<std::string> PipelineLibrary::FileNames_;
	std::unordered_map<std::string, gfx::GraphicsPipeline *> PipelineLibrary::GraphicsPipelineMap_;

	void PipelineLibrary::LoadNoAdd(std::string FileName)
	{
		if (FileName == "") return;
		gfx::GraphicsPipeline * pipeline = new gfx::GraphicsPipeline;
		ReadFromFile(FileName, *pipeline);
		if (GraphicsPipelineMap_.find(pipeline->GetName()) == GraphicsPipelineMap_.end())
		{
			GraphicsPipelineMap_.insert(std::make_pair(pipeline->GetName(), pipeline));
		}
		else
		{
			auto pos = std::find(FileNames_.begin(), FileNames_.end(), FileName);
			if (pos != FileNames_.end()) FileNames_.erase(pos);
			delete pipeline;
		}
	}

	gfx::GraphicsPipeline * PipelineLibrary::Load(std::string FileName)
	{
		gfx::GraphicsPipeline * pipeline = new gfx::GraphicsPipeline;
		ReadFromFile(FileName, *pipeline);
		if (GraphicsPipelineMap_.find(pipeline->GetName()) == GraphicsPipelineMap_.end())
		{
			
			GraphicsPipelineMap_.insert(std::make_pair(pipeline->GetName(), pipeline));
			FileNames_.push_back(FileName);
		}
		else
		{
			delete pipeline;
		}
		return pipeline;
	}

	gfx::GraphicsPipeline * PipelineLibrary::Get(std::string TextureName)
	{
		auto it = GraphicsPipelineMap_.find(TextureName);
		if (it == GraphicsPipelineMap_.end())
		{
			LUMINOS_ERROR_FUNC("PIPELINE COULD NOT BE FOUND");
			return nullptr;
		}
		return it->second;
	}

	void PipelineLibrary::Remove(std::string TextureName)
	{
		delete GraphicsPipelineMap_[TextureName];
		GraphicsPipelineMap_.erase(TextureName);
	}

	void PipelineLibrary::Clear()
	{
		for (auto i : GraphicsPipelineMap_) delete i.second;
		GraphicsPipelineMap_.clear();
	}

	void PipelineLibrary::SaveLibrary(std::string FileName)
	{
		PipelineLibrary lib;
		SendToFile(FileName, lib);
	}

	void PipelineLibrary::LoadLibrary(std::string FileName)
	{
		PipelineLibrary lib;
		ReadFromFile(FileName, lib);
	}

	void PipelineLibrary::Create(std::shared_ptr<ECSManager> & pManager)
	{
		for (auto it : GraphicsPipelineMap_) it.second->Create(pManager);
	}

}