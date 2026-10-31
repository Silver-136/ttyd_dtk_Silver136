/*
 * Copyright (C) 1991-2000 Silicon Graphics, Inc. All Rights Reserved.
 * SGI Free Software License B (Version 2.0, Sept. 18, 2008), http://oss.sgi.com/projects/FreeB/;
 * the full notice is at the top of src/glu/tess.c.
 */

#pragma once

/* The parts of the OpenGL / GLU interface that tst.rel's copy of SGI's GLU polygon tessellator (libtess, Eric Veach,
 * 1994) uses: the GL scalar types, the primitive and boolean constants, and the gluTess* entry points. */

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

#define GLAPIENTRY
#define GLAPI extern

#define GL_FALSE 0
#define GL_TRUE 1

#define GL_LINE_LOOP 0x0002
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006

/* Errors */
#define GLU_INVALID_ENUM 100900
#define GLU_INVALID_VALUE 100901
#define GLU_OUT_OF_MEMORY 100902

/* TessCallback */
#define GLU_TESS_BEGIN 100100
#define GLU_BEGIN 100100
#define GLU_TESS_VERTEX 100101
#define GLU_VERTEX 100101
#define GLU_TESS_END 100102
#define GLU_END 100102
#define GLU_TESS_ERROR 100103
#define GLU_TESS_EDGE_FLAG 100104
#define GLU_EDGE_FLAG 100104
#define GLU_TESS_COMBINE 100105
#define GLU_TESS_BEGIN_DATA 100106
#define GLU_TESS_VERTEX_DATA 100107
#define GLU_TESS_END_DATA 100108
#define GLU_TESS_ERROR_DATA 100109
#define GLU_TESS_EDGE_FLAG_DATA 100110
#define GLU_TESS_COMBINE_DATA 100111

/* TessContour */
#define GLU_CW 100120
#define GLU_CCW 100121
#define GLU_INTERIOR 100122
#define GLU_EXTERIOR 100123
#define GLU_UNKNOWN 100124

/* TessProperty */
#define GLU_TESS_WINDING_RULE 100140
#define GLU_TESS_BOUNDARY_ONLY 100141
#define GLU_TESS_TOLERANCE 100142

/* TessError */
#define GLU_TESS_ERROR1 100151
#define GLU_TESS_ERROR2 100152
#define GLU_TESS_ERROR3 100153
#define GLU_TESS_ERROR4 100154
#define GLU_TESS_ERROR5 100155
#define GLU_TESS_ERROR6 100156
#define GLU_TESS_ERROR7 100157
#define GLU_TESS_ERROR8 100158
#define GLU_TESS_MISSING_BEGIN_POLYGON 100151
#define GLU_TESS_MISSING_BEGIN_CONTOUR 100152
#define GLU_TESS_MISSING_END_POLYGON 100153
#define GLU_TESS_MISSING_END_CONTOUR 100154
#define GLU_TESS_COORD_TOO_LARGE 100155
#define GLU_TESS_NEED_COMBINE_CALLBACK 100156

/* TessWinding */
#define GLU_TESS_WINDING_ODD 100130
#define GLU_TESS_WINDING_NONZERO 100131
#define GLU_TESS_WINDING_POSITIVE 100132
#define GLU_TESS_WINDING_NEGATIVE 100133
#define GLU_TESS_WINDING_ABS_GEQ_TWO 100134

#define GLU_TESS_MAX_COORD 1.0e150

typedef struct GLUtesselator GLUtesselator;
typedef GLUtesselator GLUtesselatorObj;
typedef GLUtesselator GLUtriangulatorObj;

typedef void(GLAPIENTRY* _GLUfuncptr)();

GLAPI void GLAPIENTRY gluBeginPolygon(GLUtesselator* tess);
GLAPI void GLAPIENTRY gluDeleteTess(GLUtesselator* tess);
GLAPI void GLAPIENTRY gluEndPolygon(GLUtesselator* tess);
GLAPI void GLAPIENTRY gluGetTessProperty(GLUtesselator* tess, GLenum which, GLdouble* data);
GLAPI GLUtesselator* GLAPIENTRY gluNewTess(void);
GLAPI void GLAPIENTRY gluNextContour(GLUtesselator* tess, GLenum type);
GLAPI void GLAPIENTRY gluTessBeginContour(GLUtesselator* tess);
GLAPI void GLAPIENTRY gluTessBeginPolygon(GLUtesselator* tess, GLvoid* data);
GLAPI void GLAPIENTRY gluTessCallback(GLUtesselator* tess, GLenum which, _GLUfuncptr CallBackFunc);
GLAPI void GLAPIENTRY gluTessEndContour(GLUtesselator* tess);
GLAPI void GLAPIENTRY gluTessEndPolygon(GLUtesselator* tess);
GLAPI void GLAPIENTRY gluTessNormal(GLUtesselator* tess, GLdouble valueX, GLdouble valueY, GLdouble valueZ);
GLAPI void GLAPIENTRY gluTessProperty(GLUtesselator* tess, GLenum which, GLdouble data);
GLAPI void GLAPIENTRY gluTessVertex(GLUtesselator* tess, GLdouble* location, GLvoid* data);
