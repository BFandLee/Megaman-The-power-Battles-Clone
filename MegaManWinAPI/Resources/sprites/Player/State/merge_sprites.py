import os
from PIL import Image

def merge_sprites():
    dir_path = r"D:\GitHub\Megaman-The-power-Battles-Clone\MegaManWinAPI\Resources\sprites\Player\State"
    
    # We know there are Idle_1 to Idle_9
    images = []
    for i in range(1, 10):
        img_path = os.path.join(dir_path, f"Idle_{i}.png")
        if os.path.exists(img_path):
            images.append(Image.open(img_path))
        else:
            print(f"Warning: {img_path} not found.")
            
    if not images:
        print("No images found.")
        return
        
    widths, heights = zip(*(i.size for i in images))
    
    total_width = sum(widths)
    max_height = max(heights)
    
    # Create new image with transparent background
    new_im = Image.new('RGBA', (total_width, max_height), (0, 0, 0, 0))
    
    x_offset = 0
    for im in images:
        new_im.paste(im, (x_offset, 0))
        x_offset += im.size[0]
        
    out_path = os.path.join(dir_path, "Player_Idle_Sheet.png")
    new_im.save(out_path)
    print(f"Successfully saved merged sprite sheet to {out_path}")
    print(f"Total Width: {total_width}, Max Height: {max_height}")
    print(f"Individual frame size: {widths[0]}x{heights[0]}")

if __name__ == "__main__":
    merge_sprites()
