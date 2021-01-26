// Copyright © 2008-2021 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef _BACKGROUND_H
#define _BACKGROUND_H

#include "galaxy/SystemPath.h"
#include "graphics/Drawables.h"

class Random;
class Galaxy;
class Space;

namespace Graphics {
	class Renderer;
	class Material;
	class RenderState;
	class Texture;

} // namespace Graphics

/*
 * Classes to draw background stars and the milky way
 */

namespace Background {


	enum BackgroundDrawFlags {
		DRAW_STAR_FIELD = 1 << 0,
		DRAW_HYPERSPACE_STARS = 1 << 1,
		DRAW_SKYBOX = 1 << 2
	};

	class BackgroundElement {
	public:
		void SetIntensity(float intensity);

	protected:
		Graphics::Renderer *m_renderer;
		RefCountedPtr<Graphics::Material> m_material;
		RefCountedPtr<Graphics::Material> m_materialStreaks;

		float m_rMin;
		float m_rMax;
		float m_gMin;
		float m_gMax;
		float m_bMin;
		float m_bMax;
	};

	class UniverseBox : public BackgroundElement {
	public:
		UniverseBox(Graphics::Renderer *r);
		~UniverseBox();

		void Draw(Graphics::RenderState *);
		void SetCubeMap(const std::string& filePath);

	private:
		void Init();

		std::unique_ptr<Graphics::VertexBuffer> m_vertexBuffer;
		RefCountedPtr<Graphics::Texture> m_cubemap;
	};

	class Starfield : public BackgroundElement {
	public:
		//does not Fill the starfield
		Starfield(Graphics::Renderer *, Random &rand, const SystemPath *const systemPath, RefCountedPtr<Galaxy> galaxy);
		void Draw(Graphics::RenderState *, const Uint32 flags);
		//create or recreate the starfield
		void Fill(Random &rand, const SystemPath *const systemPath, RefCountedPtr<Galaxy> galaxy);

	private:
		void Init();

		std::unique_ptr<Graphics::Drawables::PointSprites> m_pointSprites;
		Graphics::RenderState *m_renderState; // NB: we don't own RenderState pointers, just borrow them

		//hyperspace animation vertex data
		std::unique_ptr<vector3f[]> m_hyperVtx; // BG_STAR_MAX * 3
		std::unique_ptr<Color[]> m_hyperCol;	// BG_STAR_MAX * 3
		std::unique_ptr<Graphics::VertexBuffer> m_animBuffer;
	};

	// contains starfield, milkyway, possibly other Background elements
	class Container {
	public:

		Container(Graphics::Renderer *, Random &rand, const Space *space, RefCountedPtr<Galaxy> galaxy, const SystemPath *const systemPath = nullptr);
		void Draw(const matrix4x4d &transform);

		void SetIntensity(float intensity);
		void SetDrawFlags(const Uint32 flags);
		Uint32 GetDrawFlags() const { return m_drawFlags; }
		void SetSkybox(const std::string& filePath) { m_universeBox.SetCubeMap(filePath); }

	private:
		Graphics::Renderer *m_renderer;
		Starfield m_starField;
		UniverseBox m_universeBox;
		Uint32 m_drawFlags;
		Graphics::RenderState *m_renderState;
	};

} //namespace Background

#endif
