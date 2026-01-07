#ifndef LINALG_H
#define LINALG_H

#define PI 3.14159265358979323846264338327950288419716939937510582097494
#define E 2.718281828459045235360287471352662497757247093699959574966967

#define RAD(x) x*(PI/180.0)
#define DEG(x) x*(180.0/PI)

typedef struct {
    float m[4][4];
} mat4;

typedef union {
    struct {
        float x;
        float y;
    };
    struct {
        float u;
        float v;
    };
    struct {
        float s;
        float t;
    };
    float vec[2];
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
    float vec[3];
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
    float vec[4];
} vec4;

void printMat4(const char* name, mat4 m);
void printVec4(const char* name, vec4 vec);
void printVec3(const char* name, vec3 vec);

vec3 vec3_floor(vec3 v);
vec3 vec3_ceil(vec3 v);
vec3 vec3_round(vec3 v);

vec3 vec3_negate(vec3 a);
vec3 vec3_normalize(vec3 v);
float vec3_length(vec3 v);

vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_scale(vec3 a, float f);

vec3 vec3_cross(vec3 a, vec3 b);
float vec3_dot(vec3 a, vec3 b);

mat4 mat4_multiply(mat4 m1, mat4 m2);
vec4 mat4_X_vec4(mat4 mat, vec4 vec);
vec3 vec4_to_vec3(vec4 vec);
vec4 vec3_to_vec4(vec3 vec);

mat4 linAlg_Perspective(float fov, float nearZ, float farZ, float aspect);
mat4 linAlg_Orthographic(float left, float right, float bottom, float top, float nearZ, float farZ);
mat4 linAlg_View(vec3 position, vec3 direction, vec3 up);

mat4 linAlg_Id(void);
mat4 linAlg_Translate(mat4 mat, float x, float y, float z);
mat4 linAlg_Rotate(mat4 mat, vec3 a, float th);
mat4 linAlg_Scale(mat4 mat, float x, float y, float z);

typedef union {
    struct {
        unsigned int x;
        unsigned int y;
        unsigned int z;
    };
    struct {
        unsigned int r;
        unsigned int g;
        unsigned int b;
    };
    unsigned int vec[3];
} vec3ui;

vec3ui vec3_truncate(vec3 vec);

#endif
