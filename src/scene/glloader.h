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

#ifndef GLLOADER_H
#define GLLOADER_H

#include "glcore.h"

class GLLoader
{
public:
    explicit GLLoader( const QGLContext* context );
    ~GLLoader();

public:
    PFNGLBLENDCOLORPROC m_glBlendColor;
    PFNGLBLENDEQUATIONPROC m_glBlendEquation;
    PFNGLDRAWRANGEELEMENTSPROC m_glDrawRangeElements;
    PFNGLTEXIMAGE3DPROC m_glTexImage3D;
    PFNGLTEXSUBIMAGE3DPROC m_glTexSubImage3D;
    PFNGLCOPYTEXSUBIMAGE3DPROC m_glCopyTexSubImage3D;
    PFNGLACTIVETEXTUREPROC m_glActiveTexture;
    PFNGLSAMPLECOVERAGEPROC m_glSampleCoverage;
    PFNGLCOMPRESSEDTEXIMAGE3DPROC m_glCompressedTexImage3D;
    PFNGLCOMPRESSEDTEXIMAGE2DPROC m_glCompressedTexImage2D;
    PFNGLCOMPRESSEDTEXIMAGE1DPROC m_glCompressedTexImage1D;
    PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC m_glCompressedTexSubImage3D;
    PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC m_glCompressedTexSubImage2D;
    PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC m_glCompressedTexSubImage1D;
    PFNGLGETCOMPRESSEDTEXIMAGEPROC m_glGetCompressedTexImage;
    PFNGLBLENDFUNCSEPARATEPROC m_glBlendFuncSeparate;
    PFNGLMULTIDRAWARRAYSPROC m_glMultiDrawArrays;
    PFNGLMULTIDRAWELEMENTSPROC m_glMultiDrawElements;
    PFNGLPOINTPARAMETERFPROC m_glPointParameterf;
    PFNGLPOINTPARAMETERFVPROC m_glPointParameterfv;
    PFNGLPOINTPARAMETERIPROC m_glPointParameteri;
    PFNGLPOINTPARAMETERIVPROC m_glPointParameteriv;
    PFNGLGENQUERIESPROC m_glGenQueries;
    PFNGLDELETEQUERIESPROC m_glDeleteQueries;
    PFNGLISQUERYPROC m_glIsQuery;
    PFNGLBEGINQUERYPROC m_glBeginQuery;
    PFNGLENDQUERYPROC m_glEndQuery;
    PFNGLGETQUERYIVPROC m_glGetQueryiv;
    PFNGLGETQUERYOBJECTIVPROC m_glGetQueryObjectiv;
    PFNGLGETQUERYOBJECTUIVPROC m_glGetQueryObjectuiv;
    PFNGLBINDBUFFERPROC m_glBindBuffer;
    PFNGLDELETEBUFFERSPROC m_glDeleteBuffers;
    PFNGLGENBUFFERSPROC m_glGenBuffers;
    PFNGLISBUFFERPROC m_glIsBuffer;
    PFNGLBUFFERDATAPROC m_glBufferData;
    PFNGLBUFFERSUBDATAPROC m_glBufferSubData;
    PFNGLGETBUFFERSUBDATAPROC m_glGetBufferSubData;
    PFNGLMAPBUFFERPROC m_glMapBuffer;
    PFNGLUNMAPBUFFERPROC m_glUnmapBuffer;
    PFNGLGETBUFFERPARAMETERIVPROC m_glGetBufferParameteriv;
    PFNGLGETBUFFERPOINTERVPROC m_glGetBufferPointerv;
    PFNGLBLENDEQUATIONSEPARATEPROC m_glBlendEquationSeparate;
    PFNGLDRAWBUFFERSPROC m_glDrawBuffers;
    PFNGLSTENCILOPSEPARATEPROC m_glStencilOpSeparate;
    PFNGLSTENCILFUNCSEPARATEPROC m_glStencilFuncSeparate;
    PFNGLSTENCILMASKSEPARATEPROC m_glStencilMaskSeparate;
    PFNGLATTACHSHADERPROC m_glAttachShader;
    PFNGLBINDATTRIBLOCATIONPROC m_glBindAttribLocation;
    PFNGLCOMPILESHADERPROC m_glCompileShader;
    PFNGLCREATEPROGRAMPROC m_glCreateProgram;
    PFNGLCREATESHADERPROC m_glCreateShader;
    PFNGLDELETEPROGRAMPROC m_glDeleteProgram;
    PFNGLDELETESHADERPROC m_glDeleteShader;
    PFNGLDETACHSHADERPROC m_glDetachShader;
    PFNGLDISABLEVERTEXATTRIBARRAYPROC m_glDisableVertexAttribArray;
    PFNGLENABLEVERTEXATTRIBARRAYPROC m_glEnableVertexAttribArray;
    PFNGLGETACTIVEATTRIBPROC m_glGetActiveAttrib;
    PFNGLGETACTIVEUNIFORMPROC m_glGetActiveUniform;
    PFNGLGETATTACHEDSHADERSPROC m_glGetAttachedShaders;
    PFNGLGETATTRIBLOCATIONPROC m_glGetAttribLocation;
    PFNGLGETPROGRAMIVPROC m_glGetProgramiv;
    PFNGLGETPROGRAMINFOLOGPROC m_glGetProgramInfoLog;
    PFNGLGETSHADERIVPROC m_glGetShaderiv;
    PFNGLGETSHADERINFOLOGPROC m_glGetShaderInfoLog;
    PFNGLGETSHADERSOURCEPROC m_glGetShaderSource;
    PFNGLGETUNIFORMLOCATIONPROC m_glGetUniformLocation;
    PFNGLGETUNIFORMFVPROC m_glGetUniformfv;
    PFNGLGETUNIFORMIVPROC m_glGetUniformiv;
    PFNGLGETVERTEXATTRIBDVPROC m_glGetVertexAttribdv;
    PFNGLGETVERTEXATTRIBFVPROC m_glGetVertexAttribfv;
    PFNGLGETVERTEXATTRIBIVPROC m_glGetVertexAttribiv;
    PFNGLGETVERTEXATTRIBPOINTERVPROC m_glGetVertexAttribPointerv;
    PFNGLISPROGRAMPROC m_glIsProgram;
    PFNGLISSHADERPROC m_glIsShader;
    PFNGLLINKPROGRAMPROC m_glLinkProgram;
    PFNGLSHADERSOURCEPROC m_glShaderSource;
    PFNGLUSEPROGRAMPROC m_glUseProgram;
    PFNGLUNIFORM1FPROC m_glUniform1f;
    PFNGLUNIFORM2FPROC m_glUniform2f;
    PFNGLUNIFORM3FPROC m_glUniform3f;
    PFNGLUNIFORM4FPROC m_glUniform4f;
    PFNGLUNIFORM1IPROC m_glUniform1i;
    PFNGLUNIFORM2IPROC m_glUniform2i;
    PFNGLUNIFORM3IPROC m_glUniform3i;
    PFNGLUNIFORM4IPROC m_glUniform4i;
    PFNGLUNIFORM1FVPROC m_glUniform1fv;
    PFNGLUNIFORM2FVPROC m_glUniform2fv;
    PFNGLUNIFORM3FVPROC m_glUniform3fv;
    PFNGLUNIFORM4FVPROC m_glUniform4fv;
    PFNGLUNIFORM1IVPROC m_glUniform1iv;
    PFNGLUNIFORM2IVPROC m_glUniform2iv;
    PFNGLUNIFORM3IVPROC m_glUniform3iv;
    PFNGLUNIFORM4IVPROC m_glUniform4iv;
    PFNGLUNIFORMMATRIX2FVPROC m_glUniformMatrix2fv;
    PFNGLUNIFORMMATRIX3FVPROC m_glUniformMatrix3fv;
    PFNGLUNIFORMMATRIX4FVPROC m_glUniformMatrix4fv;
    PFNGLVALIDATEPROGRAMPROC m_glValidateProgram;
    PFNGLVERTEXATTRIB1DPROC m_glVertexAttrib1d;
    PFNGLVERTEXATTRIB1DVPROC m_glVertexAttrib1dv;
    PFNGLVERTEXATTRIB1FPROC m_glVertexAttrib1f;
    PFNGLVERTEXATTRIB1FVPROC m_glVertexAttrib1fv;
    PFNGLVERTEXATTRIB1SPROC m_glVertexAttrib1s;
    PFNGLVERTEXATTRIB1SVPROC m_glVertexAttrib1sv;
    PFNGLVERTEXATTRIB2DPROC m_glVertexAttrib2d;
    PFNGLVERTEXATTRIB2DVPROC m_glVertexAttrib2dv;
    PFNGLVERTEXATTRIB2FPROC m_glVertexAttrib2f;
    PFNGLVERTEXATTRIB2FVPROC m_glVertexAttrib2fv;
    PFNGLVERTEXATTRIB2SPROC m_glVertexAttrib2s;
    PFNGLVERTEXATTRIB2SVPROC m_glVertexAttrib2sv;
    PFNGLVERTEXATTRIB3DPROC m_glVertexAttrib3d;
    PFNGLVERTEXATTRIB3DVPROC m_glVertexAttrib3dv;
    PFNGLVERTEXATTRIB3FPROC m_glVertexAttrib3f;
    PFNGLVERTEXATTRIB3FVPROC m_glVertexAttrib3fv;
    PFNGLVERTEXATTRIB3SPROC m_glVertexAttrib3s;
    PFNGLVERTEXATTRIB3SVPROC m_glVertexAttrib3sv;
    PFNGLVERTEXATTRIB4NBVPROC m_glVertexAttrib4Nbv;
    PFNGLVERTEXATTRIB4NIVPROC m_glVertexAttrib4Niv;
    PFNGLVERTEXATTRIB4NSVPROC m_glVertexAttrib4Nsv;
    PFNGLVERTEXATTRIB4NUBPROC m_glVertexAttrib4Nub;
    PFNGLVERTEXATTRIB4NUBVPROC m_glVertexAttrib4Nubv;
    PFNGLVERTEXATTRIB4NUIVPROC m_glVertexAttrib4Nuiv;
    PFNGLVERTEXATTRIB4NUSVPROC m_glVertexAttrib4Nusv;
    PFNGLVERTEXATTRIB4BVPROC m_glVertexAttrib4bv;
    PFNGLVERTEXATTRIB4DPROC m_glVertexAttrib4d;
    PFNGLVERTEXATTRIB4DVPROC m_glVertexAttrib4dv;
    PFNGLVERTEXATTRIB4FPROC m_glVertexAttrib4f;
    PFNGLVERTEXATTRIB4FVPROC m_glVertexAttrib4fv;
    PFNGLVERTEXATTRIB4IVPROC m_glVertexAttrib4iv;
    PFNGLVERTEXATTRIB4SPROC m_glVertexAttrib4s;
    PFNGLVERTEXATTRIB4SVPROC m_glVertexAttrib4sv;
    PFNGLVERTEXATTRIB4UBVPROC m_glVertexAttrib4ubv;
    PFNGLVERTEXATTRIB4UIVPROC m_glVertexAttrib4uiv;
    PFNGLVERTEXATTRIB4USVPROC m_glVertexAttrib4usv;
    PFNGLVERTEXATTRIBPOINTERPROC m_glVertexAttribPointer;
    PFNGLUNIFORMMATRIX2X3FVPROC m_glUniformMatrix2x3fv;
    PFNGLUNIFORMMATRIX3X2FVPROC m_glUniformMatrix3x2fv;
    PFNGLUNIFORMMATRIX2X4FVPROC m_glUniformMatrix2x4fv;
    PFNGLUNIFORMMATRIX4X2FVPROC m_glUniformMatrix4x2fv;
    PFNGLUNIFORMMATRIX3X4FVPROC m_glUniformMatrix3x4fv;
    PFNGLUNIFORMMATRIX4X3FVPROC m_glUniformMatrix4x3fv;
    PFNGLCOLORMASKIPROC m_glColorMaski;
    PFNGLGETBOOLEANI_VPROC m_glGetBooleani_v;
    PFNGLGETINTEGERI_VPROC m_glGetIntegeri_v;
    PFNGLENABLEIPROC m_glEnablei;
    PFNGLDISABLEIPROC m_glDisablei;
    PFNGLISENABLEDIPROC m_glIsEnabledi;
    PFNGLBEGINTRANSFORMFEEDBACKPROC m_glBeginTransformFeedback;
    PFNGLENDTRANSFORMFEEDBACKPROC m_glEndTransformFeedback;
    PFNGLBINDBUFFERRANGEPROC m_glBindBufferRange;
    PFNGLBINDBUFFERBASEPROC m_glBindBufferBase;
    PFNGLTRANSFORMFEEDBACKVARYINGSPROC m_glTransformFeedbackVaryings;
    PFNGLGETTRANSFORMFEEDBACKVARYINGPROC m_glGetTransformFeedbackVarying;
    PFNGLCLAMPCOLORPROC m_glClampColor;
    PFNGLBEGINCONDITIONALRENDERPROC m_glBeginConditionalRender;
    PFNGLENDCONDITIONALRENDERPROC m_glEndConditionalRender;
    PFNGLVERTEXATTRIBIPOINTERPROC m_glVertexAttribIPointer;
    PFNGLGETVERTEXATTRIBIIVPROC m_glGetVertexAttribIiv;
    PFNGLGETVERTEXATTRIBIUIVPROC m_glGetVertexAttribIuiv;
    PFNGLVERTEXATTRIBI1IPROC m_glVertexAttribI1i;
    PFNGLVERTEXATTRIBI2IPROC m_glVertexAttribI2i;
    PFNGLVERTEXATTRIBI3IPROC m_glVertexAttribI3i;
    PFNGLVERTEXATTRIBI4IPROC m_glVertexAttribI4i;
    PFNGLVERTEXATTRIBI1UIPROC m_glVertexAttribI1ui;
    PFNGLVERTEXATTRIBI2UIPROC m_glVertexAttribI2ui;
    PFNGLVERTEXATTRIBI3UIPROC m_glVertexAttribI3ui;
    PFNGLVERTEXATTRIBI4UIPROC m_glVertexAttribI4ui;
    PFNGLVERTEXATTRIBI1IVPROC m_glVertexAttribI1iv;
    PFNGLVERTEXATTRIBI2IVPROC m_glVertexAttribI2iv;
    PFNGLVERTEXATTRIBI3IVPROC m_glVertexAttribI3iv;
    PFNGLVERTEXATTRIBI4IVPROC m_glVertexAttribI4iv;
    PFNGLVERTEXATTRIBI1UIVPROC m_glVertexAttribI1uiv;
    PFNGLVERTEXATTRIBI2UIVPROC m_glVertexAttribI2uiv;
    PFNGLVERTEXATTRIBI3UIVPROC m_glVertexAttribI3uiv;
    PFNGLVERTEXATTRIBI4UIVPROC m_glVertexAttribI4uiv;
    PFNGLVERTEXATTRIBI4BVPROC m_glVertexAttribI4bv;
    PFNGLVERTEXATTRIBI4SVPROC m_glVertexAttribI4sv;
    PFNGLVERTEXATTRIBI4UBVPROC m_glVertexAttribI4ubv;
    PFNGLVERTEXATTRIBI4USVPROC m_glVertexAttribI4usv;
    PFNGLGETUNIFORMUIVPROC m_glGetUniformuiv;
    PFNGLBINDFRAGDATALOCATIONPROC m_glBindFragDataLocation;
    PFNGLGETFRAGDATALOCATIONPROC m_glGetFragDataLocation;
    PFNGLUNIFORM1UIPROC m_glUniform1ui;
    PFNGLUNIFORM2UIPROC m_glUniform2ui;
    PFNGLUNIFORM3UIPROC m_glUniform3ui;
    PFNGLUNIFORM4UIPROC m_glUniform4ui;
    PFNGLUNIFORM1UIVPROC m_glUniform1uiv;
    PFNGLUNIFORM2UIVPROC m_glUniform2uiv;
    PFNGLUNIFORM3UIVPROC m_glUniform3uiv;
    PFNGLUNIFORM4UIVPROC m_glUniform4uiv;
    PFNGLTEXPARAMETERIIVPROC m_glTexParameterIiv;
    PFNGLTEXPARAMETERIUIVPROC m_glTexParameterIuiv;
    PFNGLGETTEXPARAMETERIIVPROC m_glGetTexParameterIiv;
    PFNGLGETTEXPARAMETERIUIVPROC m_glGetTexParameterIuiv;
    PFNGLCLEARBUFFERIVPROC m_glClearBufferiv;
    PFNGLCLEARBUFFERUIVPROC m_glClearBufferuiv;
    PFNGLCLEARBUFFERFVPROC m_glClearBufferfv;
    PFNGLCLEARBUFFERFIPROC m_glClearBufferfi;
    PFNGLGETSTRINGIPROC m_glGetStringi;
    PFNGLDRAWARRAYSINSTANCEDPROC m_glDrawArraysInstanced;
    PFNGLDRAWELEMENTSINSTANCEDPROC m_glDrawElementsInstanced;
    PFNGLTEXBUFFERPROC m_glTexBuffer;
    PFNGLPRIMITIVERESTARTINDEXPROC m_glPrimitiveRestartIndex;
    PFNGLGETINTEGER64I_VPROC m_glGetInteger64i_v;
    PFNGLGETBUFFERPARAMETERI64VPROC m_glGetBufferParameteri64v;
    PFNGLFRAMEBUFFERTEXTUREPROC m_glFramebufferTexture;
    PFNGLISRENDERBUFFERPROC m_glIsRenderbuffer;
    PFNGLBINDRENDERBUFFERPROC m_glBindRenderbuffer;
    PFNGLDELETERENDERBUFFERSPROC m_glDeleteRenderbuffers;
    PFNGLGENRENDERBUFFERSPROC m_glGenRenderbuffers;
    PFNGLRENDERBUFFERSTORAGEPROC m_glRenderbufferStorage;
    PFNGLGETRENDERBUFFERPARAMETERIVPROC m_glGetRenderbufferParameteriv;
    PFNGLISFRAMEBUFFERPROC m_glIsFramebuffer;
    PFNGLBINDFRAMEBUFFERPROC m_glBindFramebuffer;
    PFNGLDELETEFRAMEBUFFERSPROC m_glDeleteFramebuffers;
    PFNGLGENFRAMEBUFFERSPROC m_glGenFramebuffers;
    PFNGLCHECKFRAMEBUFFERSTATUSPROC m_glCheckFramebufferStatus;
    PFNGLFRAMEBUFFERTEXTURE1DPROC m_glFramebufferTexture1D;
    PFNGLFRAMEBUFFERTEXTURE2DPROC m_glFramebufferTexture2D;
    PFNGLFRAMEBUFFERTEXTURE3DPROC m_glFramebufferTexture3D;
    PFNGLFRAMEBUFFERRENDERBUFFERPROC m_glFramebufferRenderbuffer;
    PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC m_glGetFramebufferAttachmentParameteriv;
    PFNGLGENERATEMIPMAPPROC m_glGenerateMipmap;
    PFNGLBLITFRAMEBUFFERPROC m_glBlitFramebuffer;
    PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC m_glRenderbufferStorageMultisample;
    PFNGLFRAMEBUFFERTEXTURELAYERPROC m_glFramebufferTextureLayer;
    PFNGLMAPBUFFERRANGEPROC m_glMapBufferRange;
    PFNGLFLUSHMAPPEDBUFFERRANGEPROC m_glFlushMappedBufferRange;
    PFNGLBINDVERTEXARRAYPROC m_glBindVertexArray;
    PFNGLDELETEVERTEXARRAYSPROC m_glDeleteVertexArrays;
    PFNGLGENVERTEXARRAYSPROC m_glGenVertexArrays;
    PFNGLISVERTEXARRAYPROC m_glIsVertexArray;
    PFNGLGETUNIFORMINDICESPROC m_glGetUniformIndices;
    PFNGLGETACTIVEUNIFORMSIVPROC m_glGetActiveUniformsiv;
    PFNGLGETACTIVEUNIFORMNAMEPROC m_glGetActiveUniformName;
    PFNGLGETUNIFORMBLOCKINDEXPROC m_glGetUniformBlockIndex;
    PFNGLGETACTIVEUNIFORMBLOCKIVPROC m_glGetActiveUniformBlockiv;
    PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC m_glGetActiveUniformBlockName;
    PFNGLUNIFORMBLOCKBINDINGPROC m_glUniformBlockBinding;
    PFNGLCOPYBUFFERSUBDATAPROC m_glCopyBufferSubData;
    PFNGLDRAWELEMENTSBASEVERTEXPROC m_glDrawElementsBaseVertex;
    PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC m_glDrawRangeElementsBaseVertex;
    PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC m_glDrawElementsInstancedBaseVertex;
    PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC m_glMultiDrawElementsBaseVertex;
    PFNGLPROVOKINGVERTEXPROC m_glProvokingVertex;
    PFNGLFENCESYNCPROC m_glFenceSync;
    PFNGLISSYNCPROC m_glIsSync;
    PFNGLDELETESYNCPROC m_glDeleteSync;
    PFNGLCLIENTWAITSYNCPROC m_glClientWaitSync;
    PFNGLWAITSYNCPROC m_glWaitSync;
    PFNGLGETINTEGER64VPROC m_glGetInteger64v;
    PFNGLGETSYNCIVPROC m_glGetSynciv;
    PFNGLTEXIMAGE2DMULTISAMPLEPROC m_glTexImage2DMultisample;
    PFNGLTEXIMAGE3DMULTISAMPLEPROC m_glTexImage3DMultisample;
    PFNGLGETMULTISAMPLEFVPROC m_glGetMultisamplefv;
    PFNGLSAMPLEMASKIPROC m_glSampleMaski;
    PFNGLVERTEXATTRIBDIVISORPROC m_glVertexAttribDivisor;
    PFNGLBINDFRAGDATALOCATIONINDEXEDPROC m_glBindFragDataLocationIndexed;
    PFNGLGETFRAGDATAINDEXPROC m_glGetFragDataIndex;
    PFNGLGENSAMPLERSPROC m_glGenSamplers;
    PFNGLDELETESAMPLERSPROC m_glDeleteSamplers;
    PFNGLISSAMPLERPROC m_glIsSampler;
    PFNGLBINDSAMPLERPROC m_glBindSampler;
    PFNGLSAMPLERPARAMETERIPROC m_glSamplerParameteri;
    PFNGLSAMPLERPARAMETERIVPROC m_glSamplerParameteriv;
    PFNGLSAMPLERPARAMETERFPROC m_glSamplerParameterf;
    PFNGLSAMPLERPARAMETERFVPROC m_glSamplerParameterfv;
    PFNGLSAMPLERPARAMETERIIVPROC m_glSamplerParameterIiv;
    PFNGLSAMPLERPARAMETERIUIVPROC m_glSamplerParameterIuiv;
    PFNGLGETSAMPLERPARAMETERIVPROC m_glGetSamplerParameteriv;
    PFNGLGETSAMPLERPARAMETERIIVPROC m_glGetSamplerParameterIiv;
    PFNGLGETSAMPLERPARAMETERFVPROC m_glGetSamplerParameterfv;
    PFNGLGETSAMPLERPARAMETERIUIVPROC m_glGetSamplerParameterIuiv;
    PFNGLQUERYCOUNTERPROC m_glQueryCounter;
    PFNGLGETQUERYOBJECTI64VPROC m_glGetQueryObjecti64v;
    PFNGLGETQUERYOBJECTUI64VPROC m_glGetQueryObjectui64v;

public:
    static void initialize( const QGLContext* context );

