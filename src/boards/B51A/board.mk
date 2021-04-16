INC += $(SIMBA_ROOT)/src/boards/B51A
SRC += $(SIMBA_ROOT)/src/boards/B51A/board.c

LINKER_SCRIPT ?= simba.flash.512k.ld
ESP_FLASH_SIZE = 512K

BOARD_HOMEPAGE = "http://espressif.com"
BOARD_PINOUT = "B51A-pinout.png"
BOARD_DESC = "B51A"

MCU = esp8266
SERIAL_PORT ?= /dev/arduino
BOARD_PY = $(SIMBA_ROOT)/src/boards/B51A/board.py
RUN_PY ?= $(SIMBA_ROOT)/src/boards/B51A/run.py
CONSOLE_RESET_TYPE ?= 0
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
