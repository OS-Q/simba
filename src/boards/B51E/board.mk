INC += $(SIMBA_ROOT)/src/boards/B51E
SRC += $(SIMBA_ROOT)/src/boards/B51E/board.c

LINKER_SCRIPT ?= simba.flash.4m.ld
ESP_FLASH_SIZE = 4M

BOARD_HOMEPAGE = "https://wiki.wemos.cc/products:d1:d1_mini"
BOARD_PINOUT = "wemos-d1-mini-pinout.jpg"
BOARD_DESC = "B51E"

MCU = esp8266
SERIAL_PORT ?= /dev/arduino
BOARD_PY = $(SIMBA_ROOT)/src/boards/B51E/board.py
RUN_PY ?= $(SIMBA_ROOT)/src/boards/B51E/run.py
CONSOLE_RESET_TYPE ?= -1
TIMEOUT ?= 10
BAUDRATE ?= 76800

upload:
	@echo "Uploading $(EXE)"
	python -u $(BOARD_PY) upload --port $(SERIAL_PORT) $(BIN)

rerun:
	@echo "Running '$(EXE)'."
	python -u $(RUN_PY) --port $(SERIAL_PORT) \
			    --timeout $(TIMEOUT) \
			    --baudrate $(BAUDRATE) \
			    --pattern $(RUN_END_PATTERN)\
			    --pattern-success $(RUN_END_PATTERN_SUCCESS) \
			    | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0
