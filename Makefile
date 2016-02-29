cflag=$(shell pkg-config --cflags gtk+-3.0)
libs=$(shell pkg-config --libs gtk+-3.0)
objectfile=main.o io.o mainUI.o callback.o share.o BookType.o BookInfo.o RentInfo.o queryUI.o query.o

all: $(objectfile)
	gcc $(objectfile) $(cflag) -o main.exe $(libs)

$(objectfile): %.o: %.c
	gcc $< -c $(cflag) -o $@ $(libs)

clean:
	del *.o