#include "../CGLPyramid.h"
#include "Util.h"
namespace Upp{
	/*
CGLPyramid::CGLPyramid(SGLRenderer& _openGLRenderer) : CGLPolygon(_openGLRenderer){
	ONCELOCK{
		CreateRenderer(openGLRenderer);
		CreateShader(openGLRenderer);
		if(! openGLRenderer.GetModelManager().Has(prefixName + GetName())){
			Model& model = openGLRenderer.GetModelManager().Create(prefixName + GetName());
			model.AddVertices(pyramidVertices,18);
			model.Load();
		}
	}
	beforeRendering = [&](Renderer& renderer, ShaderProgram& shader, CGLCamera& camera, Object& object) -> void{
		try{
			shader.UniformMat4("model",object.GetTransform().GetModelMatrix());
		}catch(Exc& exception){
			LOG("Pyramid component beforeRendering failled in object named '"+ GetObject().GetName() +"' : " + exception );
		}
	};
}
*/
void CGLPyramid::CreateShaderRenderer(){
	ONCELOCK{
		CreateRenderer(openGLRenderer);
		CreateShader(openGLRenderer);
	}
}

CGLPyramid::CGLPyramid(SGLRenderer& _openGLRenderer, const Array<glm::vec3>& _basePoints, const glm::vec3& _apexPoint) : CGLPolygon(_openGLRenderer){
	instanceCount++;
	ID = instanceCount;
	
	apexPoints.Create<glm::vec3>(_apexPoint);
	basePoints.Append(clone(_basePoints));
	
	CreateShaderRenderer();
	bool found = false;
	for(PolygonUseCount<Pyramid>& cuc : allPyramids){
		if(cuc.polygon.Compare(basePoints, apexPoints)){
			found = true;
			modelName = cuc.ModelName;
			cuc.count++;
			break;
		}
	}
	if(!found){
		PolygonUseCount<Pyramid>& cuc = allPyramids.Create<PolygonUseCount<Pyramid>>(basePoints, apexPoints);
		Model& model = openGLRenderer.GetModelManager().Create(prefixName + GetName() + AsString(ID));
		Vector<float> datas = pick(cuc.polygon.GetContiniousData());
		model.AddVertices(datas, datas.GetCount()/3);
		model.Load();
		cuc.ModelName = prefixName + GetName() + AsString(ID);
		modelName = cuc.ModelName;
	}
}
CGLPyramid::CGLPyramid(SGLRenderer& _openGLRenderer, unsigned int basePointCount, float apexHeight, float pyramidRadius) : CGLPolygon(_openGLRenderer){
	
}
CGLPyramid::CGLPyramid(SGLRenderer& _openGLRenderer, const Array<glm::vec3>& _basePoints, float angleDegree, float Height) : CGLPolygon(_openGLRenderer){
	
}
CGLPyramid::CGLPyramid(const CGLPyramid& cglParallepiped) : CGLPolygon(cglParallepiped){
	
}
CGLPyramid::~CGLPyramid(){
	
}
void CGLPyramid::Update(double timeEllapsed, double deltaTime){
	
}

Pyramid::Pyramid(const Array<glm::vec3>& _basePoints, const Array<glm::vec3>& _apexPoints){
	
}
Pyramid::~Pyramid(){
	
}
bool Pyramid::Compare(const Array<glm::vec3>& _basePoints, const Array<glm::vec3>& _apexPoints){
	return false;
}
Vector<float> Pyramid::GetContiniousData(){
	return Vector<float>{};
}

}