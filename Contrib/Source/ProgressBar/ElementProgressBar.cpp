#include <Rocket/ProgressBar/ElementProgressBar.h>
#include <Rocket/Core/GeometryUtilities.h>
#include <Rocket/Core/ElementDocument.h>

namespace Rocket {
namespace ProgressBar {

// Constructs a new ElementProgressBar. This should not be called directly; use the Factory instead.
ElementProgressBar::ElementProgressBar(const Rocket::Core::String& tag) : Core::Element(tag), left_geometry(this), center_geometry(this), right_geometry(this), geometry_dirty(true), value(0.0f)
{
}

ElementProgressBar::~ElementProgressBar()
{
}

// Returns float value between 0 and 1 of the progress bar.
float ElementProgressBar::GetValue() const
{
	return GetAttribute< float >("value", 0.0f);
}

// Sets the current value of the progress bar.
void ElementProgressBar::SetValue(const float value)
{
	SetAttribute("value", value);
}

// Called during the update loop after children are rendered.
void ElementProgressBar::OnUpdate()
{
}

// Called during render after backgrounds, borders, decorators, but before children, are rendered.
void ElementProgressBar::OnRender()
{
	if(geometry_dirty)
		GenerateGeometry();

	left_geometry.Render(GetAbsoluteOffset(Rocket::Core::Box::CONTENT));
	center_geometry.Render(GetAbsoluteOffset(Rocket::Core::Box::CONTENT));
	right_geometry.Render(GetAbsoluteOffset(Rocket::Core::Box::CONTENT));
}

// Called when attributes on the element are changed.
void ElementProgressBar::OnAttributeChange(const Core::AttributeNameList& changed_attributes)
{
	Element::OnAttributeChange(changed_attributes);

	if (changed_attributes.find("value") != changed_attributes.end())
	{
		value = GetAttribute< float >("value", 0.0f);

		if (value < 0)
			value = 0.0f;
		else if (value > 1)
			value = 1.0f;

		geometry_dirty = true;
	}
}

// Called when properties on the element are changed.
void ElementProgressBar::OnPropertyChange(const Core::PropertyNameList& changed_properties)
{
	Element::OnPropertyChange(changed_properties);

	if (changed_properties.find("progress-left-src") != changed_properties.end())
	{
		LoadTexture(0);
	}
	
	if (changed_properties.find("progress-center-src") != changed_properties.end())
	{
		LoadTexture(1);
	}
	
	if (changed_properties.find("progress-right-src") != changed_properties.end())
	{
		LoadTexture(2);
	}
}

// Called when value has changed.
void ElementProgressBar::GenerateGeometry()
{
	const Core::Vector2f complete_extent = GetBox().GetSize(Core::Box::CONTENT);
	Core::Vector2i initial_part_size[3];
	Core::Vector2f final_part_size[3];
	float progress_size;

	initial_part_size[0] = texture[0].GetDimensions(GetRenderInterface());
	initial_part_size[2] = texture[2].GetDimensions(GetRenderInterface());

	progress_size = value * (complete_extent.x - initial_part_size[0].x - initial_part_size[2].x);

	left_geometry.Release(true);
	center_geometry.Release(true);
	right_geometry.Release(true);

	final_part_size[0].y = final_part_size[1].y = final_part_size[2].y = float(complete_extent.y);
	final_part_size[0].x = float(initial_part_size[0].x);
	final_part_size[2].x = float(initial_part_size[2].x);
	final_part_size[1].x = progress_size;

	// Generate left part geometry.
	{
		std::vector< Rocket::Core::Vertex >& vertices = left_geometry.GetVertices();
		std::vector< int >& indices = left_geometry.GetIndices();

		vertices.resize(4);
		indices.resize(6);

		Rocket::Core::GeometryUtilities::GenerateQuad(&vertices[0],
													  &indices[0],
													  Core::Vector2f(0, 0),
													  final_part_size[0],
													  Core::Colourb(255, 255, 255, 255),
													  texcoords[0][0],
													  texcoords[0][1]);
	}

	// Generate center part geometry.
	if (final_part_size[1].x > 0.0f)
	{
		std::vector< Rocket::Core::Vertex >& vertices = center_geometry.GetVertices();
		std::vector< int >& indices = center_geometry.GetIndices();

		vertices.resize(4);
		indices.resize(6);

		Rocket::Core::GeometryUtilities::GenerateQuad(&vertices[0],
													  &indices[0],
													  Core::Vector2f(final_part_size[0].x, 0),
													  final_part_size[1],
													  Core::Colourb(255, 255, 255, 255),
													  texcoords[1][0],
													  texcoords[1][1]);
	}

	// Generate center part geometry.
	{
		std::vector< Rocket::Core::Vertex >& vertices = right_geometry.GetVertices();
		std::vector< int >& indices = right_geometry.GetIndices();

		vertices.resize(4);
		indices.resize(6);

		Rocket::Core::GeometryUtilities::GenerateQuad(&vertices[0],
													  &indices[0],
													  Core::Vector2f(final_part_size[0].x + final_part_size[1].x, 0),
													  final_part_size[2],
													  Core::Colourb(255, 255, 255, 255),
													  texcoords[2][0],
													  texcoords[2][1]);
	}

	geometry_dirty = false;
}

/// Called when source texture has changed.
void ElementProgressBar::LoadTexture(int index)
{
	Core::ElementDocument* document = GetOwnerDocument();
	Core::URL source_url(document == NULL ? "" : document->GetSourceURL());

	switch(index)
	{
		case 0:
		{
			LoadTexture(source_url, 0, "progress-left-src", left_geometry);
			break;
		}

		case 1:
		{
			LoadTexture(source_url, 1, "progress-center-src", center_geometry);
			break;
		}

		case 2:
		{
			LoadTexture(source_url, 2, "progress-right-src", right_geometry);
			break;
		}
	}
}

/// Called when source texture has changed.
void ElementProgressBar::LoadTexture(Core::URL & source_url, int index, const char *property_name, Core::Geometry & geometry)
{
	Core::RenderInterface* render_interface = GetRenderInterface();
	Core::StringList words;
	Core::String source = GetProperty< Core::String >(property_name);
	Core::StringUtilities::ExpandString(words, source, ' ');
	bool it_uses_tex_coords = false;

	if (words.size() == 5)
	{
		it_uses_tex_coords = true;
		source = words[0];
		Core::TypeConverter< Core::String, float >::Convert(words[1], texcoords[index][0].x);
		Core::TypeConverter< Core::String, float >::Convert(words[2], texcoords[index][0].y);
		Core::TypeConverter< Core::String, float >::Convert(words[3], texcoords[index][1].x);
		Core::TypeConverter< Core::String, float >::Convert(words[4], texcoords[index][1].y);
	}

	if (!texture[index].Load(source, source_url.GetPath()))
	{
		geometry.SetTexture(NULL);
		return;
	}

	if (it_uses_tex_coords)
	{
		for (int i = 0; i < 2; i++)
		{
			texcoords[index][i].x /= texture[index].GetDimensions(render_interface).x;
			texcoords[index][i].y /= texture[index].GetDimensions(render_interface).y;
		}
	}
	else
	{
		texcoords[index][0].x = 0.0f;
		texcoords[index][0].y = 0.0f;
		texcoords[index][1].x = 1.0f;
		texcoords[index][1].y = 1.0f;
	}

	geometry.SetTexture(&texture[index]);
}

}
}
