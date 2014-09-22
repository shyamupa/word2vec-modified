echo "No min-count!" # some wikititles appear less than 2 times, the vectors are going to be crappy, but atleast we will have some vectors

# use skipgram
time ./word2vec -train $1 -output $2 -cbow 0 -min-count 0 -size 200 -window 5 -negative 0 -hs 1 -sample 1e-3 -threads 16 -binary 1

# ./distance $2