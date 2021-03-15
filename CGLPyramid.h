#ifndef _pluginUFE_OpenGLRenderer_CGLPyramid_h_
#define _pluginUFE_OpenGLRenderer_CGLPyramid_h_
#include <UFEngine/UFEngine.h>
#include "CGLPolygon.h"

namespace Upp{

class SGLRenderer;
/*
class CGLPyramid : public CGLPolygon{
	public:
		virtual String GetName()const{return "CGLPyramid";}
		CGLPyramid(SGLRenderer& _openGLRenderer);
};*/
class Pyramid : public Polygon{
	public:
		Pyramid(const Array<glm::vec3>& _basePoints, const Array<glm::vec3>& _apexPoints);
		virtual ~Pyramid();
		virtual bool Compare(const Array<glm::vec3>& _basePoints, const Array<glm::vec3>& _apexPoints);
		virtual Vector<float> GetContiniousData();
	private:
		Array<glm::vec3> apexPoint;
		Array<glm::vec3> basePoints;
		
		Vector<float> datas;
		
		void GeneratePyramid();
		void InsertVec3(glm::vec3 p);
		void InsertVec2(glm::vec2 p);
};

class CGLPyramid : public CGLPolygon{
	public:
		virtual String GetName()const{return "CGLPyramid";}
		
		CGLPyramid(SGLRenderer& _openGLRenderer, const Array<glm::vec3>& _basePoints, const glm::vec3& _apexPoints);
		CGLPyramid(SGLRenderer& _openGLRenderer, unsigned int basePointCount, float apexHeight, float pyramidRadius);
		CGLPyramid(SGLRenderer& _openGLRenderer, const Array<glm::vec3>& _basePoints, float angleDegree, float Height);
		
		
		CGLPyramid(const CGLPyramid& cglParallepiped);
		~CGLPyramid();
		
		virtual void Update(double timeEllapsed, double deltaTime = 0.0);
	protected:
		virtual CGLPyramid* Clone(Object& object) const{
			CGLPyramid* ptr = new CGLPyramid(*this);
			ptr->SetObject(object);
			return ptr;
		}
	private:
		void CreateShaderRenderer();
		
		String modelName;
		int ID;
			
		Array<glm::vec3> apexPoints;
		Array<glm::vec3> basePoints;
		
	inline static int instanceCount = 0;
	inline static Array<PolygonUseCount<Pyramid>> allPyramids;
};

}
#endif
