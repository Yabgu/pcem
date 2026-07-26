#ifndef _WX_SDL2_GLW_H_
#define _WX_SDL2_GLW_H_

#include <stdlib.h>

typedef struct glw_t {
        PFNGLACTIVETEXTUREPROC glActiveTexture;
        PFNGLCREATESHADERPROC glCreateShader;
        PFNGLSHADERSOURCEPROC glShaderSource;
        PFNGLCOMPILESHADERPROC glCompileShader;
        PFNGLGETSHADERIVPROC glGetShaderiv;
        PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
        PFNGLCREATEPROGRAMPROC glCreateProgram;
        PFNGLATTACHSHADERPROC glAttachShader;
        PFNGLLINKPROGRAMPROC glLinkProgram;
        PFNGLGETPROGRAMIVPROC glGetProgramiv;
        PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
        PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
        PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
        PFNGLUSEPROGRAMPROC glUseProgram;
        PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
        PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
        PFNGLDELETESHADERPROC glDeleteShader;
        PFNGLDELETEPROGRAMPROC glDeleteProgram;
        PFNGLDELETEBUFFERSPROC glDeleteBuffers;
        PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
        PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
        PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
        PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
        PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
        PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
        PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
        PFNGLGENBUFFERSPROC glGenBuffers;
        PFNGLBINDBUFFERPROC glBindBuffer;
        PFNGLBUFFERDATAPROC glBufferData;
        PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
        PFNGLUNIFORM1IPROC glUniform1i;
        PFNGLUNIFORM1FPROC glUniform1f;
        PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
        PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
        PFNGLUNIFORM2FVPROC glUniform2fv;
        PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
        PFNGLBUFFERSUBDATAPROC glBufferSubData;
} glw_t;

#define GLW_LOAD_PROC(name) glw->name = (decltype(glw->name))SDL_GL_GetProcAddress(#name)

glw_t *glw_init() {
        glw_t *glw = (glw_t *)malloc(sizeof(glw_t));
        GLW_LOAD_PROC(glActiveTexture);
        GLW_LOAD_PROC(glCreateShader);
        GLW_LOAD_PROC(glShaderSource);
        GLW_LOAD_PROC(glCompileShader);
        GLW_LOAD_PROC(glGetShaderiv);
        GLW_LOAD_PROC(glGetShaderInfoLog);
        GLW_LOAD_PROC(glCreateProgram);
        GLW_LOAD_PROC(glAttachShader);
        GLW_LOAD_PROC(glLinkProgram);
        GLW_LOAD_PROC(glGetProgramiv);
        GLW_LOAD_PROC(glGetProgramInfoLog);
        GLW_LOAD_PROC(glGetUniformLocation);
        GLW_LOAD_PROC(glGetAttribLocation);
        GLW_LOAD_PROC(glUseProgram);
        GLW_LOAD_PROC(glGenerateMipmap);
        GLW_LOAD_PROC(glDeleteFramebuffers);
        GLW_LOAD_PROC(glDeleteShader);
        GLW_LOAD_PROC(glDeleteProgram);
        GLW_LOAD_PROC(glDeleteBuffers);
        GLW_LOAD_PROC(glDeleteVertexArrays);
        GLW_LOAD_PROC(glGenFramebuffers);
        GLW_LOAD_PROC(glBindFramebuffer);
        GLW_LOAD_PROC(glFramebufferTexture2D);
        GLW_LOAD_PROC(glCheckFramebufferStatus);
        GLW_LOAD_PROC(glGenVertexArrays);
        GLW_LOAD_PROC(glBindVertexArray);
        GLW_LOAD_PROC(glGenBuffers);
        GLW_LOAD_PROC(glBindBuffer);
        GLW_LOAD_PROC(glBufferData);
        GLW_LOAD_PROC(glVertexAttribPointer);
        GLW_LOAD_PROC(glUniform1i);
        GLW_LOAD_PROC(glUniform1f);
        GLW_LOAD_PROC(glEnableVertexAttribArray);
        GLW_LOAD_PROC(glUniformMatrix4fv);
        GLW_LOAD_PROC(glUniform2fv);
        GLW_LOAD_PROC(glDisableVertexAttribArray);
        GLW_LOAD_PROC(glBufferSubData);
        return glw;
}

#undef GLW_LOAD_PROC

void glw_free(glw_t *glw) { free(glw); }

#endif /* _WX_SDL2_GLW_H_ */
