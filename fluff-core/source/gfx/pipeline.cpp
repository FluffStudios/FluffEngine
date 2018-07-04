#include <gfx/pipeline.h>

#include <glew.h>
#include <gfx/context.h>

namespace fluff { namespace gfx
{

	GraphicsPipeline::GraphicsPipeline()
		: RenderTarget_(nullptr), ShaderHandle_(nullptr)
	{
		
	}

	void GraphicsPipeline::Enable() const
	{
		// enable blend state
		if (Descriptor_.BlendState.ShouldBlend)
		{
			if (Descriptor_.BlendState.AlphaToCoverage) 
			{
				glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
			}
			else
			{
				glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
			}

			// enable blending
			if (Descriptor_.BlendState.IndependentBlendTarget)
			{
				glEnable(GL_BLEND);
				glBlendFuncSeparate(Descriptor_.BlendState.RenderTarget[0].SourceAlphaFunction, Descriptor_.BlendState.RenderTarget[0].DestinationAlphaFunction, Descriptor_.BlendState.RenderTarget[0].SourceFunction, Descriptor_.BlendState.RenderTarget[0].DestinationFunction);
			}
			else
			{
				// set blend for each attachment
				int counter = 0;
				for (uint32_t * texture : RenderTarget_->GetTextureIDs());
				{
					glEnablei(GL_BLEND, *RenderTarget_->GetTextureIDs()[counter]);
					glBlendFuncSeparatei(*RenderTarget_->GetTextureIDs()[counter], Descriptor_.BlendState.RenderTarget[counter].SourceFunction, Descriptor_.BlendState.RenderTarget[counter].DestinationFunction, Descriptor_.BlendState.RenderTarget[counter].SourceAlphaFunction, Descriptor_.BlendState.RenderTarget[counter].DestinationAlphaFunction);
					counter++;
				}
			}
		}
		else
		{
			glDisable(GL_BLEND);
		}

		// enable sample mask
		if (Descriptor_.SampleMask)
		{
			glEnable(GL_SAMPLE_MASK);
			glSampleMaski(0, Descriptor_.SampleMask);
		}
		else
		{
			glDisable(GL_SAMPLE_MASK);
		}

		// enable depth test
		if (Descriptor_.DepthStencil.DepthEnable)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthMask(Descriptor_.DepthStencil.WriteMask);
			glDepthFunc(Descriptor_.DepthStencil.DepthFunction);
			glDepthRangef(1, 0);
			// enable depth stencil
			if (Descriptor_.DepthStencil.StencilEnable)
			{
				glEnable(GL_STENCIL_TEST);
				glStencilMask(Descriptor_.DepthStencil.StencilWriteMask);
				glStencilOpSeparate(FRONT, Descriptor_.DepthStencil.FrontFace.StencilFailOp, Descriptor_.DepthStencil.FrontFace.StencilDepthFailOp, Descriptor_.DepthStencil.FrontFace.StencilDepthPassOp);
				glStencilOpSeparate(BACK, Descriptor_.DepthStencil.BackFace.StencilFailOp, Descriptor_.DepthStencil.BackFace.StencilDepthFailOp, Descriptor_.DepthStencil.BackFace.StencilDepthPassOp);
				glStencilMask(Descriptor_.DepthStencil.StencilWriteMask);

				glStencilFuncSeparate(Descriptor_.DepthStencil.FrontFace.StencilFunction, Descriptor_.DepthStencil.BackFace.StencilFunction, Descriptor_.DepthStencil.StencilReference, Descriptor_.DepthStencil.StencilMask);
			}
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_STENCIL_TEST);
		}

