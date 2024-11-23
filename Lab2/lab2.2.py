# AI used
import numpy as np
import matplotlib.pyplot as plt

# Lambertian BRDF function
def lambertian_brdf(in_direction, out_direction, normal):
    # albedo = 1.0
    return 1 / np.pi

def main():
    # Fixed normal and outgoing direction (along z-axis)
    normal = np.array([0.0, 0.0, 1.0])
    out_direction = np.array([0.0, 0.0, 1.0])
    
    angles = []  # Angles from -90 to 90
    brdf_values = []  # BRDF values for each angle

    # Loop over angles from -90 to 90 degrees
    for angle in range(-90, 91):  # From -90 to 90 degrees
        # Calculate the incoming direction (in_direction) in the xy-plane
        rad = np.radians(angle)  # Convert angle to radians
        in_direction = np.array([np.cos(rad), np.sin(rad), 0.0])
        
        # Normalize the incoming direction (although it's already a unit vector)
        in_direction /= np.linalg.norm(in_direction)
        
        # Call BRDF function and store the result
        brdf = lambertian_brdf(in_direction, out_direction, normal)
        
        # Store the angle and corresponding BRDF value
        angles.append(angle)
        brdf_values.append(brdf)

    # Plotting the BRDF values
    plt.plot(angles, brdf_values, label="Lambertian BRDF")
    plt.xlabel("Angle (degrees)")
    plt.ylabel("BRDF Value")
    plt.title("Lambertian BRDF vs Angle")
    plt.grid(True)
    plt.legend()
    plt.show()

if __name__ == "__main__":
    main()
