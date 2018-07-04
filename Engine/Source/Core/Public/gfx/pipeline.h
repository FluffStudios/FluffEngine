#pragma once

#include <debug.h>

#include <gfx/shader.h>
#include <gfx/frame_buffer.h>
#include <json_serializable.h>

#include <cereal/external/rapidjson/filereadstream.h>
#include <cereal/external/rapidjson/filewritestream.h>

#include <cereal/access.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include <core/ecs/ecs_manager.h>

#define DEFAULT_STENCIL_READ_MASK	0xff
#define DEFAULT_STENCIL_WRITE_MASK	0xff

namespace fluff { namespace gfx
{

	enum FLUFF_API BlendFunction : uint32_t
	{
		ZERO							= 0x0000, // GL_ZERO						
		ONE								= 0x0001, // GL_ONE						
		SOURCE_COLOR					= 0x0300, // GL_SRC_COLOR					
		ONE_MINUS_SOURCE_COLOR			= 0x0301, // GL_ONE_MINUS_SRC_COLOR
		SOURCE_ALPHA					= 0x0302, // GL_SRC_ALPHA
		ONE_MINUS_SOURCE_ALPHA			= 0x0303, // GL_ONE_MINUS_SRC_ALPHA
		DESTINATION_ALPHA				= 0x0304, // GL_DST_ALPHA
		ONE_MINUS_DESTINATION_ALPHA		= 0x0305, // GL_ONE_MINUS_DST_ALPHA
		DESTINATION_COLOR				= 0x0306, // GL_DST_COLOR
 		ONE_MINUS_DESTINATION_COLOR		= 0x0307, // GL_ONE_MINUS_DST_COLOR
		BLEND_SOURCE_ALPHA_SATURATE		= 0x0308, // GL_SRC_ALPHA_SATURATE
		BLEND_CONSTANT_COLOR			= 0x8001, // GL_CONSTANT_COLOR
		BLEND_ONE_MINUS_CONSTANT_COLOR	= 0x8002, // GL_ONE_MINUS_CONSTANT_COLOR
		BLEND_CONSTANT_ALPHA			= 0x8003, // GL_CONSTANT_ALPHA
		BLEND_ONE_MINUS_CONSTANT_ALPHA	= 0x8004, // GL_ONE_MINUS_CONSTANT_ALPHA
	};

	enum FLUFF_API Face : uint32_t
	{
		FRONT			= 0x0404, // GL_FRONT
		BACK			= 0x0405, // GL_BACK
		FRONT_AND_BACK	= 0x0408, // GL_FRONT_AND_BACK
		NONE			= 0x0000, // GL_NONE
	};

	enum FLUFF_API ComparisonFunction : uint32_t
	{
		NEVER			= 0x0200, // GL_NEVER
		LESS			= 0x0201, // GL_LESS
		EQUALS			= 0x0202, // GL_EQUAL
		LESS_THAN		= 0x0203, // GL_LEQUAL
		GREATER			= 0x0204, // GL_GREATER
		NOT_EQUALS		= 0x0205, // GL_NOTEQUAL
		GREATER_THAN	= 0x0206, // GL_GEQUAL
		ALWAYS			= 0x0207, // GL_ALWAYS
	};

	enum FLUFF_API TransformBufferMode : uint32_t
	{
		INTERLEAVED = 0x8C8C, // GL_INTERLEAVED_ATTRIBS
		SEPARATE	= 0x8C8D, // GL_SEPARATE_ATTRIBS
	};

	enum FLUFF_API FillMode : uint32_t
	{
		FILL_POINT		= 0x1B00, // GL_POINT
		FILL_LINE		= 0x1B01, // GL_LINE
		FILL_POLYGON	= 0x1B02, // GL_FILL
	};