    static GLLoader* p;
};

#define glBlendColor GLLoader::p->m_glBlendColor
#define glBlendEquation GLLoader::p->m_glBlendEquation
#define glDrawRangeElements GLLoader::p->m_glDrawRangeElements
#define glTexImage3D GLLoader::p->m_glTexImage3D
#define glTexSubImage3D GLLoader::p->m_glTexSubImage3D
#define glCopyTexSubImage3D GLLoader::p->m_glCopyTexSubImage3D
#define glActiveTexture GLLoader::p->m_glActiveTexture
#define glSampleCoverage GLLoader::p->m_glSampleCoverage
#define glCompressedTexImage3D GLLoader::p->m_glCompressedTexImage3D
#define glCompressedTexImage2D GLLoader::p->m_glCompressedTexImage2D
#define glCompressedTexImage1D GLLoader::p->m_glCompressedTexImage1D
#define glCompressedTexSubImage3D GLLoader::p->m_glCompressedTexSubImage3D
#define glCompressedTexSubImage2D GLLoader::p->m_glCompressedTexSubImage2D
#define glCompressedTexSubImage1D GLLoader::p->m_glCompressedTexSubImage1D
#define glGetCompressedTexImage GLLoader::p->m_glGetCompressedTexImage
#define glBlendFuncSeparate GLLoader::p->m_glBlendFuncSeparate
#define glMultiDrawArrays GLLoader::p->m_glMultiDrawArrays
#define glMultiDrawElements GLLoader::p->m_glMultiDrawElements
#define glPointParameterf GLLoader::p->m_glPointParameterf
#define glPointParameterfv GLLoader::p->m_glPointParameterfv
#define glPointParameteri GLLoader::p->m_glPointParameteri
#define glPointParameteriv GLLoader::p->m_glPointParameteriv
#define glGenQueries GLLoader::p->m_glGenQueries
#define glDeleteQueries GLLoader::p->m_glDeleteQueries
#define glIsQuery GLLoader::p->m_glIsQuery
#define glBeginQuery GLLoader::p->m_glBeginQuery
#define glEndQuery GLLoader::p->m_glEndQuery
#define glGetQueryiv GLLoader::p->m_glGetQueryiv
#define glGetQueryObjectiv GLLoader::p->m_glGetQueryObjectiv
#define glGetQueryObjectuiv GLLoader::p->m_glGetQueryObjectuiv
#define glBindBuffer GLLoader::p->m_glBindBuffer
#define glDeleteBuffers GLLoader::p->m_glDeleteBuffers
#define glGenBuffers GLLoader::p->m_glGenBuffers
#define glIsBuffer GLLoader::p->m_glIsBuffer
#define glBufferData GLLoader::p->m_glBufferData
#define glBufferSubData GLLoader::p->m_glBufferSubData
#define glGetBufferSubData GLLoader::p->m_glGetBufferSubData
#define glMapBuffer GLLoader::p->m_glMapBuffer
#define glUnmapBuffer GLLoader::p->m_glUnmapBuffer
#define glGetBufferParameteriv GLLoader::p->m_glGetBufferParameteriv
#define glGetBufferPointerv GLLoader::p->m_glGetBufferPointerv
#define glBlendEquationSeparate GLLoader::p->m_glBlendEquationSeparate
#define glDrawBuffers GLLoader::p->m_glDrawBuffers
#define glStencilOpSeparate GLLoader::p->m_glStencilOpSeparate
#define glStencilFuncSeparate GLLoader::p->m_glStencilFuncSeparate
#define glStencilMaskSeparate GLLoader::p->m_glStencilMaskSeparate
#define glAttachShader GLLoader::p->m_glAttachShader
#define glBindAttribLocation GLLoader::p->m_glBindAttribLocation
#define glCompileShader GLLoader::p->m_glCompileShader
#define glCreateProgram GLLoader::p->m_glCreateProgram
#define glCreateShader GLLoader::p->m_glCreateShader
#define glDeleteProgram GLLoader::p->m_glDeleteProgram
#define glDeleteShader GLLoader::p->m_glDeleteShader
#define glDetachShader GLLoader::p->m_glDetachShader
#define glDisableVertexAttribArray GLLoader::p->m_glDisableVertexAttribArray
#define glEnableVertexAttribArray GLLoader::p->m_glEnableVertexAttribArray
#define glGetActiveAttrib GLLoader::p->m_glGetActiveAttrib
#define glGetActiveUniform GLLoader::p->m_glGetActiveUniform
#define glGetAttachedShaders GLLoader::p->m_glGetAttachedShaders
#define glGetAttribLocation GLLoader::p->m_glGetAttribLocation
#define glGetProgramiv GLLoader::p->m_glGetProgramiv
#define glGetProgramInfoLog GLLoader::p->m_glGetProgramInfoLog
#define glGetShaderiv GLLoader::p->m_glGetShaderiv
#define glGetShaderInfoLog GLLoader::p->m_glGetShaderInfoLog
#define glGetShaderSource GLLoader::p->m_glGetShaderSource
#define glGetUniformLocation GLLoader::p->m_glGetUniformLocation
#define glGetUniformfv GLLoader::p->m_glGetUniformfv
#define glGetUniformiv GLLoader::p->m_glGetUniformiv
#define glGetVertexAttribdv GLLoader::p->m_glGetVertexAttribdv
#define glGetVertexAttribfv GLLoader::p->m_glGetVertexAttribfv
#define glGetVertexAttribiv GLLoader::p->m_glGetVertexAttribiv
#define glGetVertexAttribPointerv GLLoader::p->m_glGetVertexAttribPointerv
#define glIsProgram GLLoader::p->m_glIsProgram
#define glIsShader GLLoader::p->m_glIsShader
#define glLinkProgram GLLoader::p->m_glLinkProgram
#define glShaderSource GLLoader::p->m_glShaderSource
#define glUseProgram GLLoader::p->m_glUseProgram
#define glUniform1f GLLoader::p->m_glUniform1f
#define glUniform2f GLLoader::p->m_glUniform2f
#define glUniform3f GLLoader::p->m_glUniform3f
#define glUniform4f GLLoader::p->m_glUniform4f
#define glUniform1i GLLoader::p->m_glUniform1i
#define glUniform2i GLLoader::p->m_glUniform2i
#define glUniform3i GLLoader::p->m_glUniform3i
#define glUniform4i GLLoader::p->m_glUniform4i
#define glUniform1fv GLLoader::p->m_glUniform1fv
#define glUniform2fv GLLoader::p->m_glUniform2fv
#define glUniform3fv GLLoader::p->m_glUniform3fv
#define glUniform4fv GLLoader::p->m_glUniform4fv
#define glUniform1iv GLLoader::p->m_glUniform1iv
#define glUniform2iv GLLoader::p->m_glUniform2iv
#define glUniform3iv GLLoader::p->m_glUniform3iv
#define glUniform4iv GLLoader::p->m_glUniform4iv
#define glUniformMatrix2fv GLLoader::p->m_glUniformMatrix2fv
#define glUniformMatrix3fv GLLoader::p->m_glUniformMatrix3fv
#define glUniformMatrix4fv GLLoader::p->m_glUniformMatrix4fv
#define glValidateProgram GLLoader::p->m_glValidateProgram
#define glVertexAttrib1d GLLoader::p->m_glVertexAttrib1d
#define glVertexAttrib1dv GLLoader::p->m_glVertexAttrib1dv
#define glVertexAttrib1f GLLoader::p->m_glVertexAttrib1f
#define glVertexAttrib1fv GLLoader::p->m_glVertexAttrib1fv
#define glVertexAttrib1s GLLoader::p->m_glVertexAttrib1s
#define glVertexAttrib1sv GLLoader::p->m_glVertexAttrib1sv
#define glVertexAttrib2d GLLoader::p->m_glVertexAttrib2d
#define glVertexAttrib2dv GLLoader::p->m_glVertexAttrib2dv
#define glVertexAttrib2f GLLoader::p->m_glVertexAttrib2f
#define glVertexAttrib2fv GLLoader::p->m_glVertexAttrib2fv
#define glVertexAttrib2s GLLoader::p->m_glVertexAttrib2s
#define glVertexAttrib2sv GLLoader::p->m_glVertexAttrib2sv
#define glVertexAttrib3d GLLoader::p->m_glVertexAttrib3d
#define glVertexAttrib3dv GLLoader::p->m_glVertexAttrib3dv
#define glVertexAttrib3f GLLoader::p->m_glVertexAttrib3f
#define glVertexAttrib3fv GLLoader::p->m_glVertexAttrib3fv
#define glVertexAttrib3s GLLoader::p->m_glVertexAttrib3s
#define glVertexAttrib3sv GLLoader::p->m_glVertexAttrib3sv
#define glVertexAttrib4Nbv GLLoader::p->m_glVertexAttrib4Nbv
#define glVertexAttrib4Niv GLLoader::p->m_glVertexAttrib4Niv
#define glVertexAttrib4Nsv GLLoader::p->m_glVertexAttrib4Nsv
#define glVertexAttrib4Nub GLLoader::p->m_glVertexAttrib4Nub
#define glVertexAttrib4Nubv GLLoader::p->m_glVertexAttrib4Nubv
#define glVertexAttrib4Nuiv GLLoader::p->m_glVertexAttrib4Nuiv
#define glVertexAttrib4Nusv GLLoader::p->m_glVertexAttrib4Nusv
#define glVertexAttrib4bv GLLoader::p->m_glVertexAttrib4bv
#define glVertexAttrib4d GLLoader::p->m_glVertexAttrib4d
#define glVertexAttrib4dv GLLoader::p->m_glVertexAttrib4dv
#define glVertexAttrib4f GLLoader::p->m_glVertexAttrib4f
#define glVertexAttrib4fv GLLoader::p->m_glVertexAttrib4fv
#define glVertexAttrib4iv GLLoader::p->m_glVertexAttrib4iv
#define glVertexAttrib4s GLLoader::p->m_glVertexAttrib4s
#define glVertexAttrib4sv GLLoader::p->m_glVertexAttrib4sv
#define glVertexAttrib4ubv GLLoader::p->m_glVertexAttrib4ubv
#define glVertexAttrib4uiv GLLoader::p->m_glVertexAttrib4uiv
#define glVertexAttrib4usv GLLoader::p->m_glVertexAttrib4usv
#define glVertexAttribPointer GLLoader::p->m_glVertexAttribPointer
#define glUniformMatrix2x3fv GLLoader::p->m_glUniformMatrix2x3fv
#define glUniformMatrix3x2fv GLLoader::p->m_glUniformMatrix3x2fv
#define glUniformMatrix2x4fv GLLoader::p->m_glUniformMatrix2x4fv
#define glUniformMatrix4x2fv GLLoader::p->m_glUniformMatrix4x2fv
#define glUniformMatrix3x4fv GLLoader::p->m_glUniformMatrix3x4fv
#define glUniformMatrix4x3fv GLLoader::p->m_glUniformMatrix4x3fv
#define glColorMaski GLLoader::p->m_glColorMaski
#define glGetBooleani_v GLLoader::p->m_glGetBooleani_v
#define glGetIntegeri_v GLLoader::p->m_glGetIntegeri_v
#define glEnablei GLLoader::p->m_glEnablei
#define glDisablei GLLoader::p->m_glDisablei
#define glIsEnabledi GLLoader::p->m_glIsEnabledi
#define glBeginTransformFeedback GLLoader::p->m_glBeginTransformFeedback
#define glEndTransformFeedback GLLoader::p->m_glEndTransformFeedback
#define glBindBufferRange GLLoader::p->m_glBindBufferRange
#define glBindBufferBase GLLoader::p->m_glBindBufferBase
#define glTransformFeedbackVaryings GLLoader::p->m_glTransformFeedbackVaryings
#define glGetTransformFeedbackVarying GLLoader::p->m_glGetTransformFeedbackVarying
#define glClampColor GLLoader::p->m_glClampColor
#define glBeginConditionalRender GLLoader::p->m_glBeginConditionalRender
#define glEndConditionalRender GLLoader::p->m_glEndConditionalRender
#define glVertexAttribIPointer GLLoader::p->m_glVertexAttribIPointer
#define glGetVertexAttribIiv GLLoader::p->m_glGetVertexAttribIiv
#define glGetVertexAttribIuiv GLLoader::p->m_glGetVertexAttribIuiv
#define glVertexAttribI1i GLLoader::p->m_glVertexAttribI1i
#define glVertexAttribI2i GLLoader::p->m_glVertexAttribI2i
#define glVertexAttribI3i GLLoader::p->m_glVertexAttribI3i
#define glVertexAttribI4i GLLoader::p->m_glVertexAttribI4i
#define glVertexAttribI1ui GLLoader::p->m_glVertexAttribI1ui
#define glVertexAttribI2ui GLLoader::p->m_glVertexAttribI2ui
#define glVertexAttribI3ui GLLoader::p->m_glVertexAttribI3ui
#define glVertexAttribI4ui GLLoader::p->m_glVertexAttribI4ui
#define glVertexAttribI1iv GLLoader::p->m_glVertexAttribI1iv
#define glVertexAttribI2iv GLLoader::p->m_glVertexAttribI2iv
#define glVertexAttribI3iv GLLoader::p->m_glVertexAttribI3iv
#define glVertexAttribI4iv GLLoader::p->m_glVertexAttribI4iv
#define glVertexAttribI1uiv GLLoader::p->m_glVertexAttribI1uiv
#define glVertexAttribI2uiv GLLoader::p->m_glVertexAttribI2uiv
#define glVertexAttribI3uiv GLLoader::p->m_glVertexAttribI3uiv
#define glVertexAttribI4uiv GLLoader::p->m_glVertexAttribI4uiv
#define glVertexAttribI4bv GLLoader::p->m_glVertexAttribI4bv
#define glVertexAttribI4sv GLLoader::p->m_glVertexAttribI4sv
#define glVertexAttribI4ubv GLLoader::p->m_glVertexAttribI4ubv
#define glVertexAttribI4usv GLLoader::p->m_glVertexAttribI4usv
#define glGetUniformuiv GLLoader::p->m_glGetUniformuiv
#define glBindFragDataLocation GLLoader::p->m_glBindFragDataLocation
#define glGetFragDataLocation GLLoader::p->m_glGetFragDataLocation
#define glUniform1ui GLLoader::p->m_glUniform1ui
#define glUniform2ui GLLoader::p->m_glUniform2ui
#define glUniform3ui GLLoader::p->m_glUniform3ui
#define glUniform4ui GLLoader::p->m_glUniform4ui
#define glUniform1uiv GLLoader::p->m_glUniform1uiv
#define glUniform2uiv GLLoader::p->m_glUniform2uiv
#define glUniform3uiv GLLoader::p->m_glUniform3uiv
#define glUniform4uiv GLLoader::p->m_glUniform4uiv
#define glTexParameterIiv GLLoader::p->m_glTexParameterIiv
#define glTexParameterIuiv GLLoader::p->m_glTexParameterIuiv
#define glGetTexParameterIiv GLLoader::p->m_glGetTexParameterIiv
#define glGetTexParameterIuiv GLLoader::p->m_glGetTexParameterIuiv
#define glClearBufferiv GLLoader::p->m_glClearBufferiv
#define glClearBufferuiv GLLoader::p->m_glClearBufferuiv
#define glClearBufferfv GLLoader::p->m_glClearBufferfv
#define glClearBufferfi GLLoader::p->m_glClearBufferfi
#define glGetStringi GLLoader::p->m_glGetStringi
#define glDrawArraysInstanced GLLoader::p->m_glDrawArraysInstanced
#define glDrawElementsInstanced GLLoader::p->m_glDrawElementsInstanced
#define glTexBuffer GLLoader::p->m_glTexBuffer
#define glPrimitiveRestartIndex GLLoader::p->m_glPrimitiveRestartIndex
#define glGetInteger64i_v GLLoader::p->m_glGetInteger64i_v
#define glGetBufferParameteri64v GLLoader::p->m_glGetBufferParameteri64v
#define glFramebufferTexture GLLoader::p->m_glFramebufferTexture
#define glIsRenderbuffer GLLoader::p->m_glIsRenderbuffer
#define glBindRenderbuffer GLLoader::p->m_glBindRenderbuffer
#define glDeleteRenderbuffers GLLoader::p->m_glDeleteRenderbuffers
#define glGenRenderbuffers GLLoader::p->m_glGenRenderbuffers
#define glRenderbufferStorage GLLoader::p->m_glRenderbufferStorage
#define glGetRenderbufferParameteriv GLLoader::p->m_glGetRenderbufferParameteriv
#define glIsFramebuffer GLLoader::p->m_glIsFramebuffer
#define glBindFramebuffer GLLoader::p->m_glBindFramebuffer
#define glDeleteFramebuffers GLLoader::p->m_glDeleteFramebuffers
#define glGenFramebuffers GLLoader::p->m_glGenFramebuffers
#define glCheckFramebufferStatus GLLoader::p->m_glCheckFramebufferStatus
#define glFramebufferTexture1D GLLoader::p->m_glFramebufferTexture1D
#define glFramebufferTexture2D GLLoader::p->m_glFramebufferTexture2D
#define glFramebufferTexture3D GLLoader::p->m_glFramebufferTexture3D
#define glFramebufferRenderbuffer GLLoader::p->m_glFramebufferRenderbuffer
#define glGetFramebufferAttachmentParameteriv GLLoader::p->m_glGetFramebufferAttachmentParameteriv
#define glGenerateMipmap GLLoader::p->m_glGenerateMipmap
#define glBlitFramebuffer GLLoader::p->m_glBlitFramebuffer
#define glRenderbufferStorageMultisample GLLoader::p->m_glRenderbufferStorageMultisample
#define glFramebufferTextureLayer GLLoader::p->m_glFramebufferTextureLayer
#define glMapBufferRange GLLoader::p->m_glMapBufferRange
#define glFlushMappedBufferRange GLLoader::p->m_glFlushMappedBufferRange
#define glBindVertexArray GLLoader::p->m_glBindVertexArray
#define glDeleteVertexArrays GLLoader::p->m_glDeleteVertexArrays
#define glGenVertexArrays GLLoader::p->m_glGenVertexArrays
#define glIsVertexArray GLLoader::p->m_glIsVertexArray
#define glGetUniformIndices GLLoader::p->m_glGetUniformIndices
#define glGetActiveUniformsiv GLLoader::p->m_glGetActiveUniformsiv
#define glGetActiveUniformName GLLoader::p->m_glGetActiveUniformName
#define glGetUniformBlockIndex GLLoader::p->m_glGetUniformBlockIndex
#define glGetActiveUniformBlockiv GLLoader::p->m_glGetActiveUniformBlockiv
#define glGetActiveUniformBlockName GLLoader::p->m_glGetActiveUniformBlockName
#define glUniformBlockBinding GLLoader::p->m_glUniformBlockBinding
#define glCopyBufferSubData GLLoader::p->m_glCopyBufferSubData
#define glDrawElementsBaseVertex GLLoader::p->m_glDrawElementsBaseVertex
#define glDrawRangeElementsBaseVertex GLLoader::p->m_glDrawRangeElementsBaseVertex
#define glDrawElementsInstancedBaseVertex GLLoader::p->m_glDrawElementsInstancedBaseVertex
#define glMultiDrawElementsBaseVertex GLLoader::p->m_glMultiDrawElementsBaseVertex
#define glProvokingVertex GLLoader::p->m_glProvokingVertex
#define glFenceSync GLLoader::p->m_glFenceSync
#define glIsSync GLLoader::p->m_glIsSync
#define glDeleteSync GLLoader::p->m_glDeleteSync
#define glClientWaitSync GLLoader::p->m_glClientWaitSync
#define glWaitSync GLLoader::p->m_glWaitSync
#define glGetInteger64v GLLoader::p->m_glGetInteger64v
#define glGetSynciv GLLoader::p->m_glGetSynciv
#define glTexImage2DMultisample GLLoader::p->m_glTexImage2DMultisample
#define glTexImage3DMultisample GLLoader::p->m_glTexImage3DMultisample
#define glGetMultisamplefv GLLoader::p->m_glGetMultisamplefv
#define glSampleMaski GLLoader::p->m_glSampleMaski
#define glVertexAttribDivisor GLLoader::p->m_glVertexAttribDivisor;
#define glBindFragDataLocationIndexed GLLoader::p->m_glBindFragDataLocationIndexed;
#define glGetFragDataIndex GLLoader::p->m_glGetFragDataIndex;
#define glGenSamplers GLLoader::p->m_glGenSamplers;
#define glDeleteSamplers GLLoader::p->m_glDeleteSamplers;
#define glIsSampler GLLoader::p->m_glIsSampler;
#define glBindSampler GLLoader::p->m_glBindSampler;
#define glSamplerParameteri GLLoader::p->m_glSamplerParameteri;
#define glSamplerParameteriv GLLoader::p->m_glSamplerParameteriv;
#define glSamplerParameterf GLLoader::p->m_glSamplerParameterf;
#define glSamplerParameterfv GLLoader::p->m_glSamplerParameterfv;
#define glSamplerParameterIiv GLLoader::p->m_glSamplerParameterIiv;
#define glSamplerParameterIuiv GLLoader::p->m_glSamplerParameterIuiv;
#define glGetSamplerParameteriv GLLoader::p->m_glGetSamplerParameteriv;
#define glGetSamplerParameterIiv GLLoader::p->m_glGetSamplerParameterIiv;
#define glGetSamplerParameterfv GLLoader::p->m_glGetSamplerParameterfv;
#define glGetSamplerParameterIuiv GLLoader::p->m_glGetSamplerParameterIuiv;
#define glQueryCounter GLLoader::p->m_glQueryCounter;
#define glGetQueryObjecti64v GLLoader::p->m_glGetQueryObjecti64v;
#define glGetQueryObjectui64v GLLoader::p->m_glGetQueryObjectui64v;

#endif
