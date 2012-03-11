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

#ifndef MISMATH_P_H
#define MISMATH_P_H

#include "miscvalue_p.h"

#include <math.h>
#include <float.h>

namespace Misc { namespace Math {

inline float signf( float x )
{
    return ( x > 0.0f ) ? 1.0f : ( x < 0.0f ) ? -1.0f : x;
}

inline float lerpf( float x, float y, float a )
{
    return x * ( 1.0f - a ) + y * a;
}

template<class Impl>
struct calc_vec
{
    template<typename T>
    static inline void calc( MiscValuePrivate* result, const T& arg )
    {
        result->m_vector[ 0 ] = Impl::template calc<0>( arg );
        result->m_vector[ 1 ] = Impl::template calc<1>( arg );
        result->m_vector[ 2 ] = Impl::template calc<2>( arg );
        result->m_vector[ 3 ] = Impl::template calc<3>( arg );
    }

    template<typename T1, typename T2>
    static inline void calc( MiscValuePrivate* result, const T1& arg1, const T2& arg2 )
    {
        result->m_vector[ 0 ] = Impl::template calc<0>( arg1, arg2 );
        result->m_vector[ 1 ] = Impl::template calc<1>( arg1, arg2 );
        result->m_vector[ 2 ] = Impl::template calc<2>( arg1, arg2 );
        result->m_vector[ 3 ] = Impl::template calc<3>( arg1, arg2 );
    }

    template<typename T1, typename T2, typename T3>
    static inline void calc( MiscValuePrivate* result, const T1& arg1, const T2& arg2, const T3& arg3 )
    {
        result->m_vector[ 0 ] = Impl::template calc<0>( arg1, arg2, arg3 );
        result->m_vector[ 1 ] = Impl::template calc<1>( arg1, arg2, arg3 );
        result->m_vector[ 2 ] = Impl::template calc<2>( arg1, arg2, arg3 );
        result->m_vector[ 3 ] = Impl::template calc<3>( arg1, arg2, arg3 );
    }
};

template<class Impl>
struct calc_row
{
    template<int R>
    static inline void calc( MiscValuePrivate* result )
    {
        result->m_matrix[ R ][ 0 ] = Impl::template calc<R, 0>();
        result->m_matrix[ R ][ 1 ] = Impl::template calc<R, 1>();
        result->m_matrix[ R ][ 2 ] = Impl::template calc<R, 2>();
        result->m_matrix[ R ][ 3 ] = Impl::template calc<R, 3>();
    }

    template<int R, typename T>
    static inline void calc( MiscValuePrivate* result, const T& arg )
    {
        result->m_matrix[ R ][ 0 ] = Impl::template calc<R, 0>( arg );
        result->m_matrix[ R ][ 1 ] = Impl::template calc<R, 1>( arg );
        result->m_matrix[ R ][ 2 ] = Impl::template calc<R, 2>( arg );
        result->m_matrix[ R ][ 3 ] = Impl::template calc<R, 3>( arg );
    }

    template<int R, typename T1, typename T2>
    static inline void calc( MiscValuePrivate* result, const T1& arg1, const T2& arg2 )
    {
        result->m_matrix[ R ][ 0 ] = Impl::template calc<R, 0>( arg1, arg2 );
        result->m_matrix[ R ][ 1 ] = Impl::template calc<R, 1>( arg1, arg2 );
        result->m_matrix[ R ][ 2 ] = Impl::template calc<R, 2>( arg1, arg2 );
        result->m_matrix[ R ][ 3 ] = Impl::template calc<R, 3>( arg1, arg2 );
    }
};

template<class Impl>
struct calc_mat
{
    static inline void calc( MiscValuePrivate* result )
    {
        calc_row<Impl>::template calc<0>( result );
        calc_row<Impl>::template calc<1>( result );
        calc_row<Impl>::template calc<2>( result );
        calc_row<Impl>::template calc<3>( result );
    }

    template<typename T>
    static inline void calc( MiscValuePrivate* result, const T& arg )
    {
        calc_row<Impl>::template calc<0>( result, arg );
        calc_row<Impl>::template calc<1>( result, arg );
        calc_row<Impl>::template calc<2>( result, arg );
        calc_row<Impl>::template calc<3>( result, arg );
    }

