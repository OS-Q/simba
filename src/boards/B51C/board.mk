INC += $(SIMBA_ROOT)/src/boards/B51C
SRC += $(SIMBA_ROOT)/src/boards/B51C/board.c

LINKER_SCRIPT ?= simba.flash.4m.ld
ESP_FLASH_SIZE = 4M

BOARD_HOMEPAGE = "http://www.nodemcu.com"
BOARD_PINOUT = "nodemcu-pinout.png"
BOARD_DESC = "B51C"

MCU = esp8266
SERIAL_PORT ?= /dev/arduino
BOARD_PY = $(SIMBA_ROOT)/src/boards/B51C/board.py
RUN_PY ?= $(SIMBA_ROOT)/src/boards/B51C/run.py
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
