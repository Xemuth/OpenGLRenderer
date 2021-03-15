#include "../CGLParallelepiped.h"
#include "Util.h"
#include "../OpenGLRenderer.h"

namespace Upp{
	
/*
CGLParallelepiped::CGLParallelepiped(SGLRenderer& _openGLRenderer) : CGLPolygon(_openGLRenderer){
	ONCELOCK{
		CreateRenderer(openGLRenderer);
		CreateShader(openGLRenderer);
		if(! openGLRenderer.GetModelManager().Has(prefixName + GetName())){
			Model& model = openGLRenderer.GetModelManager().Create(prefixName + GetName());
			model.AddVertices(cubeVertices,36);
			model.Load();
		}
	}
	beforeRendering = [&](Renderer& renderer, ShaderProgram& shader, CGLCamera& camera, Object& object) -> void{
		try{
			shader.UniformMat4("model",object.GetTransform().GetModelMatrix());
		}catch(Exc& exception){
			LOG("Cube component beforeRendering failled in object named '"+ GetObject().GetName() +"' : " + exception );
		}
	};
}
*/


CGLParallelepiped::CGLParallelepiped(SGLRenderer& _openGLRenderer, float _length, float _width, float _height, float _angleDegree, int _sectorCount) : CGLPolygon(_openGLRenderer), length(_length), width(_width), height(_height), angleDegree(_angleDegree), sectorCount(_sectorCount){
	instanceCount++;
	ID = instanceCount;
	ONCELOCK{
		CreateRenderer(openGLRenderer);
		CreateShader(openGLRenderer);
	}
	bool found = false;
	for(PolygonUseCount<Parallelepiped>& cuc : allParallelepiped){
		if(cuc.polygon.Compare(length, width, height, angleDegree, sectorCount)){
			found = true;
			modelName = cuc.ModelName;
			cuc.count++;
			break;
		}
	}
	if(!found){
		PolygonUseCount<Parallelepiped>& cuc = allParallelepiped.Create<PolygonUseCount<Parallelepiped>>(length, width, height, angleDegree, sectorCount);
		Model& model = openGLRenderer.GetModelManager().Create(prefixName + GetName() + AsString(ID));
		Vector<float> datas = pick(cuc.polygon.GetContiniousData());
		model.AddVertices(datas, datas.GetCount()/3);
		model.Load();
		cuc.ModelName = prefixName + GetName() + AsString(ID);
		modelName = cuc.ModelName;
	}
}

CGLParallelepiped::CGLParallelepiped(const CGLParallelepiped& cglParallepiped): CGLPolygon(cglParallepiped), length(cglParallepiped.length), width(cglParallepiped.width), height(cglParallepiped.height), angleDegree(cglParallepiped.angleDegree), sectorCount(cglParallepiped.sectorCount){
	instanceCount++;
	ID = instanceCount;
	for(PolygonUseCount<Parallelepiped>& cuc : allParallelepiped){
		if(cuc.polygon.Compare(length, width, height, angleDegree, sectorCount)){
			modelName = cuc.ModelName;
			cuc.count++;
			break;
		}
	}
}

CGLParallelepiped::~CGLParallelepiped(){
	if(!picked){
		for(int e = 0; e < allParallelepiped.GetCount(); e++){
			PolygonUseCount<Parallelepiped>& cuc = allParallelepiped[e];
			if(cuc.polygon.Compare(length, width, height, angleDegree, sectorCount)){
				cuc.count--;
				if(cuc.count == 0){
					openGLRenderer.GetModelManager().Remove(cuc.ModelName);
					allParallelepiped.Remove(e);
				}
				break;
			}
		}
	}
}

void CGLParallelepiped::Update(double timeEllapsed, double deltaTime){
	String shader = shaderName + ConvertDrawType(drawType);
	if(customShader.GetCount()){
		shader = customShader;
	}else if(GetObject().GetComponentManager().Has<Upp::CGLShader>()){
		shader = GetObject().GetComponentManager().Get<Upp::CGLShader>().GetName();
	}
	
	String renderer = rendererName;
	if(customRenderer.GetCount()) renderer = customRenderer;
	
	openGLRenderer.AddToRenderQueue(
		openGLRenderer.GetRendererManager().Get(renderer),
		openGLRenderer.GetShaderManager().Get(shader),
	    openGLRenderer.GetModelManager().Get(modelName),
	    GetObject()
	);
}

Parallelepiped::Parallelepiped(float _length, float _height, float _width, float _angleDegree, int _sectorCount) : length(_length), width(_width), height(_height), angleDegree(_angleDegree * -1.0f), sectorCount(_sectorCount) {
	GenerateParallelepiped();
}
Parallelepiped::~Parallelepiped(){}

void Parallelepiped::GenerateParallelepiped(){
	glm::vec3 p1(-(length/2), -(height/2),-(width/2));

	glm::vec3 p2 = p1 + glm::vec3(length, 0.0f, 0.0f);
	glm::vec3 p3 = p2 + glm::vec3(0.0f, 0.0f, width);
	glm::vec3 p4 = p1 + glm::vec3(0.0f, 0.0f, width);

	glm::vec3 rotation = glm::rotateZ(glm::vec3(0.0f, height, 0.0f),glm::radians(angleDegree));
	glm::vec3 p5 = p1 + rotation;
	glm::vec3 p6 = p2 + rotation;
	glm::vec3 p7 = p3 + rotation;
	glm::vec3 p8 = p4 + rotation;
	
	GenerateData(p1,p2,p5,p6,sectorCount, glm::vec3(1.0f,0.0f,0.0f));
	GenerateData(p1,p2,p4,p3,sectorCount, glm::vec3(1.0f,0.0f,0.0f));
	GenerateData(p4,p3,p8,p7,sectorCount, glm::vec3(1.0f,0.0f,0.0f));
	GenerateData(p5,p6,p8,p7,sectorCount, glm::vec3(1.0f,0.0f,0.0f));
	GenerateData(p1,p4,p5,p8,sectorCount, glm::vec3(0.0f,0.0f,1.0f));
	GenerateData(p2,p3,p6,p7,sectorCount, glm::vec3(0.0f,0.0f,1.0f));
}

void Parallelepiped::GenerateData(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4 ,int _sectorCount, glm::vec3 position){
	glm::vec3 pBuffer1;
	glm::vec3 pBuffer2;
	pBuffer1 = p1;
	pBuffer2 = p3;
	
	glm::vec2 pTexC1(0.0f,0.0f);
	glm::vec2 pTexC2(1.0f,0.0f);
	glm::vec2 pTexC3(0.0f,1.0f);
	glm::vec2 pTexC4(1.0f,1.0f);
	
	glm::vec2 pTexCBuffer1;
	glm::vec2 pTexCBuffer2;
	pTexCBuffer1 = pTexC1;
	pTexCBuffer2 = pTexC3;
	
	glm::vec3 padding = (position * (p2 - p1))/(float)(_sectorCount);
	glm::vec2 paddingTexC = pTexC2 / (float)_sectorCount;
	
	for(int e = 0; e <= _sectorCount; e++){
		glm::vec3 divided =  p1 + (padding * (float)e);
		glm::vec3 divided2 = p3 + (padding * (float)e);
		
		glm::vec3 dir = glm::cross(pBuffer2 - pBuffer1,divided - pBuffer1);
		glm::vec3 norm1 = glm::normalize(dir);
		glm::vec3 norm2 = dir / glm::length(dir);
		
		InsertVec3(pBuffer1); InsertVec3(norm1); InsertVec2(pTexCBuffer1);
		InsertVec3(pBuffer2); InsertVec3(norm1); InsertVec2(pTexCBuffer2);
		InsertVec3(divided); InsertVec3(norm1); InsertVec2( pTexC1 + (paddingTexC * (float)e));

		InsertVec3(divided); InsertVec3(norm2); InsertVec2(pTexC1 + (paddingTexC * (float)e));
		InsertVec3(divided2); InsertVec3(norm2); InsertVec2(pTexC3 + (paddingTexC * (float)e));
		InsertVec3(pBuffer2); InsertVec3(norm2); InsertVec2(pTexCBuffer2);
		
		pBuffer1 = divided;
		pBuffer2 = divided2;
		
		pTexCBuffer1 = pTexC1 + (paddingTexC * (float)e);
		pTexCBuffer2 = pTexC3 + (paddingTexC * (float)e);
	}
}

void Parallelepiped::InsertVec3(glm::vec3 p){
	datas << p.x << p.y << p.z;
}
void Parallelepiped::InsertVec2(glm::vec2 p){
	datas << p.x << p.y ;
}

bool Parallelepiped::Compare(float _length, float _width, float _height, float _angleDegree, int _sectorCount){
	return ((length == _length) && (width == _width) && (height == _height) && (angleDegree == _angleDegree) && (sectorCount == _sectorCount));
}
Vector<float> Parallelepiped::GetContiniousData(){
	return clone(datas);
}

}