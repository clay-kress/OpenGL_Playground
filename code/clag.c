/***********************************************************************/
/* Copyright (c) 2022 Clay Kress                                       */
/*                                                                     */
/* This file is part of VioletGE.                                      */
/* VioletGE is free software: you can redistribute it and/or modify it */
/* under the terms of the GNU General Public License as published by   */
/* the Free Software Foundation, either version 3 of the License, or   */
/* (at your option) any later version.                                 */
/*                                                                     */
/* VioletGE is distributed in the hope that it will be useful,         */
/* but WITHOUT ANY WARRANTY; without even the implied warranty         */
/* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.             */
/*                                                                     */
/* See the GNU General Public License for more details.                */
/*                                                                     */
/* You should have received a copy of the GNU General Public License   */
/* along with VioletGE. If not, see <https://www.gnu.org/licenses/>.   */
/*                                                                     */
/***********************************************************************/

#include "clag.h"

/* Matrix Functions ***************************************************/
// --> Matrix printing functions
void mat4_fprintf_COL(FILE* stream, const char* name, mat4* m) {
      fprintf(stream, "%s {\n   ", name);
      for (int i= 0; i < 4; i++) {
            for (int j= 0; j < 4; j++) {
                  fprintf(stream, "%f", m->m[j][i]);
                  if (j != 3 || i != 3) {
                        fprintf(stream, ", ");
                  }
            }
            if (i != 3) {
                  fprintf(stream, "\n   ");
            }
      }
      fprintf(stream, "\n}\n");
      return;
}
void mat4_fprintf_ROW(FILE* stream, const char* name, mat4* m) {
      fprintf(stream, "%s {\n   ", name);
      for (int i= 0; i < 4; i++) {
            for (int j= 0; j < 4; j++) {
                  fprintf(stream, "%f", m->m[i][j]);
                  if (j != 3 || i != 3) {
                        fprintf(stream, ", ");
                  }
            }
            if (i != 3) {
                  fprintf(stream, "\n   ");
            }
      }
      fprintf(stream, "\n}\n");
      return;
}
// --> Convert row major coordinates to column major coordinates
float mat4_get(mat4* m, int i, int j) {
      return m->m[j][i];
}
void mat4_set(mat4* m, int i, int j, float x) {
      m->m[j][i]= x;
      return;
}
// --> Initialize matrix at m to specific values
void mat4_zeros(mat4* m) {
      memset(m, 0x00, sizeof(mat4));
      return;
}
void mat4_identity(mat4* m) {
      for (int i= 0; i < 4; i++) {
            for (int j= 0; j < 4; j++) {
                  m->m[j][i]= j == i ? 1 : 0;
            }
      }
      return;
}
void mat4_digits(mat4* m) {
      for (int i= 0; i < 4; i++) {
            for (int j= 0; j < 4; j++) {
                  m->m[j][i]= 4 * i + j;
            }
      }
      return;
}
void mat4_translation(mat4* m, float x, float y, float z) {
      for (int i= 0; i < 4; i++) {
            for (int j= 0; j < 4; j++) {
                  m->m[j][i]= j == i ? 1 : 0;
            }
      }
      m->m[3][0]= x;
      m->m[3][1]= y;
      m->m[3][2]= z;
      return;
}
// --> Mat4 computing functions
void mat4_mul(mat4* a, mat4* b, mat4* out) {
      memset(out, 0x00, sizeof(mat4));
      for (int i= 0; i < 4; i++) {
            for (int j= 0; j < 4; j++) {
                  for (int k= 0; k < 4; k++) {
                        out->m[j][i] += a->m[k][i] * b->m[j][k];
                  }
            }
      }
      return;
}
void mat4_X_vec4(mat4* m, vec4* v, vec4* out) {
      memset(out, 0x00, sizeof(vec4));
      for (int i= 0; i < 4; i++) {
            for (int j= 0; j < 4; j++) {
                  out->v[i] += m->m[j][i] * v->v[j];
            }
      }
      return;
}
void mat4_translate(mat4* m, vec3* v, mat4* out) {
      for (int i= 0; i < 3; i++) {
            out->m[3][i]= m->m[3][i] + v->v[i];
      }
      return;
}
void mat4_rotate(mat4* m, float radians, vec3* v, mat4* out) {
      float cosT= cos(radians);
      float sinT= sin(radians);
      vec3_normalize(v, v);
      float x= v->x;
      float y= v->y;
      float z= v->z;
      mat4 rot;
      rot.m[0][0]= (cosT + (x * x) * (1.0 - cosT));
      rot.m[1][0]= (x * y * (1.0 - cosT) - z * sinT);
      rot.m[2][0]= (x * z * (1.0 - cosT) + y * sinT);
      rot.m[3][0]= 0.0;
      rot.m[0][1]= (y * x * (1.0 - cosT) + z * sinT);
      rot.m[1][1]= (cosT + (y * y) * (1.0 - cosT));
      rot.m[2][1]= (y * z * (1.0 - cosT) - x * sinT);
      rot.m[3][1]= 0.0;
      rot.m[0][2]= (z * x * (1.0 - cosT) - y * sinT);
      rot.m[1][2]= (z * y * (1.0 - cosT) + x * sinT);
      rot.m[2][2]= (cosT + (z * z) * (1.0 - cosT));
      rot.m[3][2]= 0.0;
      rot.m[0][3]= 0.0;
      rot.m[1][3]= 0.0;
      rot.m[2][3]= 0.0;
      rot.m[3][3]= 1.0;
      mat4 in;
      memcpy(&in, m, sizeof(mat4));
      mat4_mul(&in, &rot, out);
      return;
}
void mat4_scale(mat4* m, vec3* v, mat4* out) {
      for (int i= 0; i < 3; i++) {
            for (int j= 0; j < 3; j++) {
                  out->m[j][i]= m->m[j][i] * v->v[j];
            }
      }
      return;
}
// --> Camera matrix functions
void glm_lookAt(vec3* pos, vec3* target, vec3* up, mat4* viewMat) {
      memset(viewMat, 0x00, sizeof(mat4));
      vec3 zaxis;
      vec3_sub(target, pos, &zaxis);
      vec3_normalize(&zaxis, &zaxis);
      vec3 xaxis;
      vec3_cross(&zaxis, up, &xaxis);
      vec3_normalize(&xaxis, &xaxis);
      vec3 yaxis;
      vec3_cross(&xaxis, &zaxis, &yaxis);
      vec3_negate(&zaxis, &zaxis);
      viewMat->m[0][0]= xaxis.v[0];
      viewMat->m[1][0]= xaxis.v[1];
      viewMat->m[2][0]= xaxis.v[2];
      viewMat->m[3][0]= -vec3_dot(&xaxis, pos);
      viewMat->m[0][1]= yaxis.v[0];
      viewMat->m[1][1]= yaxis.v[1];
      viewMat->m[2][1]= yaxis.v[2];
      viewMat->m[3][1]= -vec3_dot(&yaxis, pos);
      viewMat->m[0][2]= zaxis.v[0];
      viewMat->m[1][2]= zaxis.v[1];
      viewMat->m[2][2]= zaxis.v[2];
      viewMat->m[3][2]= -vec3_dot(&zaxis, pos);
      viewMat->m[0][3]= 0.0;
      viewMat->m[1][3]= 0.0;
      viewMat->m[2][3]= 0.0;
      viewMat->m[3][3]= 1.0;
      return;
}
void glm_perspective(float fov, float aspect, float nearZ, float farZ, mat4* projMat) {
      memset(projMat, 0x00, sizeof(mat4));
      float f= 1.0 / tan(fov * 0.5);
      float fn= 1.0 / (nearZ - farZ);
      projMat->m[0][0]= f / aspect;
      projMat->m[1][1]= f;
      projMat->m[2][2]= farZ * fn;
      projMat->m[2][3]= -1.0;
      projMat->m[3][2]= nearZ * farZ * fn;
      return;
}
void glm_orthographic(float l, float r, float b, float t, float nearZ, float farZ, mat4* projMat) {
      memset(projMat, 0x00, sizeof(mat4));
      float rl= 1.0 / (r - l);
      float tb= 1.0 / (t - b);
      float fn= -1.0 / (farZ - nearZ);
      projMat->m[0][0]= 2.0 * rl;
      projMat->m[1][1]= 2.0 * tb;
      projMat->m[2][2]= fn;
      projMat->m[3][0]= -(r + l) * rl;
      projMat->m[3][1]= -(t + b) * tb;
      projMat->m[3][2]= nearZ * fn;
      projMat->m[3][3]= 1.0;
      return;
}

