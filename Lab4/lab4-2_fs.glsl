#version 400
out vec4 o_fragment_color;

uniform sampler2D i_texture;
uniform bool i_display;
uniform uint i_frame_count;

uniform vec2 i_window_size;
uniform float i_global_time;
uniform vec4 i_mouse_state;

uniform vec3 i_position;
uniform vec3 i_up;
uniform vec3 i_right;
uniform vec3 i_dir;
uniform vec3 i_light_position;
uniform vec3 i_light_color;
uniform float i_focal_dist;

#define NUM_SPHERES 5
#define MAX_DEPTH 10
#define MAX_SAMPLES 1

struct Ray { vec3 origin, dir; float weight;};

//-------------------------------------------------------------------------//
// Keep a global stack of rays for recursion

Ray ray_stack[MAX_DEPTH];
int ray_stack_size = 0;

void push( Ray ray )
{
  if (ray_stack_size < MAX_DEPTH)
  {
    ray_stack[ray_stack_size] = ray;
    ray_stack_size++;
  }
  //else stack overflow -- silently ignore
}

Ray pop() // This is included for completeness, but actually isn't needed.
{
  if (ray_stack_size > 0)
  {
    ray_stack_size--;
    return ray_stack[ray_stack_size];
  }
  // else stack underflow -- silently ignore
}

//-------------------------------------------------------------------------//

struct Material{
  vec3 color_emission;
  vec3 color_diffuse;
  vec3 color_glossy;
  float roughness;
  float reflection;
  float transmission;
  float ior;  
};

