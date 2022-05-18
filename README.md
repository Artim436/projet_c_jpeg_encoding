# Notre encodeur JPEG à nous

Bienvenue sur la page d'accueil de _votre_ projet JPEG, un grand espace de liberté, sous le regard bienveillant de vos enseignants préférés.
Le sujet sera disponible dès le lundi 2 mai à l'adresse suivante : [https://formationc.pages.ensimag.fr/projet/jpeg/jpeg/](https://formationc.pages.ensimag.fr/projet/jpeg/jpeg/).


Vous êtes sur le README du projet C de l'équipe 28 composée de LASSALLE Cedric, AUGER Arthur et Lebeurrier Arthur.
Dans le répertoire principal vous trouverez tout ce qu'il faut pour encoder une image en format brut au format JPEG. Pour cela il suffit de compiler à l'aide de la commande make puis d'éxecuter la commande suivante ./ppm2jpeg images/nom_de_l_image.pgm depuis le répertoire team28.
Toutes les images données dans l'énoncé au format brut sont bien encodées dans un temps acceptable, mais nous avons également testé notre encodeur sur des images bruts tirées d'internet. 
Dans le répertoire modules, vous trouverez une autre version de notre projet, sans le downsampling mais avec les modules bitstream, huffman et jpeg_writer écrits par nos soins. Ces derniers fonctionnent pour toutes les images en noir et blanc (la preuve en est que dans cette version de notre projet nous avons supprimés les modules données en amont par les professeurs, ainsi que les .o correspondants aux modules que nous avons écrit).

Nous vous laissons apprécier le journal de bord que nous avons tenu durant ces deux dernières semaines et qui témoigne des difficultées que nous avons du surmonter afin de vous présenter la meilleure version de notre projet.



#### Journal de bord
##### Lundi 2 Mai :
Nous allons commencer par réaliser un encodeur simple pour pouvoir transformer l'image invader :
Auger s'occupe d'extraire les données de l'image PGM, créer les headers pour l'image JPEG et transformer les données RGB en YCbCr.
Cédric se charge du découpage en MCU, gestion des tables de Huffman, écrire bit à bit dans le flux. 
Lebeurrier se charge du DCT, Zigzag et quantification. 

##### A l'aubre du mardi 3 Mai:
Extraction donnée ppm et passage en argument fait.
Découpage MCU fait.
DCT et quantification fait.
On va tester les liens entre mcu et extraction

##### Mardi 3 Mai:
La conversion RGB to YCbCr faite et adaptée pour les images en couleurs.
L'écriture en jpeg est également faite
Le "zig-zag" est terminé.
Demain on écrit les tables de Huffman, l'objectif est de terminer invader demain.

##### Mercredi 4 Mai:
On a décidé de créer une structure commune à toutes les fonctions.
Nous sommes obligés de réadapter notre code en conséquence.
Nous avons également exploiter les tables de Huffman pour l'écriture en jpeg.

##### Jeudi 5 Mai :
On a passé la journée à réadapter notre code à la nouvelle structure.
Nous pouvons à présent terminer les codes restants pour finir invader.

##### Vendredi 6 Mai :
Nous avons corriger les dernières erreurs de conversions, nous avons également commencé l'encodage RLE ainsi que les tables de Huffman.

##### Samedi 7 Mai :
Journée consacrée à l'encodage RLE et à l'utilisation des tables de Huffman. Création des fonctions magnitudes et indices dans la magnitude.

##### Dimanche 8 Mai :
Travail sur le fichier write_jpeg_file afin d'avoir un rendu.

##### Lundi 9 Mai :
Invaders marche, mais gris nous indique qu'il y a un problème sur l'encoding des coefficients AC. Nous faisons également l'encoding de DC.

##### Mardi 10 Mai :
Gris marche assez tôt dans la matinée grâce à l'encoding des coefficients. Cependant complexite.pgm ne marche pas ce qui pousse Cedric à retravailler le decoupage d'une image en MCU pednant que Arthur A. travaille avec l'outil valgrind afin de nettoyer notre code et empêcher tout problème qui pourrait être lié à l'écriture sur une case mémoire déjà occupée. Arthur L. de son côté réalise une optimisation du DCT avec la méthode de stockage du produit de cosinus.
Le groupe est bloqué car aucune image générée par gimp en pgm (peu importe sa taille) n'est encodée par notre encodeur jpeg.

##### Mercredi 11 Mai :
Le groupe avait travaillé tard dans la soirée de la veille et avait réussi à débloquer bisou.pgm et complexite.pgm
Nous avons ainsi pu profiter de la soirée pour adapter notre code aux couleurs.
Nous avons eu un problème pendant toute l'après-midi sur les couleurs mais celui-ci sera réglé dans la soirée pour zig-zag.pgm.
Au vu des compétences de chacun, nous commençons à nous demander si il n'est pas préférable de se séparer les tâches et de commencer à coder les modules pendant que d'autres se chargent des dernières images.

##### Jeudi 12 Mai :
Toutes les images couleurs marchent enfin même biiig avec un temps d'éxecution de moins de 40 secondes. Le problème se situait au niveau du découpage des mcus.
Le reste de la journée a donc été consacré au downsampling et à la création des premiers modules.

##### Vendredi 13 Mai :
Avec l'optimisation du stockage des cosinus et du "zoning" pour dct, biiiiig prend moins de 10 secondes, de plus nous nous sommes attelés à continuer la création des modules jpegwriter, bitsream et huffman. De plus nous travaillons encore sur le downsampling qui prend plus de temps que prévu.

##### Lundi 16 Mai :
Bitsream est fini dans la journée, nous commençons à préparer un merge final de nos branch qui peut s'avérer compliqué. Jpegwriter semble marcher également. Pour huffman tout est fait à part le get_path.

##### Mardi 17 Mai :
Finalement le get_path à pris plus de temps que prévu, j'ai complétement changé ma manière de faire ce qui me simplifie le get_path mais qui m'oblige à créer des fonctions auxiliaires et à changer la structure de huff_table_M. Ce dernier point pose particulièrement problème dans la fusion du travail avec les modules.

##### Mercredi 18 Mai :
Dernier jour consacré au merge et à la finalisation de la création des modules. Il faut également supprimer les .o et .bla ainsi que ppm2jpeg afin de nettoyer la branch master



