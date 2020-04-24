#!/bin/bash
 
# Pour toujours faire les tests à partir d'un disque identique à l'original
echo "Je copie le fichier DisqueVirtuel.dat.orig vers DisqueVirtuel.dat" 
cp DisqueVirtuel.dat.orig DisqueVirtuel.dat

echo
echo "--------------------------------------------------------------------"
echo "montrer le contenu du disque"
echo "--------------------------------------------------------------------"
./ufs ls /
./ufs ls /doc
./ufs ls /doc/tmp
./ufs ls /doc/tmp/subtmp
./ufs ls /rep
./ufs ls /Bonjour

echo
echo
echo "--------------------------------------------------------------------"
echo "Tester les cas ou ls est fait sur un repertoire non-existant ou un fichier ordinaire"
echo "--------------------------------------------------------------------"
./ufs ls /mauvais
./ufs ls /b.txt

echo
echo
echo "--------------------------------------------------------------------"
echo "Maintenant on verifie que les bons b.txt sont accedes"
echo "--------------------------------------------------------------------"
./ufs stat /doc/tmp/subtmp/b.txt 
./ufs stat /b.txt 

echo
echo
echo "--------------------------------------------------------------------"
echo "Tester la commande chmod"
echo "--------------------------------------------------------------------"
./ufs chmod 760 /b.txt
./ufs ls /
./ufs chmod 430 /b.txt
./ufs ls /
./ufs chmod 170 /b.txt
./ufs ls /
./ufs chmod 610 /Bonjour
./ufs ls /

echo
echo
echo "--------------------------------------------------------------------"
echo "Tester la commande hardlink"
echo "--------------------------------------------------------------------"
./ufs blockfree
./ufs hardlink /b.txt /hlnb.txt
./ufs hardlink /b.txt /hlnb.txt
./ufs blockfree
./ufs ls /

echo
echo
echo "--------------------------------------------------------------------"
echo "Tester la commande unlink"
echo "--------------------------------------------------------------------"
./ufs unlink /b.txt
./ufs ls /
./ufs blockfree
./ufs unlink /hlnb.txt
./ufs ls /
./ufs blockfree
./ufs unlink /b.txt
./ufs unlink /doc/tmp/b.txt 
./ufs unlink /doc/tmp/subtmp/b.txt 
./ufs ls /doc/tmp/subtmp
./ufs unlink /doc

echo
echo
echo "--------------------------------------------------------------------"
echo "Tester la commande rmdir"
echo "--------------------------------------------------------------------"
./ufs blockfree
./ufs rmdir /rep
./ufs ls /
./ufs blockfree
./ufs rmdir /doc
./ufs ls /
./ufs rmdir /doc/tmp

echo
echo
echo "--------------------------------------------------------------------"
echo "Tester la création d'un fichier vide"
echo "--------------------------------------------------------------------"
./ufs create /Doge.wow
./ufs ls /
./ufs create /doc/tmp/new.txt 
./ufs ls /
./ufs ls /doc/tmp

echo
echo
echo "--------------------------------------------------------------------"
echo "Tester la fonction rename sur fichier ordinaire"
echo "--------------------------------------------------------------------"
./ufs rename /Bonjour/LesAmis.txt /Bonjour/OncleG.txt
./ufs ls /Bonjour
./ufs rename /Bonjour/OncleG.txt /DansRoot.txt
./ufs ls /Bonjour
./ufs ls /

echo
echo
echo "--------------------------------------------------------------------"
echo "Tester la création d'un répertoire"
echo "--------------------------------------------------------------------"
./ufs blockfree
./ufs ls /Bonjour
./ufs mkdir /Bonjour/newdir
./ufs chmod 440 /Bonjour/newdir
./ufs blockfree
# On vérifie que le nombre de lien pour /Bonjour augmente de 1
./ufs ls /Bonjour
./ufs ls /

echo
echo
echo "--------------------------------------------------------------------"
echo "Tester la fonction rename sur répertoire"
echo "--------------------------------------------------------------------"
./ufs ls /Bonjour
./ufs ls /doc
./ufs rename /doc/tmp /Bonjour/tmpmv
# On vérifie que le nombre de lien pour /doc diminue de 1, et que pour /Bonjour il augmente de 1
./ufs ls /Bonjour/tmpmv
./ufs ls /doc
# On vérifie que le nombre de lien vers ce même répertoire n'augmente pas si on répète l'opération
./ufs rename /Bonjour/tmpmv /Bonjour/tmpmv2
./ufs rename /Bonjour/tmpmv2 /Bonjour/tmpmv3
./ufs ls /Bonjour/tmpmv3

