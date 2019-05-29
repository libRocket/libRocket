/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "../../Include/Rocket/Controls/ElementDataGrid.h"
#include "../../Include/Rocket/Controls/DataSource.h"
#include "../../Include/Rocket/Core/Math.h"
#include "../../Include/Rocket/Core/XMLParser.h"
#include "../../Include/Rocket/Core/Event.h"
#include "../../Include/Rocket/Core/ElementDocument.h"
#include "../../Include/Rocket/Core/Factory.h"
#include "../../Include/Rocket/Core/Property.h"
#include "../../Include/Rocket/Controls/DataFormatter.h"
#include "../../Include/Rocket/Controls/ElementDataGridRow.h"

namespace Rocket {
namespace Controls {

ElementDataGrid::ElementDataGrid(const Rocket::Core::String& tag) : Core::Element(tag)
{
	Rocket::Core::XMLAttributes attributes;

	// Create the row for the column headers:
	header = static_cast< ElementDataGridRow* >(Core::Factory::InstanceElement(this, "#rktctl_datagridrow", "datagridheader", attributes));
	header->SetProperty("display", Core::Property(Core::Style::Display::Block));
	header->Initialise(this);
	AppendChild(header);
	header->RemoveReference();

	body = Core::Factory::InstanceElement(this, "*", "datagridbody", attributes);
	body->SetProperty("display", Core::Property(Core::Style::Display::None));
	body->SetProperty("width", Core::Property(Core::Style::Width::Auto));
	AppendChild(body);
	body->RemoveReference();

	body_visible = false;

	root = static_cast< ElementDataGridRow* >(Core::Factory::InstanceElement(this, "#rktctl_datagridrow", "datagridroot", attributes));
	root->SetProperty("display", Core::Property(Core::Style::Display::None));
	root->Initialise(this);

	SetProperty("overflow", Core::Property(Core::Style::Overflow::Auto));

	new_data_source = "";
}

ElementDataGrid::~ElementDataGrid()
{
	root->RemoveReference();
}

void ElementDataGrid::SetDataSource(const Rocket::Core::String& data_source_name)
{
	new_data_source = data_source_name;
}

// Adds a column to the table.
bool ElementDataGrid::AddColumn(const Rocket::Core::String& fields, const Rocket::Core::String& formatter, float initial_width, const Rocket::Core::String& header_rml)
{
	Core::Element* header_element = Core::Factory::InstanceElement(this, "datagridcolumn", "datagridcolumn", Rocket::Core::XMLAttributes());
	if (header_element == NULL)
		return false;

	if (!Core::Factory::InstanceElementText(header_element, header_rml))
	{
		header_element->RemoveReference();
		return false;
	}

	AddColumn(fields, formatter, initial_width, header_element);
	header_element->RemoveReference();
	return true;
}

// Adds a column to the table.
void ElementDataGrid::AddColumn(const Rocket::Core::String& fields, const Rocket::Core::String& formatter, float initial_width, Core::Element* header_element)
{
	Column column;
	Rocket::Core::StringUtilities::ExpandString(column.fields, fields);
	column.formatter = DataFormatter::GetDataFormatter(formatter);
	column.header = header;
	column.current_width = initial_width;
	column.refresh_on_child_change = false;

	// The header elements are added to the header row at the top of the table.
	if (header_element)
	{
		header_element->SetProperty("display", Core::Property(Core::Style::Display::InlineBlock));

		// Push all the width properties from the column onto the header element.
		Rocket::Core::String width = header_element->GetAttribute<Rocket::Core::String>("width", "100%");
		header_element->SetProperty("width", width);

		header->AppendChild(header_element);
	}

	// Add the fields that this column requires to the concatenated string of all column fields.
	for (size_t i = 0; i < column.fields.size(); i++)
	{
		// Don't append the depth or num_children fields, as they're handled by the row.
		if (column.fields[i] == Rocket::Controls::DataSource::NUM_CHILDREN)
		{
			column.refresh_on_child_change = true;
		}
		else if (column.fields[i] != Rocket::Controls::DataSource::DEPTH)
		{
			if (!column_fields.empty())
			{
				column_fields += ",";
			}
			column_fields += column.fields[i];
		}
	}

	columns.push_back(column);

	Rocket::Core::Dictionary parameters;
	parameters["index"] = (int)(columns.size() - 1);
	DispatchEvent("columnadd", parameters, false, false, Core::DefaultActionPhase::Target);
}

// Returns the number of columns in this table
int ElementDataGrid::GetNumColumns()
{
	return (int)columns.size();
}

// Returns the column at the specified index.
const ElementDataGrid::Column* ElementDataGrid::GetColumn(int column_index)
{
	if (column_index < 0 || column_index >= (int)columns.size())
	{
		ROCKET_ERROR;
		return NULL;
	}

	return &columns[column_index];
}

/// Returns a CSV string containing all the fields that each column requires, in order.
const Rocket::Core::String& ElementDataGrid::GetAllColumnFields()
{
	return column_fields;
}

// Adds a new row to the table - usually only called by child rows.
ElementDataGridRow* ElementDataGrid::AddRow(ElementDataGridRow* parent, int index)
{
	// Now we make a new row at the right place then return it.
	Rocket::Core::XMLAttributes attributes;
	ElementDataGridRow* new_row = dynamic_cast< ElementDataGridRow* >(Core::Factory::InstanceElement(this, "#rktctl_datagridrow", "datagridrow", attributes));

	new_row->Initialise(this, parent, index, header, parent->GetDepth() + 1);

	// We need to work out the table-specific row.
	int table_relative_index = parent->GetChildTableRelativeIndex(index);

	Core::Element* child_to_insert_before = NULL;
	if (table_relative_index < body->GetNumChildren())
	{
		child_to_insert_before = body->GetChild(table_relative_index);
	}
	body->InsertBefore(new_row, child_to_insert_before);
	new_row->RemoveReference();

	// As the rows have changed, we need to lay out the document again.
	DirtyLayout();

	return new_row;
}

// Removes a row from the table.
void ElementDataGrid::RemoveRows(int index, int num_rows)
{
	for (int i = 0; i < num_rows; i++)
	{
		ElementDataGridRow* row = GetRow(index);
		row->SetDataSource("");
		body->RemoveChild(row);
	}

	// As the rows have changed, we need to lay out the document again.
	DirtyLayout();
}

// Returns the number of rows in the table
int ElementDataGrid::GetNumRows() const
{
	return body->GetNumChildren();
}

// Returns the row at the given index in the table.
ElementDataGridRow* ElementDataGrid::GetRow(int index) const
{
	// We need to add two to the index, to skip the header row.
	ElementDataGridRow* row = dynamic_cast< ElementDataGridRow* >(body->GetChild(index));
	return row;
}

void ElementDataGrid::OnUpdate()
{
	if (!new_data_source.empty())
	{
		root->SetDataSource(new_data_source);
		new_data_source = "";
	}

	bool any_new_children = root->UpdateChildren();
	if (any_new_children)
	{
		// @performance: Does anyone really use this?
		DispatchEvent("rowupdate", Rocket::Core::Dictionary(), false, true, Core::DefaultActionPhase::None);
	}
	
	if (!body_visible && (!any_new_children || root->GetNumLoadedChildren() >= GetAttribute("min-rows", 0)))
	{
		body->SetProperty("display", Core::Property(Core::Style::Display::Block));
		body_visible = true;
	}
}


void ElementDataGrid::OnResize()
{
	SetScrollTop(GetScrollHeight() - GetClientHeight());

	for (int i = 0; i < header->GetNumChildren(); i++)
	{
		Core::Element* child = header->GetChild(i);
		columns[i].current_width = child->GetBox().GetSize(Core::Box::MARGIN).x;
	}
}

void ElementDataGrid::ProcessEvent(Core::Event& event)
{
	Core::Element::ProcessEvent(event);

	if (event == "columnadd")
	{
		if (event.GetTargetElement() == this)
		{
			root->RefreshRows();
			DirtyLayout();
		}
	}
}

// Gets the markup and content of the element.
void ElementDataGrid::GetInnerRML(Rocket::Core::String& content) const
{
	// The only content we have is the columns, and inside them the header elements.
	for (size_t i = 0; i < columns.size(); i++)
	{
		Core::Element* header_element = header->GetChild((int)i);

		Rocket::Core::String column_fields;
		for (size_t j = 0; j < columns[i].fields.size(); j++)
		{
			if (j != columns[i].fields.size() - 1)
			{
				column_fields += ",";
			}
			column_fields += columns[i].fields[j];
		}
		Rocket::Core::String width_attribute = header_element->GetAttribute<Rocket::Core::String>("width", "");

		content += Core::CreateString(column_fields.size() + 32, "<col fields=\"%s\"", column_fields.c_str());
		if (!width_attribute.empty())
			content += Core::CreateString(width_attribute.size() + 32, " width=\"%s\"", width_attribute.c_str());
		content += ">";
		header_element->GetInnerRML(content);
		content += "</col>";
	}
}

}
}
