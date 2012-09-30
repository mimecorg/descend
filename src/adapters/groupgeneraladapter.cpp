/**************************************************************************
* This file is part of the Descend program
* Copyright (C) 2012 Michał Męciński
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#include "adapters/groupgeneraladapter.h"
#include "project/groupitem.h"

GroupGeneralAdapter::GroupGeneralAdapter( GroupItem* group ) : m_group( group )
{
}

GroupGeneralAdapter::~GroupGeneralAdapter()
{
}

SceneNodeColor::ColorFlags GroupGeneralAdapter::hasColorFlags() const
{
    return SceneNodeColor::DualColors;
}

void GroupGeneralAdapter::setColor( const SceneNodeColor& color )
{
    m_group->setColor( color );
}

SceneNodeColor GroupGeneralAdapter::color() const
{
    return m_group->color();
}
