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

#ifndef PARAMETRICMESHCALCCODEADAPTER_H
#define PARAMETRICMESHCALCCODEADAPTER_H

#include "adapters/codeadapter.h"

class ParametricMeshItem;

class ParametricMeshCalcCodeAdapter : public CodeAdapter
{
public:
    ParametricMeshCalcCodeAdapter( ParametricMeshItem* mesh );
    ~ParametricMeshCalcCodeAdapter();

public:
    void setCode( const QString& text );
    QString code() const;

    bool hasInputsOutputs() const;

    QStringList inputs( Renderer::AttributeType attr, const SceneNodeColor& color ) const;
    QStringList outputs( Renderer::AttributeType attr, const SceneNodeColor& color ) const;

private:
    ParametricMeshItem* m_mesh;
};

#endif