		// enable offsets
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonMode(FRONT_AND_BACK, Descriptor_.Rasterizer.Fill);
		if (Descriptor_.Rasterizer.CullMode == NONE)
		{
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glEnable(GL_CULL_FACE);
			glCullFace(Descriptor_.Rasterizer.CullMode);
		}
		glFrontFace(Descriptor_.Rasterizer.Order);
	}

	GraphicsPipeline* GraphicsPipeline::CreatePipeline(std::shared_ptr<ECSManager> Manager, const GraphicsPipelineDesc& Descriptor)
	{
		GraphicsPipeline * pipeline = new GraphicsPipeline;

		// assert the vertex and fragment files exit
		FLUFF_ASSERT(Descriptor.VertexShaderFile)
		FLUFF_ASSERT(Descriptor.FragmentShaderFile)

		// create shader with vertex, fragment, geometry, and tesselation 
		if (Descriptor.TesselationEvaluationShaderFile && Descriptor.TesselationControlShaderFile && Descriptor.GeometryShaderFile)
		{
			pipeline->ShaderHandle_ = new Shader(Manager, Descriptor.VertexShaderFile, Descriptor.GeometryShaderFile, Descriptor.TesselationControlShaderFile, Descriptor.TesselationEvaluationShaderFile, Descriptor.FragmentShaderFile);
		}
		// create shader with vertex, fragment, and geometry
		else if (Descriptor.GeometryShaderFile)
		{
			pipeline->ShaderHandle_ = new Shader(Manager, Descriptor.VertexShaderFile, Descriptor.GeometryShaderFile, Descriptor.FragmentShaderFile);
		}
		// create shader with vertex and fragment
		else
		{
			pipeline->ShaderHandle_ =new Shader(Manager, Descriptor.VertexShaderFile, Descriptor.FragmentShaderFile);
		}

		FLUFF_ASSERT(pipeline->ShaderHandle_ != nullptr)
		FLUFF_ASSERT(Descriptor.RenderWidth)
		FLUFF_ASSERT(Descriptor.RenderHeight)

		// create render target
//		pipeline->RenderTarget_ = Context::CreateFrameBuffer(Descriptor.RenderWidth, Descriptor.RenderHeight, Descriptor.RenderSampleCount, Descriptor.RenderTargetCount);

		// create transform feedback
		pipeline->ShaderHandle_->Bind();
		glTransformFeedbackVaryings(pipeline->ShaderHandle_->GetHandle(), Descriptor.VertexFeedback.NumFeedback, Descriptor.VertexFeedback.OutputVars, Descriptor.VertexFeedback.BufferMode);
		pipeline->ShaderHandle_->Unbind();

		// copy pipeline
		memcpy_s((GraphicsPipeline*) &pipeline->Descriptor_, sizeof(GraphicsPipelineDesc), &Descriptor, sizeof(GraphicsPipelineDesc));

		return pipeline;
	}

	void GraphicsPipeline::SetDefault()
	{
		glBlendFunc(GL_ONE, GL_ONE);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		glDisable(GL_SAMPLE_MASK);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_POLYGON_OFFSET_LINE);
		glDisable(GL_POLYGON_OFFSET_POINT);
	}

	void GraphicsPipeline::Create(std::shared_ptr<ECSManager> & pManager)
	{

		// assert the vertex and fragment files exit
		FLUFF_ASSERT(Descriptor_.VertexShaderFile)
		FLUFF_ASSERT(Descriptor_.FragmentShaderFile)

			// create shader with vertex, fragment, geometry, and tesselation 
		if (Descriptor_.TesselationEvaluationShaderFile && Descriptor_.TesselationControlShaderFile && Descriptor_.GeometryShaderFile)
		{
			this->ShaderHandle_ = new Shader(pManager, Descriptor_.VertexShaderFile, Descriptor_.GeometryShaderFile, Descriptor_.TesselationControlShaderFile, Descriptor_.TesselationEvaluationShaderFile, Descriptor_.FragmentShaderFile);
		}
		// create shader with vertex, fragment, and geometry
		else if (Descriptor_.GeometryShaderFile)
		{
			this->ShaderHandle_ = new Shader(pManager, Descriptor_.VertexShaderFile, Descriptor_.GeometryShaderFile, Descriptor_.FragmentShaderFile);
		}
		// create shader with vertex and fragment
		else
		{
			this->ShaderHandle_ = new Shader(pManager, Descriptor_.VertexShaderFile, Descriptor_.FragmentShaderFile);
		}

		FLUFF_ASSERT(this->ShaderHandle_ != nullptr)
		FLUFF_ASSERT(Descriptor_.RenderWidth)
		FLUFF_ASSERT(Descriptor_.RenderHeight)

			// create render target
			//		pipeline->RenderTarget_ = Context::CreateFrameBuffer(Descriptor.RenderWidth, Descriptor.RenderHeight, Descriptor.RenderSampleCount, Descriptor.RenderTargetCount);

			// create transform feedback
		if (ShaderHandle_)
		{
			this->ShaderHandle_->Bind();
			glTransformFeedbackVaryings(this->ShaderHandle_->GetHandle(), Descriptor_.VertexFeedback.NumFeedback, Descriptor_.VertexFeedback.OutputVars, Descriptor_.VertexFeedback.BufferMode);
			this->ShaderHandle_->Unbind();
		}
		
	}
	
	void Serialize(rapidjson::Document & Doc, GraphicsPipelineDesc & Descriptor, const char * Name)
	{
		using namespace rapidjson;

		// Shader Information

		Value vertex_shader;
		char * str = Descriptor.VertexShaderFile == nullptr ? "nullptr" : Descriptor.VertexShaderFile;
		vertex_shader.SetString(str, static_cast<SizeType>(strlen(str)), Doc.GetAllocator());

		Value tess_eval_shader;
		str = Descriptor.TesselationEvaluationShaderFile == nullptr ? "nullptr" : Descriptor.TesselationEvaluationShaderFile;
		tess_eval_shader.SetString(str, static_cast<SizeType>(strlen(str)), Doc.GetAllocator());

		Value tess_control_shader;
		str = Descriptor.TesselationControlShaderFile == nullptr ? "nullptr" : Descriptor.TesselationControlShaderFile;
		tess_control_shader.SetString(str, static_cast<SizeType>(strlen(str)), Doc.GetAllocator());

		Value geo_shader;
		str = Descriptor.GeometryShaderFile == nullptr ? "nullptr" : Descriptor.GeometryShaderFile;
		geo_shader.SetString(str, static_cast<SizeType>(strlen(str)), Doc.GetAllocator());

		Value frag_shader;
		str = Descriptor.FragmentShaderFile == nullptr ? "nullptr" : Descriptor.FragmentShaderFile;
		frag_shader.SetString(str, static_cast<SizeType>(strlen(str)), Doc.GetAllocator());

		// Transform Feedback

		Value transform_feedback_descriptor;
		transform_feedback_descriptor.SetObject();

		Value num_transform_feedback;
		num_transform_feedback.SetUint(Descriptor.VertexFeedback.NumFeedback);

		Value output_var;
		output_var.SetArray();

		for (size_t i = 0; i < num_transform_feedback.GetUint(); i++)
		{
			Value tmp;
			tmp.SetString(Descriptor.VertexFeedback.OutputVars[i], static_cast<SizeType>(strlen(Descriptor.VertexFeedback.OutputVars[i])), Doc.GetAllocator());
			output_var.PushBack(tmp, Doc.GetAllocator());
		}

		Value buffer_mode;
		buffer_mode.SetUint(Descriptor.VertexFeedback.BufferMode);

		transform_feedback_descriptor.AddMember("num_outputs", num_transform_feedback, Doc.GetAllocator());
		transform_feedback_descriptor.AddMember("output_names", output_var, Doc.GetAllocator());
		transform_feedback_descriptor.AddMember("buffer_mode", buffer_mode, Doc.GetAllocator());

		// Blend state

		Value blend_state_descriptor;
		blend_state_descriptor.SetObject();

		Value alpha_to_coverage;
		alpha_to_coverage.SetBool(Descriptor.BlendState.AlphaToCoverage);

		Value independent_blend_target;
		independent_blend_target.SetBool(Descriptor.BlendState.IndependentBlendTarget);

		Value should_blend;
		should_blend.SetBool(Descriptor.BlendState.ShouldBlend);

		Value render_target;
		render_target.SetArray();

		for (size_t i = 0; i < 8; i++)
		{
			Value render_target_desc;
			render_target_desc.SetObject();

			Value src_func;
			src_func.SetUint(Descriptor.BlendState.RenderTarget[i].SourceFunction);

			Value dst_func;
			dst_func.SetUint(Descriptor.BlendState.RenderTarget[i].DestinationFunction);

			Value srca_func;
			srca_func.SetUint(Descriptor.BlendState.RenderTarget[i].SourceAlphaFunction);

			Value dsta_func;
			dsta_func.SetUint(Descriptor.BlendState.RenderTarget[i].DestinationAlphaFunction);

			render_target_desc.AddMember("src_color", src_func, Doc.GetAllocator());
			render_target_desc.AddMember("dst_color", dst_func, Doc.GetAllocator());
			render_target_desc.AddMember("src_alpha", srca_func, Doc.GetAllocator());
			render_target_desc.AddMember("dst_alpha", dsta_func, Doc.GetAllocator());

			render_target.PushBack(render_target_desc, Doc.GetAllocator());
		}

		blend_state_descriptor.AddMember("alpha_to_coverage", alpha_to_coverage, Doc.GetAllocator());
		blend_state_descriptor.AddMember("independent_blend_target", independent_blend_target, Doc.GetAllocator());
		blend_state_descriptor.AddMember("should_blend", should_blend, Doc.GetAllocator());
		blend_state_descriptor.AddMember("render_target_desc", render_target, Doc.GetAllocator());

		// Sample and framebuffer information
		Value sample_mask;
		sample_mask.SetUint(Descriptor.SampleMask);

		Value render_target_count;
		render_target_count.SetUint(Descriptor.RenderTargetCount);
		
		Value render_sample_count;
		render_sample_count.SetUint(Descriptor.RenderSampleCount);

		Value render_width;
		render_width.SetUint(Descriptor.RenderWidth);

		Value render_height;
		render_height.SetUint(Descriptor.RenderHeight);
		
		// Depth stencil

		Value depth_stencil_descriptor;
		depth_stencil_descriptor.SetObject();

		Value depth_enable;
		depth_enable.SetBool(Descriptor.DepthStencil.DepthEnable);

		Value depth_write_mask;
		depth_write_mask.SetUint(Descriptor.DepthStencil.WriteMask);

		Value depth_function;
		depth_function.SetUint(Descriptor.DepthStencil.DepthFunction);

		Value stencil_enable;
		stencil_enable.SetBool(Descriptor.DepthStencil.StencilEnable);

		Value stencil_read_mask;
		stencil_read_mask.SetUint(Descriptor.DepthStencil.StencilReadMask);

		Value stencil_write_mask;
		stencil_write_mask.SetUint(Descriptor.DepthStencil.StencilWriteMask);

		Value stencil_reference;
		stencil_reference.SetUint(Descriptor.DepthStencil.StencilReference);

		Value stencil_mask;
		stencil_mask.SetUint(Descriptor.DepthStencil.StencilMask);

		Value front_face_op;
		front_face_op.SetObject();

		Value front_fail_op;
		front_fail_op.SetUint(Descriptor.DepthStencil.FrontFace.StencilFailOp);
		
		Value front_depth_fail_op;
		front_depth_fail_op.SetUint(Descriptor.DepthStencil.FrontFace.StencilDepthFailOp);

		Value front_depth_pass_op;
		front_depth_pass_op.SetUint(Descriptor.DepthStencil.FrontFace.StencilDepthPassOp);

		Value front_comp_func;
		front_comp_func.SetUint(Descriptor.DepthStencil.FrontFace.StencilFunction);

		front_face_op.AddMember("fail", front_fail_op, Doc.GetAllocator());
		front_face_op.AddMember("depth_fail", front_depth_fail_op, Doc.GetAllocator());
		front_face_op.AddMember("depth_pass", front_depth_pass_op, Doc.GetAllocator());
		front_face_op.AddMember("function", front_comp_func, Doc.GetAllocator());

		Value back_face_op;
		back_face_op.SetObject();

		Value back_fail_op;
		back_fail_op.SetUint(Descriptor.DepthStencil.FrontFace.StencilFailOp);

		Value back_depth_fail_op;
		back_depth_fail_op.SetUint(Descriptor.DepthStencil.FrontFace.StencilDepthFailOp);

		Value back_depth_pass_op;
		back_depth_pass_op.SetUint(Descriptor.DepthStencil.FrontFace.StencilDepthPassOp);

		Value back_comp_func;
		back_comp_func.SetUint(Descriptor.DepthStencil.FrontFace.StencilFunction);

		back_face_op.AddMember("fail", back_fail_op, Doc.GetAllocator());
		back_face_op.AddMember("depth_fail", back_depth_fail_op, Doc.GetAllocator());
		back_face_op.AddMember("depth_pass", back_depth_pass_op, Doc.GetAllocator());
		back_face_op.AddMember("function", back_comp_func, Doc.GetAllocator());

		depth_stencil_descriptor.AddMember("depth_enable", depth_enable, Doc.GetAllocator());
		depth_stencil_descriptor.AddMember("depth_write_mask", depth_write_mask, Doc.GetAllocator());
		depth_stencil_descriptor.AddMember("depth_comparison_function", depth_function, Doc.GetAllocator());
		depth_stencil_descriptor.AddMember("stencil_enable", stencil_enable, Doc.GetAllocator());
		depth_stencil_descriptor.AddMember("stencil_read_mask", stencil_read_mask, Doc.GetAllocator());
		depth_stencil_descriptor.AddMember("stencil_write_mask", stencil_write_mask, Doc.GetAllocator());
		depth_stencil_descriptor.AddMember("stencil_reference", stencil_reference, Doc.GetAllocator());
		depth_stencil_descriptor.AddMember("stencil_mask", stencil_mask, Doc.GetAllocator());
		depth_stencil_descriptor.AddMember("front_face", front_face_op, Doc.GetAllocator());
		depth_stencil_descriptor.AddMember("back_face", back_face_op, Doc.GetAllocator());

		// Primitive Topology

		Value primitive;
		primitive.SetUint(Descriptor.Topology);

		// Rasterizer

		Value rasterizer_descriptor;
		rasterizer_descriptor.SetObject();

		Value fill_mode;
		fill_mode.SetUint(Descriptor.Rasterizer.Fill);

		Value cull_mode;
		cull_mode.SetUint(Descriptor.Rasterizer.CullMode);

		Value face_order;
		face_order.SetUint(Descriptor.Rasterizer.Order);

		Value poly_mode;
		poly_mode.SetUint(Descriptor.Rasterizer.PolyMode);

		Value depth_bias;
		depth_bias.SetUint(Descriptor.Rasterizer.DepthBias);

		Value depth_clamp;
		depth_clamp.SetFloat(Descriptor.Rasterizer.DepthClamp);

		Value slope_scaled_depth_bias;
		slope_scaled_depth_bias.SetFloat(Descriptor.Rasterizer.SlopeScaledDepthBias);

		Value depth_clip_enable;
		depth_clip_enable.SetBool(Descriptor.Rasterizer.DepthClipEnable);

		Value multisample_enable;
		multisample_enable.SetBool(Descriptor.Rasterizer.MultisampleEnable);

		Value antialias_lines_enable;
		antialias_lines_enable.SetBool(Descriptor.Rasterizer.AntialiasLineEnable);

		Value force_samples_count;
		force_samples_count.SetUint(Descriptor.Rasterizer.ForceSampleCount);

		rasterizer_descriptor.AddMember("fill_mode", fill_mode, Doc.GetAllocator());
		rasterizer_descriptor.AddMember("cull_mode", cull_mode, Doc.GetAllocator());
		rasterizer_descriptor.AddMember("face_order", face_order, Doc.GetAllocator());
		rasterizer_descriptor.AddMember("polygon_offset_mode", poly_mode, Doc.GetAllocator());
		rasterizer_descriptor.AddMember("depth_bias", depth_bias, Doc.GetAllocator());
		rasterizer_descriptor.AddMember("depth_clamp", depth_clamp, Doc.GetAllocator());
		rasterizer_descriptor.AddMember("slope_scaled_depth_bias", slope_scaled_depth_bias, Doc.GetAllocator());
		rasterizer_descriptor.AddMember("depth_clip_enable", depth_clip_enable, Doc.GetAllocator());
		rasterizer_descriptor.AddMember("multisample_enable", multisample_enable, Doc.GetAllocator());
		rasterizer_descriptor.AddMember("antialias_line_enable", antialias_lines_enable, Doc.GetAllocator());
		rasterizer_descriptor.AddMember("force_samples_count", force_samples_count, Doc.GetAllocator());

		Value pipeline_root;
		pipeline_root.SetObject();

		pipeline_root.AddMember("vertex", vertex_shader, Doc.GetAllocator());
		pipeline_root.AddMember("tess_eval", tess_eval_shader, Doc.GetAllocator());
		pipeline_root.AddMember("tess_ctrl", tess_control_shader, Doc.GetAllocator());
		pipeline_root.AddMember("geometry", geo_shader, Doc.GetAllocator());
		pipeline_root.AddMember("fragment", frag_shader, Doc.GetAllocator());
		pipeline_root.AddMember("transform_feedback", transform_feedback_descriptor, Doc.GetAllocator());
		pipeline_root.AddMember("blend_state", blend_state_descriptor, Doc.GetAllocator());
		pipeline_root.AddMember("sample_mask", sample_mask, Doc.GetAllocator());
		pipeline_root.AddMember("render_target_count", render_target_count, Doc.GetAllocator());
		pipeline_root.AddMember("render_sample_count", render_sample_count, Doc.GetAllocator());
		pipeline_root.AddMember("render_width", render_width, Doc.GetAllocator());
		pipeline_root.AddMember("render_height", render_height, Doc.GetAllocator());
		pipeline_root.AddMember("depth_stencil", depth_stencil_descriptor, Doc.GetAllocator());
		pipeline_root.AddMember("primitive_topo", primitive, Doc.GetAllocator());
		pipeline_root.AddMember("rasterizer", rasterizer_descriptor, Doc.GetAllocator());

		Doc.AddMember(rapidjson::StringRef(Name, strlen(Name)), pipeline_root, Doc.GetAllocator());
	}

	GraphicsPipelineDesc Deserialize(rapidjson::Document & Doc, const char * PipelineName)
	{
		using namespace rapidjson;
		
		Value pipeline = Doc[PipelineName].GetObject();

		char * VertexShaderFile = nullptr;
		if (strcmp(pipeline["vertex"].GetString(), "nullptr") != 0)
		{
			VertexShaderFile = new char[strlen(pipeline["vertex"].GetString()) + 1];
			strcpy_s(VertexShaderFile, strlen(pipeline["vertex"].GetString()) + 1, pipeline["vertex"].GetString());
		}

		char * TesselationEvaluationShaderFile = nullptr;
		if (strcmp(pipeline["tess_eval"].GetString(), "nullptr") != 0)
		{
			TesselationEvaluationShaderFile = new char[strlen(pipeline["tess_eval"].GetString()) + 1];
			strcpy_s(TesselationEvaluationShaderFile, strlen(pipeline["tess_eval"].GetString()) + 1, pipeline["tess_eval"].GetString());
		}

		char * TesselationControlShaderFile = nullptr;
		if (strcmp(pipeline["tess_ctrl"].GetString(), "nullptr") != 0)
		{
			TesselationControlShaderFile = new char[strlen(pipeline["tess_ctrl"].GetString()) + 1];
			strcpy_s(TesselationControlShaderFile, strlen(pipeline["tess_ctrl"].GetString()) + 1, pipeline["tess_ctrl"].GetString());
		}

		char * GeometryShaderFile = nullptr;
		if (strcmp(pipeline["geometry"].GetString(), "nullptr") != 0)
		{
			GeometryShaderFile = new char[strlen(pipeline["geometry"].GetString()) + 1];
			strcpy_s(GeometryShaderFile, strlen(pipeline["geometry"].GetString()) + 1, pipeline["geometry"].GetString());
		}

		char * FragmentShaderFile = nullptr;
		if (strcmp(pipeline["fragment"].GetString(), "nullptr") != 0)
		{
			FragmentShaderFile = new char[strlen(pipeline["fragment"].GetString()) + 1];
			strcpy_s(FragmentShaderFile, strlen(pipeline["fragment"].GetString()) + 1, pipeline["fragment"].GetString());
		}

		Value transform_feedback = pipeline["transform_feedback"].GetObject();
		TransformFeedbackDesc tfd =
		{
			transform_feedback["num_outputs"].GetUint(),
			new char*[transform_feedback["num_outputs"].GetUint()],
			static_cast<TransformBufferMode>(transform_feedback["buffer_mode"].GetUint())
		};
		for (uint32_t i = 0; i < tfd.NumFeedback; ++i)
		{
			auto itr = (transform_feedback["num_outputs"].Begin() + i);
			tfd.OutputVars[i] = (char*) itr->GetString();
		}

		Value blend_state = pipeline["blend_state"].GetObject();
		auto alpha_to_coverage = blend_state["alpha_to_coverage"].GetBool();
		auto independent_blend = blend_state["independent_blend_target"].GetBool();
		auto should_blend = blend_state["should_blend"].GetBool();
		RenderTargetBlendDesc targets[8];

		auto render_target = blend_state["render_target_desc"].GetArray();
		size_t counter = 0;
		for (auto & v : render_target)
		{
			auto & t = targets[counter++];
			t.SourceFunction = static_cast<BlendFunction>(v["src_color"].GetUint());
			t.DestinationFunction = static_cast<BlendFunction>(v["dst_color"].GetUint());
			t.SourceAlphaFunction = static_cast<BlendFunction>(v["src_alpha"].GetUint());
			t.DestinationAlphaFunction = static_cast<BlendFunction>(v["dst_alpha"].GetUint());
		}

		BlendStateDesc bdesc = 
		{
			alpha_to_coverage,
			independent_blend,
			should_blend,
		};

		memcpy(bdesc.RenderTarget, targets, sizeof(targets));

		auto sample_mask = pipeline["sample_mask"].GetUint();
		auto render_target_count = pipeline["render_target_count"].GetUint();
		auto render_sample_count = pipeline["render_sample_count"].GetUint();
		auto render_width = pipeline["render_width"].GetUint();
		auto render_height = pipeline["render_height"].GetUint();

		Value depth_stencil = pipeline["depth_stencil"].GetObject();
		auto depth_enable = depth_stencil["depth_enable"].GetBool();
		auto depth_write_mask = depth_stencil["depth_write_mask"].GetUint();
		auto depth_comp_func = depth_stencil["depth_comparison_function"].GetUint();
		auto stencil_enable = depth_stencil["stencil_enable"].GetBool();
		auto stencil_read_mask = depth_stencil["stencil_read_mask"].GetUint();
		auto stencil_write_mask = depth_stencil["stencil_write_mask"].GetUint();
		auto stencil_reference = depth_stencil["stencil_reference"].GetUint();
		auto stencil_mask = depth_stencil["stencil_mask"].GetUint();
		
		auto front_face_op = depth_stencil["front_face"].GetObject();
		auto ffail = front_face_op["fail"].GetUint();
		auto fdfail = front_face_op["depth_fail"].GetUint();
		auto fdpass = front_face_op["depth_pass"].GetUint();
		auto ffunc = front_face_op["function"].GetUint();

		auto back_face_op = depth_stencil["back_face"].GetObject();

		auto bfail = back_face_op["fail"].GetUint();
		auto bdfail = back_face_op["depth_fail"].GetUint();
		auto bdpass = back_face_op["depth_pass"].GetUint();
		auto bfunc = back_face_op["function"].GetUint();

		DepthStencilDesc dpd = 
		{
			depth_enable,
			static_cast<DepthWriteMask>(depth_write_mask),
			static_cast<ComparisonFunction>(depth_comp_func),
			stencil_enable,
			static_cast<uint8_t>(stencil_read_mask),
			static_cast<uint8_t>(stencil_write_mask),
			static_cast<uint8_t>(stencil_reference),
			stencil_mask,
			{
				static_cast<StencilOp>(ffail),
				static_cast<StencilOp>(fdfail),
				static_cast<StencilOp>(fdpass),
				static_cast<ComparisonFunction>(ffunc)
			},
			{
				static_cast<StencilOp>(bfail),
				static_cast<StencilOp>(bdfail),
				static_cast<StencilOp>(bdpass),
				static_cast<ComparisonFunction>(bfunc)
			}
		};

		auto topo = pipeline["primitive_topo"].GetUint();

		Value rasterizer = pipeline["rasterizer"].GetObject();

		auto fill_mode = rasterizer["fill_mode"].GetUint();
		auto cull_mode = rasterizer["cull_mode"].GetUint();
		auto face_order = rasterizer["face_order"].GetUint();
		auto poly_mode = rasterizer["polygon_offset_mode"].GetUint();
		auto depth_bias = rasterizer["depth_bias"].GetUint();
		auto depth_clamp = rasterizer["depth_clamp"].GetFloat();
		auto slope_scaled_depth_bias = rasterizer["slope_scaled_depth_bias"].GetFloat();
		auto depth_clip_enable = rasterizer["depth_clip_enable"].GetBool();
		auto multisample_enable = rasterizer["multisample_enable"].GetBool();
		auto antialias_line_enable = rasterizer["antialias_line_enable"].GetBool();
		auto force_samples_count = rasterizer["force_samples_count"].GetUint();

		RasterizerDesc rd = 
		{
			static_cast<FillMode>(fill_mode),
			static_cast<Face>(cull_mode),
			static_cast<RenderOrder>(face_order),
			static_cast<PolygonMode>(poly_mode),
			depth_bias,
			depth_clamp,
			slope_scaled_depth_bias,
			depth_clip_enable,
			multisample_enable,
			antialias_line_enable,
			force_samples_count
		};

		GraphicsPipelineDesc desc = 
		{
			VertexShaderFile,
			TesselationEvaluationShaderFile,
			TesselationControlShaderFile,
			GeometryShaderFile,
			FragmentShaderFile,
			tfd,
			bdesc,
			sample_mask,
			static_cast<uint8_t>(render_target_count),
			static_cast<uint8_t>(render_sample_count),
			render_width,
			render_height,
			dpd,
			static_cast<PrimitiveTopology>(topo),
			rd
		};
		return desc;
	}
} }
