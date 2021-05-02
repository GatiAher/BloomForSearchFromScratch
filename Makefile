##############
# murmurhash #
##############

murmurhash.o: deps/murmurhash/murmurhash.c deps/murmurhash/murmurhash.h
	gcc -c deps/murmurhash/murmurhash.c

test_murmurhash: bloom/test/test_murmurhash.c murmurhash.o
	gcc -Wpedantic -Wall -Wextra -o test_murmurhash bloom/test/test_murmurhash.c murmurhash.o

################
# Bloom filter #
################

bloom.o: bloom/src/bloom.c bloom/src/bloom.h murmurhash.o
	gcc -c bloom/src/bloom.c

test_bloom: bloom/test/test_bloom_main.c bloom/test/test_bloom_main.h bloom/test/test_bloom.c bloom/test/test_bloom.h bloom.o
	gcc -Wpedantic -Wall -Wextra -o test_bloom bloom/test/test_bloom_main.c bloom/test/test_bloom.c bloom.o murmurhash.o

bf_editor: bloom/src/bf_editor_main.c bloom/src/bf_editor_main.h bloom/src/bf_editor.c bloom/src/bf_editor.h bloom.o
	gcc -Wpedantic -Wall -Wextra -o bf_editor bloom/src/bf_editor_main.c bloom/src/bf_editor.c bloom.o murmurhash.o

bf_play: bloom/src/bf_play_main.c bloom/src/bf_play_main.h bloom/src/bf_play.c bloom/src/bf_play.h bloom.o
	gcc -Wpedantic -Wall -Wextra -o bf_play bloom/src/bf_play_main.c bloom/src/bf_play.c bloom.o murmurhash.o

# demos

bf_spellcheck.dat: bf_editor
	./bf_editor -o bf_spellcheck.dat -i /usr/share/dict/words -m 1963040 -k 13

bf_spellcheck_w_Gati.dat: bf_spellcheck.dat
	echo "Gati" | ./bf_editor -o bf_spellcheck_w_Gati.dat -f bf_spellcheck.dat

bf_spellcheck: bf_play bf_spellcheck.dat
	echo "hi my name is Gati" | ./bf_play -f bf_spellcheck.dat

bf_spellcheck_w_Gati: bf_play bf_spellcheck_w_Gati.dat
	echo "hi my name is Gati" | ./bf_play -f bf_spellcheck_w_Gati.dat

# spellcheck, color definitely not in words set red
bf_spellcheck_readme: bf_play bf_spellcheck.dat
	./bf_play -f bf_spellcheck.dat -i README.md

# spellcheck, color may be in words set blue
bf_spellcheck_readme_x: bf_play bf_spellcheck.dat
	./bf_play -f bf_spellcheck.dat -i README.md -x

# spellcheck, print definitely not in words set
bf_spellcheck_readme_s: bf_play bf_spellcheck.dat
	./bf_play -f bf_spellcheck.dat -i README.md -s

# spellcheck, print words that may be in words set
bf_spellcheck_readme_sx: bf_play bf_spellcheck.dat
	./bf_play -f bf_spellcheck.dat -i README.md -sx

#########################
# Bit-Sliced Signatures #
#########################

bitslicedsig.o: bitslicedsig/src/bitslicedsig.c bitslicedsig/src/bitslicedsig.h murmurhash.o
	gcc -c bitslicedsig/src/bitslicedsig.c

test_bitslicedsig: bitslicedsig/test/test_bitslicedsig_main.c bitslicedsig/test/test_bitslicedsig_main.h bitslicedsig/test/test_bitslicedsig.c bitslicedsig/test/test_bitslicedsig.h bitslicedsig.o murmurhash.o
	gcc -Wpedantic -Wall -Wextra -o test_bitslicedsig bitslicedsig/test/test_bitslicedsig_main.c bitslicedsig/test/test_bitslicedsig.c bitslicedsig.o murmurhash.o

clean:
	rm *.o murmurhash.o test_* prog_* bf_*



