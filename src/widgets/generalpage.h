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

#ifndef GENERALPAGE_H
#define GENERALPAGE_H

#include "ui_generalpage.h"
#include "scene/renderer.h"

class GeneralAdapter;
class SceneNodeColor;

class GeneralPage : public QWidget
{
    Q_OBJECT
public:
    GeneralPage( GeneralAdapter* adapter, QWidget* parent );
    ~GeneralPage();

public:
    void apply();

    Renderer::AttributeType attributeType() const;
    SceneNodeColor color() const;

signals:
    void colorChanged();

private slots:
    void updateWidgets();

private:
    void populateSecondaryComboBox( int type, int& type2 );

private:
    enum ColorMode
    {
        SingleColor,
        DualColors,
        VertexColor
    };

private:
    Ui::GeneralPage m_ui;

    GeneralAdapter* m_adapter;

    bool m_updating;
};

#endif
