import os
import re
import json
from PIL import Image

def merge_sprites(target_dir, character_name):
    dir_path = target_dir
    anim_dir_path = os.path.join(os.path.dirname(dir_path), "Animation")
    
    if not os.path.exists(anim_dir_path):
        os.makedirs(anim_dir_path)
    
    # [수정됨] 띄어쓰기, 숫자 포함, 언더바 유무 상관없이 모두 잡아내는 강력한 규칙
    # 예: "Max Charge Shot_0.png", "FiringEffect0.png", "Attack 1.png" 모두 인식 가능
    pattern = re.compile(r"^(.+?)[ _]*(\d+)\.png$")
    
    groups = {}
    
    for filename in os.listdir(dir_path):
        # Skip if it is already a sheet
        if "Sheet" in filename:
            continue
            
        match = pattern.match(filename)
        if match:
            prefix = match.group(1).strip() # 앞뒤 공백 깔끔하게 제거
            
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
        "JumpAttack": False,
        "Max Charge Shot": False # 차지샷도 반복 안함 처리 추가
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
            
        # 파일명 저장 시 띄어쓰기를 언더바로 변경해서 저장(안전한 파일명을 위해)
        safe_prefix = prefix.replace(" ", "_")
        out_path = os.path.join(dir_path, f"{character_name}_{safe_prefix}_Sheet.png")
        new_im.save(out_path)
        
        # Determine bLoop
        bLoop = loop_exceptions.get(prefix, True)
        
        # Create JSON data
        json_data = {
            "texturePath": f"{character_name}_{safe_prefix}_Sheet",
            "bLoop": bLoop,
            "frames": frames
        }
        
        # Save JSON file
        json_path = os.path.join(anim_dir_path, f"{safe_prefix}.json")
        with open(json_path, 'w', encoding='utf-8') as f:
            json.dump(json_data, f, indent=2)
            
        print(f"✅ 병합 완료: '{prefix}' 그룹 -> {len(images)}장 묶음")
        
if __name__ == "__main__":
    print("=== 스프라이트 시트 자동 병합 툴 ===")
    
    raw_dir = input("1. 이미지가 있는 폴더의 경로를 입력하세요: ")
    input_dir = raw_dir.strip().strip("\"'")
    
    input_name = input("2. 캐릭터 이름(예: Player, Boss_Cutman 등)을 입력하세요: ").strip()
    
    if os.path.exists(input_dir):
        merge_sprites(input_dir, input_name)
        print("\n작업이 모두 완료되었습니다!")
    else:
        print(f"\n[오류] 경로를 찾을 수 없습니다: {input_dir}")
        print("입력하신 경로를 다시 확인해주세요.")