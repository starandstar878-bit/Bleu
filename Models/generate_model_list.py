import os
import json

def generate_model_list():
    models_dir = os.path.dirname(os.path.abspath(__file__))
    model_list = []
    
    for filename in os.listdir(models_dir):
        if filename.endswith(".nam"):
            # 파일 이름 파싱 시도 (보통 'Author Name Model Name.nam' 형식)
            full_name = os.path.splitext(filename)[0]
            parts = full_name.split(' ', 1)
            
            author = parts[0] if len(parts) > 1 else "Unknown"
            model_name = parts[1] if len(parts) > 1 else full_name
            
            # 캡처 타입 판별 (DI vs Full Rig)
            lower_name = filename.lower()
            di_keywords = ['di', 'direct', 'head', 'amp only', 'captor', 'loadbox']
            cab_keywords = ['cab', 'full', 'miked', 'sm57', 'v30', 'g12', 'royer', 'celestion']
            
            capture_type = "Full" # 기본값
            if any(kw in lower_name for kw in di_keywords):
                capture_type = "DI"
            elif any(kw in lower_name for kw in cab_keywords):
                capture_type = "Full"
            
            model_list.append({
                "name": full_name,
                "author": author,
                "model": model_name,
                "filename": filename,
                "type": capture_type
            })
            
    # 알파벳 순 정렬
    model_list.sort(key=lambda x: x["name"])
    
    output_path = os.path.join(models_dir, "model_list.json")
    with open(output_path, "w", encoding="utf-8") as f:
        json.dump({"models": model_list}, f, indent=4, ensure_ascii=False)
    
    print(f"Successfully generated {len(model_list)} models in model_list.json")

if __name__ == "__main__":
    generate_model_list()