/* Vector Functions ***************************************************/
// --> Vector printing functions
void vec2_fprintf(FILE* stream, const char* name, vec2* m) {
      fprintf(stream, "%s { %f, %f }\n", name, m->x, m->y);
      return;
}
void vec3_fprintf(FILE* stream, const char* name, vec3* m) {
      fprintf(stream, "%s { %f, %f, %f }\n", name, m->x, m->y, m->z);
      return;
}
void vec4_fprintf(FILE* stream, const char* name, vec4* m) {
      fprintf(stream, "%s { %f, %f, %f, %f }\n", name, m->x, m->y, m->z, m->w);
      return;
}
// --> Initialize vector at v to specific values
void vec2_init(vec2* v, float x, float y) {
      v->x= x;
      v->y= y;
      return;
}
void vec3_init(vec3* v, float x, float y, float z) {
      v->x= x;
      v->y= y;
      v->z= z;
      return;
}
void vec4_init(vec4* v, float x, float y, float z, float w) {
      v->x= x;
      v->y= y;
      v->z= z;
      v->w= w;
      return;
}
vec2 vec2_getVec(float x, float y) {
      vec2 v;
      vec2_init(&v, x, y);
      return v;
}
vec3 vec3_getVec(float x, float y, float z) {
      vec3 v;
      vec3_init(&v, x, y, z);
      return v;
}
vec4 vec4_getVec(float x, float y, float z, float w) {
      vec4 v;
      vec4_init(&v, x, y, z, w);
      return v;
}
// --> Vec2 computing functions
float vec2_length(vec2* v) {
      return sqrt(v->x * v->x + v->y * v->y);
}
void vec2_normalize(vec2* v, vec2* out) {
      float len= vec2_length(v);
      out->x= v->x / len;
      out->y= v->y / len;
      return;
}
void vec2_negate(vec2* v, vec2* out) {
      out->x= -v->x;
      out->y= -v->y;
      return;
}
void vec2_scale(vec2* v, float s, vec2* out) {
      out->x= v->x * s;
      out->y= v->y * s;
      return;
}
void vec2_add(vec2* a, vec2* b, vec2* out) {
      out->x= a->x + b->x;
      out->y= a->y + b->y;
      return;
}
void vec2_sub(vec2* a, vec2* b, vec2* out) {
      out->x= a->x - b->x;
      out->y= a->y - b->y;
      return;
}
float vec2_dot(vec2* a, vec2* b) {
      return a->x * b->x + a->y * b->y;
}
float vec2_angle(vec2* a, vec2* b) {
      float dot= vec2_dot(a, b);
      float lenA= vec2_length(a);
      float lenB= vec2_length(b);
      return acos(dot / (lenA * lenB));
}
// --> Vec3 computing functions
float vec3_length(vec3* v) {
      return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}
