CC = g++
CFLAGS = -std=c++11 -fno-stack-protector -rdynamic -O2

_OBJ = pen-lang.o \
	   pen-interfaces.o \
	   pen-keyword.o \
	main.o 

_DEPS = pen-interfaces.hpp \
		pen-lang.hpp \
		pen-utility.hpp

ODIR = bin
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

IDIR = includes
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

$(ODIR)/%.o : %.cpp  $(DEPS)
	 @mkdir $(ODIR) -p
	 @$(CC) -c -o $@ $< $(CFLAGS)

pencil : $(OBJ)
	@$(CC) -o $@ $^ $(CFLAGS)
	@cp ./pencil ./bin/pencil

.PHONY : clean rebuild

all : pencil

clean : 
	@rm -rf $(ODIR)

install : pencil
	@sudo cp ./pencil /usr/bin/pencil

rebuild : clean pencil