	enum FLUFF_API PolygonMode : uint32_t
	{
		POLY_POINT		= 0x2A01, // GL_POLYGON_OFFSET_POINT
		POLY_LINE		= 0x2A02, // GL_POLYGON_OFFSET_LINE
		POLY_FILL		= 0x8037, // GL_POLYGON_OFFSET_FILL
	};

	enum FLUFF_API RenderOrder : uint32_t
	{
		CLOCKWISE			= 0x0900, // GL_CW
		COUNTER_CLOCKWISE	= 0x0901, // GL_CCW
	};

	enum FLUFF_API DepthWriteMask : uint32_t
	{
		DEPTH_MASK_WRITE_NONE = 0x0,
		DEPTH_MASK_WRITE_ALL  = 0xFFFFFFFF
	};

	enum FLUFF_API StencilOp : uint32_t
	{
		STENCIL_ZERO			= 0x0000, // GL_ZERO
		STENCIL_KEEP			= 0x1E00, // GL_KEEP
		STENCIL_REPLACE			= 0x1E01, // GL_REPLACE
		STENCIL_INCREASE		= 0x1E02, // GL_INCR
		STENCIL_DECREASE		= 0x1E03, // GL_DECR
		STENCIL_INVERT			= 0x150A, // GL_INVERT
		STENCIL_INCREASE_WRAP	= 0x8507, // GL_INCR_WRAP
		STENCIL_DECREASE_WRAP	= 0x8508, // GL_DECR_WRAP

	};

	enum FLUFF_API PrimitiveTopology : uint32_t
	{
		TOPO_POINT		= 0x0000, // GL_POINTS
		TOPO_LINE		= 0x0001, // GL_LINES
		TOPO_TRIANGLE	= 0x0004, // GL_TRIANGLES
		TOPO_PATCH		= 0x000E, // GL_PATCHES
	};

	struct FLUFF_API TransformFeedbackDesc
	{
		uint32_t					NumFeedback = 0;
		char **						OutputVars	= nullptr;
		enum TransformBufferMode	BufferMode	= SEPARATE;

		~TransformFeedbackDesc()
		{
			if (OutputVars && NumFeedback)
			{
				for (auto i = 0; i < NumFeedback; i++)
				{
					if (OutputVars[i]) delete [] OutputVars[i];
				}
				delete[] OutputVars;
				OutputVars = nullptr;
			}
		}
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive & ar) const
		{
			std::vector<std::string> outs;
			for (auto i = 0; i < NumFeedback; i++)
			{
				outs.push_back(OutputVars[i]);
			}

			ar(NumFeedback, outs, BufferMode);
		}

		template <class Archive>
		void load(Archive & ar)
		{
			std::vector<std::string> outs;

			ar(NumFeedback, outs, BufferMode);
			if (NumFeedback) OutputVars = new char *[NumFeedback];
			for (auto i = 0; i < NumFeedback; i++)
			{
				OutputVars[i] = new char[outs[i].length() + 1];
				memcpy(OutputVars[i], outs[i].data(), outs[i].length());
				OutputVars[outs[i].length()] = '\0';
			}
		}
	};

