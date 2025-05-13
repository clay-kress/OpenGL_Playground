#version 450 core

in vec2 texOut;

out vec4 pixelColor;

layout (binding = 0) uniform samplerCube starMap;
layout (binding = 1) uniform sampler2D UItexture;
layout (binding = 2) uniform sampler2D colorBuffer;
layout (binding = 3) uniform sampler2D depthBuffer;

uniform mat4 starRotation;
uniform float dayNightCircle;

uniform vec3 sunPos;
uniform vec3 playerPos;
uniform vec3 focus;
uniform vec3 up;
uniform vec3 right;

uniform float width;
uniform float height;
uniform float fov;

float dstToSpace(float phi, int downShft, int radius);

void main() {

    float aspectRatio= width/height;
    float fovY= fov/aspectRatio;
    float fovX= fov;
    float pixelVerticalAngle= (gl_FragCoord.y/height * 2.0 - 1.0) * (fovY);
    float pixelHorizontalAngle= (gl_FragCoord.x/width * 2.0 - 1.0) * (fovX);
    vec3 pixelAngle= normalize(focus + (right*pixelHorizontalAngle + up*pixelVerticalAngle));
    vec3 sunDirection= normalize(sunPos - playerPos);

    int atmosphereDownShft= 500, atmosphereRadius= 1000;
    vec3 atmosphereCenter= vec3(playerPos.x, playerPos.y-atmosphereDownShft, playerPos.z);
    float atmosphereDepthAtPixel= dstToSpace(pixelAngle.y, atmosphereDownShft, atmosphereRadius)/(atmosphereRadius);

    vec4 calculatedPixelColor= vec4(0.0);
    float amountTowardsTheSun= dot(pixelAngle, sunDirection) / 2.0 + 0.5;
    float amountThatSunIsUpward= dot(vec3(0.0, 1.0, 0.0), sunDirection);
    float red= min(0.6, pow(amountTowardsTheSun+0.5, 2) * pow(length(cross(sunDirection, vec3(0, 1, 0))), 69) / (10*max(0.03, pixelAngle.y))) * (1 - 5*pow(max(0.0, -amountThatSunIsUpward), 1));
    float green= pow(max(0.0, amountThatSunIsUpward), 0.35) * 0.4 + 0.01 * pow(1-abs(amountThatSunIsUpward), 42) / (max(0.04, pixelAngle.y));
    float blue= pow(max(0.0, amountThatSunIsUpward), 0.4) * 0.8 + 0.2 * pow(1-abs(amountThatSunIsUpward), 21);
    calculatedPixelColor = atmosphereDepthAtPixel * vec4(red, green, blue, 1.0);
    if (texture(depthBuffer, texOut).r != 1.0) {
        // Do world stuff
        pixelColor = texture(colorBuffer, texOut) + calculatedPixelColor * 0.1 * pow(amountTowardsTheSun, 0.8);
    } else {
        // Do sky stuff
        pixelColor= calculatedPixelColor + max(0, pow(amountTowardsTheSun, 69)) * (1 - 5*pow(max(0.0, -amountThatSunIsUpward), 1)) * vec4(0.7, 0.5, 0.1, 1.0) + max(0.0, -cos(dayNightCircle)) * texture(starMap, vec3(starRotation*vec4(pixelAngle, 1.0)));
    }
    pixelColor += texture(UItexture, texOut);
}

float dstToSpace(float phi, int downShft, int radius) {
    float c= downShft;
    float r= radius;
    float rho = sqrt(pow(c,2) * pow((phi),2) - pow(c,2) + pow(r,2)) - c * (phi);
    return rho;
}
