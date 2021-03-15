#ifndef _pluginUFE_OpenGLRenderer_CGLSkyBox_h_
#define _pluginUFE_OpenGLRenderer_CGLSkyBox_h_
#include <UFEngine/UFEngine.h>
/*
	A skybox component
*/

#include "SGLRenderer.h"
#include "CGLRoutineBeforeAfterRendering.h"

namespace Upp{
	
	class CGLSkyBox : public CGLRoutineBeforeAfterRendering{
		public :
			bool active = true;
			CGLSkyBox(){}
			~CGLSkyBox(){
				if(loaded){
					SGLRenderer& sglService = GetObject().GetScene().GetContext().GetServiceManager().Get<SGLRenderer>();
					sglService.GetShaderManager().Remove(GetShaderName());
					sglService.GetTextureManager().Remove(GetTextureName());
					sglService.GetModelManager().Remove(GetModelName());
					sglService.GetRendererManager().Remove(GetRendererName());
				}
			}
					
			Image skyboxRight;
			Image skyboxLeft;
			Image skyboxTop;
			Image skyboxBottom;
			Image skyboxFront;
			Image skyboxBack;
			
			bool IsActive(){return active;}
			CGLSkyBox& SetActive(bool b = true){active = b; return *this;}
			
			String GetRendererName()const{
				return Replace(rendererName,Vector<String>{"%SCENE_NAME%"},Vector<String>{GetObject().GetScene().GetName()});
			}
			
			String GetTextureName()const{
				return Replace(textureName,Vector<String>{"%SCENE_NAME%"},Vector<String>{GetObject().GetScene().GetName()});
			}
			
			String GetModelName()const{
				return Replace(modelName,Vector<String>{"%SCENE_NAME%"},Vector<String>{GetObject().GetScene().GetName()});
			}
			
