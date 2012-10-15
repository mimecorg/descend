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

#include "utils/dataserializer.h"

#include <QFile>
#include <QBuffer>

int DataSerializer::m_dataVersion = 0;

static const int MagicHeader = 0xD053C7D1;

// increment this value every time data format is changed
static const int CurrentVersion = 1;

// minimum supported version
static const int MinimumVersion = 1;

DataSerializer::DataSerializer( const QString& path ) :
    m_device( new QFile( path ) )
{
}

DataSerializer::DataSerializer( QByteArray* data ) :
    m_device( new QBuffer( data ) )
{
}

DataSerializer::~DataSerializer()
{
    delete m_device;
}

bool DataSerializer::openForReading()
{
    if ( !m_device->open( QIODevice::ReadOnly ) )
        return false;

    m_stream.setDevice( m_device );
    m_stream.setVersion( QDataStream::Qt_4_6 );

    qint32 header;
    m_stream >> header;

    if ( header != MagicHeader )
        return false;

    qint32 version;
    m_stream >> version;

    if ( version < MinimumVersion || version > CurrentVersion )
        return false;

    m_dataVersion = version;

    return true;
}

bool DataSerializer::openForWriting()
{
    if ( !m_device->open( QIODevice::WriteOnly | QIODevice::Truncate ) )
        return false;

    m_stream.setDevice( m_device );
    m_stream.setVersion( QDataStream::Qt_4_6 );

    m_stream << (qint32)MagicHeader;
    m_stream << (qint32)CurrentVersion;

    m_dataVersion = CurrentVersion;

    return true;
}

void DataSerializer::close()
{
    m_device->close();
}
