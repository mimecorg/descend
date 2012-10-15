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

#ifndef ZIPFILE_H
#define ZIPFILE_H

#include <qglobal.h>

class ZipFilePrivate;

class ZipFileReader
{
public:
    explicit ZipFileReader( const QString& path );
    ~ZipFileReader();

public:
    bool open();

    QByteArray fileData( const QString& path ) const;

    void close();

private:
    ZipFilePrivate* d;

    Q_DISABLE_COPY( ZipFileReader )
};

class ZipFileWriter
{
public:
    explicit ZipFileWriter( const QString& path );
    ~ZipFileWriter();

public:
    bool open();

    void addFile( const QString& path, const QByteArray& data, bool compress = true );

    void close();

private:
    ZipFilePrivate* d;

    Q_DISABLE_COPY( ZipFileWriter )
};

#endif
