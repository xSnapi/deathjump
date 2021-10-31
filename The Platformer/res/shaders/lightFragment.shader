uniform sampler2D texture;

#define PI 3.1415

//vec4 color = vec4(0.7, 0.4, 0.05, 0.75);
const float alpha_power = 2.0;

uniform float u_time;
uniform float u_move;
uniform vec4 color;

void main()
{
    vec2 uv = gl_TexCoord[0].xy * 2.0 - 1.0;

    float div = 0.5 * abs(sin(u_time + PI * u_move)) + 0.7;

    float distance = 1.0 - length(uv / div);

    /*if (distance < 0.5) {
        color.r = 0.85;
        color.g = 0.4;
    }*/

    if (distance < 0.0)
        discard;

    //distance = 1.0;

    gl_FragColor = vec4(color.rgb, pow(distance, alpha_power) * color.a);
}
