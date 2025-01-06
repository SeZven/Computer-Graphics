// Based on Lab 4.3

#version 400
out vec4 o_fragment_color; // The final colour

// User input:
uniform vec2 i_window_size;
uniform float i_global_time;
uniform vec4 i_mouse_state;

// Camera parameters:
uniform vec3 i_position;
uniform vec3 i_up;
uniform vec3 i_right;
uniform vec3 i_dir;
uniform float i_focal_dist;

#define NUM_SPHERES 5
#define MAX_SAMPLES 20 // Lab 4.4.1
#define MAX_DEPTH 5 // Lab 4.4.1
//#define NUM_TRIANGLES 4 // Lab 4.3

struct Ray {
	vec3 origin;
	vec3 dir;
};

struct Material {
	vec3 color_emission;
	vec3 color_diffuse;
	vec3 color_glossy;
	float roughness;
	float reflection;
	float transmission;
	float ior;
};

struct Intersection {
	vec3 point;
	vec3 normal;
	Material material;
};

struct Sphere {
	float radius;
	vec3 center;
	Material material;
};

struct Plane {
	float offset;
	vec3 normal;
	Material material;
};

// Lab 4.3
struct Triangle {
	vec3 v0, v1, v2;
	Material material;
};

struct Scene {
	Sphere spheres[NUM_SPHERES];
	Plane ground_plane[1];
	//Triangle triangles[NUM_TRIANGLES]; // Lab 4.3
	vec3 sun_position;
	float sun_brightness;
};

Scene scene;

// Lab 4.4.6
float seed = 0.0;
float rand(void) {
	return fract(sin(seed += 0.14) * 43758.5453123);
}

void init(float sun_bright) {
	// Hard-coded single point light source
	scene.sun_brightness = 1;
	scene.sun_position = vec3(6e3, 1e4, 1e4);

	// Initialize 5 spheres and a ground plane
	scene.spheres[0].center = vec3(0, 0.3, 0.5);
	scene.spheres[0].radius = 0.3;
	scene.spheres[0].material.color_diffuse = vec3(0.3, 0.1, 0.1);
	scene.spheres[0].material.color_glossy = vec3(1);
	scene.spheres[0].material.color_emission = vec3(0);
	scene.spheres[0].material.roughness = 100;
	scene.spheres[0].material.reflection = 0.5; // reflective red ball
	scene.spheres[0].material.transmission = 0;
	scene.spheres[0].material.ior = 1;

	scene.spheres[1].center = vec3(0.8, 0.3, 0.8);
	scene.spheres[1].radius = 0.3;
	scene.spheres[1].material.color_diffuse = 0.5 * vec3(0.0, 1.0, 0.0);
	scene.spheres[1].material.color_glossy = vec3(1);
	scene.spheres[1].material.roughness = 10000;
	scene.spheres[1].material.color_emission = vec3(0);
	scene.spheres[1].material.reflection = 0.1;
	scene.spheres[1].material.transmission = 0.8; // glass green ball
	scene.spheres[1].material.ior = 1.4;

	scene.spheres[2].center = vec3(0.55, 0.1, 0.2);
	scene.spheres[2].radius = 0.1;
	scene.spheres[2].material.color_diffuse = 0.8 * vec3(1.0, 0.0, 0.0);
	scene.spheres[2].material.color_glossy = vec3(1);
	scene.spheres[2].material.roughness = 1000;
	scene.spheres[2].material.color_emission = vec3(1, 0, 0); // glowing red ball
	scene.spheres[2].material.reflection = 0.0;
	scene.spheres[2].material.transmission = 0;
	scene.spheres[2].material.ior = 1;

	scene.spheres[3].center = vec3(0.7, 0.8, -0.5);
	scene.spheres[3].radius = 0.8;
	scene.spheres[3].material.color_diffuse = 0.5 * vec3(0.2, 0.2, 0.15);
	scene.spheres[3].material.color_glossy = vec3(1);
	scene.spheres[3].material.roughness = 10;
	scene.spheres[3].material.color_emission = vec3(0);
	scene.spheres[3].material.reflection = 0.0;
	scene.spheres[3].material.transmission = 0;
	scene.spheres[3].material.ior = 1;

	scene.spheres[4].center = vec3(-0.65, 0.6, -1);
	scene.spheres[4].radius = 0.6;
	scene.spheres[4].material.color_diffuse = 0.5 * vec3(0.5, 0.4, 0.25);
	scene.spheres[4].material.color_glossy = vec3(1);
	scene.spheres[4].material.roughness = 5000;
	scene.spheres[4].material.color_emission = vec3(0);
	scene.spheres[4].material.reflection = 0.0;
	scene.spheres[4].material.transmission = 0;
	scene.spheres[4].material.ior = 1;

	scene.ground_plane[0].normal = vec3(0, 1, 0);
	scene.ground_plane[0].offset = 0;
	scene.ground_plane[0].material.color_diffuse = 1.0 * vec3(0.6);
	scene.ground_plane[0].material.color_glossy = vec3(0);
	scene.ground_plane[0].material.roughness = 0;
	scene.ground_plane[0].material.color_emission = vec3(0);
	scene.ground_plane[0].material.reflection = 0.0;
	scene.ground_plane[0].material.transmission = 0;
	scene.ground_plane[0].material.ior = 1;

	/*
	// Lab 4.3 Initialize triangles (tetrahedron)
	scene.triangles[0].v0 = vec3(0.0, 0.0, 0.0);
	scene.triangles[0].v1 = vec3(1.0, 0.0, 0.0);
	scene.triangles[0].v2 = vec3(0.5, 1.0, 0.0);
	scene.triangles[0].material.color_diffuse = vec3(0.8, 0.2, 0.2);
	scene.triangles[0].material.color_glossy = vec3(1);
	scene.triangles[0].material.roughness = 100;
	scene.triangles[0].material.color_emission = vec3(0);
	scene.triangles[0].material.reflection = 0.5;
	scene.triangles[0].material.transmission = 0;
	scene.triangles[0].material.ior = 1;

	scene.triangles[1].v0 = vec3(0.0, 0.0, 0.0);
	scene.triangles[1].v1 = vec3(0.5, 1.0, 0.0);
	scene.triangles[1].v2 = vec3(0.5, 0.5, 1.0);
	scene.triangles[1].material = scene.triangles[0].material;

	scene.triangles[2].v0 = vec3(0.0, 0.0, 0.0);
	scene.triangles[2].v1 = vec3(1.0, 0.0, 0.0);
	scene.triangles[2].v2 = vec3(0.5, 0.5, 1.0);
	scene.triangles[2].material = scene.triangles[0].material;

	scene.triangles[3].v0 = vec3(1.0, 0.0, 0.0);
	scene.triangles[3].v1 = vec3(0.5, 1.0, 0.0);
	scene.triangles[3].v2 = vec3(0.5, 0.5, 1.0);
	scene.triangles[3].material = scene.triangles[0].material;
	*/
}

