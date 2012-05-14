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

#include "glloader.h"

GLLoader* GLLoader::p = NULL;

GLLoader::GLLoader( const QGLContext* ctx )
{
    m_glBlendColor = (PFNGLBLENDCOLORPROC) ctx->getProcAddress( "glBlendColor" );
    m_glBlendEquation = (PFNGLBLENDEQUATIONPROC) ctx->getProcAddress( "glBlendEquation" );
    m_glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC) ctx->getProcAddress( "glDrawRangeElements" );
    m_glTexImage3D = (PFNGLTEXIMAGE3DPROC) ctx->getProcAddress( "glTexImage3D" );
    m_glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC) ctx->getProcAddress( "glTexSubImage3D" );
    m_glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC) ctx->getProcAddress( "glCopyTexSubImage3D" );
    m_glActiveTexture = (PFNGLACTIVETEXTUREPROC) ctx->getProcAddress( "glActiveTexture" );
    m_glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC) ctx->getProcAddress( "glSampleCoverage" );
    m_glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC) ctx->getProcAddress( "glCompressedTexImage3D" );
    m_glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC) ctx->getProcAddress( "glCompressedTexImage2D" );
    m_glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC) ctx->getProcAddress( "glCompressedTexImage1D" );
    m_glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC) ctx->getProcAddress( "glCompressedTexSubImage3D" );
    m_glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC) ctx->getProcAddress( "glCompressedTexSubImage2D" );
    m_glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC) ctx->getProcAddress( "glCompressedTexSubImage1D" );
    m_glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC) ctx->getProcAddress( "glGetCompressedTexImage" );
    m_glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC) ctx->getProcAddress( "glBlendFuncSeparate" );
    m_glMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC) ctx->getProcAddress( "glMultiDrawArrays" );
    m_glMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC) ctx->getProcAddress( "glMultiDrawElements" );
    m_glPointParameterf = (PFNGLPOINTPARAMETERFPROC) ctx->getProcAddress( "glPointParameterf" );
    m_glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC) ctx->getProcAddress( "glPointParameterfv" );
    m_glPointParameteri = (PFNGLPOINTPARAMETERIPROC) ctx->getProcAddress( "glPointParameteri" );
    m_glPointParameteriv = (PFNGLPOINTPARAMETERIVPROC) ctx->getProcAddress( "glPointParameteriv" );
    m_glGenQueries = (PFNGLGENQUERIESPROC) ctx->getProcAddress( "glGenQueries" );
    m_glDeleteQueries = (PFNGLDELETEQUERIESPROC) ctx->getProcAddress( "glDeleteQueries" );
    m_glIsQuery = (PFNGLISQUERYPROC) ctx->getProcAddress( "glIsQuery" );
    m_glBeginQuery = (PFNGLBEGINQUERYPROC) ctx->getProcAddress( "glBeginQuery" );
    m_glEndQuery = (PFNGLENDQUERYPROC) ctx->getProcAddress( "glEndQuery" );
    m_glGetQueryiv = (PFNGLGETQUERYIVPROC) ctx->getProcAddress( "glGetQueryiv" );
    m_glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC) ctx->getProcAddress( "glGetQueryObjectiv" );
    m_glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC) ctx->getProcAddress( "glGetQueryObjectuiv" );
    m_glBindBuffer = (PFNGLBINDBUFFERPROC) ctx->getProcAddress( "glBindBuffer" );
    m_glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) ctx->getProcAddress( "glDeleteBuffers" );
    m_glGenBuffers = (PFNGLGENBUFFERSPROC) ctx->getProcAddress( "glGenBuffers" );
    m_glIsBuffer = (PFNGLISBUFFERPROC) ctx->getProcAddress( "glIsBuffer" );
    m_glBufferData = (PFNGLBUFFERDATAPROC) ctx->getProcAddress( "glBufferData" );
    m_glBufferSubData = (PFNGLBUFFERSUBDATAPROC) ctx->getProcAddress( "glBufferSubData" );
    m_glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC) ctx->getProcAddress( "glGetBufferSubData" );
    m_glMapBuffer = (PFNGLMAPBUFFERPROC) ctx->getProcAddress( "glMapBuffer" );
    m_glUnmapBuffer = (PFNGLUNMAPBUFFERPROC) ctx->getProcAddress( "glUnmapBuffer" );
    m_glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC) ctx->getProcAddress( "glGetBufferParameteriv" );
    m_glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC) ctx->getProcAddress( "glGetBufferPointerv" );
    m_glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC) ctx->getProcAddress( "glBlendEquationSeparate" );
    m_glDrawBuffers = (PFNGLDRAWBUFFERSPROC) ctx->getProcAddress( "glDrawBuffers" );
    m_glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC) ctx->getProcAddress( "glStencilOpSeparate" );
    m_glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC) ctx->getProcAddress( "glStencilFuncSeparate" );
    m_glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC) ctx->getProcAddress( "glStencilMaskSeparate" );
    m_glAttachShader = (PFNGLATTACHSHADERPROC) ctx->getProcAddress( "glAttachShader" );
    m_glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC) ctx->getProcAddress( "glBindAttribLocation" );
    m_glCompileShader = (PFNGLCOMPILESHADERPROC) ctx->getProcAddress( "glCompileShader" );
    m_glCreateProgram = (PFNGLCREATEPROGRAMPROC) ctx->getProcAddress( "glCreateProgram" );
    m_glCreateShader = (PFNGLCREATESHADERPROC) ctx->getProcAddress( "glCreateShader" );
    m_glDeleteProgram = (PFNGLDELETEPROGRAMPROC) ctx->getProcAddress( "glDeleteProgram" );
    m_glDeleteShader = (PFNGLDELETESHADERPROC) ctx->getProcAddress( "glDeleteShader" );
    m_glDetachShader = (PFNGLDETACHSHADERPROC) ctx->getProcAddress( "glDetachShader" );
    m_glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) ctx->getProcAddress( "glDisableVertexAttribArray" );
    m_glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) ctx->getProcAddress( "glEnableVertexAttribArray" );
    m_glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC) ctx->getProcAddress( "glGetActiveAttrib" );
    m_glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC) ctx->getProcAddress( "glGetActiveUniform" );
    m_glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC) ctx->getProcAddress( "glGetAttachedShaders" );
    m_glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) ctx->getProcAddress( "glGetAttribLocation" );
    m_glGetProgramiv = (PFNGLGETPROGRAMIVPROC) ctx->getProcAddress( "glGetProgramiv" );
    m_glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) ctx->getProcAddress( "glGetProgramInfoLog" );
    m_glGetShaderiv = (PFNGLGETSHADERIVPROC) ctx->getProcAddress( "glGetShaderiv" );
    m_glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) ctx->getProcAddress( "glGetShaderInfoLog" );
    m_glGetShaderSource = (PFNGLGETSHADERSOURCEPROC) ctx->getProcAddress( "glGetShaderSource" );
    m_glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) ctx->getProcAddress( "glGetUniformLocation" );
    m_glGetUniformfv = (PFNGLGETUNIFORMFVPROC) ctx->getProcAddress( "glGetUniformfv" );
    m_glGetUniformiv = (PFNGLGETUNIFORMIVPROC) ctx->getProcAddress( "glGetUniformiv" );
    m_glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC) ctx->getProcAddress( "glGetVertexAttribdv" );
    m_glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC) ctx->getProcAddress( "glGetVertexAttribfv" );
    m_glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC) ctx->getProcAddress( "glGetVertexAttribiv" );
    m_glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC) ctx->getProcAddress( "glGetVertexAttribPointerv" );
    m_glIsProgram = (PFNGLISPROGRAMPROC) ctx->getProcAddress( "glIsProgram" );
    m_glIsShader = (PFNGLISSHADERPROC) ctx->getProcAddress( "glIsShader" );
    m_glLinkProgram = (PFNGLLINKPROGRAMPROC) ctx->getProcAddress( "glLinkProgram" );
    m_glShaderSource = (PFNGLSHADERSOURCEPROC) ctx->getProcAddress( "glShaderSource" );
    m_glUseProgram = (PFNGLUSEPROGRAMPROC) ctx->getProcAddress( "glUseProgram" );
    m_glUniform1f = (PFNGLUNIFORM1FPROC) ctx->getProcAddress( "glUniform1f" );
    m_glUniform2f = (PFNGLUNIFORM2FPROC) ctx->getProcAddress( "glUniform2f" );
    m_glUniform3f = (PFNGLUNIFORM3FPROC) ctx->getProcAddress( "glUniform3f" );
    m_glUniform4f = (PFNGLUNIFORM4FPROC) ctx->getProcAddress( "glUniform4f" );
    m_glUniform1i = (PFNGLUNIFORM1IPROC) ctx->getProcAddress( "glUniform1i" );
    m_glUniform2i = (PFNGLUNIFORM2IPROC) ctx->getProcAddress( "glUniform2i" );
    m_glUniform3i = (PFNGLUNIFORM3IPROC) ctx->getProcAddress( "glUniform3i" );
    m_glUniform4i = (PFNGLUNIFORM4IPROC) ctx->getProcAddress( "glUniform4i" );
    m_glUniform1fv = (PFNGLUNIFORM1FVPROC) ctx->getProcAddress( "glUniform1fv" );
    m_glUniform2fv = (PFNGLUNIFORM2FVPROC) ctx->getProcAddress( "glUniform2fv" );
    m_glUniform3fv = (PFNGLUNIFORM3FVPROC) ctx->getProcAddress( "glUniform3fv" );
    m_glUniform4fv = (PFNGLUNIFORM4FVPROC) ctx->getProcAddress( "glUniform4fv" );
    m_glUniform1iv = (PFNGLUNIFORM1IVPROC) ctx->getProcAddress( "glUniform1iv" );
    m_glUniform2iv = (PFNGLUNIFORM2IVPROC) ctx->getProcAddress( "glUniform2iv" );
    m_glUniform3iv = (PFNGLUNIFORM3IVPROC) ctx->getProcAddress( "glUniform3iv" );
    m_glUniform4iv = (PFNGLUNIFORM4IVPROC) ctx->getProcAddress( "glUniform4iv" );
    m_glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC) ctx->getProcAddress( "glUniformMatrix2fv" );
    m_glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC) ctx->getProcAddress( "glUniformMatrix3fv" );
    m_glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) ctx->getProcAddress( "glUniformMatrix4fv" );
    m_glValidateProgram = (PFNGLVALIDATEPROGRAMPROC) ctx->getProcAddress( "glValidateProgram" );
    m_glVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC) ctx->getProcAddress( "glVertexAttrib1d" );
    m_glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC) ctx->getProcAddress( "glVertexAttrib1dv" );
    m_glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC) ctx->getProcAddress( "glVertexAttrib1f" );
    m_glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC) ctx->getProcAddress( "glVertexAttrib1fv" );
    m_glVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC) ctx->getProcAddress( "glVertexAttrib1s" );
    m_glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC) ctx->getProcAddress( "glVertexAttrib1sv" );
    m_glVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC) ctx->getProcAddress( "glVertexAttrib2d" );
    m_glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC) ctx->getProcAddress( "glVertexAttrib2dv" );
    m_glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC) ctx->getProcAddress( "glVertexAttrib2f" );
    m_glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC) ctx->getProcAddress( "glVertexAttrib2fv" );
    m_glVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC) ctx->getProcAddress( "glVertexAttrib2s" );
    m_glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC) ctx->getProcAddress( "glVertexAttrib2sv" );
    m_glVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC) ctx->getProcAddress( "glVertexAttrib3d" );
    m_glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC) ctx->getProcAddress( "glVertexAttrib3dv" );
    m_glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC) ctx->getProcAddress( "glVertexAttrib3f" );
    m_glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC) ctx->getProcAddress( "glVertexAttrib3fv" );
    m_glVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC) ctx->getProcAddress( "glVertexAttrib3s" );
    m_glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC) ctx->getProcAddress( "glVertexAttrib3sv" );
    m_glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC) ctx->getProcAddress( "glVertexAttrib4Nbv" );
    m_glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC) ctx->getProcAddress( "glVertexAttrib4Niv" );
    m_glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC) ctx->getProcAddress( "glVertexAttrib4Nsv" );
    m_glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC) ctx->getProcAddress( "glVertexAttrib4Nub" );
    m_glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC) ctx->getProcAddress( "glVertexAttrib4Nubv" );
    m_glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC) ctx->getProcAddress( "glVertexAttrib4Nuiv" );
    m_glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC) ctx->getProcAddress( "glVertexAttrib4Nusv" );
    m_glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC) ctx->getProcAddress( "glVertexAttrib4bv" );
    m_glVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC) ctx->getProcAddress( "glVertexAttrib4d" );
    m_glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC) ctx->getProcAddress( "glVertexAttrib4dv" );
    m_glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC) ctx->getProcAddress( "glVertexAttrib4f" );
    m_glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC) ctx->getProcAddress( "glVertexAttrib4fv" );
    m_glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC) ctx->getProcAddress( "glVertexAttrib4iv" );
    m_glVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC) ctx->getProcAddress( "glVertexAttrib4s" );
    m_glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC) ctx->getProcAddress( "glVertexAttrib4sv" );
    m_glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC) ctx->getProcAddress( "glVertexAttrib4ubv" );
    m_glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC) ctx->getProcAddress( "glVertexAttrib4uiv" );
    m_glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC) ctx->getProcAddress( "glVertexAttrib4usv" );
    m_glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) ctx->getProcAddress( "glVertexAttribPointer" );
    m_glUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC) ctx->getProcAddress( "glUniformMatrix2x3fv" );
    m_glUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC) ctx->getProcAddress( "glUniformMatrix3x2fv" );
    m_glUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC) ctx->getProcAddress( "glUniformMatrix2x4fv" );
    m_glUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC) ctx->getProcAddress( "glUniformMatrix4x2fv" );
    m_glUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC) ctx->getProcAddress( "glUniformMatrix3x4fv" );
    m_glUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC) ctx->getProcAddress( "glUniformMatrix4x3fv" );
    m_glColorMaski = (PFNGLCOLORMASKIPROC) ctx->getProcAddress( "glColorMaski" );
    m_glGetBooleani_v = (PFNGLGETBOOLEANI_VPROC) ctx->getProcAddress( "glGetBooleani_v" );
    m_glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC) ctx->getProcAddress( "glGetIntegeri_v" );
    m_glEnablei = (PFNGLENABLEIPROC) ctx->getProcAddress( "glEnablei" );
    m_glDisablei = (PFNGLDISABLEIPROC) ctx->getProcAddress( "glDisablei" );
    m_glIsEnabledi = (PFNGLISENABLEDIPROC) ctx->getProcAddress( "glIsEnabledi" );
    m_glBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC) ctx->getProcAddress( "glBeginTransformFeedback" );
    m_glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC) ctx->getProcAddress( "glEndTransformFeedback" );
    m_glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC) ctx->getProcAddress( "glBindBufferRange" );
    m_glBindBufferBase = (PFNGLBINDBUFFERBASEPROC) ctx->getProcAddress( "glBindBufferBase" );
    m_glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC) ctx->getProcAddress( "glTransformFeedbackVaryings" );
    m_glGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC) ctx->getProcAddress( "glGetTransformFeedbackVarying" );
    m_glClampColor = (PFNGLCLAMPCOLORPROC) ctx->getProcAddress( "glClampColor" );
    m_glBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC) ctx->getProcAddress( "glBeginConditionalRender" );
    m_glEndConditionalRender = (PFNGLENDCONDITIONALRENDERPROC) ctx->getProcAddress( "glEndConditionalRender" );
    m_glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC) ctx->getProcAddress( "glVertexAttribIPointer" );
    m_glGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC) ctx->getProcAddress( "glGetVertexAttribIiv" );
    m_glGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC) ctx->getProcAddress( "glGetVertexAttribIuiv" );
    m_glVertexAttribI1i = (PFNGLVERTEXATTRIBI1IPROC) ctx->getProcAddress( "glVertexAttribI1i" );
    m_glVertexAttribI2i = (PFNGLVERTEXATTRIBI2IPROC) ctx->getProcAddress( "glVertexAttribI2i" );
    m_glVertexAttribI3i = (PFNGLVERTEXATTRIBI3IPROC) ctx->getProcAddress( "glVertexAttribI3i" );
    m_glVertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC) ctx->getProcAddress( "glVertexAttribI4i" );
    m_glVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UIPROC) ctx->getProcAddress( "glVertexAttribI1ui" );
    m_glVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UIPROC) ctx->getProcAddress( "glVertexAttribI2ui" );
    m_glVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UIPROC) ctx->getProcAddress( "glVertexAttribI3ui" );
    m_glVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC) ctx->getProcAddress( "glVertexAttribI4ui" );
    m_glVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IVPROC) ctx->getProcAddress( "glVertexAttribI1iv" );
    m_glVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IVPROC) ctx->getProcAddress( "glVertexAttribI2iv" );
    m_glVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IVPROC) ctx->getProcAddress( "glVertexAttribI3iv" );
    m_glVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC) ctx->getProcAddress( "glVertexAttribI4iv" );
    m_glVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIVPROC) ctx->getProcAddress( "glVertexAttribI1uiv" );
    m_glVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIVPROC) ctx->getProcAddress( "glVertexAttribI2uiv" );
    m_glVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIVPROC) ctx->getProcAddress( "glVertexAttribI3uiv" );
    m_glVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC) ctx->getProcAddress( "glVertexAttribI4uiv" );
    m_glVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BVPROC) ctx->getProcAddress( "glVertexAttribI4bv" );
    m_glVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SVPROC) ctx->getProcAddress( "glVertexAttribI4sv" );
    m_glVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBVPROC) ctx->getProcAddress( "glVertexAttribI4ubv" );
    m_glVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USVPROC) ctx->getProcAddress( "glVertexAttribI4usv" );
    m_glGetUniformuiv = (PFNGLGETUNIFORMUIVPROC) ctx->getProcAddress( "glGetUniformuiv" );
    m_glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC) ctx->getProcAddress( "glBindFragDataLocation" );
    m_glGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC) ctx->getProcAddress( "glGetFragDataLocation" );
    m_glUniform1ui = (PFNGLUNIFORM1UIPROC) ctx->getProcAddress( "glUniform1ui" );
    m_glUniform2ui = (PFNGLUNIFORM2UIPROC) ctx->getProcAddress( "glUniform2ui" );
    m_glUniform3ui = (PFNGLUNIFORM3UIPROC) ctx->getProcAddress( "glUniform3ui" );
    m_glUniform4ui = (PFNGLUNIFORM4UIPROC) ctx->getProcAddress( "glUniform4ui" );
    m_glUniform1uiv = (PFNGLUNIFORM1UIVPROC) ctx->getProcAddress( "glUniform1uiv" );
    m_glUniform2uiv = (PFNGLUNIFORM2UIVPROC) ctx->getProcAddress( "glUniform2uiv" );
    m_glUniform3uiv = (PFNGLUNIFORM3UIVPROC) ctx->getProcAddress( "glUniform3uiv" );
    m_glUniform4uiv = (PFNGLUNIFORM4UIVPROC) ctx->getProcAddress( "glUniform4uiv" );
    m_glTexParameterIiv = (PFNGLTEXPARAMETERIIVPROC) ctx->getProcAddress( "glTexParameterIiv" );
    m_glTexParameterIuiv = (PFNGLTEXPARAMETERIUIVPROC) ctx->getProcAddress( "glTexParameterIuiv" );
    m_glGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIVPROC) ctx->getProcAddress( "glGetTexParameterIiv" );
    m_glGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIVPROC) ctx->getProcAddress( "glGetTexParameterIuiv" );
    m_glClearBufferiv = (PFNGLCLEARBUFFERIVPROC) ctx->getProcAddress( "glClearBufferiv" );
    m_glClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC) ctx->getProcAddress( "glClearBufferuiv" );
    m_glClearBufferfv = (PFNGLCLEARBUFFERFVPROC) ctx->getProcAddress( "glClearBufferfv" );
    m_glClearBufferfi = (PFNGLCLEARBUFFERFIPROC) ctx->getProcAddress( "glClearBufferfi" );
    m_glGetStringi = (PFNGLGETSTRINGIPROC) ctx->getProcAddress( "glGetStringi" );
    m_glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC) ctx->getProcAddress( "glDrawArraysInstanced" );
    m_glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC) ctx->getProcAddress( "glDrawElementsInstanced" );
    m_glTexBuffer = (PFNGLTEXBUFFERPROC) ctx->getProcAddress( "glTexBuffer" );
    m_glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC) ctx->getProcAddress( "glPrimitiveRestartIndex" );
    m_glGetInteger64i_v = (PFNGLGETINTEGER64I_VPROC) ctx->getProcAddress( "glGetInteger64i_v" );
    m_glGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC) ctx->getProcAddress( "glGetBufferParameteri64v" );
    m_glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC) ctx->getProcAddress( "glFramebufferTexture" );
    m_glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) ctx->getProcAddress( "glIsRenderbuffer" );
    m_glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) ctx->getProcAddress( "glBindRenderbuffer" );
    m_glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) ctx->getProcAddress( "glDeleteRenderbuffers" );
    m_glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) ctx->getProcAddress( "glGenRenderbuffers" );
    m_glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) ctx->getProcAddress( "glRenderbufferStorage" );
    m_glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) ctx->getProcAddress( "glGetRenderbufferParameteriv" );
    m_glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) ctx->getProcAddress( "glIsFramebuffer" );
    m_glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) ctx->getProcAddress( "glBindFramebuffer" );
    m_glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) ctx->getProcAddress( "glDeleteFramebuffers" );
    m_glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) ctx->getProcAddress( "glGenFramebuffers" );
    m_glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) ctx->getProcAddress( "glCheckFramebufferStatus" );
    m_glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) ctx->getProcAddress( "glFramebufferTexture1D" );
    m_glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) ctx->getProcAddress( "glFramebufferTexture2D" );
    m_glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) ctx->getProcAddress( "glFramebufferTexture3D" );
    m_glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) ctx->getProcAddress( "glFramebufferRenderbuffer" );
    m_glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) ctx->getProcAddress( "glGetFramebufferAttachmentParameteriv" );
    m_glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) ctx->getProcAddress( "glGenerateMipmap" );
    m_glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC) ctx->getProcAddress( "glBlitFramebuffer" );
    m_glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) ctx->getProcAddress( "glRenderbufferStorageMultisample" );
    m_glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC) ctx->getProcAddress( "glFramebufferTextureLayer" );
    m_glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC) ctx->getProcAddress( "glMapBufferRange" );
    m_glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC) ctx->getProcAddress( "glFlushMappedBufferRange" );
    m_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) ctx->getProcAddress( "glBindVertexArray" );
    m_glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) ctx->getProcAddress( "glDeleteVertexArrays" );
    m_glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) ctx->getProcAddress( "glGenVertexArrays" );
    m_glIsVertexArray = (PFNGLISVERTEXARRAYPROC) ctx->getProcAddress( "glIsVertexArray" );
    m_glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC) ctx->getProcAddress( "glGetUniformIndices" );
    m_glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC) ctx->getProcAddress( "glGetActiveUniformsiv" );
    m_glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC) ctx->getProcAddress( "glGetActiveUniformName" );
    m_glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC) ctx->getProcAddress( "glGetUniformBlockIndex" );
    m_glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC) ctx->getProcAddress( "glGetActiveUniformBlockiv" );
    m_glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC) ctx->getProcAddress( "glGetActiveUniformBlockName" );
    m_glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC) ctx->getProcAddress( "glUniformBlockBinding" );
    m_glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC) ctx->getProcAddress( "glCopyBufferSubData" );
    m_glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC) ctx->getProcAddress( "glDrawElementsBaseVertex" );
    m_glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC) ctx->getProcAddress( "glDrawRangeElementsBaseVertex" );
    m_glDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC) ctx->getProcAddress( "glDrawElementsInstancedBaseVertex" );
    m_glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC) ctx->getProcAddress( "glMultiDrawElementsBaseVertex" );
    m_glProvokingVertex = (PFNGLPROVOKINGVERTEXPROC) ctx->getProcAddress( "glProvokingVertex" );
    m_glFenceSync = (PFNGLFENCESYNCPROC) ctx->getProcAddress( "glFenceSync" );
    m_glIsSync = (PFNGLISSYNCPROC) ctx->getProcAddress( "glIsSync" );
    m_glDeleteSync = (PFNGLDELETESYNCPROC) ctx->getProcAddress( "glDeleteSync" );
    m_glClientWaitSync = (PFNGLCLIENTWAITSYNCPROC) ctx->getProcAddress( "glClientWaitSync" );
    m_glWaitSync = (PFNGLWAITSYNCPROC) ctx->getProcAddress( "glWaitSync" );
    m_glGetInteger64v = (PFNGLGETINTEGER64VPROC) ctx->getProcAddress( "glGetInteger64v" );
    m_glGetSynciv = (PFNGLGETSYNCIVPROC) ctx->getProcAddress( "glGetSynciv" );
    m_glTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC) ctx->getProcAddress( "glTexImage2DMultisample" );
    m_glTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC) ctx->getProcAddress( "glTexImage3DMultisample" );
    m_glGetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC) ctx->getProcAddress( "glGetMultisamplefv" );
    m_glSampleMaski = (PFNGLSAMPLEMASKIPROC) ctx->getProcAddress( "glSampleMaski" );
    m_glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC) ctx->getProcAddress( "glVertexAttribDivisor" );
    m_glBindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXEDPROC) ctx->getProcAddress( "glBindFragDataLocationIndexed" );
    m_glGetFragDataIndex = (PFNGLGETFRAGDATAINDEXPROC) ctx->getProcAddress( "glGetFragDataIndex" );
    m_glGenSamplers = (PFNGLGENSAMPLERSPROC) ctx->getProcAddress( "glGenSamplers" );
    m_glDeleteSamplers = (PFNGLDELETESAMPLERSPROC) ctx->getProcAddress( "glDeleteSamplers" );
    m_glIsSampler = (PFNGLISSAMPLERPROC) ctx->getProcAddress( "glIsSampler" );
    m_glBindSampler = (PFNGLBINDSAMPLERPROC) ctx->getProcAddress( "glBindSampler" );
    m_glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC) ctx->getProcAddress( "glSamplerParameteri" );
    m_glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC) ctx->getProcAddress( "glSamplerParameteriv" );
    m_glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC) ctx->getProcAddress( "glSamplerParameterf" );
    m_glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC) ctx->getProcAddress( "glSamplerParameterfv" );
    m_glSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC) ctx->getProcAddress( "glSamplerParameterIiv" );
    m_glSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC) ctx->getProcAddress( "glSamplerParameterIuiv" );
    m_glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC) ctx->getProcAddress( "glGetSamplerParameteriv" );
    m_glGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC) ctx->getProcAddress( "glGetSamplerParameterIiv" );
    m_glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC) ctx->getProcAddress( "glGetSamplerParameterfv" );
    m_glGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC) ctx->getProcAddress( "glGetSamplerParameterIuiv" );
    m_glQueryCounter = (PFNGLQUERYCOUNTERPROC) ctx->getProcAddress( "glQueryCounter" );
    m_glGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64VPROC) ctx->getProcAddress( "glGetQueryObjecti64v" );
    m_glGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64VPROC) ctx->getProcAddress( "glGetQueryObjectui64v" );
}

GLLoader::~GLLoader()
{
}

void GLLoader::initialize( const QGLContext* ctx )
{
    if ( GLLoader::p == NULL )
        GLLoader::p = new GLLoader( ctx );
}
