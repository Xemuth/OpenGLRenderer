#ifndef _pluginUFE_OpenGLRenderer_Texture_h_
#define _pluginUFE_OpenGLRenderer_Texture_h_

#include <Core/Core.h>
#include <Draw/Draw.h>

namespace Upp{

struct TextureLoaderParameters : public Moveable<TextureLoaderParameters>{
	public:
		GLenum target;
		GLenum pname;
		GLint param;
		
		TextureLoaderParameters(){
		
		}
		
		TextureLoaderParameters(GLenum _target, GLenum _pname, GLint _param){
			target = _target; pname = _pname; param = _param;
		}
				
		TextureLoaderParameters(TextureLoaderParameters&& params){
			*this = pick(params);
		}
		
		TextureLoaderParameters& operator=(TextureLoaderParameters&& params){
			target = params.target;
			pname = params.pname;
			param = params.param;
			return *this;
		}
		
		TextureLoaderParameters(const TextureLoaderParameters& params){
			*this = clone(params);
		}
		
		TextureLoaderParameters& operator=(const TextureLoaderParameters& params){
			target = params.target;
			pname = params.pname;
			param = params.param;
			return *this;
		}
};

class Texture : public Moveable<Texture>{
	public:
		Texture(){}
		Texture(const Image& img,GLenum target = GL_TEXTURE_2D, bool mipmap = true, const
			Vector<TextureLoaderParameters>& loaderParameters = Vector<TextureLoaderParameters>()){
			Load(img,target,mipmap,loaderParameters);
		}
		
		Texture(Texture&& texture){
			*this = pick(texture);
		}
		
		Texture& operator=(Texture&& texture){
			loaded = texture.loaded;
			ID = texture.ID;
			target = texture.target;
			
			texture.loaded = false;
			texture.ID = 0;
			texture.target = 0;
			return *this;
		}
		
		~Texture(){
			if(loaded) glDeleteTextures(1,&ID);
		}
		
		Texture& Clear(){
			if(loaded) glDeleteTextures(1,&ID);
			ID = 0;
			target = 0;
			loaded = false;
			return *this;
		}
		
		GLuint GetID()const{return ID;}
		GLenum GetTarget()const{return target;}
		
		bool IsLoaded()const{return loaded;}

		Texture& Bind(){if(loaded)glBindTexture(target, ID); return *this;}
		Texture& Unbind(){glBindTexture(target, 0); return *this;}
	
		bool Load(const Image& img,GLenum _target = GL_TEXTURE_2D, bool mipmap = true, const Vector<TextureLoaderParameters>& loaderParameters = Vector<TextureLoaderParameters>()){
			if(!loaded && img){
				Vector<TextureLoaderParameters> params;
				if(loaderParameters.GetCount() == 0) params.Append(pick(GetDefaultLoaderParameters()));
				else params.Append(pick(loaderParameters));

				CreateTexture(_target);
				LoadParameters(params);
				Image flippedImage = FlipImage(img,FLIP_MIRROR_VERT);

				return LoadImages(Vector<Image>{flippedImage},target, 0, GL_RGBA, GL_BGRA, GL_UNSIGNED_BYTE, mipmap);
			}
			return false;
		}
		
		void CreateTexture(GLenum _target){
			glGenTextures(1, &ID);
			target = _target;
			glBindTexture(target, ID);
		}
		void LoadParameters(const Vector<TextureLoaderParameters>& loaderParameters){
			if(ID != 0){
				glBindTexture(target, ID);
				for(const TextureLoaderParameters& loaderParam : loaderParameters){
					glTexParameteri(loaderParam.target, loaderParam.pname, loaderParam.param);
				}
			}
		}
		bool LoadImages(const Vector<Image>& images, GLenum target, GLint level, GLint internalFormat, GLint format, GLenum type, bool mipmap){
			if(!loaded){
				if(ID != 0){
					for(int i = 0; i < images.GetCount(); i++){
						glTexImage2D(target + i, level, internalFormat, images[i].GetSize().cx, images[i].GetSize().cy,0 , format, type, ~(images[i]));
					}
					if(mipmap) glGenerateMipmap(target);
					loaded = true;
				}
				return loaded;
			}
			return false;
		}
		
		const String ToString()const{
			String toString;
			toString << "texture ID: " << AsString(ID) << " on target no " << AsString(target)<<  " is " << ((loaded)? "":"not") << "loaded";
			return toString;
		}
	
	private:
		bool loaded = false;
		GLuint ID = 0;
		GLenum target = 0;
		
		Vector<TextureLoaderParameters> GetDefaultLoaderParameters(){
			Vector<TextureLoaderParameters> test;
			test.Add() = TextureLoaderParameters(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			test.Add() = TextureLoaderParameters(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			test.Add() = TextureLoaderParameters(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			test.Add() = TextureLoaderParameters(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			return test;
		}
		
};

}
#endif
	