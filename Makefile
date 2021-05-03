##############
# murmurhash #
##############

murmurhash.o: deps/murmurhash/murmurhash.c deps/murmurhash/murmurhash.h
	gcc -c deps/murmurhash/murmurhash.c

# test

test_murmurhash: bloom/test/test_murmurhash.c murmurhash.o
	gcc -Wpedantic -Wall -Wextra -o test_murmurhash bloom/test/test_murmurhash.c murmurhash.o

################
# Bloom filter #
################

bloom.o: bloom/src/bloom.c bloom/src/bloom.h murmurhash.o
	gcc -c bloom/src/bloom.c

# test

test_bloom: bloom/test/test_bloom_main.c bloom/test/test_bloom_main.h bloom/test/test_bloom.c bloom/test/test_bloom.h bloom.o
	gcc -Wpedantic -Wall -Wextra -o test_bloom bloom/test/test_bloom_main.c bloom/test/test_bloom.c bloom.o murmurhash.o

run_test_bloom: test_bloom
	./test_bloom -v

# demo

bf_editor: bloom/demo/bf_editor_main.c bloom/demo/bf_editor_main.h bloom/demo/bf_editor.c bloom/demo/bf_editor.h bloom.o
	gcc -Wpedantic -Wall -Wextra -o bf_editor bloom/demo/bf_editor_main.c bloom/demo/bf_editor.c bloom.o murmurhash.o

bf_play: bloom/demo/bf_play_main.c bloom/demo/bf_play_main.h bloom/demo/bf_play.c bloom/demo/bf_play.h bloom.o
	gcc -Wpedantic -Wall -Wextra -o bf_play bloom/demo/bf_play_main.c bloom/demo/bf_play.c bloom.o murmurhash.o

bf_spellcheck.dat: bf_editor
	./bf_editor -o bf_spellcheck.dat -i /usr/share/dict/words -m 1963040 -k 13

bf_spellcheck_w_Gati.dat: bf_spellcheck.dat
	echo "Gati" | ./bf_editor -o bf_spellcheck_w_Gati.dat -f bf_spellcheck.dat

# spellcheck, should mark "Gati" as wrong and highlight it red
demo_bf_spellcheck: bf_play bf_spellcheck.dat
	echo "hi my name is Gati" | ./bf_play -f bf_spellcheck.dat -v

# spellcheck, should not mark "Gati" as wrong
demo_bf_spellcheck_w_Gati: bf_play bf_spellcheck_w_Gati.dat
	echo "hi my name is Gati" | ./bf_play -f bf_spellcheck_w_Gati.dat

# spellcheck, color definitely not in words set red
demo_bf_spellcheck_readme: bf_play bf_spellcheck.dat
	./bf_play -f bf_spellcheck.dat -i README.md

# spellcheck, color may be in words set blue
demo_bf_spellcheck_readme_x: bf_play bf_spellcheck.dat
	./bf_play -f bf_spellcheck.dat -i README.md -x

# spellcheck, print definitely not in words set
demo_bf_spellcheck_readme_s: bf_play bf_spellcheck.dat
	./bf_play -f bf_spellcheck.dat -i README.md -s

# spellcheck, print words that may be in words set
demo_bf_spellcheck_readme_sx: bf_play bf_spellcheck.dat
	./bf_play -f bf_spellcheck.dat -i README.md -sx

#########################
# Bit-Sliced Signatures #
#########################

queryres.o: bitslicedsig/src/queryres.c bitslicedsig/src/queryres.h
	gcc -c bitslicedsig/src/queryres.c

bitslicedsig.o: bitslicedsig/src/bitslicedsig.c bitslicedsig/src/bitslicedsig.h murmurhash.o
	gcc -c bitslicedsig/src/bitslicedsig.c

# test

test_bitslicedsig: bitslicedsig/test/test_bitslicedsig_main.c bitslicedsig/test/test_bitslicedsig_main.h bitslicedsig/test/test_bitslicedsig.c bitslicedsig/test/test_bitslicedsig.h bitslicedsig.o queryres.o murmurhash.o
	gcc -Wpedantic -Wall -Wextra -o test_bitslicedsig bitslicedsig/test/test_bitslicedsig_main.c bitslicedsig/test/test_bitslicedsig.c bitslicedsig.o queryres.o murmurhash.o

run_test_bitslicedsig: test_bitslicedsig
	./test_bitslicedsig -v

# xkcd keyword search demo

bss_editor: bitslicedsig/demo/bss_editor_main.c bitslicedsig/demo/bss_editor_main.h bitslicedsig/demo/bss_editor.c bitslicedsig/demo/bss_editor.h bitslicedsig.o queryres.o murmurhash.o
	gcc -Wpedantic -Wall -Wextra -o bss_editor bitslicedsig/demo/bss_editor_main.c bitslicedsig/demo/bss_editor.c bitslicedsig.o queryres.o murmurhash.o

bss_play: bitslicedsig/demo/bss_play_main.c bitslicedsig/demo/bss_play_main.h bitslicedsig/demo/bss_play.c bitslicedsig/demo/bss_play.h bitslicedsig.o queryres.o murmurhash.o
	gcc -Wpedantic -Wall -Wextra -o bss_play bitslicedsig/demo/bss_play_main.c bitslicedsig/demo/bss_play.c bitslicedsig.o queryres.o murmurhash.o

# get the corpus
start_index = 1
end_index = 40

bitslicedsig/demo/xkcd_corpus/xkcd_index.txt: bitslicedsig/demo/save_xkcd_text_docs.py
	mkdir -p bitslicedsig/demo/xkcd_corpus
	python3 bitslicedsig/demo/save_xkcd_text_docs.py --start_index $(start_index) --end_index $(end_index)

bss_xkcd.dat: bitslicedsig/demo/xkcd_corpus/xkcd_index.txt bss_editor
	./bss_editor -o bss_xkcd.dat -i bitslicedsig/demo/xkcd_corpus/xkcd_index.txt -m 400 -k 3 -d $(end_index) -v

# keyword search, should perform boolean AND query and check if any documents contain all the (exact) query words
demo_bss_xkcd_query: bss_play bss_xkcd.dat
	echo "outside" | ./bss_play -f bss_xkcd.dat -s "https://xkcd.com/%d"

# clean

clean:
	rm *.o murmurhash.o test_* bf_* demo_* bss_*



