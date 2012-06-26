#ifndef ROCKETPROGRESSBARELEMENTPROGRESSBAR_H
#define ROCKETPROGRESSBARELEMENTPROGRESSBAR_H

#include <Rocket/ProgressBar/Header.h>
#include <Rocket/Core/Element.h>
#include <Rocket/Core/Geometry.h>
#include <Rocket/Core/Texture.h>

namespace Rocket {
namespace ProgressBar {

/**
	A progress bar element using a value between 0 and 1.

	@author Gauthier Billot
 */

class ROCKETPROGRESSBAR_API ElementProgressBar : public Core::Element
{
public:
	/// Constructs a new ElementProgressBar. This should not be called directly; use the Factory instead.
	/// @param[in] tag The tag the element was declared as in RML.
	ElementProgressBar(const Rocket::Core::String& tag);
	virtual ~ElementProgressBar();

	/// Returns float value between 0 and 1.
	/// @return The value of the progress bar.
	float GetValue() const;
	/// Sets the current value of the progress bar.
	/// @param[in] value The new value of the progress bar.
	void SetValue(const float value);

protected:
	/// Called during the update loop after children are updated.
	virtual void OnUpdate();
	/// Called during render after backgrounds, borders, decorators, but before children, are rendered.
	virtual void OnRender();

	/// Called when attributes on the element are changed.
	/// @param[in] changed_attributes The attributes changed on the element.
	virtual void OnAttributeChange(const Core::AttributeNameList& changed_attributes);
	/// Called when properties on the element are changed.
	/// @param[in] changed_properties The properties changed on the element.
	virtual void OnPropertyChange(const Core::PropertyNameList& changed_properties);

private:
	/// Called when value has changed.
	void GenerateGeometry();

	/// Called when source texture has changed.
	void LoadTexture(int index);

	/// Called when source texture has changed.
	void LoadTexture(Core::URL & source_url, int index, const char *property_name, Core::Geometry & geometry);

	Core::Texture texture[3];
	Core::Vector2f texcoords[3][2];

	// The geometries used to render this element.
	Core::Geometry left_geometry;
	Core::Geometry center_geometry;
	Core::Geometry right_geometry;
	bool geometry_dirty;

	// The current value of the progress bar.
	float value;
};

}
}

#endif
