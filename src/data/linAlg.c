#include "linAlg.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

void printMat4(const char* name, mat4 mat) {
    printf("%s {\n   ", name);
    for (int i= 0; i < 4; i++) {
        for (int j= 0; j < 4; j++) {
            printf("%f", mat.m[i][j]);
            if (j != 3 || i != 3) {
                printf(", ");
            }
        }
        if (i != 3) {
            printf("\n   ");
        }
    }
    printf("\n}\n");
    return;
}

void printVec4(const char* name, vec4 vec) {
    printf("%s {", name);
    for (int i= 0; i < 4; i++) {
        printf("%f", vec.vec[i]);
        if (i != 3) {
            printf(", ");
        }
    }
    printf("}\n");
    return;
}

void printVec3(const char* name, vec3 vec) {
    printf("%s {", name);
    for (int i= 0; i < 3; i++) {
        printf("%f", vec.vec[i]);
        if (i != 2) {
            printf(", ");
        }
    }
    printf("}\n");
    return;
}

vec3 vec3_floor(vec3 v) {
    return (vec3) {{floor(v.x), floor(v.y), floor(v.z)}};
}
vec3 vec3_ceil(vec3 v) {
    return (vec3) {{ceil(v.x), ceil(v.y), ceil(v.z)}};
}
vec3 vec3_round(vec3 v) {
    return (vec3) {{round(v.x), round(v.y), round(v.z)}};
}

vec3 vec3_negate(vec3 a) {
    a.x *= -1;
    a.y *= -1;
    a.z *= -1;
    return a;
}

vec3 vec3_normalize(vec3 v) {
    float length= sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    v.x /= length;
    v.y /= length;
    v.z /= length;
    return v;
}

