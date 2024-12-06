import numpy as np
import matplotlib.pyplot as plt

# Blinn-Phong BRDF function
def blinn_phong_brdf(in_direction, out_direction, normal, kL, kG, s, rhoL, rhoG):
    # Calculate half vector
    h = (in_direction + out_direction) / np.linalg.norm(in_direction + out_direction)
    
    # Lambertian part
    lambertian = rhoL / np.pi

    # Specular part (Blinn-Phong)
    cosNH = max(np.dot(normal, h), 0.0)
    specular = (rhoG / (8.0 + s)) * (cosNH ** s)

    return kL * lambertian + kG * specular

# Parameters
kL = 0.9
kG = 0.1
s = 100
rhoL = 1.0
rhoG = 1.0

# Loop over angles
angles = np.arange(-90, 91, 1)
brdf_values = []

# Set normal and view direction
normal = np.array([0.0, 0.0, 1.0])
view_direction = np.array([0.0, 0.0, 1.0])

for angle in angles:
    in_direction = np.array([np.cos(np.radians(angle)), np.sin(np.radians(angle)), 0.0])
    
    brdf = blinn_phong_brdf(in_direction, view_direction, normal, kL, kG, s, rhoL, rhoG)
    brdf_values.append(brdf)

# Plotting the BRDF
plt.plot(angles, brdf_values)
plt.xlabel("Angle (degrees)")
plt.ylabel("BRDF Value")
plt.title("Blinn-Phong BRDF vs Angle")
plt.grid(True)
plt.show()
