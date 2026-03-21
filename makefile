TARGET = project #название проекта

PREF_SRC = ./src/ #папка с файлами исходного кода
PREF_OBJ = ./obj/ #папка с объектными файлами

SRC = $(wildcard $(PREF_SRC)*.c)
OBJ = $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRC))

$(TARGET) : $(OBJ)
	gcc $(OBJ) -o $(TARGET)

$(PREF_OBJ)%.o : $(PREF_SRC)%.c
	gcc -c $< -o $@

clean : #очистка (удаление объектных файлов)
	rm $(TARGET) $(PREF_OBJ)*.o