struct Intersection
{
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

struct Scene {
  Sphere spheres[NUM_SPHERES];
  Plane ground_plane[1];
	vec3 sun_position;
  float sun_brightness;
};







Scene scene;


void init( )
{
	// Hard-coded single point light source
	scene.sun_brightness = 1;
	scene.sun_position = vec3(6e3,  1e4, 1e4);
	
	// Initialise 5 spheres and a ground plane

    scene.spheres[1].center = vec3(0.8, 0.3, 0.8);
	scene.spheres[1].radius = 0.3;
	scene.spheres[1].material.color_diffuse = 0.5 * vec3( 0.0, 1.0, 0.0 );
	scene.spheres[1].material.color_glossy = vec3( 1 );
	scene.spheres[1].material.roughness = 10000;
	scene.spheres[1].material.color_emission = vec3( 0 );
	scene.spheres[1].material.reflection = 0.1;
	scene.spheres[1].material.transmission = 0.8; // glass green ball
	scene.spheres[1].material.ior = 1.4;
    
	scene.spheres[0].center = vec3(0, 0.3, 0.5) ; 
	scene.spheres[0].radius = 0.3;
	scene.spheres[0].material.color_diffuse = vec3( 0.3, 0.1, 0.1 );
	scene.spheres[0].material.color_glossy = vec3( 1 );
	scene.spheres[0].material.color_emission = vec3( 0 );
	scene.spheres[0].material.roughness = 100;
	scene.spheres[0].material.reflection = 0.5; //reflective red ball
	scene.spheres[0].material.transmission = 0;
	scene.spheres[0].material.ior = 1;
  
	

	scene.spheres[2].center = vec3(0.55, 0.1, 0.2) ;
	scene.spheres[2].radius = 0.1;
	scene.spheres[2].material.color_diffuse = 0.8 * vec3( 1.0, 0.0, 0.0 );
	scene.spheres[2].material.color_glossy = vec3( 1 );
	scene.spheres[2].material.roughness = 1000;
	scene.spheres[2].material.color_emission = vec3( 1, 0, 0 ); // glowing red ball
	scene.spheres[2].material.reflection = 0.0;
	scene.spheres[2].material.transmission = 0;
	scene.spheres[2].material.ior = 1;

	scene.spheres[3].center = vec3(0.7, 0.8, -0.5) ;
	scene.spheres[3].radius = 0.8;
	scene.spheres[3].material.color_diffuse = 0.5 * vec3( 0.2, 0.2, 0.15 );
	scene.spheres[3].material.color_glossy = vec3( 1 );
	scene.spheres[3].material.roughness = 10;
	scene.spheres[3].material.color_emission = vec3( 0 );
	scene.spheres[3].material.reflection = 0.0;
	scene.spheres[3].material.transmission = 0;
	scene.spheres[3].material.ior = 1;

	scene.spheres[4].center = vec3(-0.65, 0.6, -1) ;
	scene.spheres[4].radius = 0.6;
	scene.spheres[4].material.color_diffuse = 0.5 * vec3( 0.5, 0.4, 0.25 );
	scene.spheres[4].material.color_glossy = vec3( 1 );
	scene.spheres[4].material.roughness = 5000;
	scene.spheres[4].material.color_emission = vec3( 0 );
	scene.spheres[4].material.reflection = 0.0;
	scene.spheres[4].material.transmission = 0;
	scene.spheres[4].material.ior = 1;

	scene.ground_plane[0].normal = vec3(0,1,0);
	scene.ground_plane[0].offset = 0;
	scene.ground_plane[0].material.color_diffuse = 1.0 * vec3( 0.6 );
	scene.ground_plane[0].material.color_glossy = vec3( 0 );
	scene.ground_plane[0].material.roughness = 0;
	scene.ground_plane[0].material.color_emission = vec3( 0 );
	scene.ground_plane[0].material.reflection = 0.0;
	scene.ground_plane[0].material.transmission = 0;
	scene.ground_plane[0].material.ior = 1;  
}


// This function computes a nice-looking sky sphere, with a sun.
vec3 simple_sky(vec3 direction)
{
	float emission_sky = 1*scene.sun_brightness;
	float emission_sun = 10*scene.sun_brightness*scene.sun_brightness;
  vec3 sky_color = vec3(0.35, 0.65, 0.85);
  vec3 haze_color = vec3(0.8, 0.85, 0.9);
  vec3 light_color = clamp(i_light_color,0,1);

  float sun_spread = 2500.0;
  float haze_spread = 1.3;
  float elevation = acos(direction.y);
    
  float angle = abs(dot(direction, normalize(i_light_position)));
  float response_sun = pow(angle, sun_spread);
  float response_haze = pow(elevation, haze_spread);

  vec3 sun_component = mix(emission_sky*sky_color, emission_sun*light_color,response_sun);
  vec3 haze_component = mix(vec3(0),  emission_sky*haze_color,response_haze);

  return (sun_component+haze_component);
}


// Ray-sphere intersection
float intersect(Ray ray, Sphere s) 
{
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

// Ray-plane intersection
float intersect(Ray ray, Plane p) 
{
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

// Check for intersection of a ray and all objects in the scene
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

vec3 raytrace() 
{
    vec3 color = vec3(0);

    // Iterate over all rays on the stack
    for (int ray_stack_pos = 0; ray_stack_pos < ray_stack_size; ++ray_stack_pos)
    {
        Ray ray = ray_stack[ray_stack_pos];
        Intersection isec = intersect(ray);

        vec3 nl = isec.normal * sign(-dot(isec.normal, ray.dir)); 
        vec3 light_direction = normalize(i_light_position - isec.point);

        vec3 this_color = vec3(0);

      // Transmission (refraction) handling
        if (isec.material.transmission > 0)
        {
            // Calculate the IOR ratio (we assume the outside IOR is 1.0, the air index)
            float ior_in = isec.material.ior;  // IOR of the material inside the object
            float ior_out = 1.0; // IOR of the outside medium (air/vacuum)

            // Check if the ray is coming from the inside or outside
            float cosi = dot(ray.dir, isec.normal); // Angle of incidence

            // If the ray is inside the object, invert the normal for refractive computation
            if (cosi > 0) {
                isec.normal = -isec.normal;  // Invert the normal if we are inside
            }

            // Compute the refraction direction using GLSL refract function
            float eta = (cosi > 0) ? (ior_in / ior_out) : (ior_out / ior_in);
            vec3 refracted_dir = refract(ray.dir, isec.normal, eta);


           
                // Offset the origin of the refracted ray slightly to avoid self-intersection
                vec3 refracted_origin = isec.point + 1e-4 * refracted_dir;

                // Create the refracted ray
                Ray refracted_ray;
                refracted_ray.origin = refracted_origin;
                refracted_ray.dir = normalize(refracted_dir);  // Ensure the direction is normalized
                refracted_ray.weight = ray.weight * isec.material.transmission;  // Transfer weight to refracted ray

                // Add the refracted color to the accumulated color
                color += refracted_ray.weight * isec.material.color_diffuse;

                // Reduce the weight of the original ray based on transmission
               ray.weight *= (1.0 - isec.material.transmission);

                // If the refracted ray has a significant weight, push it to the ray stack
                if (refracted_ray.weight > 0.01) {
                    push(refracted_ray); 
                }
            
        }

        // Reflection handling (unchanged)
        if (isec.material.reflection > 0) {
            // Compute reflected direction
            vec3 reflected_dir = reflect(ray.dir, isec.normal);

            // Slightly offset origin to avoid self-intersection
            vec3 reflected_origin = isec.point + 1e-4 * reflected_dir;

            Ray reflected_ray;
            reflected_ray.origin = reflected_origin;
            reflected_ray.dir = normalize(reflected_dir);
            reflected_ray.weight = ray.weight * isec.material.reflection;

            // Add the reflected color to the total color, not replace
            color += reflected_ray.weight * isec.material.color_diffuse;

            // Reduce the weight of the main ray based on the reflection
            ray.weight *= (1.0 - isec.material.reflection);

            if (reflected_ray.weight > 0.095) {
                push(reflected_ray); // Push the reflected ray onto the stack
            }
        }

        // Shadow ray and diffuse contribution (unchanged)
        {
            Ray shadow_ray;
            shadow_ray.origin = isec.point + 1e-4 * light_direction; // Offset to avoid shadow acne
            shadow_ray.dir = light_direction;

            bool in_shadow = false;

            // Check against spheres
            for (int i = 0; i < NUM_SPHERES; ++i) {
                float t = intersect(shadow_ray, scene.spheres[i]);
                if (t > 0.0 && scene.spheres[i].material.color_emission == vec3(0.0)) {
                    in_shadow = true;
                    break;
                }
            }

            // Check against the ground plane
        if (!in_shadow) {
            float t = intersect(shadow_ray, scene.ground_plane[0]);
            if (t > 0.0) {
                in_shadow = true;
            }
        }

        if (!in_shadow) {
            float lambertian = max(0.0, dot(nl, light_direction));
            vec3 diffuse = isec.material.color_diffuse * lambertian;
            this_color += diffuse;
        }

        // Add emission
        this_color += isec.material.color_emission;

        // Accumulate this intersection's contribution to the final color
        color += ray.weight * this_color;
    }
        
    }

    return color;
}




void main() {

  vec2 tex_coords = gl_FragCoord.xy / i_window_size.xy;

  if(i_display)    
  {
    o_fragment_color = texture(i_texture,tex_coords);
  }    
  else
  {

    init();

    //basis for defining the image plane
	vec3 cx = i_right;
	vec3 cy = i_up;   
	vec3 cz = i_dir;  

    Ray ray;
	ray.origin = i_position;
	///\todo YOUR CODE GOES HERE: Compute the correct ray direction using gl_fragCoord and the camera vectors above.
	//ray.dir = gl_FragCoord.xy;

   // Compute the ray direction in camera space
	ray.dir = normalize(
    i_focal_dist * cz +       // Contribution of the main direction
    gl_FragCoord.x * cx +     // Horizontal contribution (left/right)
    gl_FragCoord.y * cy       // Vertical contribution (up/down)
	);

    ray.weight = 1.0;

    // Push the ray noto the ray stack
    push( ray );
    
    vec3 color = raytrace(); 
    

    // gamma corrected output color, and blended over several frames (good for path tracer)
    o_fragment_color = (texture(i_texture,tex_coords)*i_frame_count + vec4( pow ( clamp(color.xyz/MAX_SAMPLES, 0., 1.), vec3(1./2.2)), 1.))/float(1+ i_frame_count); 

  }
}
