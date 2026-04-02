#version 330 core
in vec4 vColor;
in vec4 vColor2;
in vec2 vTexCoord;
in vec2 vLocalPos;
in float vTexIndex;
in float vShapeType;
in float vThickness;
in float vRoundness;
in float vGradientType;
in float vIsFont;
in float vIsDashed;

uniform sampler2D uTextures[32];

out vec4 FragColor;

float sdRoundRect(vec2 p, vec2 b, float r) {
    vec2 d = abs(p) - b + vec2(r);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - r;
}

float sdCircle(vec2 p, float r) {
    return length(p) - r;
}

float sdTriangle(vec2 p, float r) {
    const float k = sqrt(3.0);
    p.x = abs(p.x) - r;
    p.y = p.y + r/k;
    if( p.x+k*p.y>0.0 ) p = vec2(p.x-k*p.y,-k*p.x-p.y)/2.0;
    p.x -= clamp( p.x, -2.0*r, 0.0 );
    return -length(p)*sign(p.y);
}

void main() {
    vec4 texColor;
    int index = int(vTexIndex);
    switch(index) {
        case 0:  texColor = texture(uTextures[0], vTexCoord); break;
        case 1:  texColor = texture(uTextures[1], vTexCoord); break;
        case 2:  texColor = texture(uTextures[2], vTexCoord); break;
        case 3:  texColor = texture(uTextures[3], vTexCoord); break;
        case 4:  texColor = texture(uTextures[4], vTexCoord); break;
        case 5:  texColor = texture(uTextures[5], vTexCoord); break;
        case 6:  texColor = texture(uTextures[6], vTexCoord); break;
        case 7:  texColor = texture(uTextures[7], vTexCoord); break;
        case 8:  texColor = texture(uTextures[8], vTexCoord); break;
        case 9:  texColor = texture(uTextures[9], vTexCoord); break;
        case 10: texColor = texture(uTextures[10], vTexCoord); break;
        case 11: texColor = texture(uTextures[11], vTexCoord); break;
        case 12: texColor = texture(uTextures[12], vTexCoord); break;
        case 13: texColor = texture(uTextures[13], vTexCoord); break;
        case 14: texColor = texture(uTextures[14], vTexCoord); break;
        case 15: texColor = texture(uTextures[15], vTexCoord); break;
        case 16: texColor = texture(uTextures[16], vTexCoord); break;
        case 17: texColor = texture(uTextures[17], vTexCoord); break;
        case 18: texColor = texture(uTextures[18], vTexCoord); break;
        case 19: texColor = texture(uTextures[19], vTexCoord); break;
        case 20: texColor = texture(uTextures[20], vTexCoord); break;
        case 21: texColor = texture(uTextures[21], vTexCoord); break;
        case 22: texColor = texture(uTextures[22], vTexCoord); break;
        case 23: texColor = texture(uTextures[23], vTexCoord); break;
        case 24: texColor = texture(uTextures[24], vTexCoord); break;
        case 25: texColor = texture(uTextures[25], vTexCoord); break;
        case 26: texColor = texture(uTextures[26], vTexCoord); break;
        case 27: texColor = texture(uTextures[27], vTexCoord); break;
        case 28: texColor = texture(uTextures[28], vTexCoord); break;
        case 29: texColor = texture(uTextures[29], vTexCoord); break;
        case 30: texColor = texture(uTextures[30], vTexCoord); break;
        case 31: texColor = texture(uTextures[31], vTexCoord); break;
        default: texColor = vec4(1.0); break;
    }

    if (vIsFont > 0.5) {
        texColor = vec4(1.0, 1.0, 1.0, texColor.r);
    }

    float dist = 0.0;
    if (vShapeType == 0.0) { // Quad/Rounded Quad
        dist = sdRoundRect(vLocalPos, vec2(1.0, 1.0), vRoundness);
    } else if (vShapeType == 1.0) { // Circle
        dist = sdCircle(vLocalPos, 1.0);
    } else if (vShapeType == 2.0) { // Triangle
        dist = sdTriangle(vLocalPos, 1.0);
    } else if (vShapeType == 3.0) { // Line
        dist = 0.0;
    } else if (vShapeType == 4.0) { // Point
        dist = sdCircle(vLocalPos, 1.0);
    } else if (vShapeType == 5.0) { // Polygon
        dist = 0.0; // Polygons are rendered as triangle fans
    }

    if (dist > 0.0 && vShapeType != 5.0) discard;

    vec4 color1 = vColor;
    vec4 color2 = vColor2;
    vec4 baseColor = color1;

    if (vGradientType == 1.0) { // Linear
        float t = clamp(vLocalPos.y * 0.5 + 0.5, 0.0, 1.0);
        baseColor = mix(color1, color2, t);
    } else if (vGradientType == 2.0) { // Radial
        float t = clamp(length(vLocalPos), 0.0, 1.0);
        baseColor = mix(color1, color2, t);
    }

    vec4 finalColor = baseColor * texColor;

    if (vShapeType == 3.0 && vIsDashed > 0.5) {
        // Simple pixel-space-like dashing using LocalPos
        // We assume LocalPos.x is the normalized distance along the line.
        // Actually, it's easier to just use a fixed dash size.
        if (mod(vLocalPos.x * 20.0, 2.0) < 1.0) discard;
    }

    if (vThickness > 0.0 && vShapeType != 3.0 && vShapeType != 5.0) {
        float edge = 0.02; // Anti-aliasing
        float alpha = smoothstep(0.0, -edge, dist) - smoothstep(-vThickness, -vThickness - edge, dist);
        finalColor.a *= alpha;
    }

    FragColor = finalColor;
}
