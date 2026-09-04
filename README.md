# IT4062 — Thực hành Lập trình mạng

Mã nguồn và tài liệu thực hành học phần **IT4062 — Thực hành Lập trình mạng**,
Khoa Kỹ thuật máy tính, Trường Công nghệ Thông tin và Truyền thông,
Đại học Bách khoa Hà Nội.

Giảng viên: **ThS. Mai Xuân Ngọc**

---

## Mục lục

1. [Cấu trúc repo](#1-cấu-trúc-repo)
2. [Đề cương môn học](#2-đề-cương-môn-học)
3. [Cài đặt môi trường](#3-cài-đặt-môi-trường)
   - [Windows (qua WSL)](#31-windows--qua-wsl)
   - [Ubuntu / Linux](#32-ubuntu--linux)
   - [macOS](#33-macos)
   - [Kiểm tra môi trường](#34-kiểm-tra-môi-trường)
4. [Cài đặt Visual Studio Code và extension](#4-cài-đặt-visual-studio-code-và-extension)
5. [Biên dịch và chạy chương trình](#5-biên-dịch-và-chạy-chương-trình)
6. [Gỡ lỗi (debug)](#6-gỡ-lỗi-debug)
7. [Danh sách bài lab](#7-danh-sách-bài-lab)
8. [Tài liệu tham khảo](#8-tài-liệu-tham-khảo)

---

## 1. Cấu trúc repo

```
IT4062/
├── .vscode/
│   ├── tasks.json          # biên dịch / biên dịch + chạy
│   ├── launch.json         # cấu hình gỡ lỗi
│   └── extensions.json     # extension khuyến nghị
├── 1-Docs/                 # tài liệu môn học (xem ghi chú bên dưới)
├── 2-Chap2/                # Lab ôn tập lập trình C trên UNIX
│   ├── Makefile
│   ├── README.md           # hướng dẫn chi tiết từng bài lab
│   ├── lab00_build/        # ví dụ biên dịch nhiều tệp nguồn
│   └── lab01…lab13.c       # 13 bài lab, mã nguồn đầy đủ
├── .gitignore
└── README.md               # tệp bạn đang đọc
```

> **Ghi chú về `1-Docs/`** — slide `*.pptx` và tệp bài giảng PDF **không được đưa lên
> repo** (đã liệt kê trong `.gitignore`) vì thuộc bản quyền của nhóm tác giả học phần.
> Sinh viên lấy tài liệu từ hệ thống của Nhà trường và đặt vào thư mục này.

---

## 2. Đề cương môn học

Học phần gồm **13 buổi**. Cột cuối cho biết slide và mã nguồn tương ứng có trong repo này.

| Buổi | Nội dung | Content | Hoạt động dạy – học | CĐR | Tài liệu trong repo |
|:---:|---|---|---|:---:|---|
| 1 | Nhắc lại các khái niệm trong Mạng máy tính | Computer network review | Giảng dạy | CLO1 | `1-Docs/Chapter 1.pptx` |
| 2 | Nhắc lại về lập trình C/C++ trong môi trường Linux | UNIX C/C++ programming review | Giảng dạy và hướng dẫn làm bài tập | CLO1 | `1-Docs/Chapter 2.pptx` → **[`2-Chap2/`](2-Chap2/)** |
| 3 | Giới thiệu về Socket | Socket introduction | Giảng dạy | CLO1 | `1-Docs/Chapter 3.pptx` |
| 4 | Lập trình với các hàm socket cơ bản | Elementary sockets | Giảng dạy và hướng dẫn làm bài tập | CLO1, CLO2 | `1-Docs/Chapter 4.pptx` |
| 5 | Lập trình đa luồng | Threads | Giảng dạy và hướng dẫn làm bài tập | CLO1, CLO2 | `1-Docs/Chapter 5.pptx`, `Chapter 6.pptx` |
| 6 | Các phương pháp vào ra trộn | I/O multiplexing | Giảng dạy và hướng dẫn làm bài tập | CLO1, CLO2 | `1-Docs/Chapter 7.pptx` |
| 7 | Lập trình UDP | UDP sockets | Giảng dạy và hướng dẫn làm bài tập | CLO1, CLO2 | `1-Docs/Chapter 4.pptx` (phần UDP) |
| 8 | Thiết kế giao thức | Custom protocol design | Giảng dạy và hướng dẫn làm bài tập | CLO1, CLO2 | — |
| 9 | Phát triển các dịch vụ trên Linux | Daemon processes | Giảng dạy và hướng dẫn làm bài tập | CLO1, CLO2 | — |
| 10 | Các phương pháp vào/ra nâng cao | Advanced I/O | Giảng dạy và hướng dẫn làm bài tập | CLO1, CLO2 | — |
| 11 | Các phương pháp vào/ra bất đồng bộ | IOCTL and non-blocking I/O | Giảng dạy và hướng dẫn làm bài tập | CLO1, CLO2 | — |
| 12 | Truyền theo nhóm và truyền quảng bá | Multicasting and broadcasting | Giảng dạy và hướng dẫn làm bài tập | CLO1, CLO2 | — |
| 13 | Sinh viên trình bày dự án | Mini project | Nghe sinh viên thuyết trình và góp ý | CLO2 | — |

**Chuẩn đầu ra (CĐR / CLO)**

- **CLO1** — Hiểu nguyên lý hoạt động của ứng dụng mạng: mô hình TCP/IP, vai trò của
  tầng giao vận, cơ chế socket, các kiến trúc server–client.
- **CLO2** — Thiết kế và cài đặt được ứng dụng mạng bằng C trên môi trường UNIX:
  giao thức tự định nghĩa, phục vụ nhiều client, vào/ra nâng cao.

> Các buổi chưa có tài liệu trong repo sẽ được bổ sung dần theo tiến độ học kỳ.

## 3. Cài đặt môi trường

Mục tiêu: có được **`gcc`** (biên dịch), **`gdb`** hoặc **`lldb`** (gỡ lỗi),
**`make`**, và các công cụ mạng để dùng cho các chương sau.

### 3.1. Windows — qua WSL

Mọi bài thực hành phải **biên dịch và chạy trong môi trường Linux**, không dùng
trình dịch của Windows.

1. Bật tính năng **Windows Subsystem for Linux** trong *Windows Features*.
2. Cài **Windows Subsystem for Linux** từ Microsoft Store.
3. Cài **Ubuntu** từ Microsoft Store.
4. Cài **Windows Terminal** từ Microsoft Store.
5. Mở Terminal, gõ `wsl` để vào môi trường Ubuntu.
6. Tiếp tục theo đúng các lệnh ở mục **2.2** ngay bên trong WSL.

```powershell
# Cách nhanh hơn, chạy PowerShell với quyền Administrator:
wsl --install -d Ubuntu
```

### 3.2. Ubuntu / Linux

```bash
sudo apt update
sudo apt install build-essential      # gcc, g++, make
sudo apt install gdb                  # trình gỡ lỗi
sudo apt install manpages-dev         # tra cứu lời gọi hệ thống: man 2 socket

# Công cụ dùng cho các chương sau
sudo apt install net-tools iproute2   # ifconfig, ip, ss
sudo apt install netcat-openbsd       # nc
sudo apt install wireshark tshark     # bắt và phân tích gói tin
sudo apt install valgrind             # kiểm tra rò rỉ bộ nhớ
```

### 3.3. macOS

```bash
# 1. Cài homebrew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 2. Bộ công cụ dòng lệnh của Apple (đã có sẵn clang, lldb, make)
xcode-select --install

# 3. Trình biên dịch gcc thật (tuỳ chọn — brew đặt tên có số hiệu: gcc-16)
brew install gcc

# 4. Công cụ hỗ trợ
brew install netcat wireshark
```

Ba khác biệt cần biết trên macOS:

| Vấn đề | Trên Linux | Trên macOS |
|---|---|---|
| Lệnh `gcc` | là GCC thật | thực chất là **clang**; GCC của brew tên `gcc-15`, `gcc-16`… |
| Trình gỡ lỗi | `gdb` | dùng **`lldb`** (gdb cần ký mã, rất khó dùng trên Apple Silicon) |
| Xem địa chỉ IP | `ip a`, `ss -tlnp` | `ifconfig`, `lsof -i -P -n` |

macOS là hệ Unix (BSD) nên hầu hết mã socket chạy được, nhưng **bài nộp cần được
kiểm tra lại trên Ubuntu**.

### 3.4. Kiểm tra môi trường

```bash
gcc -v && gdb -v          # Linux / WSL
gcc-16 -v && lldb --version   # macOS (nếu cài gcc qua brew)
make -v
```

Rồi thử biên dịch bài đầu tiên:

```bash
cd 2-Chap2/lab00_build
make
./hello
./calc
```

---

## 4. Cài đặt Visual Studio Code và extension

1. Tải VS Code: <https://code.visualstudio.com/>
2. Mở thư mục repo này bằng VS Code (`File → Open Folder…`).
3. VS Code sẽ hiện thông báo **"This workspace has extension recommendations"** →
   bấm **Install** (danh sách nằm trong `.vscode/extensions.json`).

Cài bằng tay nếu không thấy thông báo — mở Extensions (`Ctrl/Cmd + Shift + X`):

| Extension | ID | Bắt buộc? | Dùng để |
|---|---|---|---|
| **C/C++ Extension Pack** | `ms-vscode.cpptools-extension-pack` | **Có** | tô màu cú pháp, gợi ý mã, biên dịch, gỡ lỗi |
| **WSL** | `ms-vscode-remote.remote-wsl` | Chỉ Windows | mở project bên trong môi trường Ubuntu |

**Người dùng Windows** phải mở project *bên trong* WSL thì mới biên dịch được:
nhấn nút **"Open a Remote Window"** ở góc trái dưới → **"Connect to WSL"** →
mở lại thư mục repo. Thanh trạng thái phải hiện `WSL: Ubuntu`.

Toàn bộ cấu hình biên dịch và gỡ lỗi đã có sẵn trong `.vscode/`, **không cần tự tạo**
(xem phần 5 nếu bạn dùng WSL/Ubuntu — cần sửa tên trình biên dịch cho đúng máy).

---

## 5. Biên dịch và chạy chương trình

### Cách 1 — Bằng VS Code (khuyến nghị)

Mở tệp `.c` cần chạy rồi nhấn:

| Phím | Task | Kết quả |
|---|---|---|
| **`Ctrl/Cmd + Shift + B`** | *C/C++: bien dich va CHAY tep hien tai* | Biên dịch **rồi chạy luôn** trong tab **TERMINAL** — **gõ được dữ liệu vào** |
| `Terminal → Run Task…` | *C/C++: gcc-16 build active file* | Chỉ biên dịch, không chạy |

> **Người dùng WSL / Ubuntu cần sửa 1 dòng.** Cấu hình trong repo được đặt theo máy
> macOS của giảng viên nên gọi thẳng `/opt/homebrew/bin/gcc-16`. Mở
> `.vscode/tasks.json`, thay mọi chỗ `/opt/homebrew/bin/gcc-16` bằng `gcc`:
>
> ```jsonc
> "command": "gcc",                                  // trong task "gcc-16 build active file"
> "command": "gcc -fdiagnostics-color=always ...",   // trong task "bien dich va CHAY"
> ```
>
> Tương tự trong `.vscode/launch.json`, đổi `"MIMode": "lldb"` thành `"MIMode": "gdb"`.

### Cách 2 — Bằng `make`

```bash
cd 2-Chap2
make                            # biên dịch tất cả vào bin/
make run-lab03_dynamic_array    # biên dịch và chạy một bài
make clean                      # dọn dẹp
make sanitize                   # biên dịch lại với -fsanitize=address
make CC=gcc-16                  # chỉ định trình biên dịch khác
```

### Cách 3 — Gõ tay

```bash
gcc -std=gnu11 -Wall -Wextra -g lab03_dynamic_array.c -o lab03 -lm
./lab03
```

Các cờ hay dùng:

| Cờ | Ý nghĩa |
|---|---|
| `-Wall -Wextra` | bật cảnh báo — **luôn dùng**, và phải sửa hết cảnh báo |
| `-g` | sinh thông tin gỡ lỗi (bắt buộc nếu muốn debug) |
| `-o tên` | đặt tên tệp thực thi |
| `-c` | chỉ dịch, chưa liên kết |
| `-lm` | liên kết thư viện toán (`math.h`) |
| `-pthread` | thư viện luồng — chương 3 |
| `-lssl -lcrypto` | OpenSSL — chương 5 |
| `-fsanitize=address` | bắt lỗi bộ nhớ ngay khi chạy |

---

## 6. Gỡ lỗi (debug)

### 6.1. Bằng VS Code

1. Đặt breakpoint: bấm vào **lề trái số dòng** (chấm đỏ).
2. Nhấn **`F5`**, chọn cấu hình:

| Cấu hình | Dùng cho | Chương trình chạy ở |
|---|---|---|
| **Debug tep C hien tai** (`externalConsole: true`) | Bài **có nhập liệu** (lab03, lab06, lab13) | Một **cửa sổ terminal riêng** — gõ dữ liệu ở đó |
| **Debug tep C hien tai (khong nhap lieu)** (`externalConsole: false`) | Bài chỉ in ra (lab02, lab05, lab10…) | Debug Console của VS Code |

3. Khi dừng ở breakpoint: xem biến ở panel **VARIABLES**, bước tiếp bằng
   `F10` (next) / `F11` (step into) / `F5` (continue).

### 6.2. Vì sao không gõ được dữ liệu vào Debug Console?

Đây là câu hỏi gặp nhiều nhất.

```
   chương trình  ──stdout──▶  trình gỡ lỗi  ──▶  Debug Console   ✅ một chiều
   chương trình  ◀──stdin───       ✗ không có đường này
```

**Debug Console không phải terminal.** Ô `>` ở đáy Debug Console gửi những gì bạn gõ
tới **trình gỡ lỗi** để tính biểu thức (yêu cầu `evaluate` của Debug Adapter
Protocol), chứ không gửi tới `stdin` của chương trình. Chương trình đang chạy thì
không có khung ngăn xếp để tính biểu thức, nên báo
*"Unable to perform this action because the process is running"*.

Ở tầng hệ điều hành: `scanf` đọc từ mô tả tệp số 0. Chạy trong Debug Console thì fd 0
nối vào một **ống (pipe)** không ai ghi vào → treo. Chạy trong terminal thì fd 0 nối
vào một **pseudo-terminal thật** → gõ phím là đọc được.

Vì vậy:

- **Terminal** — để nhập/xuất dữ liệu (`Ctrl/Cmd + Shift + B`, hoặc `externalConsole: true`).
- **Debug Console** — để hỏi trình gỡ lỗi khi chương trình **đang dừng**: gõ `n`,
  `ds[0]`, `dung_luong`, `parray 5 ds`…

### 6.3. Bằng dòng lệnh

```bash
gcc -g -Wall -o prog prog.c
gdb ./prog          # Linux / WSL
lldb ./prog         # macOS
```

| Việc cần làm | gdb | lldb |
|---|---|---|
| Đặt breakpoint | `break main` / `b 42` | `breakpoint set -n main` / `b prog.c:42` |
| Chạy | `run` | `run` |
| Bước qua / bước vào | `next` / `step` | `next` / `step` |
| Chạy tiếp | `continue` | `continue` |
| In biến | `print x` | `print x` |
| Xem biến cục bộ | `info locals` | `frame variable` |
| Ngăn xếp lời gọi | `backtrace` | `bt` |

Cách này chạy hoàn toàn trong terminal nên **nhập liệu luôn hoạt động**.

---

## 7. Danh sách bài lab

Chi tiết từng bài xem [`2-Chap2/README.md`](2-Chap2/README.md).

| Bài | Nội dung |
|---|---|
| 00 | Biên dịch một tệp, nhiều tệp, Makefile |
| 01 | Con trỏ đọc từng byte của `float`, phân tích **IEEE 754** |
| 02 | Số học con trỏ, quan hệ con trỏ ↔ mảng |
| 03 | Mảng cấp phát động: `malloc` / `calloc` / `realloc` / `free` |
| 04 | Con trỏ tới con trỏ, `argc` / `argv` |
| 05 | Con trỏ hàm, call-back, bảng điều phối lệnh |
| 06 | `stdin` / `stdout` / `stderr`, `printf` / `scanf`, `errno` |
| 07 | Tệp qua `FILE *`: `fopen` / `fprintf` / `fscanf` / `fgetc` |
| 08 | Vào/ra mức thấp: `open` / `read` / `write` / `close` |
| 09 | Xử lý xâu ký tự, `strtok_r`, sao chép an toàn |
| 10 | `memset` / `memcpy` / `memmove` / `memcmp`, padding của struct |
| 11 | `getcwd` / `chdir` / `stat` / `scandir` / `rename` |
| 12 | `system` / `fork` / `execl` / `wait` / `exit` |
| 13 | Bài tập lớn: liệt kê thư mục dạng HTML + duyệt thư mục |

---

## 8. Tài liệu tham khảo

1. Lã Thế Vinh, Trương Diệu Linh, Lê Bá Vui, Bùi Trọng Tùng. *Bài giảng học phần
   Thực hành Lập trình mạng (IT4062)*, SoICT — HUST, 2025.
2. W. Richard Stevens. *Unix Network Programming, Vol. 1*, 3rd Ed., Prentice Hall.
3. Keir Davis, John W. Turner, Nathan Yocom. *The Definitive Guide to Linux Network
   Programming*, Apress.
4. Michael Donahoo, Kenneth Calvert. *TCP/IP Sockets in C: Practical Guide for
   Programmers*, Elsevier.
5. Tra cứu lời gọi hệ thống: <https://manpages.ubuntu.com>
