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

#ifndef MISC_P_H
#define MISC_P_H

#include <QtGlobal>

#if !defined( QT_NO_DEBUG ) && !defined( MISC_NO_DEBUG ) && !defined( MISC_DEBUG )
#define MISC_DEBUG
#endif

#if defined( Q_CC_MSVC )
#define MISC_FORCEINLINE __forceinline
#elif defined( Q_CC_GNU )
#define MISC_FORCEINLINE inline __attribute__((__always_inline__))
#else
#define MISC_FORCEINLINE inline
#endif

namespace Misc
{
    enum Type
    {
        InvalidType,
        VoidType,
        BoolType,
        FloatType,
        VectorType,
        MatrixType,
        LastType = MatrixType
    };

    enum ErrorCode
    {
        NoError,
        SyntaxError,
        TypeError,
        ArgumentError,
        RuntimeError
    };

    enum SymbolFlag
    {
        ConstantSymbol = 1,
        FunctionSymbol = 2
    };

    Q_DECLARE_FLAGS( SymbolFlags, SymbolFlag )
}

Q_DECLARE_OPERATORS_FOR_FLAGS( Misc::SymbolFlags )

#endif
