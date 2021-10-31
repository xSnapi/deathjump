uniform sampler2D texture;

uniform float u_blur_radius;

void main()
{
    vec2 offx = vec2(u_blur_radius, 0.0);
    vec2 offy = vec2(0.0, u_blur_radius);

    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy) * 4.0 +
        texture2D(texture, gl_TexCoord[0].xy - offx) * 2.0 +
        texture2D(texture, gl_TexCoord[0].xy + offx) * 2.0 +
        texture2D(texture, gl_TexCoord[0].xy - offy) * 2.0 +
        texture2D(texture, gl_TexCoord[0].xy + offy) * 2.0 +
        texture2D(texture, gl_TexCoord[0].xy - offx - offy) * 1.0 +
        texture2D(texture, gl_TexCoord[0].xy - offx + offy) * 1.0 +
        texture2D(texture, gl_TexCoord[0].xy + offx - offy) * 1.0 +
        texture2D(texture, gl_TexCoord[0].xy + offx + offy) * 1.0;

    gl_FragColor = gl_Color * (pixel / 16.0);
}