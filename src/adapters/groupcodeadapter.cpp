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

#include "adapters/groupcodeadapter.h"
#include "project/groupitem.h"

GroupCodeAdapter::GroupCodeAdapter( GroupItem* group ) : m_group( group )
{
}

GroupCodeAdapter::~GroupCodeAdapter()
{
}

void GroupCodeAdapter::setCode( const QString& text )
{
    m_group->setCode( text );
}

QString GroupCodeAdapter::code() const
{
    return m_group->code();
}

bool GroupCodeAdapter::hasInputsOutputs() const
{
    return true;
}

QStringList GroupCodeAdapter::inputs() const
{
    return QStringList();
}

QStringList GroupCodeAdapter::outputs() const
{
    return QStringList() << "m_matrix";
}