// This function computes a nice-looking sky sphere, with a sun.
vec3 simple_sky(vec3 direction) {
	float emission_sky = 1 * scene.sun_brightness;
	float emission_sun = 10 * scene.sun_brightness * scene.sun_brightness;
	vec3 sky_color = vec3(0.35, 0.65, 0.85);
	vec3 haze_color = vec3(0.8, 0.85, 0.9);
	vec3 light_color = vec3(1);

	float sun_spread = 2500.0;
	float haze_spread = 1.3;
	float elevation = acos(direction.y);

	float angle = abs(dot(direction, normalize(scene.sun_position)));
	float response_sun = pow(angle, sun_spread);
	float response_haze = pow(elevation, haze_spread);

	vec3 sun_component = mix(emission_sky * sky_color, emission_sun * light_color, response_sun);
	vec3 haze_component = mix(vec3(0), emission_sky * haze_color, response_haze);

	return (sun_component + haze_component);
}

float intersect(Ray ray, Sphere s) {
	vec3 oc = ray.origin - s.center; // Vector from ray origin to sphere center
	float a = dot(ray.dir, ray.dir); // Coefficient of t^2 (ray.dir is normalized, so a = 1)
	float b = 2.0 * dot(oc, ray.dir); // Coefficient of t
	float c = dot(oc, oc) - s.radius * s.radius; // Constant term

	float discriminant = b * b - 4.0 * a * c; // Discriminant of the quadratic equation

	if (discriminant < 0.0) {
		return 0.0; // No intersection
	}

	// Compute the two possible solutions for t
	float sqrtDiscriminant = sqrt(discriminant);
	float t1 = (-b - sqrtDiscriminant) / (2.0 * a); // Closest intersection
	float t2 = (-b + sqrtDiscriminant) / (2.0 * a); // Farther intersection

	if (t1 > 0.0) {
		return t1; // Return closest intersection if it's in front of the ray
	} else if (t2 > 0.0) {
		return t2; // If t1 is behind the ray, return t2 if it's in front
	}

	return 0.0; // Both intersections are behind the ray
}

