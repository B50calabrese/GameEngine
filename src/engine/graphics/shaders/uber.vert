#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec4 aColor2;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in vec2 aLocalPos;
layout (location = 5) in float aTexIndex;
layout (location = 6) in float aShapeType;
layout (location = 7) in float aThickness;
layout (location = 8) in float aRoundness;
layout (location = 9) in float aGradientType;
layout (location = 10) in float aIsFont;
layout (location = 11) in float aIsDashed;

uniform mat4 u_ViewProjection;

out vec4 vColor;
out vec4 vColor2;
out vec2 vTexCoord;
out vec2 vLocalPos;
out float vTexIndex;
out float vShapeType;
out float vThickness;
out float vRoundness;
out float vGradientType;
out float vIsFont;
out float vIsDashed;

void main() {
    vColor = aColor;
    vColor2 = aColor2;
    vTexCoord = aTexCoord;
    vLocalPos = aLocalPos;
    vTexIndex = aTexIndex;
    vShapeType = aShapeType;
    vThickness = aThickness;
    vRoundness = aRoundness;
    vGradientType = aGradientType;
    vIsFont = aIsFont;
    vIsDashed = aIsDashed;
    gl_Position = u_ViewProjection * vec4(aPos, 0.0, 1.0);
}
