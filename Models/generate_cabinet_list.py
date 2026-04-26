import os
import json

def generate_cabinet_list():
    cabinets_dir = "Cabinets"
    output_file = os.path.join(cabinets_dir, "cabinet_list.json")
    
    if not os.path.exists(cabinets_dir):
        print(f"Error: {cabinets_dir} directory not found.")
        return

    cab_list = []
    
    for filename in os.listdir(cabinets_dir):
        if filename.endswith(".wav"):
            full_name = filename.replace(".wav", "")
            
            # 카테고리 판별 (파일명의 첫 단어 또는 대시 이전 부분)
            category = "Unknown"
            if " - " in full_name:
                category = full_name.split(" - ")[0]
            elif "_" in full_name:
                category = full_name.split("_")[0]
            else:
                category = full_name.split(" ")[0]
                
            # 카테고리가 너무 짧거나 숫자인 경우 보정
            if len(category) < 2 or category.isdigit():
                category = "General"

            cab_list.append({
                "name": full_name,
                "category": category,
                "filename": filename
            })
            
    # 카테고리 및 이름 순 정렬
    cab_list.sort(key=lambda x: (x['category'], x['name']))
    
    output_data = {
        "cabinets": cab_list
    }
    
    with open(output_file, "w", encoding="utf-8") as f:
        json.dump(output_data, f, indent=4, ensure_ascii=False)
        
    print(f"Successfully generated {len(cab_list)} cabinets in {output_file}")

if __name__ == "__main__":
    generate_cabinet_list()
