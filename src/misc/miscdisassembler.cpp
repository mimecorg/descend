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

#include "miscdisassembler_p.h"

#if defined( MISC_DEBUG )

#include "miscop_p.h"

#include <QTextStream>

MiscDisassembler::MiscDisassembler( QTextStream& stream ) :
    m_stream( stream )
{
}

MiscDisassembler::~MiscDisassembler()
{
}

#define INSTR( code ) \
    case Misc::OP::code: \
        m_stream << #code; \
        cp += Misc::OP::InstrSize; \
        break;

#define INSTR_FLOAT( code ) \
    case Misc::OP::code: { \
        m_stream << qSetFieldWidth( 16 ) << qSetPadChar( ' ' ) << left << #code << reset; \
        m_stream << forcepoint << instr->m_float << reset << "f"; \
        cp += Misc::OP::InstrFloatSize; \
        break; \
    }

#define INSTR_INT( code ) \
    case Misc::OP::code: { \
        m_stream << qSetFieldWidth( 16 ) << qSetPadChar( ' ' ) << left << #code; \
        m_stream << hex << qSetFieldWidth( 8 ) << qSetPadChar( '0' ) << right << instr->m_arg[ 0 ] << reset; \
        cp += Misc::OP::InstrArg1Size; \
        break; \
    }

#define INSTR_INT2( code ) \
    case Misc::OP::code: { \
        m_stream << qSetFieldWidth( 16 ) << qSetPadChar( ' ' ) << left << #code; \
        m_stream << hex << qSetFieldWidth( 8 ) << qSetPadChar( '0' ) << right << instr->m_arg[ 0 ] << reset; \
        m_stream << ", " << hex << qSetFieldWidth( 8 ) << qSetPadChar( '0' ) << right << instr->m_arg[ 1 ] << reset; \
        cp += Misc::OP::InstrArg2Size; \
        break; \
    }

void MiscDisassembler::print( char* ops, size_t size )
{
    size_t cp = 0;

    while ( cp < size ) {
        m_stream << hex << qSetFieldWidth( 8 ) << qSetPadChar( '0' ) << right << cp << reset << ": ";

        Misc::OP::Instruction* instr = reinterpret_cast<Misc::OP::Instruction*>( ops + cp );

        switch ( instr->m_code ) {
            INSTR( push_void )
            INSTR( push_false )
            INSTR( push_true )
            INSTR( push_zerof )
            INSTR( push_zerov )
            INSTR( push_zerom )
            INSTR_FLOAT( push_float )
            INSTR( make_vec2 )
            INSTR( make_vec3 )
            INSTR( make_vec4 )
            INSTR( make_mat2 )
            INSTR( make_mat3 )
            INSTR( make_mat4 )
            INSTR( op_nop )
            INSTR( op_negf )
            INSTR( op_negv )
            INSTR( op_negm )
            INSTR( op_notb )
            INSTR( op_addff )
            INSTR( op_addvv )
            INSTR( op_addmm )
            INSTR( op_subff )
            INSTR( op_subvv )
            INSTR( op_submm )
            INSTR( op_mulff )
            INSTR( op_mulfv )
            INSTR( op_mulvf )
            INSTR( op_mulvv )
            INSTR( op_mulfm )
            INSTR( op_mulmf )
            INSTR( op_mulvm )
            INSTR( op_mulmv )
            INSTR( op_mulmm )
            INSTR( op_divff )
            INSTR( op_divvf )
            INSTR( op_divmf )
            INSTR( op_modff )
            INSTR( op_powff )
            INSTR( op_eqbb )
            INSTR( op_eqff )
            INSTR( op_eqvv )
            INSTR( op_eqmm )
            INSTR( op_neqbb )
            INSTR( op_neqff )
            INSTR( op_neqvv )
            INSTR( op_neqmm )
            INSTR( op_ltff )
            INSTR( op_lteff )
            INSTR( op_gtff )
            INSTR( op_gteff )
            INSTR( op_andbb )
            INSTR( op_orbb )
            INSTR( fn_sin )
            INSTR( fn_cos )
            INSTR( fn_tan )
            INSTR( fn_asin )
            INSTR( fn_acos )
            INSTR( fn_atan )
            INSTR( fn_atan2 )
            INSTR( fn_sinh )
            INSTR( fn_cosh )
            INSTR( fn_tanh )
            INSTR( fn_asinh )
            INSTR( fn_acosh )
            INSTR( fn_atanh )
            INSTR( fn_exp )
            INSTR( fn_log )
            INSTR( fn_sqrt )
            INSTR( fn_abs )
            INSTR( fn_sign )
            INSTR( fn_floor )
            INSTR( fn_round )
            INSTR( fn_ceil )
            INSTR( fn_min )
            INSTR( fn_max )
            INSTR( fn_clamp )
            INSTR( fn_lerp )
            INSTR( fn_isnan )
            INSTR( fn_isinf )
            INSTR( fn_length )
            INSTR( fn_distance )
            INSTR( fn_cross )
            INSTR( fn_normalize )
            INSTR( fn_lerpv )
            INSTR( fn_identity )
            INSTR( fn_translate )
            INSTR( fn_scale )
            INSTR( fn_rotate )
            INSTR( fn_transpose )
            INSTR( fn_determinant )
            INSTR( fn_inverse )
            INSTR_INT( load_gvarb )
            INSTR_INT( load_gvarf )
            INSTR_INT( load_gvarv )
            INSTR_INT( load_gvarm )
            INSTR_INT( load_lvarb )
            INSTR_INT( load_lvarf )
            INSTR_INT( load_lvarv )
            INSTR_INT( load_lvarm )
            INSTR_INT2( load_gcompv )
            INSTR_INT2( load_gcompm )
            INSTR_INT2( load_lcompv )
            INSTR_INT2( load_lcompm )
            INSTR_INT( store_gvarb )
            INSTR_INT( store_gvarf )
            INSTR_INT( store_gvarv )
            INSTR_INT( store_gvarm )
            INSTR_INT( store_lvarb )
            INSTR_INT( store_lvarf )
            INSTR_INT( store_lvarv )
            INSTR_INT( store_lvarm )
            INSTR_INT2( store_gcompv )
            INSTR_INT2( store_gcompm )
            INSTR_INT2( store_lcompv )
            INSTR_INT2( store_lcompm )
            INSTR_INT( jmp )
            INSTR_INT( jmp_false )
            INSTR_INT( jmp_true )
            INSTR_INT( jmp_and )
            INSTR_INT( jmp_or )
            INSTR_INT( call )
            INSTR( pop )
            INSTR_INT( pop_n )
            INSTR( ret )
            INSTR_INT( ret_n )
            INSTR( exit )
            default:
                m_stream << "???" << endl;
                return;
        }

        m_stream << endl;
    }
}

#endif // defined( MISC_DEBUG )
