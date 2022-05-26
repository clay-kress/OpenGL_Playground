#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/*** CLAG Computing for Linear Algebra and Graphics
 * vec2: 2d vector
 * vec3: 3d vector
 * vec4: 4d vector
 * mat4: 4x4 matrix
 */

typedef union {
      struct {
            float x;
            float y;
      };
      float v[2];
} vec2;

typedef union {
      struct {
            float x;
            float y;
            float z;
      };
      struct {
            float r;
            float g;
            float b;
      };
      float v[3];
} vec3;

typedef union {
      struct {
            float x;
            float y;
            float z;
            float w;
      };
      struct {
            float r;
            float g;
            float b;
            float a;
      };
      float v[4];
} vec4;

typedef struct {
      float m[4][4];
} mat4;

#define PI 3.14159265358979323846264338327950288419716939937510582097494
#define E 2.718281828459045235360287471352662497757247093699959574966967

#define rad(x) x*(PI/180.0)
#define deg(x) x*(180.0/PI)

/* Matrix Functions ***************************************************/
// --> Matrix printing functions
void mat4_fprintf_COL(FILE* stream, const char* name, mat4* m);
void mat4_fprintf_ROW(FILE* stream, const char* name, mat4* m);

// --> Convert row major coordinates to column major coordinates
float mat4_get(mat4* m, int i, int j);
void mat4_set(mat4* m, int i, int j, float x);

// --> Initialize matrix at m to specific values
void mat4_zeros(mat4* m);
void mat4_identity(mat4* m);
void mat4_digits(mat4* m);
void mat4_translation(mat4* m, float x, float y, float z);

// --> Mat4 computing functions
void mat4_mul(mat4* a, mat4* b, mat4* out);
void mat4_X_vec4(mat4* m, vec4* v, vec4* out);
void mat4_translate(mat4* m, vec3* v, mat4* out);
void mat4_rotate(mat4* m, float radians, vec3* v, mat4* out);
void mat4_scale(mat4* m, vec3* v, mat4* out);

// --> Camera matrix functions
void glm_lookAt(vec3* pos, vec3* target, vec3* up, mat4* viewMat); // --> Creates a view matrix
void glm_perspective(float fov, float aspect, float nearZ, float farZ, mat4* projMat); // --> Creates a perspective projection matrix
void glm_orthographic(float l, float r, float b, float t, float nearZ, float farZ, mat4* projMat); // --> Creates an orthographic projection matrix

/* Vector Functions ***************************************************/
// --> Vector printing functions
void vec2_fprintf(FILE* stream, const char* name, vec2* v);
void vec3_fprintf(FILE* stream, const char* name, vec3* v);
void vec4_fprintf(FILE* stream, const char* name, vec4* v);

// --> Initialize vector at v to specific values
void vec2_init(vec2* v, float x, float y);
void vec3_init(vec3* v, float x, float y, float z);
void vec4_init(vec4* v, float x, float y, float z, float w);

vec2 vec2_getVec(float x, float y);
vec3 vec3_getVec(float x, float y, float z);
vec4 vec4_getVec(float x, float y, float z, float w);

// --> Vec2 computing functions
float vec2_length(vec2* v);
void vec2_normalize(vec2* v, vec2* out);
void vec2_negate(vec2* v, vec2* out);
void vec2_scale(vec2* v, float s, vec2* out);
void vec2_add(vec2* a, vec2* b, vec2* out);
void vec2_sub(vec2* a, vec2* b, vec2* out);
float vec2_dot(vec2* a, vec2* b);
float vec2_angle(vec2* a, vec2* b);

// --> Vec3 computing functions
float vec3_length(vec3* v);
void vec3_normalize(vec3* v, vec3* out);
void vec3_negate(vec3* v, vec3* out);
void vec3_scale(vec3* v, float s, vec3* out);
void vec3_add(vec3* a, vec3* b, vec3* out);
void vec3_sub(vec3* a, vec3* b, vec3* out);
float vec3_dot(vec3* a, vec3* b);
float vec3_angle(vec3* a, vec3* b);
void vec3_cross(vec3* a, vec3* b, vec3* out);

// --> Vec4 computing functions
float vec4_length(vec4* v);
void vec4_normalize(vec4* v, vec4* out);
void vec4_negate(vec4* v, vec4* out);
void vec4_scale(vec4* v, float s, vec4* out);
void vec4_add(vec4* a, vec4* b, vec4* out);
void vec4_sub(vec4* a, vec4* b, vec4* out);
float vec4_dot(vec4* a, vec4* b);

// --> Special vector functions for graphics computations
void vec3_translate(vec3* v, mat4* m, vec3* out);