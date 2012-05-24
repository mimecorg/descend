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

#ifndef DATASERIALIZER_H
#define DATASERIALIZER_H

class DataSerializer
{
public:
    explicit DataSerializer( const QString& path );
    explicit DataSerializer( QByteArray* data );
    ~DataSerializer();

public:
    bool openForReading();
    bool openForWriting();

    void close();

    QDataStream& stream() { return m_stream; }

    static int dataVersion() { return m_dataVersion; }

private:
    QIODevice* m_device;
    QDataStream m_stream;

    static int m_dataVersion;
};

#endif
