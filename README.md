# README Projet informatique SEI SICOM 2021


### **Comment compiler :** <br><br>
   Faire la commande make dans la racine des dossiers


### **Comment executer le code :** <br><br>

- Executer le lexer : `./bin/lexer.exe config/pyas.conf <nomDuFichierDeCodeAssembleurPython ex : testAsm.txt>`<br>

- Executer le parseur et pycodblockeur : `./bin/py_codeblock_integration_test.exe config/pyas.conf <nomDuFichierDeCodeAssembleurPython ex : testAsm.txt>`<br>

- Executer la compilation : `./bin/pyas.exe config/pyas.conf <nomDuFichierDeCodeAssembleurPython ex : testAsm.txt> <nomDuFichierBinaireAcreer ex: binary.pyc`<br>
   Le fichier binaire généré sera binary.pyc .

### **Comment tester :**
   * *Tests d'intégration :*

   `cd tests/integration`<br>
   `python3 execute_tests.py runtest ../../bin/<prog>.exe ./<dossier associe a prog>`<br><br>
   prog testable : (prog  : dossier)
   - [x] regexp-read : 01_test_chargroup_parse_and_regexp
   - [x]    lexer  : 03_test_lexer
   - [x]    testlexlec  : 04_test_doclex_parse
   - [x]    parser_test  : 05_test_parser
   - [x]    py_codeblock_integration_test  : 06_test_py_codeblock_integration

   <br>



   * *Tests unitaires :*

   executer `./bin/unit/<prog>.exe`

   prog testable :
   - [x] test-lexem
   - [x]    test-list
   - [x]    test-py_codeblock
   - [x]    test-pyobj
   - [x]    test-queue
<br>



   * *Test global :*

   `cd tests/global`<br>
   Générer les pys :
   `python genpys.py`<br>
   Puis lancer les tests :
   `python test_global.py`<br><br>
<hr><br><br><br>

## Rappel sur les rendus
Les rendus de chaque incrément et le rendu final sont attendus sur Git.

C'est l'état du dépôt sur le serveur à la date du rendu qui fait foi.

Aucun rendu par mail !

Votre code doit fonctionner sur les machines Phelma ou sur la machine virtuelle Phelma 2020-21.



## Contenu attendu pour votre dépot Git

Dans tous les cas, votre dépôt Git devra contenir a minima :
* un fichier README.md à la racine (ce fichier, à mettre à jour), qui explique brièvement comment compiler, comment exécuter, comment tester
* un fichier Makefile qui permet de compiler avec la commande make
* votre code source, organisé en sous-répertoires
* tous vos programmes principaux de test et les informations indiquant tous les tests que vous avez réalisés
* les autres fichiers nécessaires au fonctionnement de votre projet - typiquement le fichier de description des lexèmes, etc.
* vos rapports (un par incrément), au format PDF

A l'inverse, un dépôt Git ne doit PAS contenir :
* les fichiers binaires objet .o et les binaires exécutables : ils sont régénérés à chaque fois !

Le fichier .gitignore fourni à la racine du dépôt permet déjà que tous les fichiers objet .o ou et tous les fichiers d'extension .exe soient ignorés par Git. Vous pouvez modifier / étendre ce fichier si besoin.

## Structure typique du dépôt

Nous conseillons de vous inspirer de l'arborescence suivante pour votre dépot git :

```bash
Racine du dépôt Git
  |_ .gitignore : le fichier <gitignore>
  |_ README.md : ce README (à mettre à jour !)
  |_ Makefile : votre fichier Makefile, qui doit permettre de tout compiler facilement (exécutable final, tests...)
  |_ include
     |_ tous les fichiers header .h, éventuellement organisés dans des sous-répertoires
  |_ src    
     |_ tous les fichiers source .c (sauf les programmes principaux)
  |_ tests
     |_  les fichiers source .c de tous les main de test
     |_  les données nécessaires aux tests
  |_ rapports
     |_ Les rapports de chaque incrément et le rapport final
```
