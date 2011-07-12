#!/bin/bash
ODBC_CONNECT="DRIVER={PostgreSQL64};DATABASE=perf;SERVER=jawa09;PORT=35432;Uid=admin;Pwd=teiid;"
SQL=${1:-"select * from perf.long.persons where person_id < 100000"};
COUNT=100;
THREADS=10;

./build.sh;
#java -agentpath:/mnt/jqa/jprofiler-6.2.2/bin/linux-x64/libjprofilerti.so=port=8849 -Djava.library.path=. org.jboss.odbc.RunTest javaODBC $ODBC_CONNECT "$SQL" $THREADS $COUNT
java -Djava.library.path=. org.jboss.odbc.RunTest javaODBC $ODBC_CONNECT "$SQL" $THREADS $COUNT
