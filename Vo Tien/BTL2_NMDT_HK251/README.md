# 🧮 ASSIGNMENT 2 HỌC NÓI TIẾNG HAWAIIAN

## ⚙️ Cách chạy chương trình với 1 test

```sh
➜ g++ -o main main.cpp
➜ echo "hawai'i au e wai ai" | ./main 
Pronunciation: h-ah-veye-'ee- ow- eh- weye- eye-     
```

## ⚙️ Cách chạy chương trình với nhiều test

| Tên file                  | Mô tả                                                                                                                                                | Vai trò trong quá trình test                                                       |
| ------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------- |
| **`input.txt`**           | Chứa **dữ liệu đầu vào** cho từng test case. Mỗi dòng là **một bộ input** được truyền vào chương trình. | Được script đọc từng dòng và đưa vào `cin` của chương trình để chạy test.          |
| **`expected_output.txt`** | Chứa **kết quả mong đợi** tương ứng với mỗi test case. Mỗi test case thường gồm **4 dòng output** (hoặc số dòng bạn quy định).                       | Dùng để so sánh với kết quả chương trình thực tế, xác định PASS/FAIL.              |
| **`output.txt`**          | Chứa **kết quả chương trình thực tế** sau khi chạy tất cả các test trong `input.txt`.                                                                | Script tự động tạo file này khi chạy, dùng để đối chiếu với `expected_output.txt`. |


### Cách chạy WINDOWN

```sh
PS C:\Code\NMDT\BTL2> .\test.bat
All test cases PASSED!
```

### Cách chạy Linux/MacOS

```sh
➜ chmod +x test.sh
➜ ./test.sh
All test cases PASSED!      
```

---
<p align="center">
  <a href="https://www.facebook.com/Shiba.Vo.Tien">
    <img src="https://img.shields.io/badge/Facebook-1877F2?style=for-the-badge&logo=facebook&logoColor=white" alt="Facebook VÕ TIẾN"/>
  </a>
  <a href="https://www.tiktok.com/@votien_shiba">
    <img src="https://img.shields.io/badge/TikTok-000000?style=for-the-badge&logo=tiktok&logoColor=white" alt="TikTok"/>
  </a>
  <a href="https://www.facebook.com/groups/khmt.ktmt.cse.bku?locale=vi_VN">
    <img src="https://img.shields.io/badge/Facebook%20Group-4267B2?style=for-the-badge&logo=facebook&logoColor=white" alt="Facebook Group"/>
  </a>
  <a href="https://www.facebook.com/CODE.MT.BK">
    <img src="https://img.shields.io/badge/Page%20CODE.MT.BK-0057FF?style=for-the-badge&logo=facebook&logoColor=white" alt="Facebook Page"/>
  </a>
  <a href="https://github.com/VoTienBKU">
    <img src="https://img.shields.io/badge/GitHub-181717?style=for-the-badge&logo=github&logoColor=white" alt="GitHub"/>
  </a>
</p>
