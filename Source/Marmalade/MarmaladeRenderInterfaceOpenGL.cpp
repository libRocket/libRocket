#include "MarmaladeRenderInterfaceOpenGL.h"
#include <Rocket/Core.h>
#include <IwGx.h>

MarmaladeRenderInterfaceOpenGL::MarmaladeRenderInterfaceOpenGL()
  : _enableScissors(false)
{
}

// Called by Rocket when it wants to render geometry that it does not wish to optimise.
void MarmaladeRenderInterfaceOpenGL::RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation)
{
	// uvs + verts + colors
	CIwFVec2 *uvs = texture ? IW_GX_ALLOC(CIwFVec2, num_vertices) : NULL;
	CIwSVec2 *verts = IW_GX_ALLOC(CIwSVec2, num_vertices);
	CIwColour *colors = IW_GX_ALLOC(CIwColour, num_vertices);
	for (int i = 0; i < num_vertices; ++i) {
		if (texture) {
			uvs[i].x = vertices[i].tex_coord.x;
			uvs[i].y = vertices[i].tex_coord.y;
		}

		verts[i].x = (int)((vertices[i].position.x + translation.x)*8.f);
		verts[i].y = (int)((vertices[i].position.y + translation.y)*8.f);

		colors[i].r = vertices[i].colour.red;
		colors[i].g = vertices[i].colour.green;
		colors[i].b = vertices[i].colour.blue;
		colors[i].a = vertices[i].colour.alpha;
	}

	// indices
	uint16 *indices_stream = IW_GX_ALLOC(uint16, num_indices);
	for (int i = 0; i < num_indices; ++i)
		indices_stream[i] = (uint16)indices[i];

	CIwMaterial* mat = IW_GX_ALLOC_MATERIAL();
	if (texture) {
		CIwTexture* tex = _textures.element_at(texture-1);
		mat->SetTexture(tex);
	} else {
		mat->SetTexture(NULL);
	}
	mat->SetDepthWriteMode(CIwMaterial::DEPTH_WRITE_DISABLED);
	mat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	IwGxSetMaterial(mat);		// can be cached to improve performance

	IwGxSetUVStream(uvs);
	IwGxSetVertStreamScreenSpaceSubPixel(verts, num_vertices);
	IwGxSetColStream(colors, num_vertices);
	IwGxSetNormStream(NULL);
	IwGxDrawPrims(IW_GX_TRI_LIST, indices_stream, num_indices);
}

// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.		
Rocket::Core::CompiledGeometryHandle MarmaladeRenderInterfaceOpenGL::CompileGeometry(Rocket::Core::Vertex* ROCKET_UNUSED(vertices), int ROCKET_UNUSED(num_vertices), int* ROCKET_UNUSED(indices), int ROCKET_UNUSED(num_indices), const Rocket::Core::TextureHandle ROCKET_UNUSED(texture))
{
	return (Rocket::Core::CompiledGeometryHandle) NULL;
}

// Called by Rocket when it wants to render application-compiled geometry.		
void MarmaladeRenderInterfaceOpenGL::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle ROCKET_UNUSED(geometry), const Rocket::Core::Vector2f& ROCKET_UNUSED(translation))
{
}

// Called by Rocket when it wants to release application-compiled geometry.		
void MarmaladeRenderInterfaceOpenGL::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle ROCKET_UNUSED(geometry))
{
}

// Called by Rocket when it wants to enable or disable scissoring to clip content.		
void MarmaladeRenderInterfaceOpenGL::EnableScissorRegion(bool enable)
{
  _enableScissors = enable;
  if (!_enableScissors)
    IwGxClearScissorScreenSpace();
}

// Called by Rocket when it wants to change the scissor region.		
void MarmaladeRenderInterfaceOpenGL::SetScissorRegion(int x, int y, int width, int height)
{
  _scissors.Make(x, y, width, height);
  if (_enableScissors)
    IwGxSetScissorScreenSpace(_scissors.x, _scissors.y, _scissors.w, _scissors.h);
}

// Called by Rocket when a texture is required by the library.		
bool MarmaladeRenderInterfaceOpenGL::LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
{
	CIwTexture *s_Texture = new CIwTexture;
	s_Texture->SetFormatHW(CIwImage::RGBA_8888);
	s_Texture->SetFormatSW(CIwImage::RGBA_8888);
	s_Texture->LoadFromFile(source.CString());
	s_Texture->Upload();

	texture_dimensions.x = s_Texture->GetWidth();
	texture_dimensions.y = s_Texture->GetHeight();

	_textures.append(s_Texture);
	texture_handle = (Rocket::Core::TextureHandle)_textures.size();   // indexing from 1, 0 means no texture

	return true;
}

// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
bool MarmaladeRenderInterfaceOpenGL::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
	CIwTexture *s_Texture = new CIwTexture;
	s_Texture->SetFormatHW(CIwImage::RGBA_8888);
	s_Texture->SetFormatSW(CIwImage::RGBA_8888);
	s_Texture->CopyFromBuffer(source_dimensions.x, source_dimensions.y, CIwImage::ARGB_8888, source_dimensions.x << 2, (uint8*)source, NULL);
	s_Texture->Upload();

	_textures.append(s_Texture);
	texture_handle = (Rocket::Core::TextureHandle)_textures.size();		// indexing from 1, 0 means no texture

	return true;
}

// Called by Rocket when a loaded texture is no longer required.		
void MarmaladeRenderInterfaceOpenGL::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
	CIwTexture *tex = _textures.element_at(texture_handle-1);
	delete tex;
	tex = NULL;
}