			String GetShaderName()const {
				return Replace(shaderName,Vector<String>{"%SCENE_NAME%"},Vector<String>{GetObject().GetScene().GetName()});
			}
			
			
			bool LoadSkybox(){
				if(!loaded){
					if( skyboxRight && skyboxLeft && skyboxTop && skyboxBottom && skyboxFront && skyboxBack ){
						try{
							if(GetObject().GetScene().GetContext().GetServiceManager().Has<SGLRenderer>()){
								SGLRenderer& sglService = GetObject().GetScene().GetContext().GetServiceManager().Get<SGLRenderer>();
								
								//First we create Model (the cube that will be used to map texture)
								Model& mesh = sglService.GetModelManager().Create(GetModelName());
								mesh.AddVertices(cubeMapData,cubeMapData.GetCount()/3);
								mesh.Load();
								
								//Now we load texture
								Texture& texture = sglService.GetTextureManager().Create(GetTextureName());
								texture.CreateTexture(GL_TEXTURE_CUBE_MAP);
								Vector<TextureLoaderParameters> test;
								test.Add(TextureLoaderParameters(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
								test.Add(TextureLoaderParameters(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
								test.Add(TextureLoaderParameters(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
								test.Add(TextureLoaderParameters(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
								test.Add(TextureLoaderParameters(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
								texture.LoadParameters(test);
								texture.LoadImages(Vector<Image>{skyboxRight,skyboxLeft,skyboxTop,skyboxBottom,skyboxFront,skyboxBack},GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, GL_BGRA, GL_UNSIGNED_BYTE, false);
							 
								//Now we load the Renderer
								Renderer& renderer = sglService.GetRendererManager().Create(GetRendererName());
								renderer.GetVertexArrayAttribFormat().Add(VAAF(0,3, GL_FLOAT, GL_FALSE, 0* sizeof(float)));
								renderer.LoadVAO();
								
								ShaderProgram& shader = sglService.GetShaderManager().Create(GetShaderName());
								try{
									shader.LoadShaderFromString(vertexCubeMap,Upp::ShaderType::VERTEX);
									shader.LoadShaderFromString(fragmentCubeMap,Upp::ShaderType::FRAGMENT);
									shader.Link();
									
									shader.SetUniformProjectionMatrixName("ProjectionMatrix");
									shader.SetUniformViewMatrixName("ViewMatrix");
									
									shader.beforeRendering = [&](Upp::ShaderProgram& shader, Upp::CGLCamera& camera){
										shader.Bind();
										shader.UniformMat4(shader.uniformViewMatrixName,glm::mat4(glm::mat3(camera.GetViewMatrix())));
										shader.UniformMat4(shader.uniformProjectionMatrixName,camera.GetProjectionMatrix());
									};
									
								}catch(Upp::Exc& e){
									Upp::Cout() << e << Upp::EOL;
								}
								
								SetBeforeRendering(
									[&](Renderer& renderer, ShaderProgram& shader, CGLCamera& camera, Object& object) -> void{
										try{
											
											SGLRenderer& sglService = GetObject().GetScene().GetContext().GetServiceManager().Get<SGLRenderer>();
											Texture& texture = sglService.GetTextureManager().Get(GetTextureName());
											shader.UniformInt("skybox",1);
											glActiveTexture(GL_TEXTURE1);
											texture.Bind();
											
											glDepthFunc(GL_LEQUAL);
											
										}catch(Exc& exception){
											LOG(exception);
										}
									}
								).SetAfterRendering(
									[&](Renderer& renderer, ShaderProgram& shader, CGLCamera& camera, Object& object) -> void{
										glDepthFunc(GL_LESS); // set depth function back to default
									}
								);
								
								loaded = true;
								return true;
							}
							LOG("GLCSkybox component : Can't load skybox, SGLRenderer (OpenGLRenderer service) don't exist");
							return false;
						}catch(Exc& exception){
							LOG(exception);
							return false;
						}
					}
					return false;
				}
				return false;
			 }
			
			
			void Update(double timeEllapsed, double deltaTime = 0.0){
				//if loaded then we add this skybox to the renderQueue
				if(loaded && active){
					if(GetObject().GetScene().GetContext().GetServiceManager().Has<SGLRenderer>()){
						SGLRenderer& sglService = GetObject().GetScene().GetContext().GetServiceManager().Get<SGLRenderer>();
						try{
							sglService.AddToRenderQueue(
								sglService.GetRendererManager().Get(GetRendererName()),
								sglService.GetShaderManager().Get(GetShaderName()),
								sglService.GetModelManager().Get(GetModelName()),
								GetObject()
							);
						}catch(Exc& exception){
							LOG(exception);
						}
					}
				}
			}

			
			static const InstantiationConstraint Constraint = InstantiationConstraint::ONE_BY_SCENE;
			virtual InstantiationConstraint GetConstraint()const{return Constraint;}


			virtual String GetName()const{
				return "GLCSkyBox";
			}
	
	
			virtual String ToString()const{
				String toString;
				toString << "Object name: " << GetObject().GetName() << "\n";
				toString << "Skybox loaded: " << loaded << "\n";
				
				int imgCpt = 0;
				if(skyboxRight) imgCpt++;
				if(skyboxLeft) imgCpt++;
				if(skyboxTop) imgCpt++;
				if(skyboxBottom) imgCpt++;
				if(skyboxFront) imgCpt++;
				if(skyboxBack) imgCpt++;
				
				toString << AsString(imgCpt) <<  " / 6 textures loaded" << "\n";
				
				String toString2;
				try{
					const SGLRenderer& service = GetObject().GetScene().GetContext().GetServiceManager().Get<SGLRenderer>();
					toString2 << "Model name : " << GetModelName() << "Status: " << "valide\n\t" << Replace(service.GetModelManager().Get(GetModelName()).ToString(), Vector<String>{"\n"}, Vector<String>{"\n\t"}) << "\n";
					toString2 << "Texture name : " << GetTextureName() << "Status: " << "valide\n\t" << Replace(service.GetTextureManager().Get(GetTextureName()).ToString(), Vector<String>{"\n"}, Vector<String>{"\n\t"})<< "\n";
					toString2 << "Renderer name : " << GetRendererName() << "Status: " << "valide\n\t" << Replace(service.GetRendererManager().Get(GetRendererName()).ToString(), Vector<String>{"\n"}, Vector<String>{"\n\t"}) << "\n";
					toString2 << "Shader name : " << GetShaderName() << "Status: " << "valide\n\t" << Replace(service.GetShaderManager().Get(GetShaderName()).ToString(), Vector<String>{"\n"}, Vector<String>{"\n\t"}) << "\n";
				}catch(Exc& exception){
					toString2 ="";
					toString2 << "This skybox is invalide, Exception raised: " << exception;
				}
				toString << toString2;
				toString << Replace(CGLRoutineBeforeAfterRendering::ToString(), Vector<String>{"Object name: " << GetObject().GetName() << "\n"}, Vector<String>{""});
				return toString;
			}
		protected:
			virtual CGLSkyBox* Clone(Object& object) const{
				CGLSkyBox* ptr = new CGLSkyBox(*this);
				ptr->SetObject(object);
				if(loaded) ptr->LoadSkybox();
				return ptr;
			}
			CGLSkyBox(const CGLSkyBox& sky) : CGLRoutineBeforeAfterRendering(sky){
				active = sky.active;
				skyboxRight = sky.skyboxRight;
				skyboxLeft = sky.skyboxLeft;
				skyboxTop = sky.skyboxTop;
				skyboxBottom = sky.skyboxBottom;
				skyboxFront = sky.skyboxFront;
				skyboxBack = sky.skyboxBack;
			}
	
		private:
			String rendererName = "SkyboxRenderer%SCENE_NAME%";
			String textureName = "SkyboxTexture%SCENE_NAME%";
			String modelName = "SkyboxModel%SCENE_NAME%";
			String shaderName ="SkyboxShader%SCENE_NAME%";
			
			bool loaded = false;
			
			//This cubeMap data are used to create a cube that will take the entire screen
			static const Vector<float> cubeMapData;
			static const String vertexCubeMap;
			static const String fragmentCubeMap;
	};
	
	
inline const Vector<float> CGLSkyBox::cubeMapData{
		-1.0f ,  1.0f , -1.0f, -1.0f , -1.0f , -1.0f, 1.0f , -1.0f , -1.0f,
		1.0f , -1.0f , -1.0f, 1.0f ,  1.0f , -1.0f, -1.0f ,  1.0f , -1.0f,
		-1.0f , -1.0f ,  1.0f, -1.0f , -1.0f , -1.0f, -1.0f ,  1.0f , -1.0f,
		-1.0f ,  1.0f , -1.0f, -1.0f ,  1.0f ,  1.0f, -1.0f , -1.0f ,  1.0f,
		1.0f , -1.0f , -1.0f, 1.0f , -1.0f ,  1.0f, 1.0f ,  1.0f ,  1.0f,
		1.0f ,  1.0f ,  1.0f, 1.0f ,  1.0f , -1.0f, 1.0f , -1.0f , -1.0f,
		-1.0f , -1.0f ,  1.0f, -1.0f ,  1.0f ,  1.0f, 1.0f ,  1.0f ,  1.0f,
		1.0f ,  1.0f ,  1.0f, 1.0f , -1.0f ,  1.0f, -1.0f , -1.0f ,  1.0f,
		-1.0f ,  1.0f , -1.0f, 1.0f ,  1.0f , -1.0f, 1.0f ,  1.0f ,  1.0f,
		1.0f ,  1.0f ,  1.0f, -1.0f ,  1.0f ,  1.0f, -1.0f ,  1.0f , -1.0f,
		-1.0f , -1.0f , -1.0f, -1.0f , -1.0f ,  1.0f, 1.0f , -1.0f , -1.0f,
		1.0f , -1.0f , -1.0f, -1.0f , -1.0f ,  1.0f, 1.0f , -1.0f ,  1.0f
	};
	
inline const String CGLSkyBox::vertexCubeMap = "#version 400 core\n\nin vec3 aPos;in vec3 normals; in vec3 colors; in vec2 texCoords;\r\n\r\n\tout vec3 fs_texCoord;\r\n\r\n\tuniform mat4 ProjectionMatrix;\r\n    uniform mat4 ViewMatrix;\r\n\r\n    void main()\r\n    {\r\n\t\tfs_texCoord = aPos;\r\n\t    vec4 pos = ProjectionMatrix * ViewMatrix * vec4(aPos, 1.0);\r\n\t    gl_Position = pos.xyww;\r\n}";
inline const String CGLSkyBox::fragmentCubeMap = "#version 400 core\n\nin vec3 fs_texCoord;\r\n\r\nout vec4 FragColor;\r\n\r\nuniform samplerCube skybox;\r\n\r\nvoid main(){\r\n    FragColor = texture(skybox, fs_texCoord);\r\n}";

}
#endif
