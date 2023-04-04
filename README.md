# Camoufler des données binaires dans le bruit ambiant

les zones uniformes tel que le ciel font apparaitre le bruit à une ordre plus faible lorsque l'on cherche à cacher de l'information dans les bits de poids faible d'une image, il serait donc intéressant de pouvoir altérer les pixels dans les zones non-uniformes afin de pouvoir augment l’ordre sans que l’altération de l’image soit
visible comme le permet cet algorithme.

Afin de différencier les pixels dans les zones uniformes des pixels des zones contrastés, on applique une fonction median() récupérant la valeur du pixels et de ses 8 voisins afin de ressortir la médiane parmi ces 9 valeurs.

On définit ensuite un seuil afin de faire la différence entre la médiane, si la différence est suffisamment grande, cela signifie qu’on considère le pixel comme dans une zone non uniforme et on va pouvoir altérer les bits de poids faible de ce pixel là.

Cependant, lors du déchiffrage, il va falloir différencier les
pixels altérés de ceux qui ne le sont pas, c’est pourquoi nous
allons devoir réserver le bits de poids faible à un témoin,
valant 0 s’il est d’origine ou s’il contient une information
caché.
On décale donc d’un bits les informations dissimulé et on
effectue un test avec les informations récupéré par le calcul de
la médiane afin de remplacer le dernier bit par un 1 ou un 0.
On part donc d’une
image à l’ordre 5
qui donne l’image
ci-dessous lorsque
l’on n’applique pas
le filtre médian.

![](https://github.com/AxelPatron/Steganography-Optimization/blob/main/unfiltered5.jpg)

Voici donc le
résultat obtenu avec
un ordre 5 selon le
seuil, ainsi que les
pixels altérés en
rouge.

![](https://github.com/AxelPatron/Steganography-Optimization/blob/main/filtered.jpg)
On remarque déjà avec un seuil=5 que le ciel n’est pas altéré
visuellement, ce qui rend déjà le changement de l’image
quasiment invisible, tandis qu’avec un seuil=10, on n’aperçoit
plus du tout de changement remarquable visuellement.

Il ne nous reste donc qu’à récupérer nos informations
dissimulé dans l’image en testant le bits de poids faibles de
chaque pixels et de récupérer les informations des pixels avec
un témoin.
