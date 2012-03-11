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

#ifndef MISCIDENTIFIER_H
#define MISCIDENTIFIER_H

#include "misc.h"

class MiscIdentifierPrivate;
class MiscEngine;

class QString;
class QStringRef;

class MiscIdentifier
{
public:
    MiscIdentifier();
    MiscIdentifier( const MiscIdentifier& other );
    MiscIdentifier( const QString& name, MiscEngine* engine );
    MiscIdentifier( const QStringRef& name, MiscEngine* engine );
    ~MiscIdentifier();

public:
    bool isValid() const { return d != NULL; }

    QString name() const;

public:
    MiscIdentifier& operator =( const MiscIdentifier& other );

    friend bool operator ==( const MiscIdentifier& lhs, const MiscIdentifier& rhs );
    friend bool operator !=( const MiscIdentifier& lhs, const MiscIdentifier& rhs );

private:
    explicit MiscIdentifier( MiscIdentifierPrivate* data );

private:
    MiscIdentifierPrivate* d;

    friend class MiscIdentifierPrivate;
};

#endif
