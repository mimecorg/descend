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

bool GroupGeneralAdapter::hasColor() const
{
    return true;
}

bool GroupGeneralAdapter::hasBackColor() const
{
    return true;
}

void GroupGeneralAdapter::setFrontColor( const QColor& color )
{
    m_group->setFrontColor( color );
}

QColor GroupGeneralAdapter::frontColor() const
{
    return m_group->frontColor();
}

void GroupGeneralAdapter::setBackColor( const QColor& color )
{
    m_group->setBackColor( color );
}

QColor GroupGeneralAdapter::backColor() const
{
    return m_group->backColor();
}
