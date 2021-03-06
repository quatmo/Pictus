#ifndef FACTORY_RENDERER_H
#define FACTORY_RENDERER_H

#include "w32_renderer.h"
#include "app_types.h"

namespace App {
	class RendererFactory {
	public:
		Win::Renderer::Ptr CreateRenderer();
	};
}

#endif
