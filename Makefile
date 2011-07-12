CC=gcc
CFLAGS=-shared -fPIC -w -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux
PROJECT=org_jboss_odbc_OdbcClient
EXT=c

all: $(PROJECT).so

$(PROJECT).so: $(PROJECT).$(EXT)
	$(CC) $(CFLAGS) util.$(EXT) $(PROJECT).$(EXT) -o $(PROJECT).so -lodbc -lpthread -lltdl -ldl
	ln -sf $(PROJECT).so libjavaODBC.so

clean:
	rm $(PROJECT).so libjavaODBC.so
