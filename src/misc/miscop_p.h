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

#ifndef MISCOP_P_H
#define MISCOP_P_H

#include "misc.h"

namespace Misc { namespace OP {

enum Code
{
    push_void,
    push_false,
    push_true,
    push_zerof,
    push_zerov,
    push_zerom,
    push_float, // float value

    make_vec2,
    make_vec3,
    make_vec4,
    make_mat2,
    make_mat3,
    make_mat4,

    op_nop,
    op_negf,
    op_negv,
    op_negm,
    op_notb,

    op_addff,
    op_addvv,
    op_addmm,
    op_subff,
    op_subvv,
    op_submm,
    op_mulff,
    op_mulfv,
    op_mulvf,
    op_mulvv,
    op_mulfm,
    op_mulmf,
    op_mulvm,
    op_mulmv,
    op_mulmm,
    op_divff,
    op_divvf,
    op_divmf,
    op_modff,
    op_powff,
    op_eqbb,
    op_eqff,
    op_eqvv,
    op_eqmm,
    op_neqbb,
    op_neqff,
    op_neqvv,
    op_neqmm,
    op_ltff,
    op_lteff,
    op_gtff,
    op_gteff,
    op_andbb,
    op_orbb,

    fn_sin,
    fn_cos,
    fn_tan,
    fn_asin,
    fn_acos,
    fn_atan,
    fn_atan2,
    fn_sinh,
    fn_cosh,
    fn_tanh,
    fn_asinh,
    fn_acosh,
    fn_atanh,
    fn_exp,
    fn_log,
    fn_sqrt,
    fn_abs,
    fn_sign,
    fn_floor,
    fn_round,
    fn_ceil,
    fn_min,
    fn_max,
    fn_clamp,
    fn_lerp,
    fn_isnan,
    fn_isinf,
    fn_length,
    fn_distance,
    fn_dot,
    fn_cross,
    fn_normalize,
    fn_lerpv,
    fn_identity,
    fn_translate,
    fn_scale,
    fn_scalev,
    fn_rotate,
    fn_transpose,
    fn_determinant,
    fn_inverse,

    load_gvarb, // int index
    load_gvarf, // int index
    load_gvarv, // int index
    load_gvarm, // int index
    load_lvarb, // int index
    load_lvarf, // int index
    load_lvarv, // int index
    load_lvarm, // int index
    load_gcompv, // int index, int comp
    load_gcompm, // int index, int comp
    load_lcompv, // int index, int comp
    load_lcompm, // int index, int comp

    store_gvarb, // int index
    store_gvarf, // int index
    store_gvarv, // int index
    store_gvarm, // int index
    store_lvarb, // int index
    store_lvarf, // int index
    store_lvarv, // int index
    store_lvarm, // int index
    store_gcompv, // int index, int comp
    store_gcompm, // int index, int comp
    store_lcompv, // int index, int comp
    store_lcompm, // int index, int comp

    jmp, // int offset
    jmp_false, // int offset
    jmp_true, // int offset
    jmp_and, // int offset
    jmp_or, // int offset

    call, // int index

    pop,
    pop_n, // int count

    ret,
    ret_n, // int count

    exit
};

struct Instruction
{
    Code m_code;
    union {
        char m_data[ 1 ];
        float m_float;
        int m_arg[ 2 ];
    };
};

static const size_t InstrSize = offsetof( Instruction, m_data );
static const size_t InstrArg1Size = offsetof( Instruction, m_data ) + sizeof( int );
static const size_t InstrArg2Size = offsetof( Instruction, m_data ) + 2 * sizeof( int );
static const size_t InstrFloatSize = offsetof( Instruction, m_data ) + sizeof( float );

} }

#endif
