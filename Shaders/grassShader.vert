#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aTexCoord;

layout(std430, binding = 0) buffer grassPositions {
    float positions[];
};

out vec3 Normal;
out vec3 FragPos;

const float windStrength = 0.8; // Influency on the velocity of the grass movement
const float maxGrassOndulation = windStrength*2; // Depends of the wind strength

uniform mat4 view;
uniform mat4 projection;

uniform float time;


float random(vec2 st) {
    return 0.5 * fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
    (c - a)* u.y * (1.0 - u.x) +
    (d - b) * u.x * u.y;
}

void main()
{
    // Calculate the grass base bend
    float baseCurve = random(vec2(gl_InstanceID, 0.0)) * aPos.y;

    // Calculate a random movement (noise + time) independent for each grass (gl_InstanceID) and each vertex of it (aPos.xz)
    float curveAmount = baseCurve + noise(vec2(time * windStrength - gl_InstanceID) + aPos.xz) * maxGrassOndulation;

    // Random size for each grass
    mat4 scale = mat4(
        (random(vec2(gl_InstanceID, 0.0)) + 0.7), 0.0, 0.0, 0.0,
        0.0, (random(vec2(gl_InstanceID, 0.0)) + 0.7), 0.0, 0.0,
        0.0, 0.0, (random(vec2(gl_InstanceID, 0.0)) + 0.7), 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    // Rotation for bend the grass
    float sinCurve = sin(curveAmount);
    float cosCurve = cos(curveAmount);

    mat4 rotX = mat4(
        1, 0, 0, 0,
        0, cosCurve, -sinCurve, 0,
        0, sinCurve, cosCurve, 0,
        0, 0, 0, 1
    );

    // Rotation to change the grass orientation
    float sinY = sin(gl_InstanceID);
    float cosY = cos(gl_InstanceID);

    mat4 rotY = mat4(
        cosY, 0, sinY, 0,
        0, 1, 0, 0,
        -sinY, 0, cosY, 0,
        0, 0, 0, 1
    );

    // (Pre-calculated CPU) traslation
    mat4 traslation = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        positions[gl_InstanceID*3], positions[gl_InstanceID*3 + 1], positions[gl_InstanceID*3 + 2], 1.0
    );

    mat4 myTransform = traslation * rotY * rotX * scale;

    gl_Position = projection * view * myTransform * vec4(aPos, 1.0);

    // Illumination
    Normal = mat3(transpose(inverse(myTransform))) * aNormal; 
    FragPos = vec3(myTransform * vec4(aPos, 1.0));
}