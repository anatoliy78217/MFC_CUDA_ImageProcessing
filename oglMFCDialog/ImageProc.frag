#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture samplers
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

// Image processing method
uniform int procType;
uniform vec3 filterColor;

//================================= RGB to Illuminance============================= 

float RGB2Luminance (float r, float g, float b)
{
	return dot (vec3(r, g, b), vec3 (0.2125, 0.7154, 0.0721)); // NTSC
	//return dot (vec3(r, g, b), vec3 (0.3086, 0.6094, 0.0820));
}

//================================= Color Filter ==================================

float Maximum (float x, float y, float z)
{
	float max;

	max = x;

	if (max < y) max = y;
	if (max < z) max = z;

	return max;
}

float Minimum (float x, float y, float z)
{
	float min;

	min = x;

	if (min > y) min = y;
	if (min > z) min = z;

	return min;
}


vec3 RGBToHSV (float r, float g, float b)
// r, g, and b are in [0, 1]
// h(ue) in [0, 360]
// s(aturation) and v(alue) are in [0,1] (if s = 0, then h is -1 (undefined))
{
	float max, min, h = 0, s = 0, v = 0;

	max = Maximum (r, g, b);
	min = Minimum (r, g, b);

	v = max;

	s = (max != 0.0)? ((max - min)/max) : 0.0;
	if (s == 0.0) h = -1.0; // h is undefined
	else
	{
		float delta = max - min;
		if (r == max) h = (g - b) / delta;
		else if (g == max) h = 2.0 + (b - r) / delta;
			 else if (b == max) h = 4.0 + (r - g) / delta;

		h *= 60.0;

		if (h < 0.0) h += 360;
	}

	return vec3(h, s, v);
}

vec3 HSVToRGB (float h, float s, float v)
{
	float r = 0, g = 0, b = 0;

	if (s == 0.0)
	{
		if (h == -1.0) // Undefined
		{
			r = v;
			g = v;
			b = v;
		}
		else return vec3(0, 0, 0);
	}
	else
	{
		float f, p, q, t;
		int	   i;

		if (h == 360.0) h = 0.0;

		h /= 60.0;
		i = int(floor(h));		// type conversion (GLSL dostn't support C-style casting)
		f = h - i;
		p = v * (1.0 - s);
		q = v * (1.0 - s * f);
		t = v * (1.0 - s * (1.0 - f));

		switch (i)
		{
			case 0: r = v; g = t; b = p; break;
			case 1: r = q; g = v; b = p; break;
			case 2: r = p; g = v; b = t; break;
			case 3: r = p; g = q; b = v; break;
			case 4: r = t; g = p; b = v; break;
			case 5: r = v; g = p; b = q; break;
		}
	}

	return vec3(r, g, b);
}

vec3 ColorFilter (vec3 color, vec3 filterColor)
{
	// filter color in (h,s,v) color space
	vec3 fhsv = RGBToHSV (filterColor.r, filterColor.g, filterColor.b);
	vec3 hsv  = RGBToHSV (color.r, color.g, color.b);

	return HSVToRGB (fhsv[0], fhsv[1], hsv[2]);
}

//========================================================================

//=================================== Spatial Filters ====================

vec3 Embossing ()
{
	ivec2 ires = textureSize (ourTexture1, 0);
	float ResS = float(ires.s);
	float ResT = float(ires.t);

	vec2 stp0 = vec2(1.0/ResS, 0.0);
	vec2 stpp = vec2 (1.0/ResS, 1.0/ResT);
	vec3 c00  = texture(ourTexture1, TexCoord).rgb;
	vec3 cp1p1 = texture(ourTexture1, TexCoord + stpp).rgb;

	vec3 diffs = c00 - cp1p1;
	float max = diffs.r;
	if (abs(diffs.g) > abs(max)) max = diffs.g;
	if (abs(diffs.b) > abs(max)) max = diffs.b;

	float gray = clamp(max + 0.5, 0.0, 1.0);

	return vec3(gray, gray, gray);

}

vec3 EdgeDetection ()
{
	ivec2 ires = textureSize (ourTexture1, 0);
	float ResS = float(ires.s);
	float ResT = float(ires.t);

	vec2 stp0 = vec2(1.0/ResS, 0.0);
	vec2 st0p = vec2 (0.0, 1.0/ResT);
	vec2 stpp = vec2 (1.0/ResS, 1.0/ResT);
	vec2 stpm = vec2 (1.0/ResS, -1.0/ResT);

	const vec3 w = vec3(0.2125, 0.754, 0.0721);
	float i00 = dot (texture(ourTexture1, TexCoord).rgb, w);
	float im1m1 = dot (texture(ourTexture1, TexCoord-stpp).rgb, w);
	float ip1p1 = dot (texture(ourTexture1, TexCoord+stpp).rgb, w);
	float im1p1 = dot (texture(ourTexture1, TexCoord-stpm).rgb, w);
	float ip1m1 = dot (texture(ourTexture1, TexCoord+stpm).rgb, w);
	float im10 = dot (texture(ourTexture1, TexCoord-stp0).rgb, w);
	float ip10 = dot (texture(ourTexture1, TexCoord+stp0).rgb, w);
	float i0m1 = dot (texture(ourTexture1, TexCoord-st0p).rgb, w);
	float i0p1 = dot (texture(ourTexture1, TexCoord+st0p).rgb, w);

	float h = -1.0 * im1p1 - 2.0 * i0p1 - 1.0 *ip1p1 + 1.0 * im1m1 + 2.0 * i0m1 + 1.0*ip1m1;
	float v = -1.0 * im1m1 - 2.0 * im10 - 1.0 *im1p1 + 1.0 * ip1m1 + 2.0 * ip10 + 1.0*ip1p1;

	float mag = length(vec2(h,v));

	return vec3(mag, mag, mag);

}


//=======================================================================

void main()
{
	color = texture(ourTexture1, TexCoord);
	float lum;

	switch (procType)
	{
		case 1:	// RGB to illuminance
				lum = dot(vec3(color.r, color.g, color.b), vec3(0.2125, 0.7154, 0.0721)) ;
				color = vec4 (lum, lum, lum, 1.0);
				break;
		case 2:	// RGB invert (negative)
				color = vec4(vec3(1.0, 1.0, 1.0)-vec3(color.r,color.g, color.b), 1.0);
				break;
		case 3:	// RGB -> illuminance -> invert (negative)
				lum = dot(vec3(color.r, color.g, color.b), vec3(0.2125, 0.7154, 0.0721)) ;
				color = vec4(vec3(1.0, 1.0, 1.0)-vec3(lum, lum, lum), 1.0);
				break;
		case 4: // Color filter
				vec3 nc = vec3(color.r, color.g, color.b);
				nc = ColorFilter (nc, filterColor);
				color = vec4(nc.r, nc.g, nc.b, 1.0);
				//nc = RGBToHSV(color.r, color.g, color.b);
				//nc = HSVToRGB(nc[0], nc[1], 0.5 * nc[2]);
				//color = vec4(nc.r, nc.g, nc.b, 1.0);
				//color = vec4(color.r * 0.5, color.g * 0.5, color.b *0.5, 1.0);
				break;
		case 5: // Embossing
				color = vec4(Embossing(), 1.0);
				break;
		case 6: // Edge detection
				color = vec4(EdgeDetection(), 1.0);
				break;
		default: break;
	}
}

