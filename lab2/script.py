f = open('script_out.txt', 'w')
literals = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h']
for i in literals:
    for j in range(1,9):
        #f.write('pos([' + str(i) + ', ' + str(j) + ']).\n')
        f.write('pos([' + i + ', ' + str(j) + ']).\n')
f.close()
