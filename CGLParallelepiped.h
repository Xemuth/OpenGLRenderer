#ifndef _pluginUFE_OpenGLRenderer_CGLParallelepiped_h_
#define _pluginUFE_OpenGLRenderer_CGLParallelepiped_h_
#include <UFEngine/UFEngine.h>
#include "CGLPolygon.h"

namespace Upp{

class Parallelepiped : public Polygon{
	public:
		Parallelepiped(float _length, float _width, float _height, float angleDegree, int _sectorCount);
		virtual ~Parallelepiped();
		virtual bool Compare(float _length, float _width, float _height, float _angleDegree, int _sectorCount);
		virtual Vector<float> GetContiniousData();
	private:
		float length;
		float width;
		float height;
		float angleDegree;
		int sectorCount;
		
		Vector<float> datas;
		
		void GenerateParallelepiped();
		void InsertVec3(glm::vec3 p);
		void InsertVec2(glm::vec2 p);
		void GenerateData(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, int _sectorCount, glm::vec3 position);
		glm::vec3 RotateAVector(glm::vec3 vector, float angleInDregree);
};

class CGLParallelepiped : public CGLPolygon{
	public:
		virtual String GetName()const{return "CGLParallelepiped";}
		CGLParallelepiped(SGLRenderer& _openGLRenderer, float _length = 1.0f, float _height = 1.0f, float _width = 1.0f, float _angleDegree = 0, int _sectorCount = 1);
		CGLParallelepiped(const CGLParallelepiped& cglParallepiped);
		~CGLParallelepiped();
		
		virtual void Update(double timeEllapsed, double deltaTime = 0.0);
	protected:
		virtual CGLParallelepiped* Clone(Object& object) const{
			CGLParallelepiped* ptr = new CGLParallelepiped(*this);
			ptr->SetObject(object);
			return ptr;
		}
	private:
		String modelName;
		int ID;
		float length;
		float width;
		float height;
		float angleDegree;
		int sectorCount;
		
	inline static int instanceCount = 0;
	inline static Array<PolygonUseCount<Parallelepiped>> allParallelepiped;
};

}

#endif
