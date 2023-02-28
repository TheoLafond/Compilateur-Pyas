from os import listdir
import os
from os.path import isfile, join

def prRed(skk): print("\033[91m {}\033[00m" .format(skk))


def prGreen(skk): print("\033[92m {}\033[00m" .format(skk))

fichiers = [f for f in listdir('./Python_Files-main/') if (isfile(join('./Python_Files-main/', f)) and f.endswith('.pyc'))]
ok = True
combien = 0;
for fichier in fichiers:
    print("Testing "+fichier+" ...")
    os.system('../../bin/pyas.exe ../../definitionLexeme.txt "Python_Files-main/'+fichier[:-1]+'s" ./binary.pyc')
    sourcefile = open('./Python_Files-main/'+fichier, "rb")
    snum=list(sourcefile.read())
    # print (snum[9:])
    sourcefile.close()
    comparefile = open('./binary.pyc', "rb")
    cnum=list(comparefile.read())
    # print ("coucou",cnum)
    comparefile.close()
    pok = True
    for i in range(9,min(len(snum),len(cnum))):

        if(snum[i]!=cnum[i]):
            prRed("ERROR")
            print(i,snum[i],cnum[i])
            pok = False
            break
    if(len(snum)!=len(cnum)):
        prRed("ERROR BINARYS DON'T HAVE THE SAME LENGTH "+str(len(snum))+","+str(len(cnum)))
        pok = False
    if (pok):
        prGreen(" PASSED")
        combien+=1
    else:
        ok = False
if(ok):
    prGreen("Every tests passed "+str(combien)+"/"+str(len(fichiers)))
else:
    prRed("Not every tests passed "+str(combien)+"/"+str(len(fichiers)))
