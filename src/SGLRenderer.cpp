#include "../SGLRenderer.h"
#include "../CGLRoutineBeforeAfterRendering.h"

#include "../CGLModel.h"
#include "../CGLRenderer.h"
#include "../CGLCamera.h"

namespace Upp{
	
SGLRenderer::SGLRenderer(UFEContext& _context) : Service(_context){}

SGLRenderer::~SGLRenderer(){
	renderQueue.Clear();
	autonomousRenderQueue.Clear();
	textures.~DataManager();
	shaders.~DataManager();
	renderers.~DataManager();
	models.~DataManager();
}

String SGLRenderer::ToString()const{
	String service;
	service << "Context Id: " << GetContextID() << "\n";
	service << "Purpose: Giving a way to draw via OpenGL bunch of object carrying component via MeshData and Renderer object\n";
	service << "Models:\n";
	for(const String& mesh : models.map.GetKeys()){
		service << "\t-" << mesh <<":\n\t\t" << mesh << Replace(models.Get(mesh).ToString(), VectorMap<String,String>{{"\n","\n\t\t"}}) <<"\n";
	}
	service << "\n---------------------\n";
	service << "Shaders:\n";
	for(const String& shader : shaders.map.GetKeys()){
		service << "\t-" << shader << ":\n\t\t" << Replace(shaders.Get(shader).ToString(), VectorMap<String,String>{{"\n","\n\t\t"}}) <<"\n";
	}
	service << "\n---------------------\n";
	service << "Renderers:\n";
	for(const String& renderer : renderers.map.GetKeys()){
		service << "\t-" << renderer << ":\n\t\t" << Replace(renderers.Get(renderer).ToString(), VectorMap<String,String>{{"\n","\n\t\t"}}) <<"\n";
	}
	service << "\n---------------------\n";
	return service;
}

void SGLRenderer::AddToRenderQueue(Renderer& renderer, ShaderProgram& shader, Model& model, Object& object){
	int shaderFind = renderQueue.Find(&shader);
	if(shaderFind != -1){
		if(renderQueue.Get(&shader).Find(&renderer) != -1){
			if(renderQueue.Get(&shader).Get(&renderer).Find(&model) != -1){
				renderQueue.Get(&shader).Get(&renderer).Get(&model).Add().SetModel(model).SetShaderProgram(shader).SetObject(object).SetRenderer(renderer);
			}else{
				renderQueue.Get(&shader).Get(&renderer).Add(&model).Add().SetModel(model).SetShaderProgram(shader).SetObject(object).SetRenderer(renderer);
			}
		}else{
			renderQueue.Get(&shader).Add(&renderer).Add(&model).Add().SetModel(model).SetShaderProgram(shader).SetObject(object).SetRenderer(renderer);
		}
	}else{
		renderQueue.Add(&shader).Add(&renderer).Add(&model).Add().SetModel(model).SetShaderProgram(shader).SetObject(object).SetRenderer(renderer);
	}
}

void SGLRenderer::AddToRenderQueue(CGLAutonomous& autonomous){
	#ifdef _DEBUG
		for(CGLAutonomous* autonomousPtr : autonomousRenderQueue){
				ASSERT_(autonomousPtr == &autonomous, "void SGLRenderer::AddToRenderQueue(CGLAutonomous& autonomous) trying to add something already in render queue :\n\t" + Replace(autonomous.ToString(),Vector<String>{"\n"},Vector<String>{"\n\t"}));
		}
	#endif
	autonomousRenderQueue.Add(&autonomous);
}

void SGLRenderer::AfterUpdate(){
	Scene& scene = GetContext().GetSceneManager().GetActive();
	CGLCamera* camera = nullptr;
	//First we iterate through all object to retrieve object wich need to be render and process
	//it to the render queue. The render queue is used to store all item to be renderer in a
	//special order. We also look for the first active camera we found, and retrieve is view
	//and projection matrix
	for(Object& object : scene.GetObjectManager().GetObjects()){
		if(!camera && object.GetComponentManager().Has<CGLCamera>() && object.GetComponentManager().Get<CGLCamera>().IsActive()){
			camera = &(object.GetComponentManager().Get<CGLCamera>());
			break;
		}
	}
	int renderNumber = 0;
	if(camera){
		//Rendering of autonomous
		for(CGLAutonomous* autonomousPtr : autonomousRenderQueue){
			if(autonomousPtr->renderOrder == RenderOrder::Before)
				autonomousPtr->Draw(*camera);
		}
		//Then we iterate through each renderer in render queue and render each object. we set shader value just before
		for(ShaderProgram* shader : renderQueue.GetKeys()){
			ShaderProgram& shad = *shader;
			if(shad.IsLinked()){
				if(shad.beforeRendering) shad.beforeRendering(shad, *camera);
				for(Renderer* renderer : renderQueue.Get(shader).GetKeys()){
					Renderer& render = *renderer;
					if(render.beforeRendering) render.beforeRendering(render, shad, *camera);
				
					for(Model* model : renderQueue.Get(shader).Get(renderer).GetKeys()){
						for(int e = 0; e < renderQueue.Get(shader).Get(renderer).Get(model).GetCount(); e++){
							
							ObjectToRender& otr = renderQueue.Get(shader).Get(renderer).Get(model)[e];
							
							VectorMap<int,	CGLRoutineBeforeAfterRendering*> allRoutineToDo = (*otr.object).GetComponentManager().GetAll<CGLRoutineBeforeAfterRendering>();
							
							for(int pos : allRoutineToDo.GetKeys()){
								allRoutineToDo.Get(pos)->beforeRendering(*otr.renderer, *otr.shader ,*camera,*(otr.object));
							}
							//Cout() << "Render number " << renderNumber << *(otr.object) << EOL;
							render.Render(*otr.model, *otr.shader, *camera);
							//renderNumber++;
								
							for(int pos : allRoutineToDo.GetKeys()){
								allRoutineToDo.Get(pos)->afterRendering(*otr.renderer, *otr.shader ,*camera,*(otr.object));
							}
						}
						
					}
					if(render.afterRendering) render.afterRendering(render, shad, *camera);
				}
				if(shad.afterRendering) shad.afterRendering(shad, *camera);
			}
		}
		for(CGLAutonomous* autonomousPtr : autonomousRenderQueue){
			if(autonomousPtr->renderOrder == RenderOrder::After)
				autonomousPtr->Draw(*camera);
		}
	}
	renderQueue.Clear(); //last we clear queue
	autonomousRenderQueue.Clear();
}



}
