# =====================================================================
#  IT4062 - Thuc hanh Lap trinh mang
#  Tuan 2: Lab on tap lap trinh C tren UNIX (theo Chapter 2.pptx)
#
#  make            : bien dich tat ca cac bai lab vao thu muc bin/
#  make lab03      : bien dich rieng mot bai
#  make run-lab03  : bien dich va chay mot bai
#  make clean      : xoa toan bo ket qua bien dich
#
#  Tren macOS co the dung trinh dich cua brew:  make CC=gcc-16
# =====================================================================
CC      ?= gcc
CFLAGS   = -std=gnu11 -Wall -Wextra -g
LDLIBS   = -lm
BIN      = bin

SRCS  := $(sort $(wildcard lab*.c))
PROGS := $(patsubst %.c,$(BIN)/%,$(SRCS))
NAMES := $(basename $(SRCS))

all: $(BIN) $(PROGS) lab00

$(BIN):
	@mkdir -p $(BIN)

$(BIN)/%: %.c | $(BIN)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

# Bai lab00 nam trong thu muc rieng (vi du bien dich nhieu tep nguon)
lab00:
	@$(MAKE) --no-print-directory -C lab00_build CC=$(CC)

# Cho phep goi tat: make lab03  /  make run-lab03
define QUY_TAC_TAT
$(1): $(BIN)/$(1)
run-$(1): $(BIN)/$(1)
	@echo "--------- chay $(1) ---------"
	@./$(BIN)/$(1)
endef
$(foreach t,$(NAMES),$(eval $(call QUY_TAC_TAT,$(t))))

# Dich tat ca voi bo kiem tra bo nho khi chay
sanitize: CFLAGS += -fsanitize=address -fno-omit-frame-pointer
sanitize: clean all

clean:
	rm -f $(PROGS)
	rm -rf $(BIN)
	@$(MAKE) --no-print-directory -C lab00_build clean
	rm -f sinhvien.txt sinhvien_ban_sao.txt nguon_mau.bin ban_sao.bin listing.html

.PHONY: all clean lab00 sanitize $(NAMES) $(addprefix run-,$(NAMES))