    template<typename T1, typename T2>
    static inline void calc( MiscValuePrivate* result, const T1& arg1, const T2& arg2 )
    {
        calc_row<Impl>::template calc<0>( result, arg1, arg2 );
        calc_row<Impl>::template calc<1>( result, arg1, arg2 );
        calc_row<Impl>::template calc<2>( result, arg1, arg2 );
        calc_row<Impl>::template calc<3>( result, arg1, arg2 );
    }
};

template<class Impl>
struct forall_vec
{
    template<typename T1, typename T2>
    static inline bool calc( const T1& arg1, const T2& arg2 )
    {
        return Impl::template calc<0>( arg1, arg2 ) && Impl::template calc<1>( arg1, arg2 )
            && Impl::template calc<2>( arg1, arg2 ) && Impl::template calc<3>( arg1, arg2 );
    }
};

template<class Impl>
struct forall_row
{
    template<int R, typename T1, typename T2>
    static inline bool calc( const T1& arg1, const T2& arg2 )
    {
        return Impl::template calc<R, 0>( arg1, arg2 ) && Impl::template calc<R, 1>( arg1, arg2 )
            && Impl::template calc<R, 2>( arg1, arg2 ) && Impl::template calc<R, 3>( arg1, arg2 );
    }
};

template<class Impl>
struct forall_mat
{
    template<typename T1, typename T2>
    static inline bool calc( const T1& arg1, const T2& arg2 )
    {
        return forall_row<Impl>::template calc<0>( arg1, arg2 ) && forall_row<Impl>::template calc<1>( arg1, arg2 )
            && forall_row<Impl>::template calc<2>( arg1, arg2 ) && forall_row<Impl>::template calc<3>( arg1, arg2 );
    }
};

template<class Impl>
struct sum_float
{
    template<typename T>
    static inline float calc( const T& arg )
    {
        return Impl::template calc<0>( arg ) + Impl::template calc<1>( arg )
             + Impl::template calc<2>( arg ) + Impl::template calc<3>( arg );
    }

