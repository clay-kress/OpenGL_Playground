#version 450 core

in vec3 vertexOut;
in vec3 normOut;
in vec4 colorOut;
in vec2 texOut;

layout (location = 0) out vec4 pixelColor;

layout (binding = 0) uniform sampler2D blockAtlas;

uniform vec3 sunPos;
uniform vec3 playerPos;
uniform float headBrightness;

void main() {
    vec3 sunDirection= normalize(sunPos - playerPos);
    float amountThatSunIsUpward= max(0.0, dot(vec3(0.0, 1.0, 0.0), sunDirection) / 2.0 + 0.1); // Bias so that even the sides perpendicular to the sun are lit up a little bit
    float playerLighting= headBrightness * abs(dot(normalize(normOut), normalize(playerPos-vertexOut))) / max(2.0, length(playerPos-vertexOut));
    playerLighting /= (1 + 5*amountThatSunIsUpward);
    if (colorOut != vec4(0.0, 0.0, 0.0, 0.0)) { // If not sun pixels
        float solarLighting= pow(max(0.0, dot(normalize(normOut), sunDirection) / 2.0 + 0.5), .6) * amountThatSunIsUpward;
        float ambient= 0.1;
        pixelColor = texture(blockAtlas, texOut) * (solarLighting + playerLighting + ambient);
    } else {
        // Sun pixels
        pixelColor = texture(blockAtlas, texOut);
    }
}
