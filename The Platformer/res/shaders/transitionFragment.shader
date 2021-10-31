uniform sampler2D texture;

float diamondPixelSize = 50.f;
uniform vec2 u_resolution;
uniform float progress;
void main()
{
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;

    uv *= 1.0 - uv.yx;

    float vig = uv.x * uv.y * 50.0;

    vig = pow(vig, 0.25); 

    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    float UVx = gl_FragCoord.x / u_resolution.x;
    float UVy = gl_FragCoord.y / u_resolution.y;

    float xFraction = fract(gl_FragCoord.x / diamondPixelSize);
    float yFraction = fract(gl_FragCoord.y / diamondPixelSize);

    float xDistance = abs(xFraction - 0.5);
    float yDistance = abs(yFraction - 0.5);

    if (xDistance + yDistance + UVx + UVy > progress * 4.f) {
        discard;
    }

    gl_FragColor = gl_Color * vig * pixel;
}