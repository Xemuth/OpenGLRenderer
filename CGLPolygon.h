#ifndef _pluginUFE_OpenGLRenderer_CGLPolygon_h_
#define _pluginUFE_OpenGLRenderer_CGLPolygon_h_
#include <UFEngine/UFEngine.h>
#include "CGLRoutineBeforeAfterRendering.h"
#include "Definition.h"

namespace Upp{

class SGLRenderer;


class Polygon{
	public:
		virtual bool Compare(const Polygon& p){return false;}
		virtual Vector<float> GetContiniousData() = 0;
		virtual ~Polygon(){}
};

template<class T>
struct PolygonUseCount{
	template<class... Args>
	PolygonUseCount(Args&&... args) : polygon(std::forward<Args>(args)...){
		count = 1;
		ModelName = "";
	}
	virtual ~PolygonUseCount(){}
	int count;
	T polygon;
	String ModelName;
};

class CGLPolygon : public CGLRoutineBeforeAfterRendering{
	public:
		DrawType drawType = DrawType::solid;
		CGLPolygon& operator=(const CGLPolygon& polygon);
		
		virtual CGLPolygon& SetDrawType(DrawType _drawType){drawType = _drawType; return *this;}
		
		virtual String GetName()const = 0;
		virtual String ToString()const;

		virtual void Update(double timeEllapsed, double deltaTime = 0.0);

		CGLPolygon& SetCustomShader(const String& shader){customShader = shader; return *this;}
		String GetCustomShader(){return customShader;}
		
		CGLPolygon& SetCustomRenderer(const String& renderer){customRenderer = renderer; return *this;}
		String GetCustomRenderer(){return customRenderer;}
		
	protected:
		CGLPolygon(SGLRenderer& _openGLRenderer);
		CGLPolygon(const CGLPolygon& polygon);

		String customRenderer = "";
		String customShader = "";
		SGLRenderer& openGLRenderer;
		
		bool picked = false;
};

}
#endif
