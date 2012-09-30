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

#ifndef CODEADAPTER_H
#define CODEADAPTER_H

#include "scene/renderer.h"

class SceneNodeColor;

class CodeAdapter
{
public:
    CodeAdapter() { }
    virtual ~CodeAdapter() { }

public:
    virtual void setCode( const QString& /*text*/ ) { }
    virtual QString code() const { return QString(); }

    virtual bool hasInputsOutputs() const { return false; }

    virtual QStringList inputs( Renderer::AttributeType /*attr*/, const SceneNodeColor& /*color*/ ) const { return QStringList(); }
    virtual QStringList outputs( Renderer::AttributeType /*attr*/, const SceneNodeColor& /*color*/ ) const { return QStringList(); }
};

#endif
