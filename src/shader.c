#include "shader.h"

GraphicsShader createGraphicsShaderProgram(const char* vShaderFile, const char* fShaderFile) {
    GraphicsShader program;
    program.shaderProgram= 0;

    glUseProgram(0);

    // Fetch the Vertex Shader source file
    FILE* vertexShaderFile= fopen(vShaderFile, "r");
    if (vertexShaderFile == NULL) {
        fprintf(stderr, "Error: Vertex shader file not found!\n");
        return program;
    }
    int vLen= 0;
    while (fgetc(vertexShaderFile) != EOF) {
        vLen++;
    }
    fseek(vertexShaderFile, 0, SEEK_SET);
    char* vBuf= malloc(vLen + 1);
    for (int i= 0; i < vLen; i++) {
        vBuf[i]= fgetc(vertexShaderFile);
    }
    vBuf[vLen]= '\0';
    fclose(vertexShaderFile);

    // Fetch the Fragment Shader source file
    FILE* fragmentShaderFile= fopen(fShaderFile, "r");
    if (fragmentShaderFile == NULL) {
        fprintf(stderr, "Error: Fragment shader file not found!\n");
        return program;
    }
    int fLen= 0;
    while (fgetc(fragmentShaderFile) != EOF) {
        fLen++;
    }
    fseek(fragmentShaderFile, 0, SEEK_SET);
    char* fBuf= malloc(fLen + 1);
    for (int i= 0; i < fLen; i++) {
        fBuf[i]= fgetc(fragmentShaderFile);
    }
    fBuf[fLen]= '\0';
    fclose(fragmentShaderFile);

    // Create Shader Program
    unsigned int vShader= 0, fShader= 0;

    // Compile vertex shader
    vShader= glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, (const char* const*) &vBuf, NULL);
    glCompileShader(vShader);

    // Do some error checking
    int vShaderStatus= GL_FALSE;
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vShaderStatus);
    if (vShaderStatus == GL_FALSE) {
        char log[512];
        memset(log, 0x00, sizeof(log));
        glGetShaderInfoLog(vShader, 512*sizeof(char), NULL, log);
        fprintf(stderr, "Vertex Shader error for %s: %s", vShaderFile, log);
        glDeleteShader(vShader);
        exit(1);
    }

    // Compile fragment shader
    fShader= glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, (const char* const*) &fBuf, NULL);
    glCompileShader(fShader);

    // Do some error checking
    int fShaderStatus= GL_FALSE;
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fShaderStatus);
    if (fShaderStatus == GL_FALSE) {
        char log[512];
        memset(log, 0x00, sizeof(log));
        glGetShaderInfoLog(fShader, 512*sizeof(char), NULL, log);
        fprintf(stderr, "Fragment Shader error for %s: %s", fShaderFile, log);
        glDeleteShader(fShader);
        exit(1);
    }

    if (vShaderStatus == GL_TRUE && fShaderStatus == GL_TRUE) {
        program.shaderProgram= glCreateProgram();
        glAttachShader(program.shaderProgram, vShader);
        glAttachShader(program.shaderProgram, fShader);
        glLinkProgram(program.shaderProgram);
        int shaderProgramStatus= GL_FALSE;
        glGetProgramiv(program.shaderProgram, GL_LINK_STATUS, &shaderProgramStatus);
        if (shaderProgramStatus == GL_FALSE) {
            char log[512];
            memset(log, 0x00, sizeof(log));
            glGetProgramInfoLog(program.shaderProgram, 512*sizeof(char), NULL, log);
            fprintf(stderr, "Shader Program Linking error for %s: %s", vShaderFile, log);
            glDeleteProgram(program.shaderProgram);
            glDeleteShader(vShader);
            glDeleteShader(fShader);
            exit(1);
        }
        glUseProgram(program.shaderProgram);
    }
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    // Free shader code buffers
    free(vBuf);
    free(fBuf);
    return program;
}
