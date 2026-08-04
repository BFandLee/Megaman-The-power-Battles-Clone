import os
import re
from PIL import Image

def merge_sprites():
    dir_path = r"d:\Megaman-The-power-Battles-Clone\MegaManWinAPI\Resources\sprites\Player\State"
    
    # regex to match Prefix_Number.png
    pattern = re.compile(r"^([a-zA-Z_]+)_(\d+)\.png$")
    
    groups = {}
    
    for filename in os.listdir(dir_path):
        # Skip if it is already a sheet
        if "Sheet" in filename:
            continue
            
        match = pattern.match(filename)
        if match:
            prefix = match.group(1)
            # Normalize Jump_Attack to JumpAttack just in case
            if prefix == "Jump_Attack":
                prefix = "JumpAttack"
            
            num = int(match.group(2))
            
            if prefix not in groups:
                groups[prefix] = []
            groups[prefix].append((num, filename))
            
    for prefix, files in groups.items():
        # Sort files by their number
        files.sort(key=lambda x: x[0])
        images = []
        for num, filename in files:
            img_path = os.path.join(dir_path, filename)
            images.append(Image.open(img_path))
            
        if not images:
            continue
            
        widths, heights = zip(*(i.size for i in images))
        total_width = sum(widths)
        max_height = max(heights)
        
        new_im = Image.new('RGBA', (total_width, max_height), (0, 0, 0, 0))
        
        x_offset = 0
        for im in images:
            new_im.paste(im, (x_offset, 0))
            x_offset += im.size[0]
            
        out_path = os.path.join(dir_path, f"Player_{prefix}_Sheet.png")
        new_im.save(out_path)
        print(f"Successfully saved merged sprite sheet for '{prefix}': {out_path}")
        print(f"  Frames: {len(images)}, Total Width: {total_width}, Max Height: {max_height}")

if __name__ == "__main__":
    merge_sprites()
