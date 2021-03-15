#ifndef _pluginUFE_OpenGLRenderer_CGLCamera_h_
#define _pluginUFE_OpenGLRenderer_CGLCamera_h_

#include <UFEngine/UFEngine.h>

namespace Upp{

/*
	In UFEngine Component always start by C letter
*/

class CGLCamera : public Component{
	public:
		static const InstantiationConstraint Constraint = InstantiationConstraint::ONE_BY_OBJECT;
		/*
			Many way of working are possible here. I could have just created a
			OpenGLComponentCamera with a glm::mat4 member for each matrix and recalculate it at
			each update. But for now I will create a component with virtual method, allowing
			creation of differente camera by inherrating it. By now, this class is only an
			interface. Lets create a simple perspective camera with FOV of 45 :
		*/
		CGLCamera& operator=(const CGLCamera& cam){
			active = cam.active;
			fov =  cam.fov;
			screenSizeX =  cam.screenSizeX;
			screenSizeY =  cam.screenSizeY;
			drawDistanceMin =  cam.drawDistanceMin;
			drawDistanceMax =  cam.drawDistanceMax;
			return *this;
		}
		
		bool active = true;
		float fov = 45.0f;
		float screenSizeX = 800;
		float screenSizeY = 600;
		float drawDistanceMin = 0.001f;
		float drawDistanceMax = 10000.0f;

		bool IsActive(){return active;}
		CGLCamera& SetActive(bool b = true){active = b; return *this;}
		
		virtual glm::mat4 GetViewMatrix() = 0;
		virtual glm::mat4 GetProjectionMatrix() = 0;
		
		virtual CGLCamera* Clone(Object& object)const = 0;
		
		virtual String ToString()const{
			String toString;
			toString << "Object name: " << GetObject().GetName() << "\n";
			toString << "Camera fov: " << AsString(fov) << "\n";
			toString << "Camera screenSizeX: " << AsString(screenSizeX) << "\n";
			toString << "Camera screenSizeY: " << AsString(screenSizeY) << "\n";
			toString << "Camera drawDistanceMin: " << AsString(drawDistanceMin) << "\n";
			toString << "Camera drawDistanceMax: " << AsString(drawDistanceMax) << "\n";
			return toString;
		}
};

class CGLCameraPerspective : public CGLCamera{
	public:
		CGLCameraPerspective(){}
		CGLCameraPerspective& operator=(const CGLCameraPerspective& cam){
			CGLCamera::operator=(cam);
			return *this;
		}
		
		virtual String GetName()const{
			return "CGLCameraPerspective";
		}
		virtual glm::mat4 GetViewMatrix(){
			glm::mat4 view = glm::lookAt( GetObject().GetTransform().GetPosition() ,  GetObject().GetTransform().GetPosition() + GetObject().GetTransform().GetFront() , GetObject().GetTransform().GetUp());
			//Cout() << String(glm::to_string(view)) << EOL;
			return view;
		}
		virtual glm::mat4 GetProjectionMatrix(){
			glm::mat4 perspective = glm::perspective(glm::radians(float(fov)),screenSizeX /	screenSizeY,drawDistanceMin,drawDistanceMax);
			//Cout() << String(glm::to_string(perspective)) << EOL;
			return perspective;
		}
		virtual Value ReceiveMessage(const String& message,const Value& args = Value()){
			//In case we want to resize the window, we must resize the screensize of camera
			//object, in order to do this, we gonna use ReceiveMessage to receive possible
			//screneSize update
			if(message.IsEqual("ScreenSize")){
				try{
					ValueArray va = args;
					if(va.GetCount()){
						if( va[0].Get<int>() != 0 && va[1].Get<int>() !=0){
							screenSizeX = (float)va[0].Get<int>();
							screenSizeY = (float)va[1].Get<int>();
							return true;
						}
						return false;
					}else{
						LOG("CGLCameraPerspective : ScreenSize error, no args provided");
						return false;
					}
				}catch(Exc& exc){
					LOG("CGLCameraPerspective : " + exc);
				}
			}else if(message.IsEqual("MouseWheel")){
				if(active){
					try{
						if(((double)args) > 0){
							GetObject().GetTransform().Move(GetObject().GetTransform().GetFront());
						}else{
							GetObject().GetTransform().Move(GetObject().GetTransform().GetFront() * -1.0f);
						}
					}catch(Exc& exc){
						LOG("CGLCameraPerspective : " + exc);
					}
				}
			}
			return false;
		}
		
		virtual String ToString()const{
			return CGLCamera::ToString();
		}
	protected:
		virtual CGLCameraPerspective* Clone(Object& object) const{
			CGLCameraPerspective* ptr = new CGLCameraPerspective(*this);
			ptr->SetObject(object);
			return ptr;
		}
		CGLCameraPerspective(const CGLCameraPerspective& cam){
			*this = cam;
		}
};
	
}

#endif