    template<typename T1, typename T2>
    static inline float calc( const T1& arg1, const T2& arg2 )
    {
        return Impl::template calc<0>( arg1, arg2 ) + Impl::template calc<1>( arg1, arg2 )
             + Impl::template calc<2>( arg1, arg2 ) + Impl::template calc<3>( arg1, arg2 );
    }
};

template<class Impl>
struct sum_vec
{
    template<int I, typename T1, typename T2>
    static inline float calc( const T1& arg1, const T2& arg2 )
    {
        return Impl::template calc<I, 0>( arg1, arg2 ) + Impl::template calc<I, 1>( arg1, arg2 )
             + Impl::template calc<I, 2>( arg1, arg2 ) + Impl::template calc<I, 3>( arg1, arg2 );
    }
};

template<class Impl>
struct sum_mat
{
    template<int R, int C, typename T1, typename T2>
    static inline float calc( const T1& arg1, const T2& arg2 )
    {
        return Impl::template calc<R, C, 0>( arg1, arg2 ) + Impl::template calc<R, C, 1>( arg1, arg2 )
             + Impl::template calc<R, C, 2>( arg1, arg2 ) + Impl::template calc<R, C, 3>( arg1, arg2 );
    }
};

struct eqvv_impl
{
    template<int I>
    static inline bool calc( const MiscValuePrivate* lhs, const MiscValuePrivate* rhs )
    {
        return lhs->m_vector[ I ] == rhs->m_vector[ I ];
    }
};

inline bool eqvv( const MiscValuePrivate* lhs, const MiscValuePrivate* rhs )
{
    return forall_vec<eqvv_impl>::calc( lhs, rhs );
}

struct eqmm_impl
{
    template<int R, int C>
    static inline bool calc( const MiscValuePrivate* lhs, const MiscValuePrivate* rhs )
    {
        return lhs->m_matrix[ R ][ C ] == rhs->m_matrix[ R ][ C ];
    }
};

inline bool eqmm( const MiscValuePrivate* lhs, const MiscValuePrivate* rhs )
{
    return forall_mat<eqmm_impl>::calc( lhs, rhs );
}

struct copyv_impl
{
    template<int I>
    static inline float calc( const MiscValuePrivate* src )
    {
        return src->m_vector[ I ];
    }
};

inline void copyv( MiscValuePrivate* dest, const MiscValuePrivate* src )
{
    return calc_vec<copyv_impl>::calc( dest, src );
}

struct copym_impl
{
    template<int R, int C>
    static inline float calc( const MiscValuePrivate* src )
    {
        return src->m_matrix[ R ][ C ];
    }
};

inline void copym( MiscValuePrivate* dest, const MiscValuePrivate* src )
{
    return calc_mat<copym_impl>::calc( dest, src );
}

template<int N>
struct makev_impl
{
    template<int I>
    static inline float calc( MiscSlot* top )
    {
        if ( I < N )
            return top[ I - N ].m_float;
        else
            return 0.0f;
    }
};

template<int N>
inline void makev( MiscValuePrivate* result, MiscSlot* top )
{
    calc_vec< makev_impl<N> >::calc( result, top );
}

template<int N>
struct makem_impl
{
    template<int R, int C>
    static inline float calc( MiscSlot* top )
    {
        if ( R < N && C < N )
            return top[ N * R + C - N * N ].m_float;
        else if ( R == C )
            return 1.0f;
        else
            return 0.0f;
    }
};

template<int N>
inline void makem( MiscValuePrivate* result, MiscSlot* top )
{
    calc_mat< makem_impl<N> >::calc( result, top );
}

struct negv_impl
{
    template<int I>
    static inline float calc( MiscValuePrivate* arg )
    {
        return -arg->m_vector[ I ];
    }
};

inline void negv( MiscValuePrivate* result, MiscValuePrivate* arg )
{
    calc_vec<negv_impl>::calc( result, arg );
}

struct negm_impl
{
    template<int R, int C>
    static inline float calc( MiscValuePrivate* arg )
    {
        return -arg->m_matrix[ R ][ C ];
    }
};

inline void negm( MiscValuePrivate* result, MiscValuePrivate* arg )
{
    calc_mat<negm_impl>::calc( result, arg );
}

struct addvv_impl
{
    template<int I>
    static inline float calc( MiscValuePrivate* lhs, MiscValuePrivate* rhs )
    {
        return lhs->m_vector[ I ] + rhs->m_vector[ I ];
    }
};

inline void addvv( MiscValuePrivate* result, MiscValuePrivate* lhs, MiscValuePrivate* rhs )
{
    calc_vec<addvv_impl>::calc( result, lhs, rhs );
}

struct addmm_impl
{
    template<int R, int C>
    static inline float calc( MiscValuePrivate* lhs, MiscValuePrivate* rhs )
    {
        return lhs->m_matrix[ R ][ C ] + rhs->m_matrix[ R ][ C ];
    }
};

inline void addmm( MiscValuePrivate* result, MiscValuePrivate* lhs, MiscValuePrivate* rhs )
{
    calc_mat<addmm_impl>::calc( result, lhs, rhs );
}

struct subvv_impl
{
    template<int I>
    static inline float calc( MiscValuePrivate* lhs, MiscValuePrivate* rhs )
    {
        return lhs->m_vector[ I ] - rhs->m_vector[ I ];
    }
};

inline void subvv( MiscValuePrivate* result, MiscValuePrivate* lhs, MiscValuePrivate* rhs )
{
    calc_vec<subvv_impl>::calc( result, lhs, rhs );
}

struct submm_impl
{
    template<int R, int C>
    static inline float calc( MiscValuePrivate* lhs, MiscValuePrivate* rhs )
    {
        return lhs->m_matrix[ R ][ C ] - rhs->m_matrix[ R ][ C ];
    }
};

inline void submm( MiscValuePrivate* result, MiscValuePrivate* lhs, MiscValuePrivate* rhs )
{
    calc_mat<submm_impl>::calc( result, lhs, rhs );
}

struct mulfv_impl
{
    template<int I>
    static inline float calc( float lhs, MiscValuePrivate* rhs )
    {
        return lhs * rhs->m_vector[ I ];
    }
};

inline void mulfv( MiscValuePrivate* result, float lhs, MiscValuePrivate* rhs )
{
    calc_vec<mulfv_impl>::calc( result, lhs, rhs );
}

struct mulvf_impl
{
    template<int I>
    static inline float calc( MiscValuePrivate* lhs, float rhs )
    {
        return lhs->m_vector[ I ] * rhs;
    }
};

inline void mulvf( MiscValuePrivate* result, MiscValuePrivate* lhs, float rhs )
{
    calc_vec<mulvf_impl>::calc( result, lhs, rhs );
}

struct mulvv_impl
{
    template<int I>
    static inline float calc( MiscValuePrivate* lhs, MiscValuePrivate* rhs )
    {
        return lhs->m_vector[ I ] * rhs->m_vector[ I ];
    }
};

inline float mulvv( MiscValuePrivate* lhs, MiscValuePrivate* rhs )
{
    return sum_float<mulvv_impl>::calc( lhs, rhs );
}

struct mulfm_impl
{
    template<int R, int C>
    static inline float calc( float lhs, MiscValuePrivate* rhs )
    {
        return lhs * rhs->m_matrix[ R ][ C ];
    }
};

inline void mulfm( MiscValuePrivate* result, float lhs, MiscValuePrivate* rhs )
{
    calc_mat<mulfm_impl>::calc( result, lhs, rhs );
}

struct mulmf_impl
{
    template<int R, int C>
    static inline float calc( MiscValuePrivate* lhs, float rhs )
    {
        return lhs->m_matrix[ R ][ C ] * rhs;
    }
};

inline void mulmf( MiscValuePrivate* result, MiscValuePrivate* lhs, float rhs )
{
    calc_mat<mulmf_impl>::calc( result, lhs, rhs );
}

struct mulvm_impl
{
    template<int I, int K>
    static inline float calc( MiscValuePrivate* lhs, MiscValuePrivate* rhs )
    {
        return lhs->m_vector[ K ] * rhs->m_matrix[ K ][ I ];
    }
};

inline void mulvm( MiscValuePrivate* result, MiscValuePrivate* lhs, MiscValuePrivate* rhs )
{
    calc_vec< sum_vec<mulvm_impl> >::calc( result, lhs, rhs );
}

struct mulmv_impl
{
    template<int I, int K>
    static inline float calc( MiscValuePrivate* lhs, MiscValuePrivate* rhs )
    {
        return lhs->m_matrix[ I ][ K ] * rhs->m_vector[ K ];
    }
};

inline void mulmv( MiscValuePrivate* result, MiscValuePrivate* lhs, MiscValuePrivate* rhs )
{
    calc_vec< sum_vec<mulmv_impl> >::calc( result, lhs, rhs );
}

struct mulmm_impl
{
    template<int R, int C, int K>
    static inline float calc( MiscValuePrivate* lhs, MiscValuePrivate* rhs )
    {
        return lhs->m_matrix[ R ][ K ] * rhs->m_matrix[ K ][ C ];
    }
};

inline void mulmm( MiscValuePrivate* result, MiscValuePrivate* lhs, MiscValuePrivate* rhs )
{
    calc_mat< sum_mat<mulmm_impl> >::calc( result, lhs, rhs );
}

struct length_impl
{
    template<int I>
    static inline float calc( MiscValuePrivate* arg )
    {
        return arg->m_vector[ I ] * arg->m_vector[ I ];
    }
};

inline float length( MiscValuePrivate* arg )
{
    return sqrtf( sum_float<length_impl>::calc( arg ) );
}

struct distance_impl
{
    template<int I>
    static inline float calc( MiscValuePrivate* arg1, MiscValuePrivate* arg2 )
    {
        return ( arg2->m_vector[ I ] - arg1->m_vector[ I ] ) * ( arg2->m_vector[ I ] - arg1->m_vector[ I ] );
    }
};

inline float distance( MiscValuePrivate* arg1, MiscValuePrivate* arg2 )
{
    return sqrtf( sum_float<distance_impl>::calc( arg1, arg2 ) );
}

struct cross_impl
{
    template<int I>
    static inline float calc( MiscValuePrivate* arg1, MiscValuePrivate* arg2 )
    {
        if ( I < 3 ) {
            return arg1->m_vector[ ( I + 1 ) % 3 ] * arg2->m_vector[ ( I + 2 ) % 3 ]
                 - arg1->m_vector[ ( I + 2 ) % 3 ] * arg2->m_vector[ ( I + 1 ) % 3 ];
        } else {
            return 0.0f;
        }
    }
};

inline void cross( MiscValuePrivate* result, MiscValuePrivate* arg1, MiscValuePrivate* arg2 )
{
    calc_vec<cross_impl>::calc( result, arg1, arg2 );
}

inline void normalize( MiscValuePrivate* result, MiscValuePrivate* arg )
{
    float len = sqrtf( sum_float<length_impl>::calc( arg ) );
    calc_vec<mulvf_impl>::calc( result, arg, 1.0f / len );
}

struct lerpv_impl
{
    template<int I>
    static inline float calc( MiscValuePrivate* x, MiscValuePrivate* y, float a )
    {
        return lerpf( x->m_vector[ I ], y->m_vector[ I ], a );
    }
};

inline void lerpv( MiscValuePrivate* result, MiscValuePrivate* x, MiscValuePrivate* y, float a )
{
    calc_vec<lerpv_impl>::calc( result, x, y, a );
}

struct identity_impl
{
    template<int R, int C>
    static inline float calc()
    {
        if ( R == C )
            return 1.0f;
        else
            return 0.0f;
    }
};

inline void identity( MiscValuePrivate* result )
{
    calc_mat<identity_impl>::calc( result );
}

struct translate_impl
{
    template<int R, int C>
    static inline float calc( MiscValuePrivate* arg )
    {
        if ( R == C )
            return 1.0f;
        else if ( C == 3 )
            return arg->m_vector[ R ];
        else
            return 0.0f;
    }
};

inline void translate( MiscValuePrivate* result, MiscValuePrivate* arg )
{
    calc_mat<translate_impl>::calc( result, arg );
}

struct scale_impl
{
    template<int R, int C>
    static inline float calc( MiscValuePrivate* arg )
    {
        if ( R == 3 && C == 3 )
            return 1.0f;
        else if ( R == C )
            return arg->m_vector[ R ];
        else
            return 0.0f;
    }
};

inline void scale( MiscValuePrivate* result, MiscValuePrivate* arg )
{
    calc_mat<scale_impl>::calc( result, arg );
}

inline void rotate( MiscValuePrivate* result, float t, MiscValuePrivate* u )
{
    float s = sinf( t );
    float c = cosf( t );
    float ic = 1.0f - c;

    float norm = 1.0f / sqrtf( u->m_vector[ 0 ] * u->m_vector[ 0 ] + u->m_vector[ 1 ] * u->m_vector[ 1 ] + u->m_vector[ 2 ] * u->m_vector[ 2 ] );
    float x = u->m_vector[ 0 ] * norm;
    float y = u->m_vector[ 1 ] * norm;
    float z = u->m_vector[ 2 ] * norm;

    result->m_matrix[ 0 ][ 0 ] = c + x * x * ic;
    result->m_matrix[ 0 ][ 1 ] = x * y * ic - z * s;
    result->m_matrix[ 0 ][ 2 ] = x * z * ic + y * s;
    result->m_matrix[ 0 ][ 3 ] = 0.0f;
    result->m_matrix[ 1 ][ 0 ] = y * x * ic + z * s;
    result->m_matrix[ 1 ][ 1 ] = c + y * y * ic;
    result->m_matrix[ 1 ][ 2 ] = y * z * ic - x * s;
    result->m_matrix[ 1 ][ 3 ] = 0.0f;
    result->m_matrix[ 2 ][ 0 ] = z * x * ic - y * s;
    result->m_matrix[ 2 ][ 1 ] = z * y * ic + x * s;
    result->m_matrix[ 2 ][ 2 ] = c + z * z * ic;
    result->m_matrix[ 2 ][ 3 ] = 0.0f;
    result->m_matrix[ 3 ][ 0 ] = 0.0f;
    result->m_matrix[ 3 ][ 1 ] = 0.0f;
    result->m_matrix[ 3 ][ 2 ] = 0.0f;
    result->m_matrix[ 3 ][ 3 ] = 1.0f;
}

struct transpose_impl
{
    template<int R, int C>
    static inline float calc( MiscValuePrivate* arg )
    {
        return arg->m_matrix[ C ][ R ];
    }
};

inline void transpose( MiscValuePrivate* result, MiscValuePrivate* arg )
{
    calc_mat<transpose_impl>::calc( result, arg );
}

struct det_impl
{
    template<int R0, int R1, int C0, int C1>
    static inline float det2( MiscValuePrivate* arg )
    {
        return arg->m_matrix[ R0 ][ C0 ] * arg->m_matrix[ R1 ][ C1 ]
             - arg->m_matrix[ R1 ][ C0 ] * arg->m_matrix[ R0 ][ C1 ];
    }

