# 定義編譯器和編譯選項
CC = gcc

# 找到當前目錄下的所有 .c 檔案
SRC = $(wildcard *.c)

# 將所有 .c 檔案替換成 .o 檔案
OBJ = $(SRC:.c=.o)

# 編譯目標文件
main: $(OBJ)
	$(CC) -o $@ $^

# 編譯所有源文件為目標文件
%.o: %.c
	$(CC) -c $< -o $@

# 清理生成的目標文件和可執行文件
clean:
	rm -f $(OBJ) main