void vec3_normalize(vec3* v, vec3* out) {
      float len= vec3_length(v);
      out->x= v->x / len;
      out->y= v->y / len;
      out->z= v->z / len;
      return;
}
void vec3_negate(vec3* v, vec3* out) {
      out->x= -v->x;
      out->y= -v->y;
      out->z= -v->z;
      return;
}
void vec3_scale(vec3* v, float s, vec3* out) {
      out->x= v->x * s;
      out->y= v->y * s;
      out->z= v->z * s;
      return;
}
void vec3_add(vec3* a, vec3* b, vec3* out) {
      out->x= a->x + b->x;
      out->y= a->y + b->y;
      out->z= a->z + b->z;
      return;
}
void vec3_sub(vec3* a, vec3* b, vec3* out) {
      out->x= a->x - b->x;
      out->y= a->y - b->y;
      out->z= a->z - b->z;
      return;
}
float vec3_dot(vec3* a, vec3* b) {
      return a->x * b->x + a->y * b->y + a->z * b->z;
}
float vec3_angle(vec3* a, vec3* b) {
      float dot= vec3_dot(a, b);
      float lenA= vec3_length(a);
      float lenB= vec3_length(b);
      return acos(dot / (lenA * lenB));
}
void vec3_cross(vec3* a, vec3* b, vec3* out) {
      out->x= a->y * b->z - a->z * b->y;
      out->y= a->z * b->x - a->x * b->z;
      out->z= a->x * b->y - a->y * b->x;
      return;
}
// --> Vec4 computing functions
float vec4_length(vec4* v) {
      return sqrt(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}
void vec4_normalize(vec4* v, vec4* out) {
      float len= vec4_length(v);
      out->x= v->x / len;
      out->y= v->y / len;
      out->z= v->z / len;
      out->w= v->w / len;
      return;
}
void vec4_negate(vec4* v, vec4* out) {
      out->x= -v->x;
      out->y= -v->y;
      out->z= -v->z;
      out->w= -v->w;
      return;
}
void vec4_scale(vec4* v, float s, vec4* out) {
      out->x= v->x * s;
      out->y= v->y * s;
      out->z= v->z * s;
      out->w= v->w * s;
      return;
}
void vec4_add(vec4* a, vec4* b, vec4* out) {
      out->x= a->x + b->x;
      out->y= a->y + b->y;
      out->z= a->z + b->z;
      out->w= a->w + b->w;
      return;
}
void vec4_sub(vec4* a, vec4* b, vec4* out) {
      out->x= a->x - b->x;
      out->y= a->y - b->y;
      out->z= a->z - b->z;
      out->w= a->w - b->w;
      return;
}
float vec4_dot(vec4* a, vec4* b) {
      return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}
// --> Special vector functions for graphics computations
void vec3_translate(vec3* v, mat4* m, vec3* out) {
      for (int i= 0; i < 3; i++) {
            out->v[i]= v->v[i] + m->m[3][i];
      }
}