    template<int R0, int R1, int R2, int C0, int C1, int C2>
    static inline float det3( MiscValuePrivate* arg )
    {
        return arg->m_matrix[ R0 ][ C0 ] * det2<R1, R2, C1, C2>( arg )
             - arg->m_matrix[ R0 ][ C1 ] * det2<R1, R2, C0, C2>( arg )
             + arg->m_matrix[ R0 ][ C2 ] * det2<R1, R2, C0, C1>( arg );
    }
};

inline float determinant( MiscValuePrivate* arg )
{
    return arg->m_matrix[ 0 ][ 0 ] * det_impl::det3<1, 2, 3, 1, 2, 3>( arg )
         - arg->m_matrix[ 0 ][ 1 ] * det_impl::det3<1, 2, 3, 0, 2, 3>( arg )
         + arg->m_matrix[ 0 ][ 2 ] * det_impl::det3<1, 2, 3, 0, 1, 3>( arg )
         - arg->m_matrix[ 0 ][ 3 ] * det_impl::det3<1, 2, 3, 0, 1, 2>( arg );
}

inline void inverse( MiscValuePrivate* result, MiscValuePrivate* arg )
{
    float det = 1.0f / determinant( arg );
    result->m_matrix[ 0 ][ 0 ] = det_impl::det3<1, 2, 3, 1, 2, 3>( arg ) * det;
    result->m_matrix[ 0 ][ 1 ] = -det_impl::det3<1, 2, 3, 0, 2, 3>( arg ) * det;
    result->m_matrix[ 0 ][ 2 ] = det_impl::det3<1, 2, 3, 0, 1, 3>( arg ) * det;
    result->m_matrix[ 0 ][ 3 ] = -det_impl::det3<1, 2, 3, 0, 1, 2>( arg ) * det;
    result->m_matrix[ 1 ][ 0 ] = -det_impl::det3<0, 2, 3, 1, 2, 3>( arg ) * det;
    result->m_matrix[ 1 ][ 1 ] = det_impl::det3<0, 2, 3, 0, 2, 3>( arg ) * det;
    result->m_matrix[ 1 ][ 2 ] = -det_impl::det3<0, 2, 3, 0, 1, 3>( arg ) * det;
    result->m_matrix[ 1 ][ 3 ] = det_impl::det3<0, 2, 3, 0, 1, 2>( arg ) * det;
    result->m_matrix[ 2 ][ 0 ] = det_impl::det3<0, 1, 3, 1, 2, 3>( arg ) * det;
    result->m_matrix[ 2 ][ 1 ] = -det_impl::det3<0, 1, 3, 0, 2, 3>( arg ) * det;
    result->m_matrix[ 2 ][ 2 ] = det_impl::det3<0, 1, 3, 0, 1, 3>( arg ) * det;
    result->m_matrix[ 2 ][ 3 ] = -det_impl::det3<0, 1, 3, 0, 1, 2>( arg ) * det;
    result->m_matrix[ 3 ][ 0 ] = -det_impl::det3<0, 1, 2, 1, 2, 3>( arg ) * det;
    result->m_matrix[ 3 ][ 1 ] = det_impl::det3<0, 1, 2, 0, 2, 3>( arg ) * det;
    result->m_matrix[ 3 ][ 2 ] = -det_impl::det3<0, 1, 2, 0, 1, 3>( arg ) * det;
    result->m_matrix[ 3 ][ 3 ] = det_impl::det3<0, 1, 2, 0, 1, 2>( arg ) * det;
}

} }

#if defined( Q_CC_MSVC )

inline float asinhf( float x )
{
    return logf( x + sqrtf( x * x + 1.0f ) );
}

inline float acoshf( float x )
{
    return logf( x + sqrtf( x * x - 1.0f ) );
}

inline float atanhf( float x )
{
    return logf( ( 1.0f + x ) / ( 1.0f - x ) ) / 2.0f;
}

inline float roundf( float x )
{
    return ( x > 0.0f ) ? floorf( x + 0.5f ) : ceilf( x - 0.5f );
}

inline bool isnanf( float x )
{
    return !!_isnan( (double)x );
}

inline bool isinff( float x )
{
    return !_finite( (double)x ) && !_isnan( (double)x );
}

#endif // defined( Q_CC_MSVC )

#endif
