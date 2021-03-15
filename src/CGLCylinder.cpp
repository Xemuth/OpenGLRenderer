#include "../CGLCylinder.h"
#include "Util.h"

#include "../OpenGLRenderer.h"

namespace Upp{
	
CGLCylinder::CGLCylinder(SGLRenderer& _openGLRenderer, float _baseRadius, float _topRadius, float _height, int _sectorCount, int _stackCount) : CGLPolygon(_openGLRenderer),
	baseRadius(_baseRadius),topRadius(_topRadius),height(_height), sectorCount(_sectorCount), stackCount(_stackCount){
	instanceCount++;
	ID = instanceCount;
	ONCELOCK{
		CreateShader(openGLRenderer);
		CreateRenderer(openGLRenderer);
	/*	if(!openGLRenderer.GetRendererManager().Has(rendererName + GetName())){
			Renderer& renderer = openGLRenderer.GetRendererManager().Create(rendererName + GetName());
			renderer.GetVertexArrayAttribFormat().Append(
				Upp::Vector<Upp::VAAF>{
					Upp::VAAF(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float)), //Position
					Upp::VAAF(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float)), //normals
					Upp::VAAF(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float)), //texCoords
				}
			);
			renderer.LoadVAO();
		}*/
	}
	bool found = false;
	for(PolygonUseCount<Cylinder>& cuc : allCylinders){
		if(cuc.polygon.Compare(baseRadius,topRadius,height,sectorCount,stackCount)){
			found = true;
			modelName = cuc.ModelName;
			cuc.count++;
			break;
		}
	}
	if(!found){
		PolygonUseCount<Cylinder>& cuc = allCylinders.Create<PolygonUseCount<Cylinder>>(baseRadius,topRadius,height,sectorCount,stackCount);
		Model& model = openGLRenderer.GetModelManager().Create(prefixName + GetName() + AsString(ID));
		Vector<float> datas = pick(cuc.polygon.GetContiniousData());
		model.AddVertices(datas, datas.GetCount()/3);
		model.Load();
		cuc.ModelName = prefixName + GetName() + AsString(ID);
		modelName = cuc.ModelName;
	}
//	customRenderer = rendererName + GetName();
}

CGLCylinder::CGLCylinder(const CGLCylinder& cglCylinder) : CGLPolygon(cglCylinder),
	baseRadius(cglCylinder.baseRadius),topRadius(cglCylinder.topRadius),height(cglCylinder.height), sectorCount(cglCylinder.sectorCount), stackCount(cglCylinder.stackCount){
	instanceCount++;
	ID = instanceCount;
	for(PolygonUseCount<Cylinder>& cuc : allCylinders){
		if(cuc.polygon.Compare(baseRadius,topRadius,height,sectorCount,stackCount)){
			modelName = cuc.ModelName;
			cuc.count++;
			break;
		}
	}
}

CGLCylinder::~CGLCylinder(){
	if(!picked){
		for(int e = 0; e < allCylinders.GetCount(); e++){
			PolygonUseCount<Cylinder>& cuc = allCylinders[e];
			if(cuc.polygon.Compare(baseRadius,topRadius,height,sectorCount,stackCount)){
				cuc.count--;
				if(cuc.count == 0){
					openGLRenderer.GetModelManager().Remove(cuc.ModelName);
					allCylinders.Remove(e);
				}
				break;
			}
		}
	}
}

void CGLCylinder::Update(double timeEllapsed, double deltaTime){
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


Cylinder::Cylinder(float _baseRadius, float _topRadius, float _height, int _sectorCount, int _stackCount) :
	baseRadius(_baseRadius),topRadius(_topRadius),height(_height), sectorCount(_sectorCount), stackCount(_stackCount){
	GenerateCylinder();
}

bool Cylinder::Compare(float _baseRadius, float _topRadius, float _height, int _sectorCount, int _stackCount){
	return ((baseRadius == _baseRadius) && (topRadius == _topRadius) && (height == _height) && (sectorCount == _sectorCount) && (stackCount == _stackCount));
}

// generate a unit circle on XY-plane
void Cylinder::BuildUnitCircleVertices(){
    const float PI = 3.1415926f;
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    //Vector<float> unitCircleVertices;
    for(int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        unitCircleVertices.push_back(cos(sectorAngle)); // x
        unitCircleVertices.push_back(sin(sectorAngle)); // y
        unitCircleVertices.push_back(0);                // z
    }
}
Vector<float> Cylinder::GetSideNormals(){
    const float PI = (float)acos(-1);
    float sectorStep = 2.0f * PI / (float)sectorCount;
    float sectorAngle;  // radian

    // compute the normal vector at 0 degree first
    // tanA = (baseRadius-topRadius) / height
    float zAngle = atan2(baseRadius - topRadius, height);
    float x0 = cos(zAngle);     // nx
    float y0 = 0;               // ny
    float z0 = sin(zAngle);     // nz

    // rotate (x0,y0,z0) per sector angle
    Vector<float> norm;
    for(int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        norm.push_back(cos(sectorAngle)*x0 - sin(sectorAngle)*y0);   // nx
        norm.push_back(sin(sectorAngle)*x0 + cos(sectorAngle)*y0);   // ny
        norm.push_back(z0);  // nz
    }

    return pick(norm);
}

void Cylinder::AddVertex(float x, float y, float z){
	vertices << x << y << z;
}
void Cylinder::AddNormals(float x, float y, float z){
	normals << x << y << z;
}
void Cylinder::AddTexCoords(float x, float y){
	texCoords << x << y;
}
void Cylinder::AddIndices(unsigned int i1, unsigned int i2, unsigned int i3){
    indices<< i1 << i2 << i3;
}


// generate vertices for a cylinder
void Cylinder::GenerateCylinder(){
    float x, y, z;                                  // vertex position
    //float s, t;                                     // texCoord
    float radius;                                   // radius for each stack

    // get normals for cylinder sides
    Vector<float> sideNormals = pick(GetSideNormals());
	BuildUnitCircleVertices();

    // put vertices of side cylinder to array by scaling unit circle
    for(int i = 0; i <= stackCount; ++i){
        z = -(height * 0.5f) + (float)i / stackCount * height;      // vertex position z
        radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);     // lerp
        float t = 1.0f - (float)i / stackCount;   // top-to-bottom

        for(int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
        {
            x = unitCircleVertices[k];
            y = unitCircleVertices[k+1];
            AddVertex(x * radius, y * radius, z);   // position
            AddNormals(sideNormals[k], sideNormals[k+1], sideNormals[k+2]); // normal
            AddTexCoords((float)j / sectorCount, t); // tex coord
        }
    }

    // remember where the base.top vertices start
    unsigned int baseVertexIndex = (unsigned int)vertices.GetCount() / 3;

    // put vertices of base of cylinder
    z = -height * 0.5f;
    AddVertex(0, 0, z);
    AddNormals(0, 0, -1);
    AddTexCoords(0.5f, 0.5f);
    for(int i = 0, j = 0; i < sectorCount; ++i, j += 3){
        x = unitCircleVertices[j];
        y = unitCircleVertices[j+1];
        AddVertex(x * baseRadius, y * baseRadius, z);
        AddNormals(0, 0, -1);
        AddTexCoords(-x * 0.5f + 0.5f, -y * 0.5f + 0.5f);    // flip horizontal
    }

    // remember where the base vertices start
    unsigned int topVertexIndex = (unsigned int)vertices.GetCount() / 3;

    // put vertices of top of cylinder
    z = height * 0.5f;
    AddVertex(0, 0, z);
    AddNormals(0, 0, 1);
    AddTexCoords(0.5f, 0.5f);
    for(int i = 0, j = 0; i < sectorCount; ++i, j += 3){
        x = unitCircleVertices[j];
        y = unitCircleVertices[j+1];
        AddVertex(x * topRadius, y * topRadius, z);
        AddNormals(0, 0, 1);
        AddTexCoords(x * 0.5f + 0.5f, -y * 0.5f + 0.5f);
    }

    // put indices for sides
    unsigned int k1, k2;
    for(int i = 0; i < stackCount; ++i){
        k1 = i * (sectorCount + 1);     // bebinning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2){
            // 2 trianles per sector
            AddIndices(k1, k1 + 1, k2);
            AddIndices(k2, k1 + 1, k2 + 1);
        }
    }

    // remember where the base indices start
    baseIndex = (unsigned int)indices.size();

    // put indices for base
    for(int i = 0, k = baseVertexIndex + 1; i < sectorCount; ++i, ++k){
        if(i < (sectorCount - 1))
            AddIndices(baseVertexIndex, k + 1, k);
        else    // last triangle
            AddIndices(baseVertexIndex, baseVertexIndex + 1, k);
    }

    // remember where the base indices start
    topIndex = (unsigned int)indices.size();

    for(int i = 0, k = topVertexIndex + 1; i < sectorCount; ++i, ++k){
        if(i < (sectorCount - 1))
            AddIndices(topVertexIndex, k, k + 1);
        else
            AddIndices(topVertexIndex, k, topVertexIndex + 1);
    }
    
	
	
	
	/*
    // get unit circle vectors on XY-plane
    Vector<float> unitVertices = pick(GetUnitCircleVertices());
	int verticesSize = 0;
    // put side vertices to arrays
    for(int i = 0; i < 2; ++i)
    {
        float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
        float t = 1.0f - i;                              // vertical tex coord; 1 to 0

        for(int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
        {
            float ux = unitVertices[k];
            float uy = unitVertices[k+1];
            float uz = unitVertices[k+2];
            // position vector
            vertices << ux * baseRadius << uy * baseRadius << h;
            // normal vector
            normals << ux << uy << uz;
            // texture coordinate
            texCoords << (float)j / sectorCount << t;
        }
    }

    // the starting index for the base/top surface
    //NOTE: it is used for generating indices later
    baseCenterIndex = vertices.GetCount() / 3;
    topCenterIndex = baseCenterIndex + sectorCount + 1; // include center vertex

    // put base and top vertices to arrays
    for(int i = 0; i < 2; ++i)
    {
        float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
        float nz = -1.0f + (float)i * 2.0f;                           // z value of normal; -1 to 1

        // center point
        vertices << 0 << 0 << h;
        normals << 0 << 0 << nz;
        texCoords << 0.5f << 0.5f;

        for(int j = 0, k = 0; j < sectorCount; ++j, k += 3)
        {
            float ux = unitVertices[k];
            float uy = unitVertices[k+1];
            // position vector
            vertices << ux * topRadius << uy * topRadius << h;
            // normal vector
            normals << 0 << 0 << nz;
            // texture coordinate
            texCoords << -ux * 0.5f + 0.5f << -uy * 0.5f + 0.5f;
        }
    }
    
    */
}
/*
void Cylinder::FillIndicesVector(){
	int k1 = 0;                         // 1st vertex index at base
	int k2 = sectorCount + 1;           // 1st vertex index at top
	
	// indices for the side surface
	for(int i = 0; i < sectorCount; ++i, ++k1, ++k2){
	    // 2 triangles per sector
	    // k1 => k1+1 => k2
	    indices << k1 << k1 + 1 << k2;
	    // k2 => k1+1 => k2+1
	    indices << k2 << k1 +1 << k2;
	}
	
	// indices for the base surface
	//NOTE: baseCenterIndex and topCenterIndices are pre-computed during vertex generation
	//      please see the previous code snippet
	for(int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k){
	    if(i < sectorCount - 1){
	        indices << baseCenterIndex << k + 1 << k;
	    }
	    else{ // last triangle
	        indices << baseCenterIndex << baseCenterIndex +1 << k;
	    }
	}
	
	// indices for the top surface
	for(int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k){
	    if(i < sectorCount - 1){
	        indices << topCenterIndex << k << k + 1;
	    }
	    else{ // last triangle
			indices << topCenterIndex << k << topCenterIndex +1;
	    }
	}
}
*/
Vector<float> Cylinder::GetContiniousData(){
	Vector<float> continiousData;
	for(int i : indices){
		continiousData << vertices[(i *3)] << vertices[(i *3) +1] << vertices[(i *3)+2] << normals[(i *3)] << normals[(i *3) +1] << normals[(i *3) +2] << texCoords[(i * 2)] << texCoords[(i * 2)+1];
	}
	return pick(continiousData);
}
	


}

/*
float radius, halfLength;
int slices;
for(int i=0; i<slices; i++) {
	float theta = ((float)i)*2.0*M_PI;
	float nextTheta = ((float)i+1)*2.0*M_PI;
	glBegin(GL_TRIANGLE_STRIP);
		//vertex at middle of end
		glVertex3f(0.0, halfLength, 0.0);
		//vertices at edges of circle
		glVertex3f(radius*cos(theta), halfLength, radius*sin(theta));
		glVertex3f (radius*cos(nextTheta), halfLength, radius*sin(nextTheta));
		// the same vertices at the bottom of the cylinder
		glVertex3f (radius*cos(nextTheta), -halfLength, radius*sin(nextTheta));
		glVertex3f(radius*cos(theta), -halfLength, radius*sin(theta));
		glVertex3f(0.0, -halfLength, 0.0);
	glEnd();
}
*/