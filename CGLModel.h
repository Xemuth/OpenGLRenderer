#ifndef _pluginUFE_OpenGLRenderer_CGLModel_h_
#define _pluginUFE_OpenGLRenderer_CGLModel_h_

#include <UFEngine/UFEngine.h>

namespace Upp{

/*
	In UFEngine Component always start by C letter
*/

class CGLModel : public Component{
	public:
		CGLModel(){}
		CGLModel(const CGLModel& cglModel){
			*this = cglModel;
		}
		CGLModel& operator=(const CGLModel& cglModel){
			model = cglModel.model;
			return *this;
		}
		
		String model;
		CGLModel& SetModel(const String& modelName){model = modelName; return *this;}
		
		virtual String ToString()const{
			String toString;
			toString << "Object name: " << GetObject().GetName() << "\n";
			toString << "Model name: " << model << "\n";
			toString << "Purpose: Bind a model (meshs data) to an object\n";
			String toString2;
			try{
				const SGLRenderer& service = GetObject().GetScene().GetContext().GetServiceManager().Get<SGLRenderer>();
				toString2 << "This model is valide: \n\t" << Replace(service.GetModelManager().Get(model).ToString(), Vector<String>{"\n"}, Vector<String>{"\n\t"});
			}catch(Exc& exception){
				toString2 ="";
				toString2 << "This model is invalide, Exception raised: " << exception;
			}
			toString << toString2 << "\n";
			return toString;
		}
		
		virtual String GetName()const{
			return "CGLModel";
		}
		
	protected:
		virtual CGLModel* Clone(Object& object) const{
			CGLModel* ptr = new CGLModel(*this);
			ptr->SetObject(object);
			return ptr;
		}
};

}


#endif
