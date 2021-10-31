uniform float u_time;
uniform vec2 u_resolution;

#define PI 3.1415

//const vec4 color = vec4(0.7, 0.19, 0.89, 0.73);
//const vec4 color = vec4(0.7, 0.65, 0.8, 0.95);
const vec4 color = vec4(0.8, 0.8, 0.8, 0.85);
const float noise_scale = 20.0;
const float alpha_power = 2.0;
const int OCTAVES = 8;

float rand(vec2 coord) {
	return fract(5.0 * sin(dot(coord, vec2(56.0, 78.0)) * 1000.0) * 1000.0);
}

float noise(vec2 coord) {
	vec2 i = floor(coord);
	vec2 f = fract(coord);

	float a = rand(i);
	float b = rand(i + vec2(1.0, 0.0));
	float c = rand(i + vec2(0.0, 1.0));
	float d = rand(i + vec2(1.0, 1.0));

	vec2 cubic = f * f * (3.0 - 2.0 * f);

	return mix(a, b, cubic.x) + (c - a) * cubic.y * (1.0 - cubic.x) + (d - b) * cubic.x * cubic.y;
}

float fbm(vec2 coord) {
	float value = 0.0;
	float scale = 0.5;

	for (int i = 0; i < OCTAVES; i++) {
		value += noise(coord) * scale;
		coord *= 2.0;
		scale *= 0.5;
	}
	return value;
}

float parabola(float x, float k) {
	return pow(cos(PI * x / 2.0), k);
}


void main()
{
	vec2 UV = gl_FragCoord.xy / u_resolution.xy;

	vec2 coord = UV * noise_scale;
	vec2 motion = vec2(fbm(coord + vec2(u_time * -0.2, u_time * 0.2)));
	float final = fbm(coord + motion);
	
	float y = parabola(UV.y, 8.0);
	vec3 parabolaColor = vec3(y);


    gl_FragColor = vec4(color.rgb, min(parabolaColor, pow(final, alpha_power)) * color.a);
}