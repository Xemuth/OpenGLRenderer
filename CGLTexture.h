#ifndef _pluginUFE_OpenGLRenderer_CGLTexture_h_
#define _pluginUFE_OpenGLRenderer_CGLTexture_h_
#include <UFEngine/UFEngine.h>

#include "OpenGLRenderer.h"

namespace Upp{

/*
	In UFEngine Component always start by C letter
*/

class CGLTexture : public CGLRoutineBeforeAfterRendering{
	public:
		CGLTexture(){
			beforeRendering = [&](Renderer& renderer, ShaderProgram& shader, CGLCamera& camera, Object& object) -> void{
				try{
					if(shader.UniformExist(uniformName)){
						SGLRenderer& service = GetObject().GetScene().GetContext().GetServiceManager().Get<SGLRenderer>();
						Texture& tex = service.GetTextureManager().Get(texture);
						glActiveTexture(GL_TEXTURE0 + channelToUse);
						tex.Bind();
						shader.UniformInt(uniformName,(int)channelToUse);
					}
				}catch(Exc& exception){
					LOG("Texture component beforeRendering failled in object named '"+ GetObject().GetName() +"' : " + exception );
				}
			};
		}
		
		CGLTexture(const CGLTexture& cglTexture){
			*this = cglTexture;
		}
		CGLTexture& operator=(const CGLTexture& cglTexture){
			CGLRoutineBeforeAfterRendering::operator=(cglTexture);
			texture = cglTexture.texture;
			channelToUse = cglTexture.channelToUse;
			uniformName = cglTexture.uniformName;
			return *this;
		}
		CGLTexture(CGLTexture&& cglTexture){
			*this = pick(cglTexture);
		}
		CGLTexture& operator=(CGLTexture&& cglTexture){
			CGLRoutineBeforeAfterRendering::operator=(pick(cglTexture));
			texture = cglTexture.texture;
			channelToUse = cglTexture.channelToUse;
			uniformName = cglTexture.uniformName;
			return *this;
		}
		
		
		String texture;
		unsigned int channelToUse = 0;
		String uniformName = "";
		
		CGLTexture& SetTexture(const String& textureName){texture = textureName; return *this;}
		CGLTexture& SetChannelToUse(unsigned int channel){channelToUse = channel; return *this;}
		CGLTexture& SetUniformName(const String& name){uniformName = name; return *this;}
		
		virtual String ToString()const{
			String toString;
			toString << "Object name: " << GetObject().GetName() << "\n";
			toString << "Uniform name: " << uniformName << "\n";
			toString << "Purpose: bind a texture to an Object\n";
			toString << "Actual binded texture: " << texture << "\n";
			String toString2;
			try{
				const SGLRenderer& service = GetObject().GetScene().GetContext().GetServiceManager().Get<SGLRenderer>();
				toString2 << "This texture is valide: " << service.GetTextureManager().Get(texture).ToString();
			}catch(Exc& exception){
				toString2 ="";
				toString2 << "This texture is invalide, Exception raised: " << exception;
			}
			toString << toString2 <<"\n";
			toString << Replace(CGLRoutineBeforeAfterRendering::ToString(), Vector<String>{"Object name: " << GetObject().GetName() << "\n"}, Vector<String>{""});
			return toString;
		}
		
		virtual String GetName()const{
			return "CGLTexture";
		}
	protected:
		virtual CGLTexture* Clone(Object& object) const{
			CGLTexture* ptr = new CGLTexture(*this);
			ptr->SetObject(object);
			return ptr;
		}
};

}
#endif
