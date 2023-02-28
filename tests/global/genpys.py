from os import listdir
import os
from os.path import isfile, join
os.system('python2 -m compileall Python_Files-main/*.py')
fichiers = [f for f in listdir('./Python_Files-main/') if (isfile(join('./Python_Files-main/', f)) and f.endswith('.pyc'))]
for fichier in fichiers:
    os.system('pyc-objdump disasm "Python_Files-main/'+fichier+'" > "Python_Files-main/' + fichier[:-1] + 's"')
