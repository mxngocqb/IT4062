# Tuần 2 — Lab ôn tập lập trình C trên UNIX

Bộ bài thực hành này bám theo slide **`Docs/Chapter 2.pptx` — "UNIX C++ Programming Review"**,
bao gồm cả **3 bài Exercise** có trong slide (IEEE 754, mảng động, duyệt thư mục).
Toàn bộ mã nguồn đã được viết sẵn, biên dịch không cảnh báo với `-Wall -Wextra` trên
macOS và Ubuntu/Linux.

---

## 1. Biên dịch và chạy

```bash
cd Week_2
make                 # biên dịch tất cả vào thư mục bin/
make lab03_dynamic_array      # biên dịch riêng một bài
make run-lab03_dynamic_array  # biên dịch và chạy luôn
make clean           # xoá kết quả biên dịch và các tệp sinh ra
make sanitize        # biên dịch lại với -fsanitize=address (bắt lỗi bộ nhớ)
```

Trên macOS, nếu muốn dùng gcc thật do Homebrew cài (thay vì clang):

```bash
make CC=gcc-16
```

---

## 2. Danh sách bài lab

| Bài | Tệp nguồn | Nội dung | Slide |
|-----|-----------|----------|-------|
| 00 | `lab00_build/` | Biên dịch một tệp, nhiều tệp, `-c` `-l` `-L` `-I` `-g`, Makefile | 4–6 |
| 01 | `lab01_ieee754.c` | **Exercise:** con trỏ đọc từng byte của `float`, phân tích IEEE 754 | 8–10 |
| 02 | `lab02_pointer_array.c` | Số học con trỏ, quan hệ con trỏ ↔ mảng, `sizeof` | 9, 11 |
| 03 | `lab03_dynamic_array.c` | **Exercise:** nhập số thực đến khi gặp 0, mảng động, tính tổng | 12–13 |
| 04 | `lab04_pointer_to_pointer.c` | `char **`, `argc/argv`, tham số ra kiểu con trỏ tới con trỏ | 14 |
| 05 | `lab05_function_pointer.c` | Con trỏ hàm, call-back, `qsort`, bảng điều phối lệnh | 15 |
| 06 | `lab06_io_basic.c` | `stdin/stdout/stderr`, `printf/scanf`, `sprintf/sscanf`, `errno`, `perror` | 16–18 |
| 07 | `lab07_file_stdio.c` | `fopen/fprintf/fscanf/fgetc/fputc/feof/ferror/fflush/fclose` | 19–20 |
| 08 | `lab08_lowlevel_io.c` | `open/read/write/close`, vòng lặp ghi đủ byte, hexdump | 21 |
| 09 | `lab09_string.c` | Toàn bộ nhóm hàm `str*`, tìm kiếm, `strtok_r`, sao chép an toàn | 22–23 |
| 10 | `lab10_memory.c` | `memset/memcpy/memmove/memcmp/memchr`, padding của struct | 24 |
| 11 | `lab11_filesystem.c` | `getcwd/chdir/access/stat/fstat/scandir/rename/remove/tmpfile` | 25–27 |
| 12 | `lab12_process.c` | `system()`, `fork()`, `execl()`, `wait()`, `exit()` | 28, 30–32 |
| 13 | `lab13_dir_browser.c` | **Exercise 1:** `ifconfig` + `ls -a -l` + `scandir` → HTML + duyệt thư mục | 29 |

---

## 3. Ba bài Exercise trong slide

### Exercise slide 10 — biểu diễn IEEE 754

```bash
./bin/lab01_ieee754
# nhập 1.5  →  0x3FC00000 = 0 01111111 10000000000000000000000
```

Chương trình in ra 4 byte theo đúng thứ tự trong bộ nhớ, ghép lại thành số 32 bit,
tách dấu / số mũ / phần định trị rồi **dựng lại giá trị** để so với số ban đầu.
Kết quả khớp với phép kiểm chứng bằng tay ghi ở cuối màn hình.

### Exercise slide 13 — mảng cấp phát động

```bash
./bin/lab03_dynamic_array
# nhập lần lượt: 1.5  2.5  3  4  5  0
# → TỔNG = 16, mảng tự mở rộng 4 → 8 phần tử bằng realloc
```

### Exercise 1 slide 29 — duyệt thư mục

```bash
./bin/lab13_dir_browser
```

Chương trình chạy lần lượt cả 4 phần của đề bài, sau đó vào vòng lặp tương tác:
nhập tên thư mục để đi vào, `..` để đi lên (chặn khi đã ở thư mục gốc), `ip`, `ls`,
`html`, `q`. Kết quả HTML vừa in ra màn hình vừa ghi vào tệp `listing.html` —
mở bằng trình duyệt để thấy các liên kết hoạt động.

---

## 4. Bài tập cho sinh viên (mở rộng từ mã có sẵn)

1. **Lab 01** — sửa để phân tích `double` (8 byte, số mũ 11 bit, định trị 52 bit).
2. **Lab 03** — thêm tuỳ chọn thu nhỏ mảng bằng `realloc` khi nhập xong; đo số lần
   `realloc` khi tăng dung lượng theo cấp số nhân so với tăng thêm 1 phần tử mỗi lần.
3. **Lab 05** — thêm lệnh `RETR <n>` vào bảng điều phối; đây chính là cách server
   POP3 ở chương 4 xử lý lệnh.
4. **Lab 08** — thêm tham số `-n <số byte>` để chỉ sao chép phần đầu tệp; giải thích
   vì sao vòng lặp `ghi_het()` là bắt buộc khi chuyển sang `send()` trên socket.
5. **Lab 09** — viết hàm tách một yêu cầu HTTP thành *method*, *path*, *version* và
   danh sách các dòng header.
6. **Lab 12** — sửa thành mô hình *một tiến trình con phục vụ một yêu cầu*: cha lặp
   `fork()`, con làm việc rồi `_exit()`; đây là khung của server đa tiến trình ở chương 3.
7. **Lab 13** — thêm cột kích thước và thời gian sửa đổi vào bảng HTML; thêm lệnh
   `mkdir <tên>` và `rm <tên>`.

---

## 5. Lưu ý khi làm bài

- Luôn biên dịch với `-Wall -Wextra` và **sửa hết cảnh báo** trước khi nộp.
- Luôn kiểm tra giá trị trả về của mọi lời gọi hệ thống (`-1` là lỗi, kèm `errno`).
- Mỗi `malloc/calloc/realloc` phải có đúng một `free`; kiểm tra bằng
  `make sanitize` hoặc `valgrind --leak-check=full ./bin/<chương trình>`.
- Dữ liệu đọc từ tệp/mạng **không tự có ký tự kết thúc `'\0'`** — phải tự thêm.
- Bài nộp cần chạy được trên **Ubuntu/Linux**, kể cả khi làm trên macOS.
