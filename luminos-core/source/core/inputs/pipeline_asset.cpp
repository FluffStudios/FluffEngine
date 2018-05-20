#include <core/assets/pipeline_asset.h>
#include <serialization_database.h>
#include <rendering/resource_library.h>

namespace luminos { 

	void PipelineAsset::LoadFromFile(AssetLibrary * pLibrary, const char * File)
	{
		using namespace luminos::utilities;

		Database db;
		SerializationObject pipeline;

		this->Pipeline_ = render::ResourceLibrary::GetGraphicsPipeline(File);
		pLibrary->Add(File, PIPELINE, this);
	}

	void PipelineAsset::SendToFile()
	{
		using namespace luminos::utilities;

		Database db;
		SerializationObject pipe;
		pipe.name = Pipeline_->GetName();
		pipe.AttachString({
			"name",
			Name_
		});

		pipe.AttachString({
			"type",
			"pipeline"
		});

		pipe.AttachString({ "descriptor", Pipeline_->GetName() });

		db.Attach(pipe);

		db.SendToFile(Name_.append(".asset").c_str());
	}

	gfx::GraphicsPipeline * PipelineAsset::Get()
	{
		return Pipeline_;
	}

}