	struct FLUFF_API RenderTargetBlendDesc
	{
		enum BlendFunction SourceFunction			= ONE;
		enum BlendFunction DestinationFunction		= ZERO;
		enum BlendFunction SourceAlphaFunction		= ONE;
		enum BlendFunction DestinationAlphaFunction = ZERO;
	private:
		friend class cereal::access;

		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(SourceFunction, DestinationFunction, SourceAlphaFunction, DestinationAlphaFunction);
		}
	};

	struct FLUFF_API BlendStateDesc
	{
		bool							AlphaToCoverage;
		bool							IndependentBlendTarget;
		bool							ShouldBlend;
		struct RenderTargetBlendDesc	RenderTarget[8];
	private:
		friend class cereal::access;

		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(AlphaToCoverage, IndependentBlendTarget, ShouldBlend, RenderTarget[0], 
				RenderTarget[1],
				RenderTarget[2],
				RenderTarget[3],
				RenderTarget[4],
				RenderTarget[5],
				RenderTarget[6],
				RenderTarget[7]);
		}
	};

	struct FLUFF_API RasterizerDesc
	{
		FillMode	Fill					= FILL_POLYGON;
		Face		CullMode				= BACK;
		RenderOrder	Order					= COUNTER_CLOCKWISE;
		PolygonMode	PolyMode				= POLY_FILL;
		uint32_t	DepthBias				= 0;
		float32_t	DepthClamp				= 0.0f;
		float32_t	SlopeScaledDepthBias	= 0.0f;
		bool		DepthClipEnable			= true;
		bool		MultisampleEnable		= false;
		bool		AntialiasLineEnable		= false;
		uint32_t	ForceSampleCount		= 0;
	private:
		friend class cereal::access;

		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(Fill, CullMode, Order, PolyMode, DepthBias, DepthClamp, SlopeScaledDepthBias, DepthClipEnable, MultisampleEnable, AntialiasLineEnable, ForceSampleCount);
		}
	};

	struct FLUFF_API DepthStencilOpDesc
	{
		StencilOp			StencilFailOp		= STENCIL_KEEP;
		StencilOp			StencilDepthFailOp	= STENCIL_KEEP;
		StencilOp			StencilDepthPassOp	= STENCIL_KEEP;
		ComparisonFunction	StencilFunction		= ALWAYS;
	private:
		friend class cereal::access;

		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(StencilFailOp, StencilDepthFailOp, StencilDepthPassOp, StencilFunction);
		}
	};

	struct FLUFF_API DepthStencilDesc
	{
		bool				DepthEnable			= false;
		DepthWriteMask		WriteMask			= DEPTH_MASK_WRITE_ALL;
		ComparisonFunction	DepthFunction		= LESS;
		bool				StencilEnable		= false;
		uint8_t				StencilReadMask		= DEFAULT_STENCIL_READ_MASK;
		uint8_t				StencilWriteMask	= DEFAULT_STENCIL_WRITE_MASK;
		uint8_t				StencilReference	= 0;
		uint32_t			StencilMask			= 1;
		DepthStencilOpDesc	FrontFace;
		DepthStencilOpDesc	BackFace;
	private:
		friend class cereal::access;

		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(DepthEnable, WriteMask, DepthFunction, StencilEnable, StencilReadMask, StencilWriteMask, StencilReference, StencilMask, FrontFace, BackFace);
		}
	};

	FLUFF_API struct GraphicsPipelineDesc
	{
					char*	 				VertexShaderFile				= nullptr;
					char*	 				TesselationEvaluationShaderFile	= nullptr;
					char*	 				TesselationControlShaderFile	= nullptr;
					char*	 				GeometryShaderFile				= nullptr;
					char*	 				FragmentShaderFile				= nullptr;
		struct		TransformFeedbackDesc	VertexFeedback;
		struct		BlendStateDesc			BlendState;
					uint32_t				SampleMask						= 0;
					uint8_t					RenderTargetCount				= 1;
					uint8_t					RenderSampleCount				= 1;
					uint32_t				RenderWidth						= 0;
					uint32_t				RenderHeight					= 0;
		struct		DepthStencilDesc		DepthStencil;
		enum		PrimitiveTopology		Topology						= TOPO_TRIANGLE;
		struct		RasterizerDesc			Rasterizer;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive & ar) const
		{
			std::string vert = VertexShaderFile == nullptr ? "" : VertexShaderFile;
			std::string eval = TesselationEvaluationShaderFile == nullptr ? "" : TesselationEvaluationShaderFile;
			std::string ctrl = TesselationControlShaderFile == nullptr ? "" : TesselationControlShaderFile;
			std::string geom = GeometryShaderFile == nullptr ? "" : GeometryShaderFile;
			std::string frag = FragmentShaderFile == nullptr ? "" : FragmentShaderFile;

			ar(vert, eval, ctrl, geom, frag, VertexFeedback, BlendState, SampleMask, RenderTargetCount,
				RenderSampleCount, RenderWidth, RenderHeight, DepthStencil, Topology, Rasterizer);
		}

		template <class Archive>
		void load(Archive & ar)
		{
			std::string vert;
			std::string eval;
			std::string ctrl;
			std::string geom;
			std::string frag;

			ar(vert, eval, ctrl, geom, frag, VertexFeedback, BlendState, SampleMask, RenderTargetCount, RenderSampleCount, RenderWidth, RenderHeight, DepthStencil,
				Topology, Rasterizer);

			if (vert.length())
			{
				VertexShaderFile = new char[vert.length() + 1];
				strcpy_s(VertexShaderFile, vert.length() + 1, vert.c_str());
			}
			if (eval.length())
			{
				TesselationEvaluationShaderFile = new char[eval.length() + 1];
				strcpy_s(TesselationEvaluationShaderFile, eval.length() + 1, eval.c_str());
			}
			if (ctrl.length())
			{
				TesselationControlShaderFile = new char[ctrl.length() + 1];
				strcpy_s(TesselationControlShaderFile, ctrl.length() + 1, ctrl.c_str());
			}
			if (geom.length())
			{
				GeometryShaderFile = new char[geom.length() + 1];
				strcpy_s(GeometryShaderFile, geom.length() + 1, geom.c_str());
			}
			if (frag.length())
			{
				FragmentShaderFile = new char[frag.length() + 1];
				strcpy_s(FragmentShaderFile, frag.length() + 1, frag.c_str());
			}
		}
	};

	FLUFF_API void Serialize(rapidjson::Document & Doc, GraphicsPipelineDesc& Descriptor, const char * Name);
	FLUFF_API GraphicsPipelineDesc Deserialize(rapidjson::Document & Doc, const char * PipelineName);

	class FLUFF_API GraphicsPipeline
	{
		GraphicsPipelineDesc			Descriptor_;
		FrameBuffer*				RenderTarget_;
		Shader*						ShaderHandle_;
		std::string Name_;

		friend class cereal::access;
		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(Descriptor_, Name_);
		}
	public:
		GraphicsPipeline();

		/*
			Enables the graphics pipeline and sets the 
			defined states
		*/
		void Enable() const;

		/*
			Gets the shader associated with the pipeline

			Returns the shader
		*/
		Shader* GetShader()
		{
			return ShaderHandle_;
		}

		/*
			Gets the render target associated with the pipeline

			Returns render target
		*/
		FrameBuffer* GetRenderTarget()
		{
			return RenderTarget_;
		}

		/*
			Gets if the pipeline should use blending

			Returns if pipeline should blend
		*/
		bool Blended()
		{
			return Descriptor_.BlendState.ShouldBlend;
		}

		/*
			Gets the name of the pipeline

			Returns name of pipeline
		*/
		const std::string GetName()
		{
			return Name_;
		}

		/*
			Creates a pipeline state
			
			param Descriptor - Descriptor for loading the states of
		*/
		static GraphicsPipeline * CreatePipeline(std::shared_ptr<ECSManager> Manager, const GraphicsPipelineDesc &Descriptor);

		/*
			Creates a pipeline state
			
			Descriptor - Descriptor for loading the states of
		*/
		static GraphicsPipeline * CreatePipeline(std::shared_ptr<ECSManager> Manager, const GraphicsPipelineDesc &Descriptor, const char * Name)
		{
			auto line = GraphicsPipeline::CreatePipeline(Manager, Descriptor);
			line->Name_ = Name;
			return line;
		}

		/*
			Sets OpenGL to the default state
		 */
		static void SetDefault();

		void Create(std::shared_ptr<ECSManager> & Manager);
	};
	
} }