# 🧮 ASSIGNMENT 1 THE INFINITY CASTLE BATTLE

## ⚙️ Cách chạy chương trình với 1 test

```sh
➜ g++ -o main main.cpp
➜  echo "10 1000 1.0 1 N 1 1 100 3 100 150 0" | ./main 
[Scene 1] Rank: Hashira (power = 250.0)
[Scene 2] Open silently.
[Scene 3] Engage head-on (adv = 141.0)
[Scene 4] Boss defeated! (finalHP = 0)      
```

## ⚙️ Cách chạy chương trình với nhiều test

| Tên file                  | Mô tả                                                                                                                                                | Vai trò trong quá trình test                                                       |
| ------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------- |
| **`input.txt`**           | Chứa **dữ liệu đầu vào** cho từng test case. Mỗi dòng là **một bộ input** được truyền vào chương trình (ví dụ: `10 1000 1.0 1 N 1 1 100 3 100 150 0`). | Được script đọc từng dòng và đưa vào `cin` của chương trình để chạy test.          |
| **`expected_output.txt`** | Chứa **kết quả mong đợi** tương ứng với mỗi test case. Mỗi test case thường gồm **4 dòng output** (hoặc số dòng bạn quy định).                       | Dùng để so sánh với kết quả chương trình thực tế, xác định PASS/FAIL.              |
| **`output.txt`**          | Chứa **kết quả chương trình thực tế** sau khi chạy tất cả các test trong `input.txt`.                                                                | Script tự động tạo file này khi chạy, dùng để đối chiếu với `expected_output.txt`. |


### Cách chạy WINDOWN

```sh
PS C:\Code\NMDT\BTL1> .\test.bat
All test cases PASSED!
```

### Cách chạy Linux/MacOS

```sh
➜ chmod +x test.sh
➜ ./test.sh
Test 1 PASS
Test 2 PASS
Test 3 PASS
All test cases PASSED!      
```

