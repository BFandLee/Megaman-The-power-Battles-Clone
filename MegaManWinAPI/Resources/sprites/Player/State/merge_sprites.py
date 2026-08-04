import os
import re
import json
from PIL import Image

def merge_sprites():
    dir_path = r"d:\Megaman-The-power-Battles-Clone\MegaManWinAPI\Resources\sprites\Player\State"
    anim_dir_path = os.path.join(os.path.dirname(dir_path), "Animation")
    
    if not os.path.exists(anim_dir_path):
        os.makedirs(anim_dir_path)
    
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
            
    # 반복(Loop) 처리를 안 할 예외 상태 리스트
    loop_exceptions = {
        "Dead": False,
        "Spone": False,
        "Jump": False,
        "JumpAttack": False
    }
            
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
        
        frames = []
        x_offset = 0
        for im in images:
            w, h = im.size
            new_im.paste(im, (x_offset, 0))
            
            frame_data = {
                "startPos": { "x": float(x_offset), "y": 0.0 },
                "size": { "x": float(w), "y": float(h) },
                "offset": { "x": w / 2.0, "y": float(h) },
                "duration": 0.2
            }
            frames.append(frame_data)
            
            x_offset += w
            
        out_path = os.path.join(dir_path, f"Player_{prefix}_Sheet.png")
        new_im.save(out_path)
        
        # Determine bLoop
        bLoop = loop_exceptions.get(prefix, True)
        
        # Create JSON data
        json_data = {
            "texturePath": f"Player_{prefix}_Sheet",
            "bLoop": bLoop,
            "frames": frames
        }
        
        # Save JSON file
        json_path = os.path.join(anim_dir_path, f"{prefix}.json")
        with open(json_path, 'w', encoding='utf-8') as f:
            json.dump(json_data, f, indent=2)
            
        print(f"Successfully saved merged sprite sheet for '{prefix}': {out_path}")
        print(f"  Frames: {len(images)}, Total Width: {total_width}, Max Height: {max_height}")
        print(f"  Generated JSON: {json_path}")

if __name__ == "__main__":
    merge_sprites()
