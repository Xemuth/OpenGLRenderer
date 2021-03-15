#ifndef _pluginUFE_OpenGLRenderer_CGLAutonomous_h_
#define _pluginUFE_OpenGLRenderer_CGLAutonomous_h_
#include <UFEngine/UFEngine.h>

namespace Upp{
/*
	Render order is used to define if this Autonomous component should be draw before drawing
	of clasic SGLRenderer object or after it. If you want to make an interface, it's better to
	draw the interface before without depth testing then draw the rest by exemple
*/
enum class RenderOrder{Before, After};

/*
	Unlike most of OpenGL component, CGLAutonomous is used to draw/use OpenGL features outscope
	of the SGLRenderer. You can declare your own shader/ model / renderer in it and define how
	you want to draw something. It need to be added to the Render queue via the method:
		void SGLRenderer::AddToRenderQueue(CGLAutonomous& cglautonome);
	it will then be drawn after every classic objects.
	This class inherrite from CGLRoutineBeforeAfterRendering. it mean you can declare some
	routine before/after being drawing. It's important to use the before routine to add this
	object to the render queue, else it wont be rendered at all for this frame
*/
class CGLAutonomous : public Component{
	public:
			RenderOrder renderOrder = RenderOrder::Before;
			virtual String GetName()const = 0;
			/*
				ToString is forced to be defined. It's important since CGLAutonomous can carry OpenGL
				object without noticing or passing through SGLRenderer. the function purpose is
				to describe all data the carry and manage in order to help debuging and have a
				strong globazl view of every openGL object existing in VRAM
			*/
			virtual String ToString()const = 0;
			
			virtual void Draw(CGLCamera& camera) = 0;
};

}

#endif
