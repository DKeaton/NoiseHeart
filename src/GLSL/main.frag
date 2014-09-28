#version 120

uniform sampler2D texture;
uniform sampler2D palette;
uniform vec4 offset;
uniform int layouts[4];

varying vec4 params;

vec4 calcColor(int lo) {
    float result = 0.0;
    vec2 point = vec2(params.x, (params.w + offset[lo] + lo) * 0.25);
    for (int i = 0; i < params.y; i++) {
        result += texture2D(texture, point).r;
        point.x += params.z;
    }
    result /= params.y;
    return texture2D(palette, vec2(result, 0.25 * lo));
}

void main(void) {
    gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < 4; i++) {
        if (layouts[i] != 0) {
            gl_FragColor = max(gl_FragColor, calcColor(i));
        }
    }
}
