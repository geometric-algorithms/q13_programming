import matplotlib.pyplot as plt
import imageio
import os

# Change these if needed
OUTPUT_FILE = "output.txt"
FRAME_DIR = "frames"
GIF_FILE = "voronoi_animation.gif"

def parse_output(filename):
    with open(filename) as f:
        lines = f.read().splitlines()

    frames = []
    i = 0
    while i < len(lines):
        if lines[i].isdigit():
            point_count = int(lines[i])
            i += 1
            edges = []
            while i < len(lines) and not lines[i].isdigit():
                parts = list(map(float, lines[i].split()))
                if len(parts) == 4:
                    edges.append(tuple(parts))
                i += 1
            frames.append(edges)
        else:
            i += 1
    return frames

def draw_frame(edges, idx):
    plt.figure(figsize=(6, 6))
    ax = plt.gca()
    ax.set_xlim(-100, 100)
    ax.set_ylim(-100, 100)
    ax.set_aspect('equal')
    for (x1, y1, x2, y2) in edges:
        plt.plot([x1, x2], [y1, y2], 'b-')
    plt.title(f"After {idx + 1} insertions")
    plt.savefig(f"{FRAME_DIR}/frame_{idx:03}.png")
    plt.close()

def create_gif():
    images = []
    filenames = sorted([f for f in os.listdir(FRAME_DIR) if f.endswith('.png')])
    for filename in filenames:
        images.append(imageio.v2.imread(os.path.join(FRAME_DIR, filename)))
    imageio.mimsave(GIF_FILE, images, duration=0.5)
    print(f"GIF saved to {GIF_FILE}")

def main():
    if not os.path.exists(FRAME_DIR):
        os.makedirs(FRAME_DIR)

    frames = parse_output(OUTPUT_FILE)
    for idx, edges in enumerate(frames):
        draw_frame(edges, idx)

    create_gif()

if __name__ == "__main__":
    main()
