#!/usr/local/bin/python
# -*- coding: utf-8 -*-

import random

alphabeta = [ ch for ch in range(256) ]
hash_map1 = [ 0 ] * 256
hash_map2 = [ 0 ] * 256
print(len(alphabeta))
for i in range(256):
    if len(alphabeta) is 0:
        break
    idx = random.randint(0, len(alphabeta)-1)
    ch = alphabeta[idx]
    hash_map1[i] = ch
    hash_map2[ch] = i
    alphabeta.remove(ch)

print("Hash map:")

for i in range(0, 256):
    print("%3d => %3d  ||  %3d => %3d" 
        % (i, hash_map1[i], hash_map1[i], hash_map2[hash_map1[i]]))

print("\n\nVerify:")
orignal = "{\"type\":3,\"act\":1,\"seed\":627812450}"
print("Orignal: "+orignal)
data = []
for o in orignal:
    data.append(hash_map1[ord(o)])
print("Data: ", end='')
print(data)
words = ""
for d in data:
    words += chr(hash_map2[d])
print("Words: " + words)

print("OK." if words==orignal else "Fail.", end="\n\n")


print("static const char chEncryptHash[256] = {", end='\n')
for i in range(15):
    for j in range(16):
        print("'\\x%02x', " % (hash_map1[i*16+j]), end='')
    print("\n", end='')

for j in range(15):
    print("'\\x%02x', " % (hash_map1[15*16+j]), end='')
print("'\\x%02x'};" % (hash_map1[255]), end='\n')

print("static const char chDecryptHash[256] = {", end='\n')
for i in range(15):
    for j in range(16):
        print("'\\x%02x', " % (hash_map2[i*16+j]), end='')
    print("\n", end='')

for j in range(15):
    print("'\\x%02x', " % (hash_map2[15*16+j]), end='')
print("'\\x%02x'};" % (hash_map2[255]), end='\n')
