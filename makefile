CC = gcc

EXE = calculo_punto_fijo

SRC = src/c
BUILD = build
BIN = bin
test_data = tests/data
REPORTS = reports

# Flags para gcov (cobertura)
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
GCOV_LIBS = -lgcov

# Flags normales + gcov
CFLAGS = -g -Wall `pkg-config --cflags gtk+-3.0` -Iinclude -fPIC $(GCOV_FLAGS)
LIBS = `pkg-config --libs gtk+-3.0` -lm $(GCOV_LIBS)

all: create-dirs $(BIN)/$(EXE) $(BUILD)/libcomaflotante.so $(BUILD)/libpuntofijo.so generate-test-data

# Ejecutables y librerías
$(BIN)/$(EXE): $(SRC)/interfaz.c $(SRC)/gestion.c $(SRC)/puntofijo.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILD)/libcomaflotante.so: $(SRC)/comaflotante.c
	$(CC) -fPIC $(GCOV_FLAGS) -c $^ -o $(BUILD)/comaflotante.o
	$(CC) -shared -o $@ $(BUILD)/comaflotante.o -lm $(GCOV_LIBS)

$(BUILD)/libpuntofijo.so: $(SRC)/puntofijo.c
	$(CC) -fPIC $(GCOV_FLAGS) -c $^ -o $(BUILD)/puntofijo.o
	$(CC) -shared -o $@ $(BUILD)/puntofijo.o -lm $(GCOV_LIBS)


# Creación de directorios para evitar errores
create-dirs:
	mkdir -p $(BUILD)
	mkdir -p $(BIN)
	mkdir -p $(test_data)
	mkdir -p $(REPORTS)


# Generacion de datos de testeo
generate-test-data:
	python3 ./tests/generacion.py


# Tests de capas
test_capa1: $(BUILD)/libpuntofijo.so
	python ./tests/GUI_basic.py

test_capa2: $(BUILD)/libpuntofijo.so
	pytest ./tests/pytest_cap2.py

test_capa3: $(BUILD)/libpuntofijo.so
	python3 ./tests/test_puntofijo.py
	

# Reglas específicas para análisis de cobertura
coverage: create-dirs test_capa3
	mv -f *.gcda $(BUILD)/ 2>/dev/null || true
	mv -f *.gcno $(BUILD)/ 2>/dev/null || true
	gcov -o $(BUILD) $(BUILD)/puntofijo.o
	mv -f *.gcov $(REPORTS)/ 2>/dev/null || true
	lcov --capture --directory $(BUILD) --output-file $(REPORTS)/coverage.info
	genhtml $(REPORTS)/coverage.info --output-directory $(REPORTS)/coverage_report


run: $(BIN)/$(EXE)
	./$(BIN)/$(EXE)

clean:
	rm -rf $(BIN) $(BUILD) $(test_data) $(REPORTS) *.gcda *.gcno *.gcov coverage.info coverage_report

.PHONY: all create-dirs generate-test-data test_capa1 test_capa2 test_capa3 coverage run clean
