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

#include "dialogs/propertiesdialog.h"

#include "adapters/projectcodeadapter.h"
#include "adapters/foldercodeadapter.h"
#include "adapters/groupgeneraladapter.h"
#include "adapters/groupcodeadapter.h"
#include "adapters/parametricmeshgeneraladapter.h"
#include "adapters/parametricmeshinitcodeadapter.h"
#include "adapters/parametricmeshcalccodeadapter.h"
#include "project/project.h"
#include "project/folderitem.h"
#include "project/groupitem.h"
#include "project/parametricmeshitem.h"
#include "utils/iconloader.h"
#include "widgets/codepage.h"
#include "widgets/generalpage.h"

PropertiesDialog::PropertiesDialog( ProjectItem* item, QWidget* parent ) : QDialog( parent ),
    m_item( item ),
    m_generalPage( NULL )
{
    m_ui.setupUi( this );

    m_ui.promptPixmap->setPixmap( IconLoader::pixmap( "edit-properties", 22 ) );
    m_ui.promptLabel->setText( tr( "Edit properties of <b>%1</b>:" ).arg( item->name() ) );

    m_ui.promptLabel->setMinimumWidth( 350 );
    m_ui.promptLabel->setFixedHeight( m_ui.promptLabel->heightForWidth( 350 ) );

    switch ( item->type() ) {
        case ProjectItem::Project:
            m_codePages.append( new CodePage( new ProjectCodeAdapter( static_cast<Project*>( item ) ), tr( "Definitions" ), m_ui.tabWidget ) );
            break;

        case ProjectItem::Folder:
            m_codePages.append( new CodePage( new FolderCodeAdapter( static_cast<FolderItem*>( item ) ), tr( "Definitions" ), m_ui.tabWidget ));
            break;

        case ProjectItem::Group:
            m_generalPage = new GeneralPage( new GroupGeneralAdapter( static_cast<GroupItem*>( item ) ), m_ui.tabWidget );
            m_codePages.append( new CodePage( new GroupCodeAdapter( static_cast<GroupItem*>( item ) ), tr( "Initialization" ), m_ui.tabWidget ) );
            break;

        case ProjectItem::Curve:
        case ProjectItem::Surface:
            m_generalPage = new GeneralPage( new ParametricMeshGeneralAdapter( static_cast<ParametricMeshItem*>( item ) ), m_ui.tabWidget );
            m_codePages.append( new CodePage( new ParametricMeshInitCodeAdapter( static_cast<ParametricMeshItem*>( item ) ), tr( "Initialization" ), m_ui.tabWidget ) );
            m_codePages.append( new CodePage( new ParametricMeshCalcCodeAdapter( static_cast<ParametricMeshItem*>( item ) ), tr( "Calculation" ), m_ui.tabWidget ) );
            break;

        default:
            break;
    }

    if ( m_generalPage )
        m_ui.tabWidget->addTab( m_generalPage, tr( "General" ) );

    foreach ( CodePage* page, m_codePages )
        m_ui.tabWidget->addTab( page, page->label() );

    if ( m_generalPage && !m_codePages.isEmpty() ) {
        connect( m_generalPage, SIGNAL( colorChanged() ), this, SLOT( updateInputsOutputs() ) );
        updateInputsOutputs();
    }

    m_ui.buttonBox->button( QDialogButtonBox::Ok )->setText( tr( "&OK" ) );
    m_ui.buttonBox->button( QDialogButtonBox::Cancel )->setText( tr( "&Cancel" ) );

    QWidget* widget = m_ui.tabWidget->widget( 0 );
    while ( widget && widget->focusPolicy() == Qt::NoFocus )
        widget = widget->nextInFocusChain();
    if ( widget )
        widget->setFocus();
}

PropertiesDialog::~PropertiesDialog()
{
}

void PropertiesDialog::accept()
{
    if ( m_generalPage )
        m_generalPage->apply();

    foreach ( CodePage* page, m_codePages )
        page->apply();

    QDialog::accept();
}

void PropertiesDialog::updateInputsOutputs()
{
    Renderer::AttributeType attr = m_generalPage->attributeType();
    SceneNodeColor color = m_generalPage->color();

    foreach ( CodePage* page, m_codePages )
        page->updateInputsOutputs( attr, color );
}