float intersect(Ray ray, Plane p) {
	// Dot product of plane normal and ray direction
	float denom = dot(p.normal, ray.dir);

	// Check if the ray is parallel to the plane
	if (abs(denom) < 1e-6) {
		return 0.0; // No intersection
	}

	// Calculate t using the plane offset and the ray origin
	float t = (p.offset - dot(p.normal, ray.origin)) / denom;

	// Return t only if it's in front of the ray
	return (t > 0.0) ? t : 0.0;
}

// Lab 4.3
float intersect(Ray ray, Triangle tri) {
  vec3 edge1 = tri.v1 - tri.v0;
  vec3 edge2 = tri.v2 - tri.v0;
  vec3 h = cross(ray.dir, edge2);
  float a = dot(edge1, h);
  if (abs(a) < 1e-6) return 0.0; // Ray is parallel to the triangle

  float f = 1.0 / a;
  vec3 s = ray.origin - tri.v0;
  float u = f * dot(s, h);
  if (u < 0.0 || u > 1.0) return 0.0;

  vec3 q = cross(s, edge1);
  float v = f * dot(ray.dir, q);
  if (v < 0.0 || u + v > 1.0) return 0.0;

  float t = f * dot(edge2, q);
  return (t > 1e-6) ? t : 0.0; // Return t if it's in front of the ray
}

Intersection intersect(Ray ray) {
	Intersection I;
	float t = 1e32; // closest hit so far along this ray
	int id = -1;

	// Check for intersection with spheres
	for (int i = 0; i < NUM_SPHERES; ++i) {
		float d = intersect(ray, scene.spheres[i]);
		if (d > 0 && d <= t) { // if sphere `i` is closer than `t`, update `t` and `id`
			t = d;
			id = i;

			// Populate I with all relevant data for the sphere
			I.point = ray.origin + t * ray.dir; // Intersection point
			I.normal = normalize(I.point - scene.spheres[i].center); // Normal at intersection
			I.material = scene.spheres[i].material; // Material of the sphere
		}
	}

	// Check for intersection with planes
	{
		float d = intersect(ray, scene.ground_plane[0]);
		if (d > 0 && d <= t) { // if the plane is closer than `t`, update `t`
			t = d;

			// Populate I with all relevant data for the plane
			I.point = ray.origin + t * ray.dir; // Intersection point
			I.normal = scene.ground_plane[0].normal; // Normal of the plane (assumed constant)

			// Adding a procedural checkerboard texture:
			if (mod(floor(I.point.x) + floor(I.point.z), 2.0) == 0.0) {
				I.material.color_diffuse = scene.ground_plane[0].material.color_diffuse;
			} else {
				I.material.color_diffuse = vec3(1.0) - scene.ground_plane[0].material.color_diffuse;
			}

			I.material.color_glossy = scene.ground_plane[0].material.color_glossy;
			I.material.roughness = scene.ground_plane[0].material.roughness;
			I.material.color_emission = scene.ground_plane[0].material.color_emission;
			I.material.reflection = scene.ground_plane[0].material.reflection;
			I.material.transmission = scene.ground_plane[0].material.transmission;
			I.material.ior = scene.ground_plane[0].material.ior;
		}
	}
	/*
	// Lab 4.3 Check for intersection with triangles
	for (int i = 0; i < NUM_TRIANGLES; ++i) {
    float d = intersect(ray, scene.triangles[i]);
    if (d > 0 && d <= t) {
      t = d;
      I.point = ray.origin + t * ray.dir;
      I.normal = normalize(cross(scene.triangles[i].v1 - scene.triangles[i].v0, scene.triangles[i].v2 - scene.triangles[i].v0));
      I.material = scene.triangles[i].material;
    }
  }
	*/
	// If no sphere or plane hit, we hit the sky instead
	if (t > 1e20) {
		I.point = ray.dir * t;
		I.normal = -ray.dir; // Sky has no real surface; invert ray direction
		vec3 sky = simple_sky(ray.dir); // Pick color from sky function

		// Sky is all emission, no diffuse or glossy shading:
		I.material.color_diffuse = vec3(0.0);
		I.material.color_glossy = vec3(0.0);
		I.material.roughness = 1.0;
		I.material.color_emission = 0.3 * sky;
		I.material.reflection = 0.0;
		I.material.transmission = 0.0;
		I.material.ior = 1.0;
	}

	return I;
}

