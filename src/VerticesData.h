#ifndef _pluginUFE_OpenGLRenderer_VerticesData_h_
#define _pluginUFE_OpenGLRenderer_VerticesData_h_

namespace Upp{

#define A  -1.0f,0.0f,1.0f
#define B  1.0f,0.0f,1.0f
#define C  1.0f,0.0f,-1.0f
#define D  -1.0f,0.0f,-1.0f
#define E  0.0f,2.0f,0.0f

static Upp::Vector<float> pyramidVertices{
	A, 0.0f,-1.0f,0.0f ,0.0f,0.0f,
	B, 0.0f,-1.0f,0.0f ,1.0f,0.0f,
	C, 0.0f,-1.0f,0.0f ,1.0f,1.0f,
	
	C, 0.0f,-1.0f,0.0f ,1.0f,1.0f,
	D, 0.0f,-1.0f,0.0f ,0.0f,1.0f,
	A, 0.0f,-1.0f,0.0f ,0.0f,0.0f,
	
	A, -1.0f,1.0f,0.0f ,1.0f,0.0f,
	E, -1.0f,1.0f,0.0f ,0.5f,1.0f,
	D, -1.0f,1.0f,0.0f ,0.0f,0.0f,
	
	D, 0.0f,1.0f,-1.0f ,1.0f,0.0f,
	E, 0.0f,1.0f,-1.0f ,0.5f,1.0f,
	C, 0.0f,1.0f,-1.0f ,0.0f,0.0f,
	
	C, 1.0f,1.0f,0.0f ,1.0f,0.0f,
	E, 1.0f,1.0f,0.0f ,0.5f,1.0f,
	B, 1.0f,1.0f,0.0f ,0.0f,0.0f,
	
	B, 0.0f,1.0f,1.0f ,1.0f,0.0f,
	E, 0.0f,1.0f,1.0f ,0.5f,1.0f,
	A, 0.0f,1.0f,1.0f ,0.0f,0.0f
};

#undef A
#undef B
#undef C
#undef D
#undef E

#define A  -1.0f,0.0f,1.0f
#define B  1.0f,0.0f,1.0f
#define C  1.0f,0.0f,-1.0f
#define D  -1.0f,0.0f,-1.0f

#define E  -1.0f,2.0f,1.0f
#define F  1.0f,2.0f,1.0f
#define G  1.0f,2.0f,-1.0f
#define H  -1.0f,2.0f,-1.0f

static Upp::Vector<float> cubeVertices{
	A, 0.0f,-1.0f,0.0f ,0.0f,0.0f,
	B, 0.0f,-1.0f,0.0f ,1.0f,0.0f,
	C, 0.0f,-1.0f,0.0f ,1.0f,1.0f,
	
	C, 0.0f,-1.0f,0.0f, 1.0f,1.0f,
	D, 0.0f,-1.0f,0.0f, 0.0f,1.0f,
	A, 0.0f,-1.0f,0.0f, 0.0f,0.0f,
	
	A, -1.0f,0.0f,0.0f ,1.0f,0.0f,
	E, -1.0f,0.0f,0.0f ,1.0f,1.0f,
	H, -1.0f,0.0f,0.0f ,0.0f,1.0f,
	
	H, -1.0f,0.0f,0.0f ,0.0f,1.0f,
	D, -1.0f,0.0f,0.0f ,0.0f,0.0f,
	A, -1.0f,0.0f,0.0f ,1.0f,0.0f,
	
	D, 0.0f,0.0f,-1.0f ,1.0f,0.0f,
	H, 0.0f,0.0f,-1.0f ,1.0f,1.0f,
	G, 0.0f,0.0f,-1.0f ,0.0f,1.0f,
	
	G, 0.0f,0.0f,-1.0f ,0.0f,1.0f,
	C, 0.0f,0.0f,-1.0f ,0.0f,0.0f,
	D, 0.0f,0.0f,-1.0f ,1.0f,0.0f,
	
	C, 1.0f,0.0f,0.0f ,1.0f,0.0f,
	G, 1.0f,0.0f,0.0f ,1.0f,1.0f,
	F, 1.0f,0.0f,0.0f ,0.0f,1.0f,
	
	F, 1.0f,0.0f,0.0f ,0.0f,1.0f,
	B, 1.0f,0.0f,0.0f ,0.0f,0.0f,
	C, 1.0f,0.0f,0.0f ,1.0f,0.0f,
	
	B, 0.0f,0.0f,1.0f ,1.0f,0.0f,
	F, 0.0f,0.0f,1.0f ,1.0f,1.0f,
	E, 0.0f,0.0f,1.0f ,0.0f,1.0f,
	
	E, 0.0f,0.0f,1.0f ,0.0f,1.0f,
	A, 0.0f,0.0f,1.0f ,0.0f,0.0f,
	B, 0.0f,0.0f,1.0f ,1.0f,0.0f,
	
	F, 0.0f,1.0f,0.0f, 1.0f,0.0f,
	E, 0.0f,1.0f,0.0f, 0.0f,0.0f,
	G, 0.0f,1.0f,0.0f, 1.0f,1.0f,
	
	G, 0.0f,1.0f,0.0f, 1.0f,1.0f,
	H, 0.0f,1.0f,0.0f, 0.0f,1.0f,
	E, 0.0f,1.0f,0.0f, 0.0f,0.0f,
};

#undef A
#undef B
#undef C
#undef D
#undef E
#undef F
#undef G
#undef H


/*
static Vector<float> GenerateCylinder(float radius = 1.0f ,float halfLength = 2.0f, int slices = 10){
	Upp::Vector<float> datas;
	for(int i=0; i<slices; i++) {
		float theta = ((float)i)*2.0f*M_PI;
		float nextTheta = ((float)i+1)*2.0f*M_PI;
		//vertex at middle of end
		datas << 0.0f << halfLength << 0.0f;
		//vertices at edges of circle
		datas << radius*cos(theta) << halfLength << radius*sin(theta);
		datas << radius*cos(nextTheta) << halfLength << radius*sin(nextTheta);
		// the same vertices at the bottom of the cylinder
		datas << radius*cos(nextTheta) << -halfLength << radius*sin(nextTheta);
		datas << radius*cos(theta) << -halfLength << radius*sin(theta);
		datas << 0.0 << -halfLength << 0.0;
	}
	DUMP(datas);
	return datas;
}*/

}

#endif
