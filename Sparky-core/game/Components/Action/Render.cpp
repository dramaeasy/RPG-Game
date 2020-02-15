#include "Render.h"

Render::Render(Renderable2D* renderable)
{
	id = ComponentIDEnum::RenderComp;
	name = "Render Component";
	bitmask = RENDER_MASK;
	this->renderable = renderable;
}

Renderable2D* Render::GetRenderable()
{
	return this->renderable;
}