// Lab 4.4.7
vec3 random_direction(vec3 mean, float spread) {
	float r2 = spread * rand();
	float phi = 2.0 * 3.14159265358979323846 * rand();
	float sina = sqrt(r2);
	float cosa = sqrt(1.0 - r2);
	vec3 w = normalize(mean), u = normalize(cross(w.yzx, w)), v = cross(w, u);
	return (u * cos(phi) + v * sin(phi)) * sina + w * cosa;
}

vec3 raycast(Ray ray) 
{
  vec3 color = vec3(0); 

  Intersection isec = intersect(ray); //ray intersection with scene

  // Light position
  vec3 light_position = scene.sun_position;
  vec3 light_color = vec3(scene.sun_brightness); // Color and intensity

  // light direction
  vec3 light_dir = normalize(light_position - isec.point);
  
  // lambertian contribution
  float lambertian = max(0.0, dot(isec.normal, light_dir));

  // Lambertian BRDF 
  vec3 diffuse = isec.material.color_diffuse * lambertian;
  vec3 emission = isec.material.color_emission;

  color = diffuse + emission;

  return color;
}

// Lab 4.4.1
vec3 pathtrace(Ray ray) {
	vec3 coefficient = vec3(1); // Lab 4.4.2
	vec3 color = vec3(0); // Lab 4.4.2

	for (int depth = 0; depth < MAX_DEPTH; ++depth) { // Lab 4.4.1
		Intersection isec = intersect(ray); // Lab 4.4.3

		// If the ray hits an emissive surface, add emission * coefficient to the color
		if (length(isec.material.color_emission) > 0.0) { // Lab 4.4.4
			color += isec.material.color_emission * coefficient; // Lab 4.4.4
			break;
		}

		// Update the coefficient with the color of the surface
		coefficient *= isec.material.color_diffuse; // Lab 4.4.2

		// Lab 4.4.6
		// Scatter the ray
		float r = rand(); 
		if (r < isec.material.reflection) {
			// Reflective path
			ray.origin = isec.point;
			ray.dir = reflect(ray.dir, isec.normal);
		} else if (r < isec.material.reflection + isec.material.transmission) {
			// Refractive path
			ray.origin = isec.point;
			ray.dir = refract(ray.dir, isec.normal, isec.material.ior);
		} else {
			// Diffuse path
			ray.origin = isec.point;
			ray.dir = random_direction(isec.normal, 1.0); // Lab 4.4.7
		}
	}

	return color;
}

void main() {
	vec2 uv = gl_FragCoord.xy - 0.5 * i_window_size.xy;

	//init(i_light_position, 10.0 ); //i_mouse_state.y); 
	init(10.0);

	int num_samples = MAX_SAMPLES; // Lab 4.4.1
	vec3 color = vec3(0);

	// Crude zooming by pressing right mouse button
	float f_dist = i_focal_dist + i_focal_dist * i_mouse_state.w;

	// Basis for defining the image plane
	vec3 cx = i_right;
	vec3 cy = i_up;
	vec3 cz = i_dir;

	Ray ray;
	ray.origin = i_position;

	// Compute the ray direction in camera space
	for (int i = 0; i < num_samples; ++i) { // Lab 4.4.1
		seed = i_global_time + i_window_size.y * gl_FragCoord.x / i_window_size.x + gl_FragCoord.y / i_window_size.y; // Lab 4.4.6
		ray.dir = normalize(
			f_dist * cz +       // Contribution of the main direction
			(uv.x / i_window_size.x) * cx +     // Horizontal contribution (left/right)
			(uv.y / i_window_size.y) * cy       // Vertical contribution (up/down)
		);
		color += pathtrace(ray); // Lab 4.4.1
	}

	//linear blend
	// o_fragment_color =  vec4((color),1);

	// Gamma correction
	o_fragment_color = vec4(pow(clamp(color / num_samples, 0.0, 1.0), vec3(1.0 / 2.2)), 1.0);

	///\todo REMOVE THIS LINE after you have the triangles set up
	//o_fragment_color = vec4(1,0.9,0.8,1); 
}


