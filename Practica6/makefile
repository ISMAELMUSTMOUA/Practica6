CC = gcc

EXE = calculo_punto_fijo

SRC = src/c
BUILD = build
BIN = bin
test_data = tests/data

CFLAGS = -g -Wall `pkg-config --cflags gtk+-3.0` -Iinclude -fPIC
LIBS = `pkg-config --libs gtk+-3.0` -lm

all: create-dirs $(BIN)/$(EXE) $(BUILD)/libcomaflotante.so $(BUILD)/libpuntofijo.so generate-test-data

# Ejecutables y librerías
$(BIN)/$(EXE): $(SRC)/interfaz.c $(SRC)/gestion.c $(SRC)/puntofijo.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILD)/libcomaflotante.so: $(SRC)/comaflotante.c
	$(CC) -fPIC -c $^ -o $(BUILD)/comaflotante.o
	$(CC) -shared -o $@ $(BUILD)/comaflotante.o -lm

$(BUILD)/libpuntofijo.so: $(SRC)/puntofijo.c
	$(CC) -fPIC -c $^ -o $(BUILD)/puntofijo.o
	$(CC) -shared -o $@ $(BUILD)/puntofijo.o -lm


# Creación de directorios para evitar errores
create-dirs:
	mkdir -p $(BUILD)
	mkdir -p $(BIN)
	mkdir -p $(test_data)


# Generacion de datos de testeo
generate-test-data:
	python3 ./tests/generacion.py


# Tests de capas
test_capa1: $(BUILD)/libpuntofijo.so
	python ./tests/GUI_test.py

test_capa2: $(BUILD)/libpuntofijo.so
	pytest ./tests/pytest_cap2.py

test_capa3: $(BUILD)/libpuntofijo.so
	pytest ./tests/pytest_cap3.py


run: $(BIN)/$(EXE)
	./$(BIN)/$(EXE)

clean:
	rm -rf $(BIN) $(BUILD) $(test_data)

.PHONY: all create-dirs generate-test-data test_capa1 test_capa2 test_capa3 run clean