float vec3_length(vec3 v) {
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

vec3 vec3_add(vec3 a, vec3 b) {
    return (vec3) {{a.x + b.x, a.y + b.y, a.z + b.z}};
}

vec3 vec3_sub(vec3 a, vec3 b) {
    return (vec3) {{a.x - b.x, a.y - b.y, a.z - b.z}};
}

vec3 vec3_scale(vec3 a, float f) {
    return (vec3) {{a.x * f, a.y * f, a.z * f}};
}

vec3 vec3_cross(vec3 a, vec3 b) {
    return (vec3) {{a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x}};
}

float vec3_dot(vec3 a, vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

mat4 mat4_multiply(mat4 m1, mat4 m2) {
      mat4 res;
      memset(&res, 0, sizeof(mat4));
      for (int i= 0; i < 4; i++) {
            for (int j= 0; j < 4; j++) {
                  res.m[i][j] += (m1.m[i][0] * m2.m[0][j]) + (m1.m[i][1] * m2.m[1][j]) + (m1.m[i][2] * m2.m[2][j]) + (m1.m[i][3] * m2.m[3][j]);
            }
      }
      return res;
}

vec4 mat4_X_vec4(mat4 mat, vec4 vec) {
      vec4 res;
      memset(&res, 0, sizeof(vec4));
      for (int i= 0; i < 4; i++) {
            for (int j= 0; j < 4; j++) {
                  res.vec[i] += mat.m[i][j] * vec.vec[j];
            }
      }
      return res;
}

vec3 vec4_to_vec3(vec4 vec) {
    vec3 vNew= (vec3) {{vec.x, vec.y, vec.z}};
    return vNew;
}

vec4 vec3_to_vec4(vec3 vec) {
    vec4 vNew= (vec4) {{vec.x, vec.y, vec.z, 1.0}};
    return vNew;
}

mat4 linAlg_Perspective(float fov, float nearZ, float farZ, float aspect) {
    mat4 projMat;
    memset(&projMat, 0, sizeof(projMat));

    float cotFov= 1.0 / tan(RAD(fov * 0.5));
    float fn= (farZ - nearZ);

    projMat.m[0][0]= cotFov/aspect;
    projMat.m[1][1]= cotFov;
    projMat.m[2][2]= -(farZ + nearZ) / fn;
    projMat.m[2][3]= -2 * (nearZ * farZ) / fn;
    projMat.m[3][2]= -1.0;
    return projMat;
}

mat4 linAlg_Orthographic(float left, float right, float bottom, float top, float nearZ, float farZ) {
    mat4 projMat;
    memset(&projMat, 0, sizeof(projMat));

    float rl= 1.0 / (right - left);
    float tb= 1.0 / (top - bottom);
    float fn= 1.0 / (farZ - nearZ);

    projMat.m[0][0]= 2.0 / (right - left);
    projMat.m[1][1]= 2.0 / (top - bottom);
    projMat.m[2][2]= -2.0 * fn;
    projMat.m[0][3]= -(right + left) * rl;
    projMat.m[1][3]= -(top + bottom) * tb;
    projMat.m[2][3]= -(farZ + nearZ) * fn;
    projMat.m[3][3]= 1.0;
    return projMat;
}

mat4 linAlg_View(vec3 position, vec3 direction, vec3 up) {
    mat4 viewMat;
    memset(&viewMat, 0, sizeof(viewMat));

    vec3 right= vec3_normalize(vec3_cross(direction, up));
    up= vec3_normalize(vec3_cross(right, direction));

    viewMat.m[0][0]= right.x;
    viewMat.m[0][1]= right.y;
    viewMat.m[0][2]= right.z;
    viewMat.m[0][3]= -vec3_dot(right, position);
    viewMat.m[1][0]= up.x;
    viewMat.m[1][1]= up.y;
    viewMat.m[1][2]= up.z;
    viewMat.m[1][3]= -vec3_dot(up, position);
    viewMat.m[2][0]= -direction.x;
    viewMat.m[2][1]= -direction.y;
    viewMat.m[2][2]= -direction.z;
    viewMat.m[2][3]= vec3_dot(direction, position);
    viewMat.m[3][0]= 0.0;
    viewMat.m[3][1]= 0.0;
    viewMat.m[3][2]= 0.0;
    viewMat.m[3][3]= 1.0;
    return viewMat;
}

mat4 linAlg_Id(void) {
    return (mat4) {.m[0] = {1,0,0,0}, .m[1] = {0,1,0,0}, .m[2] = {0,0,1,0}, .m[3] = {0,0,0,1}};
}

mat4 linAlg_Translate(mat4 mat, float x, float y, float z) {
    mat.m[0][3] += x;
    mat.m[1][3] += y;
    mat.m[2][3] += z;
    return mat;
}

mat4 linAlg_Rotate(mat4 mat, vec3 a, float th) {
    mat4 rot= linAlg_Id();
    a= vec3_normalize(a);
    rot.m[0][0] = cos(th) + a.x*a.x*(1-cos(th));
    rot.m[0][1] = a.x*a.y*(1-cos(th))-a.z*sin(th);
    rot.m[0][2] = a.x*a.z*(1-cos(th))+a.y*sin(th);
    rot.m[0][3] = 0;
    rot.m[1][0] = a.y*a.x*(1-cos(th))+a.z*sin(th);
    rot.m[1][1] = cos(th) + a.y*a.y*(1-cos(th));
    rot.m[1][2] = a.y*a.z*(1-cos(th))-a.x*sin(th);
    rot.m[1][3] = 0;
    rot.m[2][0] = a.z*a.x*(1-cos(th))-a.y*sin(th);
    rot.m[2][1] = a.z*a.y*(1-cos(th))+a.x*sin(th);
    rot.m[2][2] = cos(th) + a.z*a.z*(1-cos(th));
    rot.m[2][3] = 0;
    rot.m[3][0] = 0;
    rot.m[3][1] = 0;
    rot.m[3][2] = 0;
    rot.m[3][3] = 1;
    return mat4_multiply(rot, mat);
}

mat4 linAlg_Scale(mat4 mat, float x, float y, float z) {
    mat.m[0][0] *= x;
    mat.m[1][1] *= y;
    mat.m[2][2] *= z;
    return mat;
}

vec3ui vec3_truncate(vec3 vec) {
    return (vec3ui) {{(unsigned int) vec.x, (unsigned int) vec.y, (unsigned int) vec.z}};
}
