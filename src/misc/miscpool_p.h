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

#ifndef MISCPOOL_P_H
#define MISCPOOL_P_H

#include "misc.h"

#include <stdlib.h>

class MiscPool
{
public:
    MiscPool() :
        m_chunkSize( InitialChunkSize ),
        m_current( NULL ),
        m_left( 0 ),
        m_head( NULL )
    {
    }

    ~MiscPool()
    {
        freeChunks();
    }

public:
    void* alloc( size_t size )
    {
        size = ( size + sizeof( Chunk* ) - 1 ) & ~( sizeof( Chunk* ) - 1 );

        if ( size > m_left )
            appendChunk( size );

        void* ptr = m_current;

        m_current += size;
        m_left -= size;

        return ptr;
    }

    void clear()
    {
        freeChunks();

        m_chunkSize = InitialChunkSize;
        m_current = NULL;
        m_left = 0;
        m_head = NULL;
    }

private:
    void appendChunk( size_t minSize )
    {
        size_t size;
        if ( m_chunkSize >= minSize ) {
            size = m_chunkSize;
            m_chunkSize <<= 1;
        } else {
            size = minSize;
        }

        Chunk* chunk = static_cast<Chunk*>( malloc( offsetof( Chunk, m_data ) + size ) );

        chunk->m_next = m_head;
        m_head = chunk;

        m_current = &chunk->m_data[ 0 ];
        m_left = size;
    }

    void freeChunks()
    {
        Chunk* chunk = m_head;
        while ( chunk != NULL ) {
            Chunk* next = chunk->m_next;
            free( chunk );
            chunk = next;
        }
    }

private:
    static const int InitialChunkSize = 4096;

    struct Chunk
    {
        Chunk* m_next;
        char m_data[ 1 ];
    };

    size_t m_chunkSize;

    char* m_current;
    size_t m_left;

    Chunk* m_head;
};

inline void* operator new( size_t size, MiscPool* pool )
{
    return pool->alloc( size );
}

inline void operator delete( void* /*ptr*/, MiscPool* /*pool*/ )
{
}

inline void* operator new( size_t /*size*/, MiscPool* pool, size_t actualSize )
{
    return pool->alloc( actualSize );
}

inline void operator delete( void* /*ptr*/, MiscPool* /*pool*/, size_t /*actualSize*/ )
{
}

#endif
