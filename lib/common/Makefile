MAKEFILE_DIR=makefiles
LIB_DIR=lib
OBJ_DIR=obj

all:
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(OBJ_DIR)
	@echo -e "\033[0;32m[Compiling STM32F767 Static Library]\033[0m"
	@make -C $(MAKEFILE_DIR)/stm32f767zi

clean:
	@rm -rf $(LIB_DIR) $(OBJ_DIR)
