/**************************************************************************
* This file is part of the Descend program
* Copyright (C) 2012 Michał Męciński
*
* This file is based on qzip.cpp from the QtGui module of the Qt Toolkit.
* Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "utils/zipfile.h"

#if ( QT_VERSION >= 0x050000 )
#include <QtZlib/zlib.h>
#else
#include "zlib.h"
#endif

#include <QDateTime>
#include <QFile>

static inline uint readUInt( const uchar* data )
{
    return data[ 0 ] + ( data[ 1 ] << 8 ) + ( data[ 2 ] << 16 ) + ( data[ 3 ] << 24 );
}

static inline ushort readUShort( const uchar* data )
{
    return data[ 0 ] + ( data[ 1 ] << 8 );
}

static inline void writeUInt( uchar* data, uint i )
{
    data[ 0 ] = i & 0xff;
    data[ 1 ] = ( i >> 8 ) & 0xff;
    data[ 2 ] = ( i >> 16 ) & 0xff;
    data[ 3 ] = ( i >> 24 ) & 0xff;
}

static inline void writeUShort( uchar* data, ushort i )
{
    data[ 0 ] = i & 0xff;
    data[ 1 ] = ( i>>8 ) & 0xff;
}

static inline void copyUInt( uchar* dest, const uchar* src )
{
    dest[ 0 ] = src[ 0 ];
    dest[ 1 ] = src[ 1 ];
    dest[ 2 ] = src[ 2 ];
    dest[ 3 ] = src[ 3 ];
}

static inline void copyUShort( uchar* dest, const uchar* src )
{
    dest[ 0 ] = src[ 0 ];
    dest[ 1 ] = src[ 1 ];
}

static void writeMSDosDate( uchar* dest, const QDateTime& dt )
{
    ushort time = ( dt.time().hour() << 11 ) | ( dt.time().minute() << 5 ) | ( dt.time().second() >> 1 );

    dest[ 0 ] = time & 0xff;
    dest[ 1 ] = ( time >> 8 ) & 0xff;

    ushort date = ( ( dt.date().year() - 1980 ) << 9 ) | ( dt.date().month() << 5 ) | dt.date().day();

    dest[ 2 ] = date & 0xff;
    dest[ 3 ] = ( date >> 8 ) & 0xff;
}

static int inflate( Bytef* dest, ulong* destLen, const Bytef* source, ulong sourceLen )
{
    z_stream stream;
    int err;

    stream.next_in = (Bytef*)source;
    stream.avail_in = (uInt)sourceLen;
    if ( (uLong)stream.avail_in != sourceLen )
        return Z_BUF_ERROR;

    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;
    if ( (uLong)stream.avail_out != *destLen )
        return Z_BUF_ERROR;

    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;

    err = inflateInit2( &stream, -MAX_WBITS );
    if ( err != Z_OK )
        return err;

    err = inflate( &stream, Z_FINISH );
    if ( err != Z_STREAM_END ) {
        inflateEnd( &stream );
        if ( err == Z_NEED_DICT || ( err == Z_BUF_ERROR && stream.avail_in == 0 ) )
            return Z_DATA_ERROR;
        return err;
    }
    *destLen = stream.total_out;

    err = inflateEnd( &stream );
    return err;
}

static int deflate( Bytef* dest, ulong* destLen, const Bytef* source, ulong sourceLen )
{
    z_stream stream;
    int err;

    stream.next_in = (Bytef*)source;
    stream.avail_in = (uInt)sourceLen;
    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;

    if ( (uLong)stream.avail_out != *destLen )
        return Z_BUF_ERROR;

    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;
    stream.opaque = (voidpf)0;

    err = deflateInit2( &stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -MAX_WBITS, 8, Z_DEFAULT_STRATEGY );
    if ( err != Z_OK )
        return err;

    err = deflate( &stream, Z_FINISH );
    if ( err != Z_STREAM_END ) {
        deflateEnd( &stream );
        return err == Z_OK ? Z_BUF_ERROR : err;
    }
    *destLen = stream.total_out;

    err = deflateEnd( &stream );
    return err;
}

static const uint LFH_SIGNATURE = 0x04034b50;

struct LocalFileHeader
{
    uchar signature[ 4 ];
    uchar version_needed[ 2 ];
    uchar general_purpose_bits[ 2 ];
    uchar compression_method[ 2 ];
    uchar last_mod_file[ 4 ];
    uchar crc_32[ 4 ];
    uchar compressed_size[ 4 ];
    uchar uncompressed_size[ 4 ];
    uchar file_name_length[ 2 ];
    uchar extra_field_length[ 2 ];
};

static const uint CFH_SIGNATURE = 0x02014b50;

struct CentralFileHeader
{
    uchar signature[ 4 ];
    uchar version_made[ 2 ];
    uchar version_needed[ 2 ];
    uchar general_purpose_bits[ 2 ];
    uchar compression_method[ 2 ];
    uchar last_mod_file[ 4 ];
    uchar crc_32[ 4 ];
    uchar compressed_size[ 4 ];
    uchar uncompressed_size[ 4 ];
    uchar file_name_length[ 2 ];
    uchar extra_field_length[ 2 ];
    uchar file_comment_length[ 2 ];
    uchar disk_start[ 2 ];
    uchar internal_file_attributes[ 2 ];
    uchar external_file_attributes[ 4 ];
    uchar offset_local_header[ 4 ];
};

static const uint EOD_SIGNATURE = 0x06054b50;

struct EndOfDirectory
{
    uchar signature[ 4 ];
    uchar this_disk[ 2 ];
    uchar start_of_directory_disk[ 2 ];
    uchar num_dir_entries_this_disk[ 2 ];
    uchar num_dir_entries[ 2 ];
    uchar directory_size[ 4 ];
    uchar dir_start_offset[ 4 ];
    uchar comment_length[ 2 ];
};

class ZipFilePrivate
{
public:
    ZipFilePrivate( const QString& path ) :
        m_file( path )
    {
    }

    ~ZipFilePrivate()
    {
    }

public:
    struct FileHeader
    {
        CentralFileHeader h;
        QByteArray fileName;
    };

public:
    QFile m_file;
    QList<FileHeader> m_headers;
};

ZipFileReader::ZipFileReader( const QString& path ) :
    d( new ZipFilePrivate( path ) )
{
}

ZipFileReader::~ZipFileReader()
{
    delete d;
}

bool ZipFileReader::open()
{
    if ( !d->m_file.open( QIODevice::ReadOnly ) )
        return false;

    uchar signature[4];
    d->m_file.read( (char*)signature, 4 );

    if ( readUInt( signature ) != LFH_SIGNATURE )
        false;

    EndOfDirectory eod;

    for ( int i = 0; ; i++ ) {
        int pos = d->m_file.size() - sizeof( EndOfDirectory ) - i;

        if ( pos < 0 || i > 65535 )
            return false;

        d->m_file.seek( pos );
        d->m_file.read( (char*)&eod, sizeof( EndOfDirectory ) );

        if ( readUInt( eod.signature ) == EOD_SIGNATURE )
            break;
    }

    int start = readUInt( eod.dir_start_offset );
    int count = readUShort( eod.num_dir_entries );

    d->m_file.seek( start );

    for ( int i = 0; i < count; i++ ) {
        ZipFilePrivate::FileHeader header;

        int read = d->m_file.read( (char*)&header.h, sizeof( CentralFileHeader ) );

        if ( read < (int)sizeof( CentralFileHeader ) )
            break;

        if ( readUInt( header.h.signature ) != CFH_SIGNATURE )
            break;

        int length = readUShort( header.h.file_name_length );

        header.fileName = d->m_file.read( length );

        if ( header.fileName.length() != length )
            break;

        int skip = readUShort( header.h.extra_field_length ) + readUShort( header.h.file_comment_length );

        if ( skip > 0 )
            d->m_file.seek( d->m_file.pos() + skip );

        d->m_headers.append( header );
    }

    return true;
}

QByteArray ZipFileReader::fileData( const QString& path ) const
{
    int index = -1;

    QByteArray fileName = path.toLocal8Bit();

    for ( int i = 0; i < d->m_headers.count(); i++ ) {
        if ( d->m_headers.at( i ).fileName == fileName ) {
            index = i;
            break;
        }
    }

    if ( index < 0 )
        return QByteArray();

    const ZipFilePrivate::FileHeader& header = d->m_headers.at( index );

    int compressedSize = readUInt( header.h.compressed_size );
    int uncompressedSize = readUInt( header.h.uncompressed_size );
    int start = readUInt( header.h.offset_local_header );

    d->m_file.seek( start );

    LocalFileHeader localHeader;

    d->m_file.read( (char*)&localHeader, sizeof( LocalFileHeader ) );

    int skip = readUShort( localHeader.file_name_length ) + readUShort( localHeader.extra_field_length );
    d->m_file.seek( d->m_file.pos() + skip );

    int compressionMethod = readUShort( localHeader.compression_method );

    QByteArray compressed = d->m_file.read( compressedSize );

    if ( compressionMethod == 0 ) {
        compressed.truncate( uncompressedSize );
        return compressed;
    }

    if ( compressionMethod == 8 ) {
        compressed.truncate( compressedSize );

        QByteArray data;

        for ( ulong len = qMax( uncompressedSize, 1 ); ; len *= 2 ) {
            data.resize( len );

            int res = inflate( (uchar*)data.data(), &len, (uchar*)compressed.constData(), compressedSize );

            if ( res == Z_OK ) {
                data.resize( len );
                return data;
            }

            if ( res != Z_BUF_ERROR )
                return QByteArray();
        }
    }

    return QByteArray();
}

void ZipFileReader::close()
{
    d->m_file.close();
    d->m_headers.clear();
}

ZipFileWriter::ZipFileWriter( const QString& path ) :
    d( new ZipFilePrivate( path ) )
{
}

ZipFileWriter::~ZipFileWriter()
{
    delete d;
}

bool ZipFileWriter::open()
{
    if ( !d->m_file.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
        return false;

    return true;
}

void ZipFileWriter::addFile( const QString& path, const QByteArray& data, bool compress /*= true*/ )
{
    ZipFilePrivate::FileHeader header;
    memset( &header.h, 0, sizeof( CentralFileHeader ) );

    writeUInt( header.h.signature, CFH_SIGNATURE );
    writeUShort( header.h.version_made, 0x031E );
    writeUShort( header.h.version_needed, 0x0014 );
    writeUInt( header.h.uncompressed_size, data.length() );
    writeMSDosDate( header.h.last_mod_file, QDateTime::currentDateTime() );

    QByteArray compressed;

    if ( compress ) {
        writeUShort( header.h.compression_method, 8 );

        for ( ulong len = data.length() + ( data.length() >> 12 ) + ( data.length() >> 14 ) + 11; ; len *= 2 ) {
            compressed.resize( len );

            int res = deflate( (uchar*)compressed.data(), &len, (uchar*)data.constData(), data.length() );

            if ( res == Z_OK ) {
                compressed.resize( len );
                break;
            }

            if ( res != Z_BUF_ERROR ) {
                compressed.resize( 0 );
                break;
            }
        }
    } else {
        compressed = data;
    }

    writeUInt( header.h.compressed_size, compressed.length() );

    uint checksum = crc32( 0, NULL, 0 );
    checksum = crc32( checksum, (const uchar*)data.constData(), data.length() );
    writeUInt( header.h.crc_32, checksum );

    header.fileName = path.toLocal8Bit();
    writeUShort( header.h.file_name_length, header.fileName.length() );

    writeUInt( header.h.external_file_attributes, 0100644 << 16 );
    writeUInt( header.h.offset_local_header, d->m_file.pos() );

    d->m_headers.append( header );

    LocalFileHeader localHeader;
    memset( &localHeader, 0, sizeof( LocalFileHeader ) );

    writeUInt( localHeader.signature, LFH_SIGNATURE );
    copyUShort( localHeader.version_needed, header.h.version_needed );
    copyUShort( localHeader.compression_method, header.h.compression_method );
    copyUInt( localHeader.last_mod_file, header.h.last_mod_file );
    copyUInt( localHeader.crc_32, header.h.crc_32 );
    copyUInt( localHeader.compressed_size, header.h.compressed_size );
    copyUInt( localHeader.uncompressed_size, header.h.uncompressed_size );
    copyUShort( localHeader.file_name_length, header.h.file_name_length );

    d->m_file.write( (const char*)&localHeader, sizeof( LocalFileHeader ) );
    d->m_file.write( header.fileName );
    d->m_file.write( compressed );
}

void ZipFileWriter::close()
{
    int start = d->m_file.pos();

    for ( int i = 0; i < d->m_headers.count(); i++ ) {
        const ZipFilePrivate::FileHeader& header = d->m_headers.at( i );
        d->m_file.write( (const char*)&header.h, sizeof( CentralFileHeader ) );
        d->m_file.write( header.fileName );
    }

    int directorySize = d->m_file.pos() - start;

    EndOfDirectory eod;
    memset( &eod, 0, sizeof( EndOfDirectory ) );

    writeUInt( eod.signature, EOD_SIGNATURE );
    writeUShort( eod.num_dir_entries_this_disk, d->m_headers.count() );
    writeUShort( eod.num_dir_entries, d->m_headers.count() );
    writeUInt( eod.directory_size, directorySize );
    writeUInt( eod.dir_start_offset, start );

    d->m_file.write( (const char*)&eod, sizeof( EndOfDirectory ) );

    d->m_file.close();
    d->m_headers.clear();
}
