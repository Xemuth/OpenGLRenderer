#ifndef _pluginUFE_OpenGLRenderer_CGLCylinder_h_
#define _pluginUFE_OpenGLRenderer_CGLCylinder_h_
#include <UFEngine/UFEngine.h>
#include "CGLPolygon.h"

namespace Upp{

class SGLRenderer;

/*
	class below have been taken from http://www.songho.ca/opengl/gl_cylinder.html
	UPP Compliant
*/
class Cylinder : public Polygon {
	public:
		Cylinder(float _baseRadius, float _topRadius, float _height, int _sectorCount, int _stackCount);
		virtual ~Cylinder(){}
		
		virtual bool Compare(float _baseRadius, float _topRadius, float _height, int _sectorCount, int _stackCount);
		virtual Vector<float> GetContiniousData();
		
	private:
		float baseRadius;
		float topRadius;
		float height;
		int sectorCount;
		int stackCount;
		
		unsigned int baseIndex; // starting index of base
		unsigned int topIndex; // starting index of top
		int baseCenterIndex;
		int topCenterIndex;
		
		Vector<unsigned int> indices;
		
		Vector<float> vertices;
	    Vector<float> normals;
	    Vector<float> texCoords;
	    Vector<float> unitCircleVertices;;

		void GenerateCylinder();
		void BuildUnitCircleVertices();
		Vector<float> GetSideNormals();
		//void FillIndicesVector();
		
		void AddVertex(float x, float y, float z);
		void AddNormals(float x, float y, float z);
		void AddTexCoords(float x, float y);
		void AddIndices(unsigned int i1, unsigned int i2, unsigned int i3);
};

class CGLCylinder : public CGLPolygon{
	public:
		virtual String GetName()const{return "CGLCylinder";}
		CGLCylinder(SGLRenderer& _openGLRenderer, float _baseRadius = 1, float _topRadius = 1, float _height =2, int _sectorCount=10, int _stackCount=3);
		CGLCylinder(const CGLCylinder& cglCylinder);
		~CGLCylinder();
	
		virtual void Update(double timeEllapsed, double deltaTime = 0.0);
		
	protected:
		virtual CGLCylinder* Clone(Object& object) const{
			CGLCylinder* ptr = new CGLCylinder(*this);
			ptr->SetObject(object);
			return ptr;
		}

	private:
		String modelName;
		int ID;
		float baseRadius;
		float topRadius;
		float height;
		int sectorCount;
		int stackCount;
		
	inline static int instanceCount = 0;
	inline static Array<PolygonUseCount<Cylinder>>allCylinders;
};

}
#endif
