#ifndef _pluginUFE_OpenGLRenderer_SGLRenderer_h_
#define _pluginUFE_OpenGLRenderer_SGLRenderer_h_

#include <UFEngine/UFEngine.h>

#include "DataManager.h"

#include "Model.h"
#include "Renderer.h"
#include "Texture.h"
#include "CGLAutonomous.h"

namespace Upp{

/*
	In UFEngine Service always start by S letter
*/
class SGLRenderer : public Service{
	public:
		SGLRenderer(UFEContext& _context);
		virtual ~SGLRenderer();
		
		String GetName()const{return "SGLRenderer";}
		
		DataManager<Model>& GetModelManager(){return models;}
		const DataManager<Model>& GetModelManager()const{return models;}
		
		DataManager<ShaderProgram>& GetShaderManager(){return shaders;}
		const DataManager<ShaderProgram>& GetShaderManager()const{return shaders;}
		
		DataManager<Renderer>& GetRendererManager(){return renderers;}
		const DataManager<Renderer>& GetRendererManager()const{return renderers;}
		
		DataManager<Texture>& GetTextureManager(){return textures;}
		const DataManager<Texture>& GetTextureManager()const{return textures;}
		
		void AddToRenderQueue(Renderer& renderer, ShaderProgram& shader, Model& model, Object& object);
		void AddToRenderQueue(CGLAutonomous& autonomous);
		
		virtual String ToString()const;
		
		virtual void AfterUpdate();
	private:
		struct ObjectToRender : public Moveable<ObjectToRender> {
			ObjectToRender& SetRenderer(Renderer& ptr){renderer = &ptr; return *this;}
			ObjectToRender& SetShaderProgram(ShaderProgram& ptr){shader = &ptr; return *this;}
			ObjectToRender& SetModel(Model& ptr){model = &ptr; return *this;}
			ObjectToRender& SetObject(Object& ptr){object = &ptr; return *this;}
			
			Renderer* renderer = nullptr;
			ShaderProgram* shader = nullptr;
			Model* model = nullptr;
			Object* object = nullptr;
		};
		
		VectorMap<ShaderProgram*, VectorMap<Renderer*, VectorMap<Model*, Vector<ObjectToRender>>>> renderQueue;
		Vector<CGLAutonomous*> autonomousRenderQueue;
		
		DataManager<Model> models;
		DataManager<Renderer> renderers;
		DataManager<ShaderProgram> shaders;
		DataManager<Texture> textures;
};
	
}


#endif
