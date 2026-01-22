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
	

#generacion de datos de testeo
generate-test-data:
	python3 generacion_de_testeo.py

# creación de directorios para evitar errores
create-dirs:
	mkdir -p $(BUILD)
	mkdir -p $(BIN)
	mkdir -p $(test_data)

# Tests de punto fijo
test-moles: $(BUILD)/libpuntofijo.so
	python ./src/python/testeo_moles.py

test-densidad: $(BUILD)/libpuntofijo.so
	python ./src/python/testeo_dens.py

test-energia: $(BUILD)/libpuntofijo.so
	python ./src/python/testeo_ener.py

# Tests de capas
test-cap1: $(BUILD)/libpuntofijo.so
	pytest testeo_cap1.py

testeo_cap3: $(BUILD)/libpuntofijo.so
	python testeo_cap3.py

test-all: test-moles test-densidad test-energia

run: $(BIN)/$(EXE)
	./$(BIN)/$(EXE)

clean:
	rm -rf $(BIN) $(BUILD) $(test_data)

.PHONY: all clean create-dirs test-moles test-densidad test-energia test_cap1 test_cap3 test-all run
