CC=		gcc
CFLAGS=		-g -gdwarf-2 -Wall -std=gnu99
LD=		gcc
LDFLAGS=	-L.
AR=		ar
ARFLAGS=	rcs
TARGETS=	freq

all:		$(TARGETS)

#-------------------------------------------------------------------------------

# TODO: Add rules for test_entry, test_fnv, test_map, freq, measure

test_entry.o:	test_entry.c map.h
	@echo Compiling test_entry.o...
	@$(CC) $(CFLAGS) -c -o $@ $<

test_fnv.o:	test_fnv.c map.h
	@echo Compiling test_fnv.o...
	@$(CC) $(CFLAGS) -c -o $@ $<

test_map.o:	test_map.c map.h
	@echo Compiling test_map.o...
	@$(CC) $(CFLAGS) -c -o $@ $<

freq.o:		freq.c map.h
	@echo Compiling freq.o...
	@$(CC) $(CFLAGS) -c -o $@ $<

map.o:		map.c map.h
	@echo Compiling map.o...
	@$(CC) $(CFLAGS) -c -o $@ $<

fnv.o:		fnv.c map.h
	@echo Compiling fnv.o...
	@$(CC) $(CFLAGS) -c -o $@ $<

entry.o:	entry.c map.h
	@echo Compiling entry.o...
	@$(CC) $(CFLAGS) -c -o $@ $<

measure.o:	measure.c
	@echo Compiling measure...
	@$(CC) $(CFLAGS) -c -o $@ $^

libmap.a:	entry.o fnv.o map.o
	@echo Linking libmap.a...
	@$(AR) $(ARFLAGS) $@ $^

test_entry:	test_entry.o libmap.a
	@echo Linking test_entry...
	@$(LD) $(LDFLAGS) -o $@ $^

test_fnv:	test_fnv.o libmap.a
	@echo Linking test_fnv...
	@$(LD) $(LDFLAGS) -o $@ $^

test_map:	test_map.o libmap.a
	@echo Linking test_map...
	@$(LD) $(LDFLAGS) -o $@ $^

freq:	freq.o libmap.a
	@echo Linking freq...
	@$(LD) $(LDFLAGS) -o $@ $^

#-------------------------------------------------------------------------------

test:		test-entry test-fnv test-map test-freq

test-entry:	test_entry
	@echo Testing entry...
	@[ `valgrind --leak-check=full ./test_entry 2>&1 | tee test_entry.log | grep ERROR | awk '{print $$4}'` = 0 ] && ! grep -q 'failed' test_entry.log || cat test_entry.log

test-fnv:	test_fnv
	@echo Testing fnv...
	@[ `valgrind --leak-check=full ./test_fnv 2>&1 | tee test_fnv.log | grep ERROR | awk '{print $$4}'` = 0 ] && ! grep -q 'failed' test_fnv.log || cat test_fnv.log

test-map:	test_map
	@echo "Testing map..."
	@[ `valgrind --leak-check=full ./test_map 2>&1 | tee test_map.log | grep ERROR | awk '{print $$4}'` = 0 ] && ! grep -q 'failed' test_map.log || cat test_map.log

test-freq:	test_freq.sh freq
	@echo "Testing freq..."
	@./test_freq.sh 2>&1 | tee test_freq.log
	@[ `valgrind --leak-check=full ./freq < map.h 2>&1 | tee test_freq.log | grep ERROR | awk '{print $$4}'` = 0 ] && ! grep -q 'failed' test_freq.log || cat test_freq.log

benchmark:	measure freq benchmark.py
	@echo Benchmarking
	@./benchmark.py

clean:
	@echo Cleaning...
	@rm -f $(TARGETS) test_entry test_fnv test_map measure *.a *.o *.log *.input

.PHONY:		all test benchmark clean
