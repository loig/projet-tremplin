#!/bin/bash


rm -rf labs;
# Tableau de boolean [0000, ..., 1111]
D2B=({0..1}{0..1}{0..1}{0..1})
mkdir labs;
# Les 4 boucles qui suivent gerent chacunes une ligne du fragment de labyrinthe.
for i in `seq 0 15`;
do
	for j in `seq 0 15`;
	do
		for h in `seq 0 15`;
		do
			for f in `seq 0 15`;
			do
				# Création du dossier si nécessaire.
				mkdir -p labs/$i/$j/$h;
				# Création du nouveau fragment de labyrinthe.
				touch labs/$i/$j/$h/$f;
				# Saisie de chacune dans le fichier.
				echo ${D2B[$i]} >> labs/$i/$j/$h/$f;
				echo ${D2B[$j]} >> labs/$i/$j/$h/$f;
                                echo ${D2B[$h]} >> labs/$i/$j/$h/$f;
                                echo ${D2B[$f]} >> labs/$i/$j/$h/$f;
				# Le echo ci-dessous ne sert juste à ce que la personne derriere la machine, pendant l'éxecution du script, sache où l'on en est dans la création des fichiers.
				echo $i/$j/$h/$f;
			done

		done

	done

done
