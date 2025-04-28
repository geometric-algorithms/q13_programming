import matplotlib.pyplot as plt

def read_voronoi_file(filename):
    with open(filename, "r") as f:
        lines = f.readlines()
    
    points = []
    edges = []

    i = 0
    while i < len(lines):
        parts = lines[i].split()
        if parts[0] == "POINTS":
            num_points = int(parts[1])
            i += 1
            for _ in range(num_points):
                x, y = map(float, lines[i].split())
                points.append((x, y))
                i += 1

        elif parts[0] == "EDGES":
            num_edges = int(parts[1])
            i += 1
            for _ in range(num_edges):
                x1, y1, x2, y2 = map(float, lines[i].split())
                edges.append(((x1, y1), (x2, y2)))
                i += 1
        else:
            i += 1

    return points, edges


def read_delaunyfile(filename):
    with open(filename, "r") as f:
        lines = f.readlines()
    
    edges = []

    i = 0
    while i < len(lines):
        parts = lines[i].split()
        
        if parts[0] == "EDGES":
            num_edges = int(parts[1])
            i += 1
            for _ in range(num_edges):
                x1, y1, x2, y2 = map(float, lines[i].split())
                edges.append(((x1, y1), (x2, y2)))
                i += 1
        else:
            i += 1

    return edges


# Read Voronoi data
points, edges = read_voronoi_file("voronoi_output.txt")
d_e = read_delaunyfile("delauny_edges.txt")
# Extract coordinates
px, py = zip(*points) if points else ([], [])

# Determine axis limits with a margin
all_x = list(px)
all_y = list(py)

x_min, x_max = min(all_x), max(all_x)
y_min, y_max = min(all_y), max(all_y)

x_margin = (x_max - x_min) * 0.1
y_margin = (y_max - y_min) * 0.1

plt.xlim(x_min - x_margin, x_max + x_margin)
plt.ylim(y_min - y_margin, y_max + y_margin)

# Plot input points
plt.scatter(px, py, c="black", label="Input Points")

# Plot Voronoi edges
for (p1, p2) in edges:
    plt.plot([p1[0], p2[0]], [p1[1], p2[1]], "r-", alpha=0.6, linewidth=1)

# Plot DELAUNY edges
for (p1, p2) in d_e:
    plt.plot([p1[0], p2[0]], [p1[1], p2[1]], "g-", alpha=0.6, linewidth=1)


plt.legend()
plt.title("Delaunay Edges with Voronoi Edges")
plt.xlabel("X")
plt.ylabel("Y")
plt.grid(True)
plt.savefig("delaunay.png", dpi=300)
# plt.show()
